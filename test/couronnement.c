#include "global.h"
#include "couronnement.h"
#include "event_data.h"
#include "test/test.h"
#include "data.h"
#include "pokemon.h"
#include "constants/moves.h"

extern const struct Trainer gCouronnementTrainers[DIFFICULTY_COUNT][COURONNEMENT_ADV_COUNT];

// Émeraude Couronnement : Couronnement Mondial (Elo, rang, paliers).

TEST("Couronnement: starting Elo is 1500 at rank 10000")
{
    VarSet(VAR_COURONNEMENT_ELO, 0);
    EXPECT_EQ(Couronnement_GetElo(), 1500);
    EXPECT_EQ(Couronnement_RangDepuisElo(1500), 10000);
    EXPECT_EQ(Couronnement_PalierDepuisElo(1500), PALIER_SUPER);
}

TEST("Couronnement: tiers follow Elo thresholds")
{
    EXPECT_EQ(Couronnement_PalierDepuisElo(1299), PALIER_POKE);
    EXPECT_EQ(Couronnement_PalierDepuisElo(1300), PALIER_SUPER);
    EXPECT_EQ(Couronnement_PalierDepuisElo(1600), PALIER_HYPER);
    EXPECT_EQ(Couronnement_PalierDepuisElo(1800), PALIER_MASTER);
    EXPECT_EQ(Couronnement_PalierDepuisElo(2000), PALIER_CHAMPIONS);
}

TEST("Couronnement: rank drops as Elo rises, Top 32 near 2000")
{
    u32 r2000 = Couronnement_RangDepuisElo(2000);
    EXPECT_LT(Couronnement_RangDepuisElo(1700), Couronnement_RangDepuisElo(1600));
    EXPECT_LE(r2000, 32);
    EXPECT_GE(r2000, 25);
    EXPECT_GT(Couronnement_RangDepuisElo(1950), 32);
    EXPECT_EQ(Couronnement_RangDepuisElo(1400), 12000);
}

TEST("Couronnement: Elo change follows K=32 expectation")
{
    EXPECT_EQ(Couronnement_VariationElo(1500, 1500, TRUE), 16);
    EXPECT_EQ(Couronnement_VariationElo(1500, 1500, FALSE), -16);
    EXPECT_GT(Couronnement_VariationElo(1500, 1700, TRUE), 16);
    EXPECT_LT(Couronnement_VariationElo(1700, 1500, TRUE), 16);
    EXPECT_GE(Couronnement_VariationElo(1500, 2400, TRUE), 31);
}

TEST("Couronnement: only one legendary per team")
{
    u16 ok[4] = {SPECIES_GROUDON, SPECIES_PIKACHU, SPECIES_ZIGZAGOON, SPECIES_SQUIRTLE};
    u16 bad[4] = {SPECIES_GROUDON, SPECIES_KYOGRE, SPECIES_ZIGZAGOON, SPECIES_SQUIRTLE};
    EXPECT(Couronnement_EquipeValide(ok, 4));
    EXPECT(!Couronnement_EquipeValide(bad, 4));
}

TEST("Couronnement: 12 tournaments of 3 rounds then a 5-round final")
{
    EXPECT_EQ(COURONNEMENT_ADV_COUNT, 41);
    EXPECT_EQ(Couronnement_PremierAdversaire(COURONNEMENT_TOURNOI_NENUCRIQUE), 33);
    EXPECT_EQ(Couronnement_PremierAdversaire(COURONNEMENT_TOURNOI_FINALE), 36);
    EXPECT_EQ(Couronnement_NbManches(COURONNEMENT_TOURNOI_FINALE), 5);
    EXPECT_EQ(Couronnement_NbManches(COURONNEMENT_TOURNOI_MEROUVILLE), 3);
    EXPECT_EQ(Couronnement_TournoiDepuisAdversaire(40), COURONNEMENT_TOURNOI_FINALE);
    EXPECT_EQ(Couronnement_TournoiDepuisAdversaire(35), COURONNEMENT_TOURNOI_NENUCRIQUE);
    EXPECT_EQ(Couronnement_EloAdversaire(0), 1350);
    EXPECT_EQ(Couronnement_EloAdversaire(40), 2150);
}

TEST("Couronnement: every opponent shows 6 Pokemon")
{
    u32 i;
    VarSet(VAR_COURONNEMENT_ADVERSAIRE, 0);
    for (i = 0; i < COURONNEMENT_ADV_COUNT; i++)
        EXPECT_EQ((u32)gCouronnementTrainers[DIFFICULTY_NORMAL][i].partySize, 6);
}

TEST("Couronnement: opponent scores type matchups against the chosen team")
{
    struct Pokemon joueur[1];
    struct TrainerMon eau = {.species = SPECIES_SQUIRTLE, .moves = {MOVE_WATER_GUN}};
    struct TrainerMon plante = {.species = SPECIES_TREECKO, .moves = {MOVE_ABSORB}};
    CreateMon(&joueur[0], SPECIES_CHARMANDER, 50, 0, OTID_STRUCT_PRESET(0));
    SetMonMoveSlot(&joueur[0], MOVE_EMBER, 0);
    EXPECT_GT(Couronnement_ScoreAdverse(&eau, joueur, 1), Couronnement_ScoreAdverse(&plante, joueur, 1));
}
