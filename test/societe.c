#include "global.h"
#include "societe.h"
#include "immobilier.h"
#include "event_data.h"
#include "money.h"
#include "string_util.h"
#include "test/test.h"
#include "constants/flags.h"

// Émeraude Couronnement (v0.35) : société du joueur.

#define SOC (gSaveBlock2Ptr->pokedex.societe)
#define IMMO (gSaveBlock1Ptr->immo)

static const u8 sNom[] = _("TEST CORP");

static void Preparer(u32 argent)
{
    memset(&SOC, 0, sizeof(SOC));
    memset(&IMMO, 0, sizeof(IMMO));
    SetMoney(&gSaveBlock1Ptr->money, argent);
    Immo_MettreAJourJusqua(700);
}

TEST("Societe: created with the chosen name and 100% of the shares")
{
    Preparer(0);
    Societe_Creer(sNom, 700);
    EXPECT_EQ(SOC.etat, SOCIETE_ETAT_CREEE);
    EXPECT_EQ(StringCompare(SOC.nom, sNom), 0);
    EXPECT_EQ(SOC.actionsJoueur, SOCIETE_ACTIONS);
}

TEST("Societe: rents go to the treasury, withdrawal pays the player's share")
{
    u32 j, avant;
    Preparer(500000);
    FlagSet(FLAG_SOOTOPOLIS_ARCHIE_MAXIE_LEAVE);
    EXPECT(Immo_Acheter(Immo_BonneAffaire(700) == 0 ? 1 : 0, 700));
    Societe_Creer(sNom, 700);
    avant = GetMoney(&gSaveBlock1Ptr->money);
    for (j = 701; j <= 730; j++)
        Immo_MettreAJourJusqua(j);
    EXPECT_GT(SOC.tresorerie, 0);
    EXPECT_LE(GetMoney(&gSaveBlock1Ptr->money), avant); // les loyers ne vont plus au joueur
    SOC.actionsJoueur = 7500;
    SOC.tresorerie = 10000;
    SetMoney(&gSaveBlock1Ptr->money, 0);
    EXPECT_EQ(Societe_RetirerTresorerie(), 7500);
    EXPECT_EQ(SOC.tresorerie, 0);
    FlagClear(FLAG_SOOTOPOLIS_ARCHIE_MAXIE_LEAVE);
}

TEST("Societe: IPO needs 100 000 of value and sells 25% of the shares")
{
    u32 cours;
    Preparer(0);
    Societe_Creer(sNom, 700);
    SOC.tresorerie = 50000;
    EXPECT(!Societe_Introduire(700));
    SOC.tresorerie = 200000;
    cours = Societe_Cours(700);
    EXPECT_EQ(cours, 20);
    EXPECT(Societe_Introduire(700));
    EXPECT_EQ(SOC.etat, SOCIETE_ETAT_COTEE);
    EXPECT_EQ(SOC.actionsJoueur, SOCIETE_ACTIONS - SOCIETE_INTRO_ACTIONS);
    EXPECT_EQ(GetMoney(&gSaveBlock1Ptr->money), SOCIETE_INTRO_ACTIONS * cours * 97 / 100);
}

TEST("Societe: the player always keeps 51%")
{
    Preparer(0);
    Societe_Creer(sNom, 700);
    SOC.tresorerie = 200000;
    EXPECT(Societe_Introduire(700));
    EXPECT(Societe_VendreActions(700)); // 7000
    EXPECT(Societe_VendreActions(700)); // 6500
    EXPECT(Societe_VendreActions(700)); // 6000
    EXPECT(Societe_VendreActions(700)); // 5500
    EXPECT(!Societe_VendreActions(700)); // 5000 < 51 %
    EXPECT_EQ(SOC.actionsJoueur, 5500);
    EXPECT(Societe_RacheterActions(700));
    EXPECT_EQ(SOC.actionsJoueur, 6000);
}

TEST("Societe: market mood stays bounded")
{
    u32 j;
    Preparer(0);
    Societe_Creer(sNom, 700);
    for (j = 701; j < 1100; j++)
    {
        Societe_MettreAJourJusqua(j);
        EXPECT_GE(SOC.sentiment, SOCIETE_SENTIMENT_MIN);
        EXPECT_LE(SOC.sentiment, SOCIETE_SENTIMENT_MAX);
    }
}
