#include "global.h"
#include "couronnement.h"
#include "battle.h"
#include "data.h"
#include "event_data.h"
#include "item.h"
#include "pokemon.h"
#include "string_util.h"
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

const struct Trainer gCouronnementTrainers[DIFFICULTY_COUNT][COURONNEMENT_ADV_COUNT] =
{
#include "data/couronnement.h"
};

EWRAM_DATA u8 gCouronnementTexteEquipe[128] = {0};

// 1000 * E pour un écart (Elo adverse - Elo joueur) de -800 à +800, pas de 50.
static const u16 sAttente[] =
{
    990, 987, 983, 977, 969, 960, 947, 930, 909, 882, 849, 808, 760, 703, 640, 571,
    500,
    429, 360, 297, 240, 192, 151, 118, 91, 70, 53, 40, 31, 23, 17, 13, 10,
};

// Badge requis pour chaque tournoi de ville.
static const u16 sBadgeRequis[COURONNEMENT_NB_TOURNOIS_VILLES] =
{
    FLAG_BADGE01_GET, FLAG_BADGE02_GET, FLAG_BADGE03_GET, FLAG_BADGE04_GET,
    FLAG_BADGE05_GET, FLAG_BADGE06_GET, FLAG_BADGE07_GET, FLAG_BADGE08_GET,
};

static const u16 sRecompense[COURONNEMENT_NB_TOURNOIS_VILLES] =
{
    ITEM_MUSCLE_BAND, ITEM_WISE_GLASSES, ITEM_EXPERT_BELT, ITEM_FOCUS_SASH,
    ITEM_LIFE_ORB, ITEM_CHOICE_SCARF, ITEM_CHOICE_BAND, ITEM_CHOICE_SPECS,
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

u16 Couronnement_EloAdversaire(u32 indexAdversaire)
{
    u32 tournoi = indexAdversaire / COURONNEMENT_MANCHES;
    u32 manche = indexAdversaire % COURONNEMENT_MANCHES;
    return 1350 + 50 * tournoi + 40 * manche;
}

u32 Couronnement_NbTournoisGagnes(void)
{
    u32 bits = VarGet(VAR_COURONNEMENT_TOURNOIS), n = 0;
    while (bits)
    {
        n += bits & 1;
        bits >>= 1;
    }
    return n;
}

const struct Trainer *GetCouronnementTrainer(void)
{
    u32 index = VarGet(VAR_COURONNEMENT_ADVERSAIRE);
    if (index >= COURONNEMENT_ADV_COUNT)
        index = 0;
    return &gCouronnementTrainers[DIFFICULTY_NORMAL][index];
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
    else if (t >= COURONNEMENT_NB_TOURNOIS_VILLES || !FlagGet(sBadgeRequis[t]))
        gSpecialVar_Result = TOURNOI_BADGE_MANQUANT;
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
    VarSet(VAR_COURONNEMENT_ADVERSAIRE, gSpecialVar_0x8004 * COURONNEMENT_MANCHES);
}

// STR_VAR_1 = nom, STR_VAR_2 = manche, STR_VAR_3 = Elo adverse ; gCouronnementTexteEquipe = équipe montrée.
void Couronnement_PreparerManche(void)
{
    u32 index = VarGet(VAR_COURONNEMENT_ADVERSAIRE);
    const struct Trainer *trainer = GetCouronnementTrainer();
    u8 *s;
    u32 i;

    StringCopy(gStringVar1, trainer->trainerName);
    ConvertIntToDecimalStringN(gStringVar2, index % COURONNEMENT_MANCHES + 1, STR_CONV_MODE_LEFT_ALIGN, 1);
    ConvertIntToDecimalStringN(gStringVar3, Couronnement_EloAdversaire(index), STR_CONV_MODE_LEFT_ALIGN, 4);

    s = StringCopy(gCouronnementTexteEquipe, sText_EquipeDe);
    s = StringAppend(s, trainer->trainerName);
    s = StringAppend(s, sText_DeuxPoints);
    for (i = 0; i < trainer->partySize; i++)
    {
        if (i > 0)
            s = StringAppend(s, i + 1 == trainer->partySize ? sText_Et : (i == 2 ? sText_VirguleLigne : sText_Virgule));
        s = StringAppend(s, GetSpeciesName(trainer->party[i].species));
    }
    StringAppend(s, sText_Point);
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
    u32 index = VarGet(VAR_COURONNEMENT_ADVERSAIRE) + 1;
    VarSet(VAR_COURONNEMENT_ADVERSAIRE, index);
    gSpecialVar_Result = (index % COURONNEMENT_MANCHES) != 0;
}

// Marque le tournoi courant comme gagné ; VAR_0x8006 = objet de récompense.
void Couronnement_TournoiGagne(void)
{
    u32 t = (VarGet(VAR_COURONNEMENT_ADVERSAIRE) - 1) / COURONNEMENT_MANCHES;
    if (t >= COURONNEMENT_NB_TOURNOIS_VILLES)
        t = 0;
    VarSet(VAR_COURONNEMENT_TOURNOIS, VarGet(VAR_COURONNEMENT_TOURNOIS) | (1 << t));
    gSpecialVar_0x8006 = sRecompense[t];
}
