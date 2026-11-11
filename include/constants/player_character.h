#ifndef GUARD_CONSTANTS_PLAYER_CHARACTER_H
#define GUARD_CONSTANTS_PLAYER_CHARACTER_H

// Émeraude Couronnement : personnages jouables (choisis dans l'intro du Prof. Seko, stockés dans VAR_PLAYER_CHARACTER).
// Le sexe du joueur (playerGender) découle du personnage : il décide toujours de la maison et du rival.
// Pour ajouter Blue, Ash, Lucas ou Dawn : il faut leurs sprites complets (terrain, vélo, surf, pêche, face, dos).
enum PlayerCharacter
{
    PLAYER_CHAR_BRENDAN, // Brice
    PLAYER_CHAR_MAY,     // Flora
    PLAYER_CHAR_RED,
    PLAYER_CHAR_GREEN,   // Leaf / Green de Rouge Feu - Vert Feuille
    PLAYER_CHAR_COUNT,
};

#endif // GUARD_CONSTANTS_PLAYER_CHARACTER_H
