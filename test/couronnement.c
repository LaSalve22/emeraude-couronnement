#include "global.h"
#include "couronnement.h"
#include "event_data.h"
#include "test/test.h"

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
