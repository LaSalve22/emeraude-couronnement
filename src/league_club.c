#include "global.h"
#include "league_club.h"
#include "battle.h"
#include "caps.h"
#include "data.h"
#include "event_data.h"
#include "item.h"
#include "money.h"
#include "pokemon.h"
#include "rtc.h"
#include "script_pokemon_util.h"
#include "string_util.h"
#include "constants/battle_ai.h"
#include "constants/characters.h"
#include "constants/flags.h"
#include "constants/items.h"
#include "constants/rgb.h"
#include "constants/species.h"
#include "constants/trainers.h"
#include "constants/vars.h"

// Émeraude Couronnement : Ligue des Clubs (v0.42). Voir include/league_club.h.
// Rien n'est rejoué : tout ce qui est simulé l'est le jour venu, enregistré dans la sauvegarde (4 bits par duel), et le hasard
// est déterministe (n° de Dresseur + saison + match + duel). Horloge remise en arrière : aucun jour n'est retraité.

#define LC (gSaveBlock3Ptr->ligueClubs)

STATIC_ASSERT(CLUB_LIGUE_NB_FORME == CLUB_NB * CLUB_DRESSEURS, ClubLigueForme);
STATIC_ASSERT(CLUB_LIGUE_NB_DUELS == CLUB_NB_MATCHS + 1, ClubLigueDuels);

enum
{
    CLUB_DRAP_FINALE_CONCLUE = 1 << 0,  // finale de la saison terminée (vainqueur connu, primes versées)
};

enum
{
    CLUB_RAPPORT_RENOUVELEE = 1 << 0,
    CLUB_RAPPORT_PERDUE     = 1 << 1,
    CLUB_RAPPORT_FORFAIT    = 1 << 2,
    CLUB_RAPPORT_FINALISTE  = 1 << 3,
    CLUB_RAPPORT_CHAMPION   = 1 << 4,
    CLUB_RAPPORT_SAISON     = 1 << 5,
};

struct ClubInfo
{
    const u8 *nom;
    const u8 *court;
    const u8 *stade;
    u8 tier;
    u8 effet;
    u16 couleur;
    struct ClubDresseur d[CLUB_DRESSEURS];
};

static const struct ClubInfo sClubs[CLUB_NB] =
{
    [0] = {
        COMPOUND_STRING("AC Clémenti"), COMPOUND_STRING("Clémenti"), COMPOUND_STRING("Arène des Pétales"),
        CLUB_TIER_C, CLUB_STADE_HERBU, RGB(6, 20, 6),
        {
            {COMPOUND_STRING("Sylvain"), TRAINER_CLASS_PKMN_RANGER, TRAINER_PIC_POKEMON_RANGER_M, FALSE, 58,
             {SPECIES_BULBASAUR, SPECIES_TURTWIG, SPECIES_SHROOMISH, SPECIES_SEEDOT, SPECIES_TANGELA, SPECIES_SKIDDO}},
            {COMPOUND_STRING("Rose"), TRAINER_CLASS_AROMA_LADY, TRAINER_PIC_AROMA_LADY, TRUE, 52,
             {SPECIES_ODDISH, SPECIES_HOPPIP, SPECIES_SUNKERN, SPECIES_BUDEW, SPECIES_PETILIL, SPECIES_BOUNSWEET}},
            {COMPOUND_STRING("Basile"), TRAINER_CLASS_CAMPER, TRAINER_PIC_CAMPER, FALSE, 47,
             {SPECIES_LOTAD, SPECIES_CACNEA, SPECIES_SEWADDLE, SPECIES_FOONGUS, SPECIES_PHANTUMP, SPECIES_BELLSPROUT}},
            {COMPOUND_STRING("Iris"), TRAINER_CLASS_PICNICKER, TRAINER_PIC_PICNICKER, TRUE, 42,
             {SPECIES_CHIKORITA, SPECIES_SNOVER, SPECIES_DEERLING, SPECIES_PUMPKABOO, SPECIES_FOMANTIS, SPECIES_SMOLIV}},
        },
    },
    [1] = {
        COMPOUND_STRING("AS Autéquia"), COMPOUND_STRING("Autéquia"), COMPOUND_STRING("Cratère des Météores"),
        CLUB_TIER_C, CLUB_STADE_GRAVITE, RGB(14, 10, 20),
        {
            {COMPOUND_STRING("Gaston"), TRAINER_CLASS_RUIN_MANIAC, TRAINER_PIC_RUIN_MANIAC, FALSE, 58,
             {SPECIES_LUNATONE, SPECIES_SOLROCK, SPECIES_BALTOY, SPECIES_BRONZOR, SPECIES_NOSEPASS, SPECIES_ROGGENROLA}},
            {COMPOUND_STRING("Céleste"), TRAINER_CLASS_PSYCHIC, TRAINER_PIC_PSYCHIC_F, TRUE, 52,
             {SPECIES_NATU, SPECIES_SPOINK, SPECIES_ELGYEM, SPECIES_SOLOSIS, SPECIES_CHINGLING, SPECIES_GOTHITA}},
            {COMPOUND_STRING("Bruno"), TRAINER_CLASS_HIKER, TRAINER_PIC_HIKER, FALSE, 47,
             {SPECIES_GEODUDE, SPECIES_ONIX, SPECIES_ARON, SPECIES_SANDSHREW, SPECIES_DRILBUR, SPECIES_RHYHORN}},
            {COMPOUND_STRING("Lune"), TRAINER_CLASS_LASS, TRAINER_PIC_LASS, TRUE, 42,
             {SPECIES_ABRA, SPECIES_DROWZEE, SPECIES_CARBINK, SPECIES_MEDITITE, SPECIES_RALTS, SPECIES_CLEFAIRY}},
        },
    },
    [2] = {
        COMPOUND_STRING("SC Vermilava"), COMPOUND_STRING("Vermilava"), COMPOUND_STRING("Caldeira Thermale"),
        CLUB_TIER_B, CLUB_STADE_SOLEIL, RGB(26, 8, 4),
        {
            {COMPOUND_STRING("Ardent"), TRAINER_CLASS_KINDLER, TRAINER_PIC_KINDLER, FALSE, 68,
             {SPECIES_CHARMANDER, SPECIES_GROWLITHE, SPECIES_PONYTA, SPECIES_NUMEL, SPECIES_LITWICK, SPECIES_TORKOAL}},
            {COMPOUND_STRING("Flamme"), TRAINER_CLASS_BEAUTY, TRAINER_PIC_BEAUTY, TRUE, 62,
             {SPECIES_VULPIX, SPECIES_FLETCHLING, SPECIES_LITLEO, SPECIES_HOUNDOUR, SPECIES_SLUGMA, SPECIES_SALANDIT}},
            {COMPOUND_STRING("Pyro"), TRAINER_CLASS_KINDLER, TRAINER_PIC_KINDLER, FALSE, 57,
             {SPECIES_CYNDAQUIL, SPECIES_TORCHIC, SPECIES_TEPIG, SPECIES_FENNEKIN, SPECIES_ROLYCOLY, SPECIES_SIZZLIPEDE}},
            {COMPOUND_STRING("Cendre"), TRAINER_CLASS_PICNICKER, TRAINER_PIC_PICNICKER, TRUE, 52,
             {SPECIES_HEATMOR, SPECIES_DARUMAKA, SPECIES_CHARCADET, SPECIES_LARVESTA, SPECIES_TURTONATOR, SPECIES_PANSEAR}},
        },
    },
    [3] = {
        COMPOUND_STRING("Aigles de Cimetronelle"), COMPOUND_STRING("Cimetron."), COMPOUND_STRING("Canopée Suspendue"),
        CLUB_TIER_B, CLUB_STADE_VENT, RGB(6, 14, 26),
        {
            {COMPOUND_STRING("Aquila"), TRAINER_CLASS_BIRD_KEEPER, TRAINER_PIC_BIRD_KEEPER, FALSE, 68,
             {SPECIES_PIDGEY, SPECIES_TAILLOW, SPECIES_STARLY, SPECIES_SKARMORY, SPECIES_SWABLU, SPECIES_ROOKIDEE}},
            {COMPOUND_STRING("Brise"), TRAINER_CLASS_PARASOL_LADY, TRAINER_PIC_PARASOL_LADY, TRUE, 62,
             {SPECIES_HOOTHOOT, SPECIES_SPEAROW, SPECIES_PIDOVE, SPECIES_RUFFLET, SPECIES_NOIBAT, SPECIES_TROPIUS}},
            {COMPOUND_STRING("Faucon"), TRAINER_CLASS_BIRD_KEEPER, TRAINER_PIC_BIRD_KEEPER, FALSE, 57,
             {SPECIES_DODUO, SPECIES_MURKROW, SPECIES_WINGULL, SPECIES_DUCKLETT, SPECIES_PIKIPEK, SPECIES_WATTREL}},
            {COMPOUND_STRING("Zéphyr"), TRAINER_CLASS_YOUNGSTER, TRAINER_PIC_YOUNGSTER, FALSE, 52,
             {SPECIES_ZUBAT, SPECIES_GLIGAR, SPECIES_YANMA, SPECIES_HAWLUCHA, SPECIES_CHATOT, SPECIES_NATU}},
        },
    },
    [4] = {
        COMPOUND_STRING("Olympique Mérouville"), COMPOUND_STRING("Mérouville"), COMPOUND_STRING("Stade Minéral Devon"),
        CLUB_TIER_B, CLUB_STADE_SABLE, RGB(22, 16, 8),
        {
            {COMPOUND_STRING("Pierrick"), TRAINER_CLASS_HIKER, TRAINER_PIC_HIKER, FALSE, 68,
             {SPECIES_LARVITAR, SPECIES_ARON, SPECIES_SANDSHREW, SPECIES_HIPPOPOTAS, SPECIES_GIBLE, SPECIES_NACLI}},
            {COMPOUND_STRING("Opale"), TRAINER_CLASS_COOLTRAINER, TRAINER_PIC_COOLTRAINER_F, TRUE, 62,
             {SPECIES_CRANIDOS, SPECIES_SHIELDON, SPECIES_TYRUNT, SPECIES_TRAPINCH, SPECIES_DIGLETT, SPECIES_SILICOBRA}},
            {COMPOUND_STRING("Gravier"), TRAINER_CLASS_RUIN_MANIAC, TRAINER_PIC_RUIN_MANIAC, FALSE, 57,
             {SPECIES_GEODUDE, SPECIES_ONIX, SPECIES_MAGNEMITE, SPECIES_BELDUM, SPECIES_DRILBUR, SPECIES_KLINK}},
            {COMPOUND_STRING("Sablé"), TRAINER_CLASS_CAMPER, TRAINER_PIC_CAMPER, FALSE, 52,
             {SPECIES_SANDILE, SPECIES_CUBONE, SPECIES_PHANPY, SPECIES_BARBOACH, SPECIES_RHYHORN, SPECIES_BINACLE}},
        },
    },
    [5] = {
        COMPOUND_STRING("Dynamo Lavandia"), COMPOUND_STRING("Lavandia"), COMPOUND_STRING("Circuit Volt-Ampère"),
        CLUB_TIER_B, CLUB_STADE_ELECTRIQUE, RGB(28, 24, 2),
        {
            {COMPOUND_STRING("Volt"), TRAINER_CLASS_GUITARIST, TRAINER_PIC_GUITARIST, FALSE, 68,
             {SPECIES_PIKACHU, SPECIES_ELECTRIKE, SPECIES_MAGNEMITE, SPECIES_SHINX, SPECIES_ROTOM, SPECIES_JOLTIK}},
            {COMPOUND_STRING("Watt"), TRAINER_CLASS_COOLTRAINER, TRAINER_PIC_COOLTRAINER_M, FALSE, 62,
             {SPECIES_MAREEP, SPECIES_VOLTORB, SPECIES_ELECTABUZZ, SPECIES_BLITZLE, SPECIES_TYNAMO, SPECIES_GRUBBIN}},
            {COMPOUND_STRING("Ohm"), TRAINER_CLASS_GUITARIST, TRAINER_PIC_GUITARIST, FALSE, 57,
             {SPECIES_PLUSLE, SPECIES_MINUN, SPECIES_EMOLGA, SPECIES_STUNFISK, SPECIES_HELIOPTILE, SPECIES_TOXEL}},
            {COMPOUND_STRING("Pila"), TRAINER_CLASS_LASS, TRAINER_PIC_LASS, TRUE, 52,
             {SPECIES_PACHIRISU, SPECIES_DEDENNE, SPECIES_TOGEDEMARU, SPECIES_PAWMI, SPECIES_YAMPER, SPECIES_CHINCHOU}},
        },
    },
    [6] = {
        COMPOUND_STRING("FC Poivressel"), COMPOUND_STRING("Poivressel"), COMPOUND_STRING("Dôme Maritime"),
        CLUB_TIER_A, CLUB_STADE_PLUIE, RGB(4, 12, 28),
        {
            {COMPOUND_STRING("Marin"), TRAINER_CLASS_SAILOR, TRAINER_PIC_SAILOR, FALSE, 76,
             {SPECIES_MAGIKARP, SPECIES_MUDKIP, SPECIES_HORSEA, SPECIES_CARVANHA, SPECIES_LAPRAS, SPECIES_CORPHISH}},
            {COMPOUND_STRING("Perle"), TRAINER_CLASS_SWIMMER_F, TRAINER_PIC_SWIMMER_F, TRUE, 71,
             {SPECIES_STARYU, SPECIES_SHELLDER, SPECIES_CHINCHOU, SPECIES_LOTAD, SPECIES_MARILL, SPECIES_BUIZEL}},
            {COMPOUND_STRING("Corail"), TRAINER_CLASS_SWIMMER_M, TRAINER_PIC_SWIMMER_M, FALSE, 66,
             {SPECIES_TENTACOOL, SPECIES_POLIWAG, SPECIES_KRABBY, SPECIES_WOOPER, SPECIES_TYMPOLE, SPECIES_WIMPOD}},
            {COMPOUND_STRING("Loïc"), TRAINER_CLASS_FISHERMAN, TRAINER_PIC_FISHERMAN, FALSE, 60,
             {SPECIES_PSYDUCK, SPECIES_SEEL, SPECIES_SLOWPOKE, SPECIES_BARBOACH, SPECIES_FRILLISH, SPECIES_CHEWTLE}},
        },
    },
    [7] = {
        COMPOUND_STRING("Cosmos Algatia"), COMPOUND_STRING("Algatia"), COMPOUND_STRING("Observatoire Orbital"),
        CLUB_TIER_A, CLUB_STADE_DISTORSION, RGB(16, 4, 22),
        {
            {COMPOUND_STRING("Orion"), TRAINER_CLASS_PSYCHIC, TRAINER_PIC_PSYCHIC_M, FALSE, 76,
             {SPECIES_BELDUM, SPECIES_SLOWPOKE, SPECIES_BRONZOR, SPECIES_DUSKULL, SPECIES_SNORLAX, SPECIES_LITWICK}},
            {COMPOUND_STRING("Véga"), TRAINER_CLASS_PSYCHIC, TRAINER_PIC_PSYCHIC_F, TRUE, 71,
             {SPECIES_ELGYEM, SPECIES_SOLOSIS, SPECIES_GOTHITA, SPECIES_HATENNA, SPECIES_BALTOY, SPECIES_YAMASK}},
            {COMPOUND_STRING("Comète"), TRAINER_CLASS_EXPERT, TRAINER_PIC_EXPERT_M, FALSE, 66,
             {SPECIES_ARON, SPECIES_FERROSEED, SPECIES_MAGNEMITE, SPECIES_HONEDGE, SPECIES_KLEFKI, SPECIES_CUFANT}},
            {COMPOUND_STRING("Astre"), TRAINER_CLASS_HEX_MANIAC, TRAINER_PIC_HEX_MANIAC, TRUE, 60,
             {SPECIES_DUSKULL, SPECIES_SHUPPET, SPECIES_SABLEYE, SPECIES_SPIRITOMB, SPECIES_GOLETT, SPECIES_SINISTEA}},
        },
    },
    [8] = {
        COMPOUND_STRING("Stade Nénucrique"), COMPOUND_STRING("Nénucrique"), COMPOUND_STRING("Palais des Arts"),
        CLUB_TIER_S, CLUB_STADE_PSYCHIQUE, RGB(28, 10, 20),
        {
            {COMPOUND_STRING("Apollon"), TRAINER_CLASS_GENTLEMAN, TRAINER_PIC_GENTLEMAN, FALSE, 88,
             {SPECIES_RALTS, SPECIES_ABRA, SPECIES_TOGEPI, SPECIES_SNUBBULL, SPECIES_SPRITZEE, SPECIES_FLITTLE}},
            {COMPOUND_STRING("Célia"), TRAINER_CLASS_BEAUTY, TRAINER_PIC_BEAUTY, TRUE, 82,
             {SPECIES_CLEFAIRY, SPECIES_JIGGLYPUFF, SPECIES_SWIRLIX, SPECIES_COMFEY, SPECIES_MAWILE, SPECIES_IMPIDIMP}},
            {COMPOUND_STRING("Orphée"), TRAINER_CLASS_PSYCHIC, TRAINER_PIC_PSYCHIC_M, FALSE, 77,
             {SPECIES_NATU, SPECIES_SPOINK, SPECIES_MEDITITE, SPECIES_MUNNA, SPECIES_DROWZEE, SPECIES_WOOBAT}},
            {COMPOUND_STRING("Muse"), TRAINER_CLASS_LADY, TRAINER_PIC_LADY, TRUE, 70,
             {SPECIES_CUTIEFLY, SPECIES_CHINGLING, SPECIES_GIRAFARIG, SPECIES_ESPURR, SPECIES_FLITTLE, SPECIES_TINKATINK}},
        },
    },
    [9] = {
        COMPOUND_STRING("Royale Atalanopolis"), COMPOUND_STRING("Atalanop."), COMPOUND_STRING("Colisée du Gouffre"),
        CLUB_TIER_S, CLUB_STADE_NEIGE, RGB(10, 24, 28),
        {
            {COMPOUND_STRING("Borée"), TRAINER_CLASS_EXPERT, TRAINER_PIC_EXPERT_M, FALSE, 88,
             {SPECIES_SNORUNT, SPECIES_SPHEAL, SPECIES_SWINUB, SPECIES_SNOVER, SPECIES_BERGMITE, SPECIES_FRIGIBAX}},
            {COMPOUND_STRING("Givre"), TRAINER_CLASS_COOLTRAINER, TRAINER_PIC_COOLTRAINER_F, TRUE, 82,
             {SPECIES_LAPRAS, SPECIES_SNEASEL, SPECIES_VANILLITE, SPECIES_CUBCHOO, SPECIES_CRYOGONAL, SPECIES_SNOM}},
            {COMPOUND_STRING("Frimas"), TRAINER_CLASS_SAILOR, TRAINER_PIC_SAILOR, FALSE, 77,
             {SPECIES_SEEL, SPECIES_SHELLDER, SPECIES_JYNX, SPECIES_DELIBIRD, SPECIES_AMAURA, SPECIES_CETODDLE}},
            {COMPOUND_STRING("Neva"), TRAINER_CLASS_LASS, TRAINER_PIC_LASS, TRUE, 70,
             {SPECIES_SPHEAL, SPECIES_SNORUNT, SPECIES_SNEASEL, SPECIES_BERGMITE, SPECIES_SEEL, SPECIES_CRYOGONAL}},
        },
    },
};

// Par tier C / B / A / S.
static const u32 sPrixLicence[CLUB_NB_TIERS] = {4000, 8000, 15000, 25000};
static const u32 sPrimeBase[CLUB_NB_TIERS] = {800, 1200, 1800, 2500};
static const u32 sPrimeParPoint[CLUB_NB_TIERS] = {400, 600, 900, 1200};
static const u8 sEvsTier[CLUB_NB_TIERS][NUM_STATS] =
{
    {0, 0, 0, 0, 0, 0},
    {40, 40, 40, 40, 40, 40},
    {64, 64, 64, 64, 64, 64},
    {84, 84, 84, 84, 84, 84},
};

// Jours de la saison où se jouent les 10 journées (tous les 2 ou 3 jours) ; finale le jour 30.
static const u8 sJoursJournees[CLUB_NB_JOURNEES] = {3, 5, 8, 11, 13, 16, 19, 21, 24, 27};

static EWRAM_DATA struct Trainer sAdversaire = {0};
static EWRAM_DATA struct TrainerMon sAdversaireEquipe[CLUB_TAILLE_EQUIPE] = {0};
static EWRAM_DATA bool8 sCombat = FALSE;
static EWRAM_DATA u8 sMatchCombat = 0;
static EWRAM_DATA u8 sPretPrevu = 0;

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

static u32 Alea(u32 a, u32 b, u32 c)
{
    const u8 *id = gSaveBlock2Ptr->playerTrainerId;
    u32 graine = id[0] | (id[1] << 8) | (id[2] << 16) | (id[3] << 24);
    return Melanger(graine ^ 0xC1B5C1B5 ^ (a * 0x9E3779B1u) ^ Melanger(b * 0x85EBCA6Bu + c));
}

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

static void Verifier(u32 jour)
{
    if (LC.magie == CLUB_MAGIE)
        return;
    memset(&LC, 0, sizeof(LC));
    LC.magie = CLUB_MAGIE;
    LC.jourDebut = jour;
    LC.jourTraite = jour - 1;
}

static void ValiderMatch(u32 *match)
{
    if (*match > CLUB_MATCH_FINALE)
        *match = CLUB_MATCH_FINALE;
}

// ---------------------------------------------------------------- barème « modèle 1 »

void CalculateDuelPoints(u8 homeSurvivors, u8 awaySurvivors, u8 *outHomePts, u8 *outAwayPts)
{
    if (homeSurvivors == 0 && awaySurvivors == 0) // double K.O. au dernier tour
    {
        *outHomePts = 2;
        *outAwayPts = 2;
    }
    else if (awaySurvivors == 0)
    {
        *outHomePts = 4 + (homeSurvivors == CLUB_TAILLE_EQUIPE ? 1 : 0); // bonus offensif : 6-0
        *outAwayPts = (homeSurvivors == 1 ? 1 : 0);                      // bonus défensif : 5-6
    }
    else
    {
        *outAwayPts = 4 + (awaySurvivors == CLUB_TAILLE_EQUIPE ? 1 : 0);
        *outHomePts = (awaySurvivors == 1 ? 1 : 0);
    }
}

static void Survivants(u32 code, u8 *domicile, u8 *exterieur)
{
    *domicile = *exterieur = 0;
    if (code >= 1 && code <= 6)
        *domicile = code;
    else if (code >= 7 && code <= 12)
        *exterieur = code - 6;
}

u32 Club_PointsDuel(u32 code, bool32 domicile)
{
    u8 sd, se, pd, pe;
    if (code == CLUB_DUEL_PAS_JOUE || code > CLUB_DUEL_NUL)
        return 0;
    Survivants(code, &sd, &se);
    CalculateDuelPoints(sd, se, &pd, &pe);
    return domicile ? pd : pe;
}

u32 Club_KOInfliges(u32 code, bool32 domicile)
{
    u8 sd, se;
    if (code == CLUB_DUEL_PAS_JOUE || code > CLUB_DUEL_NUL)
        return 0;
    if (code == CLUB_DUEL_NUL)
        return CLUB_TAILLE_EQUIPE;
    Survivants(code, &sd, &se);
    return CLUB_TAILLE_EQUIPE - (domicile ? se : sd);
}

static bool32 EstBonusOffensif(u32 code, bool32 domicile)
{
    return domicile ? code == CLUB_DUEL_DOMICILE(6) : code == CLUB_DUEL_EXTERIEUR(6);
}

// ---------------------------------------------------------------- calendrier et tirage

u32 Club_Saison(void)
{
    return LC.saison;
}

u32 Club_JourDeSaison(void)
{
    if (LC.magie != CLUB_MAGIE)
        return 1;
    return ((u16)(LC.jourTraite - LC.jourDebut)) % CLUB_JOURS_SAISON + 1;
}

u32 Club_JourDeJournee(u32 journee)
{
    return sJoursJournees[journee % CLUB_NB_JOURNEES] * CLUB_ECHELLE;
}

void Club_Tirage(u32 saison, u8 *clubs)
{
    u32 i;
    for (i = 0; i < CLUB_NB; i++)
        clubs[i] = i;
    for (i = CLUB_NB - 1; i > 0; i--)
    {
        u32 j = Alea(saison, 1000 + i, 7) % (i + 1);
        u8 t = clubs[i];
        clubs[i] = clubs[j];
        clubs[j] = t;
    }
}

u32 Club_Poule(u32 club)
{
    u8 clubs[CLUB_NB];
    u32 i;
    Club_Tirage(LC.saison, clubs);
    for (i = 0; i < CLUB_NB; i++)
        if (clubs[i] == club)
            return i / CLUB_PAR_POULE;
    return 0;
}

// Méthode du cercle : à la ronde r (0-4), la position r est exempte ; (r+1, r+4) et (r+2, r+3) s'affrontent.
// Journées 5 à 9 : matchs retour, domicile et extérieur inversés. Chaque club reçoit 4 fois.
static void AfficheDePoule(u32 match, u8 *domicile, u8 *exterieur)
{
    u8 clubs[CLUB_NB];
    u32 journee = match / CLUB_MATCHS_PAR_JOURNEE, poule = (match % CLUB_MATCHS_PAR_JOURNEE) / 2, k = match % 2;
    u32 r = journee % CLUB_PAR_POULE, a, b;

    Club_Tirage(LC.saison, clubs);
    a = (r + 1 + k) % CLUB_PAR_POULE;
    b = (r + 4 - k) % CLUB_PAR_POULE;
    if (journee >= CLUB_PAR_POULE)
    {
        u32 t = a;
        a = b;
        b = t;
    }
    *domicile = clubs[poule * CLUB_PAR_POULE + a];
    *exterieur = clubs[poule * CLUB_PAR_POULE + b];
}

u32 Club_Duel(u32 match, u32 slot)
{
    ValiderMatch(&match);
    return (LC.duels[match] >> (4 * (slot % CLUB_DRESSEURS))) & 0xF;
}

static void EcrireDuel(u32 match, u32 slot, u32 code)
{
    LC.duels[match] = (LC.duels[match] & ~(0xF << (4 * slot))) | ((code & 0xF) << (4 * slot));
}

// Points du club dans un match (0 s'il n'y joue pas). *joue = le match a commencé.
static u32 PointsMatch(u32 match, u32 club, s32 *diff, u32 *bo, bool32 *joue)
{
    u8 dom, ext;
    u32 slot, pts = 0;
    bool32 domicile;

    if (match == CLUB_MATCH_FINALE)
        Club_Affiche(match, &dom, &ext);
    else
        AfficheDePoule(match, &dom, &ext);
    if (joue)
        *joue = FALSE;
    if (club != dom && club != ext)
        return 0;
    domicile = (club == dom);
    if (joue)
        *joue = (LC.duels[match] != 0);
    for (slot = 0; slot < CLUB_DRESSEURS; slot++)
    {
        u32 code = Club_Duel(match, slot);
        pts += Club_PointsDuel(code, domicile);
        if (diff)
            *diff += (s32)Club_KOInfliges(code, domicile) - (s32)Club_KOInfliges(code, !domicile);
        if (bo && EstBonusOffensif(code, domicile))
            (*bo)++;
    }
    return pts;
}

u32 Club_Points(u32 club)
{
    u32 m, pts = 0;
    for (m = 0; m < CLUB_NB_MATCHS; m++)
        pts += PointsMatch(m, club, NULL, NULL, NULL);
    return pts;
}

s32 Club_DiffKO(u32 club)
{
    u32 m;
    s32 diff = 0;
    for (m = 0; m < CLUB_NB_MATCHS; m++)
        PointsMatch(m, club, &diff, NULL, NULL);
    return diff;
}

u32 Club_BonusOffensifs(u32 club)
{
    u32 m, bo = 0;
    for (m = 0; m < CLUB_NB_MATCHS; m++)
        PointsMatch(m, club, NULL, &bo, NULL);
    return bo;
}

u32 Club_MatchsJoues(u32 club)
{
    u32 m, n = 0;
    for (m = 0; m < CLUB_NB_MATCHS; m++)
    {
        bool32 joue;
        PointsMatch(m, club, NULL, NULL, &joue);
        n += joue;
    }
    return n;
}

// Confrontation directe : points de a contre b sur l'aller et le retour.
static u32 PointsContre(u32 a, u32 b)
{
    u32 m, pts = 0;
    for (m = 0; m < CLUB_NB_MATCHS; m++)
    {
        u8 dom, ext;
        AfficheDePoule(m, &dom, &ext);
        if ((dom == a && ext == b) || (dom == b && ext == a))
            pts += PointsMatch(m, a, NULL, NULL, NULL);
    }
    return pts;
}

// a passe devant b ? Points, différence de K.O., confrontation directe, bonus offensifs, puis n° du club.
static bool32 Devant(u32 a, u32 b)
{
    u32 pa = Club_Points(a), pb = Club_Points(b);
    s32 da, db;
    u32 ha, hb, ba, bb;

    if (pa != pb)
        return pa > pb;
    da = Club_DiffKO(a);
    db = Club_DiffKO(b);
    if (da != db)
        return da > db;
    ha = PointsContre(a, b);
    hb = PointsContre(b, a);
    if (ha != hb)
        return ha > hb;
    ba = Club_BonusOffensifs(a);
    bb = Club_BonusOffensifs(b);
    if (ba != bb)
        return ba > bb;
    return a < b;
}

u32 Club_Classement(u32 poule, u8 *ordre)
{
    u8 clubs[CLUB_NB];
    u32 i, j, joues = 0;

    Club_Tirage(LC.saison, clubs);
    for (i = 0; i < CLUB_PAR_POULE; i++)
        ordre[i] = clubs[(poule % 2) * CLUB_PAR_POULE + i];
    for (i = 1; i < CLUB_PAR_POULE; i++) // tri par insertion (5 clubs)
    {
        u8 c = ordre[i];
        for (j = i; j > 0 && Devant(c, ordre[j - 1]); j--)
            ordre[j] = ordre[j - 1];
        ordre[j] = c;
    }
    for (i = 0; i < CLUB_PAR_POULE; i++)
        joues += Club_MatchsJoues(ordre[i]);
    return joues / 2;
}

void Club_Affiche(u32 match, u8 *domicile, u8 *exterieur)
{
    if (match >= CLUB_MATCH_FINALE)
    {
        u8 a[CLUB_PAR_POULE], b[CLUB_PAR_POULE];
        Club_Classement(0, a);
        Club_Classement(1, b);
        *domicile = a[0];
        *exterieur = b[0];
        return;
    }
    AfficheDePoule(match, domicile, exterieur);
}

// ---------------------------------------------------------------- clubs, dresseurs, rating

static u32 ClubJoueur(void)
{
    if (LC.magie != CLUB_MAGIE || LC.club == 0 || LC.club > CLUB_NB)
        return CLUB_AUCUN;
    return LC.club - 1;
}

u32 Club_DuJoueur(void)
{
    return ClubJoueur();
}

u32 Club_Tier(u32 club)
{
    return sClubs[club % CLUB_NB].tier;
}

u32 Club_Stade(u32 club)
{
    return sClubs[club % CLUB_NB].effet;
}

const u8 *Club_Nom(u32 club)
{
    return sClubs[club % CLUB_NB].nom;
}

const u8 *Club_NomCourt(u32 club)
{
    return sClubs[club % CLUB_NB].court;
}

const u8 *Club_NomStade(u32 club)
{
    return sClubs[club % CLUB_NB].stade;
}

u32 Club_Proprio(void)
{
    if (LC.magie != CLUB_MAGIE || LC.proprio == 0 || LC.proprio > CLUB_NB)
        return CLUB_AUCUN;
    return LC.proprio - 1;
}

const struct ClubDresseur *Club_Dresseur(u32 club, u32 slot)
{
    club %= CLUB_NB;
    slot %= CLUB_DRESSEURS;
    if (club == Club_Proprio() && slot < CLUB_PLACES_RECRUTEMENT && LC.effectif[slot] != 0)
        return Club_Recrue(LC.effectif[slot] - 1);
    return &sClubs[club].d[slot];
}

const u8 *Club_NomDresseur(u32 club, u32 slot)
{
    if (club == ClubJoueur() && slot == CLUB_SLOT_JOUEUR)
        return gSaveBlock2Ptr->playerName;
    return Club_Dresseur(club, slot)->nom;
}

u32 Club_CouleurClub(u32 club)
{
    return sClubs[club % CLUB_NB].couleur;
}

u32 Club_Titres(void)
{
    return LC.magie == CLUB_MAGIE ? LC.titres : 0;
}

u32 Club_Rating(u32 club, u32 slot)
{
    u32 i = (club % CLUB_NB) * CLUB_DRESSEURS + (slot % CLUB_DRESSEURS);
    u32 r = Club_Dresseur(club, slot)->rating + (LC.forme[i] >> 2);
    if (club == Club_Proprio() && slot < CLUB_PLACES_RECRUTEMENT)
        r += LC.coaching[slot];
    return r;
}

// Espèce évoluée pour un niveau : évolutions par niveau à leur niveau, les autres (pierre, échange, amitié) au N.36.
u32 Club_EvoluerPourNiveau(u32 species, u32 niveau)
{
    u32 n;
    for (n = 0; n < 3; n++)
    {
        const struct Evolution *evo = GetSpeciesEvolutions(species);
        u32 i, suivante = SPECIES_NONE;
        if (evo == NULL)
            break;
        for (i = 0; evo[i].method != EVOLUTIONS_END; i++)
        {
            u32 seuil;
            if (evo[i].method == EVO_LEVEL)
                seuil = evo[i].param ? evo[i].param : 30;
            else if (evo[i].method == EVO_ITEM || evo[i].method == EVO_TRADE)
                seuil = 36;
            else
                continue;
            if (seuil <= niveau && evo[i].targetSpecies != SPECIES_NONE && IsSpeciesEnabled(evo[i].targetSpecies))
            {
                suivante = evo[i].targetSpecies;
                break;
            }
        }
        if (suivante == SPECIES_NONE)
            break;
        species = suivante;
    }
    return species;
}

static u32 NiveauDresseur(u32 club, u32 slot)
{
    s32 niveau = (s32)GetCurrentLevelCap() - (s32)(100 - min(Club_Rating(club, slot), 100)) / 12;
    return niveau < 5 ? 5 : (niveau > MAX_LEVEL ? MAX_LEVEL : niveau);
}

static bool32 ProfiteDuStade(u32 species, u32 stade)
{
    u32 t1 = GetSpeciesType(species, 0), t2 = GetSpeciesType(species, 1);
#define TYPE(x) (t1 == (x) || t2 == (x))
    switch (stade)
    {
    case CLUB_STADE_HERBU:      return TYPE(TYPE_GRASS);
    case CLUB_STADE_GRAVITE:    return TYPE(TYPE_ROCK) || TYPE(TYPE_PSYCHIC) || TYPE(TYPE_GROUND);
    case CLUB_STADE_SOLEIL:     return TYPE(TYPE_FIRE);
    case CLUB_STADE_VENT:       return TYPE(TYPE_FLYING);
    case CLUB_STADE_SABLE:      return TYPE(TYPE_ROCK) || TYPE(TYPE_GROUND) || TYPE(TYPE_STEEL);
    case CLUB_STADE_ELECTRIQUE: return TYPE(TYPE_ELECTRIC);
    case CLUB_STADE_PLUIE:      return TYPE(TYPE_WATER);
    case CLUB_STADE_DISTORSION: return gSpeciesInfo[species].baseSpeed <= 60; // les lents passent devant
    case CLUB_STADE_PSYCHIQUE:  return TYPE(TYPE_PSYCHIC);
    case CLUB_STADE_NEIGE:      return TYPE(TYPE_ICE);
    default:                    return FALSE;
    }
#undef TYPE
}

u32 Club_RatingEffectif(u32 club, u32 slot, u32 stade, bool32 domicile)
{
    const struct ClubDresseur *d = Club_Dresseur(club, slot);
    u32 i, niveau = NiveauDresseur(club, slot), profitent = 0, r = Club_Rating(club, slot);

    for (i = 0; i < CLUB_TAILLE_EQUIPE; i++)
        profitent += ProfiteDuStade(Club_EvoluerPourNiveau(d->especes[i], niveau), stade);
    if (domicile && stade != CLUB_STADE_NEUTRE)
        r += 6;
    if (profitent >= 3)
        r += 4;
    return r;
}

// ---------------------------------------------------------------- simulation

static void StadeDuMatch(u32 match, u8 *dom, u8 *ext, u32 *stade)
{
    Club_Affiche(match, dom, ext);
    *stade = (match >= CLUB_MATCH_FINALE) ? CLUB_STADE_NEUTRE : Club_Stade(*dom);
}

// Écart de rating (domicile - extérieur) : >= +25 pousse vers le 6-0, entre -5 et +5 vers les fins à 1 survivant (1 % de nul).
u32 Club_SimulerDuel(u32 match, u32 slot)
{
    u8 dom, ext;
    u32 stade, h1, h2, survivants;
    s32 rd, re, ecart, avance;
    bool32 domicileGagne;

    ValiderMatch(&match);
    StadeDuMatch(match, &dom, &ext, &stade);
    rd = Club_RatingEffectif(dom, slot, stade, TRUE);
    re = Club_RatingEffectif(ext, slot, stade, FALSE);
    ecart = rd - re;
    h1 = Alea(LC.saison, match * 8 + slot, 1);
    h2 = Alea(LC.saison, match * 8 + slot, 2);

    if (ecart >= -5 && ecart <= 5 && h1 % 100 == 0)
        return CLUB_DUEL_NUL;
    domicileGagne = (s32)(h2 % 1000) < 500 + ecart * 15;
    avance = domicileGagne ? ecart : -ecart;

    if (avance >= 25)
        survivants = (h1 >> 8) % 10 < 6 ? 6 : 5;
    else if (avance >= -5 && avance <= 5)
        survivants = (h1 >> 8) % 2 ? 1 : 2;
    else if (avance > 5)
        survivants = 2 + (avance - 5) / 5 + (s32)((h1 >> 8) % 3) - 1;
    else // surprise : le moins bien noté l'emporte de justesse
        survivants = 1 + (h1 >> 8) % 2;
    survivants = survivants < 1 ? 1 : (survivants > 6 ? 6 : survivants);
    return domicileGagne ? CLUB_DUEL_DOMICILE(survivants) : CLUB_DUEL_EXTERIEUR(survivants);
}

// Forme de la saison : +1 toutes les 3 victoires, +2 pour un 6-0 (bonus de 0 à 15, remis à zéro à chaque saison).
static void CompterVictoire(u32 club, u32 slot, bool32 sixZero)
{
    u32 i = (club % CLUB_NB) * CLUB_DRESSEURS + (slot % CLUB_DRESSEURS);
    u32 victoires = (LC.forme[i] & 3) + 1, bonus = LC.forme[i] >> 2;

    if (club == ClubJoueur() && slot == CLUB_SLOT_JOUEUR)
        return;
    if (victoires >= 3)
    {
        victoires = 0;
        bonus++;
    }
    if (sixZero)
        bonus += 2;
    if (bonus > 15)
        bonus = 15;
    LC.forme[i] = (bonus << 2) | victoires;
}

static void AppliquerDuel(u32 match, u32 slot, u32 code)
{
    u8 dom, ext;
    Club_Affiche(match, &dom, &ext);
    EcrireDuel(match, slot, code);
    if (code >= 1 && code <= 6)
        CompterVictoire(dom, slot, code == 6);
    else if (code >= 7 && code <= 12)
        CompterVictoire(ext, slot, code == 12);
}

static bool32 JoueurDansMatch(u32 match)
{
    u8 dom, ext;
    u32 club = ClubJoueur();
    if (club == CLUB_AUCUN)
        return FALSE;
    Club_Affiche(match, &dom, &ext);
    return dom == club || ext == club;
}

static void ResoudreMatch(u32 match)
{
    u32 slot;
    bool32 joueur = JoueurDansMatch(match);
    for (slot = 0; slot < CLUB_DRESSEURS; slot++)
    {
        if (joueur && slot == CLUB_SLOT_JOUEUR)
            continue; // le joueur joue son duel en vrai
        AppliquerDuel(match, slot, Club_SimulerDuel(match, slot));
    }
}

// Duel du joueur pas joué à temps : forfait, défaite 0-6.
static void Forfait(u32 match)
{
    u8 dom, ext;
    if (LC.duels[match] == 0 || Club_Duel(match, CLUB_SLOT_JOUEUR) != CLUB_DUEL_PAS_JOUE)
        return;
    Club_Affiche(match, &dom, &ext);
    if (dom == ClubJoueur())
        AppliquerDuel(match, CLUB_SLOT_JOUEUR, CLUB_DUEL_EXTERIEUR(6));
    else if (ext == ClubJoueur())
        AppliquerDuel(match, CLUB_SLOT_JOUEUR, CLUB_DUEL_DOMICILE(6));
    else // plus licencié dans ce club : la Recrue joue
        AppliquerDuel(match, CLUB_SLOT_JOUEUR, Club_SimulerDuel(match, CLUB_SLOT_JOUEUR));
    LC.rapport |= CLUB_RAPPORT_FORFAIT;
}

static void ForfaitsAvant(u32 matchLimite)
{
    u32 m;
    for (m = 0; m < matchLimite && m <= CLUB_MATCH_FINALE; m++)
        Forfait(m);
}

u32 Club_MatchDuJoueur(void)
{
    u32 m;
    if (ClubJoueur() == CLUB_AUCUN)
        return 0xFF;
    for (m = 0; m <= CLUB_MATCH_FINALE; m++)
        if (LC.duels[m] != 0 && Club_Duel(m, CLUB_SLOT_JOUEUR) == CLUB_DUEL_PAS_JOUE && JoueurDansMatch(m))
            return m;
    return 0xFF;
}

// ---------------------------------------------------------------- finale

static bool32 FinaleComplete(void)
{
    u32 slot;
    if (LC.duels[CLUB_MATCH_FINALE] == 0)
        return FALSE;
    for (slot = 0; slot < CLUB_DRESSEURS; slot++)
        if (Club_Duel(CLUB_MATCH_FINALE, slot) == CLUB_DUEL_PAS_JOUE)
            return FALSE;
    return TRUE;
}

u32 Club_Vainqueur(void)
{
    u8 dom, ext;
    u32 pd, pe;
    if (!FinaleComplete())
        return CLUB_AUCUN;
    Club_Affiche(CLUB_MATCH_FINALE, &dom, &ext);
    pd = PointsMatch(CLUB_MATCH_FINALE, dom, NULL, NULL, NULL);
    pe = PointsMatch(CLUB_MATCH_FINALE, ext, NULL, NULL, NULL);
    if (pd != pe)
        return pd > pe ? dom : ext;
    if (LC.mortSubite == 0)
        return CLUB_AUCUN;
    return LC.mortSubite == 1 ? dom : ext;
}

// Égalité parfaite : mort subite en 3 contre 3 entre les Capitaines (simulée, terrain neutre).
static void MortSubite(void)
{
    u8 dom, ext;
    s32 ecart;
    if (LC.mortSubite)
        return;
    Club_Affiche(CLUB_MATCH_FINALE, &dom, &ext);
    ecart = (s32)Club_RatingEffectif(dom, 0, CLUB_STADE_NEUTRE, FALSE) - (s32)Club_RatingEffectif(ext, 0, CLUB_STADE_NEUTRE, FALSE);
    LC.mortSubite = (s32)(Alea(LC.saison, 9999, 3) % 1000) < 500 + ecart * 15 ? 1 : 2;
}

static void ConclureFinale(void)
{
    u8 dom, ext;
    u32 club = ClubJoueur(), vainqueur;

    if ((LC.drapeaux & CLUB_DRAP_FINALE_CONCLUE) || !FinaleComplete())
        return;
    if (Club_Vainqueur() == CLUB_AUCUN)
        MortSubite();
    vainqueur = Club_Vainqueur();
    LC.drapeaux |= CLUB_DRAP_FINALE_CONCLUE;
    Club_Affiche(CLUB_MATCH_FINALE, &dom, &ext);
    if (club == CLUB_AUCUN || (club != dom && club != ext))
        return;
    if (LC.finales < 0xFF)
        LC.finales++;
    if (vainqueur == club)
    {
        if (LC.titres < 0xFF)
            LC.titres++;
        AddMoney(&gSaveBlock1Ptr->money, CLUB_PRIME_VAINQUEUR);
        AddBagItem(ITEM_ABILITY_PATCH, 1);
        LC.rapportArgent += CLUB_PRIME_VAINQUEUR;
        LC.rapport |= CLUB_RAPPORT_CHAMPION;
    }
    else
    {
        AddMoney(&gSaveBlock1Ptr->money, CLUB_PRIME_FINALISTE);
        LC.rapportArgent += CLUB_PRIME_FINALISTE;
        LC.rapport |= CLUB_RAPPORT_FINALISTE;
    }
}

// ---------------------------------------------------------------- jours et saisons

static void TraiterJour(u32 jourSaison)
{
    u32 j;
    for (j = 0; j < CLUB_NB_JOURNEES; j++)
    {
        if (sJoursJournees[j] * CLUB_ECHELLE == jourSaison)
        {
            u32 k;
            ForfaitsAvant(j * CLUB_MATCHS_PAR_JOURNEE);
            for (k = 0; k < CLUB_MATCHS_PAR_JOURNEE; k++)
                ResoudreMatch(j * CLUB_MATCHS_PAR_JOURNEE + k);
        }
    }
    if (jourSaison == CLUB_JOUR_FINALE)
    {
        ForfaitsAvant(CLUB_MATCH_FINALE);
        ResoudreMatch(CLUB_MATCH_FINALE);
        ConclureFinale();
    }
}

// Fin de saison : finale bouclée, puis saison suivante (nouveau tirage, forme remise à zéro, licence renouvelée).
static void FinSaison(u32 nouvelleSaison)
{
    u32 club = ClubJoueur();

    ForfaitsAvant(CLUB_MATCH_FINALE + 1);
    ConclureFinale();
    if (Club_Proprio() != CLUB_AUCUN)
        Club_Bilan(); // billetterie, droits TV, boutique, salaires, entretien, impôt : avant la remise à zéro
    LC.saison = nouvelleSaison;
    memset(LC.duels, 0, sizeof(LC.duels));
    memset(LC.forme, 0, sizeof(LC.forme));
    LC.mortSubite = 0;
    LC.drapeaux &= ~CLUB_DRAP_FINALE_CONCLUE;
    LC.rapport |= CLUB_RAPPORT_SAISON;
    if (club != CLUB_AUCUN && club == Club_Proprio())
    {
        LC.rapport |= CLUB_RAPPORT_RENOUVELEE; // le propriétaire joue sans licence
    }
    else if (club != CLUB_AUCUN)
    {
        u32 prix = Club_PrixLicence(club);
        if (IsEnoughMoney(&gSaveBlock1Ptr->money, prix))
        {
            RemoveMoney(&gSaveBlock1Ptr->money, prix);
            LC.rapport |= CLUB_RAPPORT_RENOUVELEE;
        }
        else
        {
            LC.club = 0;
            LC.rapport |= CLUB_RAPPORT_PERDUE;
        }
    }
}

void Club_MettreAJourJusqua(u32 aujourdhui)
{
    u32 pas = 0;

    Verifier(aujourdhui);
    if (aujourdhui < LC.jourDebut)
        return;
    // Longue absence : la saison en cours est bouclée, puis on saute au début de la saison d'aujourd'hui.
    if (aujourdhui > (u32)LC.jourTraite + 2 * CLUB_JOURS_SAISON)
    {
        u32 saison = (aujourdhui - LC.jourDebut) / CLUB_JOURS_SAISON;
        if (saison != LC.saison)
        {
            FinSaison(saison);
            LC.jourTraite = LC.jourDebut + saison * CLUB_JOURS_SAISON - 1;
        }
    }
    while (LC.jourTraite < aujourdhui && pas++ < 3 * CLUB_JOURS_SAISON)
    {
        u32 jour = LC.jourTraite + 1;
        u32 saison = (jour - LC.jourDebut) / CLUB_JOURS_SAISON;
        if (saison != LC.saison)
            FinSaison(saison);
        TraiterJour((jour - LC.jourDebut) % CLUB_JOURS_SAISON + 1);
        LC.jourTraite = jour;
    }
}

// ---------------------------------------------------------------- licence

u32 Club_PrixLicence(u32 club)
{
    return sPrixLicence[Club_Tier(club)];
}

u32 Club_CoutSignature(u32 club)
{
    u32 actuel = ClubJoueur(), prix = Club_PrixLicence(club);
    if (actuel != CLUB_AUCUN && actuel != club && Club_JourDeSaison() > CLUB_JOURS_MERCATO)
        prix += Club_PrixLicence(actuel) * CLUB_CLAUSE_POURCENT / 100;
    return prix;
}

bool32 Club_Signer(u32 club)
{
    u32 cout;

    if (club >= CLUB_NB || club == ClubJoueur() || Club_Proprio() != CLUB_AUCUN)
        return FALSE; // un propriétaire joue dans son club
    cout = Club_CoutSignature(club);
    if (!IsEnoughMoney(&gSaveBlock1Ptr->money, cout))
        return FALSE;
    RemoveMoney(&gSaveBlock1Ptr->money, cout);
    Club_QuitterClub();
    LC.club = club + 1;
    return TRUE;
}

// Un duel en attente dans l'ancien club est joué par la Recrue qui reprend sa place.
void Club_QuitterClub(void)
{
    u32 m = Club_MatchDuJoueur();
    LC.club = 0;
    if (m != 0xFF)
        AppliquerDuel(m, CLUB_SLOT_JOUEUR, Club_SimulerDuel(m, CLUB_SLOT_JOUEUR));
}

// Prime du match pour le joueur : base + par point marqué par son club dans la rencontre.
static u32 PrimeDuMatch(u32 match)
{
    u32 club = ClubJoueur(), tier;
    if (club == CLUB_AUCUN)
        return 0;
    tier = Club_Tier(club);
    return sPrimeBase[tier] + sPrimeParPoint[tier] * PointsMatch(match, club, NULL, NULL, NULL);
}

static EWRAM_DATA u32 sDernierePrime = 0;

void Club_EnregistrerDuelJoueur(u32 code)
{
    u32 m = Club_MatchDuJoueur();
    sDernierePrime = 0;
    if (m == 0xFF)
        return;
    AppliquerDuel(m, CLUB_SLOT_JOUEUR, code);
    sDernierePrime = PrimeDuMatch(m);
    AddMoney(&gSaveBlock1Ptr->money, sDernierePrime);
    if (m == CLUB_MATCH_FINALE)
        ConclureFinale();
}

// ---------------------------------------------------------------- combat du joueur

bool32 Club_CombatEnCours(void)
{
    return sCombat;
}

const struct Trainer *Club_Adversaire(void)
{
    return &sAdversaire;
}

static void ConstruireAdversaire(u32 match, u32 club, u32 stade)
{
    const struct ClubDresseur *d = &sClubs[club].d[CLUB_SLOT_JOUEUR];
    u32 i, niveau = NiveauDresseur(club, CLUB_SLOT_JOUEUR), rating = min(Club_Rating(club, CLUB_SLOT_JOUEUR), 100);
    u32 iv = rating * 31 / 100;

    memset(&sAdversaire, 0, sizeof(sAdversaire));
    memset(sAdversaireEquipe, 0, sizeof(sAdversaireEquipe));
    for (i = 0; i < CLUB_TAILLE_EQUIPE; i++)
    {
        sAdversaireEquipe[i].species = Club_EvoluerPourNiveau(d->especes[i], niveau);
        sAdversaireEquipe[i].lvl = niveau;
        sAdversaireEquipe[i].iv = TRAINER_PARTY_IVS(iv, iv, iv, iv, iv, iv);
        sAdversaireEquipe[i].ev = sEvsTier[Club_Tier(club)];
        sAdversaireEquipe[i].nature = Alea(LC.saison, match * 8 + i, 9) % NUM_NATURES;
    }
    sAdversaire.aiFlags = rating >= 65 ? AI_FLAG_SMART_TRAINER : AI_FLAG_BASIC_TRAINER;
    sAdversaire.party = sAdversaireEquipe;
    sAdversaire.partySize = CLUB_TAILLE_EQUIPE;
    sAdversaire.trainerClass = d->classe;
    sAdversaire.trainerPic = d->pic;
    sAdversaire.gender = d->fille;
    sAdversaire.encounterMusic = d->fille ? TRAINER_ENCOUNTER_MUSIC_FEMALE : TRAINER_ENCOUNTER_MUSIC_MALE;
    sAdversaire.battleType = TRAINER_BATTLE_TYPE_SINGLES;
    StringCopy(sAdversaire.trainerName, d->nom);

    // Effet du stade du club qui reçoit, lancé au 1er tour (pas en finale, terrain neutre).
    switch (stade)
    {
    case CLUB_STADE_HERBU:      sAdversaire.startingStatus.grassyTerrainTemporary = TRUE; break;
    case CLUB_STADE_GRAVITE:    sAdversaire.startingStatus.gravityTemporary = TRUE; break;
    case CLUB_STADE_SOLEIL:     sAdversaire.startingStatus.weatherSunTemporary = TRUE; break;
    case CLUB_STADE_VENT:
        sAdversaire.startingStatus.tailwindPlayerTemporary = TRUE;
        sAdversaire.startingStatus.tailwindOpponentTemporary = TRUE;
        break;
    case CLUB_STADE_SABLE:      sAdversaire.startingStatus.weatherSandstormTemporary = TRUE; break;
    case CLUB_STADE_ELECTRIQUE: sAdversaire.startingStatus.electricTerrainTemporary = TRUE; break;
    case CLUB_STADE_PLUIE:      sAdversaire.startingStatus.weatherRainTemporary = TRUE; break;
    case CLUB_STADE_DISTORSION: sAdversaire.startingStatus.trickRoomTemporary = TRUE; break;
    case CLUB_STADE_PSYCHIQUE:  sAdversaire.startingStatus.psychicTerrainTemporary = TRUE; break;
    case CLUB_STADE_NEIGE:      sAdversaire.startingStatus.weatherSnowTemporary = TRUE; break;
    default: break;
    }
}

static u32 PokemonValides(void)
{
    u32 i, n = 0;
    for (i = 0; i < PARTY_SIZE; i++)
        if (GetMonData(&gParties[B_TRAINER_PLAYER][i], MON_DATA_SPECIES) != SPECIES_NONE && !GetMonData(&gParties[B_TRAINER_PLAYER][i], MON_DATA_IS_EGG))
            n++;
    return n;
}

static u32 Vivants(struct Pokemon *equipe)
{
    u32 i, n = 0;
    for (i = 0; i < PARTY_SIZE; i++)
        if (GetMonData(&equipe[i], MON_DATA_SPECIES) != SPECIES_NONE && !GetMonData(&equipe[i], MON_DATA_IS_EGG)
         && GetMonData(&equipe[i], MON_DATA_HP) > 0)
            n++;
    return n;
}

static const u8 *const sNomsSlots[CLUB_DRESSEURS] =
{
    COMPOUND_STRING("Capitaine"), COMPOUND_STRING("Titulaire"), COMPOUND_STRING("Titulaire"), COMPOUND_STRING("Recrue"),
};

static const u8 *const sNomsEffets[] =
{
    [CLUB_STADE_HERBU]      = COMPOUND_STRING("Champ Herbu"),
    [CLUB_STADE_GRAVITE]    = COMPOUND_STRING("Gravité"),
    [CLUB_STADE_SOLEIL]     = COMPOUND_STRING("Soleil"),
    [CLUB_STADE_VENT]       = COMPOUND_STRING("Vent Arrière"),
    [CLUB_STADE_SABLE]      = COMPOUND_STRING("Tempête de sable"),
    [CLUB_STADE_ELECTRIQUE] = COMPOUND_STRING("Champ Électrifié"),
    [CLUB_STADE_PLUIE]      = COMPOUND_STRING("Pluie"),
    [CLUB_STADE_DISTORSION] = COMPOUND_STRING("Distorsion"),
    [CLUB_STADE_PSYCHIQUE]  = COMPOUND_STRING("Champ Psychique"),
    [CLUB_STADE_NEIGE]      = COMPOUND_STRING("Neige"),
    [CLUB_STADE_NEUTRE]     = COMPOUND_STRING("aucun effet"),
};

const u8 *Club_NomSlot(u32 slot)
{
    return sNomsSlots[slot % CLUB_DRESSEURS];
}

const u8 *Club_NomEffet(u32 stade)
{
    return sNomsEffets[stade > CLUB_STADE_NEUTRE ? CLUB_STADE_NEUTRE : stade];
}

static const u8 sText_PasDeLicence[] = _("Tu n'as pas de licence. Signe\ndans un club pour jouer.");
static const u8 sText_PasDeMatch[] = _("Pas de duel à jouer pour l'instant.\nProchaine journée : jour ");
static const u8 sText_Point[] = _(".");
static const u8 sText_FinaleTour[] = _("La finale se joue à la Tour de\nCombat. Rendez-vous là-bas !");
static const u8 sText_Tournoi[] = _("Termine d'abord ton tournoi du\nCouronnement Mondial.");
static const u8 sText_SixPokemon[] = _("C'est du 6 contre 6 : il te faut\n6 Pokémon dans ton équipe.");
static const u8 sText_Prepa1[] = _("Ton duel : contre ");
static const u8 sText_Prepa2[] = _(",\nRecrue de ");
static const u8 sText_Prepa4[] = _(".\p6 Pokémon au N.");
static const u8 sText_Prepa5[] = _(" en face.\p");
static const u8 sText_Prepa6[] = _("Stade : ");
static const u8 sText_Prepa7[] = _(".\nEffet : ");
static const u8 sText_Prepa8[] = _(", 5 tours.");
static const u8 sText_PrepaFinale[] = _("Grande Finale, Tour de Combat :\nterrain neutre, aucun effet.");
static const u8 sText_PrepaSac[] = _("\pPas d'objets du sac en ligue.");
static const u8 sText_Pret1[] = _("\pLe club te prête ");
static const u8 sText_Pret2[] = _(" Pokémon\npour compléter ton équipe : ");
static const u8 sText_PretGratuit[] = _("offert");
static const u8 sText_PretTropCher[] = _("Le club peut te prêter des Pokémon,\nmais il faut ");

static u32 ProchaineJournee(void)
{
    u32 j, jour = Club_JourDeSaison();
    for (j = 0; j < CLUB_NB_JOURNEES; j++)
        if (sJoursJournees[j] * CLUB_ECHELLE > jour)
            return sJoursJournees[j] * CLUB_ECHELLE;
    return CLUB_JOUR_FINALE;
}

// VAR_RESULT = un duel à jouer ; gStringVar4 = présentation ou raison.
void Club_PreparerMatch(void)
{
    u8 dom, ext;
    u32 m, adverse, stade;
    u8 *fin;

    Club_MettreAJourJusqua(RtcGetLocalDayCount());
    gSpecialVar_Result = FALSE;
    if (ClubJoueur() == CLUB_AUCUN)
    {
        StringCopy(gStringVar4, sText_PasDeLicence);
        return;
    }
    m = Club_MatchDuJoueur();
    if (m == 0xFF)
    {
        fin = StringCopy(gStringVar4, sText_PasDeMatch);
        fin = AjouterNombre(fin, ProchaineJournee());
        StringCopy(fin, sText_Point);
        return;
    }
    // Après la Ligue, la Grande Finale se joue sur place, au guichet de la Tour de Combat (avant : n'importe quel guichet).
    if (m == CLUB_MATCH_FINALE && FlagGet(FLAG_SYS_GAME_CLEAR)
     && (gSaveBlock1Ptr->location.mapGroup != MAP_GROUP(MAP_BATTLE_FRONTIER_BATTLE_TOWER_LOBBY)
      || gSaveBlock1Ptr->location.mapNum != MAP_NUM(MAP_BATTLE_FRONTIER_BATTLE_TOWER_LOBBY)))
    {
        StringCopy(gStringVar4, sText_FinaleTour);
        return;
    }
    if (FlagGet(FLAG_COURONNEMENT_EN_COURS))
    {
        StringCopy(gStringVar4, sText_Tournoi);
        return;
    }
    sPretPrevu = 0;
    if (PokemonValides() < CLUB_TAILLE_EQUIPE)
    {
        u32 n = Club_PretManquants(), cout = Club_PretCout(n);
        if (n == 0 || PokemonValides() + n < CLUB_TAILLE_EQUIPE)
        {
            StringCopy(gStringVar4, sText_SixPokemon);
            return;
        }
        if (!IsEnoughMoney(&gSaveBlock1Ptr->money, cout))
        {
            fin = StringCopy(gStringVar4, sText_PretTropCher);
            fin = AjouterYens(fin, cout);
            StringCopy(fin, sText_Point);
            return;
        }
        sPretPrevu = n;
    }
    StadeDuMatch(m, &dom, &ext, &stade);
    adverse = (dom == ClubJoueur()) ? ext : dom;
    ConstruireAdversaire(m, adverse, stade);
    sMatchCombat = m;
    fin = StringCopy(gStringVar4, sText_Prepa1);
    fin = StringCopy(fin, sAdversaire.trainerName);
    fin = StringCopy(fin, sText_Prepa2);
    fin = StringCopy(fin, Club_NomCourt(adverse));
    fin = StringCopy(fin, sText_Prepa4);
    fin = AjouterNombre(fin, sAdversaireEquipe[0].lvl);
    fin = StringCopy(fin, sText_Prepa5);
    if (stade == CLUB_STADE_NEUTRE)
    {
        fin = StringCopy(fin, sText_PrepaFinale);
    }
    else
    {
        fin = StringCopy(fin, sText_Prepa6);
        fin = StringCopy(fin, Club_NomStade(dom));
        fin = StringCopy(fin, sText_Prepa7);
        fin = StringCopy(fin, Club_NomEffet(stade));
        fin = StringCopy(fin, sText_Prepa8);
    }
    fin = StringCopy(fin, sText_PrepaSac);
    if (sPretPrevu)
    {
        u32 cout = Club_PretCout(sPretPrevu);
        fin = StringCopy(fin, sText_Pret1);
        fin = AjouterNombre(fin, sPretPrevu);
        fin = StringCopy(fin, sText_Pret2);
        if (cout)
            fin = AjouterYens(fin, cout);
        else
            fin = StringCopy(fin, sText_PretGratuit);
        StringCopy(fin, sText_Point);
    }
    gSpecialVar_Result = TRUE;
}

void Club_DebutMatch(void)
{
    if (sPretPrevu)
    {
        RemoveMoney(&gSaveBlock1Ptr->money, Club_PretCout(sPretPrevu));
        Club_PretPreparer(ClubJoueur(), sAdversaireEquipe[0].lvl, sPretPrevu);
        sPretPrevu = 0;
    }
    HealPlayerParty();
    sCombat = TRUE;
    FlagSet(FLAG_COURONNEMENT_EN_COURS); // pas de blackout
}

static const u8 sText_Victoire[] = _("Victoire ");
static const u8 sText_Defaite[] = _("Défaite ");
static const u8 sText_Nul[] = _("Match nul, double K.O. !");
static const u8 sText_Tiret[] = _("-");
static const u8 sText_Exclam[] = _(" !");
static const u8 sText_BO[] = _(" Bonus offensif !");
static const u8 sText_BD[] = _(" Bonus défensif !");
static const u8 sText_Score1[] = _("\pScore de la rencontre : ");
static const u8 sText_Score3[] = _(".\nPrime du match : ");
static const u8 sText_Champion[] = _("\p{PLAYER} et son club sont\nchampions de la Ligue !");
static const u8 sText_Finaliste[] = _("\pLa coupe s'envole. Prime de\nfinaliste versée.");

// Après le combat : résultat enregistré (survivants), prime versée. VAR_RESULT = victoire ; bilan dans gStringVar4.
void Club_FinMatch(void)
{
    u8 dom, ext;
    u32 joueur = Vivants(gParties[B_TRAINER_PLAYER]), adverse = Vivants(gParties[B_TRAINER_OPPONENT_A]), code, m = sMatchCombat, club = ClubJoueur();
    u32 ptsJoueur, ptsAdverse, autre;
    bool32 domicile;
    u8 *fin;

    sCombat = FALSE;
    FlagClear(FLAG_COURONNEMENT_EN_COURS);
    Club_PretRendre(); // les Pokémon prêtés repartent (les survivants sont déjà comptés)
    HealPlayerParty();
    Club_Affiche(m, &dom, &ext);
    domicile = (dom == club);
    autre = domicile ? ext : dom;
    joueur = joueur < 1 ? 1 : (joueur > 6 ? 6 : joueur);
    adverse = adverse < 1 ? 1 : (adverse > 6 ? 6 : adverse);

    if (gBattleOutcome == B_OUTCOME_WON)
        code = domicile ? CLUB_DUEL_DOMICILE(joueur) : CLUB_DUEL_EXTERIEUR(joueur);
    else if (gBattleOutcome == B_OUTCOME_DREW)
        code = CLUB_DUEL_NUL;
    else if (gBattleOutcome == B_OUTCOME_LOST)
        code = domicile ? CLUB_DUEL_EXTERIEUR(adverse) : CLUB_DUEL_DOMICILE(adverse);
    else // fuite ou abandon
        code = domicile ? CLUB_DUEL_EXTERIEUR(6) : CLUB_DUEL_DOMICILE(6);

    Club_EnregistrerDuelJoueur(code);
    gSpecialVar_Result = (gBattleOutcome == B_OUTCOME_WON);

    if (code == CLUB_DUEL_NUL)
    {
        fin = StringCopy(gStringVar4, sText_Nul);
    }
    else
    {
        u8 sd, se;
        bool32 gagne = gSpecialVar_Result;
        Survivants(code, &sd, &se);
        fin = StringCopy(gStringVar4, gagne ? sText_Victoire : sText_Defaite);
        fin = AjouterNombre(fin, domicile ? sd : se);
        fin = StringCopy(fin, sText_Tiret);
        fin = AjouterNombre(fin, domicile ? se : sd);
        fin = StringCopy(fin, sText_Exclam);
        if (EstBonusOffensif(code, domicile))
            fin = StringCopy(fin, sText_BO);
        else if (!gagne && Club_PointsDuel(code, domicile) == 1)
            fin = StringCopy(fin, sText_BD);
    }
    ptsJoueur = PointsMatch(m, club, NULL, NULL, NULL);
    ptsAdverse = PointsMatch(m, autre, NULL, NULL, NULL);
    fin = StringCopy(fin, sText_Score1);
    fin = AjouterNombre(fin, ptsJoueur);
    fin = StringCopy(fin, sText_Tiret);
    fin = AjouterNombre(fin, ptsAdverse);
    fin = StringCopy(fin, sText_Score3);
    fin = AjouterYens(fin, sDernierePrime);
    fin = StringCopy(fin, sText_Point);
    if (m == CLUB_MATCH_FINALE && (LC.drapeaux & CLUB_DRAP_FINALE_CONCLUE))
    {
        StringCopy(fin, Club_Vainqueur() == club ? sText_Champion : sText_Finaliste);
        LC.rapport &= ~(CLUB_RAPPORT_CHAMPION | CLUB_RAPPORT_FINALISTE); // déjà annoncé
        LC.rapportArgent = 0;
    }
}

// ---------------------------------------------------------------- specials de l'appli

static const u8 sText_R_Saison[] = _("Nouvelle saison de la Ligue des\nClubs : tirage des poules fait.");
static const u8 sText_R_Renouvelee[] = _("\pTa licence est renouvelée.");
static const u8 sText_R_Perdue[] = _("\pPas assez d'argent : ta licence\nn'est pas renouvelée.");
static const u8 sText_R_Forfait[] = _("\pTu n'as pas joué un duel à temps :\nforfait, défaite 0-6.");
static const u8 sText_R_Champion[] = _("\pTon club a gagné la finale ! Tu\nreçois ");
static const u8 sText_R_Finaliste[] = _("\pTon club a perdu la finale. Prime\nde finaliste : ");
static const u8 sText_R_Patch[] = _(" et un Patch Talent.");

// VAR_RESULT = il y a un rapport à afficher (gStringVar4).
void Club_Ouvrir(void)
{
    u8 *fin = gStringVar4;
    u8 r;

    Club_MettreAJourJusqua(RtcGetLocalDayCount());
    r = LC.rapport;
    *fin = EOS;
    gSpecialVar_Result = (r != 0);
    if (r == 0)
        return;
    if (r & CLUB_RAPPORT_SAISON)
        fin = StringCopy(fin, sText_R_Saison);
    if (r & CLUB_RAPPORT_RENOUVELEE)
        fin = StringCopy(fin, sText_R_Renouvelee);
    if (r & CLUB_RAPPORT_PERDUE)
        fin = StringCopy(fin, sText_R_Perdue);
    if (r & CLUB_RAPPORT_FORFAIT)
        fin = StringCopy(fin, sText_R_Forfait);
    if (r & CLUB_RAPPORT_CHAMPION)
    {
        fin = StringCopy(fin, sText_R_Champion);
        fin = AjouterYens(fin, LC.rapportArgent);
        fin = StringCopy(fin, sText_R_Patch);
    }
    else if (r & CLUB_RAPPORT_FINALISTE)
    {
        fin = StringCopy(fin, sText_R_Finaliste);
        fin = AjouterYens(fin, LC.rapportArgent);
        fin = StringCopy(fin, sText_Point);
    }
    if (r & CLUB_RAPPORT_BILAN)
        fin = Club_TexteBilan(fin);
    if (gStringVar4[0] == CHAR_PROMPT_SCROLL || gStringVar4[0] == CHAR_PROMPT_CLEAR)
        memmove(gStringVar4, gStringVar4 + 1, StringLength(gStringVar4)); // pas de saut de page en tête
    LC.rapport = 0;
    LC.rapportArgent = 0;
}

static const u8 sText_A1[] = _("Saison ");
static const u8 sText_A2[] = _(", jour ");
#if EDITION_TYCOON_SIM
static const u8 sText_A3[] = _("/270");
#else
static const u8 sText_A3[] = _("/30");
#endif
static const u8 sText_AMercato[] = _(" (mercato)");
static const u8 sText_AFinale[] = _(".\nGrande Finale aujourd'hui !\p");
static const u8 sText_A4[] = _(".\nProchaine journée : jour ");
static const u8 sText_A5[] = _("Sans club.");
static const u8 sText_A6[] = _(" : ");
static const u8 sText_A7[] = _("e poule ");
static const u8 sText_A8[] = _(", ");
static const u8 sText_A9[] = _(" pts.");
static const u8 sText_A10[] = _("\nUn duel t'attend !");

void Club_Resume(void)
{
    u32 jour, club;
    u8 *fin;

    Club_MettreAJourJusqua(RtcGetLocalDayCount());
    jour = Club_JourDeSaison();
    club = ClubJoueur();
    fin = StringCopy(gStringVar4, sText_A1);
    fin = AjouterNombre(fin, LC.saison + 1);
    fin = StringCopy(fin, sText_A2);
    fin = AjouterNombre(fin, jour);
    fin = StringCopy(fin, sText_A3);
    if (jour <= CLUB_JOURS_MERCATO)
        fin = StringCopy(fin, sText_AMercato);
    if (jour == CLUB_JOUR_FINALE)
    {
        fin = StringCopy(fin, sText_AFinale);
    }
    else
    {
        fin = StringCopy(fin, sText_A4);
        fin = AjouterNombre(fin, ProchaineJournee());
        fin = StringCopy(fin, sText_Point);
        *fin++ = CHAR_PROMPT_CLEAR;
    }
    if (club == CLUB_AUCUN)
    {
        StringCopy(fin, sText_A5);
        return;
    }
    {
        u8 ordre[CLUB_PAR_POULE];
        u32 poule = Club_Poule(club), rang = 0, i;
        Club_Classement(poule, ordre);
        for (i = 0; i < CLUB_PAR_POULE; i++)
            if (ordre[i] == club)
                rang = i + 1;
        fin = StringCopy(fin, Club_NomCourt(club));
        fin = StringCopy(fin, sText_A6);
        fin = AjouterNombre(fin, rang);
        fin = StringCopy(fin, sText_A7);
        *fin++ = poule ? CHAR_B : CHAR_A;
        fin = StringCopy(fin, sText_A8);
        fin = AjouterNombre(fin, Club_Points(club));
        fin = StringCopy(fin, sText_A9);
        if (Club_MatchDuJoueur() != 0xFF)
            fin = StringCopy(fin, sText_A10);
        *fin = EOS;
    }
}

static const u8 sText_L1[] = _(" (tier ");
static const u8 sText_L2[] = _(")\nLicence : ");
static const u8 sText_L3[] = _(" / saison.\p");
static const u8 sText_L4[] = _("Stade : ");
static const u8 sText_L5[] = _(".\nEffet : ");
static const u8 sText_L6[] = _(".\pPrime par match : ");
static const u8 sText_L7[] = _(" ¥\n+ ");
static const u8 sText_L8[] = _(" ¥ par point du club.");
static const u8 sText_L9[] = _("\pHors mercato, la clause de ton\nclub te coûte ");
static const u8 sText_L10[] = _(" de plus.");
static const u8 sText_LDeja[] = _("\pC'est déjà ton club.");
static const u8 sText_L11[] = _("\pTu signes pour ");
static const u8 sText_L12[] = _(" ?");
static const u8 sTiers[] = _("CBAS");

// VAR_0x8004 = club. VAR_RESULT = signature possible.
void Club_FicheLicence(void)
{
    u32 club = gSpecialVar_0x8004 % CLUB_NB, tier = Club_Tier(club), cout = Club_CoutSignature(club);
    u8 *fin;

    Club_MettreAJourJusqua(RtcGetLocalDayCount());
    fin = StringCopy(gStringVar4, Club_Nom(club));
    fin = StringCopy(fin, sText_L1);
    *fin++ = sTiers[tier];
    fin = StringCopy(fin, sText_L2);
    fin = AjouterYens(fin, Club_PrixLicence(club));
    fin = StringCopy(fin, sText_L3);
    fin = StringCopy(fin, sText_L4);
    fin = StringCopy(fin, Club_NomStade(club));
    fin = StringCopy(fin, sText_L5);
    fin = StringCopy(fin, Club_NomEffet(Club_Stade(club)));
    fin = StringCopy(fin, sText_L6);
    fin = AjouterNombre(fin, sPrimeBase[tier]);
    fin = StringCopy(fin, sText_L7);
    fin = AjouterNombre(fin, sPrimeParPoint[tier]);
    fin = StringCopy(fin, sText_L8);
    if (club == ClubJoueur())
    {
        StringCopy(fin, sText_LDeja);
        gSpecialVar_Result = FALSE;
        return;
    }
    if (cout > Club_PrixLicence(club))
    {
        fin = StringCopy(fin, sText_L9);
        fin = AjouterYens(fin, cout - Club_PrixLicence(club));
        fin = StringCopy(fin, sText_L10);
    }
    fin = StringCopy(fin, sText_L11);
    fin = AjouterYens(fin, cout);
    StringCopy(fin, sText_L12);
    gSpecialVar_Result = TRUE;
}

static const u8 sText_Signe[] = _("Licence signée ! Tu es la Recrue\ndu club ");
static const u8 sText_PasAssez[] = _("Il te manque de l'argent pour\nsigner cette licence.");

void Club_ScriptSigner(void)
{
    u32 club = gSpecialVar_0x8004 % CLUB_NB;
    u8 *fin;
    gSpecialVar_Result = Club_Signer(club);
    if (!gSpecialVar_Result)
    {
        StringCopy(gStringVar4, sText_PasAssez);
        return;
    }
    fin = StringCopy(gStringVar4, sText_Signe);
    fin = StringCopy(fin, Club_Nom(club));
    StringCopy(fin, sText_Point);
}
