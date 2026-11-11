#include "global.h"
#include "difficulty.h"
#include "event_data.h"
#include "item_use.h"
#include "test/test.h"
#include "constants/items.h"

// Émeraude Couronnement : règles des modes Hard et Kaizo.

TEST("Difficulty rules: healing items are allowed in battle in Normal")
{
    SetCurrentDifficultyLevel(DIFFICULTY_NORMAL);
    EXPECT(!IsHealingItemBannedInBattle(ITEM_POTION));
    EXPECT(!IsHealingItemBannedInBattle(ITEM_REVIVE));
}

TEST("Difficulty rules: healing items are banned in battle in Hard and Kaizo")
{
    u32 difficulty;
    PARAMETRIZE { difficulty = DIFFICULTY_HARD; }
    PARAMETRIZE { difficulty = DIFFICULTY_KAIZO; }
    SetCurrentDifficultyLevel(difficulty);
    EXPECT(IsHealingItemBannedInBattle(ITEM_POTION));
    EXPECT(IsHealingItemBannedInBattle(ITEM_FULL_RESTORE));
    EXPECT(IsHealingItemBannedInBattle(ITEM_ANTIDOTE));
    EXPECT(IsHealingItemBannedInBattle(ITEM_REVIVE));
    EXPECT(IsHealingItemBannedInBattle(ITEM_ETHER));
    SetCurrentDifficultyLevel(DIFFICULTY_NORMAL);
}

TEST("Difficulty rules: Poke Balls and X items stay usable in Hard")
{
    SetCurrentDifficultyLevel(DIFFICULTY_HARD);
    EXPECT(!IsHealingItemBannedInBattle(ITEM_POKE_BALL));
    EXPECT(!IsHealingItemBannedInBattle(ITEM_X_ATTACK));
    SetCurrentDifficultyLevel(DIFFICULTY_NORMAL);
}
