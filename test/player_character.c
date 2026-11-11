#include "global.h"
#include "event_data.h"
#include "field_player_avatar.h"
#include "player_character.h"
#include "pokemon.h"
#include "trainer.h"
#include "test/test.h"
#include "constants/event_objects.h"
#include "constants/trainers.h"

// Émeraude Couronnement : personnages jouables.

static void SetCharacter(enum PlayerCharacter character)
{
    gSaveBlock2Ptr->playerGender = GetPlayerCharacterGender(character);
    VarSet(VAR_PLAYER_CHARACTER, character);
}

TEST("Playable characters: Red uses Red's sprites everywhere")
{
    SetCharacter(PLAYER_CHAR_RED);
    EXPECT_EQ(GetPlayerAvatarGraphicsIdByStateIdAndGender(PLAYER_AVATAR_STATE_NORMAL, MALE), OBJ_EVENT_GFX_RED_NORMAL);
    EXPECT_EQ(GetPlayerAvatarGraphicsIdByStateIdAndGender(PLAYER_AVATAR_STATE_SURFING, MALE), OBJ_EVENT_GFX_RED_SURF);
    EXPECT_EQ(PlayerGenderToFrontTrainerPicId(MALE), TRAINER_PIC_RED);
    EXPECT_EQ(GetPlayerTrainerPic(MALE, VERSION_EMERALD), TRAINER_PIC_RED);
}

TEST("Playable characters: Green uses Green's sprites everywhere")
{
    SetCharacter(PLAYER_CHAR_GREEN);
    EXPECT_EQ(GetPlayerAvatarGraphicsIdByStateIdAndGender(PLAYER_AVATAR_STATE_NORMAL, FEMALE), OBJ_EVENT_GFX_GREEN_NORMAL);
    EXPECT_EQ(PlayerGenderToFrontTrainerPicId(FEMALE), TRAINER_PIC_LEAF);
    EXPECT_EQ(GetPlayerTrainerPic(FEMALE, VERSION_EMERALD), TRAINER_PIC_LEAF);
}

TEST("Playable characters: Brendan and May keep the Hoenn sprites")
{
    SetCharacter(PLAYER_CHAR_BRENDAN);
    EXPECT_EQ(GetPlayerAvatarGraphicsIdByStateIdAndGender(PLAYER_AVATAR_STATE_NORMAL, MALE), OBJ_EVENT_GFX_BRENDAN_NORMAL);
    EXPECT_EQ(PlayerGenderToFrontTrainerPicId(MALE), TRAINER_PIC_BRENDAN);
    SetCharacter(PLAYER_CHAR_MAY);
    EXPECT_EQ(GetPlayerAvatarGraphicsIdByStateIdAndGender(PLAYER_AVATAR_STATE_NORMAL, FEMALE), OBJ_EVENT_GFX_MAY_NORMAL);
    EXPECT_EQ(GetPlayerTrainerPic(FEMALE, VERSION_EMERALD), TRAINER_PIC_MAY);
}

TEST("Playable characters: a character that does not match the gender falls back to Hoenn")
{
    gSaveBlock2Ptr->playerGender = FEMALE;
    VarSet(VAR_PLAYER_CHARACTER, PLAYER_CHAR_RED);
    EXPECT_EQ(GetPlayerCharacter(), PLAYER_CHAR_MAY);
    EXPECT_EQ(PlayerGenderToFrontTrainerPicId(FEMALE), TRAINER_PIC_MAY);
}
