#include "global.h"
#include "league_club.h"
#include "money.h"
#include "tycoon.h"
#include "event_data.h"
#include "test/test.h"

// Émeraude Couronnement (v0.46) : règles de l'édition Carrière & Business (les fonctions existent dans les deux éditions).

TEST("Carriere: field moves unlock with the club tier, taxis always")
{
    memset(&gSaveBlock3Ptr->ligueClubs, 0, sizeof(gSaveBlock3Ptr->ligueClubs));
    SetMoney(&gSaveBlock1Ptr->money, 100000);
    Club_MettreAJourJusqua(1000);
    EXPECT(Tycoon_CapaciteDebloquee(FIELD_MOVE_SURF));
    EXPECT(Tycoon_CapaciteDebloquee(FIELD_MOVE_FLY));
    EXPECT(!Tycoon_CapaciteDebloquee(FIELD_MOVE_CUT));   // pas de club
    EXPECT(Club_Signer(2));                              // SC Vermilava, tier B
    EXPECT(Tycoon_CapaciteDebloquee(FIELD_MOVE_CUT));
    EXPECT(Tycoon_CapaciteDebloquee(FIELD_MOVE_STRENGTH));
    EXPECT(!Tycoon_CapaciteDebloquee(FIELD_MOVE_DIVE));
    EXPECT(!Tycoon_CapaciteDebloquee(FIELD_MOVE_TELEPORT)); // règle d'origine, pas gérée ici
}

TEST("Carriere: the Champions rank is remembered")
{
    memset(&gSaveBlock3Ptr->couronnementSaison, 0, sizeof(gSaveBlock3Ptr->couronnementSaison));
    EXPECT(!Couronnement_ChampionsAtteint());
    gSaveBlock3Ptr->couronnementSaison.paliers |= COURONNEMENT_CHAMPIONS_ATTEINT;
    EXPECT(Couronnement_ChampionsAtteint());
}

TEST("Carriere: the starting profile sets the money")
{
    gSpecialVar_0x8004 = 0;
    Tycoon_ChoisirProfil();
    EXPECT_EQ(GetMoney(&gSaveBlock1Ptr->money), 12000);
    gSpecialVar_0x8004 = 2;
    Tycoon_ChoisirProfil();
    EXPECT_EQ(GetMoney(&gSaveBlock1Ptr->money), 6000);
    EXPECT_EQ(Tycoon_Profil(), 2);
}
