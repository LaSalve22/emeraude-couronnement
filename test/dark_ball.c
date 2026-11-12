#include "global.h"
#include "test/battle.h"

// Émeraude Couronnement : Dark Ball (ex-Étrange Ball). Pokémon marqué par MON_DATA_IS_SHADOW.

TEST("Dark Ball: +30% stats except HP and no friendship")
{
    struct Pokemon mon;
    u32 hp, atk, spe;
    u8 friendship = 200, dark = TRUE;

    CreateMon(&mon, SPECIES_ZIGZAGOON, 50, 0, OTID_STRUCT_PRESET(0));
    CalculateMonStats(&mon);
    hp = GetMonData(&mon, MON_DATA_MAX_HP);
    atk = GetMonData(&mon, MON_DATA_ATK);
    spe = GetMonData(&mon, MON_DATA_SPEED);
    SetMonData(&mon, MON_DATA_IS_SHADOW, &dark);
    CalculateMonStats(&mon);
    EXPECT_EQ(GetMonData(&mon, MON_DATA_MAX_HP), hp);
    EXPECT_EQ(GetMonData(&mon, MON_DATA_ATK), atk * (100 + COURONNEMENT_DARK_BALL_BONUS) / 100);
    EXPECT_EQ(GetMonData(&mon, MON_DATA_SPEED), spe * (100 + COURONNEMENT_DARK_BALL_BONUS) / 100);
    SetMonData(&mon, MON_DATA_FRIENDSHIP, &friendship);
    EXPECT_EQ(GetMonData(&mon, MON_DATA_FRIENDSHIP), 0);
}

SINGLE_BATTLE_TEST("Dark Ball: loses 10% max HP after a damaging move")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Shadow(TRUE); MaxHP(200); HP(200); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(player, MOVE_POUND); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_POUND, player);
        HP_BAR(opponent);
        HP_BAR(player, damage: 20);
    }
}

SINGLE_BATTLE_TEST("Dark Ball: no recoil after a status move")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Shadow(TRUE); MaxHP(200); HP(200); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(player, MOVE_GROWL); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_GROWL, player);
        NOT HP_BAR(player);
    }
}
