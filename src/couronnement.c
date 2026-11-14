#include "global.h"
#include "calendrier.h"
#include "couronnement.h"
#include "arenes.h"
#include "battle.h"
#include "battle_util.h"
#include "data.h"
#include "event_data.h"
#include "item.h"
#include "pokemon.h"
#include "string_util.h"
#include "move.h"
#include "random.h"
#include "constants/player_character.h"
#include "constants/items.h"
#include "constants/trainers.h"
#include "constants/battle_ai.h"
#include "constants/abilities.h"
#include "constants/moves.h"
#include "constants/songs.h"
#include "trainer_pools.h"
#include "battle_transition.h"
#include "item_icon.h"
#include "money.h"
#include "sprite.h"

// Émeraude Couronnement : Couronnement Mondial.
// - Elo de départ 1500 ; K = 32 ; attente E = 1 / (1 + 10^((Elo adverse - Elo joueur) / 400)).
// - Rang : 10 000 à 1500 d'Elo, divisé par ~1,12 tous les 10 points au-dessus (2000 -> ~32, Top 32 = Champions),
//   +20 places par point en dessous de 1500.
// - Paliers : Poké < 1300 <= Super < 1600 <= Hyper < 1800 <= Master ; Champions = Top 32.
// - Tournois : 3 combats en double (4 Pokémon choisis), soin entre chaque, une défaite élimine.
//   Un seul emplacement de dresseur (TRAINER_COURONNEMENT) ; l'équipe vient de gCouronnementTrainers.
// - Chaque adversaire montre 6 Pokémon ; le joueur en choisit 4 à chaque manche, puis l'adversaire garde
//   les 4 qui s'en sortent le mieux face à eux (Couronnement_ChoixAdverse).
// - Tournois 0-7 : villes d'Arène ; 8-11 : salles de Concours ; 12 : finale à 32 (Tour de Combat, 5 tours,
//   soin seulement avant la demi-finale, Blue puis Red).

const struct Trainer gCouronnementTrainers[DIFFICULTY_COUNT][COURONNEMENT_ADV_COUNT] =
{
#include "data/couronnement.h"
};

EWRAM_DATA u8 gCouronnementTexteEquipe[192] = {0};
// Adversaire de la manche en cours (copie modifiable : nom, et 4 Pokémon gardés sur 6).
static EWRAM_DATA struct Trainer sAdversaire = {0};
static EWRAM_DATA struct TrainerMon sAdversaireEquipe[COURONNEMENT_TAILLE_ADVERSE] = {0};
static EWRAM_DATA bool8 sAdversairePret = FALSE;

// 1000 * E pour un écart (Elo adverse - Elo joueur) de -800 à +800, pas de 50.
static const u16 sAttente[] =
{
    990, 987, 983, 977, 969, 960, 947, 930, 909, 882, 849, 808, 760, 703, 640, 571,
    500,
    429, 360, 297, 240, 192, 151, 118, 91, 70, 53, 40, 31, 23, 17, 13, 10,
};

// Condition d'accès de chaque tournoi (badge, ou Ligue battue pour la finale).
static const u16 sBadgeRequis[COURONNEMENT_NB_TOURNOIS_TOTAL] =
{
    FLAG_BADGE01_GET, FLAG_BADGE02_GET, FLAG_BADGE03_GET, FLAG_BADGE04_GET,
    FLAG_BADGE05_GET, FLAG_BADGE06_GET, FLAG_BADGE07_GET, FLAG_BADGE08_GET,
    FLAG_BADGE02_GET, FLAG_BADGE03_GET, FLAG_BADGE04_GET, FLAG_BADGE06_GET,
    FLAG_SYS_GAME_CLEAR,
};

static const u16 sRecompense[COURONNEMENT_NB_TOURNOIS_TOTAL] =
{
    ITEM_MUSCLE_BAND, ITEM_WISE_GLASSES, ITEM_EXPERT_BELT, ITEM_FOCUS_SASH,
    ITEM_LIFE_ORB, ITEM_CHOICE_SCARF, ITEM_CHOICE_BAND, ITEM_CHOICE_SPECS,
    ITEM_ROCKY_HELMET, ITEM_ASSAULT_VEST, ITEM_WEAKNESS_POLICY, ITEM_LEFTOVERS,
    ITEM_ABILITY_PATCH,
};

// Elo du premier adversaire de chaque tournoi (+50 par manche).
static const u16 sEloBase[COURONNEMENT_NB_TOURNOIS_TOTAL] =
{
    1350, 1410, 1470, 1530, 1590, 1650, 1710, 1770,
    1420, 1480, 1540, 1720,
    1950,
};

static u8 *AjouterNombre(u8 *fin, u32 n);

// ------------------------------------------------------------------ Saisons (v0.39)
// Une saison se termine à la finale (gagnée ou perdue) ; l'Elo revient à mi-chemin de 1500, les tournois se rejouent,
// les adversaires prennent +100 d'Elo par saison et passent au niveau 88-100 avec des équipes qui tournent,
// les finalistes changent. Récompenses : primes de palier (une fois par saison), prime de tournoi, prime de finale.
#define SAISON (gSaveBlock3Ptr->couronnementSaison)
#define POOL_DEBUT   12  // équipes des tournois 5 à 12 (évoluées) + 3 premiers tours de la finale
#define POOL_TAILLE  27

static const u8 sFinalistes[] = {39, 40, COURONNEMENT_ADV_FINALISTES_SAISON, COURONNEMENT_ADV_FINALISTES_SAISON + 1,
                                 COURONNEMENT_ADV_FINALISTES_SAISON + 2, COURONNEMENT_ADV_FINALISTES_SAISON + 3};

struct PrimePalier
{
    u32 argent;
    u16 objets[2];
};

static const struct PrimePalier sPrimesPaliers[] =
{
    [PALIER_POKE]      = {0,       {ITEM_NONE, ITEM_NONE}},
    [PALIER_SUPER]     = {30000,   {ITEM_BOTTLE_CAP, ITEM_NONE}},
    [PALIER_HYPER]     = {100000,  {ITEM_BOTTLE_CAP, ITEM_STRANGE_BALL}},
    [PALIER_MASTER]    = {300000,  {ITEM_GOLD_BOTTLE_CAP, ITEM_STRANGE_BALL}},
    [PALIER_CHAMPIONS] = {1000000, {ITEM_GOLD_BOTTLE_CAP, ITEM_ABILITY_PATCH}},
};

static u32 BitsJusquAuPalier(u32 palier)
{
    return (2u << palier) - 1;
}

static void SaisonVerifier(void)
{
    if (SAISON.magie == COURONNEMENT_SAISON_MAGIE)
        return;
    SAISON.magie = COURONNEMENT_SAISON_MAGIE;
    SAISON.saison = 1;
    SAISON.titres = (VarGet(VAR_COURONNEMENT_TOURNOIS) & (1 << COURONNEMENT_TOURNOI_FINALE)) ? 1 : 0;
    SAISON.finales = SAISON.titres;
    SAISON.paliers = BitsJusquAuPalier(Couronnement_PalierDepuisElo(Couronnement_GetElo())); // pas de rattrapage
}

u32 Couronnement_Saison(void)
{
    SaisonVerifier();
    return SAISON.saison;
}

u32 Couronnement_Titres(void)
{
    SaisonVerifier();
    return SAISON.titres;
}

// Primes x1 en saison 1, +50 % par saison, x3 au maximum.
u32 Couronnement_MultiplicateurSaison(void)
{
    u32 m = 100 + 50 * (Couronnement_Saison() - 1);
    return m > 300 ? 300 : m;
}

// Équipe réellement jouée par l'emplacement « index » selon la saison.
u32 Couronnement_EquipeDeLaSaison(u32 index)
{
    u32 s = Couronnement_Saison(), finale = COURONNEMENT_TOURNOI_FINALE * COURONNEMENT_MANCHES;

    if (s < 2 || index >= COURONNEMENT_ADV_FINALISTES_SAISON)
        return index;
    if (index == finale + COURONNEMENT_DEMI_FINALE)
        return sFinalistes[(2 * s) % ARRAY_COUNT(sFinalistes)];
    if (index == finale + COURONNEMENT_MANCHES_FINALE - 1)
        return sFinalistes[(2 * s + 1) % ARRAY_COUNT(sFinalistes)];
    return POOL_DEBUT + (index * 7 + s * 11 + (index >= finale ? 5 : 0)) % POOL_TAILLE;
}

// Niveau des adversaires à partir de la saison 2 (0 = niveau des données).
u32 Couronnement_NiveauDeLaSaison(u32 index)
{
    u32 tournoi = Couronnement_TournoiDepuisAdversaire(index), niveau;

    if (Couronnement_Saison() < 2)
        return 0;
    if (tournoi == COURONNEMENT_TOURNOI_FINALE)
        return MAX_LEVEL;
    niveau = 88 + tournoi + (index - Couronnement_PremierAdversaire(tournoi));
    return niveau > MAX_LEVEL ? MAX_LEVEL : niveau;
}

static const u8 sText_Poke[] = _("Poké Ball");
static const u8 sText_Super[] = _("Super Ball");
static const u8 sText_Hyper[] = _("Hyper Ball");
static const u8 sText_Master[] = _("Master Ball");
static const u8 sText_Champions[] = _("Champions");
static const u8 *const sNomsPaliers[] = { sText_Poke, sText_Super, sText_Hyper, sText_Master, sText_Champions };

static const u8 sText_Statut[] = _("Rang mondial : {STR_VAR_2}\nPalier : {STR_VAR_1}\pElo : {STR_VAR_3}");
static const u8 sText_EquipeDe[] = _("Équipe de ");
static const u8 sText_DeuxPoints[] = _(" :\n");
static const u8 sText_Virgule[] = _(", ");
static const u8 sText_VirguleLigne[] = _(",\l");
static const u8 sText_Et[] = _(" et ");
static const u8 sText_Point[] = _(".");
static const u8 sText_Plus[] = _("+");
static const u8 sText_Saison[] = _("\pSaison ");
static const u8 sText_SaisonFinales[] = _(". Finales jouées : ");
static const u8 sText_Yens[] = _(" ¥");
static const u8 sText_PrimeTournoi[] = _("Prime de saison : ");
static const u8 sText_PrimeTournoiFin[] = _(" ¥ !\nEt ton Elo a monté.");
static const u8 sText_Moins[] = _("-");
static const u8 sText_Green[] = _("Green");
static const u8 sText_Red[] = _("Red");

u16 Couronnement_GetElo(void)
{
    u16 elo = VarGet(VAR_COURONNEMENT_ELO);
    return elo == 0 ? COURONNEMENT_ELO_DEPART : elo;
}

void Couronnement_SetElo(u16 elo)
{
    if (elo < COURONNEMENT_ELO_MIN)
        elo = COURONNEMENT_ELO_MIN;
    if (elo > COURONNEMENT_ELO_MAX)
        elo = COURONNEMENT_ELO_MAX;
    VarSet(VAR_COURONNEMENT_ELO, elo);
}

u32 Couronnement_RangDepuisElo(u16 elo)
{
    u32 rang, steps, i;

    if (elo <= COURONNEMENT_ELO_DEPART)
    {
        rang = COURONNEMENT_RANG_DEPART + (COURONNEMENT_ELO_DEPART - elo) * 20;
        return rang;
    }
    // 0.8916 ~= 913/1024 par tranche de 10 points ; rang gardé en 1/1024 pour la précision.
    rang = COURONNEMENT_RANG_DEPART * 1024;
    steps = (elo - COURONNEMENT_ELO_DEPART) / 10;
    for (i = 0; i < steps && rang > 1024; i++)
        rang = rang / 1024 * 913 + (rang % 1024) * 913 / 1024;
    rang /= 1024;
    return rang < 1 ? 1 : rang;
}

u32 Couronnement_PalierDepuisElo(u16 elo)
{
    if (Couronnement_RangDepuisElo(elo) <= COURONNEMENT_TOP_CHAMPIONS)
        return PALIER_CHAMPIONS;
    if (elo >= COURONNEMENT_SEUIL_MASTER)
        return PALIER_MASTER;
    if (elo >= COURONNEMENT_SEUIL_HYPER)
        return PALIER_HYPER;
    if (elo >= COURONNEMENT_SEUIL_SUPER)
        return PALIER_SUPER;
    return PALIER_POKE;
}

s32 Couronnement_VariationElo(u16 eloJoueur, u16 eloAdversaire, bool32 victoire)
{
    s32 diff = (s32)eloAdversaire - (s32)eloJoueur;
    s32 i, frac, attente, score;

    if (diff < -800)
        diff = -800;
    if (diff > 800)
        diff = 800;
    i = (diff + 800) / 50;
    frac = (diff + 800) % 50;
    attente = sAttente[i];
    if (frac && i + 1 < (s32)ARRAY_COUNT(sAttente))
        attente += ((s32)sAttente[i + 1] - (s32)sAttente[i]) * frac / 50;
    score = victoire ? 1000 : 0;
    // arrondi au plus proche
    if (score >= attente)
        return (COURONNEMENT_K * (score - attente) + 500) / 1000;
    return -((COURONNEMENT_K * (attente - score) + 500) / 1000);
}

u32 Couronnement_PremierAdversaire(u32 tournoi)
{
    return tournoi * COURONNEMENT_MANCHES;  // la finale (12) commence à 36
}

u32 Couronnement_NbManches(u32 tournoi)
{
    return tournoi == COURONNEMENT_TOURNOI_FINALE ? COURONNEMENT_MANCHES_FINALE : COURONNEMENT_MANCHES;
}

u32 Couronnement_TournoiDepuisAdversaire(u32 indexAdversaire)
{
    u32 t = indexAdversaire / COURONNEMENT_MANCHES;
    return t > COURONNEMENT_TOURNOI_FINALE ? COURONNEMENT_TOURNOI_FINALE : t;
}

u16 Couronnement_EloAdversaire(u32 indexAdversaire)
{
    u32 tournoi = Couronnement_TournoiDepuisAdversaire(indexAdversaire);
    u32 manche = indexAdversaire - Couronnement_PremierAdversaire(tournoi);
    return sEloBase[tournoi] + 50 * manche + 100 * (Couronnement_Saison() - 1);
}

u32 Couronnement_NbTournoisGagnes(void)
{
    u32 bits = VarGet(VAR_COURONNEMENT_TOURNOIS) & ((1 << COURONNEMENT_NB_TOURNOIS) - 1), n = 0;
    while (bits)
    {
        n += bits & 1;
        bits >>= 1;
    }
    return n;
}

static const struct Trainer *AdversaireDeBase(void)
{
    u32 index = VarGet(VAR_COURONNEMENT_ADVERSAIRE);
    if (index >= COURONNEMENT_ADV_COUNT)
        index = 0;
    return &gCouronnementTrainers[DIFFICULTY_NORMAL][Couronnement_EquipeDeLaSaison(index)];
}

const struct Trainer *GetCouronnementTrainer(void)
{
    if (Arene_CombatEnCours()) // v0.40 : challenger d'une arène du joueur
        return Arene_Challenger();
    return sAdversairePret ? &sAdversaire : AdversaireDeBase();
}

// Copie l'adversaire de la manche dans sAdversaire (6 Pokémon). Si le joueur joue Red, Red devient Green.
static void PreparerAdversaire(void)
{
    const struct Trainer *base = AdversaireDeBase();
    u32 i, niveau = Couronnement_NiveauDeLaSaison(VarGet(VAR_COURONNEMENT_ADVERSAIRE));

    sAdversaire = *base;
    for (i = 0; i < base->partySize && i < COURONNEMENT_TAILLE_ADVERSE; i++)
    {
        sAdversaireEquipe[i] = base->party[i];
        if (niveau)
            sAdversaireEquipe[i].lvl = niveau;
    }
    sAdversaire.party = sAdversaireEquipe;
    if (base->trainerPic == TRAINER_PIC_LEAF && VarGet(VAR_PLAYER_CHARACTER) == PLAYER_CHAR_GREEN)
    {
        sAdversaire.trainerPic = TRAINER_PIC_RED;
        sAdversaire.gender = FALSE;
        StringCopy(sAdversaire.trainerName, sText_Red);
    }
    if (base->trainerPic == TRAINER_PIC_RED && VarGet(VAR_PLAYER_CHARACTER) == PLAYER_CHAR_RED)
    {
        sAdversaire.trainerPic = TRAINER_PIC_LEAF;
        sAdversaire.gender = TRUE;
        StringCopy(sAdversaire.trainerName, sText_Green);
    }
    sAdversairePret = TRUE;
}

// Efficacité (x4 = 1.0) d'une attaque de type atkType sur une espèce.
static u32 Efficacite(enum Type atkType, u16 species)
{
    uq4_12_t mod = GetTypeModifier(atkType, GetSpeciesType(species, 0));
    if (GetSpeciesType(species, 1) != GetSpeciesType(species, 0))
        mod = uq4_12_multiply(mod, GetTypeModifier(atkType, GetSpeciesType(species, 1)));
    return mod * 4 / UQ_4_12(1.0);  // 0, 1 (x0,25), 2 (x0,5), 4, 8, 16
}

static u32 MeilleureEfficacite(const enum Move *moves, u16 cible)
{
    u32 i, best = 0, e;
    for (i = 0; i < MAX_MON_MOVES; i++)
    {
        if (moves[i] == MOVE_NONE || GetMoveCategory(moves[i]) == DAMAGE_CATEGORY_STATUS || GetMovePower(moves[i]) == 0)
            continue;
        e = Efficacite(GetMoveType(moves[i]), cible);
        if (e > best)
            best = e;
    }
    return best;
}

// Score d'un Pokémon adverse face aux 4 Pokémon choisis par le joueur.
s32 Couronnement_ScoreAdverse(const struct TrainerMon *mon, struct Pokemon *joueur, u32 nbJoueur)
{
    s32 score = 0;
    u32 j, k;
    enum Move moves[MAX_MON_MOVES];

    for (j = 0; j < nbJoueur; j++)
    {
        u16 sp = GetMonData(&joueur[j], MON_DATA_SPECIES);
        if (sp == SPECIES_NONE)
            continue;
        score += MeilleureEfficacite(mon->moves, sp);
        for (k = 0; k < MAX_MON_MOVES; k++)
            moves[k] = GetMonData(&joueur[j], MON_DATA_MOVE1 + k);
        score -= MeilleureEfficacite(moves, mon->species);
    }
    return score;
}

static bool32 IsLegendaire(u16 species)
{
    return gSpeciesInfo[species].isRestrictedLegendary
        || gSpeciesInfo[species].isSubLegendary
        || gSpeciesInfo[species].isMythical;
}

// Un seul légendaire par équipe.
bool32 Couronnement_EquipeValide(const u16 *species, u32 count)
{
    u32 i, legendaires = 0;
    for (i = 0; i < count; i++)
    {
        if (species[i] != SPECIES_NONE && IsLegendaire(species[i]))
            legendaires++;
    }
    return legendaires <= 1;
}

// ------------------------------------------------------------------ specials

void Couronnement_DonnerPass(void)
{
    FlagSet(FLAG_COURONNEMENT_PASS);
    if (VarGet(VAR_COURONNEMENT_ELO) == 0)
        VarSet(VAR_COURONNEMENT_ELO, COURONNEMENT_ELO_DEPART);
}

void Couronnement_BufferStatut(void)
{
    u16 elo = Couronnement_GetElo();
    u32 rang = Couronnement_RangDepuisElo(elo);

    StringCopy(gStringVar1, sNomsPaliers[Couronnement_PalierDepuisElo(elo)]);
    ConvertIntToDecimalStringN(gStringVar2, rang, STR_CONV_MODE_LEFT_ALIGN, 6);
    ConvertIntToDecimalStringN(gStringVar3, elo, STR_CONV_MODE_LEFT_ALIGN, 4);
    StringExpandPlaceholders(gCouronnementTexteEquipe, sText_Statut);
    {
        u8 *fin = gCouronnementTexteEquipe + StringLength(gCouronnementTexteEquipe);
        fin = StringCopy(fin, sText_Saison);
        fin = AjouterNombre(fin, Couronnement_Saison());
        fin = StringCopy(fin, sText_SaisonFinales);
        fin = AjouterNombre(fin, SAISON.finales);
        StringCopy(fin, sText_Point);
    }
    // Tournois gagnés, pour le message suivant.
    ConvertIntToDecimalStringN(gStringVar1, Couronnement_NbTournoisGagnes(), STR_CONV_MODE_LEFT_ALIGN, 2);
}

// VAR_0x8004 = tournoi -> VAR_RESULT = TOURNOI_*
void Couronnement_EtatTournoi(void)
{
    u32 t = gSpecialVar_0x8004;

    if (!FlagGet(FLAG_COURONNEMENT_PASS))
        gSpecialVar_Result = TOURNOI_PAS_DE_PASS;
    else if (t >= COURONNEMENT_NB_TOURNOIS_TOTAL)
        gSpecialVar_Result = TOURNOI_BADGE_MANQUANT;
    else if (!FlagGet(sBadgeRequis[t]))
        gSpecialVar_Result = t == COURONNEMENT_TOURNOI_FINALE ? TOURNOI_LIGUE_MANQUANTE : TOURNOI_BADGE_MANQUANT;
    else if (t == COURONNEMENT_TOURNOI_FINALE && Couronnement_PalierDepuisElo(Couronnement_GetElo()) != PALIER_CHAMPIONS)
    {
        gSpecialVar_Result = TOURNOI_PAS_TOP32;
        ConvertIntToDecimalStringN(gStringVar1, Couronnement_RangDepuisElo(Couronnement_GetElo()), STR_CONV_MODE_LEFT_ALIGN, 6);
    }
    else if (VarGet(VAR_COURONNEMENT_TOURNOIS) & (1 << t))
        gSpecialVar_Result = TOURNOI_DEJA_GAGNE;
    else
        gSpecialVar_Result = TOURNOI_OUVERT;
}

void Couronnement_CompterPokemonAptes(void)
{
    u32 i, n = 0;
    for (i = 0; i < PARTY_SIZE; i++)
    {
        struct Pokemon *mon = &gParties[B_TRAINER_PLAYER][i];
        if (GetMonData(mon, MON_DATA_SPECIES) != SPECIES_NONE && !GetMonData(mon, MON_DATA_IS_EGG))
            n++;
    }
    gSpecialVar_Result = n;
}

// VAR_0x8004 = tournoi
void Couronnement_DebutTournoi(void)
{
    VarSet(VAR_COURONNEMENT_ADVERSAIRE, Couronnement_PremierAdversaire(gSpecialVar_0x8004));
}

// STR_VAR_1 = nom, STR_VAR_2 = manche, STR_VAR_3 = Elo adverse ; gCouronnementTexteEquipe = les 6 Pokémon montrés.
// VAR_RESULT = numéro de manche (0..), VAR_0x8005 = nombre de manches.
void Couronnement_PreparerManche(void)
{
    u32 index = VarGet(VAR_COURONNEMENT_ADVERSAIRE);
    u32 tournoi = Couronnement_TournoiDepuisAdversaire(index);
    u32 manche = index - Couronnement_PremierAdversaire(tournoi);
    const struct Trainer *trainer;
    u8 *s;
    u32 i;

    PreparerAdversaire();
    trainer = &sAdversaire;
    gSpecialVar_Result = manche;
    gSpecialVar_0x8005 = Couronnement_NbManches(tournoi);
    StringCopy(gStringVar1, trainer->trainerName);
    ConvertIntToDecimalStringN(gStringVar2, manche + 1, STR_CONV_MODE_LEFT_ALIGN, 1);
    ConvertIntToDecimalStringN(gStringVar3, Couronnement_EloAdversaire(index), STR_CONV_MODE_LEFT_ALIGN, 4);

    s = StringCopy(gCouronnementTexteEquipe, sText_EquipeDe);
    s = StringAppend(s, trainer->trainerName);
    s = StringAppend(s, sText_DeuxPoints);
    for (i = 0; i < trainer->partySize; i++)
    {
        if (i > 0)
            s = StringAppend(s, i + 1 == trainer->partySize ? sText_Et : ((i == 2 || i == 4) ? sText_VirguleLigne : sText_Virgule));
        s = StringAppend(s, GetSpeciesName(trainer->party[i].species));
    }
    StringAppend(s, sText_Point);
}

// Après le choix du joueur : l'adversaire garde ses 4 meilleurs Pokémon face à l'équipe choisie.
void Couronnement_ChoixAdverse(void)
{
    s32 scores[COURONNEMENT_TAILLE_ADVERSE];
    u8 ordre[COURONNEMENT_TAILLE_ADVERSE];
    struct TrainerMon gardes[COURONNEMENT_TAILLE_EQUIPE];
    u32 i, j, n;

    if (!sAdversairePret)
        PreparerAdversaire();
    n = sAdversaire.partySize;
    if (n <= COURONNEMENT_TAILLE_EQUIPE)
        return;
    for (i = 0; i < n; i++)
    {
        // petit aléa pour varier les équipes d'une tentative à l'autre
        scores[i] = Couronnement_ScoreAdverse(&sAdversaireEquipe[i], gParties[B_TRAINER_PLAYER], COURONNEMENT_TAILLE_EQUIPE) * 4
                  + (Random() % 5);
        ordre[i] = i;
    }
    for (i = 0; i < n; i++)
        for (j = i + 1; j < n; j++)
            if (scores[ordre[j]] > scores[ordre[i]])
            {
                u8 tmp = ordre[i];
                ordre[i] = ordre[j];
                ordre[j] = tmp;
            }
    for (i = 0; i < COURONNEMENT_TAILLE_EQUIPE; i++)
        gardes[i] = sAdversaireEquipe[ordre[i]];
    for (i = 0; i < COURONNEMENT_TAILLE_EQUIPE; i++)
        sAdversaireEquipe[i] = gardes[i];
    sAdversaire.partySize = COURONNEMENT_TAILLE_EQUIPE;
}

// Après le combat : VAR_RESULT = victoire ; STR_VAR_1 = variation (+12 / -8), STR_VAR_2 = nouvel Elo.
void Couronnement_EnregistrerResultat(void)
{
    bool32 victoire = (gBattleOutcome == B_OUTCOME_WON);
    u16 elo = Couronnement_GetElo();
    s32 delta = Couronnement_VariationElo(elo, Couronnement_EloAdversaire(VarGet(VAR_COURONNEMENT_ADVERSAIRE)), victoire);
    u8 *s;

    Couronnement_SetElo((s32)elo + delta);
    s = StringCopy(gStringVar1, delta >= 0 ? sText_Plus : sText_Moins);
    ConvertIntToDecimalStringN(s, delta >= 0 ? delta : -delta, STR_CONV_MODE_LEFT_ALIGN, 3);
    ConvertIntToDecimalStringN(gStringVar2, Couronnement_GetElo(), STR_CONV_MODE_LEFT_ALIGN, 4);
    gSpecialVar_Result = victoire;
}

// VAR_RESULT = TRUE s'il reste une manche.
void Couronnement_MancheSuivante(void)
{
    u32 index = VarGet(VAR_COURONNEMENT_ADVERSAIRE);
    u32 tournoi = Couronnement_TournoiDepuisAdversaire(index);
    sAdversairePret = FALSE;
    VarSet(VAR_COURONNEMENT_ADVERSAIRE, index + 1);
    gSpecialVar_Result = (index + 1 - Couronnement_PremierAdversaire(tournoi)) < Couronnement_NbManches(tournoi);
}

// Fin de tournoi (victoire ou non) : l'adversaire redevient celui des données.
void Couronnement_FinTournoi(void)
{
    sAdversairePret = FALSE;
}

// Marque le tournoi courant comme gagné ; VAR_0x8006 = récompense (ITEM_NONE si déjà gagné),
// VAR_RESULT = TRUE si c'est la finale.
void Couronnement_TournoiGagne(void)
{
    u32 t = Couronnement_TournoiDepuisAdversaire(VarGet(VAR_COURONNEMENT_ADVERSAIRE));
    u16 bits = VarGet(VAR_COURONNEMENT_TOURNOIS);

    gSpecialVar_0x8006 = (bits & (1 << t)) ? ITEM_NONE : sRecompense[t];
    gSpecialVar_0x8007 = FALSE;
    // À partir de la saison 2 : prime en argent au lieu de l'objet (déjà reçu en saison 1).
    if (Couronnement_Saison() >= 2 && gSpecialVar_0x8006 != ITEM_NONE)
    {
        gSpecialVar_0x8006 = ITEM_NONE;
        if (t != COURONNEMENT_TOURNOI_FINALE)
        {
            u32 prime = (20000 + 10000 * t) * Couronnement_MultiplicateurSaison() / 100;
            u8 *fin = StringCopy(gStringVar4, sText_PrimeTournoi);
            fin = AjouterNombre(fin, prime);
            StringCopy(fin, sText_PrimeTournoiFin);
            AddMoney(&gSaveBlock1Ptr->money, prime);
            gSpecialVar_0x8007 = TRUE;
        }
    }
    VarSet(VAR_COURONNEMENT_TOURNOIS, bits | (1 << t));
    gSpecialVar_Result = (t == COURONNEMENT_TOURNOI_FINALE);
}

// VAR_RESULT = TRUE si la finale a été gagnée.
// Menu L : STR_VAR_1 / STR_VAR_2 = libellés avec l'état actuel de la Repousse Infinie et de la course.
void Couronnement_BufferMenuL(void)
{
    static const u8 sText_RepousseOui[] = _("Repousse Infinie : OUI");
    static const u8 sText_RepousseNon[] = _("Repousse Infinie : NON");
    static const u8 sText_CourseAuto[] = _("Course : AUTO");
    static const u8 sText_CourseB[] = _("Course : BOUTON B");

    StringCopy(gStringVar1, FlagGet(FLAG_REPOUSSE_INFINIE) ? sText_RepousseOui : sText_RepousseNon);
    StringCopy(gStringVar2, FlagGet(FLAG_COURSE_AUTO) ? sText_CourseAuto : sText_CourseB);
    StringCopy(gStringVar3, Calendrier_NomJour(Calendrier_JourSemaine(Calendrier_Aujourdhui()))); // v0.37
}

void Couronnement_EstChampionDuMonde(void)
{
    gSpecialVar_Result = Couronnement_Titres() > 0 || (VarGet(VAR_COURONNEMENT_TOURNOIS) & (1 << COURONNEMENT_TOURNOI_FINALE));
    ConvertIntToDecimalStringN(gStringVar1, max(Couronnement_Titres(), 1), STR_CONV_MODE_LEFT_ALIGN, 3);
}

// ------------------------------------------------------------------ Saisons : specials (v0.39)
static const u8 sText_PalierAtteint[] = _("Palier ");
static const u8 sText_PalierAtteint2[] = _(" atteint !\nPrime de saison : ");
static const u8 sText_TuRecois[] = _("\pTu reçois aussi :\n");
static const u8 sText_Fois[] = _(" x");
static const u8 sText_FinSaison1[] = _("Fin de la saison ");
static const u8 sText_FinSaison2[] = _(" !\nPrime de finale : ");
static const u8 sText_FinSaisonTitre[] = _("\pChampion du Monde : ");
static const u8 sText_FinSaisonTitre2[] = _(" titre(s).\nBonus : Master Ball, Capsule d'Or.");

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

static u8 *AjouterYens(u8 *fin, u32 n)
{
    fin = AjouterNombre(fin, n);
    return StringCopy(fin, sText_Yens);
}

// Après une victoire : prime du palier atteint pour la première fois cette saison.
// VAR_RESULT = TRUE si une prime est donnée (texte dans gStringVar4).
void Couronnement_RecompensePalier(void)
{
    u32 palier = Couronnement_PalierDepuisElo(Couronnement_GetElo()), p, i, j, argent = 0, n = 0;
    u16 objets[8];
    u8 nb[8];
    u8 *fin;

    SaisonVerifier();
    gSpecialVar_Result = FALSE;
    if ((SAISON.paliers & BitsJusquAuPalier(palier)) == BitsJusquAuPalier(palier))
        return;
    for (p = PALIER_SUPER; p <= palier; p++)
    {
        if (SAISON.paliers & (1 << p))
            continue;
        argent += sPrimesPaliers[p].argent;
        for (i = 0; i < ARRAY_COUNT(sPrimesPaliers[p].objets); i++)
        {
            u16 objet = sPrimesPaliers[p].objets[i];
            if (objet == ITEM_NONE)
                continue;
            for (j = 0; j < n && objets[j] != objet; j++)
                ;
            if (j == n)
            {
                objets[n] = objet;
                nb[n++] = 0;
            }
            nb[j]++;
        }
    }
    SAISON.paliers |= BitsJusquAuPalier(palier);
    argent = (u64)argent * Couronnement_MultiplicateurSaison() / 100;
    AddMoney(&gSaveBlock1Ptr->money, argent);

    fin = StringCopy(gStringVar4, sText_PalierAtteint);
    fin = StringCopy(fin, sNomsPaliers[palier]);
    fin = StringCopy(fin, sText_PalierAtteint2);
    fin = AjouterYens(fin, argent);
    fin = StringCopy(fin, sText_Point);
    for (i = 0; i < n; i++)
    {
        AddBagItem(objets[i], nb[i]);
        fin = StringCopy(fin, i == 0 ? sText_TuRecois : sText_VirguleLigne);
        fin = StringCopy(fin, GetItemName(objets[i]));
        if (nb[i] > 1)
        {
            fin = StringCopy(fin, sText_Fois);
            fin = AjouterNombre(fin, nb[i]);
        }
    }
    if (n)
        StringCopy(fin, sText_Point);
    gSpecialVar_Result = TRUE;
}

// VAR_RESULT = TRUE si le tournoi en cours (ou qui vient de finir) est la finale.
void Couronnement_EtaitFinale(void)
{
    gSpecialVar_Result = Couronnement_TournoiDepuisAdversaire(VarGet(VAR_COURONNEMENT_ADVERSAIRE)) == COURONNEMENT_TOURNOI_FINALE;
}

// Fin de saison après la finale. VAR_0x8004 = victoire. gStringVar4 = bilan ; STR_VAR_1 = nouvelle saison, STR_VAR_2 = Elo.
void Couronnement_FinSaison(void)
{
    bool32 victoire = gSpecialVar_0x8004;
    u32 m = Couronnement_MultiplicateurSaison(), argent = 500000 * m / 100;
    s32 elo;
    u8 *fin;

    SaisonVerifier();
    if (SAISON.finales < 255)
        SAISON.finales++;
    if (victoire)
    {
        argent += 3000000 * m / 100;
        if (SAISON.titres < 255)
            SAISON.titres++;
        AddBagItem(ITEM_MASTER_BALL, 1);
        AddBagItem(ITEM_GOLD_BOTTLE_CAP, 1);
    }
    AddMoney(&gSaveBlock1Ptr->money, argent);
    fin = StringCopy(gStringVar4, sText_FinSaison1);
    fin = AjouterNombre(fin, SAISON.saison);
    fin = StringCopy(fin, sText_FinSaison2);
    fin = AjouterYens(fin, argent);
    fin = StringCopy(fin, sText_Point);
    if (victoire)
    {
        fin = StringCopy(fin, sText_FinSaisonTitre);
        fin = AjouterNombre(fin, SAISON.titres);
        StringCopy(fin, sText_FinSaisonTitre2);
    }

    // Nouvelle saison
    if (SAISON.saison < 255)
        SAISON.saison++;
    elo = Couronnement_GetElo();
    elo = COURONNEMENT_ELO_DEPART + (elo - COURONNEMENT_ELO_DEPART) / 2;
    Couronnement_SetElo(elo);
    VarSet(VAR_COURONNEMENT_TOURNOIS, 0);
    SAISON.paliers = BitsJusquAuPalier(Couronnement_PalierDepuisElo(Couronnement_GetElo()));
    AjouterNombre(gStringVar1, SAISON.saison);
    AjouterNombre(gStringVar2, Couronnement_GetElo());
}

// Réplique du finaliste avant la demi-finale ou la finale (gStringVar4), selon son sprite.
void Couronnement_TexteFinaliste(void)
{
    static const u8 sText_Blue[] = _("Blue : Tiens, tiens. La recrue\nd'Hoenn.\pJ'ai été Champion avant que tu\nsaches marcher. Montre-moi si tu\lmérites la finale.");
    static const u8 sText_RedMuet[] = _("…\pRed te fixe sans un mot.\nLa finale commence.");
    static const u8 sText_GreenF[] = _("Green : Alors c'est toi, le\nphénomène de Hoenn ?\pVoyons si la rumeur dit vrai.\nJe ne retiens jamais mes coups.");
    static const u8 sText_Peter[] = _("Peter : Mes dragons n'ont jamais\nperdu une finale.\pCe n'est pas aujourd'hui que ça\nva commencer.");
    static const u8 sText_Pierre[] = _("Pierre : J'ai lâché le titre pour\naller creuser des cailloux.\pMais pour toi, je ressors\nl'artillerie lourde.");
    static const u8 sText_Marc[] = _("Marc : Le Champion de Hoenn\ncontre son meilleur Dresseur.\pLe public attendait ça depuis\ndes mois. Ne le déçois pas.");
    const u8 *texte;

    if (!sAdversairePret)
        PreparerAdversaire();
    switch (sAdversaire.trainerPic)
    {
    case TRAINER_PIC_RED:                     texte = sText_RedMuet; break;
    case TRAINER_PIC_LEAF:                    texte = sText_GreenF; break;
    case TRAINER_PIC_ELITE_FOUR_LANCE_FRLG:   texte = sText_Peter; break;
    case TRAINER_PIC_STEVEN:                  texte = sText_Pierre; break;
    case TRAINER_PIC_CHAMPION_WALLACE:        texte = sText_Marc; break;
    default:                                  texte = sText_Blue; break;
    }
    StringCopy(gStringVar4, texte);
}


// ------------------------------------------------------------------ Icône de la Ball du palier (statut, menu L)
#define TAG_BALLE_PALIER 0x5DB1
EWRAM_DATA static u8 sBallePalierSprite = 0; // id du sprite + 1 (0 = aucun)

void Couronnement_AfficherBallePalier(void)
{
    static const u16 sBalles[] = {
        [PALIER_POKE] = ITEM_POKE_BALL, [PALIER_SUPER] = ITEM_GREAT_BALL, [PALIER_HYPER] = ITEM_ULTRA_BALL,
        [PALIER_MASTER] = ITEM_MASTER_BALL, [PALIER_CHAMPIONS] = ITEM_MASTER_BALL,
    };
    u8 id = AddItemIconSprite(TAG_BALLE_PALIER, TAG_BALLE_PALIER, sBalles[Couronnement_PalierDepuisElo(Couronnement_GetElo())]);

    if (id != MAX_SPRITES)
    {
        sBallePalierSprite = id + 1;
        gSprites[id].x2 = 213; // dans le coin droit de la boîte de message
        gSprites[id].y2 = 140;
        gSprites[id].oam.priority = 0;
    }
}

void Couronnement_CacherBallePalier(void)
{
    if (sBallePalierSprite == 0)
        return;
    FreeSpriteTilesByTag(TAG_BALLE_PALIER);
    FreeSpritePaletteByTag(TAG_BALLE_PALIER);
    FreeSpriteOamMatrix(&gSprites[sBallePalierSprite - 1]);
    DestroySprite(&gSprites[sBallePalierSprite - 1]);
    sBallePalierSprite = 0;
}

// ------------------------------------------------------------------ Menu L : Type Téra
// VAR_0x8004 = Pokémon de l'équipe, VAR_0x8005 = choix dans la liste du script (même ordre que sTypesTera).
void Couronnement_ChangerTypeTera(void)
{
    static const u8 sTypesTera[] = {
        TYPE_NORMAL, TYPE_FIRE, TYPE_WATER, TYPE_GRASS, TYPE_ELECTRIC, TYPE_ICE, TYPE_FIGHTING, TYPE_POISON, TYPE_GROUND,
        TYPE_FLYING, TYPE_PSYCHIC, TYPE_BUG, TYPE_ROCK, TYPE_GHOST, TYPE_DRAGON, TYPE_DARK, TYPE_STEEL, TYPE_FAIRY,
    };
    u8 type;

    if (gSpecialVar_0x8004 >= PARTY_SIZE || gSpecialVar_0x8005 >= ARRAY_COUNT(sTypesTera))
        return;
    type = sTypesTera[gSpecialVar_0x8005];
    SetMonData(&gPlayerParty[gSpecialVar_0x8004], MON_DATA_TERA_TYPE, &type);
}
