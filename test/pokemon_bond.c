#include "global.h"
#include "pokemon.h"
#include "pokemon_bond.h"
#include "test/test.h"

// Émeraude Couronnement : liens avec les Pokémon.

static void SetFriendship(struct Pokemon *mon, u8 friendship)
{
    SetMonData(mon, MON_DATA_FRIENDSHIP, &friendship);
}

TEST("Pokemon bond: levels follow friendship")
{
    struct Pokemon mon;
    CreateRandomMonWithIVs(&mon, SPECIES_ZIGZAGOON, 10, 0);
    SetFriendship(&mon, 100);
    EXPECT_EQ(GetMonBondLevel(&mon), BOND_NONE);
    SetFriendship(&mon, 150);
    EXPECT_EQ(GetMonBondLevel(&mon), BOND_NORMAL);
    SetFriendship(&mon, 230);
    EXPECT_EQ(GetMonBondLevel(&mon), BOND_STRONG);
}

TEST("Pokemon bond: a strong bond makes 252 EVs count as 302")
{
    struct Pokemon mon;
    u8 ev = 252;
    CreateRandomMonWithIVs(&mon, SPECIES_SQUIRTLE, 100, 0);
    SetMonData(&mon, MON_DATA_HP_EV, &ev);
    SetFriendship(&mon, 0);
    CalculateMonStats(&mon);
    EXPECT_EQ(GetMonData(&mon, MON_DATA_MAX_HP), 2 * 44 + 252 / 4 + 100 + 10);
    SetFriendship(&mon, 255);
    CalculateMonStats(&mon);
    EXPECT_EQ(GetMonData(&mon, MON_DATA_MAX_HP), 2 * 44 + 302 / 4 + 100 + 10);
}

TEST("Pokemon bond: exp bonus grows with bond and seniority")
{
    struct Pokemon mon;
    u32 i;
    CreateRandomMonWithIVs(&mon, SPECIES_ZIGZAGOON, 10, 0);
    SetFriendship(&mon, 0);
    EXPECT_EQ(GetMonBondExpBonusPercent(&mon), 0);
    SetFriendship(&mon, 255);
    EXPECT_EQ(GetMonBondExpBonusPercent(&mon), 10);
    for (i = 0; i < 100; i++)
        IncrementMonBondBattles(&mon);
    EXPECT_EQ(GetMonData(&mon, MON_DATA_BOND_BATTLES), 63);
    EXPECT_EQ(GetMonBondExpBonusPercent(&mon), 20);
    EXPECT_EQ(GetMonBondEVMultiplierPercent(&mon), 150);
}
