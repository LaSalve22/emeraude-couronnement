#include "global.h"
#include "couronnement_cs.h"
#include "event_data.h"
#include "field_move.h"
#include "field_player_avatar.h"
#include "fieldmap.h"
#include "item.h"
#include "main.h"
#include "overworld.h"
#include "palette.h"
#include "party_menu.h"
#include "region_map.h"
#include "script.h"
#include "task.h"
#include "constants/flags.h"
#include "constants/items.h"
#include "constants/rgb.h"

// Émeraude Couronnement (lot 1 du cahier v0.42) : CS sans capacité.
// Coupe, Éclate-Roc, Force, Surf, Cascade et Plongée passent par `checkfieldmove` (src/scrcmd.c) et PartyHasMonWithSurf :
// si aucun Pokémon ne connaît l'attaque mais que la CS est dans le sac et le badge obtenu, le premier Pokémon de l'équipe
// fait l'animation. Vol et Flash se lancent depuis le menu L.

static const u16 sCS[FIELD_MOVES_COUNT] =
{
    [FIELD_MOVE_CUT]        = ITEM_HM_CUT,
    [FIELD_MOVE_FLY]        = ITEM_HM_FLY,
    [FIELD_MOVE_SURF]       = ITEM_HM_SURF,
    [FIELD_MOVE_STRENGTH]   = ITEM_HM_STRENGTH,
    [FIELD_MOVE_FLASH]      = ITEM_HM_FLASH,
    [FIELD_MOVE_ROCK_SMASH] = ITEM_HM_ROCK_SMASH,
    [FIELD_MOVE_WATERFALL]  = ITEM_HM_WATERFALL,
    [FIELD_MOVE_DIVE]       = ITEM_HM_DIVE,
};

bool32 Couronnement_CSDisponible(enum FieldMove fieldMove)
{
    if (fieldMove >= FIELD_MOVES_COUNT || sCS[fieldMove] == ITEM_NONE)
        return FALSE;
    return IsFieldMoveUnlocked(fieldMove) && CheckBagHasItem(sCS[fieldMove], 1);
}

bool8 Couronnement_VolPossible(void)
{
    return Couronnement_CSDisponible(FIELD_MOVE_FLY) && SetUpFieldMove_Fly();
}

bool8 Couronnement_FlashPossible(void)
{
    return Couronnement_CSDisponible(FIELD_MOVE_FLASH) && gMapHeader.cave == TRUE && !FlagGet(FLAG_SYS_USE_FLASH);
}

// Vol depuis le menu L : fondu, puis la carte de vol. Annuler ramène sur le terrain (sVolMenuL, src/region_map.c).
static void Task_OuvrirVol(u8 taskId)
{
    if (!gPaletteFade.active)
    {
        gVolDepuisMenuL = TRUE;
        SetMainCallback2(CB2_OpenFlyMap);
        DestroyTask(taskId);
    }
}

void Couronnement_Vol(void)
{
    LockPlayerFieldControls();
    BeginNormalPaletteFade(PALETTES_ALL, 0, 0, 16, RGB_BLACK);
    CreateTask(Task_OuvrirVol, 0);
}
