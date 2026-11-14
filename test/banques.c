#include "global.h"
#include "banques.h"
#include "calendrier.h"
#include "event_data.h"
#include "money.h"
#include "test/test.h"
#include "constants/flags.h"
#include "constants/vars.h"

// Émeraude Couronnement (v0.37) : banques et jours de la semaine.

#define LUNDI 703   // 3 janvier 2000 + 700 jours : un lundi
#define MARDI 704

static void Preparer(u32 argent)
{
    u32 f;
    for (f = FLAG_BADGE01_GET; f <= FLAG_BADGE08_GET; f++)
        FlagClear(f);
    FlagClear(FLAG_SYS_GAME_CLEAR);
    memset(&gSaveBlock1Ptr->banque, 0, sizeof(gSaveBlock1Ptr->banque));
    gSaveBlock1Ptr->banque.magie = BANQUE_MAGIE;
    gSaveBlock1Ptr->banque.jour = 700;
    gSaveBlock2Ptr->pokedex.detteBanque[0] = 0;
    gSaveBlock2Ptr->pokedex.detteBanque[1] = 0;
    VarSet(VAR_BANQUE_DEFAUTS, 0);
    VarSet(VAR_BANQUE_PAYE_BAS, 0);
    VarSet(VAR_BANQUE_PAYE_HAUT, 0);
    SetMoney(&gSaveBlock1Ptr->money, argent);
}

TEST("Calendrier: day 1 is a Saturday and day 3 a Monday")
{
    EXPECT_EQ(Calendrier_JourSemaine(1), WEEKDAY_SAT);
    EXPECT_EQ(Calendrier_JourSemaine(3), WEEKDAY_MON);
    EXPECT_EQ(Calendrier_JourSemaine(LUNDI), WEEKDAY_MON);
    EXPECT(Calendrier_EstWeekend(LUNDI - 1));
    EXPECT(!Calendrier_EstWeekend(MARDI));
}

TEST("Banques: money can go above 9 999 999")
{
    Preparer(9999999);
    AddMoney(&gSaveBlock1Ptr->money, 5000000);
    EXPECT_EQ(GetMoney(&gSaveBlock1Ptr->money), 14999999);
}

TEST("Banques: Devon lends up to its ceiling and charges 0.3 % a day")
{
    Preparer(0);
    EXPECT_EQ(Banque_Emprunter(BANQUE_DEVON, 100000), 30000);
    EXPECT_EQ(GetMoney(&gSaveBlock1Ptr->money), 30000);
    EXPECT_EQ(Banque_Emprunter(BANQUE_DEVON, 10000), 0);
    Banque_PasserJour(MARDI);
    EXPECT_EQ(Banque_Dette(BANQUE_DEVON), 30090);
}

TEST("Banques: 5 % of the debt is taken every Monday")
{
    Preparer(1000000);
    gSaveBlock2Ptr->pokedex.detteBanque[BANQUE_DEVON] = 100000;
    Banque_PasserJour(LUNDI);
    EXPECT_EQ(Banque_Dette(BANQUE_DEVON), 100300 - 5015);
    EXPECT_EQ(GetMoney(&gSaveBlock1Ptr->money), 1000000 - 5015);
    EXPECT_EQ(VarGet(VAR_BANQUE_DEFAUTS), 0);
}

TEST("Banques: Devon default adds a 10 % penalty")
{
    Preparer(0);
    gSaveBlock2Ptr->pokedex.detteBanque[BANQUE_DEVON] = 100000;
    Banque_PasserJour(LUNDI);
    EXPECT_EQ(Banque_Dette(BANQUE_DEVON), 110330);
    EXPECT_EQ(VarGet(VAR_BANQUE_DEFAUTS), 1 << BANQUE_DEVON);
}

TEST("Banques: Casino default seizes the cash and adds 20 %")
{
    Preparer(1000);
    gSaveBlock1Ptr->banque.detteCasino = 100000;
    Banque_PasserJour(LUNDI);
    EXPECT_EQ(GetMoney(&gSaveBlock1Ptr->money), 0);
    EXPECT_EQ(Banque_Dette(BANQUE_CASINO), 121200);
}

TEST("Banques: repaying uses the money in hand")
{
    Preparer(50000);
    gSaveBlock1Ptr->banque.detteCasino = 20000;
    EXPECT_EQ(Banque_Rembourser(BANQUE_CASINO), 20000);
    EXPECT_EQ(Banque_Dette(BANQUE_CASINO), 0);
    EXPECT_EQ(GetMoney(&gSaveBlock1Ptr->money), 30000);
}

TEST("Banques: old save data is wiped by the magic number")
{
    Preparer(0);
    gSaveBlock1Ptr->banque.magie = 0;
    gSaveBlock1Ptr->banque.detteCasino = 12345;
    gSaveBlock2Ptr->pokedex.detteBanque[1] = 999;
    Banque_MettreAJourJusqua(800);
    EXPECT_EQ(Banque_Dette(BANQUE_CASINO), 0);
    EXPECT_EQ(Banque_Dette(BANQUE_FONCIER), 0);
    EXPECT_EQ(gSaveBlock1Ptr->banque.jour, 800);
}
