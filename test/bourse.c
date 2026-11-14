#include "global.h"
#include "bourse.h"
#include "event_data.h"
#include "money.h"
#include "test/test.h"
#include "constants/flags.h"

// Émeraude Couronnement (v0.33) : Bourse de Hoenn.

#define BOURSE (gSaveBlock2Ptr->pokedex.bourse)

static void Preparer(u32 argent)
{
    memset(&BOURSE, 0, sizeof(BOURSE));
    FlagClear(FLAG_RECOVERED_DEVON_GOODS);
    FlagClear(FLAG_BADGE01_GET);
    SetMoney(&gSaveBlock1Ptr->money, argent);
    Bourse_MettreAJourJusqua(100);
}

TEST("Bourse: buying and selling cost 3% fees")
{
    Preparer(100000);
    EXPECT_EQ(BOURSE.cours[BOURSE_DEVON], 600);
    EXPECT_EQ(Bourse_Acheter(BOURSE_DEVON, 10), 10);
    EXPECT_EQ(GetMoney(&gSaveBlock1Ptr->money), 100000 - 6180);
    EXPECT_EQ(Bourse_Vendre(BOURSE_DEVON, 10), 10);
    EXPECT_EQ(GetMoney(&gSaveBlock1Ptr->money), 100000 - 6180 + 5820);
    EXPECT_EQ(BOURSE.parts[BOURSE_DEVON], 0);
}

TEST("Bourse: cannot buy more than the cash allows")
{
    Preparer(1000);
    EXPECT_EQ(Bourse_Acheter(BOURSE_DEVON, 9999), 1);
    EXPECT_EQ(Bourse_Acheter(BOURSE_DEVON, 1), 0);
}

TEST("Bourse: prices change every day and stay within bounds")
{
    u32 j, i;
    Preparer(0);
    for (j = 101; j < 400; j++)
    {
        Bourse_MettreAJourJusqua(j);
        for (i = 0; i < BOURSE_NB_ENTREPRISES; i++)
        {
            EXPECT_GE(BOURSE.cours[i], BOURSE_COURS_MIN);
            EXPECT_LE(BOURSE.cours[i], BOURSE_COURS_MAX);
        }
    }
    EXPECT_EQ(BOURSE.jour, 399);
}

TEST("Bourse: the same day always gives the same move")
{
    EXPECT_EQ(Bourse_VariationDuJour(BOURSE_CASINO, 1234), Bourse_VariationDuJour(BOURSE_CASINO, 1234));
}

TEST("Bourse: story shocks apply once")
{
    u16 avant;
    Preparer(0);
    avant = BOURSE.cours[BOURSE_DEVON];
    FlagSet(FLAG_RECOVERED_DEVON_GOODS);
    Bourse_AppliquerChocs();
    EXPECT_EQ(BOURSE.cours[BOURSE_DEVON], avant * 14 / 10);
    Bourse_AppliquerChocs();
    EXPECT_EQ(BOURSE.cours[BOURSE_DEVON], avant * 14 / 10);
    FlagClear(FLAG_RECOVERED_DEVON_GOODS);
}

TEST("Bourse: loans are capped and cost 1% a day")
{
    Preparer(0);
    EXPECT_EQ(Bourse_Emprunter(50000), BOURSE_CREDIT_BASE);
    EXPECT_EQ(Bourse_Emprunter(5000), 0);
    Bourse_PasserJour(101);
    EXPECT_EQ(BOURSE.dette, BOURSE_CREDIT_BASE + BOURSE_CREDIT_BASE / 100);
    EXPECT_EQ(Bourse_Rembourser(), BOURSE_CREDIT_BASE);
    EXPECT_EQ(BOURSE.dette, BOURSE_CREDIT_BASE / 100);
}

TEST("Bourse: the bailiff sells everything and seizes cash when debt exceeds holdings")
{
    Preparer(0);
    BOURSE.parts[BOURSE_PENSION] = 10; // 10 x 100 = 1 000
    BOURSE.dette = 20000;
    SetMoney(&gSaveBlock1Ptr->money, 5000);
    Bourse_VerifierHuissier();
    EXPECT_EQ(BOURSE.parts[BOURSE_PENSION], 0);
    EXPECT_EQ(GetMoney(&gSaveBlock1Ptr->money), 0);
    EXPECT_EQ(BOURSE.dette, 20000 - 900 - 5000);
    EXPECT_EQ(BOURSE.huissier, 900 + 5000);
}

TEST("Bourse: no bailiff while holdings cover the debt")
{
    Preparer(0);
    BOURSE.parts[BOURSE_DEVON] = 100; // 60 000
    BOURSE.dette = 60000;
    Bourse_VerifierHuissier();
    EXPECT_EQ(BOURSE.parts[BOURSE_DEVON], 100);
    EXPECT_EQ(BOURSE.huissier, 0);
}
