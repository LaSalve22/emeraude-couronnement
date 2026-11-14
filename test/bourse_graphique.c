#include "global.h"
#include "bourse.h"
#include "bg.h"
#include "malloc.h"
#include "window.h"
#include "test/test.h"

// Émeraude Couronnement (v0.38) : dessine le graphique dans une fenêtre et vide ses pixels dans le journal
// (lignes « GRAPH m:<hex> »), pour en tirer une image avec tools/couronnement/apercu_graphique.py.

TEST("Graphique: draws candles and the curve")
{
    static const struct BgTemplate sBg = {.bg = 0, .charBaseIndex = 2, .mapBaseIndex = 31, .screenSize = 0, .paletteMode = 0, .priority = 0, .baseTile = 0};
    static const struct WindowTemplate sFen[] = {
        {.bg = 0, .tilemapLeft = 0, .tilemapTop = 0, .width = 30, .height = 13, .paletteNum = 13, .baseBlock = 8},
        DUMMY_WIN_TEMPLATE,
    };
    u32 mode, i, j;
    u8 *pix;
    char ligne[2 * 64 + 1];
    static const char sHex[] = "0123456789abcdef";

    memset(&gSaveBlock2Ptr->pokedex.bourse, 0, sizeof(gSaveBlock2Ptr->pokedex.bourse));
    memset(&gSaveBlock3Ptr->bourseOr, 0, sizeof(gSaveBlock3Ptr->bourseOr));
    Bourse_MettreAJourJusqua(704);
    ResetBgsAndClearDma3BusyFlags(0);
    InitBgsFromTemplates(0, &sBg, 1);
    InitWindows(sFen);
    for (mode = 0; mode < 4; mode++)
    {
        Bourse_GraphiqueDessiner(0, mode < 2 ? BOURSE_CASINO : BOURSE_OR, mode % 2);
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
            Test_MgbaPrintf("GRAPH %d:%s", mode, ligne);
        }
    }
    FreeAllWindowBuffers();
}
