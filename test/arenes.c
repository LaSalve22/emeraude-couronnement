#include "global.h"
#include "arenes.h"
#include "event_data.h"
#include "money.h"
#include "pokemon.h"
#include "pokemon_storage_system.h"
#include "test/test.h"
#include "battle.h"
#include "constants/flags.h"

// Émeraude Couronnement (v0.40) : arènes du joueur.

#define AR (gSaveBlock3Ptr->arenes)

static void Preparer(u32 argent)
{
    u32 i;
    static const u16 sEquipe[] = {SPECIES_GENGAR, SPECIES_BANETTE, SPECIES_DUSKNOIR, SPECIES_MISMAGIUS, SPECIES_SNORLAX, SPECIES_PIKACHU};

    ResetPokemonStorageSystem();
    memset(&AR, 0, sizeof(AR));
    memset(&gSaveBlock2Ptr->pokedex.societe, 0, sizeof(gSaveBlock2Ptr->pokedex.societe));
    ZeroPlayerPartyMons();
    for (i = 0; i < PARTY_SIZE; i++)
        CreateMon(&gParties[B_TRAINER_PLAYER][i], sEquipe[i], 50, i + 1, OTID_STRUCT_PRESET(0));
    CalculatePlayerPartyCount();
    SetMoney(&gSaveBlock1Ptr->money, argent);
    FlagSet(FLAG_SYS_GAME_CLEAR);
    Arene_MettreAJourJusqua(700);
}

TEST("Arenes: buying needs the League and 500000")
{
    Preparer(400000);
    EXPECT(!Arene_Acheter(1, 5));
    SetMoney(&gSaveBlock1Ptr->money, 600000);
    FlagClear(FLAG_SYS_GAME_CLEAR);
    EXPECT(!Arene_Acheter(1, 5));
    FlagSet(FLAG_SYS_GAME_CLEAR);
    EXPECT(Arene_Acheter(1, 5)); // Spectre
    EXPECT_EQ(GetMoney(&gSaveBlock1Ptr->money), 100000);
    EXPECT_EQ(Arene_Type(1), TYPE_GHOST);
    EXPECT_EQ(Arene_Valeur(), ARENE_PRIX);
}

TEST("Arenes: deposited Pokemon leave the party, are locked and count for the type")
{
    struct BoxPokemon *places[ARENE_PLACES];
    u32 i;
    Preparer(600000);
    Arene_Acheter(1, 5);
    for (i = 0; i < 3; i++)
        EXPECT(Arene_Deposer(1, 0, TRUE));
    EXPECT(!Arene_Valide(1));
    EXPECT(Arene_Deposer(1, 0, TRUE)); // 4e spectre
    EXPECT(Arene_Valide(1));
    EXPECT_EQ(CalculatePlayerPartyCount(), 2);
    EXPECT(!Arene_Deposer(1, 1, TRUE) || CalculatePlayerPartyCount() == 1);
    EXPECT_EQ(Arene_Pokemon(1, places), 5);
    EXPECT(Arene_EstBloque(places[0]));
    EXPECT(Arene_PokemonDansLePC());
    EXPECT(!Arene_Deposer(1, 0, TRUE)); // dernier Pokémon de l'équipe
}

TEST("Arenes: challengers arrive every day and Pokemon train")
{
    struct BoxPokemon *places[ARENE_PLACES];
    u32 i, exp;
    Preparer(600000);
    Arene_Acheter(1, 5);
    for (i = 0; i < 4; i++)
        Arene_Deposer(1, 0, TRUE);
    Arene_Pokemon(1, places);
    exp = GetBoxMonData(places[0], MON_DATA_EXP);
    Arene_MettreAJourJusqua(701);
    EXPECT_EQ((u32)AR.arenes[1].attente, 1);
    EXPECT_GT(GetBoxMonData(places[0], MON_DATA_EXP), exp);
    EXPECT_GT(Arene_BadgesChallenger(1), 0);
    Arene_MettreAJourJusqua(710);
    EXPECT_EQ((u32)AR.arenes[1].attente, ARENE_ATTENTE_MAX);
    EXPECT_GT(GetMoney(&gSaveBlock1Ptr->money), 100000); // sponsors
}

TEST("Arenes: a Pokemon taken out goes back to the party unlocked")
{
    struct BoxPokemon *places[ARENE_PLACES];
    Preparer(600000);
    Arene_Acheter(1, 5);
    Arene_Deposer(1, 0, TRUE);
    EXPECT_EQ(CalculatePlayerPartyCount(), 5);
    EXPECT(Arene_Retirer(1, 0));
    EXPECT_EQ(CalculatePlayerPartyCount(), 6);
    EXPECT_EQ(GetMonData(&gParties[B_TRAINER_PLAYER][5], MON_DATA_ARENE), 0);
    EXPECT_EQ(Arene_Pokemon(1, places), 0);
    EXPECT(!Arene_PokemonDansLePC());
}

TEST("Arenes: a challenge lends the arena team at the challenger's level then gives the party back")
{
    u32 badges, attente, reputation;
    Preparer(600000);
    Arene_Acheter(1, 5);
    Arene_Deposer(1, 0, TRUE);
    Arene_Deposer(1, 0, TRUE);
    Arene_Deposer(1, 0, TRUE);
    Arene_Deposer(1, 0, TRUE);
    Arene_MettreAJourJusqua(701);
    gSpecialVar_0x8008 = 1;
    badges = Arene_BadgesChallenger(1);
    Arene_PreparerChallenger(); // peut rattraper des jours selon l'horloge du test
    EXPECT_EQ(gSpecialVar_Result, TRUE);
    attente = AR.arenes[1].attente;
    reputation = AR.arenes[1].reputation;
    Arene_DebutCombat();
    EXPECT(Arene_CombatEnCours());
    {
        static const u8 sNiv[] = {18, 27, 36, 45, 54, 63, 71, 78};
        EXPECT_EQ(GetMonData(&gParties[B_TRAINER_PLAYER][0], MON_DATA_LEVEL), sNiv[badges - 1]);
    }
    gBattleOutcome = B_OUTCOME_WON;
    Arene_FinCombat();
    EXPECT(!Arene_CombatEnCours());
    EXPECT_EQ(CalculatePlayerPartyCount(), 2);
    EXPECT_EQ(GetMonData(&gParties[B_TRAINER_PLAYER][0], MON_DATA_SPECIES), SPECIES_SNORLAX);
    EXPECT_EQ((u32)AR.arenes[1].attente, attente - 1);
    EXPECT_EQ(AR.arenes[1].victoires, 1);
    EXPECT_EQ(AR.arenes[1].reputation, reputation + 3);
}
