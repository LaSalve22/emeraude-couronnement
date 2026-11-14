#include "global.h"
#include "bourse.h"
#include "bg.h"
#include "event_data.h"
#include "main.h"
#include "menu.h"
#include "palette.h"
#include "script.h"
#include "sound.h"
#include "string_util.h"
#include "task.h"
#include "text.h"
#include "window.h"
#include "constants/rgb.h"
#include "constants/songs.h"

// Émeraude Couronnement : graphique de la Bourse (v0.38).
// Fenêtre posée sur le terrain au-dessus de la boîte de message : 30 bougies japonaises (vert = hausse, rouge = baisse),
// SELECT pour passer à la courbe des clôtures, A ou B pour revenir à la fiche.

#define GRAPH_PAL        13          // palette de fond libre sur le terrain (sauvegardée et rendue à la fermeture)
#define GRAPH_LARGEUR    30
#define GRAPH_HAUTEUR    13          // 8 + 30 x 13 tuiles = 0x18E : s'arrête avant la boîte de message (0x194)
#define GRAPH_BASE       8
#define NB_BOUGIES       (BOURSE_HISTO_JOURS - 1)
#define X0               48
#define PAS              6
#define Y_HAUT           20
#define Y_BAS            86

enum
{
    COUL_TRANSPARENT,
    COUL_FOND,
    COUL_TEXTE,
    COUL_OMBRE,
    COUL_HAUSSE,
    COUL_BAISSE,
    COUL_GRILLE,
    COUL_COURBE,
};

static const u16 sPalette[16] =
{
    RGB(0, 0, 0), RGB(2, 3, 6), RGB(31, 31, 31), RGB(10, 11, 14),
    RGB(6, 26, 10), RGB(30, 8, 6), RGB(7, 8, 12), RGB(31, 26, 6),
};

static const u8 sCouleursTexte[] = {COUL_FOND, COUL_TEXTE, COUL_OMBRE};
static const u8 sText_Bougies[] = _("Bougies");
static const u8 sText_Courbe[] = _("Courbe");
static const u8 sText_30Jours[] = _("30 jours");

static EWRAM_DATA u16 sPaletteSauvee[16] = {0};

enum { T_FENETRE, T_MODE, T_ENTREPRISE };

static u32 Y(u32 v, u32 mini, u32 maxi)
{
    return Y_HAUT + (maxi - v) * (Y_BAS - Y_HAUT) / (maxi - mini);
}

static void Pixel(u8 fen, u8 coul, s32 x, s32 y, s32 l, s32 h)
{
    if (l > 0 && h > 0)
        FillWindowPixelRect(fen, coul, x, y, l, h);
}

static void Ligne(u8 fen, u8 coul, s32 x0, s32 y0, s32 x1, s32 y1)
{
    s32 dx = x1 > x0 ? x1 - x0 : x0 - x1, sx = x0 < x1 ? 1 : -1;
    s32 dy = y1 > y0 ? y0 - y1 : y1 - y0, sy = y0 < y1 ? 1 : -1;
    s32 err = dx + dy, e2;

    while (TRUE)
    {
        Pixel(fen, coul, x0, y0, 1, 1);
        if (x0 == x1 && y0 == y1)
            break;
        e2 = 2 * err;
        if (e2 >= dy)
        {
            err += dy;
            x0 += sx;
        }
        if (e2 <= dx)
        {
            err += dx;
            y0 += sy;
        }
    }
}

static void Etiquette(u8 fen, u32 valeur, u32 y)
{
    u8 txt[12];
    u32 chiffres = 1, p = 10;

    while (valeur >= p && chiffres < 7)
    {
        chiffres++;
        p *= 10;
    }
    ConvertIntToDecimalStringN(txt, valeur, STR_CONV_MODE_LEFT_ALIGN, chiffres);
    AddTextPrinterParameterized3(fen, FONT_SMALL, X0 - 6 - GetStringWidth(FONT_SMALL, txt, 0), y - 6, sCouleursTexte, TEXT_SKIP_DRAW, txt);
}

void Bourse_GraphiqueDessiner(u8 fen, u32 entreprise, u32 mode)
{
    struct BourseBougie b[NB_BOUGIES];
    u8 txt[64];
    u32 k, mini = 0xFFFFFFFF, maxi = 0, marge;
    s32 x, xPrec = 0, yPrec = 0;

    for (k = 0; k < NB_BOUGIES; k++)
    {
        Bourse_Bougie(entreprise, NB_BOUGIES - 1 - k, &b[k]); // k = 0 : la plus ancienne
        mini = min(mini, mode ? b[k].cloture : b[k].bas);
        maxi = max(maxi, mode ? b[k].cloture : b[k].haut);
    }
    marge = (maxi - mini) / 20 + 1;
    maxi += marge;
    mini = mini > marge ? mini - marge : 0;

    FillWindowPixelBuffer(fen, PIXEL_FILL(COUL_FOND));
    Bourse_TexteEntete(txt, entreprise);
    AddTextPrinterParameterized3(fen, FONT_SMALL, 4, 1, sCouleursTexte, TEXT_SKIP_DRAW, txt);
    AddTextPrinterParameterized3(fen, FONT_SMALL, 236 - GetStringWidth(FONT_SMALL, mode ? sText_Courbe : sText_Bougies, 0), 1,
                                 sCouleursTexte, TEXT_SKIP_DRAW, mode ? sText_Courbe : sText_Bougies);

    // Grille et échelle des prix
    for (x = X0 - 3; x < X0 + NB_BOUGIES * PAS; x += 4)
    {
        Pixel(fen, COUL_GRILLE, x, Y_HAUT, 2, 1);
        Pixel(fen, COUL_GRILLE, x, (Y_HAUT + Y_BAS) / 2, 2, 1);
        Pixel(fen, COUL_GRILLE, x, Y_BAS, 2, 1);
    }
    Etiquette(fen, maxi, Y_HAUT);
    Etiquette(fen, (maxi + mini) / 2, (Y_HAUT + Y_BAS) / 2);
    Etiquette(fen, mini, Y_BAS);
    AddTextPrinterParameterized3(fen, FONT_SMALL, X0, Y_BAS + 3, sCouleursTexte, TEXT_SKIP_DRAW, sText_30Jours);

    for (k = 0; k < NB_BOUGIES; k++)
    {
        x = X0 + k * PAS;
        if (mode == 0)
        {
            u8 coul = b[k].cloture >= b[k].ouverture ? COUL_HAUSSE : COUL_BAISSE;
            u32 yHaut = Y(b[k].haut, mini, maxi), yBas = Y(b[k].bas, mini, maxi);
            u32 yCorpsHaut = Y(max(b[k].ouverture, b[k].cloture), mini, maxi);
            u32 yCorpsBas = Y(min(b[k].ouverture, b[k].cloture), mini, maxi);
            Pixel(fen, coul, x + 1, yHaut, 1, yBas - yHaut + 1);
            Pixel(fen, coul, x, yCorpsHaut, 3, yCorpsBas - yCorpsHaut + 1);
        }
        else
        {
            s32 y = Y(b[k].cloture, mini, maxi);
            if (k > 0)
                Ligne(fen, COUL_COURBE, xPrec, yPrec, x + 1, y);
            xPrec = x + 1;
            yPrec = y;
        }
    }
    if (mode)
        Pixel(fen, COUL_COURBE, xPrec - 1, yPrec - 1, 3, 3); // point du cours actuel

    PutWindowTilemap(fen);
    CopyWindowToVram(fen, COPYWIN_FULL);
}

static void Task_Graphique(u8 taskId)
{
    s16 *data = gTasks[taskId].data;
    u8 fen = data[T_FENETRE];

    if (JOY_NEW(SELECT_BUTTON))
    {
        PlaySE(SE_SELECT);
        data[T_MODE] ^= 1;
        Bourse_GraphiqueDessiner(fen, data[T_ENTREPRISE], data[T_MODE]);
    }
    else if (JOY_NEW(A_BUTTON | B_BUTTON))
    {
        PlaySE(SE_SELECT);
        ClearWindowTilemap(fen);
        CopyWindowToVram(fen, COPYWIN_MAP);
        RemoveWindow(fen);
        LoadPalette(sPaletteSauvee, BG_PLTT_ID(GRAPH_PAL), PLTT_SIZE_4BPP);
        DestroyTask(taskId);
        ScriptContext_Enable();
    }
}

void Bourse_Graphique(void)
{
    static const struct WindowTemplate sFenetre =
    {
        .bg = 0, .tilemapLeft = 0, .tilemapTop = 0, .width = GRAPH_LARGEUR, .height = GRAPH_HAUTEUR,
        .paletteNum = GRAPH_PAL, .baseBlock = GRAPH_BASE,
    };
    u32 entreprise = gSpecialVar_0x8004 <= BOURSE_OR ? gSpecialVar_0x8004 : 0;
    u8 fen = AddWindow(&sFenetre), taskId;

    if (fen == WINDOW_NONE)
    {
        ScriptContext_Enable();
        return;
    }
    CpuCopy16(&gPlttBufferUnfaded[BG_PLTT_ID(GRAPH_PAL)], sPaletteSauvee, PLTT_SIZE_4BPP);
    LoadPalette(sPalette, BG_PLTT_ID(GRAPH_PAL), PLTT_SIZE_4BPP);
    Bourse_GraphiqueDessiner(fen, entreprise, 0);
    taskId = CreateTask(Task_Graphique, 80);
    gTasks[taskId].data[T_FENETRE] = fen;
    gTasks[taskId].data[T_MODE] = 0;
    gTasks[taskId].data[T_ENTREPRISE] = entreprise;
}
