#include "global.h"
#include "pokemon.h"
#include "starter_choose.h"
#include "test/test.h"

// Émeraude Couronnement : starters du joueur (étape 1 du CLAUDE.md)

TEST("Player starters are Treecko, Cyndaquil and Squirtle")
{
    EXPECT_EQ(GetStarterPokemon(0), SPECIES_TREECKO);
    EXPECT_EQ(GetStarterPokemon(1), SPECIES_CYNDAQUIL);
    EXPECT_EQ(GetStarterPokemon(2), SPECIES_SQUIRTLE);
}

TEST("Player starter gets its hidden ability")
{
    struct Pokemon mon;
    CreateRandomMonWithIVs(&mon, SPECIES_SQUIRTLE, 5, 0);
    MarkPlayerStarter(&mon);
    EXPECT_EQ(GetMonData(&mon, MON_DATA_ABILITY_NUM), 2);
    EXPECT_EQ(GetMonAbility(&mon), ABILITY_RAIN_DISH);
}

TEST("Player starter has +20% base stats, other Pokemon do not")
{
    struct Pokemon normal, starter;
    CreateRandomMonWithIVs(&normal, SPECIES_SQUIRTLE, 100, 0);
    starter = normal;
    MarkPlayerStarter(&starter);

    // Squirtle : PV de base 44 -> 52 avec le bonus. Niveau 100, 0 IV, 0 EV.
    EXPECT_EQ(GetMonData(&normal, MON_DATA_MAX_HP), 2 * 44 + 100 + 10);
    EXPECT_EQ(GetMonData(&starter, MON_DATA_MAX_HP), 2 * 52 + 100 + 10);
    EXPECT_GT(GetMonData(&starter, MON_DATA_ATK), GetMonData(&normal, MON_DATA_ATK));
    EXPECT_GT(GetMonData(&starter, MON_DATA_DEF), GetMonData(&normal, MON_DATA_DEF));
    EXPECT_GT(GetMonData(&starter, MON_DATA_SPEED), GetMonData(&normal, MON_DATA_SPEED));
    EXPECT_GT(GetMonData(&starter, MON_DATA_SPATK), GetMonData(&normal, MON_DATA_SPATK));
    EXPECT_GT(GetMonData(&starter, MON_DATA_SPDEF), GetMonData(&normal, MON_DATA_SPDEF));
}

TEST("Player starter bonus is kept after evolving")
{
    struct Pokemon mon;
    enum Species blastoise = SPECIES_BLASTOISE;
    CreateRandomMonWithIVs(&mon, SPECIES_SQUIRTLE, 100, 0);
    MarkPlayerStarter(&mon);
    SetMonData(&mon, MON_DATA_SPECIES, &blastoise);
    CalculateMonStats(&mon);

    // Tortank : PV de base 79 -> 94 avec le bonus.
    EXPECT_EQ(GetMonData(&mon, MON_DATA_PLAYER_STARTER), TRUE);
    EXPECT_EQ(GetMonData(&mon, MON_DATA_MAX_HP), 2 * 94 + 100 + 10);
    EXPECT_EQ(GetMonAbility(&mon), ABILITY_RAIN_DISH);
}
