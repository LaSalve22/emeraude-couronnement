#include "global.h"
#include "immobilier.h"
#include "event_data.h"
#include "money.h"
#include "rtc.h"
#include "string_util.h"
#include "constants/characters.h"
#include "constants/flags.h"
#include "constants/vars.h"

// Émeraude Couronnement : immobilier (v0.34).
// 12 maisons, une par ville. Prix = prix de base x indice immobilier (bouge chaque jour, krachs rares) x écart local de ±10 %
// qui change chaque semaine ; un bien par semaine est soldé de 25 %. Achat : 8 % de notaire ; revente : 5 % d'agence.
// Chaque bien loué rapporte un loyer par jour, sauf imprévus : impayé, travaux à payer, ou squatteur de la Team Aqua
// (tant qu'elle sévit) qui bloque le loyer jusqu'à ce qu'on l'expulse en combat. Hasard déterministe (jour + n° de Dresseur).

#define IMMO (gSaveBlock1Ptr->immo)

STATIC_ASSERT(sizeof(struct ImmoSave) <= 0x34, ImmoSaveTropGrande);

struct Bien
{
    const u8 *nom;
    u32 prix;        // prix de base
    u16 loyer;       // loyer de base par jour
};

static const struct Bien sBiens[IMMO_NB_BIENS] =
{
    {COMPOUND_STRING("Maison de Rosyères"),     40000,  220},
    {COMPOUND_STRING("Maison de Clémenti"),     60000,  330},
    {COMPOUND_STRING("Appart de Mérouville"),   90000,  500},
    {COMPOUND_STRING("Cabane de Myokara"),      35000,  200},
    {COMPOUND_STRING("Maison de Poivressel"),  120000,  650},
    {COMPOUND_STRING("Studio de Lavandia"),    110000,  620},
    {COMPOUND_STRING("Chalet de Vergazon"),     80000,  430},
    {COMPOUND_STRING("Maison de Vermilava"),   130000,  720},
    {COMPOUND_STRING("Cabane de Cimetronelle"),100000,  540},
    {COMPOUND_STRING("Villa de Nénucrique"),   300000, 1600},
    {COMPOUND_STRING("Maison d'Algatia"),      180000,  980},
    {COMPOUND_STRING("Grotte d'Atalanopolis"), 250000, 1350},
};

EWRAM_DATA u8 gImmoLignes[IMMO_NB_BIENS][32] = {0};

// ---------------------------------------------------------------- hasard déterministe

static u32 Melanger(u32 x)
{
    x ^= x >> 16;
    x *= 0x7FEB352D;
    x ^= x >> 15;
    x *= 0x846CA68B;
    x ^= x >> 16;
    return x;
}

static u32 Alea(u32 jour, u32 bien, u32 tirage)
{
    const u8 *id = gSaveBlock2Ptr->playerTrainerId;
    u32 graine = id[0] | (id[1] << 8) | (id[2] << 16) | (id[3] << 24);
    return Melanger((jour * 0x85EBCA6Bu) ^ graine ^ (bien << 10) ^ (tirage << 22) ^ 0x1440);
}

// ---------------------------------------------------------------- moteur

static bool32 TeamAquaActive(void)
{
    return !FlagGet(FLAG_SOOTOPOLIS_ARCHIE_MAXIE_LEAVE);
}

void Immo_Initialiser(u32 jour)
{
    memset(&IMMO, 0, sizeof(IMMO));
    IMMO.magie = IMMO_MAGIE;
    IMMO.indice = 1000;
    IMMO.jour = jour;
}

bool32 Immo_Possede(u32 bien)
{
    return bien < IMMO_NB_BIENS && (IMMO.possede & (1u << bien));
}

u32 Immo_BonneAffaire(u32 jour)
{
    return Alea(jour / 7, 99, 0) % IMMO_NB_BIENS;
}

u32 Immo_Prix(u32 bien, u32 jour)
{
    u64 prix;
    s32 local;

    if (bien >= IMMO_NB_BIENS)
        return 0;
    local = (s32)(Alea(jour / 7, bien, 1) % 201) - 100; // ±10 % chaque semaine
    prix = (u64)sBiens[bien].prix * IMMO.indice / 1000;
    prix = prix * (1000 + local) / 1000;
    if (Immo_BonneAffaire(jour) == bien)
        prix = prix * (100 - IMMO_BONNE_AFFAIRE) / 100;
    return prix;
}

u32 Immo_LoyerJour(u32 bien)
{
    if (bien >= IMMO_NB_BIENS)
        return 0;
    return (u64)sBiens[bien].loyer * IMMO.indice / 1000;
}

// Avec une société (v0.35), les loyers vont dans sa trésorerie et les travaux y sont pris en premier.
static void Payer(u32 montant)
{
    u32 argent;
    if (gSaveBlock2Ptr->pokedex.societe.etat != 0)
    {
        u32 tresorerie = gSaveBlock2Ptr->pokedex.societe.tresorerie;
        u32 pris = montant < tresorerie ? montant : tresorerie;
        gSaveBlock2Ptr->pokedex.societe.tresorerie -= pris;
        montant -= pris;
    }
    argent = GetMoney(&gSaveBlock1Ptr->money);
    RemoveMoney(&gSaveBlock1Ptr->money, montant < argent ? montant : argent);
}

static void Encaisser(u32 montant)
{
    if (gSaveBlock2Ptr->pokedex.societe.etat != 0)
    {
        u32 *tresorerie = &gSaveBlock2Ptr->pokedex.societe.tresorerie;
        *tresorerie = (u64)*tresorerie + montant > 99999999 ? 99999999 : *tresorerie + montant;
    }
    else
    {
        AddMoney(&gSaveBlock1Ptr->money, montant);
    }
}

u32 Immo_ValeurBiens(u32 jour)
{
    u32 i, valeur = 0;
    for (i = 0; i < IMMO_NB_BIENS; i++)
    {
        if (Immo_Possede(i))
            valeur += Immo_Prix(i, jour);
    }
    return valeur;
}

void Immo_PasserJour(u32 jour)
{
    s32 variation;
    u32 i;

    // Indice : +0,1 % par jour en moyenne, ±0,8 %, krach immobilier 2 fois sur 1000 (-15 à -30 %), creux pendant la crise.
    variation = 1 + (s32)(Alea(jour, 50, 0) % 17) - 8;
    if (FlagGet(FLAG_KYOGRE_ESCAPED_SEAFLOOR_CAVERN) && !FlagGet(FLAG_SOOTOPOLIS_ARCHIE_MAXIE_LEAVE))
        variation -= 3;
    if (Alea(jour, 50, 1) % 1000 < 2)
        variation -= 150 + (s32)(Alea(jour, 50, 2) % 151);
    IMMO.indice = (u32)((s32)IMMO.indice * (1000 + variation) / 1000);
    if (IMMO.indice < IMMO_INDICE_MIN)
        IMMO.indice = IMMO_INDICE_MIN;
    if (IMMO.indice > IMMO_INDICE_MAX)
        IMMO.indice = IMMO_INDICE_MAX;

    for (i = 0; i < IMMO_NB_BIENS; i++)
    {
        u32 de;

        if (!Immo_Possede(i) || (IMMO.squat & (1u << i)))
            continue;
        de = Alea(jour, i, 2) % 1000;
        if (TeamAquaActive() && de < IMMO_SQUAT_POURMILLE)
        {
            IMMO.squat |= 1u << i;
            if (IMMO.nouveauxSquats < 255)
                IMMO.nouveauxSquats++;
        }
        else if (de < IMMO_SQUAT_POURMILLE + IMMO_IMPAYE_POURMILLE)
        {
            if (IMMO.impayes < 255)
                IMMO.impayes++;
        }
        else if (de < IMMO_SQUAT_POURMILLE + IMMO_IMPAYE_POURMILLE + IMMO_TRAVAUX_POURMILLE)
        {
            u32 cout = sBiens[i].prix * IMMO_TRAVAUX_POURCENT / 100;
            Payer(cout);
            IMMO.travaux += cout;
        }
        else
        {
            u32 loyer = Immo_LoyerJour(i);
            Encaisser(loyer);
            IMMO.loyers += loyer;
        }
    }
}

void Immo_MettreAJourJusqua(u32 aujourdhui)
{
    if (IMMO.magie != IMMO_MAGIE)
        Immo_Initialiser(aujourdhui);
    if (aujourdhui < IMMO.jour)
        IMMO.jour = aujourdhui;
    if (aujourdhui - IMMO.jour > IMMO_RATTRAPAGE_MAX)
        IMMO.jour = aujourdhui - IMMO_RATTRAPAGE_MAX;
    while (IMMO.jour < aujourdhui)
    {
        IMMO.jour++;
        Immo_PasserJour(IMMO.jour);
    }
}

static u32 AvecNotaire(u32 prix)
{
    return (u64)prix * (100 + IMMO_NOTAIRE_POURCENT) / 100;
}

static u32 ApresAgence(u32 prix)
{
    return (u64)prix * (100 - IMMO_AGENCE_POURCENT) / 100;
}

bool32 Immo_Acheter(u32 bien, u32 jour)
{
    u32 prix;

    if (bien >= IMMO_NB_BIENS || Immo_Possede(bien))
        return FALSE;
    prix = Immo_Prix(bien, jour);
    if (!IsEnoughMoney(&gSaveBlock1Ptr->money, AvecNotaire(prix)))
        return FALSE;
    RemoveMoney(&gSaveBlock1Ptr->money, AvecNotaire(prix));
    IMMO.possede |= 1u << bien;
    IMMO.squat &= ~(1u << bien);
    IMMO.prixAchat[bien] = (prix + 50) / 100 > 0xFFFF ? 0xFFFF : (prix + 50) / 100;
    return TRUE;
}

bool32 Immo_Vendre(u32 bien, u32 jour)
{
    u32 net;

    if (!Immo_Possede(bien))
        return FALSE;
    net = ApresAgence(Immo_Prix(bien, jour));
    if (GetMoney(&gSaveBlock1Ptr->money) + (u64)net > MAX_MONEY)
        return FALSE;
    AddMoney(&gSaveBlock1Ptr->money, net);
    IMMO.possede &= ~(1u << bien);
    IMMO.squat &= ~(1u << bien);
    IMMO.prixAchat[bien] = 0;
    return TRUE;
}

void Immo_Expulser(u32 bien)
{
    if (bien < IMMO_NB_BIENS)
        IMMO.squat &= ~(1u << bien);
}

// ---------------------------------------------------------------- textes

static u32 Aujourdhui(void)
{
    return RtcGetLocalDayCount();
}

static u8 *AjouterNombre(u8 *fin, u32 n)
{
    u32 chiffres = 1, p = 10;
    while (n >= p && chiffres < 9)
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

static const u8 sText_Rapport1[] = _("Rapport de gestion. Loyers : ");
static const u8 sText_Rapport2[] = _("\nTravaux : ");
static const u8 sText_Rapport3[] = _(". Impayés : ");
static const u8 sText_Rapport4[] = _(".");
static const u8 sText_RapportSquat[] = _("\pAlerte : un squatteur de la Team\nAqua occupe un de tes biens !\lVa le déloger, sinon pas de loyer.");

// Met à jour le marché. VAR_RESULT = TRUE s'il y a un rapport de gestion à lire (dans gStringVar4).
void Immo_Ouvrir(void)
{
    u8 *fin;

    Immo_MettreAJourJusqua(Aujourdhui());
    gSpecialVar_Result = FALSE;
    if (IMMO.loyers == 0 && IMMO.travaux == 0 && IMMO.impayes == 0 && IMMO.nouveauxSquats == 0)
        return;
    fin = StringCopy(gStringVar4, sText_Rapport1);
    fin = AjouterYens(fin, IMMO.loyers);
    fin = StringCopy(fin, sText_Rapport2);
    fin = AjouterYens(fin, IMMO.travaux);
    fin = StringCopy(fin, sText_Rapport3);
    fin = AjouterNombre(fin, IMMO.impayes);
    fin = StringCopy(fin, sText_Rapport4);
    if (IMMO.nouveauxSquats)
        StringCopy(fin, sText_RapportSquat);
    IMMO.loyers = 0;
    IMMO.travaux = 0;
    IMMO.impayes = 0;
    IMMO.nouveauxSquats = 0;
    gSpecialVar_Result = TRUE;
}

static const u8 sText_Resume1[] = _("Tes biens : ");
static const u8 sText_Resume2[] = _(". Valeur : ");
static const u8 sText_Resume3[] = _("\nLoyers : environ ");
static const u8 sText_Resume4[] = _(" par jour.\pIndice immobilier : ");
static const u8 sText_Resume5[] = _(" %.\nPromo -25 % : ");
static const u8 sText_Resume6[] = _(".");

void Immo_Resume(void)
{
    u32 i, n = 0, valeur = 0, loyers = 0, jour = Aujourdhui();
    u8 *fin;

    for (i = 0; i < IMMO_NB_BIENS; i++)
    {
        if (!Immo_Possede(i))
            continue;
        n++;
        valeur += Immo_Prix(i, jour);
        if (!(IMMO.squat & (1u << i)))
            loyers += Immo_LoyerJour(i);
    }
    fin = StringCopy(gStringVar4, sText_Resume1);
    fin = AjouterNombre(fin, n);
    fin = StringCopy(fin, sText_Resume2);
    fin = AjouterYens(fin, valeur);
    fin = StringCopy(fin, sText_Resume3);
    fin = AjouterYens(fin, loyers);
    fin = StringCopy(fin, sText_Resume4);
    fin = AjouterNombre(fin, IMMO.indice / 10);
    fin = StringCopy(fin, sText_Resume5);
    fin = StringCopy(fin, sBiens[Immo_BonneAffaire(jour)].nom);
    StringCopy(fin, sText_Resume6);
}

// Lignes du catalogue : « Maison de Rosyères 40k » ; + = à toi, ! = squatté, % = bonne affaire de la semaine.
void Immo_PreparerListe(void)
{
    u32 i, jour = Aujourdhui();

    for (i = 0; i < IMMO_NB_BIENS; i++)
    {
        u8 *fin = gImmoLignes[i];
        if (IMMO.squat & (1u << i))
            *fin++ = CHAR_EXCL_MARK;
        else if (Immo_Possede(i))
            *fin++ = CHAR_PLUS;
        else if (Immo_BonneAffaire(jour) == i)
            *fin++ = CHAR_PERCENT;
        fin = StringCopy(fin, sBiens[i].nom);
        *fin++ = CHAR_SPACE;
        fin = AjouterNombre(fin, Immo_Prix(i, jour) / 1000);
        *fin++ = CHAR_k;
        *fin = EOS;
    }
}

static const u8 sText_Libre1[] = _("\nPrix : ");
static const u8 sText_Libre2[] = _(" + 8 % de notaire.\pLoyer : environ ");
static const u8 sText_Libre3[] = _(" par jour.\nOn achète ?");
static const u8 sText_Possede1[] = _(" : à toi.\nValeur : ");
static const u8 sText_Possede2[] = _(" (payé ");
static const u8 sText_Possede3[] = _(").\pLoyer : environ ");
static const u8 sText_Possede4[] = _(" par jour.\nRevente : 5 % de frais d'agence.");
static const u8 sText_Squatte[] = _("\pUn squatteur Aqua s'y est installé.\nPas de loyer tant qu'il est là !");

// VAR_0x8004 : bien. VAR_RESULT : 0 = libre, 1 = à toi, 2 = à toi mais squatté.
void Immo_Fiche(void)
{
    u32 b = gSpecialVar_0x8004 < IMMO_NB_BIENS ? gSpecialVar_0x8004 : 0, jour = Aujourdhui();
    u8 *fin;

    Immo_MettreAJourJusqua(jour);
    fin = StringCopy(gStringVar4, sBiens[b].nom);
    if (!Immo_Possede(b))
    {
        fin = StringCopy(fin, sText_Libre1);
        fin = AjouterYens(fin, Immo_Prix(b, jour));
        fin = StringCopy(fin, sText_Libre2);
        fin = AjouterYens(fin, Immo_LoyerJour(b));
        StringCopy(fin, sText_Libre3);
        gSpecialVar_Result = 0;
        return;
    }
    fin = StringCopy(fin, sText_Possede1);
    fin = AjouterYens(fin, Immo_Prix(b, jour));
    fin = StringCopy(fin, sText_Possede2);
    fin = AjouterYens(fin, IMMO.prixAchat[b] * 100);
    fin = StringCopy(fin, sText_Possede3);
    fin = AjouterYens(fin, Immo_LoyerJour(b));
    fin = StringCopy(fin, sText_Possede4);
    if (IMMO.squat & (1u << b))
    {
        StringCopy(fin, sText_Squatte);
        gSpecialVar_Result = 2;
    }
    else
    {
        gSpecialVar_Result = 1;
    }
}

static const u8 sText_Achete[] = _("Signé ! Les clés sont à toi.\nUn locataire emménage demain.");
static const u8 sText_AchatRate[] = _("Pas assez d'argent pour le prix et\nle notaire. Reviens plus riche.");
static const u8 sText_Vendu[] = _("Vendu ! L'argent est sur ton compte.");
static const u8 sText_VenteRatee[] = _("Ton portefeuille est trop plein\n(999 999 ¥ max). Dépense d'abord.");

void Immo_ScriptAcheter(void)
{
    gSpecialVar_Result = Immo_Acheter(gSpecialVar_0x8004, Aujourdhui());
    StringCopy(gStringVar4, gSpecialVar_Result ? sText_Achete : sText_AchatRate);
}

void Immo_ScriptVendre(void)
{
    gSpecialVar_Result = Immo_Vendre(gSpecialVar_0x8004, Aujourdhui());
    StringCopy(gStringVar4, gSpecialVar_Result ? sText_Vendu : sText_VenteRatee);
}

// VAR_IMMO_BIEN : bien libéré après la victoire contre le squatteur.
void Immo_ScriptExpulser(void)
{
    Immo_Expulser(VarGet(VAR_IMMO_BIEN));
}

// Sbire envoyé contre le joueur : 0 avant le 2e badge, 1 avant le 4e, 2 avant le 6e, 3 ensuite.
u16 Immo_NiveauSquatteur(void)
{
    if (!FlagGet(FLAG_BADGE02_GET))
        return 0;
    if (!FlagGet(FLAG_BADGE04_GET))
        return 1;
    if (!FlagGet(FLAG_BADGE06_GET))
        return 2;
    return 3;
}
