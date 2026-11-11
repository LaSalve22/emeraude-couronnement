#include "global.h"
#include "event_data.h"
#include "player_character.h"
#include "constants/vars.h"

// Émeraude Couronnement : personnages jouables.

// Choix fait pendant l'intro, avant NewGameInitData (qui remet les variables à zéro).
EWRAM_DATA u8 gPendingPlayerCharacter = PLAYER_CHAR_BRENDAN;

static const u8 sName_Brendan[] = _("Brice");
static const u8 sName_May[] = _("Flora");
static const u8 sName_Red[] = _("Red");
static const u8 sName_Green[] = _("Green");

static const u8 *const sCharacterNames[PLAYER_CHAR_COUNT] =
{
    [PLAYER_CHAR_BRENDAN] = sName_Brendan,
    [PLAYER_CHAR_MAY]     = sName_May,
    [PLAYER_CHAR_RED]     = sName_Red,
    [PLAYER_CHAR_GREEN]   = sName_Green,
};

static const u8 sCharacterGenders[PLAYER_CHAR_COUNT] =
{
    [PLAYER_CHAR_BRENDAN] = MALE,
    [PLAYER_CHAR_MAY]     = FEMALE,
    [PLAYER_CHAR_RED]     = MALE,
    [PLAYER_CHAR_GREEN]   = FEMALE,
};

enum Gender GetPlayerCharacterGender(enum PlayerCharacter character)
{
    if (character >= PLAYER_CHAR_COUNT)
        return MALE;
    return sCharacterGenders[character];
}

// Renvoie le personnage du joueur ; si la valeur ne correspond pas au sexe enregistré
// (ancienne sauvegarde), on retombe sur Brice ou Flora.
enum PlayerCharacter GetPlayerCharacter(void)
{
    u32 character = VarGet(VAR_PLAYER_CHARACTER);

    if (character >= PLAYER_CHAR_COUNT || sCharacterGenders[character] != gSaveBlock2Ptr->playerGender)
        return gSaveBlock2Ptr->playerGender == MALE ? PLAYER_CHAR_BRENDAN : PLAYER_CHAR_MAY;
    return character;
}

// Vrai si les sprites du joueur (de ce sexe) doivent être ceux de Red ou de Green.
bool32 IsKantoPlayerCharacter(enum Gender gender)
{
    enum PlayerCharacter character;

    if (gender != gSaveBlock2Ptr->playerGender)
        return FALSE;
    character = GetPlayerCharacter();
    return character == PLAYER_CHAR_RED || character == PLAYER_CHAR_GREEN;
}

const u8 *GetPlayerCharacterName(enum PlayerCharacter character)
{
    if (character >= PLAYER_CHAR_COUNT)
        character = PLAYER_CHAR_BRENDAN;
    return sCharacterNames[character];
}
