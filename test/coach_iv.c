#include "global.h"
#include "pokemon.h"
#include "test/test.h"
#include "coach_ev.h"

// Émeraude Couronnement (v0.32) : Coach IV et Capsules d'Argent / d'Or.
// Stats dans l'ordre d'affichage : 0 PV, 1 ATQ, 2 DÉF, 3 A.SPÉ, 4 D.SPÉ, 5 VIT.

static void MettreIv(struct Pokemon *mon, u32 iv)
{
    u8 v = iv;
    SetMonData(mon, MON_DATA_HP_IV, &v);
    SetMonData(mon, MON_DATA_ATK_IV, &v);
    SetMonData(mon, MON_DATA_DEF_IV, &v);
    SetMonData(mon, MON_DATA_SPATK_IV, &v);
    SetMonData(mon, MON_DATA_SPDEF_IV, &v);
    SetMonData(mon, MON_DATA_SPEED_IV, &v);
    CalculateMonStats(mon);
}

TEST("Coach IV: sets one IV to 0 or 31 and recalculates stats")
{
    struct Pokemon mon;
    u32 vit;

    CreateMon(&mon, SPECIES_ZIGZAGOON, 50, 15, OTID_STRUCT_PRESET(0));
    MettreIv(&mon, 15);
    vit = GetMonData(&mon, MON_DATA_SPEED);
    CoachIV_Regler(&mon, 5, 0);
    EXPECT_EQ(GetMonData(&mon, MON_DATA_SPEED_IV), 0);
    EXPECT_LT(GetMonData(&mon, MON_DATA_SPEED), vit);
    EXPECT_EQ(GetMonData(&mon, MON_DATA_ATK_IV), 15);
    CoachIV_Regler(&mon, 3, 31);
    EXPECT_EQ(GetMonData(&mon, MON_DATA_SPATK_IV), 31);
    CoachIV_Regler(&mon, COACH_IV_TOUT_A_31, 0);
    EXPECT_EQ(GetMonData(&mon, MON_DATA_HP_IV), 31);
    EXPECT_EQ(GetMonData(&mon, MON_DATA_SPEED_IV), 31);
}

TEST("Capsules: silver maxes one IV, gold maxes all, nothing to do returns 0")
{
    struct Pokemon mon;

    CreateMon(&mon, SPECIES_ZIGZAGOON, 50, 10, OTID_STRUCT_PRESET(0));
    MettreIv(&mon, 10);
    EXPECT_EQ(Capsule_Maximiser(&mon, 1), 1);
    EXPECT_EQ(GetMonData(&mon, MON_DATA_ATK_IV), 31);
    EXPECT_EQ(GetMonData(&mon, MON_DATA_DEF_IV), 10);
    EXPECT_EQ(Capsule_Maximiser(&mon, 1), 0);
    EXPECT_EQ(Capsule_Maximiser(&mon, CAPSULE_TOUTES_STATS), 5);
    EXPECT_EQ(GetMonData(&mon, MON_DATA_HP_IV), 31);
    EXPECT_EQ(GetMonData(&mon, MON_DATA_SPDEF_IV), 31);
    EXPECT_EQ(Capsule_Maximiser(&mon, CAPSULE_TOUTES_STATS), 0);
}

TEST("Capsules: a Dark Ball Pokemon keeps its +30% bonus")
{
    struct Pokemon mon;
    u8 dark = TRUE;
    u32 atk;

    CreateMon(&mon, SPECIES_ZIGZAGOON, 50, 0, OTID_STRUCT_PRESET(0));
    MettreIv(&mon, 31);
    atk = GetMonData(&mon, MON_DATA_ATK);
    MettreIv(&mon, 0);
    SetMonData(&mon, MON_DATA_IS_SHADOW, &dark);
    Capsule_Maximiser(&mon, 1);
    EXPECT_EQ(GetMonData(&mon, MON_DATA_ATK), atk * (100 + COURONNEMENT_DARK_BALL_BONUS) / 100);
}
