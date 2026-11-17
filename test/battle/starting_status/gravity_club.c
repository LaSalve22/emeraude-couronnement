#include "global.h"
#include "test/battle.h"

// Émeraude Couronnement (v0.42) : stade d'Autéquia de la Ligue des Clubs, Gravité au 1er tour pendant 5 tours.
SINGLE_BATTLE_TEST("Clubs: Gravity starting status lasts 5 turns")
{
    SetStartingStatus(STARTING_STATUS_GRAVITY_TEMPORARY);
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN {}
        TURN {}
        TURN {}
        TURN {}
        TURN {}
    } SCENE {
        MESSAGE("La gravité s'intensifie !");
        MESSAGE("La gravité redevient normale !");
    } THEN {
        EXPECT(!(gFieldStatuses & STATUS_FIELD_GRAVITY));
    }
}
