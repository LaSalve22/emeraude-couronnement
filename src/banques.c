#include "global.h"
#include "banques.h"
#include "bourse.h"
#include "calendrier.h"
#include "commerces.h"
#include "event_data.h"
#include "immobilier.h"
#include "money.h"
#include "string_util.h"
#include "constants/characters.h"
#include "constants/flags.h"
#include "constants/vars.h"

// Émeraude Couronnement : banques (v0.37).
// Intérêts composés chaque jour ; chaque lundi, 5 % de la dette (500 ¥ minimum) sont prélevés sur l'argent du joueur.
// S'il ne peut pas payer : défaut, et chaque banque a sa sanction (pénalité, saisie d'une maison, saisie du liquide).

STATIC_ASSERT(sizeof(struct ImmoSave) + sizeof(struct BanqueSave) <= 0x34, BanqueSaveTropGrande);
STATIC_ASSERT(sizeof(struct BourseSave) + sizeof(struct SocieteSave) + 2 * sizeof(u32) <= 0x68, BanqueSave2TropGrande);

#define BANQUE (gSaveBlock1Ptr->banque)

struct InfoBanque
{
    const u8 *nom;
    u16 tauxPourDixMille;   // intérêts par jour
    const u8 *taux;
    const u8 *sanction;
};

static const struct InfoBanque sBanques[BANQUE_NB] =
{
    [BANQUE_DEVON] =
    {
        COMPOUND_STRING("Banque Devon"), 30, COMPOUND_STRING("0,3 %"),
        COMPOUND_STRING("Défaut : +10 %. Au 2e : interdit\lbancaire et une maison saisie."),
    },
    [BANQUE_FONCIER] =
    {
        COMPOUND_STRING("Crédit Foncier"), 50, COMPOUND_STRING("0,5 %"),
        COMPOUND_STRING("Défaut : ta maison la moins chère\lest saisie et vendue."),
    },
    [BANQUE_CASINO] =
    {
        COMPOUND_STRING("Prêteur du Casino"), 200, COMPOUND_STRING("2 %"),
        COMPOUND_STRING("Défaut : tout ton liquide, +20 %.\lAu 2e : interdit et maison saisie."),
    },
};

static u32 *Dette(u32 banque)
{
    if (banque == BANQUE_CASINO)
        return &BANQUE.detteCasino;
    return &gSaveBlock2Ptr->pokedex.detteBanque[banque];
}

static u32 Borner(u64 v)
{
    return v > 2000000000 ? 2000000000 : v;
}

static u32 NombreBadges(void)
{
    u32 i, n = 0;
    for (i = FLAG_BADGE01_GET; i <= FLAG_BADGE08_GET; i++)
        n += FlagGet(i) ? 1 : 0;
    return n;
}

u32 Banque_Dette(u32 banque)
{
    return banque < BANQUE_NB ? *Dette(banque) : 0;
}

u32 Banque_Plafond(u32 banque)
{
    switch (banque)
    {
    case BANQUE_DEVON:
        return 30000 + 40000 * NombreBadges() + (FlagGet(FLAG_SYS_GAME_CLEAR) ? 1000000 : 0);
    case BANQUE_FONCIER:
        return Borner(((u64)Immo_ValeurBiens(Calendrier_Aujourdhui()) + Commerce_Valeur()) * 60 / 100);
    case BANQUE_CASINO:
        return 200000 + 50000 * NombreBadges();
    }
    return 0;
}

static void Initialiser(u32 jour)
{
    memset(&BANQUE, 0, sizeof(BANQUE));
    gSaveBlock2Ptr->pokedex.detteBanque[0] = 0;
    gSaveBlock2Ptr->pokedex.detteBanque[1] = 0;
    BANQUE.magie = BANQUE_MAGIE;
    BANQUE.jour = jour;
}

static void AjouterPaye(u32 montant)
{
    u32 total = VarGet(VAR_BANQUE_PAYE_BAS) | (VarGet(VAR_BANQUE_PAYE_HAUT) << 16);
    total = Borner((u64)total + montant);
    VarSet(VAR_BANQUE_PAYE_BAS, total & 0xFFFF);
    VarSet(VAR_BANQUE_PAYE_HAUT, total >> 16);
}

// ---------------------------------------------------------------- défauts (correctifs de l'audit, lot 0 du cahier v0.42)
// VAR_BANQUE_DEFAUTS : bits 0-2 = défauts à annoncer (un par banque) ; bits 4-15 = nombre de défauts par banque (4 bits chacune),
// remis à zéro quand la dette de cette banque est soldée.

static u32 NbDefauts(u32 banque)
{
    return (VarGet(VAR_BANQUE_DEFAUTS) >> (4 + 4 * banque)) & 0xF;
}

static void ChangerNbDefauts(u32 banque, u32 n)
{
    u32 v = VarGet(VAR_BANQUE_DEFAUTS) & ~(0xF << (4 + 4 * banque));
    VarSet(VAR_BANQUE_DEFAUTS, v | ((n > 15 ? 15 : n) << (4 + 4 * banque)));
}

// Interdit bancaire : au moins BANQUE_DEFAUTS_INTERDIT défauts chez une banque encore créancière. Plus aucun crédit
// (banques et Bourse) tant que cette dette n'est pas soldée.
bool32 Banque_InterditBancaire(void)
{
    u32 b;
    for (b = 0; b < BANQUE_NB; b++)
    {
        if (*Dette(b) != 0 && NbDefauts(b) >= BANQUE_DEFAUTS_INTERDIT)
            return TRUE;
    }
    return FALSE;
}

// Une dette soldée efface les défauts de la banque.
static void VerifierSolde(u32 banque)
{
    if (*Dette(banque) == 0)
        ChangerNbDefauts(banque, 0);
}

// Prélève une échéance : liquide, puis trésorerie de la société, puis vente forcée des placements en Bourse (or d'abord).
// Rien n'est pris si le tout ne suffit pas. Renvoie TRUE si l'échéance est payée.
static bool32 Prelever(u32 montant)
{
    struct SocieteSave *soc = &gSaveBlock2Ptr->pokedex.societe;
    u32 tresorerie = soc->etat != 0 ? soc->tresorerie : 0, argent = GetMoney(&gSaveBlock1Ptr->money), pris, e;
    u64 dispo = (u64)argent + tresorerie + (u64)Bourse_ValeurPortefeuille() * (100 - BOURSE_FRAIS_POURCENT) / 100;

    if (dispo < montant)
        return FALSE;
    pris = montant < argent ? montant : argent;
    RemoveMoney(&gSaveBlock1Ptr->money, pris);
    montant -= pris;
    pris = montant < tresorerie ? montant : tresorerie;
    if (soc->etat != 0)
        soc->tresorerie -= pris;
    montant -= pris;
    for (e = BOURSE_NB_ACTIFS; montant > 0 && e-- > 0;) // l'or (dernier actif) en premier
    {
        u32 cours = Bourse_Cours(e), net, parts;
        if (cours == 0 || Bourse_Parts(e) == 0)
            continue;
        parts = ((u64)montant * 100 + (u64)cours * 97 - 1) / ((u64)cours * 97); // arrondi au-dessus (frais 3 % au plus)
        Bourse_Vendre(e, parts);
        net = GetMoney(&gSaveBlock1Ptr->money);
        pris = montant < net ? montant : net;
        RemoveMoney(&gSaveBlock1Ptr->money, pris);
        montant -= pris;
    }
    return montant == 0;
}

// Saisie de la maison la moins chère : vendue 20 % sous le prix du marché, le produit rembourse la dette de la banque.
// Renvoie FALSE s'il n'y a plus rien à saisir.
static bool32 SaisirMaison(u32 banque, u32 jour)
{
    u32 i, choisi = IMMO_NB_BIENS, prix = 0, produit, *dette = Dette(banque);

    for (i = 0; i < IMMO_NB_BIENS; i++)
    {
        if (Immo_Possede(i) && (choisi == IMMO_NB_BIENS || Immo_Prix(i, jour) < prix))
        {
            choisi = i;
            prix = Immo_Prix(i, jour);
        }
    }
    if (choisi == IMMO_NB_BIENS)
        return FALSE;
    gSaveBlock1Ptr->immo.possede &= ~(1u << choisi);
    gSaveBlock1Ptr->immo.squat &= ~(1u << choisi);
    gSaveBlock1Ptr->immo.prixAchat[choisi] = 0;
    produit = (u64)prix * 80 / 100;
    if (produit >= *dette)
    {
        AddMoney(&gSaveBlock1Ptr->money, produit - *dette);
        *dette = 0;
    }
    else
    {
        *dette -= produit;
    }
    return TRUE;
}

// Défaut : sanction propre à la banque, puis sanctions graduelles. 2e défaut : interdit bancaire et une maison saisie ;
// 4e défaut et suivants : huissier, les maisons sont saisies jusqu'à solder la dette.
static void Defaut(u32 banque, u32 jour)
{
    u32 *dette = Dette(banque), argent, n = NbDefauts(banque) + 1;

    VarSet(VAR_BANQUE_DEFAUTS, VarGet(VAR_BANQUE_DEFAUTS) | (1 << banque));
    ChangerNbDefauts(banque, n);
    switch (banque)
    {
    case BANQUE_DEVON:
        *dette = Borner((u64)*dette * 110 / 100);
        break;
    case BANQUE_FONCIER:
        if (!SaisirMaison(banque, jour))
            *dette = Borner((u64)*dette * 110 / 100); // rien à saisir : pénalité
        break;
    case BANQUE_CASINO:
        argent = GetMoney(&gSaveBlock1Ptr->money);
        if (argent > *dette)
            argent = *dette;
        RemoveMoney(&gSaveBlock1Ptr->money, argent);
        *dette -= argent;
        AjouterPaye(argent);
        *dette = Borner((u64)*dette * 120 / 100);
        break;
    }
    if (n >= BANQUE_DEFAUTS_HUISSIER)
    {
        while (*dette != 0 && SaisirMaison(banque, jour))
            ;
    }
    else if (n >= BANQUE_DEFAUTS_INTERDIT && banque != BANQUE_FONCIER)
    {
        SaisirMaison(banque, jour);
    }
    VerifierSolde(banque);
}

void Banque_PasserJour(u32 jour)
{
    u32 b;

    for (b = 0; b < BANQUE_NB; b++)
    {
        u32 *dette = Dette(b);
        if (*dette == 0)
            continue;
        *dette = Borner((u64)*dette + ((u64)*dette * sBanques[b].tauxPourDixMille + 9999) / 10000);
        if (Calendrier_JourSemaine(jour) == WEEKDAY_MON)
        {
            u32 echeance = (u64)*dette * BANQUE_ECHEANCE_POURCENT / 100;
            if (echeance < BANQUE_ECHEANCE_MIN)
                echeance = *dette < BANQUE_ECHEANCE_MIN ? *dette : BANQUE_ECHEANCE_MIN;
            if (Prelever(echeance)) // liquide, trésorerie, puis placements
            {
                *dette -= echeance;
                AjouterPaye(echeance);
                VerifierSolde(b);
            }
            else
            {
                Defaut(b, jour);
            }
        }
    }
}

void Banque_MettreAJourJusqua(u32 aujourdhui)
{
    if (BANQUE.magie != BANQUE_MAGIE)
        Initialiser(aujourdhui);
    if (aujourdhui < BANQUE.jour) // horloge reculée : on ne rejoue jamais un jour déjà traité
        aujourdhui = BANQUE.jour;
    if (aujourdhui - BANQUE.jour > BANQUE_RATTRAPAGE_MAX)
        BANQUE.jour = aujourdhui - BANQUE_RATTRAPAGE_MAX;
    while (BANQUE.jour < aujourdhui)
    {
        BANQUE.jour++;
        Banque_PasserJour(BANQUE.jour);
    }
}

u32 Banque_Emprunter(u32 banque, u32 montant)
{
    u32 *dette, plafond, place;

    if (banque >= BANQUE_NB || Banque_InterditBancaire())
        return 0;
    dette = Dette(banque);
    plafond = Banque_Plafond(banque);
    place = MAX_MONEY - GetMoney(&gSaveBlock1Ptr->money);
    if (*dette >= plafond)
        return 0;
    if (montant > plafond - *dette)
        montant = plafond - *dette;
    if (montant > place)
        montant = place;
    *dette += montant;
    AddMoney(&gSaveBlock1Ptr->money, montant);
    return montant;
}

u32 Banque_Rembourser(u32 banque)
{
    u32 *dette, montant;

    if (banque >= BANQUE_NB)
        return 0;
    dette = Dette(banque);
    montant = GetMoney(&gSaveBlock1Ptr->money);
    if (montant > *dette)
        montant = *dette;
    RemoveMoney(&gSaveBlock1Ptr->money, montant);
    *dette -= montant;
    VerifierSolde(banque);
    return montant;
}

// ---------------------------------------------------------------- textes et specials

static u8 *AjouterNombre(u8 *fin, u32 n)
{
    u32 chiffres = 1;
    u64 p = 10;
    while (n >= p && chiffres < 10)
    {
        chiffres++;
        p *= 10;
    }
    return ConvertIntToDecimalStringN(fin, n, STR_CONV_MODE_LEFT_ALIGN, chiffres);
}

static const u8 sText_Yens[] = _(" ¥");

static u8 *AjouterYens(u8 *fin, u32 n)
{
    fin = AjouterNombre(fin, n);
    return StringCopy(fin, sText_Yens);
}

static const u8 sText_Rapport1[] = _("Échéances prélevées depuis ta\ndernière visite : ");
static const u8 sText_Rapport2[] = _(".");
static const u8 sText_RapportDefaut[] = _("\pDéfaut de paiement chez :\n");
static const u8 sText_Virgule[] = _(",\l");
static const u8 sText_RapportFin[] = _(".\pRien à prendre lundi, ni en poche,\nni en caisse, ni en Bourse.\pLa sanction est tombée.");
static const u8 sText_Interdit[] = _("\pTu es interdit bancaire : plus\naucun crédit tant que tu n'as pas\lsoldé ta dette.");

// Met à jour. VAR_RESULT = TRUE s'il y a un rapport (gStringVar4).
void Banque_Ouvrir(void)
{
    u32 paye, defauts, b;
    bool32 premier = TRUE;
    u8 *fin;

    Banque_MettreAJourJusqua(Calendrier_Aujourdhui());
    paye = VarGet(VAR_BANQUE_PAYE_BAS) | (VarGet(VAR_BANQUE_PAYE_HAUT) << 16);
    defauts = VarGet(VAR_BANQUE_DEFAUTS) & 0xF;
    gSpecialVar_Result = FALSE;
    if (paye == 0 && defauts == 0)
        return;
    fin = StringCopy(gStringVar4, sText_Rapport1);
    fin = AjouterYens(fin, paye);
    fin = StringCopy(fin, sText_Rapport2);
    if (defauts)
    {
        fin = StringCopy(fin, sText_RapportDefaut);
        for (b = 0; b < BANQUE_NB; b++)
        {
            if (!(defauts & (1 << b)))
                continue;
            if (!premier)
                fin = StringCopy(fin, sText_Virgule);
            fin = StringCopy(fin, sBanques[b].nom);
            premier = FALSE;
        }
        StringCopy(fin, sText_RapportFin);
    }
    VarSet(VAR_BANQUE_PAYE_BAS, 0);
    VarSet(VAR_BANQUE_PAYE_HAUT, 0);
    VarSet(VAR_BANQUE_DEFAUTS, VarGet(VAR_BANQUE_DEFAUTS) & ~0xF); // les compteurs de défauts restent
    if (Banque_InterditBancaire())
        StringAppend(gStringVar4, sText_Interdit);
    gSpecialVar_Result = TRUE;
}

static const u8 sText_Resume1[] = _("Devon : ");
static const u8 sText_Resume2[] = _("\nFoncier : ");
static const u8 sText_Resume3[] = _("\pCasino : ");
static const u8 sText_Resume4[] = _(".\nÉchéance : 5 % chaque lundi,\lprise aussi en caisse et en Bourse.");

void Banque_Resume(void)
{
    u8 *fin;

    Banque_MettreAJourJusqua(Calendrier_Aujourdhui());
    fin = StringCopy(gStringVar4, sText_Resume1);
    fin = AjouterYens(fin, Banque_Dette(BANQUE_DEVON));
    fin = StringCopy(fin, sText_Resume2);
    fin = AjouterYens(fin, Banque_Dette(BANQUE_FONCIER));
    fin = StringCopy(fin, sText_Resume3);
    fin = AjouterYens(fin, Banque_Dette(BANQUE_CASINO));
    StringCopy(fin, sText_Resume4);
}

static const u8 sText_Fiche1[] = _(" : ");
static const u8 sText_Fiche2[] = _(" par jour.\nDette : ");
static const u8 sText_Fiche3[] = _("\pPlafond : ");
static const u8 sText_Fiche4[] = _(".\n");
static const u8 sText_Fiche5[] = _("\pOn fait quoi ?");

// VAR_0x8004 : banque.
void Banque_Fiche(void)
{
    u32 b = gSpecialVar_0x8004 < BANQUE_NB ? gSpecialVar_0x8004 : 0;
    u8 *fin;

    fin = StringCopy(gStringVar4, sBanques[b].nom);
    fin = StringCopy(fin, sText_Fiche1);
    fin = StringCopy(fin, sBanques[b].taux);
    fin = StringCopy(fin, sText_Fiche2);
    fin = AjouterYens(fin, Banque_Dette(b));
    fin = StringCopy(fin, sText_Fiche3);
    fin = AjouterYens(fin, Banque_Plafond(b));
    fin = StringCopy(fin, sText_Fiche4);
    fin = StringCopy(fin, sBanques[b].sanction);
    StringCopy(fin, sText_Fiche5);
}

static const u32 sMontants[] = {10000, 100000, 1000000};
static const u8 sText_Prete[] = _("Accordé : ");
static const u8 sText_PreteFin[] = _(".\nLes intérêts courent dès demain.");
static const u8 sText_PreteRien[] = _("Plafond atteint, portefeuille plein\nou interdit bancaire. Prêt refusé.");
static const u8 sText_Rembourse[] = _("Remboursé : ");
static const u8 sText_RembourseRien[] = _("Rien à rembourser, ou pas un sou\nen poche.");

// VAR_0x8004 : banque, VAR_0x8005 : 0 = 10 000, 1 = 100 000, 2 = 1 000 000.
void Banque_ScriptEmprunter(void)
{
    u32 m = Banque_Emprunter(gSpecialVar_0x8004, sMontants[gSpecialVar_0x8005 < ARRAY_COUNT(sMontants) ? gSpecialVar_0x8005 : 0]);
    u8 *fin;

    gSpecialVar_Result = m != 0;
    if (m == 0)
    {
        StringCopy(gStringVar4, sText_PreteRien);
        return;
    }
    fin = StringCopy(gStringVar4, sText_Prete);
    fin = AjouterYens(fin, m);
    StringCopy(fin, sText_PreteFin);
}

void Banque_ScriptRembourser(void)
{
    u32 m = Banque_Rembourser(gSpecialVar_0x8004);
    u8 *fin;

    gSpecialVar_Result = m != 0;
    if (m == 0)
    {
        StringCopy(gStringVar4, sText_RembourseRien);
        return;
    }
    fin = StringCopy(gStringVar4, sText_Rembourse);
    fin = AjouterYens(fin, m);
    StringCopy(fin, sText_Rapport2);
}
