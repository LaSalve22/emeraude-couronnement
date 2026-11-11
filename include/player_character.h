#ifndef GUARD_PLAYER_CHARACTER_H
#define GUARD_PLAYER_CHARACTER_H

#include "constants/player_character.h"

extern u8 gPendingPlayerCharacter;

enum PlayerCharacter GetPlayerCharacter(void);
enum Gender GetPlayerCharacterGender(enum PlayerCharacter character);
bool32 IsKantoPlayerCharacter(enum Gender gender);
const u8 *GetPlayerCharacterName(enum PlayerCharacter character);

#endif // GUARD_PLAYER_CHARACTER_H
