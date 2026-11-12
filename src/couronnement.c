#include "global.h"
#include "couronnement.h"
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

static const u8 sText_Poke[] = _("Poké");
static const u8 sText_Super[] = _("Super");
static const u8 sText_Hyper[] = _("Hyper");
static const u8 sText_Master[] = _("Master");
static const u8 sText_Champions[] = _("Champions");
static const u8 *const sNomsPaliers[] = { sText_Poke, sText_Super, sText_Hyper, sText_Master, sText_Champions };

static const u8 sText_Statut[] = _("Couronnement Mondial\nPalier {STR_VAR_1}\pRang {STR_VAR_2}\nElo : {STR_VAR_3}");
static const u8 sText_EquipeDe[] = _("Équipe de ");
static const u8 sText_DeuxPoints[] = _(" :\n");
static const u8 sText_Virgule[] = _(", ");
static const u8 sText_VirguleLigne[] = _(",\l");
static const u8 sText_Et[] = _(" et ");
static const u8 sText_Point[] = _(".");
static const u8 sText_Plus[] = _("+");
static const u8 sText_Moins[] = _("-");
static const u8 sText_Green[] = _("Green");

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
    return sEloBase[tournoi] + 50 * manche;
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
    return &gCouronnementTrainers[DIFFICULTY_NORMAL][index];
}

const struct Trainer *GetCouronnementTrainer(void)
{
    return sAdversairePret ? &sAdversaire : AdversaireDeBase();
}

// Copie l'adversaire de la manche dans sAdversaire (6 Pokémon). Si le joueur joue Red, Red devient Green.
static void PreparerAdversaire(void)
{
    const struct Trainer *base = AdversaireDeBase();
    u32 i;

    sAdversaire = *base;
    for (i = 0; i < base->partySize && i < COURONNEMENT_TAILLE_ADVERSE; i++)
        sAdversaireEquipe[i] = base->party[i];
    sAdversaire.party = sAdversaireEquipe;
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
}

void Couronnement_EstChampionDuMonde(void)
{
    gSpecialVar_Result = (VarGet(VAR_COURONNEMENT_TOURNOIS) & (1 << COURONNEMENT_TOURNOI_FINALE)) != 0;
}
