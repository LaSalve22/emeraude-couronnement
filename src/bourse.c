#include "global.h"
#include "bourse.h"
#include "couronnement.h"
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

EWRAM_DATA u8 gBourseLignes[BOURSE_NB_ENTREPRISES][32] = {0};

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

static void VerserDividende(u32 jour)
{
    u32 dividende;

    if (jour < BOURSE.jourDividende + 7)
        return;
    BOURSE.jourDividende = jour;
    dividende = (u64)BOURSE.parts[BOURSE_MAGASINS] * BOURSE.cours[BOURSE_MAGASINS] * BOURSE_DIVIDENDE_POURMILLE / 1000;
    if (dividende)
        AddMoney(&gSaveBlock1Ptr->money, dividende);
}

void Bourse_PasserJour(u32 jour)
{
    u32 i;

    for (i = 0; i < BOURSE_NB_ENTREPRISES; i++)
    {
        BOURSE.veille[i] = BOURSE.cours[i];
        BOURSE.cours[i] = AppliquerPourMille(BOURSE.cours[i], Bourse_VariationDuJour(i, jour));
    }
    if (BOURSE.dette)
    {
        u32 interets = (u64)BOURSE.dette * BOURSE_INTERET_POURMILLE / 1000;
        BOURSE.dette += interets ? interets : 1;
    }
    VerserDividende(jour);
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
    return Borner(total, 0, 99999999);
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
}

static u32 PrixAvecFrais(u32 entreprise, u32 quantite)
{
    u64 brut = (u64)quantite * BOURSE.cours[entreprise];
    return Borner(brut + brut * BOURSE_FRAIS_POURCENT / 100, 0, 0xFFFFFFFF);
}

u32 Bourse_Acheter(u32 entreprise, u32 quantite)
{
    u32 argent = GetMoney(&gSaveBlock1Ptr->money), possible;

    if (entreprise >= BOURSE_NB_ENTREPRISES)
        return 0;
    possible = (u64)argent * 100 / ((u64)BOURSE.cours[entreprise] * (100 + BOURSE_FRAIS_POURCENT));
    if (possible > BOURSE_PARTS_MAX - BOURSE.parts[entreprise])
        possible = BOURSE_PARTS_MAX - BOURSE.parts[entreprise];
    if (quantite > possible)
        quantite = possible;
    if (quantite == 0)
        return 0;
    RemoveMoney(&gSaveBlock1Ptr->money, PrixAvecFrais(entreprise, quantite));
    BOURSE.parts[entreprise] += quantite;
    return quantite;
}

u32 Bourse_Vendre(u32 entreprise, u32 quantite)
{
    u32 place, cours, possible;
    u64 net;

    if (entreprise >= BOURSE_NB_ENTREPRISES)
        return 0;
    if (quantite > BOURSE.parts[entreprise])
        quantite = BOURSE.parts[entreprise];
    // On ne vend pas plus que ce que le portefeuille (999 999 ¥ max) peut encaisser.
    place = MAX_MONEY - GetMoney(&gSaveBlock1Ptr->money);
    cours = BOURSE.cours[entreprise];
    possible = (u64)place * 100 / ((u64)cours * (100 - BOURSE_FRAIS_POURCENT));
    if (quantite > possible)
        quantite = possible;
    if (quantite == 0)
        return 0;
    net = (u64)quantite * cours * (100 - BOURSE_FRAIS_POURCENT) / 100;
    AddMoney(&gSaveBlock1Ptr->money, net);
    BOURSE.parts[entreprise] -= quantite;
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
    u32 chiffres = 1, p = 10;
    while (n >= p && chiffres < 9)
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
    s32 veille = BOURSE.veille[entreprise] ? BOURSE.veille[entreprise] : 1;
    s32 pc = ((s32)BOURSE.cours[entreprise] - veille) * 100 / veille;
    *fin++ = pc < 0 ? CHAR_HYPHEN : CHAR_PLUS;
    fin = AjouterNombre(fin, pc < 0 ? -pc : pc);
    *fin++ = CHAR_SPACE;
    *fin++ = CHAR_PERCENT;
    *fin = EOS;
    return fin;
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

void Bourse_Resume(void)
{
    u8 *fin = AjouterTexte(gStringVar4, sText_Resume1);
    fin = AjouterYens(fin, GetMoney(&gSaveBlock1Ptr->money));
    fin = AjouterTexte(fin, sText_Resume2);
    fin = AjouterYens(fin, Bourse_ValeurPortefeuille());
    fin = AjouterTexte(fin, sText_Resume3);
    fin = AjouterYens(fin, BOURSE.dette);
    AjouterTexte(fin, sText_Resume4);
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
}

static const u8 sText_Fiche1[] = _(" : ");
static const u8 sText_Fiche2[] = _("\nHier : ");
static const u8 sText_Fiche3[] = _(". Tu as ");
static const u8 sText_Fiche4[] = _(" parts.\pValeur : ");
static const u8 sText_Fiche5[] = _(". Frais : 3 %.\nOn fait quoi ?");
static const u8 sText_FicheFaillite[] = _(". En faillite, ça ne\nvaut plus rien. On fait quoi ?");

// VAR_0x8004 : entreprise.
void Bourse_Fiche(void)
{
    u32 e = gSpecialVar_0x8004;
    u8 *fin;

    if (e >= BOURSE_NB_ENTREPRISES)
        e = 0;
    fin = AjouterTexte(gStringVar4, sEntreprises[e].nom);
    fin = AjouterTexte(fin, sText_Fiche1);
    fin = AjouterYens(fin, BOURSE.cours[e]);
    fin = AjouterTexte(fin, sText_Fiche2);
    fin = AjouterVariation(fin, e);
    fin = AjouterTexte(fin, sText_Fiche3);
    fin = AjouterNombre(fin, BOURSE.parts[e]);
    fin = AjouterTexte(fin, sText_Fiche4);
    fin = AjouterYens(fin, (u64)BOURSE.parts[e] * BOURSE.cours[e]);
    AjouterTexte(fin, EnFaillite(e) ? sText_FicheFaillite : sText_Fiche5);
}

static const u16 sQuantites[] = {1, 10, 100, BOURSE_PARTS_MAX};

static const u8 sText_Achat1[] = _("Acheté : ");
static const u8 sText_Vente1[] = _("Vendu : ");
static const u8 sText_Ordre2[] = _(" parts de\n");
static const u8 sText_Ordre3[] = _(".");
static const u8 sText_AchatRien[] = _("Pas assez de liquide, ou plafond de\n9999 parts atteint. Rien acheté.");
static const u8 sText_VenteRien[] = _("Rien à vendre, ou ton portefeuille\nest plein (999 999 ¥). Rien vendu.");

static void TexteOrdre(const u8 *debut, u32 quantite, u32 entreprise)
{
    u8 *fin = AjouterTexte(gStringVar4, debut);
    fin = AjouterNombre(fin, quantite);
    fin = AjouterTexte(fin, sText_Ordre2);
    fin = AjouterTexte(fin, sEntreprises[entreprise].nom);
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
static const u8 sText_TuyauHausse2[] = _("\lva s'envoler demain. Il a l'air sûr\lde lui.");
static const u8 sText_TuyauBaisse1[] = _("Rumeur à la télé : ");
static const u8 sText_TuyauBaisse2[] = _("\nva prendre une claque demain.");
static const u8 sText_TuyauFin[] = _("\pÀ toi de voir si tu y crois.");

void Bourse_Tuyau(void)
{
    u32 jour = Aujourdhui(), e = Alea(jour, 77, 0) % BOURSE_NB_ENTREPRISES;
    bool32 vrai = Alea(jour, 77, 1) % 100 < BOURSE_TUYAU_FIABLE;
    bool32 hausse = (Bourse_VariationDuJour(e, jour + 1) >= 0) == vrai;
    u8 *fin;

    fin = AjouterTexte(gStringVar4, hausse ? sText_TuyauHausse1 : sText_TuyauBaisse1);
    fin = AjouterTexte(fin, sEntreprises[e].nom);
    fin = AjouterTexte(fin, hausse ? sText_TuyauHausse2 : sText_TuyauBaisse2);
    AjouterTexte(fin, sText_TuyauFin);
}
