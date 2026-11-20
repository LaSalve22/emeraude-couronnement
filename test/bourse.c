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
    memset(&gSaveBlock3Ptr->bourseOr, 0, sizeof(gSaveBlock3Ptr->bourseOr));
    memset(&gSaveBlock3Ptr->bourseCourt, 0, sizeof(gSaveBlock3Ptr->bourseCourt));
    FlagClear(FLAG_RECOVERED_DEVON_GOODS);
    FlagClear(FLAG_SOOTOPOLIS_ARCHIE_MAXIE_LEAVE);
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

// v0.38 : historique des cours pour le graphique.
TEST("Bourse: the price history ends on today's price and candles chain")
{
    struct BourseBougie b0, b1;
    u32 e;
    Preparer(0);
    Bourse_MettreAJourJusqua(704); // mardi
    for (e = 0; e < BOURSE_NB_ENTREPRISES; e++)
    {
        EXPECT_EQ(Bourse_HistoriqueCloture(e, 0), gSaveBlock2Ptr->pokedex.bourse.cours[e]);
        Bourse_Bougie(e, 0, &b0);
        Bourse_Bougie(e, 1, &b1);
        EXPECT_EQ(b0.ouverture, b1.cloture);
        EXPECT_GE(b0.haut, max(b0.ouverture, b0.cloture));
        EXPECT_LE(b0.bas, min(b0.ouverture, b0.cloture));
    }
}

TEST("Bourse: a new candle only on trading days")
{
    u32 avant;
    Preparer(0);
    Bourse_MettreAJourJusqua(704); // mardi
    avant = Bourse_HistoriqueCloture(BOURSE_CASINO, 0);
    Bourse_MettreAJourJusqua(705); // mercredi : nouvelle bougie
    EXPECT_EQ(Bourse_HistoriqueCloture(BOURSE_CASINO, 1), avant);
    EXPECT_EQ(Bourse_HistoriqueCloture(BOURSE_CASINO, 0), gSaveBlock2Ptr->pokedex.bourse.cours[BOURSE_CASINO]);
    avant = Bourse_HistoriqueCloture(BOURSE_CASINO, 1);
    Bourse_MettreAJourJusqua(708); // jeudi, vendredi, samedi : 2 bougies de plus seulement
    EXPECT_EQ(Bourse_HistoriqueCloture(BOURSE_CASINO, 3), avant);
}

// v0.41 : l'or XAU/¥.
TEST("Bourse: gold starts at 20000 per ounce and costs 1% fees")
{
    Preparer(1000000);
    EXPECT_EQ(Bourse_Cours(BOURSE_OR), BOURSE_OR_DEPART);
    EXPECT_EQ(Bourse_Acheter(BOURSE_OR, 10), 10);
    EXPECT_EQ(GetMoney(&gSaveBlock1Ptr->money), 1000000 - 202000);
    EXPECT_EQ(Bourse_Parts(BOURSE_OR), 10);
    EXPECT_EQ(Bourse_ValeurPortefeuille(), 200000);
    EXPECT_EQ(Bourse_Vendre(BOURSE_OR, 10), 10);
    EXPECT_EQ(GetMoney(&gSaveBlock1Ptr->money), 1000000 - 202000 + 198000);
}

TEST("Bourse: gold moves gently, has a history and shines in a crash")
{
    u32 j, n = 0;
    struct BourseBougie b;
    Preparer(0);
    Bourse_MettreAJourJusqua(704);
    EXPECT_EQ(Bourse_HistoriqueCloture(BOURSE_OR, 0), Bourse_Cours(BOURSE_OR));
    Bourse_Bougie(BOURSE_OR, 0, &b);
    EXPECT_GE(b.haut, max(b.ouverture, b.cloture));
    for (j = 1; j < 2000; j++)
    {
        s32 v = Bourse_VariationOr(j);
        EXPECT_GE(v, -100);
        EXPECT_LE(v, 250);
        if (v > 0)
            n++;
    }
    EXPECT_GT(n, 900); // légère tendance haussière
}

// v0.44 : vente à découvert

#define CT (gSaveBlock3Ptr->bourseCourt)

TEST("Bourse: short selling blocks a 50% deposit plus 3% fees and pays on a fall")
{
    Preparer(100000);
    EXPECT_EQ(Bourse_OuvrirCourt(BOURSE_DEVON, 100), 100); // 60 000 vendus : 30 000 bloqués + 1 800 de frais
    EXPECT_EQ(GetMoney(&gSaveBlock1Ptr->money), 100000 - 31800);
    EXPECT_EQ(CT.garantie[BOURSE_DEVON], 30000);
    EXPECT_EQ(CT.prixMoyen[BOURSE_DEVON], 600);
    BOURSE.cours[BOURSE_DEVON] = 500; // le cours baisse de 100
    EXPECT_EQ(Bourse_ResultatCourt(BOURSE_DEVON), 10000);
    EXPECT_EQ(Bourse_RacheterCourt(BOURSE_DEVON, 100), 100); // garantie + 10 000 - 1 500 de frais
    EXPECT_EQ(GetMoney(&gSaveBlock1Ptr->money), 100000 - 31800 + 30000 + 10000 - 1500);
    EXPECT_EQ(Bourse_PartsCourtes(BOURSE_DEVON), 0);
}

TEST("Bourse: a short is bought back automatically when the loss reaches 80% of the deposit")
{
    Preparer(100000);
    Bourse_OuvrirCourt(BOURSE_DEVON, 100);
    BOURSE.cours[BOURSE_DEVON] = 830; // perte 23 000 < 24 000
    Bourse_VerifierCourts();
    EXPECT_EQ(Bourse_PartsCourtes(BOURSE_DEVON), 100);
    BOURSE.cours[BOURSE_DEVON] = 840; // perte 24 000 = 80 % de 30 000
    Bourse_VerifierCourts();
    EXPECT_EQ(Bourse_PartsCourtes(BOURSE_DEVON), 0);
    EXPECT_NE(CT.forces & (1 << BOURSE_DEVON), 0);
}

TEST("Bourse: a bankruptcy closes the short at zero for the maximum gain")
{
    Preparer(100000);
    Bourse_OuvrirCourt(BOURSE_AQUA, 10); // 8 000 vendus : 4 000 bloqués + 240 de frais
    FlagSet(FLAG_SOOTOPOLIS_ARCHIE_MAXIE_LEAVE);
    Bourse_VerifierCourts();
    EXPECT_EQ(Bourse_PartsCourtes(BOURSE_AQUA), 0);
    EXPECT_EQ(GetMoney(&gSaveBlock1Ptr->money), 100000 - 4240 + 4000 + 8000);
    EXPECT_EQ(Bourse_OuvrirCourt(BOURSE_AQUA, 10), 0); // plus de vente à découvert sur une boîte coulée
    FlagClear(FLAG_SOOTOPOLIS_ARCHIE_MAXIE_LEAVE);
}

TEST("Bourse: borrowing fees of 0.1% a day are charged on open shorts")
{
    Preparer(100000);
    Bourse_OuvrirCourt(BOURSE_DEVON, 100);
    SetMoney(&gSaveBlock1Ptr->money, 1000);
    Bourse_PasserJour(101);
    EXPECT_LE(GetMoney(&gSaveBlock1Ptr->money), 1000 - 50); // ~60 sur 60 000 de parts empruntées
    EXPECT_GT(CT.fraisCumules, 0);
}
