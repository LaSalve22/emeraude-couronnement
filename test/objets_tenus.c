#include "global.h"
#include "test/battle.h"

// Émeraude Couronnement (v0.58) : les objets tenus consommés en combat reviennent après le combat, Baies comprises.
// make check TESTS="Objets tenus"

SINGLE_BATTLE_TEST("Objets tenus: a Berry eaten in battle is back after the battle")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Item(ITEM_SITRUS_BERRY); Speed(1); }
        OPPONENT(SPECIES_WOBBUFFET) { HP(1); Speed(2); }
    } WHEN {
        TURN { MOVE(opponent, MOVE_SUPER_FANG); MOVE(player, MOVE_TACKLE); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SUPER_FANG, opponent);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_TACKLE, player);
    } THEN {
        EXPECT_EQ(GetMonData(&gParties[B_TRAINER_PLAYER][0], MON_DATA_HELD_ITEM), ITEM_SITRUS_BERRY);
    }
}

SINGLE_BATTLE_TEST("Objets tenus: a Focus Sash used in battle is back after the battle")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Item(ITEM_FOCUS_SASH); Speed(1); }
        OPPONENT(SPECIES_WOBBUFFET) { HP(1); Speed(2); }
    } WHEN {
        TURN { MOVE(opponent, MOVE_SEISMIC_TOSS); MOVE(player, MOVE_TACKLE); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SEISMIC_TOSS, opponent);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_TACKLE, player);
    } THEN {
        EXPECT_EQ(GetMonData(&gParties[B_TRAINER_PLAYER][0], MON_DATA_HELD_ITEM), ITEM_FOCUS_SASH);
    }
}
