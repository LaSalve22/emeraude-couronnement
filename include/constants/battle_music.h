#ifndef GUARD_CONSTANTS_BATTLE_MUSIC_H
#define GUARD_CONSTANTS_BATTLE_MUSIC_H

// Émeraude Couronnement : choix de la musique de combat (menu OPTIONS, stocké dans VAR_BATTLE_MUSIC).
// Pour ajouter une région (Johto, Sinnoh, animé…) : nouvelle valeur ici, nouveau libellé dans
// src/option_menu.c et nouvelle correspondance dans GetBattleBGM (src/pokemon.c).
enum BattleMusicChoice
{
    BATTLE_MUSIC_HOENN,
    BATTLE_MUSIC_KANTO,
    BATTLE_MUSIC_RANDOM,
    BATTLE_MUSIC_COUNT,
};

#endif // GUARD_CONSTANTS_BATTLE_MUSIC_H
