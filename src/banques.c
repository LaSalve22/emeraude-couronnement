#include "global.h"
#include "banques.h"
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
        COMPOUND_STRING("Défaut : +10 % de pénalité."),
    },
    [BANQUE_FONCIER] =
    {
        COMPOUND_STRING("Crédit Foncier"), 50, COMPOUND_STRING("0,5 %"),
        COMPOUND_STRING("Défaut : ta maison la moins chère\lest saisie et vendue."),
    },
    [BANQUE_CASINO] =
    {
        COMPOUND_STRING("Prêteur du Casino"), 200, COMPOUND_STRING("2 %"),
        COMPOUND_STRING("Défaut : on te prend tout ton\lliquide, plus 20 % de pénalité."),
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

// Saisie de la maison la moins chère : vendue 20 % sous le prix du marché, le produit rembourse la dette.
static void SaisirMaison(u32 jour)
{
    u32 i, choisi = IMMO_NB_BIENS, prix = 0, produit, *dette = Dette(BANQUE_FONCIER);

    for (i = 0; i < IMMO_NB_BIENS; i++)
    {
        if (Immo_Possede(i) && (choisi == IMMO_NB_BIENS || Immo_Prix(i, jour) < prix))
        {
            choisi = i;
            prix = Immo_Prix(i, jour);
        }
    }
    if (choisi == IMMO_NB_BIENS)
    {
        *dette = Borner((u64)*dette * 110 / 100); // rien à saisir : pénalité
        return;
    }
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
}

static void Defaut(u32 banque, u32 jour)
{
    u32 *dette = Dette(banque), argent;

    VarSet(VAR_BANQUE_DEFAUTS, VarGet(VAR_BANQUE_DEFAUTS) | (1 << banque));
    switch (banque)
    {
    case BANQUE_DEVON:
        *dette = Borner((u64)*dette * 110 / 100);
        break;
    case BANQUE_FONCIER:
        SaisirMaison(jour);
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
            if (IsEnoughMoney(&gSaveBlock1Ptr->money, echeance))
            {
                RemoveMoney(&gSaveBlock1Ptr->money, echeance);
                *dette -= echeance;
                AjouterPaye(echeance);
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
    if (aujourdhui < BANQUE.jour)
        BANQUE.jour = aujourdhui;
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

    if (banque >= BANQUE_NB)
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
static const u8 sText_RapportFin[] = _(".\pTu n'avais pas de quoi payer lundi.\nLa sanction est tombée.");

// Met à jour. VAR_RESULT = TRUE s'il y a un rapport (gStringVar4).
void Banque_Ouvrir(void)
{
    u32 paye, defauts, b;
    bool32 premier = TRUE;
    u8 *fin;

    Banque_MettreAJourJusqua(Calendrier_Aujourdhui());
    paye = VarGet(VAR_BANQUE_PAYE_BAS) | (VarGet(VAR_BANQUE_PAYE_HAUT) << 16);
    defauts = VarGet(VAR_BANQUE_DEFAUTS);
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
    VarSet(VAR_BANQUE_DEFAUTS, 0);
    gSpecialVar_Result = TRUE;
}

static const u8 sText_Resume1[] = _("Devon : ");
static const u8 sText_Resume2[] = _("\nFoncier : ");
static const u8 sText_Resume3[] = _("\pCasino : ");
static const u8 sText_Resume4[] = _(".\nÉchéance : 5 % chaque lundi.");

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
static const u8 sText_PreteRien[] = _("Plafond atteint, ou ton portefeuille\nest plein. Prêt refusé.");
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
