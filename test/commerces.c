#include "global.h"
#include "commerces.h"
#include "societe.h"
#include "immobilier.h"
#include "caps.h"
#include "event_data.h"
#include "money.h"
#include "pokemon.h"
#include "test/test.h"
#include "constants/vars.h"

// Émeraude Couronnement (v0.36) : commerces de la société.

#define SOC (gSaveBlock2Ptr->pokedex.societe)

static const u8 sNom[] = _("TEST CORP");

static void Preparer(u32 argent)
{
    u32 v;
    for (v = VAR_COMMERCE_BOUTIQUE; v <= VAR_COMMERCE_REMETTRE; v++)
        VarSet(v, 0);
    memset(&SOC, 0, sizeof(SOC));
    memset(&gSaveBlock1Ptr->immo, 0, sizeof(gSaveBlock1Ptr->immo));
    SetMoney(&gSaveBlock1Ptr->money, argent);
    Societe_Creer(sNom, 700);
    VarSet(VAR_COMMERCE_JOUR, 700);
}

TEST("Commerces: opening a shop costs the premises and its revenue goes to the treasury")
{
    u32 j;
    Preparer(500000);
    EXPECT(Commerce_OuvrirBoutique(9, 1)); // Nénucrique, soins : 135 000
    EXPECT_EQ(GetMoney(&gSaveBlock1Ptr->money), 500000 - 135000);
    EXPECT(!Commerce_OuvrirBoutique(0, 0));
    VarSet(VAR_COMMERCE_JOUR, 700);
    for (j = 701; j <= 710; j++)
        Commerce_MettreAJourJusqua(j);
    EXPECT_GT(SOC.tresorerie, 0);
    EXPECT_EQ(GetMoney(&gSaveBlock1Ptr->money), 500000 - 135000);
}

TEST("Commerces: upgrades raise revenue and stop at level 3")
{
    u32 avant;
    Preparer(500000);
    EXPECT(Commerce_OuvrirBoutique(0, 1));
    avant = Commerce_RecetteMoyenneBoutique();
    EXPECT(Commerce_AgrandirBoutique());
    EXPECT_GT(Commerce_RecetteMoyenneBoutique(), avant);
    EXPECT(Commerce_AgrandirBoutique());
    EXPECT(!Commerce_AgrandirBoutique());
    EXPECT_EQ(Commerce_PrixAgrandirBoutique(), 0);
}

TEST("Commerces: a competitor cuts revenue and leaves after 5 days of low prices")
{
    u32 j, sans;
    Preparer(500000);
    EXPECT(Commerce_OuvrirBoutique(5, 1));
    sans = Commerce_RecetteMoyenneBoutique();
    VarSet(VAR_COMMERCE_BOUTIQUE, VarGet(VAR_COMMERCE_BOUTIQUE) | (1 << 11));
    EXPECT_EQ(Commerce_RecetteMoyenneBoutique(), sans * COMMERCE_CONCURRENT_RECETTE / 100);
    Commerce_ChoisirPrix(0);
    VarSet(VAR_COMMERCE_JOUR, 700);
    for (j = 701; j <= 700 + COMMERCE_GUERRE_JOURS; j++)
        Commerce_PasserJour(j);
    EXPECT_EQ(Commerce_EtatBoutique(), 1);
}

TEST("Commerces: training center points level up a Pokemon but never past the cap")
{
    struct Pokemon mon;
    u32 cap = GetCurrentLevelCap();
    Preparer(500000);
    EXPECT(Commerce_OuvrirCentre());
    VarSet(VAR_COMMERCE_POINTS, 1000);
    CreateMon(&mon, SPECIES_ZIGZAGOON, cap - 1, 0, OTID_STRUCT_PRESET(0));
    EXPECT(Commerce_SeanceXP(&mon));
    EXPECT_EQ(GetMonData(&mon, MON_DATA_LEVEL), cap);
    EXPECT(!Commerce_SeanceXP(&mon));
    EXPECT_EQ(VarGet(VAR_COMMERCE_POINTS), 1000 - COMMERCE_SEANCE_XP);
    EXPECT(Commerce_SeanceEV(&mon, 5));
    EXPECT_EQ(GetMonData(&mon, MON_DATA_SPEED_EV), COMMERCE_EV_PAR_SEANCE);
}

TEST("Commerces: businesses count in the company value")
{
    u32 avant;
    Preparer(500000);
    avant = Societe_Valeur(700);
    EXPECT(Commerce_OuvrirCentre());
    EXPECT_EQ(Societe_Valeur(700), avant + COMMERCE_CENTRE_PRIX);
}
