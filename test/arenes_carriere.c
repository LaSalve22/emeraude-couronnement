#include "global.h"
#include "arenes.h"
#include "event_data.h"
#include "money.h"
#include "party_menu.h"
#include "pokemon.h"
#include "pokemon_storage_system.h"
#include "test/test.h"
#include "constants/flags.h"

// Émeraude Couronnement (v0.57), édition Carrière : Pokémon rattachés aux arènes. make check EDITION=carriere TESTS="Arenes Carriere".
#if EDITION_TYCOON_SIM

#define AR (gSaveBlock3Ptr->arenes)

static void Preparer(void)
{
    ResetPokemonStorageSystem();
    memset(&AR, 0, sizeof(AR));
    memset(&gSaveBlock2Ptr->pokedex.societe, 0, sizeof(gSaveBlock2Ptr->pokedex.societe));
    ZeroPlayerPartyMons();
    CreateMon(&gParties[B_TRAINER_PLAYER][0], SPECIES_MUDKIP, 30, 1, OTID_STRUCT_PRESET(0));
    CalculatePlayerPartyCount();
    SetMoney(&gSaveBlock1Ptr->money, 100000);
    Arene_MettreAJourJusqua(700);
}

TEST("Arenes Carriere: buying gives 6 locked Pokemon of the type, N.10 to N.20")
{
    struct BoxPokemon *places[ARENE_PLACES];
    u32 i;
    Preparer();
    EXPECT(Arene_Acheter(1, 5)); // Spectre
    EXPECT_EQ(Arene_Pokemon(1, places), ARENE_RATTACHES);
    EXPECT(Arene_Valide(1));
    EXPECT_EQ(CalculatePlayerPartyCount(), 1);
    for (i = 0; i < ARENE_RATTACHES; i++)
    {
        u32 s = GetBoxMonData(places[i], MON_DATA_SPECIES), lvl = GetLevelFromBoxMonExp(places[i]);
        EXPECT(places[i] != NULL);
        EXPECT(Arene_EstBloque(places[i]));
        EXPECT(GetSpeciesType(s, 0) == TYPE_GHOST || GetSpeciesType(s, 1) == TYPE_GHOST);
        EXPECT_EQ(lvl, 10 + 2 * i);
        EXPECT(GetBoxMonData(places[i], MON_DATA_MOVE1) != MOVE_NONE);
    }
}

TEST("Arenes Carriere: chosen Pokemon come back to the arena with their progress")
{
    struct BoxPokemon *places[ARENE_PLACES];
    u32 exp, especeAvant;
    Preparer();
    Arene_Acheter(1, 5);
    gSpecialVar_0x8008 = 1;
    gSpecialVar_0x8005 = 3;
    Arene_PreparerSelection();
    EXPECT_EQ(CalculatePlayerPartyCount(), ARENE_RATTACHES);
    EXPECT_EQ(gAreneNbChoix, 3);
    gSelectedOrderFromParty[0] = 6;
    gSelectedOrderFromParty[1] = 2;
    gSelectedOrderFromParty[2] = 4;
    Arene_GarderSelection();
    EXPECT_EQ(CalculatePlayerPartyCount(), 3);
    Arene_Pokemon(1, places);
    especeAvant = GetBoxMonData(places[5], MON_DATA_SPECIES);
    EXPECT_EQ(GetMonData(&gParties[B_TRAINER_PLAYER][0], MON_DATA_SPECIES), especeAvant);
    exp = GetMonData(&gParties[B_TRAINER_PLAYER][0], MON_DATA_EXP) + 5000;
    SetMonData(&gParties[B_TRAINER_PLAYER][0], MON_DATA_EXP, &exp);
    Arene_RendreEquipe();
    EXPECT_EQ(CalculatePlayerPartyCount(), 1);
    EXPECT_EQ(GetMonData(&gParties[B_TRAINER_PLAYER][0], MON_DATA_SPECIES), SPECIES_MUDKIP);
    Arene_Pokemon(1, places);
    EXPECT_EQ(Arene_Pokemon(1, places), ARENE_RATTACHES);
    EXPECT_EQ(GetBoxMonData(places[5], MON_DATA_EXP), exp);
    EXPECT(Arene_EstBloque(places[5]));
}

TEST("Arenes Carriere: cancelling the choice gives the party back")
{
    Preparer();
    Arene_Acheter(1, 5);
    gSpecialVar_0x8008 = 1;
    gSpecialVar_0x8005 = 4;
    Arene_PreparerSelection();
    Arene_AnnulerSelection();
    EXPECT_EQ(CalculatePlayerPartyCount(), 1);
    EXPECT_EQ(GetMonData(&gParties[B_TRAINER_PLAYER][0], MON_DATA_SPECIES), SPECIES_MUDKIP);
}

#endif
