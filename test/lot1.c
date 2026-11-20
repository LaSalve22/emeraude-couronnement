#include "global.h"
#include "caps.h"
#include "couronnement_cs.h"
#include "event_data.h"
#include "item.h"
#include "test/test.h"
#include "constants/flags.h"
#include "constants/items.h"

// Émeraude Couronnement : lot 1 du cahier v0.42 (Câble Link, shiny, Bonbon Exp. XXL, CS sans capacité).

TEST("Lot 1: the Link Cable evolves trade evolutions, with the held item if needed")
{
    struct Pokemon mon;
    u16 item = ITEM_METAL_COAT;

    CreateMon(&mon, SPECIES_KADABRA, 30, 0, OTID_STRUCT_PRESET(0));
    EXPECT_EQ(GetEvolutionTargetSpecies(&mon, EVO_MODE_ITEM_CHECK, ITEM_LINKING_CORD, NULL, NULL, CHECK_EVO), SPECIES_ALAKAZAM);
    CreateMon(&mon, SPECIES_ONIX, 30, 0, OTID_STRUCT_PRESET(0));
    EXPECT_EQ(GetEvolutionTargetSpecies(&mon, EVO_MODE_ITEM_CHECK, ITEM_LINKING_CORD, NULL, NULL, CHECK_EVO), SPECIES_NONE);
    SetMonData(&mon, MON_DATA_HELD_ITEM, &item);
    EXPECT_EQ(GetEvolutionTargetSpecies(&mon, EVO_MODE_ITEM_CHECK, ITEM_LINKING_CORD, NULL, NULL, CHECK_EVO), SPECIES_STEELIX);
    EXPECT_EQ(GetItemPocket(ITEM_LINKING_CORD), POCKET_KEY_ITEMS);
}

TEST("Lot 1: a shiny gets +10% on every stat, stacking with the Dark Ball")
{
    struct Pokemon mon;
    u32 hp, atk;
    bool8 oui = TRUE;

    CreateMon(&mon, SPECIES_ZIGZAGOON, 50, 0, OTID_STRUCT_PRESET(0));
    SetMonData(&mon, MON_DATA_IS_SHINY, &(bool8){FALSE});
    CalculateMonStats(&mon);
    hp = GetMonData(&mon, MON_DATA_MAX_HP);
    atk = GetMonData(&mon, MON_DATA_ATK);
    SetMonData(&mon, MON_DATA_IS_SHINY, &oui);
    CalculateMonStats(&mon);
    EXPECT_EQ(GetMonData(&mon, MON_DATA_MAX_HP), hp * (100 + P_SHINY_STAT_BONUS) / 100);
    EXPECT_EQ(GetMonData(&mon, MON_DATA_ATK), atk * (100 + P_SHINY_STAT_BONUS) / 100);
    SetMonData(&mon, MON_DATA_IS_SHADOW, &oui);
    CalculateMonStats(&mon);
    EXPECT_EQ(GetMonData(&mon, MON_DATA_ATK), atk * (100 + P_SHINY_STAT_BONUS + COURONNEMENT_DARK_BALL_BONUS) / 100);
}

TEST("Lot 1: the XXL Exp. Candy gives 100000 Exp. but never passes the level cap")
{
    struct Pokemon mon;
    u32 f;

    for (f = FLAG_BADGE01_GET; f <= FLAG_BADGE08_GET; f++)
        FlagClear(f);
    CreateMon(&mon, SPECIES_ZIGZAGOON, 5, 0, OTID_STRUCT_PRESET(0));
    EXPECT_EQ(GetItemHoldEffectParam(ITEM_EXP_CANDY_XXL), EXP_100000);
    ExecuteTableBasedItemEffect(&mon, ITEM_EXP_CANDY_XXL, 0, 0);
    EXPECT_EQ(GetMonData(&mon, MON_DATA_LEVEL), GetCurrentLevelCap());
}

TEST("Lot 1: an HM works on the field with the item and the badge only")
{
    ClearBag();
    FlagClear(FLAG_BADGE01_GET);
    EXPECT(!Couronnement_CSDisponible(FIELD_MOVE_CUT));
    AddBagItem(ITEM_HM_CUT, 1);
    EXPECT(!Couronnement_CSDisponible(FIELD_MOVE_CUT));
    FlagSet(FLAG_BADGE01_GET);
    EXPECT(Couronnement_CSDisponible(FIELD_MOVE_CUT));
    EXPECT(!Couronnement_CSDisponible(FIELD_MOVE_SURF));
    FlagClear(FLAG_BADGE01_GET);
    ClearBag();
}
