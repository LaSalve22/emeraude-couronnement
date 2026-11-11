#include "global.h"
#include "coach_ev.h"
#include "pokemon.h"
#include "test/test.h"

// Émeraude Couronnement : Coach EV.

TEST("Coach EV: sets a stat and recalculates")
{
    struct Pokemon mon;
    u32 avant;
    CreateMon(&mon, SPECIES_ZIGZAGOON, 50, 0, OTID_STRUCT_PRESET(0));
    CoachEV_ToutRemettre(&mon);
    avant = GetMonData(&mon, MON_DATA_SPEED);
    EXPECT_EQ(CoachEV_Regler(&mon, 5, 252), 252);
    EXPECT_EQ(GetMonData(&mon, MON_DATA_SPEED_EV), 252);
    EXPECT_GT(GetMonData(&mon, MON_DATA_SPEED), avant);
}

TEST("Coach EV: total never exceeds 510")
{
    struct Pokemon mon;
    CreateMon(&mon, SPECIES_ZIGZAGOON, 50, 0, OTID_STRUCT_PRESET(0));
    CoachEV_ToutRemettre(&mon);
    CoachEV_Regler(&mon, 1, 252);
    CoachEV_Regler(&mon, 5, 252);
    EXPECT_EQ(CoachEV_Regler(&mon, 0, 252), 6);
    EXPECT_EQ(CoachEV_Total(&mon), 510);
    EXPECT_EQ(CoachEV_Regler(&mon, 1, 0), 0);
    EXPECT_EQ(CoachEV_Regler(&mon, 0, 252), 252);
    EXPECT_EQ(CoachEV_Total(&mon), 504);
}

TEST("Coach EV: reset sets every EV to 0")
{
    struct Pokemon mon;
    CreateMon(&mon, SPECIES_ZIGZAGOON, 50, 0, OTID_STRUCT_PRESET(0));
    CoachEV_Regler(&mon, 2, 128);
    CoachEV_ToutRemettre(&mon);
    EXPECT_EQ(CoachEV_Total(&mon), 0);
}
