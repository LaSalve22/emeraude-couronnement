#include "global.h"
#include "couronnement.h"
#include "event_data.h"
#include "item.h"
#include "money.h"
#include "test/test.h"
#include "constants/items.h"

// Émeraude Couronnement (v0.39) : saisons du Couronnement Mondial.

#define SAISON (gSaveBlock3Ptr->couronnementSaison)
#define FINALE (COURONNEMENT_TOURNOI_FINALE * 3)

static void Preparer(u32 saison, u16 elo)
{
    memset(&SAISON, 0, sizeof(SAISON));
    VarSet(VAR_COURONNEMENT_TOURNOIS, 0);
    VarSet(VAR_COURONNEMENT_ELO, elo);
    SetMoney(&gSaveBlock1Ptr->money, 0);
    Couronnement_Saison(); // initialise
    SAISON.saison = saison;
}

TEST("Saisons: season 1 keeps the original opponents")
{
    u32 i;
    Preparer(1, 1500);
    for (i = 0; i < COURONNEMENT_ADV_FINALISTES_SAISON; i++)
        EXPECT_EQ(Couronnement_EquipeDeLaSaison(i), i);
    EXPECT_EQ(Couronnement_NiveauDeLaSaison(0), 0);
}

TEST("Saisons: later seasons rotate teams, level them up and change finalists")
{
    u32 t, a, b, c;
    Preparer(2, 1500);
    for (t = 0; t < COURONNEMENT_TOURNOI_FINALE; t++)
    {
        a = Couronnement_EquipeDeLaSaison(t * 3);
        b = Couronnement_EquipeDeLaSaison(t * 3 + 1);
        c = Couronnement_EquipeDeLaSaison(t * 3 + 2);
        EXPECT(a != b && b != c && a != c);
        EXPECT_GE(a, 12);
        EXPECT_LT(a, 39);
    }
    EXPECT_NE(Couronnement_EquipeDeLaSaison(FINALE + 3), Couronnement_EquipeDeLaSaison(FINALE + 4));
    EXPECT_EQ(Couronnement_NiveauDeLaSaison(0), 88);
    EXPECT_EQ(Couronnement_NiveauDeLaSaison(FINALE), 100);
    EXPECT_EQ(Couronnement_EloAdversaire(0), 1350 + 100);
}

TEST("Saisons: reaching a tier pays once per season")
{
    Preparer(1, 1500);
    VarSet(VAR_COURONNEMENT_ELO, 1650);
    Couronnement_RecompensePalier();
    EXPECT_EQ(gSpecialVar_Result, TRUE);
    EXPECT_EQ(GetMoney(&gSaveBlock1Ptr->money), 100000);
    Couronnement_RecompensePalier();
    EXPECT_EQ(gSpecialVar_Result, FALSE);
    EXPECT_EQ(GetMoney(&gSaveBlock1Ptr->money), 100000);
}

TEST("Saisons: the final ends the season and halves the Elo gap")
{
    Preparer(1, 2000);
    VarSet(VAR_COURONNEMENT_TOURNOIS, 0x1FFF);
    gSpecialVar_0x8004 = TRUE;
    Couronnement_FinSaison();
    EXPECT_EQ(Couronnement_Saison(), 2);
    EXPECT_EQ(Couronnement_Titres(), 1);
    EXPECT_EQ(Couronnement_GetElo(), 1750);
    EXPECT_EQ(VarGet(VAR_COURONNEMENT_TOURNOIS), 0);
    EXPECT_EQ(GetMoney(&gSaveBlock1Ptr->money), 3500000);
    EXPECT_EQ(Couronnement_MultiplicateurSaison(), 150);
}
