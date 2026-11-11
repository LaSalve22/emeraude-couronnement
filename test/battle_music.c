#include "global.h"
#include "battle.h"
#include "event_data.h"
#include "pokemon.h"
#include "test/test.h"
#include "constants/battle_music.h"
#include "constants/songs.h"

// Émeraude Couronnement : option « Musique combat » du menu OPTIONS.

TEST("Battle music option: Hoenn keeps the Hoenn wild theme")
{
    VarSet(VAR_BATTLE_MUSIC, BATTLE_MUSIC_HOENN);
    gBattleTypeFlags = 0;
    EXPECT_EQ(GetBattleBGM(), MUS_VS_WILD);
}

TEST("Battle music option: Kanto uses the Red/Blue wild theme")
{
    VarSet(VAR_BATTLE_MUSIC, BATTLE_MUSIC_KANTO);
    gBattleTypeFlags = 0;
    EXPECT_EQ(GetBattleBGM(), MUS_RG_VS_WILD);
    VarSet(VAR_BATTLE_MUSIC, BATTLE_MUSIC_HOENN);
}

TEST("Battle music option: random picks a Hoenn or Kanto theme")
{
    u16 song;
    VarSet(VAR_BATTLE_MUSIC, BATTLE_MUSIC_RANDOM);
    gBattleTypeFlags = 0;
    song = GetBattleBGM();
    EXPECT(song == MUS_VS_WILD || song == MUS_RG_VS_WILD);
    VarSet(VAR_BATTLE_MUSIC, BATTLE_MUSIC_HOENN);
}
