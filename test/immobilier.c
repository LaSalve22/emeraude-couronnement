#include "global.h"
#include "immobilier.h"
#include "event_data.h"
#include "money.h"
#include "test/test.h"
#include "constants/flags.h"

// Émeraude Couronnement (v0.34) : immobilier.

#define IMMO (gSaveBlock1Ptr->immo)

static void Preparer(u32 argent)
{
    memset(&IMMO, 0, sizeof(IMMO));
    SetMoney(&gSaveBlock1Ptr->money, argent);
    Immo_MettreAJourJusqua(700);
}

static u32 BienPasEnPromo(u32 jour)
{
    return Immo_BonneAffaire(jour) == 0 ? 1 : 0;
}

TEST("Immo: buying adds 8% notary fees and selling keeps 95%")
{
    u32 b, prix;
    Preparer(500000);
    b = BienPasEnPromo(700);
    prix = Immo_Prix(b, 700);
    EXPECT(Immo_Acheter(b, 700));
    EXPECT(Immo_Possede(b));
    EXPECT_EQ(GetMoney(&gSaveBlock1Ptr->money), 500000 - prix * 108 / 100);
    EXPECT(!Immo_Acheter(b, 700));
    EXPECT(Immo_Vendre(b, 700));
    EXPECT(!Immo_Possede(b));
    EXPECT_EQ(GetMoney(&gSaveBlock1Ptr->money), 500000 - prix * 108 / 100 + prix * 95 / 100);
}

TEST("Immo: cannot buy without enough money")
{
    Preparer(1000);
    EXPECT(!Immo_Acheter(0, 700));
    EXPECT(!Immo_Possede(0));
}

TEST("Immo: the weekly deal and local prices stay the same all week")
{
    u32 j;
    Preparer(0);
    for (j = 701; j < 707; j++)
    {
        EXPECT_EQ(Immo_BonneAffaire(j), Immo_BonneAffaire(700));
        EXPECT_EQ(Immo_Prix(3, j), Immo_Prix(3, 700));
    }
}

TEST("Immo: owned homes bring rent over time and the index stays bounded")
{
    u32 j, avant;
    Preparer(500000);
    FlagSet(FLAG_SOOTOPOLIS_ARCHIE_MAXIE_LEAVE); // plus de squatteurs
    EXPECT(Immo_Acheter(BienPasEnPromo(700), 700));
    avant = GetMoney(&gSaveBlock1Ptr->money);
    for (j = 701; j <= 760; j++)
    {
        Immo_MettreAJourJusqua(j);
        EXPECT_GE(IMMO.indice, IMMO_INDICE_MIN);
        EXPECT_LE(IMMO.indice, IMMO_INDICE_MAX);
    }
    EXPECT_GT(GetMoney(&gSaveBlock1Ptr->money), avant);
    EXPECT_GT(IMMO.loyers, 0);
    EXPECT_EQ(IMMO.squat, 0);
    FlagClear(FLAG_SOOTOPOLIS_ARCHIE_MAXIE_LEAVE);
}

TEST("Immo: a squatted home earns nothing until evicted")
{
    u32 b, avant;
    Preparer(500000);
    FlagSet(FLAG_SOOTOPOLIS_ARCHIE_MAXIE_LEAVE);
    b = BienPasEnPromo(700);
    EXPECT(Immo_Acheter(b, 700));
    IMMO.squat = 1u << b;
    avant = GetMoney(&gSaveBlock1Ptr->money);
    Immo_MettreAJourJusqua(720);
    EXPECT_EQ(GetMoney(&gSaveBlock1Ptr->money), avant);
    Immo_Expulser(b);
    EXPECT_EQ(IMMO.squat, 0);
    FlagClear(FLAG_SOOTOPOLIS_ARCHIE_MAXIE_LEAVE);
}

TEST("Immo: garbage in the old save area triggers a clean init")
{
    memset(&IMMO, 0xAA, sizeof(IMMO));
    Immo_MettreAJourJusqua(50);
    EXPECT_EQ(IMMO.magie, IMMO_MAGIE);
    EXPECT_EQ(IMMO.possede, 0);
    EXPECT_EQ(IMMO.indice, 1000);
}
