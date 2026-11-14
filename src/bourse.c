#include "global.h"
#include "bourse.h"
#include "couronnement.h"
#include "calendrier.h"
#include "event_data.h"
#include "money.h"
#include "rtc.h"
#include "string_util.h"
#include "constants/flags.h"
#include "constants/vars.h"
#include "constants/characters.h"

// Émeraude Couronnement : Bourse de Hoenn (v0.33).
// 9 entreprises dont le cours bouge une fois par jour de jeu (horloge de la cartouche), avec des krachs et des envolées,
// des chocs liés au scénario (délit d'initié possible), Télé Hoenn indexée sur l'Elo du Couronnement, un dividende hebdomadaire
// pour les Grands Magasins, un crédit à 1 % par jour et un huissier qui saisit tout quand la dette dépasse le portefeuille.
// Le hasard est déterministe (jour + n° de Dresseur) : recharger une sauvegarde ne change pas le cours du lendemain,
// ce qui permet aussi d'annoncer une rumeur sur demain (vraie 7 fois sur 10).

#define BOURSE (gSaveBlock2Ptr->pokedex.bourse)
#define HISTO (gSaveBlock3Ptr->bourseHisto)
#define OR (gSaveBlock3Ptr->bourseOr)

static void HistoriqueAjouter(u32 jour);
static void OrHistoriqueReconstruire(void);
static void OrInitialiser(void);

STATIC_ASSERT(sizeof(struct BourseSave) <= 0x68, BourseSaveTropGrande);

struct Entreprise
{
    const u8 *nom;
    u16 depart;      // cours de départ
    s8 tendance;     // pour mille par jour
    u8 volatilite;   // ± pour mille par jour
    u8 krach;        // chances sur 1000 par jour d'un krach (-40 à -80 %)
    u8 envolee;      // chances sur 1000 par jour d'une envolée
    u16 envoleeMax;  // envolée : +30 % à +envoleeMax pour mille
};

static const struct Entreprise sEntreprises[BOURSE_NB_ENTREPRISES] =
{
    [BOURSE_DEVON]    = {COMPOUND_STRING("Devon SA"),        600,  4,  25,  3,  3,  600},
    [BOURSE_STERN]    = {COMPOUND_STRING("Chantier Stern"),  300,  3,  35,  4,  4,  800},
    [BOURSE_CASINO]   = {COMPOUND_STRING("Casino Lavandia"), 150,  0, 120, 15, 15, 2000},
    [BOURSE_TELE]     = {COMPOUND_STRING("Télé Hoenn"),      400,  2,  40,  4,  4,  800},
    [BOURSE_MAGASINS] = {COMPOUND_STRING("Grands Magasins"), 500,  3,  15,  2,  2,  400},
    [BOURSE_AQUA]     = {COMPOUND_STRING("Aqua Corp"),       800, 10,  50,  5,  8, 1000},
    [BOURSE_MAGMA]    = {COMPOUND_STRING("Magma Corp"),      800, 10,  50,  5,  8, 1000},
    [BOURSE_PENSION]  = {COMPOUND_STRING("Pension"),         100,  2,  10,  1,  1,  300},
    [BOURSE_SAFARI]   = {COMPOUND_STRING("Parc Safari"),     120,  2,  12,  1,  2,  400},
};

// Chocs du scénario : appliqués une seule fois, dès que le drapeau est posé (bit = index dans la table).
struct Choc
{
    u16 drapeau;
    u8 entreprise;
    s16 pourmille;
};

static const struct Choc sChocs[] =
{
    {FLAG_DEVON_GOODS_STOLEN,             BOURSE_DEVON,    -250},
    {FLAG_DEVON_GOODS_STOLEN,             BOURSE_AQUA,      150},
    {FLAG_RECOVERED_DEVON_GOODS,          BOURSE_DEVON,     400},
    {FLAG_DELIVERED_DEVON_GOODS,          BOURSE_STERN,     300},
    {FLAG_DEFEATED_EVIL_TEAM_MT_CHIMNEY,  BOURSE_MAGMA,    -400},
    {FLAG_GROUDON_AWAKENED_MAGMA_HIDEOUT, BOURSE_MAGMA,     600},
    {FLAG_KYOGRE_ESCAPED_SEAFLOOR_CAVERN, BOURSE_STERN,    -500},
    {FLAG_KYOGRE_ESCAPED_SEAFLOOR_CAVERN, BOURSE_AQUA,      800},
    {FLAG_DEFEATED_MAGMA_SPACE_CENTER,    BOURSE_MAGMA,    -900},
    {FLAG_SOOTOPOLIS_ARCHIE_MAXIE_LEAVE,  BOURSE_AQUA,     -900},
    {FLAG_SOOTOPOLIS_ARCHIE_MAXIE_LEAVE,  BOURSE_STERN,    1000},
    {FLAG_SOOTOPOLIS_ARCHIE_MAXIE_LEAVE,  BOURSE_TELE,      300},
    {FLAG_SYS_GAME_CLEAR,                 BOURSE_TELE,      500},
    {FLAG_SYS_GAME_CLEAR,                 BOURSE_MAGASINS,  200},
    {FLAG_BADGE01_GET,                    BOURSE_TELE,       80},
    {FLAG_BADGE02_GET,                    BOURSE_TELE,       80},
    {FLAG_BADGE03_GET,                    BOURSE_TELE,       80},
    {FLAG_BADGE04_GET,                    BOURSE_TELE,       80},
    {FLAG_BADGE05_GET,                    BOURSE_TELE,       80},
    {FLAG_BADGE06_GET,                    BOURSE_TELE,       80},
    {FLAG_BADGE07_GET,                    BOURSE_TELE,       80},
    {FLAG_BADGE08_GET,                    BOURSE_TELE,       80},
};

STATIC_ASSERT(ARRAY_COUNT(sChocs) <= 32, BourseTropDeChocs);

EWRAM_DATA u8 gBourseLignes[BOURSE_NB_ENTREPRISES + 2][32] = {0};

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

static u32 Alea(u32 jour, u32 entreprise, u32 tirage)
{
    const u8 *id = gSaveBlock2Ptr->playerTrainerId;
    u32 graine = id[0] | (id[1] << 8) | (id[2] << 16) | (id[3] << 24);
    return Melanger(jour * 0x9E3779B1u ^ graine ^ (entreprise << 8) ^ (tirage << 20));
}

// ---------------------------------------------------------------- outils

static u32 Borner(u64 valeur, u32 min, u32 max)
{
    if (valeur < min)
        return min;
    if (valeur > max)
        return max;
    return valeur;
}

static u16 AppliquerPourMille(u16 cours, s32 pourmille)
{
    if (pourmille < -900)
        pourmille = -900;
    return Borner((u64)cours * (1000 + pourmille) / 1000, BOURSE_COURS_MIN, BOURSE_COURS_MAX);
}

static bool32 CriseDesLegendaires(void)
{
    return FlagGet(FLAG_KYOGRE_ESCAPED_SEAFLOOR_CAVERN) && !FlagGet(FLAG_SOOTOPOLIS_ARCHIE_MAXIE_LEAVE);
}

static const u8 sText_Or[] = _("Or XAU/¥");

// ---------------------------------------------------------------- actifs : 9 entreprises + l'or (v0.41)
u32 Bourse_Cours(u32 e)
{
    return e == BOURSE_OR ? OR.cours : (e < BOURSE_NB_ENTREPRISES ? BOURSE.cours[e] : 0);
}

static u32 Veille(u32 e)
{
    return e == BOURSE_OR ? OR.veille : BOURSE.veille[e];
}

u32 Bourse_Parts(u32 e)
{
    return e == BOURSE_OR ? OR.parts : (e < BOURSE_NB_ENTREPRISES ? BOURSE.parts[e] : 0);
}

static void ChangerParts(u32 e, s32 delta)
{
    if (e == BOURSE_OR)
        OR.parts += delta;
    else
        BOURSE.parts[e] += delta;
}

static u32 Frais(u32 e)
{
    return e == BOURSE_OR ? BOURSE_OR_FRAIS_POURCENT : BOURSE_FRAIS_POURCENT;
}

static const u8 *Nom(u32 e)
{
    return e == BOURSE_OR ? sText_Or : sEntreprises[e].nom;
}

static bool32 EnFaillite(u32 entreprise)
{
    if (entreprise == BOURSE_AQUA)
        return FlagGet(FLAG_SOOTOPOLIS_ARCHIE_MAXIE_LEAVE);
    if (entreprise == BOURSE_MAGMA)
        return FlagGet(FLAG_DEFEATED_MAGMA_SPACE_CENTER);
    return FALSE;
}

// ---------------------------------------------------------------- moteur

void Bourse_Initialiser(u32 jour)
{
    u32 i;

    memset(&BOURSE, 0, sizeof(BOURSE));
    for (i = 0; i < BOURSE_NB_ENTREPRISES; i++)
    {
        BOURSE.cours[i] = sEntreprises[i].depart;
        BOURSE.veille[i] = sEntreprises[i].depart;
    }
    BOURSE.jour = jour;
    BOURSE.jourDividende = jour;
    HISTO.magie = 0; // l'historique sera reconstruit
    // Ce qui s'est passé avant l'ouverture de la Bourse est déjà « dans les cours ».
    for (i = 0; i < ARRAY_COUNT(sChocs); i++)
    {
        if (FlagGet(sChocs[i].drapeau))
            BOURSE.evenements |= 1u << i;
    }
}

s32 Bourse_VariationDuJour(u32 entreprise, u32 jour)
{
    const struct Entreprise *e = &sEntreprises[entreprise];
    s32 tendance = e->tendance, volatilite = e->volatilite, krach = e->krach, envolee = e->envolee;
    u32 de = Alea(jour, entreprise, 0) % 1000;
    s32 variation;

    if (EnFaillite(entreprise))
    {
        tendance = -2;
        volatilite = 15;
        envolee = 0;
    }
    else if (CriseDesLegendaires())
    {
        if (entreprise == BOURSE_STERN)
            tendance -= 20;
        else if (entreprise == BOURSE_PENSION || entreprise == BOURSE_SAFARI)
            tendance -= 5;
        else if (entreprise == BOURSE_TELE || entreprise == BOURSE_AQUA || entreprise == BOURSE_MAGMA)
            tendance += 15;
    }
    if (entreprise == BOURSE_TELE)
    {
        u32 elo = VarGet(VAR_COURONNEMENT_ELO);
        if (elo != 0)
            tendance += ((s32)elo - 1500) / 100;
    }

    if ((s32)de < krach)
        variation = -(400 + (s32)(Alea(jour, entreprise, 2) % 401));
    else if ((s32)de >= 1000 - envolee)
        variation = 300 + (s32)(Alea(jour, entreprise, 3) % (e->envoleeMax - 300 + 1));
    else
        variation = tendance + (s32)(Alea(jour, entreprise, 1) % (2 * volatilite + 1)) - volatilite;

    // Krach général du marché : environ une fois tous les 250 jours, -30 à -60 % pour tout le monde.
    if (Alea(jour, 15, 0) % 1000 < 4)
        variation -= 300 + (s32)(Alea(jour, 15, 1) % 301);

    if (variation < -900)
        variation = -900;
    return variation;
}

// L'or (v0.41) : tendance douce, faible volatilité, valeur refuge pendant les krachs et la crise des légendaires.
s32 Bourse_VariationOr(u32 jour)
{
    s32 tendance = 1 + (CriseDesLegendaires() ? 3 : 0), variation;
    u32 de = Alea(jour, 20, 0) % 1000;

    if (de < 2)
        variation = -(50 + (s32)(Alea(jour, 20, 1) % 51));
    else if (de >= 997)
        variation = 50 + (s32)(Alea(jour, 20, 2) % 101);
    else
        variation = tendance + (s32)(Alea(jour, 20, 3) % 21) - 10;
    if (Alea(jour, 15, 0) % 1000 < 4) // krach général : tout le monde se rue sur l'or
        variation += 40 + (s32)(Alea(jour, 20, 4) % 61);
    return variation;
}

static u32 AppliquerOr(u32 cours, s32 pourmille)
{
    return Borner((u64)cours * (1000 + pourmille) / 1000, BOURSE_OR_MIN, BOURSE_OR_MAX);
}

// Dividende des Grands Magasins : chaque vendredi (v0.37).
static void VerserDividende(u32 jour)
{
    u32 dividende;

    if (Calendrier_JourSemaine(jour) != WEEKDAY_FRI || BOURSE.jourDividende == jour)
        return;
    BOURSE.jourDividende = jour;
    dividende = (u64)BOURSE.parts[BOURSE_MAGASINS] * BOURSE.cours[BOURSE_MAGASINS] * BOURSE_DIVIDENDE_POURMILLE / 1000;
    if (dividende)
        AddMoney(&gSaveBlock1Ptr->money, dividende);
}

void Bourse_PasserJour(u32 jour)
{
    u32 i;

    // Pas de cotation le samedi ni le dimanche (v0.37) : les cours restent figés.
    for (i = 0; i < BOURSE_NB_ENTREPRISES; i++)
    {
        BOURSE.veille[i] = BOURSE.cours[i];
        if (!Calendrier_EstWeekend(jour))
            BOURSE.cours[i] = AppliquerPourMille(BOURSE.cours[i], Bourse_VariationDuJour(i, jour));
    }
    if (OR.magie == BOURSE_OR_MAGIE)
    {
        OR.veille = OR.cours;
        if (!Calendrier_EstWeekend(jour))
            OR.cours = AppliquerOr(OR.cours, Bourse_VariationOr(jour));
    }
    if (!Calendrier_EstWeekend(jour))
        HistoriqueAjouter(jour);
    if (BOURSE.dette)
    {
        u32 interets = (u64)BOURSE.dette * BOURSE_INTERET_POURMILLE / 1000;
        BOURSE.dette += interets ? interets : 1;
    }
    VerserDividende(jour);
}


// ---------------------------------------------------------------- historique (v0.38)

static u32 JourOuvrePrecedent(u32 jour)
{
    do
        jour--;
    while (jour > 0 && Calendrier_EstWeekend(jour));
    return jour;
}

static u32 CaseHisto(u32 age)
{
    return (HISTO.tete + BOURSE_HISTO_JOURS - age) % BOURSE_HISTO_JOURS;
}

// Reconstitue les 30 derniers jours ouvrés à partir des cours actuels, avec les variations du hasard fixe.
void Bourse_HistoriqueInitialiser(u32 jour)
{
    u32 e, age;

    while (jour > 0 && Calendrier_EstWeekend(jour))
        jour--;
    HISTO.magie = BOURSE_HISTO_MAGIE;
    HISTO.tete = BOURSE_HISTO_JOURS - 1;
    HISTO.jour = jour;
    for (e = 0; e < BOURSE_NB_ENTREPRISES; e++)
    {
        u32 cours = BOURSE.cours[e], j = jour;
        for (age = 0; age < BOURSE_HISTO_JOURS; age++)
        {
            HISTO.clotures[e][CaseHisto(age)] = cours;
            cours = Borner((u64)cours * 1000 / (1000 + Bourse_VariationDuJour(e, j)), BOURSE_COURS_MIN, BOURSE_COURS_MAX);
            j = JourOuvrePrecedent(j);
        }
    }
    OrHistoriqueReconstruire();
}

// Reconstitue l'historique de l'or à partir du cours actuel (même tampon que les entreprises).
static void OrHistoriqueReconstruire(void)
{
    u32 age, j = HISTO.jour, cours = OR.cours;

    if (HISTO.magie != BOURSE_HISTO_MAGIE || OR.magie != BOURSE_OR_MAGIE)
        return;
    for (age = 0; age < BOURSE_HISTO_JOURS; age++)
    {
        OR.clotures[CaseHisto(age)] = cours;
        cours = Borner((u64)cours * 1000 / (1000 + Bourse_VariationOr(j)), BOURSE_OR_MIN, BOURSE_OR_MAX);
        j = JourOuvrePrecedent(j);
    }    OR.veille = OR.clotures[CaseHisto(1)]; // la variation affichée suit la dernière bougie
}

static void OrInitialiser(void)
{
    memset(&OR, 0, sizeof(OR));
    OR.magie = BOURSE_OR_MAGIE;
    OR.cours = BOURSE_OR_DEPART;
    OR.veille = BOURSE_OR_DEPART;
    OrHistoriqueReconstruire();
}

static void HistoriqueAjouter(u32 jour)
{
    u32 e;

    if (HISTO.magie != BOURSE_HISTO_MAGIE)
        return;
    HISTO.tete = (HISTO.tete + 1) % BOURSE_HISTO_JOURS;
    HISTO.jour = jour;
    for (e = 0; e < BOURSE_NB_ENTREPRISES; e++)
        HISTO.clotures[e][HISTO.tete] = BOURSE.cours[e];
    if (OR.magie == BOURSE_OR_MAGIE)
        OR.clotures[HISTO.tete] = OR.cours;
}

// La dernière clôture suit le cours actuel (chocs du scénario compris).
static void HistoriqueSynchroniser(void)
{
    u32 e;

    if (HISTO.magie != BOURSE_HISTO_MAGIE)
        return;
    for (e = 0; e < BOURSE_NB_ENTREPRISES; e++)
        HISTO.clotures[e][HISTO.tete] = BOURSE.cours[e];
    if (OR.magie == BOURSE_OR_MAGIE)
        OR.clotures[HISTO.tete] = OR.cours;
}

u32 Bourse_HistoriqueCloture(u32 entreprise, u32 age)
{
    if (entreprise > BOURSE_OR || age >= BOURSE_HISTO_JOURS || HISTO.magie != BOURSE_HISTO_MAGIE)
        return 0;
    if (entreprise == BOURSE_OR)
        return OR.magie == BOURSE_OR_MAGIE ? OR.clotures[CaseHisto(age)] : 0;
    return HISTO.clotures[entreprise][CaseHisto(age)];
}

// Bougie d'âge 0 (aujourd'hui) à 29. Les mèches viennent du hasard fixe du jour : rien à sauvegarder en plus.
void Bourse_Bougie(u32 entreprise, u32 age, struct BourseBougie *b)
{
    u32 j = HISTO.jour, i, w, haut, bas;

    b->cloture = Bourse_HistoriqueCloture(entreprise, age);
    b->ouverture = Bourse_HistoriqueCloture(entreprise, age + 1);
    if (b->ouverture == 0)
        b->ouverture = b->cloture;
    for (i = 0; i < age; i++)
        j = JourOuvrePrecedent(j);
    w = 2 + Alea(j, entreprise, 9) % ((entreprise == BOURSE_OR ? 10 : sEntreprises[entreprise].volatilite) / 2 + 1);
    haut = max(b->ouverture, b->cloture);
    bas = min(b->ouverture, b->cloture);
    b->haut = Borner((u64)haut * (1000 + w) / 1000, 1, BOURSE_OR_MAX);
    b->bas = Borner((u64)bas * (1000 - w) / 1000, 1, BOURSE_OR_MAX);
}

void Bourse_AppliquerChocs(void)
{
    u32 i;

    for (i = 0; i < ARRAY_COUNT(sChocs); i++)
    {
        if ((BOURSE.evenements & (1u << i)) || !FlagGet(sChocs[i].drapeau))
            continue;
        BOURSE.evenements |= 1u << i;
        BOURSE.cours[sChocs[i].entreprise] = AppliquerPourMille(BOURSE.cours[sChocs[i].entreprise], sChocs[i].pourmille);
    }
}

u32 Bourse_ValeurPortefeuille(void)
{
    u64 total = 0;
    u32 i;

    for (i = 0; i < BOURSE_NB_ENTREPRISES; i++)
        total += (u64)BOURSE.parts[i] * BOURSE.cours[i];
    if (OR.magie == BOURSE_OR_MAGIE)
        total += (u64)OR.parts * OR.cours;
    return Borner(total, 0, 2000000000);
}

// L'huissier passe si la dette dépasse le portefeuille + 10 000 : il vend tout (avec une pénalité de 10 %),
// puis saisit l'argent liquide jusqu'à couvrir la dette.
void Bourse_VerifierHuissier(void)
{
    u32 valeur = Bourse_ValeurPortefeuille(), recupere, argent, i;

    if (BOURSE.dette == 0 || BOURSE.dette <= (u64)valeur + BOURSE_HUISSIER_MARGE)
        return;
    recupere = (u64)valeur * (100 - BOURSE_HUISSIER_PENALITE) / 100;
    for (i = 0; i < BOURSE_NB_ENTREPRISES; i++)
        BOURSE.parts[i] = 0;
    OR.parts = 0;
    if (recupere >= BOURSE.dette)
    {
        AddMoney(&gSaveBlock1Ptr->money, recupere - BOURSE.dette);
        recupere = BOURSE.dette;
        BOURSE.dette = 0;
    }
    else
    {
        BOURSE.dette -= recupere;
        argent = GetMoney(&gSaveBlock1Ptr->money);
        if (argent > BOURSE.dette)
            argent = BOURSE.dette;
        RemoveMoney(&gSaveBlock1Ptr->money, argent);
        BOURSE.dette -= argent;
        recupere += argent;
    }
    BOURSE.huissier += recupere;
}

void Bourse_MettreAJourJusqua(u32 aujourdhui)
{
    u32 jours;

    if (BOURSE.cours[0] == 0)
        Bourse_Initialiser(aujourdhui);
    if (OR.magie != BOURSE_OR_MAGIE)
        OrInitialiser();
    if (HISTO.magie != BOURSE_HISTO_MAGIE)
        Bourse_HistoriqueInitialiser(BOURSE.jour);
    if (aujourdhui < BOURSE.jour) // horloge remise à zéro
    {
        BOURSE.jour = aujourdhui;
        BOURSE.jourDividende = aujourdhui;
    }
    jours = aujourdhui - BOURSE.jour;
    if (jours > BOURSE_RATTRAPAGE_MAX)
        BOURSE.jour = aujourdhui - BOURSE_RATTRAPAGE_MAX;
    while (BOURSE.jour < aujourdhui)
    {
        BOURSE.jour++;
        Bourse_PasserJour(BOURSE.jour);
        Bourse_VerifierHuissier();
    }
    Bourse_AppliquerChocs();
    Bourse_VerifierHuissier();
    HistoriqueSynchroniser();
}

static u32 PrixAvecFrais(u32 entreprise, u32 quantite)
{
    u64 brut = (u64)quantite * Bourse_Cours(entreprise);
    return Borner(brut + brut * Frais(entreprise) / 100, 0, 0xFFFFFFFF);
}

u32 Bourse_Acheter(u32 entreprise, u32 quantite)
{
    u32 argent = GetMoney(&gSaveBlock1Ptr->money), possible;

    if (entreprise > BOURSE_OR || Bourse_Cours(entreprise) == 0)
        return 0;
    possible = (u64)argent * 100 / ((u64)Bourse_Cours(entreprise) * (100 + Frais(entreprise)));
    if (possible > BOURSE_PARTS_MAX - Bourse_Parts(entreprise))
        possible = BOURSE_PARTS_MAX - Bourse_Parts(entreprise);
    if (quantite > possible)
        quantite = possible;
    if (quantite == 0)
        return 0;
    RemoveMoney(&gSaveBlock1Ptr->money, PrixAvecFrais(entreprise, quantite));
    ChangerParts(entreprise, quantite);
    return quantite;
}

u32 Bourse_Vendre(u32 entreprise, u32 quantite)
{
    u32 place, cours, possible;
    u64 net;

    if (entreprise > BOURSE_OR || Bourse_Cours(entreprise) == 0)
        return 0;
    if (quantite > Bourse_Parts(entreprise))
        quantite = Bourse_Parts(entreprise);
    // On ne vend pas plus que ce que le portefeuille (MAX_MONEY) peut encaisser.
    place = MAX_MONEY - GetMoney(&gSaveBlock1Ptr->money);
    cours = Bourse_Cours(entreprise);
    possible = (u64)place * 100 / ((u64)cours * (100 - Frais(entreprise)));
    if (quantite > possible)
        quantite = possible;
    if (quantite == 0)
        return 0;
    net = (u64)quantite * cours * (100 - Frais(entreprise)) / 100;
    AddMoney(&gSaveBlock1Ptr->money, net);
    ChangerParts(entreprise, -(s32)quantite);
    return quantite;
}

u32 Bourse_PlafondCredit(void)
{
    return Borner((u64)BOURSE_CREDIT_BASE + Bourse_ValeurPortefeuille() / 2, 0, BOURSE_CREDIT_MAX);
}

u32 Bourse_Emprunter(u32 montant)
{
    u32 plafond = Bourse_PlafondCredit(), place = MAX_MONEY - GetMoney(&gSaveBlock1Ptr->money);

    if (BOURSE.dette >= plafond)
        return 0;
    if (montant > plafond - BOURSE.dette)
        montant = plafond - BOURSE.dette;
    if (montant > place)
        montant = place;
    if (montant == 0)
        return 0;
    BOURSE.dette += montant;
    AddMoney(&gSaveBlock1Ptr->money, montant);
    return montant;
}

u32 Bourse_Rembourser(void)
{
    u32 montant = GetMoney(&gSaveBlock1Ptr->money);

    if (montant > BOURSE.dette)
        montant = BOURSE.dette;
    RemoveMoney(&gSaveBlock1Ptr->money, montant);
    BOURSE.dette -= montant;
    return montant;
}

// ---------------------------------------------------------------- textes

static const u8 sText_Yens[] = _(" ¥");

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

static u8 *AjouterTexte(u8 *fin, const u8 *texte)
{
    return StringCopy(fin, texte);
}

static u8 *AjouterYens(u8 *fin, u32 n)
{
    fin = AjouterNombre(fin, n);
    return AjouterTexte(fin, sText_Yens);
}

// Variation depuis la veille, en pour cent arrondi, avec le signe.
static u8 *AjouterVariation(u8 *fin, u32 entreprise)
{
    s32 veille = Veille(entreprise) ? Veille(entreprise) : 1;
    s32 pm = ((s64)Bourse_Cours(entreprise) - veille) * 1000 / veille;
    s32 pc = pm / 10;
    *fin++ = pm < 0 ? CHAR_HYPHEN : CHAR_PLUS;
    fin = AjouterNombre(fin, pc < 0 ? -pc : pc);
    if (entreprise == BOURSE_OR) // l'or bouge peu : une décimale
    {
        *fin++ = CHAR_COMMA;
        fin = AjouterNombre(fin, (pm < 0 ? -pm : pm) % 10);
    }
    *fin++ = CHAR_SPACE;
    *fin++ = CHAR_PERCENT;
    *fin = EOS;
    return fin;
}

// En-tête du graphique : « Devon SA  612 ¥  +2 % ».
u8 *Bourse_TexteEntete(u8 *dest, u32 entreprise)
{
    if (entreprise > BOURSE_OR)
        entreprise = 0;
    dest = AjouterTexte(dest, Nom(entreprise));
    *dest++ = CHAR_SPACE;
    *dest++ = CHAR_SPACE;
    dest = AjouterYens(dest, Bourse_Cours(entreprise));
    *dest++ = CHAR_SPACE;
    *dest++ = CHAR_SPACE;
    return AjouterVariation(dest, entreprise);
}

static u32 Aujourdhui(void)
{
    return RtcGetLocalDayCount();
}

static const u8 sText_Huissier1[] = _("L'huissier est passé ! Ta dette\ndépassait tes placements.\pIl a tout vendu et saisi ");
static const u8 sText_Huissier2[] = _(".\nIl reste ");
static const u8 sText_Huissier3[] = _(" de dette.");

// Ouverture de l'appli : met les cours à jour. VAR_RESULT = TRUE si l'huissier est passé (texte dans gStringVar4).
void Bourse_Ouvrir(void)
{
    u8 *fin;

    Bourse_MettreAJourJusqua(Aujourdhui());
    gSpecialVar_Result = FALSE;
    if (BOURSE.huissier == 0)
        return;
    fin = AjouterTexte(gStringVar4, sText_Huissier1);
    fin = AjouterYens(fin, BOURSE.huissier);
    fin = AjouterTexte(fin, sText_Huissier2);
    fin = AjouterYens(fin, BOURSE.dette);
    AjouterTexte(fin, sText_Huissier3);
    BOURSE.huissier = 0;
    gSpecialVar_Result = TRUE;
}

static const u8 sText_Resume1[] = _("Liquide : ");
static const u8 sText_Resume2[] = _("\nPlacements : ");
static const u8 sText_Resume3[] = _("\pDette : ");
static const u8 sText_Resume4[] = _(". Bourse de Hoenn,\non fait quoi ?");
static const u8 sText_Resume4Weekend[] = _(". Week-end : cours\nfigés jusqu'à lundi. On fait quoi ?");

void Bourse_Resume(void)
{
    u8 *fin = AjouterTexte(gStringVar4, sText_Resume1);
    fin = AjouterYens(fin, GetMoney(&gSaveBlock1Ptr->money));
    fin = AjouterTexte(fin, sText_Resume2);
    fin = AjouterYens(fin, Bourse_ValeurPortefeuille());
    fin = AjouterTexte(fin, sText_Resume3);
    fin = AjouterYens(fin, BOURSE.dette);
    AjouterTexte(fin, Calendrier_EstWeekend(Aujourdhui()) ? sText_Resume4Weekend : sText_Resume4);
}

// Lignes de la liste du marché : « Devon SA 612 ¥ ↑ ».
void Bourse_PreparerListe(void)
{
    u32 i;

    for (i = 0; i < BOURSE_NB_ENTREPRISES; i++)
    {
        u8 *fin = AjouterTexte(gBourseLignes[i], sEntreprises[i].nom);
        *fin++ = CHAR_SPACE;
        fin = AjouterYens(fin, BOURSE.cours[i]);
        *fin++ = CHAR_SPACE;
        if (BOURSE.cours[i] > BOURSE.veille[i])
            *fin++ = CHAR_UP_ARROW;
        else if (BOURSE.cours[i] < BOURSE.veille[i])
            *fin++ = CHAR_DOWN_ARROW;
        else
            *fin++ = CHAR_EQUALS;
        *fin = EOS;
    }
    // v0.41 : l'or, ligne gBourseLignes[BOURSE_NB_ENTREPRISES + 1]
    {
        u8 *fin = AjouterTexte(gBourseLignes[BOURSE_NB_ENTREPRISES + 1], sText_Or);
        *fin++ = CHAR_SPACE;
        fin = AjouterYens(fin, OR.cours);
        *fin++ = CHAR_SPACE;
        *fin++ = OR.cours > OR.veille ? CHAR_UP_ARROW : (OR.cours < OR.veille ? CHAR_DOWN_ARROW : CHAR_EQUALS);
        *fin = EOS;
    }
}

static const u8 sText_Fiche1[] = _(" : ");
static const u8 sText_Fiche2[] = _("\nHier : ");
static const u8 sText_Fiche3[] = _(". Tu as ");
static const u8 sText_Fiche4[] = _(" parts.\pValeur : ");
static const u8 sText_Fiche5[] = _(". Frais : 3 %.\nOn fait quoi ?");
static const u8 sText_Fiche4Or[] = _(" onces.\pValeur : ");
static const u8 sText_Fiche5Or[] = _(". Frais : 1 %.\nValeur refuge. On fait quoi ?");
static const u8 sText_FicheFaillite[] = _(". En faillite, ça ne\nvaut plus rien. On fait quoi ?");

// VAR_0x8004 : entreprise.
void Bourse_Fiche(void)
{
    u32 e = gSpecialVar_0x8004;
    u8 *fin;

    if (e > BOURSE_OR)
        e = 0;
    fin = AjouterTexte(gStringVar4, Nom(e));
    fin = AjouterTexte(fin, sText_Fiche1);
    fin = AjouterYens(fin, Bourse_Cours(e));
    fin = AjouterTexte(fin, sText_Fiche2);
    fin = AjouterVariation(fin, e);
    fin = AjouterTexte(fin, sText_Fiche3);
    fin = AjouterNombre(fin, Bourse_Parts(e));
    fin = AjouterTexte(fin, e == BOURSE_OR ? sText_Fiche4Or : sText_Fiche4);
    fin = AjouterYens(fin, (u64)Bourse_Parts(e) * Bourse_Cours(e));
    AjouterTexte(fin, e == BOURSE_OR ? sText_Fiche5Or : (EnFaillite(e) ? sText_FicheFaillite : sText_Fiche5));
}

static const u16 sQuantites[] = {1, 10, 100, BOURSE_PARTS_MAX};

static const u8 sText_Achat1[] = _("Acheté : ");
static const u8 sText_Vente1[] = _("Vendu : ");
static const u8 sText_Ordre2[] = _(" parts de\n");
static const u8 sText_Ordre2Or[] = _(" onces\nd'");
static const u8 sText_Ordre3[] = _(".");
static const u8 sText_AchatRien[] = _("Pas assez de liquide, ou plafond de\n9999 parts atteint. Rien acheté.");
static const u8 sText_VenteRien[] = _("Rien à vendre, ou ton portefeuille\nest plein. Rien vendu.");

static void TexteOrdre(const u8 *debut, u32 quantite, u32 entreprise)
{
    u8 *fin = AjouterTexte(gStringVar4, debut);
    fin = AjouterNombre(fin, quantite);
    fin = AjouterTexte(fin, entreprise == BOURSE_OR ? sText_Ordre2Or : sText_Ordre2);
    fin = AjouterTexte(fin, Nom(entreprise));
    AjouterTexte(fin, sText_Ordre3);
}

// VAR_0x8004 : entreprise, VAR_0x8005 : 0 = 1 part, 1 = 10, 2 = 100, 3 = le maximum. Texte du résultat dans gStringVar4.
void Bourse_ScriptAcheter(void)
{
    u32 e = gSpecialVar_0x8004, n;

    n = Bourse_Acheter(e, sQuantites[gSpecialVar_0x8005 < ARRAY_COUNT(sQuantites) ? gSpecialVar_0x8005 : 0]);
    if (n)
        TexteOrdre(sText_Achat1, n, e);
    else
        StringCopy(gStringVar4, sText_AchatRien);
    gSpecialVar_Result = n != 0;
}

void Bourse_ScriptVendre(void)
{
    u32 e = gSpecialVar_0x8004, n;

    n = Bourse_Vendre(e, sQuantites[gSpecialVar_0x8005 < ARRAY_COUNT(sQuantites) ? gSpecialVar_0x8005 : 0]);
    if (n)
        TexteOrdre(sText_Vente1, n, e);
    else
        StringCopy(gStringVar4, sText_VenteRien);
    gSpecialVar_Result = n != 0;
}

static const u8 sText_Credit1[] = _("Dette : ");
static const u8 sText_Credit2[] = _("\nPlafond : ");
static const u8 sText_Credit3[] = _("\pIntérêts : 1 % par jour. Si ta dette\ndépasse tes placements, l'huissier\lvient tout saisir. Alors ?");

void Bourse_Credit(void)
{
    u8 *fin = AjouterTexte(gStringVar4, sText_Credit1);
    fin = AjouterYens(fin, BOURSE.dette);
    fin = AjouterTexte(fin, sText_Credit2);
    fin = AjouterYens(fin, Bourse_PlafondCredit());
    AjouterTexte(fin, sText_Credit3);
}

static const u32 sEmprunts[] = {5000, 20000, 50000};
static const u8 sText_Emprunt[] = _("Emprunté : ");
static const u8 sText_EmpruntFin[] = _(".\nLe compteur des intérêts tourne.");
static const u8 sText_EmpruntRien[] = _("Plafond atteint. Place plus, ou\nrembourse d'abord.");
static const u8 sText_Rembourse[] = _("Remboursé : ");
static const u8 sText_RembourseFin[] = _(".\nIl reste ");
static const u8 sText_RembourseRien[] = _("Rien à rembourser, ou pas un sou\nen poche.");

// VAR_0x8005 : 0 = 5 000, 1 = 20 000, 2 = 50 000.
void Bourse_ScriptEmprunter(void)
{
    u32 m = Bourse_Emprunter(sEmprunts[gSpecialVar_0x8005 < ARRAY_COUNT(sEmprunts) ? gSpecialVar_0x8005 : 0]);
    u8 *fin;

    if (m == 0)
    {
        StringCopy(gStringVar4, sText_EmpruntRien);
        return;
    }
    fin = AjouterTexte(gStringVar4, sText_Emprunt);
    fin = AjouterYens(fin, m);
    AjouterTexte(fin, sText_EmpruntFin);
}

void Bourse_ScriptRembourser(void)
{
    u32 m = Bourse_Rembourser();
    u8 *fin;

    if (m == 0)
    {
        StringCopy(gStringVar4, sText_RembourseRien);
        return;
    }
    fin = AjouterTexte(gStringVar4, sText_Rembourse);
    fin = AjouterYens(fin, m);
    fin = AjouterTexte(fin, sText_RembourseFin);
    fin = AjouterYens(fin, BOURSE.dette);
    AjouterTexte(fin, sText_Ordre3);
}

// Tuyau du jour : une rumeur sur le cours de demain d'une entreprise, vraie 7 fois sur 10.
static const u8 sText_TuyauHausse1[] = _("Un type louche au Centre Pokémon\njure que ");
static const u8 sText_TuyauHausse2[] = _("\lva s'envoler ");
static const u8 sText_TuyauHausse3[] = _(". Il a l'air sûr\lde lui.");
static const u8 sText_TuyauBaisse1[] = _("Rumeur à la télé : ");
static const u8 sText_TuyauBaisse2[] = _("\nva prendre une claque ");
static const u8 sText_TuyauBaisse3[] = _(".");
static const u8 sText_Demain[] = _("demain");
static const u8 sText_Lundi[] = _("lundi");
static const u8 sText_TuyauFin[] = _("\pÀ toi de voir si tu y crois.");

void Bourse_Tuyau(void)
{
    u32 jour = Aujourdhui(), e = Alea(jour, 77, 0) % BOURSE_NB_ENTREPRISES, seance = jour + 1;
    bool32 vrai = Alea(jour, 77, 1) % 100 < BOURSE_TUYAU_FIABLE, hausse;
    u8 *fin;

    while (Calendrier_EstWeekend(seance)) // prochaine séance : lundi après un week-end
        seance++;
    hausse = (Bourse_VariationDuJour(e, seance) >= 0) == vrai;
    fin = AjouterTexte(gStringVar4, hausse ? sText_TuyauHausse1 : sText_TuyauBaisse1);
    fin = AjouterTexte(fin, sEntreprises[e].nom);
    fin = AjouterTexte(fin, hausse ? sText_TuyauHausse2 : sText_TuyauBaisse2);
    fin = AjouterTexte(fin, seance == jour + 1 ? sText_Demain : sText_Lundi);
    fin = AjouterTexte(fin, hausse ? sText_TuyauHausse3 : sText_TuyauBaisse3);
    AjouterTexte(fin, sText_TuyauFin);
}
