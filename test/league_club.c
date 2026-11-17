#include "global.h"
#include "league_club.h"
#include "event_data.h"
#include "item.h"
#include "money.h"
#include "bg.h"
#include "malloc.h"
#include "window.h"
#include "test/test.h"
#include "constants/items.h"
#include "constants/flags.h"

// Émeraude Couronnement (v0.42) : Ligue des Clubs.

#define LC (gSaveBlock3Ptr->ligueClubs)
#define DEBUT 1000

static void Preparer(u32 argent)
{
    memset(&LC, 0, sizeof(LC));
    SetMoney(&gSaveBlock1Ptr->money, argent);
    Club_MettreAJourJusqua(DEBUT); // jour 1 de la saison 0
}

TEST("Clubs: model 1 scale")
{
    u8 d, e;
    CalculateDuelPoints(6, 0, &d, &e);
    EXPECT_EQ(d, 5); EXPECT_EQ(e, 0);   // victoire + bonus offensif
    CalculateDuelPoints(1, 0, &d, &e);
    EXPECT_EQ(d, 4); EXPECT_EQ(e, 1);   // l'adversaire perd 5-6 : bonus défensif
    CalculateDuelPoints(3, 0, &d, &e);
    EXPECT_EQ(d, 4); EXPECT_EQ(e, 0);
    CalculateDuelPoints(0, 0, &d, &e);
    EXPECT_EQ(d, 2); EXPECT_EQ(e, 2);   // double K.O.
    CalculateDuelPoints(0, 6, &d, &e);
    EXPECT_EQ(d, 0); EXPECT_EQ(e, 5);
    EXPECT_EQ(Club_PointsDuel(CLUB_DUEL_EXTERIEUR(1), TRUE), 1);
    EXPECT_EQ(Club_PointsDuel(CLUB_DUEL_EXTERIEUR(1), FALSE), 4);
    EXPECT_EQ(Club_KOInfliges(CLUB_DUEL_DOMICILE(2), FALSE), 4);
    EXPECT_EQ(Club_KOInfliges(CLUB_DUEL_DOMICILE(2), TRUE), 6);
}

TEST("Clubs: each club plays 8 matches, 4 at home, inside its pool")
{
    u32 c, m;
    u8 dom, ext, clubs[CLUB_NB];
    u8 domicile[CLUB_NB] = {0}, total[CLUB_NB] = {0};

    Preparer(0);
    Club_Tirage(0, clubs);
    for (m = 0; m < CLUB_NB_MATCHS; m++)
    {
        Club_Affiche(m, &dom, &ext);
        EXPECT_NE(dom, ext);
        EXPECT_EQ(Club_Poule(dom), Club_Poule(ext));
        EXPECT_EQ(Club_Poule(dom), (m % CLUB_MATCHS_PAR_JOURNEE) / 2);
        domicile[dom]++;
        total[dom]++;
        total[ext]++;
    }
    for (c = 0; c < CLUB_NB; c++)
    {
        EXPECT_EQ(total[c], 8);
        EXPECT_EQ(domicile[c], 4);
    }
}

TEST("Clubs: a whole season is simulated and the final has a winner")
{
    u32 m, s, total = 0;
    u8 a[CLUB_PAR_POULE];

    Preparer(0);
    Club_MettreAJourJusqua(DEBUT + 28); // jour 29 : les 10 journées sont jouées
    for (m = 0; m < CLUB_NB_MATCHS; m++)
        for (s = 0; s < CLUB_DRESSEURS; s++)
            EXPECT_NE(Club_Duel(m, s), CLUB_DUEL_PAS_JOUE);
    EXPECT_EQ(Club_Classement(0, a), 20);
    for (s = 0; s < CLUB_PAR_POULE; s++)
        total += Club_MatchsJoues(a[s]);
    EXPECT_EQ(total, 40);
    EXPECT(Club_Points(a[0]) >= Club_Points(a[1]));
    EXPECT_EQ(Club_Vainqueur(), CLUB_AUCUN);
    Club_MettreAJourJusqua(DEBUT + 29); // jour 30 : finale
    EXPECT_NE(Club_Vainqueur(), CLUB_AUCUN);
    EXPECT_EQ(Club_JourDeSaison(), 30);
}

TEST("Clubs: the simulation is deterministic and the clock cannot be rewound")
{
    u16 avant[CLUB_LIGUE_NB_DUELS];
    Preparer(0);
    Club_MettreAJourJusqua(DEBUT + 10);
    memcpy(avant, LC.duels, sizeof(avant));
    Club_MettreAJourJusqua(DEBUT + 3); // horloge remise en arrière : rien ne bouge
    EXPECT_EQ(memcmp(avant, LC.duels, sizeof(avant)), 0);
    Preparer(0);
    Club_MettreAJourJusqua(DEBUT + 10);
    EXPECT_EQ(memcmp(avant, LC.duels, sizeof(avant)), 0);
}

TEST("Clubs: the player's duel waits, pays a bonus, or is forfeited")
{
    u32 club = 0, m, argent;
    u8 dom, ext;

    Preparer(10000);
    EXPECT(Club_Signer(club));
    EXPECT_EQ(GetMoney(&gSaveBlock1Ptr->money), 10000 - Club_PrixLicence(club));
    EXPECT_EQ(Club_DuJoueur(), club);
    for (m = 2; m < 20 && Club_MatchDuJoueur() == 0xFF; m++)
        Club_MettreAJourJusqua(DEBUT + m); // jusqu'à la première journée de son club
    m = Club_MatchDuJoueur();
    EXPECT_NE(m, 0xFF);
    Club_Affiche(m, &dom, &ext);
    EXPECT(dom == club || ext == club);
    EXPECT_EQ(Club_Duel(m, CLUB_SLOT_JOUEUR), CLUB_DUEL_PAS_JOUE);
    EXPECT_NE(Club_Duel(m, 0), CLUB_DUEL_PAS_JOUE);

    argent = GetMoney(&gSaveBlock1Ptr->money);
    Club_EnregistrerDuelJoueur(dom == club ? CLUB_DUEL_DOMICILE(6) : CLUB_DUEL_EXTERIEUR(6));
    EXPECT_EQ(Club_PointsDuel(Club_Duel(m, CLUB_SLOT_JOUEUR), dom == club), 5);
    EXPECT(GetMoney(&gSaveBlock1Ptr->money) >= argent + 800 + 400 * 5);
    EXPECT_EQ(Club_MatchDuJoueur(), 0xFF);

    // Duel suivant pas joué : forfait (0-6) quand la journée d'après commence.
    Club_MettreAJourJusqua(DEBUT + 26); // jour 27 : dernière journée
    for (m = 0; m < 9 * CLUB_MATCHS_PAR_JOURNEE; m++)
    {
        Club_Affiche(m, &dom, &ext);
        if (dom == club)
            EXPECT_NE(Club_Duel(m, CLUB_SLOT_JOUEUR), CLUB_DUEL_PAS_JOUE);
        if (ext == club)
            EXPECT_NE(Club_Duel(m, CLUB_SLOT_JOUEUR), CLUB_DUEL_PAS_JOUE);
    }
}

TEST("Clubs: transfers are free in the mercato, then cost a release clause")
{
    Preparer(100000);
    EXPECT(Club_Signer(0));                                  // jour 1
    EXPECT_EQ(Club_CoutSignature(2), Club_PrixLicence(2));   // mercato : licence seule
    Club_MettreAJourJusqua(DEBUT + 4);                       // jour 5
    EXPECT_EQ(Club_CoutSignature(2), 8000 + 4000 / 2);       // + 50 % de l'ancienne licence
    EXPECT(Club_Signer(2));
    EXPECT_EQ(Club_DuJoueur(), 2);
    EXPECT_EQ(GetMoney(&gSaveBlock1Ptr->money), 100000 - 4000 - 10000);
    EXPECT_EQ(Club_MatchDuJoueur(), 0xFF); // un duel en attente dans l'ancien club a été joué par la Recrue
    EXPECT(!Club_Signer(2));
}

TEST("Clubs: a new season renews the licence or drops it")
{
    Preparer(4000);
    EXPECT(Club_Signer(0));
    EXPECT_EQ(GetMoney(&gSaveBlock1Ptr->money), 0);
    SetMoney(&gSaveBlock1Ptr->money, 5000);
    Club_MettreAJourJusqua(DEBUT + 30); // jour 1 de la saison 1
    EXPECT_EQ(Club_Saison(), 1);
    EXPECT_EQ(Club_JourDeSaison(), 1);
    EXPECT_EQ(Club_DuJoueur(), 0);
    EXPECT(GetMoney(&gSaveBlock1Ptr->money) <= 5000 - 4000 + 30000);
    EXPECT_EQ(Club_Duel(0, 0), CLUB_DUEL_PAS_JOUE);
    SetMoney(&gSaveBlock1Ptr->money, 0);
    Club_MettreAJourJusqua(DEBUT + 60); // saison 2 : pas d'argent, plus de licence
    EXPECT_EQ(Club_Saison(), 2);
    EXPECT_EQ(Club_DuJoueur(), CLUB_AUCUN);
}

TEST("Clubs: a long absence jumps to the current season")
{
    Preparer(0);
    Club_MettreAJourJusqua(DEBUT + 400);
    EXPECT_EQ(Club_Saison(), 400 / 30);
    EXPECT_EQ(Club_JourDeSaison(), 400 % 30 + 1);
}

TEST("Clubs: home and stadium bonuses raise the rating")
{
    Preparer(0);
    // Le Capitaine de Poivressel (Eau) profite de la pluie de son stade : +6 à domicile, +4 pour le stade.
    EXPECT_EQ(Club_RatingEffectif(6, 0, CLUB_STADE_PLUIE, TRUE), Club_Rating(6, 0) + 10);
    EXPECT_EQ(Club_RatingEffectif(6, 0, CLUB_STADE_NEUTRE, TRUE), Club_Rating(6, 0));
    EXPECT_EQ(Club_RatingEffectif(6, 0, CLUB_STADE_SOLEIL, FALSE), Club_Rating(6, 0));
}

// Aperçu du tableau de bord : les pixels des onglets sont vidés dans le journal (lignes « CLUB m:<hex> »),
// puis python3 tools/couronnement/apercu_clubs.py <journal> <prefixe> en tire des PNG.
TEST("Clubs: dashboard preview")
{
    static const struct BgTemplate sBg = {.bg = 0, .charBaseIndex = 2, .mapBaseIndex = 31, .screenSize = 0, .paletteMode = 0, .priority = 0, .baseTile = 0};
    static const struct WindowTemplate sFen[] = {
        {.bg = 0, .tilemapLeft = 0, .tilemapTop = 0, .width = 30, .height = 13, .paletteNum = 13, .baseBlock = 8},
        DUMMY_WIN_TEMPLATE,
    };
    u32 vue, i, j, m;
    u8 *pix;
    char ligne[2 * 64 + 1];
    static const char sHex[] = "0123456789abcdef";

    Preparer(30000);
    EXPECT(Club_Signer(6));
    for (m = 2; m < 20 && Club_MatchDuJoueur() == 0xFF; m++)
        Club_MettreAJourJusqua(DEBUT + m);
    ResetBgsAndClearDma3BusyFlags(0);
    InitBgsFromTemplates(0, &sBg, 1);
    InitWindows(sFen);
    for (vue = 0; vue < 6; vue++)
    {
        m = Club_MatchDuJoueur();
        if (vue == 4) // le joueur rachète son club : onglets Mercato et Finances
        {
            gSaveBlock2Ptr->pokedex.societe.etat = 1;
            gSaveBlock2Ptr->pokedex.societe.tresorerie = 2000000;
            LC.titres = 1;
            FlagSet(FLAG_BADGE01_GET);
            EXPECT(Club_Racheter(6));
            EXPECT(Club_Recruter(0, 8));
            EXPECT(Club_Coacher(0, 2));
            FlagClear(FLAG_BADGE01_GET);
        }
        if (vue >= 4)
            Club_TableauDessiner(0, vue - 1, 0, 0, 0, 6);
        else if (vue == 0)
            Club_TableauDessiner(0, 0, Club_Poule(6), 0, 0, 6);
        else if (vue == 1)
            Club_TableauDessiner(0, 1, 0, m / CLUB_MATCHS_PAR_JOURNEE, m % CLUB_MATCHS_PAR_JOURNEE, 6);
        else if (vue == 2)
            Club_TableauDessiner(0, 1, 0, 0, 1, 6);
        else
            Club_TableauDessiner(0, 2, 0, 0, 0, 6);
        pix = gWindows[0].tileData;
        EXPECT(pix != NULL);
        for (i = 0; i < 30 * 13 * 32; i += 64)
        {
            for (j = 0; j < 64; j++)
            {
                ligne[2 * j] = sHex[pix[i + j] >> 4];
                ligne[2 * j + 1] = sHex[pix[i + j] & 15];
            }
            ligne[128] = 0;
            Test_MgbaPrintf("CLUB %d:%s", vue, ligne);
        }
    }
    FreeAllWindowBuffers();
}

// ---------------------------------------------------------------- phase 2 : propriétaire

#define SOC (gSaveBlock2Ptr->pokedex.societe)

static void PreparerProprio(u32 tresorerie)
{
    Preparer(0);
    memset(&SOC, 0, sizeof(SOC));
    SOC.etat = 1; // société créée
    SOC.tresorerie = tresorerie;
    LC.titres = 1;
}

TEST("Clubs: buying a club needs a title and a company")
{
    Preparer(1000000);
    memset(&SOC, 0, sizeof(SOC));
    EXPECT(!Club_Racheter(0));             // pas de titre
    LC.titres = 1;
    EXPECT(!Club_Racheter(0));             // pas de société
    SOC.etat = 1;
    SOC.tresorerie = 300000;
    SetMoney(&gSaveBlock1Ptr->money, 0);
    EXPECT(Club_Racheter(0));              // AC Clémenti, tier C : 250 000
    EXPECT_EQ(SOC.tresorerie, 50000);
    EXPECT_EQ(Club_Proprio(), 0);
    EXPECT_EQ(Club_DuJoueur(), 0);
    EXPECT_EQ(Club_Valeur(), 250000);
    EXPECT(!Club_Signer(2));               // un propriétaire joue dans son club
}

TEST("Clubs: the season balance is paid to the company")
{
    u32 avant, lignes[6];
    PreparerProprio(250000);
    EXPECT(Club_Racheter(0));
    Club_Previsionnel(0, FALSE, lignes);
    EXPECT_EQ(lignes[CLUB_BILAN_BILLETTERIE], 4 * 1000 * 50 / 100 * 50); // remplissage 50 % à 0 point
    EXPECT_EQ(lignes[CLUB_BILAN_TV], 20000);
    EXPECT_EQ(lignes[CLUB_BILAN_BOUTIQUE], lignes[CLUB_BILAN_BILLETTERIE] / 10);
    avant = SOC.tresorerie;
    Club_MettreAJourJusqua(DEBUT + 30); // fin de la saison 0
    EXPECT_NE(LC.bilan[CLUB_BILAN_BILLETTERIE], 0);
    EXPECT_EQ(LC.bilan[CLUB_BILAN_IMPOT], ((s32)LC.bilan[CLUB_BILAN_NET] + LC.bilan[CLUB_BILAN_IMPOT]) / 5);
    EXPECT_EQ(SOC.tresorerie, avant + LC.bilan[CLUB_BILAN_NET]);
    EXPECT_EQ(Club_DuJoueur(), 0); // pas de licence à payer
}

TEST("Clubs: recruits, coaching, stadium and resale")
{
    PreparerProprio(2000000);
    EXPECT(Club_Racheter(0));
    FlagClear(FLAG_BADGE01_GET);
    EXPECT(!Club_Recruter(0, 8));          // Roxanne : il faut l'avoir battue
    FlagSet(FLAG_BADGE01_GET);
    EXPECT(Club_Recruter(0, 8));
    EXPECT_EQ(SOC.tresorerie, 2000000 - 250000 - 80 * 80 * 5);
    EXPECT_EQ(Club_Rating(0, 0), 80);
    EXPECT(!Club_Recruter(1, 8));          // déjà au club
    EXPECT(Club_Coacher(0, 2));
    EXPECT_EQ(Club_Rating(0, 0), 82);
    EXPECT(Club_Recruter(0, CLUB_NB_RECRUES)); // retour du Capitaine d'origine
    EXPECT_EQ(Club_Rating(0, 0), 58);
    EXPECT(Club_AgrandirStade());
    EXPECT_EQ(Club_Places(), 4000);
    EXPECT_EQ(Club_Valeur(), 250000 + 150000);
    EXPECT_EQ(Club_Revendre(), (250000 + 150000) * 80 / 100);
    EXPECT_EQ(Club_Proprio(), CLUB_AUCUN);
    FlagClear(FLAG_BADGE01_GET);
}

// ---------------------------------------------------------------- Pokémon de prêt (v0.49)

TEST("Clubs: loaned Pokemon fill the team and leave after the match")
{
    u32 exp;
    Preparer(100000);
    ZeroPlayerPartyMons();
    CreateMon(&gParties[B_TRAINER_PLAYER][0], SPECIES_TREECKO, 20, 1234, OTID_STRUCT_PLAYER_ID);
    CreateMon(&gParties[B_TRAINER_PLAYER][1], SPECIES_POOCHYENA, 18, 5678, OTID_STRUCT_PLAYER_ID);
    CalculatePlayerPartyCount();
    EXPECT(Club_Signer(0));                      // AC Clémenti, tier C : prêt offert la 1re saison
    EXPECT_EQ(Club_PretCout(4), 0);
    Club_PretPreparer(0, 20, 4);
    EXPECT_EQ(CalculatePlayerPartyCount(), 6);
    EXPECT_EQ(Club_PretNombre(), 4);
    EXPECT_EQ(GetMonData(&gParties[B_TRAINER_PLAYER][2], MON_DATA_LEVEL), 20);
    exp = GetMonData(&gParties[B_TRAINER_PLAYER][1], MON_DATA_EXP) + 50;  // XP gagnée pendant le combat
    SetMonData(&gParties[B_TRAINER_PLAYER][1], MON_DATA_EXP, &exp);
    Club_PretRendre();
    EXPECT_EQ(CalculatePlayerPartyCount(), 2);
    EXPECT_EQ(GetMonData(&gParties[B_TRAINER_PLAYER][1], MON_DATA_EXP), exp);
    EXPECT(Club_Signer(2));                      // tier B : 500 ¥ par Pokémon
    EXPECT_EQ(Club_PretCout(3), 1500);
}
