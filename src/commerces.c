#include "global.h"
#include "commerces.h"
#include "caps.h"
#include "calendrier.h"
#include "coach_ev.h"
#include "event_data.h"
#include "immobilier.h"
#include "money.h"
#include "pokemon.h"
#include "rtc.h"
#include "societe.h"
#include "string_util.h"
#include "constants/characters.h"
#include "constants/flags.h"
#include "constants/vars.h"

// Émeraude Couronnement : commerces de la société (v0.36).
// Boutique : recette par jour = passage de la ville x stock x niveau x politique de prix, ±30 % (plus pour les Balls et
// les objets rares) ; un concurrent peut ouvrir en face (1 %/jour, 2 % avec des prix élevés, 0,5 % avec des prix bas) et
// prend 40 % de la recette jusqu'à ce qu'on le batte en combat ou qu'on tienne 5 jours de prix bas.
// Centre d'entraînement : 250 ¥ par jour et par niveau (dresseurs qui paient), plus 30 points d'entraînement par jour et par
// niveau : 100 points = un niveau pour un Pokémon de l'équipe (jamais au-dessus du level cap), 40 points = +12 EV.
// Les recettes vont dans la trésorerie de la société.

// VAR_COMMERCE_BOUTIQUE : bit 0 ouverte, bits 1-4 ville, 5-6 stock, 7-8 prix, 9-10 niveau, 11 concurrent, 12 concurrent à annoncer.
#define B_OUVERTE        (1 << 0)
#define B_VILLE(v)       (((v) >> 1) & 0xF)
#define B_STOCK(v)       (((v) >> 5) & 3)
#define B_PRIX(v)        (((v) >> 7) & 3)
#define B_NIVEAU(v)      (((v) >> 9) & 3)
#define B_CONCURRENT     (1 << 11)
#define B_ANNONCE        (1 << 12)
// VAR_COMMERCE_CENTRE : bit 0 ouvert, bits 1-2 niveau.
#define C_OUVERT         (1 << 0)
#define C_NIVEAU(v)      (((v) >> 1) & 3)

enum { PRIX_BAS, PRIX_NORMAL, PRIX_ELEVE };

static const u8 *const sVilles[COMMERCE_NB_VILLES] =
{
    COMPOUND_STRING("Rosyères"), COMPOUND_STRING("Clémenti"), COMPOUND_STRING("Mérouville"), COMPOUND_STRING("Myokara"),
    COMPOUND_STRING("Poivressel"), COMPOUND_STRING("Lavandia"), COMPOUND_STRING("Vergazon"), COMPOUND_STRING("Vermilava"),
    COMPOUND_STRING("Cimetronelle"), COMPOUND_STRING("Nénucrique"), COMPOUND_STRING("Algatia"), COMPOUND_STRING("Atalanopolis"),
};
static const u16 sPassage[COMMERCE_NB_VILLES] = {200, 300, 500, 150, 600, 650, 250, 400, 350, 900, 500, 450};

static const u8 *const sStocks[COMMERCE_NB_STOCKS] =
{
    COMPOUND_STRING("Baies"), COMPOUND_STRING("Soins"), COMPOUND_STRING("Poké Balls"), COMPOUND_STRING("Objets rares"),
};
static const u8 sStockPourcent[COMMERCE_NB_STOCKS] = {80, 100, 120, 150};
static const u8 sStockAlea[COMMERCE_NB_STOCKS]     = {20, 30, 40, 70};   // ± % par jour

static const u8 *const sPrix[COMMERCE_NB_PRIX] =
{
    COMPOUND_STRING("bas"), COMPOUND_STRING("normaux"), COMPOUND_STRING("élevés"),
};
static const u8 sPrixPourcent[COMMERCE_NB_PRIX]     = {90, 100, 115};
static const u8 sPrixConcurrence[COMMERCE_NB_PRIX]  = {5, 10, 20};       // pour mille par jour

static const u8 sNiveauPourcent[COMMERCE_NIVEAUX]   = {100, 160, 220};
static const u32 sAgrandirBoutique[COMMERCE_NIVEAUX - 1] = {40000, 100000};
static const u32 sAgrandirCentre[COMMERCE_NIVEAUX - 1]   = {60000, 150000};

// ---------------------------------------------------------------- outils

static u32 Melanger(u32 x)
{
    x ^= x >> 16;
    x *= 0x7FEB352D;
    x ^= x >> 15;
    x *= 0x846CA68B;
    x ^= x >> 16;
    return x;
}

static u32 Alea(u32 jour, u32 tirage)
{
    const u8 *id = gSaveBlock2Ptr->playerTrainerId;
    u32 graine = id[0] | (id[1] << 8) | (id[2] << 16) | (id[3] << 24);
    return Melanger((jour * 0x27D4EB2Fu) ^ graine ^ (tirage << 18) ^ 0xC0FFEE);
}

static u32 Aujourdhui(void)
{
    return RtcGetLocalDayCount();
}

static u32 Boutique(void) { return VarGet(VAR_COMMERCE_BOUTIQUE); }
static u32 Centre(void)   { return VarGet(VAR_COMMERCE_CENTRE); }

static void Encaisser(u32 montant)
{
    struct SocieteSave *soc = &gSaveBlock2Ptr->pokedex.societe;
    u32 recettes;

    if (soc->etat != SOCIETE_ETAT_AUCUNE)
        soc->tresorerie = (u64)soc->tresorerie + montant > 2000000000 ? 2000000000 : soc->tresorerie + montant;
    else
        AddMoney(&gSaveBlock1Ptr->money, montant);
    recettes = VarGet(VAR_COMMERCE_RECETTES_BAS) | (VarGet(VAR_COMMERCE_RECETTES_HAUT) << 16);
    recettes = (u64)recettes + montant > 0x7FFFFFFF ? 0x7FFFFFFF : recettes + montant;
    VarSet(VAR_COMMERCE_RECETTES_BAS, recettes & 0xFFFF);
    VarSet(VAR_COMMERCE_RECETTES_HAUT, recettes >> 16);
}

// Paiement : trésorerie de la société d'abord, puis l'argent du joueur. FALSE s'il manque de l'argent (rien n'est pris).
static bool32 Payer(u32 montant)
{
    struct SocieteSave *soc = &gSaveBlock2Ptr->pokedex.societe;
    u32 tresorerie = soc->etat != SOCIETE_ETAT_AUCUNE ? soc->tresorerie : 0, pris;

    if ((u64)tresorerie + GetMoney(&gSaveBlock1Ptr->money) < montant)
        return FALSE;
    pris = montant < tresorerie ? montant : tresorerie;
    if (soc->etat != SOCIETE_ETAT_AUCUNE)
        soc->tresorerie -= pris;
    RemoveMoney(&gSaveBlock1Ptr->money, montant - pris);
    return TRUE;
}

// ---------------------------------------------------------------- moteur

bool32 Commerce_BoutiqueOuverte(void) { return (Boutique() & B_OUVERTE) != 0; }
bool32 Commerce_CentreOuvert(void)    { return (Centre() & C_OUVERT) != 0; }

u32 Commerce_PrixBoutique(u32 ville)
{
    return ville < COMMERCE_NB_VILLES ? sPassage[ville] * 150 : 0;
}

u32 Commerce_PrixAgrandirBoutique(void)
{
    u32 n = B_NIVEAU(Boutique());
    return (Commerce_BoutiqueOuverte() && n < COMMERCE_NIVEAUX - 1) ? sAgrandirBoutique[n] : 0;
}

u32 Commerce_PrixAgrandirCentre(void)
{
    u32 n = C_NIVEAU(Centre());
    return (Commerce_CentreOuvert() && n < COMMERCE_NIVEAUX - 1) ? sAgrandirCentre[n] : 0;
}

u32 Commerce_RecetteMoyenneBoutique(void)
{
    u32 b = Boutique(), recette;

    if (!(b & B_OUVERTE))
        return 0;
    recette = (u64)sPassage[B_VILLE(b) % COMMERCE_NB_VILLES] * sStockPourcent[B_STOCK(b)] * sNiveauPourcent[B_NIVEAU(b) % COMMERCE_NIVEAUX]
            * sPrixPourcent[B_PRIX(b) % COMMERCE_NB_PRIX] / 1000000;
    if (b & B_CONCURRENT)
        recette = recette * COMMERCE_CONCURRENT_RECETTE / 100;
    return recette;
}

u32 Commerce_RecetteBoutique(u32 jour)
{
    u32 b = Boutique(), alea = sStockAlea[B_STOCK(b)];
    s32 ecart = (s32)(Alea(jour, 1) % (2 * alea + 1)) - (s32)alea;

    u32 recette = (u64)Commerce_RecetteMoyenneBoutique() * (100 + ecart) / 100;

    // Samedi : jour d'affluence (+30 %) ; dimanche : ouverture réduite (-50 %). (v0.37)
    if (Calendrier_JourSemaine(jour) == WEEKDAY_SAT)
        recette = recette * 130 / 100;
    else if (Calendrier_JourSemaine(jour) == WEEKDAY_SUN)
        recette = recette / 2;
    return recette;
}

bool32 Commerce_OuvrirBoutique(u32 ville, u32 stock)
{
    if (Commerce_BoutiqueOuverte() || ville >= COMMERCE_NB_VILLES || stock >= COMMERCE_NB_STOCKS)
        return FALSE;
    if (!Payer(Commerce_PrixBoutique(ville)))
        return FALSE;
    VarSet(VAR_COMMERCE_BOUTIQUE, B_OUVERTE | (ville << 1) | (stock << 5) | (PRIX_NORMAL << 7));
    VarSet(VAR_COMMERCE_GUERRE_PRIX, 0);
    if (VarGet(VAR_COMMERCE_JOUR) == 0)
        VarSet(VAR_COMMERCE_JOUR, Aujourdhui());
    return TRUE;
}

bool32 Commerce_OuvrirCentre(void)
{
    if (Commerce_CentreOuvert() || !Payer(COMMERCE_CENTRE_PRIX))
        return FALSE;
    VarSet(VAR_COMMERCE_CENTRE, C_OUVERT);
    if (VarGet(VAR_COMMERCE_JOUR) == 0)
        VarSet(VAR_COMMERCE_JOUR, Aujourdhui());
    return TRUE;
}

bool32 Commerce_AgrandirBoutique(void)
{
    u32 prix = Commerce_PrixAgrandirBoutique();
    if (prix == 0 || !Payer(prix))
        return FALSE;
    VarSet(VAR_COMMERCE_BOUTIQUE, Boutique() + (1 << 9));
    return TRUE;
}

bool32 Commerce_AgrandirCentre(void)
{
    u32 prix = Commerce_PrixAgrandirCentre();
    if (prix == 0 || !Payer(prix))
        return FALSE;
    VarSet(VAR_COMMERCE_CENTRE, Centre() + (1 << 1));
    return TRUE;
}

void Commerce_ChoisirStock(u32 stock)
{
    if (stock < COMMERCE_NB_STOCKS && Commerce_BoutiqueOuverte())
        VarSet(VAR_COMMERCE_BOUTIQUE, (Boutique() & ~(3 << 5)) | (stock << 5));
}

void Commerce_ChoisirPrix(u32 prix)
{
    if (prix < COMMERCE_NB_PRIX && Commerce_BoutiqueOuverte())
        VarSet(VAR_COMMERCE_BOUTIQUE, (Boutique() & ~(3 << 7)) | (prix << 7));
}

void Commerce_PasserJour(u32 jour)
{
    u32 b = Boutique();

    if (b & B_OUVERTE)
    {
        Encaisser(Commerce_RecetteBoutique(jour));
        if (b & B_CONCURRENT)
        {
            // Prix bas tenus 5 jours : le concurrent baisse le rideau.
            if (B_PRIX(b) == PRIX_BAS)
            {
                u32 jours = VarGet(VAR_COMMERCE_GUERRE_PRIX) + 1;
                if (jours >= COMMERCE_GUERRE_JOURS)
                {
                    b &= ~B_CONCURRENT;
                    jours = 0;
                }
                VarSet(VAR_COMMERCE_GUERRE_PRIX, jours);
            }
            else
            {
                VarSet(VAR_COMMERCE_GUERRE_PRIX, 0);
            }
        }
        else if (Alea(jour, 2) % 1000 < sPrixConcurrence[B_PRIX(b) % COMMERCE_NB_PRIX])
        {
            b |= B_CONCURRENT | B_ANNONCE;
            VarSet(VAR_COMMERCE_GUERRE_PRIX, 0);
        }
        VarSet(VAR_COMMERCE_BOUTIQUE, b);
    }
    if (Commerce_CentreOuvert())
    {
        u32 niveau = C_NIVEAU(Centre()) + 1, points;
        u32 gain = COMMERCE_CENTRE_POINTS * niveau * (Calendrier_JourSemaine(jour) == WEEKDAY_SUN ? 2 : 1); // dimanche : stage x2
        points = VarGet(VAR_COMMERCE_POINTS) + gain;
        Encaisser(COMMERCE_CENTRE_RECETTE * niveau);
        VarSet(VAR_COMMERCE_POINTS, points > COMMERCE_POINTS_MAX ? COMMERCE_POINTS_MAX : points);
    }
}

void Commerce_MettreAJourJusqua(u32 aujourdhui)
{
    u32 jour = VarGet(VAR_COMMERCE_JOUR);

    if (!Commerce_BoutiqueOuverte() && !Commerce_CentreOuvert())
        return;
    if (jour == 0 || aujourdhui < jour)
        jour = aujourdhui;
    if (aujourdhui - jour > COMMERCE_RATTRAPAGE_MAX)
        jour = aujourdhui - COMMERCE_RATTRAPAGE_MAX;
    while (jour < aujourdhui)
    {
        jour++;
        Commerce_PasserJour(jour);
    }
    VarSet(VAR_COMMERCE_JOUR, jour);
}

u32 Commerce_Valeur(void)
{
    u32 valeur = 0, b = Boutique(), c = Centre(), i;

    if (b & B_OUVERTE)
    {
        valeur += Commerce_PrixBoutique(B_VILLE(b) % COMMERCE_NB_VILLES);
        for (i = 0; i < B_NIVEAU(b) && i < COMMERCE_NIVEAUX - 1; i++)
            valeur += sAgrandirBoutique[i];
    }
    if (c & C_OUVERT)
    {
        valeur += COMMERCE_CENTRE_PRIX;
        for (i = 0; i < C_NIVEAU(c) && i < COMMERCE_NIVEAUX - 1; i++)
            valeur += sAgrandirCentre[i];
    }
    return valeur;
}

static bool32 PrendrePoints(u32 points)
{
    u32 dispo = VarGet(VAR_COMMERCE_POINTS);
    if (dispo < points)
        return FALSE;
    VarSet(VAR_COMMERCE_POINTS, dispo - points);
    return TRUE;
}

// Un niveau de plus (expérience posée au début du niveau suivant), jamais au-dessus du level cap.
bool32 Commerce_SeanceXP(struct Pokemon *mon)
{
    u32 niveau = GetMonData(mon, MON_DATA_LEVEL), species = GetMonData(mon, MON_DATA_SPECIES), exp;

    if (!Commerce_CentreOuvert() || GetMonData(mon, MON_DATA_IS_EGG) || niveau >= MAX_LEVEL || niveau >= GetCurrentLevelCap())
        return FALSE;
    if (!PrendrePoints(COMMERCE_SEANCE_XP))
        return FALSE;
    exp = gExperienceTables[gSpeciesInfo[species].growthRate][niveau + 1];
    SetMonData(mon, MON_DATA_EXP, &exp);
    CalculateMonStats(mon);
    return TRUE;
}

// +12 EV dans une stat (ordre d'affichage du Coach EV), dans la limite de 252 par stat et 510 au total.
bool32 Commerce_SeanceEV(struct Pokemon *mon, u32 stat)
{
    static const u8 sStatEv[NUM_STATS] =
    {
        MON_DATA_HP_EV, MON_DATA_ATK_EV, MON_DATA_DEF_EV, MON_DATA_SPATK_EV, MON_DATA_SPDEF_EV, MON_DATA_SPEED_EV,
    };
    u32 actuel;

    if (!Commerce_CentreOuvert() || stat >= NUM_STATS || GetMonData(mon, MON_DATA_IS_EGG))
        return FALSE;
    actuel = GetMonData(mon, sStatEv[stat]);
    if (actuel >= MAX_PER_STAT_EVS || CoachEV_Total(mon) >= MAX_TOTAL_EVS)
        return FALSE;
    if (!PrendrePoints(COMMERCE_SEANCE_EV))
        return FALSE;
    CoachEV_Regler(mon, stat, actuel + COMMERCE_EV_PAR_SEANCE);
    return TRUE;
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

static const u8 sText_Rapport1[] = _("Recettes de tes commerces depuis\nta dernière visite : ");
static const u8 sText_Rapport2[] = _(".");
static const u8 sText_RapportConcurrent[] = _("\pUn concurrent a ouvert en face de\nta boutique ! Il te vole des clients.");

// Met à jour. VAR_RESULT = TRUE s'il y a un rapport (gStringVar4).
void Commerce_Ouvrir(void)
{
    u32 recettes, b;
    u8 *fin;

    Commerce_MettreAJourJusqua(Aujourdhui());
    recettes = VarGet(VAR_COMMERCE_RECETTES_BAS) | (VarGet(VAR_COMMERCE_RECETTES_HAUT) << 16);
    b = Boutique();
    gSpecialVar_Result = FALSE;
    if (recettes == 0 && !(b & B_ANNONCE))
        return;
    fin = StringCopy(gStringVar4, sText_Rapport1);
    fin = AjouterYens(fin, recettes);
    fin = StringCopy(fin, sText_Rapport2);
    if (b & B_ANNONCE)
    {
        StringCopy(fin, sText_RapportConcurrent);
        VarSet(VAR_COMMERCE_BOUTIQUE, b & ~B_ANNONCE);
    }
    VarSet(VAR_COMMERCE_RECETTES_BAS, 0);
    VarSet(VAR_COMMERCE_RECETTES_HAUT, 0);
    gSpecialVar_Result = TRUE;
}

static const u8 sText_PasDeBoutique[] = _("Pas de boutique.");
static const u8 sText_Boutique1[] = _("Boutique de ");
static const u8 sText_Boutique2[] = _(" : ");
static const u8 sText_Boutique3[] = _("\nNiveau ");
static const u8 sText_Boutique4[] = _(", prix ");
static const u8 sText_Boutique5[] = _(", ~");
static const u8 sText_Boutique6[] = _("/jour.");
static const u8 sText_Concurrent[] = _("\lUn concurrent est en face !");
static const u8 sText_PasDeCentre[] = _("\pPas de centre d'entraînement.");
static const u8 sText_Centre1[] = _("\pCentre d'entraînement niveau ");
static const u8 sText_Centre2[] = _(".\n");
static const u8 sText_Centre3[] = _(" points, ~");

// gStringVar4 = état des commerces ; VAR_RESULT = 0 sans société, sinon 1 + (boutique ouverte) + 2 x (centre ouvert).
void Commerce_Resume(void)
{
    u32 b = Boutique(), c = Centre();
    u8 *fin = gStringVar4;

    Commerce_MettreAJourJusqua(Aujourdhui());
    if (gSaveBlock2Ptr->pokedex.societe.etat == SOCIETE_ETAT_AUCUNE)
    {
        gSpecialVar_Result = 0;
        return;
    }
    if (b & B_OUVERTE)
    {
        fin = StringCopy(fin, sText_Boutique1);
        fin = StringCopy(fin, sVilles[B_VILLE(b) % COMMERCE_NB_VILLES]);
        fin = StringCopy(fin, sText_Boutique2);
        fin = StringCopy(fin, sStocks[B_STOCK(b)]);
        fin = StringCopy(fin, sText_Boutique3);
        fin = AjouterNombre(fin, B_NIVEAU(b) + 1);
        fin = StringCopy(fin, sText_Boutique4);
        fin = StringCopy(fin, sPrix[B_PRIX(b) % COMMERCE_NB_PRIX]);
        fin = StringCopy(fin, sText_Boutique5);
        fin = AjouterYens(fin, Commerce_RecetteMoyenneBoutique());
        fin = StringCopy(fin, sText_Boutique6);
        if (b & B_CONCURRENT)
            fin = StringCopy(fin, sText_Concurrent);
    }
    else
    {
        fin = StringCopy(fin, sText_PasDeBoutique);
    }
    if (c & C_OUVERT)
    {
        fin = StringCopy(fin, sText_Centre1);
        fin = AjouterNombre(fin, C_NIVEAU(c) + 1);
        fin = StringCopy(fin, sText_Centre2);
        fin = AjouterNombre(fin, VarGet(VAR_COMMERCE_POINTS));
        fin = StringCopy(fin, sText_Centre3);
        fin = AjouterYens(fin, COMMERCE_CENTRE_RECETTE * (C_NIVEAU(c) + 1));
        StringCopy(fin, sText_Boutique6);
    }
    else
    {
        StringCopy(fin, sText_PasDeCentre);
    }
    gSpecialVar_Result = 1 + ((b & B_OUVERTE) ? 1 : 0) + ((c & C_OUVERT) ? 2 : 0);
}

// Liste des villes pour la boutique, avec le prix du local (dans gImmoLignes, libre hors de l'appli Immobilier).
void Commerce_PreparerVilles(void)
{
    u32 i;
    for (i = 0; i < COMMERCE_NB_VILLES; i++)
    {
        u8 *fin = StringCopy(gImmoLignes[i], sVilles[i]);
        *fin++ = CHAR_SPACE;
        fin = AjouterNombre(fin, Commerce_PrixBoutique(i) / 1000);
        *fin++ = CHAR_k;
        *fin = EOS;
    }
}

static const u8 sText_Ouverte[] = _("Ta boutique ouvre ses portes !\nLes recettes iront à ta société.");
static const u8 sText_PasAssez[] = _("Pas assez d'argent, même avec la\ntrésorerie de ta société.");
static const u8 sText_CentreOuvert[] = _("Ton centre d'entraînement est\nouvert. Les dresseurs affluent !");
static const u8 sText_Agrandi[] = _("Travaux terminés : c'est plus grand,\nça rapporte plus.");
static const u8 sText_PasAgrandi[] = _("Déjà au maximum, ou pas assez\nd'argent.");
static const u8 sText_Stock[] = _("Nouveau stock en rayon.");
static const u8 sText_Prix[] = _("Étiquettes changées.");
static const u8 sText_SeanceXP[] = _("Séance terminée : un niveau de plus !\nLes attaques oubliées se réapprennent\lchez le Maître des Capacités.");
static const u8 sText_SeanceEV[] = _("Séance terminée : +12 EV.");
static const u8 sText_SeanceRatee[] = _("Pas assez de points, Pokémon au level\ncap, ou EV déjà au maximum.");

// VAR_0x8004 : ville, VAR_0x8005 : stock.
void Commerce_ScriptOuvrirBoutique(void)
{
    gSpecialVar_Result = Commerce_OuvrirBoutique(gSpecialVar_0x8004, gSpecialVar_0x8005);
    StringCopy(gStringVar4, gSpecialVar_Result ? sText_Ouverte : sText_PasAssez);
}

void Commerce_ScriptOuvrirCentre(void)
{
    gSpecialVar_Result = Commerce_OuvrirCentre();
    StringCopy(gStringVar4, gSpecialVar_Result ? sText_CentreOuvert : sText_PasAssez);
}

void Commerce_ScriptStock(void)
{
    Commerce_ChoisirStock(gSpecialVar_0x8005);
    gSpecialVar_Result = TRUE;
    StringCopy(gStringVar4, sText_Stock);
}

void Commerce_ScriptPrix(void)
{
    Commerce_ChoisirPrix(gSpecialVar_0x8005);
    gSpecialVar_Result = TRUE;
    StringCopy(gStringVar4, sText_Prix);
}

void Commerce_ScriptAgrandirBoutique(void)
{
    gSpecialVar_Result = Commerce_AgrandirBoutique();
    StringCopy(gStringVar4, gSpecialVar_Result ? sText_Agrandi : sText_PasAgrandi);
}

void Commerce_ScriptAgrandirCentre(void)
{
    gSpecialVar_Result = Commerce_AgrandirCentre();
    StringCopy(gStringVar4, gSpecialVar_Result ? sText_Agrandi : sText_PasAgrandi);
}

// VAR_0x8004 : Pokémon de l'équipe.
void Commerce_ScriptSeanceXP(void)
{
    gSpecialVar_Result = Commerce_SeanceXP(&gPlayerParty[gSpecialVar_0x8004]);
    StringCopy(gStringVar4, gSpecialVar_Result ? sText_SeanceXP : sText_SeanceRatee);
}

// VAR_0x8004 : Pokémon, VAR_0x8005 : stat.
void Commerce_ScriptSeanceEV(void)
{
    gSpecialVar_Result = Commerce_SeanceEV(&gPlayerParty[gSpecialVar_0x8004], gSpecialVar_0x8005);
    StringCopy(gStringVar4, gSpecialVar_Result ? sText_SeanceEV : sText_SeanceRatee);
}

void Commerce_ConcurrentBattu(void)
{
    VarSet(VAR_COMMERCE_BOUTIQUE, Boutique() & ~(B_CONCURRENT | B_ANNONCE));
    VarSet(VAR_COMMERCE_GUERRE_PRIX, 0);
}

// Gérant concurrent : 0 avant le 2e badge, 1 avant le 4e, 2 avant le 6e, 3 ensuite.
u16 Commerce_NiveauConcurrent(void)
{
    if (!FlagGet(FLAG_BADGE02_GET))
        return 0;
    if (!FlagGet(FLAG_BADGE04_GET))
        return 1;
    if (!FlagGet(FLAG_BADGE06_GET))
        return 2;
    return 3;
}

u16 Commerce_EtatBoutique(void)
{
    u32 b = Boutique();
    if (!(b & B_OUVERTE))
        return 0;
    return (b & B_CONCURRENT) ? 2 : 1;
}

u16 Commerce_EtatCentre(void)
{
    return Commerce_CentreOuvert();
}

static const u8 sText_Agrandir1[] = _("Agrandir coûte ");
static const u8 sText_Agrandir2[] = _(".\nOn lance les travaux ?");
static const u8 sText_AgrandirMax[] = _("C'est déjà le plus grand modèle.");

void Commerce_TexteAgrandir(void)
{
    u32 prix = gSpecialVar_0x8005 ? Commerce_PrixAgrandirCentre() : Commerce_PrixAgrandirBoutique();
    u8 *fin;

    gSpecialVar_Result = prix != 0;
    if (prix == 0)
    {
        StringCopy(gStringVar4, sText_AgrandirMax);
        return;
    }
    fin = StringCopy(gStringVar4, sText_Agrandir1);
    fin = AjouterYens(fin, prix);
    StringCopy(fin, sText_Agrandir2);
}
