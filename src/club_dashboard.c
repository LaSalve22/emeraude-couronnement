#include "global.h"
#include "league_club.h"
#include "bg.h"
#include "event_data.h"
#include "main.h"
#include "menu.h"
#include "palette.h"
#include "rtc.h"
#include "script.h"
#include "sound.h"
#include "sprite.h"
#include "string_util.h"
#include "task.h"
#include "text.h"
#include "window.h"
#include "constants/characters.h"
#include "constants/rgb.h"
#include "constants/songs.h"

// Émeraude Couronnement : tableau de bord de la Ligue des Clubs (v0.42, mode licencié : 3 onglets).
// Fenêtre posée sur le terrain (comme le graphique de la Bourse) : L / R changent d'onglet, B referme.
// Classement : SELECT passe de la poule A à la poule B. Feuille de match : gauche / droite = journée, haut / bas = match.
// Effectif : haut / bas = club.

#define DASH_PAL      13
#define DASH_LARGEUR  30
#define DASH_HAUTEUR  13          // 8 + 30 x 13 tuiles : s'arrête avant la boîte de message
#define DASH_BASE     8

enum
{
    COUL_TRANSPARENT,
    COUL_FOND,
    COUL_TEXTE,
    COUL_OMBRE,
    COUL_CLUB,      // bandeau aux couleurs du club (rechargée à chaque dessin)
    COUL_OR,        // le club du joueur
    COUL_GRILLE,
    COUL_GRIS,
    COUL_VERT,
    COUL_ROUGE,
    COUL_LIGNE,     // ligne sélectionnée
};

static const u16 sPalette[16] =
{
    RGB(0, 0, 0), RGB(2, 3, 6), RGB(31, 31, 31), RGB(8, 9, 12),
    RGB(6, 14, 26), RGB(31, 25, 6), RGB(7, 8, 12), RGB(17, 18, 21),
    RGB(8, 27, 10), RGB(30, 9, 7), RGB(5, 6, 11),
};

// Fond transparent (0) : le texte ne repeint pas le fond, les lignes peuvent se serrer à 11 px (police de 12 px).
static const u8 sTexte[] = {TEXT_COLOR_TRANSPARENT, COUL_TEXTE, COUL_OMBRE};
static const u8 sTexteOr[] = {TEXT_COLOR_TRANSPARENT, COUL_OR, COUL_OMBRE};
static const u8 sTexteGris[] = {TEXT_COLOR_TRANSPARENT, COUL_GRIS, COUL_OMBRE};
static const u8 sTexteVert[] = {TEXT_COLOR_TRANSPARENT, COUL_VERT, COUL_OMBRE};
static const u8 sTexteRouge[] = {TEXT_COLOR_TRANSPARENT, COUL_ROUGE, COUL_OMBRE};
static const u8 sTexteBandeau[] = {TEXT_COLOR_TRANSPARENT, COUL_TEXTE, COUL_OMBRE};
static const u8 sTexteBandeauGris[] = {TEXT_COLOR_TRANSPARENT, COUL_GRIS, COUL_OMBRE};

#define LIGNE 11

// Licencié : 3 onglets ; propriétaire : 5 (Mercato et Finances en plus).
enum { ONGLET_CLASSEMENT, ONGLET_MATCH, ONGLET_EFFECTIF, ONGLET_MERCATO, ONGLET_FINANCES, NB_ONGLETS };

static const u8 *const sOnglets[NB_ONGLETS] =
{
    COMPOUND_STRING("Classement"), COMPOUND_STRING("Match"), COMPOUND_STRING("Effectif"),
    COMPOUND_STRING("Mercato"), COMPOUND_STRING("Finances"),
};

static u32 NbOnglets(void)
{
    return Club_Proprio() != CLUB_AUCUN ? NB_ONGLETS : ONGLET_MERCATO;
}

enum { T_FENETRE, T_ONGLET, T_POULE, T_JOURNEE, T_MATCH, T_CLUB };

static EWRAM_DATA u16 sPaletteSauvee[16] = {0};

static void Texte(u8 fen, const u8 *couleurs, u32 x, u32 y, const u8 *txt)
{
    AddTextPrinterParameterized3(fen, FONT_SMALL, x, y, couleurs, TEXT_SKIP_DRAW, txt);
}

static void TexteDroite(u8 fen, const u8 *couleurs, u32 xDroite, u32 y, const u8 *txt)
{
    u32 l = GetStringWidth(FONT_SMALL, txt, 0);
    Texte(fen, couleurs, xDroite > l ? xDroite - l : 0, y, txt);
}

static u8 *Nombre(u8 *fin, s32 n, bool32 signe)
{
    u32 v = n < 0 ? -n : n, chiffres = 1, p = 10;
    if (n < 0)
        *fin++ = CHAR_HYPHEN;
    else if (signe && n > 0)
        *fin++ = CHAR_PLUS;
    while (v >= p && chiffres < 9)
    {
        chiffres++;
        p *= 10;
    }
    return ConvertIntToDecimalStringN(fin, v, STR_CONV_MODE_LEFT_ALIGN, chiffres);
}

static void ChargerCouleurClub(u32 club)
{
    u16 c = club < CLUB_NB ? Club_CouleurClub(club) : RGB(6, 14, 26);
    LoadPalette(&c, BG_PLTT_ID(DASH_PAL) + COUL_CLUB, PLTT_SIZEOF(1));
}

// Blasons d'Adrien (v0.45) : sprites 16 x 16 (dessin de 12 px de haut, une palette chacun), tirés de sa planche par
// tools/couronnement/blasons_gfx.py. S'il n'y a plus de palette de sprite libre sur le terrain, on dessine l'écu ci-dessous.
static const u32 sBlasonGfx0[] = INCBIN_U32("graphics/couronnement/blasons/blason_petit_0.4bpp");
static const u16 sBlasonPal0[] = INCBIN_U16("graphics/couronnement/blasons/blason_petit_0.gbapal");
static const u32 sBlasonGfx1[] = INCBIN_U32("graphics/couronnement/blasons/blason_petit_1.4bpp");
static const u16 sBlasonPal1[] = INCBIN_U16("graphics/couronnement/blasons/blason_petit_1.gbapal");
static const u32 sBlasonGfx2[] = INCBIN_U32("graphics/couronnement/blasons/blason_petit_2.4bpp");
static const u16 sBlasonPal2[] = INCBIN_U16("graphics/couronnement/blasons/blason_petit_2.gbapal");
static const u32 sBlasonGfx3[] = INCBIN_U32("graphics/couronnement/blasons/blason_petit_3.4bpp");
static const u16 sBlasonPal3[] = INCBIN_U16("graphics/couronnement/blasons/blason_petit_3.gbapal");
static const u32 sBlasonGfx4[] = INCBIN_U32("graphics/couronnement/blasons/blason_petit_4.4bpp");
static const u16 sBlasonPal4[] = INCBIN_U16("graphics/couronnement/blasons/blason_petit_4.gbapal");
static const u32 sBlasonGfx5[] = INCBIN_U32("graphics/couronnement/blasons/blason_petit_5.4bpp");
static const u16 sBlasonPal5[] = INCBIN_U16("graphics/couronnement/blasons/blason_petit_5.gbapal");
static const u32 sBlasonGfx6[] = INCBIN_U32("graphics/couronnement/blasons/blason_petit_6.4bpp");
static const u16 sBlasonPal6[] = INCBIN_U16("graphics/couronnement/blasons/blason_petit_6.gbapal");
static const u32 sBlasonGfx7[] = INCBIN_U32("graphics/couronnement/blasons/blason_petit_7.4bpp");
static const u16 sBlasonPal7[] = INCBIN_U16("graphics/couronnement/blasons/blason_petit_7.gbapal");
static const u32 sBlasonGfx8[] = INCBIN_U32("graphics/couronnement/blasons/blason_petit_8.4bpp");
static const u16 sBlasonPal8[] = INCBIN_U16("graphics/couronnement/blasons/blason_petit_8.gbapal");
static const u32 sBlasonGfx9[] = INCBIN_U32("graphics/couronnement/blasons/blason_petit_9.4bpp");
static const u16 sBlasonPal9[] = INCBIN_U16("graphics/couronnement/blasons/blason_petit_9.gbapal");
static const u32 *const sBlasonsGfx[CLUB_NB] = {sBlasonGfx0, sBlasonGfx1, sBlasonGfx2, sBlasonGfx3, sBlasonGfx4, sBlasonGfx5, sBlasonGfx6, sBlasonGfx7, sBlasonGfx8, sBlasonGfx9};
static const u16 *const sBlasonsPal[CLUB_NB] = {sBlasonPal0, sBlasonPal1, sBlasonPal2, sBlasonPal3, sBlasonPal4, sBlasonPal5, sBlasonPal6, sBlasonPal7, sBlasonPal8, sBlasonPal9};

#define TAG_BLASON      0x5C20
#define MAX_BLASONS     6
static EWRAM_DATA u8 sBlasons[MAX_BLASONS] = {0};
static EWRAM_DATA u8 sNbBlasons = 0;
static EWRAM_DATA bool8 sAvecSprites = FALSE;

static const struct OamData sOamBlason =
{
    .shape = SPRITE_SHAPE(16x16),
    .size = SPRITE_SIZE(16x16),
    .priority = 0,
};

static void EffacerBlasons(void)
{
    u32 i;
    for (i = 0; i < sNbBlasons; i++)
    {
        u16 tag = gSprites[sBlasons[i]].template->tileTag;
        DestroySprite(&gSprites[sBlasons[i]]);
        FreeSpriteTilesByTag(tag);
        FreeSpritePaletteByTag(tag);
    }
    sNbBlasons = 0;
}

// (x, y) = coin haut gauche de la toile 16 x 16 dans la fenêtre (qui commence en haut à gauche de l'écran).
static bool32 BlasonSprite(s32 x, s32 y, u32 club)
{
    u16 tag = TAG_BLASON + club;
    struct SpriteSheet feuille = {sBlasonsGfx[club], 16 * 16 / 2, tag};
    struct SpritePalette palette = {sBlasonsPal[club], tag};
    struct SpriteTemplate modele =
    {
        .tileTag = tag, .paletteTag = tag, .oam = &sOamBlason, .anims = gDummySpriteAnimTable,
        .images = NULL, .affineAnims = gDummySpriteAffineAnimTable, .callback = SpriteCallbackDummy,
    };
    u8 id;

    if (!sAvecSprites || sNbBlasons >= MAX_BLASONS || club >= CLUB_NB)
        return FALSE;
    if (LoadSpritePalette(&palette) == 0xFF)
        return FALSE;
    LoadSpriteSheet(&feuille);
    id = CreateSprite(&modele, x + 8, y + 8, 0);
    if (id == MAX_SPRITES)
    {
        FreeSpriteTilesByTag(tag);
        FreeSpritePaletteByTag(tag);
        return FALSE;
    }
    sBlasons[sNbBlasons++] = id;
    return TRUE;
}

// Repli : écu 9 x 10 aux couleurs du club (couleur chargée dans l'entrée 11 à 15 de la palette), liseré doré pour les tiers A et S.
#define COUL_BLASON 11
static void Blason(u8 fen, u32 x, u32 y, u32 club, u32 entree)
{
    static const u8 sLargeurs[10] = {9, 9, 9, 9, 9, 9, 7, 5, 3, 1};
    u16 c = Club_CouleurClub(club);
    u32 i;

    if (BlasonSprite((s32)x - 4, (s32)y - 3, club)) // le dessin (env. 10 x 12) se cale sur l'écu de repli
        return;
    LoadPalette(&c, BG_PLTT_ID(DASH_PAL) + COUL_BLASON + entree, PLTT_SIZEOF(1));
    for (i = 0; i < 10; i++)
        FillWindowPixelRect(fen, COUL_BLASON + entree, x + (9 - sLargeurs[i]) / 2, y + i, sLargeurs[i], 1);
    FillWindowPixelRect(fen, COUL_TEXTE, x + 4, y + 1, 1, 6);          // pal blanc
    if (Club_Tier(club) >= CLUB_TIER_A)
        FillWindowPixelRect(fen, COUL_OR, x, y, 9, 1);
}

static const u8 sText_Saison[] = _("S");
static const u8 sText_Jour[] = _(" J");
#if EDITION_TYCOON_SIM
static const u8 sText_Sur30[] = _("/270");
#else
static const u8 sText_Sur30[] = _("/30");
#endif

static void Bandeau(u8 fen, u32 onglet, u32 clubCouleur)
{
    u8 txt[24], *fin;
    u32 i, x = 4;

    ChargerCouleurClub(clubCouleur);
    FillWindowPixelRect(fen, COUL_CLUB, 0, 0, DASH_LARGEUR * 8, 13);
    for (i = 0; i < NbOnglets(); i++)
    {
        Texte(fen, i == onglet ? sTexteBandeau : sTexteBandeauGris, x, 1, sOnglets[i]);
        if (i == onglet)
            FillWindowPixelRect(fen, COUL_OR, x, 11, GetStringWidth(FONT_SMALL, sOnglets[i], 0), 1);
        x += GetStringWidth(FONT_SMALL, sOnglets[i], 0) + 10;
    }
    if (NbOnglets() == NB_ONGLETS)
        return; // pas la place pour la saison avec 5 onglets
    fin = StringCopy(txt, sText_Saison);
    fin = Nombre(fin, Club_Saison() + 1, FALSE);
    fin = StringCopy(fin, sText_Jour);
    fin = Nombre(fin, Club_JourDeSaison(), FALSE);
    StringCopy(fin, sText_Sur30);
    TexteDroite(fen, sTexteBandeau, 236, 1, txt);
}

// ---------------------------------------------------------------- onglet Classement

static const u8 sText_Poule[] = _("Poule ");
static const u8 sText_ColJ[] = _("J");
static const u8 sText_ColPts[] = _("Pts");
static const u8 sText_ColDiff[] = _("Diff");
static const u8 sText_ColBO[] = _("BO");
#if CLUB_NPHL
static const u8 sText_AideClassement[] = _("Les 4 premiers vont en play-offs.");
static const u8 sText_Nphl[] = _("NPHL, places ");
static const u8 sText_Places1[] = _("1 à 5");
static const u8 sText_Places2[] = _("6 à 10");
static const u8 sText_AideSelectNphl[] = _("SELECT : places ");
static const u8 sText_Demi[] = _("Demi-finale ");
static const u8 sText_JourDemis[] = _(" (jour 255)");
#elif EDITION_TYCOON_SIM
static const u8 sText_AideClassement[] = _("Le 1er joue la finale du jour 270.");
#else
static const u8 sText_AideClassement[] = _("Le 1er joue la finale du jour 30.");
#endif
static const u8 sText_AideSelect[] = _("SELECT : poule ");

static void DessinerClassement(u8 fen, u32 poule)
{
    u8 ordre[CLUB_PAR_POULE], txt[24], *fin;
    u32 i, joueur = Club_DuJoueur();

    Club_Classement(poule, ordre);
#if CLUB_NPHL
    fin = StringCopy(txt, sText_Nphl);
    StringCopy(fin, (poule & 1) ? sText_Places2 : sText_Places1);
#else
    fin = StringCopy(txt, sText_Poule);
    *fin++ = poule ? CHAR_B : CHAR_A;
    *fin = EOS;
#endif
    Texte(fen, sTexteOr, 4, 15, txt);
    TexteDroite(fen, sTexteGris, 150, 15, sText_ColJ);
    TexteDroite(fen, sTexteGris, 178, 15, sText_ColPts);
    TexteDroite(fen, sTexteGris, 208, 15, sText_ColDiff);
    TexteDroite(fen, sTexteGris, 234, 15, sText_ColBO);
    FillWindowPixelRect(fen, COUL_GRILLE, 4, 26, 232, 1);
    for (i = 0; i < 5; i++) // 5 lignes par page (NPHL : places 1-5 puis 6-10)
    {
        u32 rang = CLUB_NPHL ? (poule & 1) * 5 + i : i;
        u32 c = ordre[rang], y = 28 + i * LIGNE;
        const u8 *coul = (c == joueur) ? sTexteOr : sTexte;
        if (c == joueur)
            FillWindowPixelRect(fen, COUL_LIGNE, 2, y, 236, LIGNE);
        Nombre(txt, rang + 1, FALSE);
        Texte(fen, rang < (CLUB_NPHL ? 4 : 1) ? sTexteOr : sTexteGris, 6, y, txt);
        Blason(fen, 16, y + 1, c, i);
        Texte(fen, coul, 29, y, Club_Nom(c));
        Nombre(txt, Club_MatchsJoues(c), FALSE);
        TexteDroite(fen, coul, 150, y, txt);
        Nombre(txt, Club_Points(c), FALSE);
        TexteDroite(fen, coul, 178, y, txt);
        Nombre(txt, Club_DiffKO(c), TRUE);
        TexteDroite(fen, coul, 208, y, txt);
        Nombre(txt, Club_BonusOffensifs(c), FALSE);
        TexteDroite(fen, coul, 234, y, txt);
    }
    Texte(fen, sTexteGris, 4, 28 + 5 * LIGNE + 1, sText_AideClassement);
#if CLUB_NPHL
    fin = StringCopy(txt, sText_AideSelectNphl);
    StringCopy(fin, (poule & 1) ? sText_Places1 : sText_Places2);
#else
    fin = StringCopy(txt, sText_AideSelect);
    *fin++ = poule ? CHAR_A : CHAR_B;
    *fin = EOS;
#endif
    TexteDroite(fen, sTexteGris, 236, 28 + 5 * LIGNE + 1, txt);
}

// ---------------------------------------------------------------- onglet Feuille de match

static const u8 sText_Journee[] = _("Journée ");
static const u8 sText_JourParen[] = _(" (jour ");
#if EDITION_TYCOON_SIM
static const u8 sText_Finale[] = _("Grande Finale (jour 270)");
#else
static const u8 sText_Finale[] = _("Grande Finale (jour 30)");
#endif
static const u8 sText_Provisoire[] = _(" provisoire");
static const u8 sText_Paren[] = _(")");
static const u8 sText_MatchN[] = _("  match ");
static const u8 sText_Slash[] = _("/");
static const u8 sText_Tiret[] = _("-");
static const u8 sText_PasJoue[] = _("à jouer");
static const u8 sText_Attente[] = _("à toi");
static const u8 sText_Nul[] = _("nul");
static const u8 sText_Exempts[] = _("Exempts : ");
static const u8 sText_Et[] = _(", ");
static const u8 sText_TourDeCombat[] = _("Tour de Combat : terrain neutre");
static const u8 sText_AideMatch[] = _("{LEFT_ARROW}{RIGHT_ARROW} jour. {UP_ARROW}{DOWN_ARROW} match");

#define X_DOM   96      // noms à domicile alignés à droite
#define X_SCORE 119     // score centré
#define X_EXT   142     // noms à l'extérieur

static u32 NbMatchsJournee(u32 journee)
{
    return journee >= CLUB_NB_JOURNEES ? CLUB_NB_PLAYOFFS : CLUB_MATCHS_PAR_JOURNEE;
}

static u32 IndexMatch(u32 journee, u32 k)
{
    return journee >= CLUB_NB_JOURNEES ? CLUB_NB_MATCHS + (k % CLUB_NB_PLAYOFFS) : journee * CLUB_MATCHS_PAR_JOURNEE + k;
}

static void Centre(u8 fen, const u8 *couleurs, u32 x, u32 y, const u8 *txt)
{
    u32 l = GetStringWidth(FONT_SMALL, txt, 0);
    Texte(fen, couleurs, x > l / 2 ? x - l / 2 : 0, y, txt);
}

static void DessinerMatch(u8 fen, u32 journee, u32 selection)
{
    u8 txt[48], *fin, dom, ext;
    u32 k, m = IndexMatch(journee, selection), joueur = Club_DuJoueur(), pd = 0, pe = 0;
    bool32 joue = FALSE;

    if (journee >= CLUB_NB_JOURNEES && m != CLUB_MATCH_FINALE)
    {
#if CLUB_NPHL
        fin = StringCopy(txt, sText_Demi);
        fin = Nombre(fin, selection + 1, FALSE);
        fin = StringCopy(fin, sText_JourDemis);
        if (Club_JourDeSaison() < CLUB_JOUR_DEMIS)
            StringCopy(fin, sText_Provisoire);
#endif
    }
    else if (journee >= CLUB_NB_JOURNEES)
    {
        fin = StringCopy(txt, sText_Finale);
        if (Club_JourDeSaison() < CLUB_JOUR_FINALE)
            StringCopy(fin, sText_Provisoire);
    }
    else
    {
        fin = StringCopy(txt, sText_Journee);
        fin = Nombre(fin, journee + 1, FALSE);
        fin = StringCopy(fin, sText_JourParen);
        fin = Nombre(fin, Club_JourDeJournee(journee), FALSE);
        fin = StringCopy(fin, sText_Paren);
        fin = StringCopy(fin, sText_MatchN);
        fin = Nombre(fin, selection + 1, FALSE);
        fin = StringCopy(fin, sText_Slash);
        Nombre(fin, CLUB_MATCHS_PAR_JOURNEE, FALSE);
    }
    Texte(fen, sTexteOr, 4, 15, txt);
    TexteDroite(fen, sTexteGris, 236, 15, sText_AideMatch);

    // Les deux clubs et le score de la rencontre
    Club_Affiche(m, &dom, &ext);
    for (k = 0; k < CLUB_DRESSEURS; k++)
    {
        u32 code = Club_Duel(m, k);
        if (code != CLUB_DUEL_PAS_JOUE)
            joue = TRUE;
        pd += Club_PointsDuel(code, TRUE);
        pe += Club_PointsDuel(code, FALSE);
    }
    FillWindowPixelRect(fen, COUL_LIGNE, 2, 26, 236, LIGNE + 1);
    TexteDroite(fen, dom == joueur ? sTexteOr : sTexte, X_DOM - 12, 26, Club_NomCourt(dom));
    Blason(fen, X_DOM - 9, 27, dom, 0);
    Blason(fen, X_EXT, 27, ext, 1);
    Texte(fen, ext == joueur ? sTexteOr : sTexte, X_EXT + 12, 26, Club_NomCourt(ext));
    if (joue)
    {
        fin = Nombre(txt, pd, FALSE);
        fin = StringCopy(fin, sText_Tiret);
        Nombre(fin, pe, FALSE);
    }
    else
    {
        StringCopy(txt, sText_Tiret);
    }
    Centre(fen, pd > pe ? sTexteVert : (pe > pd ? sTexteRouge : sTexte), X_SCORE, 26, txt);

    // Les 4 duels
    for (k = 0; k < CLUB_DRESSEURS; k++)
    {
        u32 y = 39 + k * LIGNE, code = Club_Duel(m, k);
        bool32 attente = (code == CLUB_DUEL_PAS_JOUE && k == CLUB_SLOT_JOUEUR && Club_MatchDuJoueur() == m);
        Texte(fen, sTexteGris, 4, y, Club_NomSlot(k));
        TexteDroite(fen, (dom == joueur && k == CLUB_SLOT_JOUEUR) ? sTexteOr : sTexte, X_DOM, y, Club_NomDresseur(dom, k));
        Texte(fen, (ext == joueur && k == CLUB_SLOT_JOUEUR) ? sTexteOr : sTexte, X_EXT, y, Club_NomDresseur(ext, k));
        if (code == CLUB_DUEL_PAS_JOUE)
        {
            Centre(fen, attente ? sTexteOr : sTexteGris, X_SCORE, y, attente ? sText_Attente : sText_PasJoue);
            continue;
        }
        if (code == CLUB_DUEL_NUL)
        {
            StringCopy(txt, sText_Nul);
        }
        else
        {
            u32 sd = code <= 6 ? code : 0, se = code > 6 ? code - 6 : 0;
            fin = Nombre(txt, sd, FALSE);
            fin = StringCopy(fin, sText_Tiret);
            Nombre(fin, se, FALSE);
        }
        Centre(fen, code <= 6 ? sTexteVert : (code == CLUB_DUEL_NUL ? sTexte : sTexteRouge), X_SCORE, y, txt);
        fin = Nombre(txt, Club_PointsDuel(code, TRUE), FALSE);
        fin = StringCopy(fin, sText_Tiret);
        Nombre(fin, Club_PointsDuel(code, FALSE), FALSE);
        TexteDroite(fen, sTexteGris, 236, y, txt);
    }

    // Pied : stade (effet) ou exempts de la journée
    if (m == CLUB_MATCH_FINALE)
    {
        Texte(fen, sTexteGris, 4, 39 + CLUB_DRESSEURS * LIGNE + 1, sText_TourDeCombat);
    }
    else if (CLUB_NPHL) // une seule poule de 10 : personne n'est exempt
    {
        Texte(fen, sTexteGris, 4, 39 + CLUB_DRESSEURS * LIGNE + 1, Club_NomEffet(Club_Stade(dom)));
    }
    else
    {
        u8 clubs[CLUB_NB];
        u32 c, j, n = 0;
        Texte(fen, sTexteGris, 4, 39 + CLUB_DRESSEURS * LIGNE + 1, Club_NomEffet(Club_Stade(dom)));
        fin = StringCopy(txt, sText_Exempts);
        Club_Tirage(Club_Saison(), clubs);
        for (c = 0; c < CLUB_NB; c++)
        {
            bool32 present = FALSE;
            for (j = 0; j < CLUB_MATCHS_PAR_JOURNEE; j++)
            {
                u8 d, e;
                Club_Affiche(IndexMatch(journee, j), &d, &e);
                if (d == clubs[c] || e == clubs[c])
                    present = TRUE;
            }
            if (!present)
            {
                if (n++)
                    fin = StringCopy(fin, sText_Et);
                fin = StringCopy(fin, Club_NomCourt(clubs[c]));
            }
        }
        TexteDroite(fen, sTexteGris, 236, 39 + CLUB_DRESSEURS * LIGNE + 1, txt);
    }
}

// ---------------------------------------------------------------- onglet Effectif

static const u8 sText_Tier[] = _("  tier ");
static const u8 sText_Tiers[] = _("CBAS");
static const u8 sText_Effet[] = _(" : ");
static const u8 sText_ColDresseur[] = _("Dresseur");
static const u8 sText_ColRating[] = _("Rating");
static const u8 sText_ColBilan[] = _("V-N-D");
static const u8 sText_Toi[] = _("toi");
static const u8 sText_AideEffectif[] = _("{UP_ARROW}{DOWN_ARROW} club");
static const u8 sText_PtsClub[] = _(" pts, ");
static const u8 sText_NphlCourt[] = _("NPHL");
static const u8 sText_TitresJoueur[] = _("Tes titres : ");

static void DessinerEffectif(u8 fen, u32 club)
{
    u8 txt[48], *fin;
    u32 s, joueur = Club_DuJoueur();

    fin = StringCopy(txt, Club_Nom(club));
    fin = StringCopy(fin, sText_Tier);
    *fin++ = sText_Tiers[Club_Tier(club)];
    *fin = EOS;
    Blason(fen, 4, 16, club, 0);
    Texte(fen, club == joueur ? sTexteOr : sTexte, 16, 15, txt);
    TexteDroite(fen, sTexteGris, 236, 15, sText_AideEffectif);
    fin = StringCopy(txt, Club_NomStade(club));
    fin = StringCopy(fin, sText_Effet);
    StringCopy(fin, Club_NomEffet(Club_Stade(club)));
    Texte(fen, sTexteGris, 16, 26, txt);

    Texte(fen, sTexteGris, 4, 37, sText_ColDresseur);
    TexteDroite(fen, sTexteGris, 176, 37, sText_ColRating);
    TexteDroite(fen, sTexteGris, 234, 37, sText_ColBilan);
    FillWindowPixelRect(fen, COUL_GRILLE, 4, 48, 232, 1);
    for (s = 0; s < CLUB_DRESSEURS; s++)
    {
        u32 y = 49 + s * LIGNE, v = 0, n = 0, d = 0, m;
        bool32 toi = (club == joueur && s == CLUB_SLOT_JOUEUR);
        const u8 *coul = toi ? sTexteOr : sTexte;

        if (toi)
            FillWindowPixelRect(fen, COUL_LIGNE, 2, y, 236, LIGNE);
        Texte(fen, sTexteGris, 4, y, Club_NomSlot(s));
        Texte(fen, coul, 56, y, Club_NomDresseur(club, s));
        if (toi)
        {
            TexteDroite(fen, coul, 176, y, sText_Toi);
        }
        else
        {
            Nombre(txt, Club_Rating(club, s), FALSE);
            TexteDroite(fen, coul, 176, y, txt);
        }
        for (m = 0; m <= CLUB_MATCH_FINALE; m++)
        {
            u8 dom, ext;
            u32 code = Club_Duel(m, s);
            if (code == CLUB_DUEL_PAS_JOUE)
                continue;
            Club_Affiche(m, &dom, &ext);
            if (dom != club && ext != club)
                continue;
            if (code == CLUB_DUEL_NUL)
                n++;
            else if ((code <= 6) == (dom == club))
                v++;
            else
                d++;
        }
        fin = Nombre(txt, v, FALSE);
        fin = StringCopy(fin, sText_Tiret);
        fin = Nombre(fin, n, FALSE);
        fin = StringCopy(fin, sText_Tiret);
        Nombre(fin, d, FALSE);
        TexteDroite(fen, coul, 234, y, txt);
    }
    fin = Nombre(txt, Club_Points(club), FALSE);
    fin = StringCopy(fin, sText_PtsClub);
    if (CLUB_NPHL)
        fin = StringCopy(fin, sText_NphlCourt);
    else
    {
        fin = StringCopy(fin, sText_Poule);
        *fin++ = Club_Poule(club) ? CHAR_B : CHAR_A;
    }
    *fin = EOS;
    Texte(fen, sTexteGris, 4, 49 + CLUB_DRESSEURS * LIGNE, txt);
    if (joueur != CLUB_AUCUN)
    {
        fin = StringCopy(txt, sText_TitresJoueur);
        Nombre(fin, Club_Titres(), FALSE);
        TexteDroite(fen, sTexteGris, 236, 49 + CLUB_DRESSEURS * LIGNE, txt);
    }
}

// ---------------------------------------------------------------- tâche

// ---------------------------------------------------------------- onglet Mercato (propriétaire)

static const u8 sText_ColSalaire[] = _("Salaire/saison");
static const u8 sText_Origine[] = _("origine");
static const u8 sText_Recrue[] = _("recrue");
static const u8 sText_Coach[] = _(" +");
static const u8 sText_Vivier[] = _("Vivier : ");
static const u8 sText_Dispo[] = _(" dresseurs battus à recruter.");
static const u8 sText_YenK[] = _(" ¥");

static void DessinerMercato(u8 fen)
{
    u8 txt[40], *fin;
    u32 club = Club_Proprio(), s, dispo = 0, i;

    Blason(fen, 4, 16, club, 0);
    Texte(fen, sTexteOr, 16, 15, Club_Nom(club));
    TexteDroite(fen, sTexteGris, 236, 15, sText_ColSalaire);
    FillWindowPixelRect(fen, COUL_GRILLE, 4, 26, 232, 1);
    for (s = 0; s < CLUB_DRESSEURS; s++)
    {
        u32 y = 28 + s * LIGNE;
        bool32 toi = (s == CLUB_SLOT_JOUEUR);
        Texte(fen, sTexteGris, 4, y, Club_NomSlot(s));
        Texte(fen, toi ? sTexteOr : sTexte, 56, y, Club_NomDresseur(club, s));
        if (toi)
        {
            TexteDroite(fen, sTexteOr, 236, y, sText_Toi);
            continue;
        }
        fin = Nombre(txt, Club_Rating(club, s), FALSE);
        if (gSaveBlock3Ptr->ligueClubs.coaching[s])
        {
            fin = StringCopy(fin, sText_Coach);
            Nombre(fin, gSaveBlock3Ptr->ligueClubs.coaching[s], FALSE);
        }
        Texte(fen, sTexte, 112, y, txt);
        Texte(fen, sTexteGris, 150, y, gSaveBlock3Ptr->ligueClubs.effectif[s] ? sText_Recrue : sText_Origine);
        fin = Nombre(txt, Club_Salaire(club, s), FALSE);
        StringCopy(fin, sText_YenK);
        TexteDroite(fen, sTexte, 236, y, txt);
    }
    for (i = 0; i < CLUB_NB_RECRUES; i++)
        dispo += Club_RecrueDisponible(i);
    fin = StringCopy(txt, sText_Vivier);
    fin = Nombre(fin, dispo, FALSE);
    StringCopy(fin, sText_Dispo);
    Texte(fen, sTexteGris, 4, 28 + CLUB_DRESSEURS * LIGNE + 1, txt);
    Texte(fen, sTexteGris, 4, 28 + (CLUB_DRESSEURS + 1) * LIGNE + 1, COMPOUND_STRING("Recruter et coacher : menu Propriété."));
}

// ---------------------------------------------------------------- onglet Finances (propriétaire)

static const u8 *const sLignesBilan[6] =
{
    COMPOUND_STRING("Billetterie (4 matchs)"), COMPOUND_STRING("Droits TV et sponsors"), COMPOUND_STRING("Boutique"),
    COMPOUND_STRING("Salaires et entretien"), COMPOUND_STRING("Impôt (20 %)"), COMPOUND_STRING("Résultat net prévu"),
};
static const u8 sText_Places[] = _(" places");
static const u8 sText_Start[] = _("START : agrandir (");
static const u8 sText_StartMax[] = _("Stade au niveau maximum.");
static const u8 sText_Dernier[] = _("Saison passée ");

static void DessinerFinances(u8 fen)
{
    u32 lignes[6], club = Club_Proprio(), i;
    u8 txt[40], *fin;

    Club_Previsionnel(Club_Points(club), FALSE, lignes);
    fin = StringCopy(txt, Club_NomNiveauStade());
    *fin++ = CHAR_COMMA;
    *fin++ = CHAR_SPACE;
    fin = Nombre(fin, Club_Places(), FALSE);
    StringCopy(fin, sText_Places);
    Texte(fen, sTexteOr, 4, 15, txt);
    fin = StringCopy(txt, sText_Dernier);
    fin = Nombre(fin, (s32)gSaveBlock3Ptr->ligueClubs.bilan[CLUB_BILAN_NET], TRUE);
    StringCopy(fin, sText_YenK);
    TexteDroite(fen, sTexteGris, 236, 15, txt);
    FillWindowPixelRect(fen, COUL_GRILLE, 4, 27, 232, 1);
    for (i = 0; i < 6; i++)
    {
        u32 y = 29 + i * 10;
        bool32 depense = (i == CLUB_BILAN_CHARGES || i == CLUB_BILAN_IMPOT);
        const u8 *coul = i == CLUB_BILAN_NET ? ((s32)lignes[i] >= 0 ? sTexteVert : sTexteRouge) : (depense ? sTexteRouge : sTexte);
        Texte(fen, i == CLUB_BILAN_NET ? sTexteOr : sTexteGris, 4, y, sLignesBilan[i]);
        if (i == CLUB_BILAN_NET)
            fin = Nombre(txt, (s32)lignes[i], TRUE);
        else
            fin = Nombre(txt, depense ? -(s32)lignes[i] : (s32)lignes[i], depense ? FALSE : TRUE);
        StringCopy(fin, sText_YenK);
        TexteDroite(fen, coul, 236, y, txt);
    }
    if (Club_CoutTravaux())
    {
        fin = StringCopy(txt, sText_Start);
        fin = Nombre(fin, Club_CoutTravaux(), FALSE);
        fin = StringCopy(fin, sText_YenK);
        *fin++ = CHAR_RIGHT_PAREN;
        *fin = EOS;
        Texte(fen, sTexteGris, 4, 92, txt);
    }
    else
    {
        Texte(fen, sTexteGris, 4, 92, sText_StartMax);
    }
}

// Dessine un onglet dans la fenêtre (aussi utilisé par le test d'aperçu, TESTS="Clubs").
void Club_TableauDessiner(u8 fen, u32 onglet, u32 poule, u32 journee, u32 match, u32 club)
{
    u32 joueur = Club_DuJoueur();

    EffacerBlasons();
    FillWindowPixelBuffer(fen, PIXEL_FILL(COUL_FOND));
    switch (onglet)
    {
    case ONGLET_CLASSEMENT:
        Bandeau(fen, onglet, joueur);
        DessinerClassement(fen, poule);
        break;
    case ONGLET_MATCH:
        Bandeau(fen, onglet, joueur);
        DessinerMatch(fen, journee, match);
        break;
    case ONGLET_MERCATO:
        Bandeau(fen, onglet, Club_Proprio());
        DessinerMercato(fen);
        break;
    case ONGLET_FINANCES:
        Bandeau(fen, onglet, Club_Proprio());
        DessinerFinances(fen);
        break;
    default:
        Bandeau(fen, onglet, club);
        DessinerEffectif(fen, club);
        break;
    }
    PutWindowTilemap(fen);
    CopyWindowToVram(fen, COPYWIN_FULL);
}

static void Dessiner(u8 taskId)
{
    s16 *data = gTasks[taskId].data;
    Club_TableauDessiner(data[T_FENETRE], data[T_ONGLET], data[T_POULE], data[T_JOURNEE], data[T_MATCH], data[T_CLUB]);
}

static void Task_TableauDeBord(u8 taskId)
{
    s16 *data = gTasks[taskId].data;
    bool32 redessiner = TRUE;

    bool32 agrandir = (data[T_ONGLET] == ONGLET_FINANCES && JOY_NEW(START_BUTTON) && Club_CoutTravaux());

    if (JOY_NEW(B_BUTTON) || agrandir)
    {
        u8 fen = data[T_FENETRE];
        gSpecialVar_Result = agrandir; // 1 : le script enchaîne sur le devis des travaux
        PlaySE(SE_SELECT);
        ClearWindowTilemap(fen);
        CopyWindowToVram(fen, COPYWIN_MAP);
        RemoveWindow(fen);
        EffacerBlasons();
        sAvecSprites = FALSE;
        LoadPalette(sPaletteSauvee, BG_PLTT_ID(DASH_PAL), PLTT_SIZE_4BPP);
        DestroyTask(taskId);
        ScriptContext_Enable();
        return;
    }
    if (JOY_NEW(R_BUTTON))
        data[T_ONGLET] = (data[T_ONGLET] + 1) % NbOnglets();
    else if (JOY_NEW(L_BUTTON))
        data[T_ONGLET] = (data[T_ONGLET] + NbOnglets() - 1) % NbOnglets();
    else if (data[T_ONGLET] == ONGLET_CLASSEMENT && JOY_NEW(SELECT_BUTTON))
        data[T_POULE] ^= 1;
    else if (data[T_ONGLET] == ONGLET_MATCH && JOY_NEW(DPAD_RIGHT))
        data[T_JOURNEE] = (data[T_JOURNEE] + 1) % (CLUB_NB_JOURNEES + 1), data[T_MATCH] = 0;
    else if (data[T_ONGLET] == ONGLET_MATCH && JOY_NEW(DPAD_LEFT))
        data[T_JOURNEE] = (data[T_JOURNEE] + CLUB_NB_JOURNEES) % (CLUB_NB_JOURNEES + 1), data[T_MATCH] = 0;
    else if (data[T_ONGLET] == ONGLET_MATCH && JOY_NEW(DPAD_DOWN))
        data[T_MATCH] = (data[T_MATCH] + 1) % NbMatchsJournee(data[T_JOURNEE]);
    else if (data[T_ONGLET] == ONGLET_MATCH && JOY_NEW(DPAD_UP))
        data[T_MATCH] = (data[T_MATCH] + NbMatchsJournee(data[T_JOURNEE]) - 1) % NbMatchsJournee(data[T_JOURNEE]);
    else if (data[T_ONGLET] == ONGLET_EFFECTIF && JOY_NEW(DPAD_DOWN))
        data[T_CLUB] = (data[T_CLUB] + 1) % CLUB_NB;
    else if (data[T_ONGLET] == ONGLET_EFFECTIF && JOY_NEW(DPAD_UP))
        data[T_CLUB] = (data[T_CLUB] + CLUB_NB - 1) % CLUB_NB;
    else
        redessiner = FALSE;
    if (redessiner)
    {
        PlaySE(SE_SELECT);
        Dessiner(taskId);
    }
}

// Journée affichée à l'ouverture : celle du duel en attente du joueur, sinon la dernière jouée.
static void PositionDeDepart(s16 *data)
{
    u32 j, jour = Club_JourDeSaison(), m = Club_MatchDuJoueur(), joueur = Club_DuJoueur();

    data[T_JOURNEE] = 0;
    for (j = 0; j < CLUB_NB_JOURNEES; j++)
        if (Club_JourDeJournee(j) <= jour)
            data[T_JOURNEE] = j;
    if (jour >= (CLUB_NPHL ? CLUB_JOUR_DEMIS : 30))
        data[T_JOURNEE] = CLUB_NB_JOURNEES;
    data[T_MATCH] = 0;
    if (m != 0xFF)
    {
        data[T_JOURNEE] = m >= CLUB_NB_MATCHS ? CLUB_NB_JOURNEES : m / CLUB_MATCHS_PAR_JOURNEE;
        data[T_MATCH] = m >= CLUB_NB_MATCHS ? m - CLUB_NB_MATCHS : m % CLUB_MATCHS_PAR_JOURNEE;
    }
    data[T_CLUB] = joueur != CLUB_AUCUN ? joueur : 0;
    data[T_POULE] = joueur != CLUB_AUCUN ? Club_Poule(joueur) : 0;
}

void Club_TableauDeBord(void)
{
    static const struct WindowTemplate sFenetre =
    {
        .bg = 0, .tilemapLeft = 0, .tilemapTop = 0, .width = DASH_LARGEUR, .height = DASH_HAUTEUR,
        .paletteNum = DASH_PAL, .baseBlock = DASH_BASE,
    };
    u8 fen, taskId;

    Club_MettreAJourJusqua(RtcGetLocalDayCount());
    fen = AddWindow(&sFenetre);
    if (fen == WINDOW_NONE)
    {
        ScriptContext_Enable();
        return;
    }
    CpuCopy16(&gPlttBufferUnfaded[BG_PLTT_ID(DASH_PAL)], sPaletteSauvee, PLTT_SIZE_4BPP);
    LoadPalette(sPalette, BG_PLTT_ID(DASH_PAL), PLTT_SIZE_4BPP);
    sNbBlasons = 0;
    sAvecSprites = TRUE;
    taskId = CreateTask(Task_TableauDeBord, 80);
    gTasks[taskId].data[T_FENETRE] = fen;
    gTasks[taskId].data[T_ONGLET] = ONGLET_CLASSEMENT;
    PositionDeDepart(gTasks[taskId].data);
    Dessiner(taskId);
}
