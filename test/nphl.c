#include "global.h"
#include "league_club.h"
#include "money.h"
#include "test/test.h"

// Émeraude Couronnement (v0.56), édition Carrière : NPHL. Ces tests ne tournent qu'avec make check EDITION=carriere.
#if CLUB_NPHL

static void Preparer(void)
{
    memset(&gSaveBlock3Ptr->ligueClubs, 0, sizeof(gSaveBlock3Ptr->ligueClubs));
    SetMoney(&gSaveBlock1Ptr->money, 100000);
    Club_MettreAJourJusqua(1000);
}

TEST("NPHL: every club meets every other twice, 9 times at home")
{
    u32 m, a, b, rencontres[CLUB_NB][CLUB_NB] = {0}, domicile[CLUB_NB] = {0};
    Preparer();
    for (m = 0; m < CLUB_NB_MATCHS; m++)
    {
        u8 dom, ext;
        Club_Affiche(m, &dom, &ext);
        EXPECT_NE(dom, ext);
        rencontres[dom][ext]++;
        domicile[dom]++;
    }
    for (a = 0; a < CLUB_NB; a++)
    {
        EXPECT_EQ(domicile[a], 9);
        for (b = 0; b < CLUB_NB; b++)
            if (a != b)
                EXPECT_EQ(rencontres[a][b], 1);
    }
}

TEST("NPHL: a full season ends with play-offs between the top 4")
{
    u8 ordre[CLUB_NB], d1, e1, d2, e2, df, ef;
    Preparer();
    Club_MettreAJourJusqua(1000 + CLUB_JOUR_FINALE - 1);
    EXPECT_EQ(Club_JourDeSaison(), CLUB_JOUR_FINALE);
    Club_Classement(0, ordre);
    Club_Affiche(CLUB_MATCH_DEMI1, &d1, &e1);
    Club_Affiche(CLUB_MATCH_DEMI2, &d2, &e2);
    EXPECT_EQ(d1, ordre[0]);
    EXPECT_EQ(e1, ordre[3]);
    EXPECT_EQ(d2, ordre[1]);
    EXPECT_EQ(e2, ordre[2]);
    Club_Affiche(CLUB_MATCH_FINALE, &df, &ef);
    EXPECT(df == d1 || df == e1);
    EXPECT(ef == d2 || ef == e2);
    EXPECT_NE(Club_Vainqueur(), CLUB_AUCUN);
}

#endif
