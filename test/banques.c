#include "global.h"
#include "banques.h"
#include "bourse.h"
#include "immobilier.h"
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
    memset(&gSaveBlock2Ptr->pokedex.bourse, 0, sizeof(gSaveBlock2Ptr->pokedex.bourse));
    memset(&gSaveBlock2Ptr->pokedex.societe, 0, sizeof(gSaveBlock2Ptr->pokedex.societe));
    memset(&gSaveBlock3Ptr->bourseOr, 0, sizeof(gSaveBlock3Ptr->bourseOr));
    memset(&gSaveBlock1Ptr->immo, 0, sizeof(gSaveBlock1Ptr->immo));
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
    EXPECT_EQ(VarGet(VAR_BANQUE_DEFAUTS) & 0xF, 1 << BANQUE_DEVON);
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

// Correctifs de l'audit (lot 0 du cahier v0.42)

TEST("Banques: the Monday payment is taken from the treasury, then the stock portfolio")
{
    Preparer(1000);
    gSaveBlock2Ptr->pokedex.societe.etat = 1;
    gSaveBlock2Ptr->pokedex.societe.tresorerie = 2000;
    gSaveBlock2Ptr->pokedex.bourse.cours[0] = 100;
    gSaveBlock2Ptr->pokedex.bourse.parts[0] = 1000;
    gSaveBlock2Ptr->pokedex.detteBanque[BANQUE_DEVON] = 100000;
    Banque_PasserJour(LUNDI); // échéance 5 015 : 1 000 de liquide, 2 000 de trésorerie, le reste en actions
    EXPECT_EQ(Banque_Dette(BANQUE_DEVON), 100300 - 5015);
    EXPECT_EQ(VarGet(VAR_BANQUE_DEFAUTS), 0);
    EXPECT_EQ(gSaveBlock2Ptr->pokedex.societe.tresorerie, 0);
    EXPECT_LT(gSaveBlock2Ptr->pokedex.bourse.parts[0], 1000);
    EXPECT_GE(gSaveBlock2Ptr->pokedex.bourse.parts[0], 970);
}

TEST("Banques: a second default bans all credit and seizes a home")
{
    Preparer(0);
    gSaveBlock1Ptr->immo.magie = IMMO_MAGIE;
    gSaveBlock1Ptr->immo.indice = 1000;
    gSaveBlock1Ptr->immo.possede = 1 << 0;
    gSaveBlock2Ptr->pokedex.detteBanque[BANQUE_DEVON] = 1000000;
    Banque_PasserJour(LUNDI);
    EXPECT(!Banque_InterditBancaire());
    EXPECT_EQ(gSaveBlock1Ptr->immo.possede, 1 << 0);
    Banque_PasserJour(LUNDI + 7);
    EXPECT(Banque_InterditBancaire());
    EXPECT_EQ(gSaveBlock1Ptr->immo.possede, 0);
    EXPECT_EQ(Banque_Emprunter(BANQUE_CASINO, 10000), 0);
    EXPECT_EQ(Bourse_Emprunter(10000), 0);
}

TEST("Banques: paying off the debt lifts the credit ban")
{
    Preparer(0);
    gSaveBlock1Ptr->banque.detteCasino = 50000;
    Banque_PasserJour(LUNDI);
    Banque_PasserJour(LUNDI + 7);
    EXPECT(Banque_InterditBancaire());
    SetMoney(&gSaveBlock1Ptr->money, 10000000);
    Banque_Rembourser(BANQUE_CASINO);
    EXPECT(!Banque_InterditBancaire());
    EXPECT_GT(Banque_Emprunter(BANQUE_CASINO, 10000), 0);
}

TEST("Banques: turning the clock back does not replay days")
{
    Preparer(0);
    gSaveBlock2Ptr->pokedex.detteBanque[BANQUE_DEVON] = 100000;
    gSaveBlock1Ptr->banque.jour = MARDI;
    Banque_MettreAJourJusqua(MARDI - 30);
    EXPECT_EQ(gSaveBlock1Ptr->banque.jour, MARDI);
    Banque_MettreAJourJusqua(MARDI);
    EXPECT_EQ(Banque_Dette(BANQUE_DEVON), 100000);
    EXPECT_GE(Calendrier_Aujourdhui(), MARDI);
}
