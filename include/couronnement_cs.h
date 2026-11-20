#ifndef GUARD_COURONNEMENT_CS_H
#define GUARD_COURONNEMENT_CS_H

#include "constants/field_move.h"

// Émeraude Couronnement (lot 1 du cahier v0.42) : CS utilisables sur le terrain sans qu'un Pokémon connaisse l'attaque,
// à condition d'avoir la CS dans le sac ET le badge qui la débloque.
bool32 Couronnement_CSDisponible(enum FieldMove fieldMove);

// Specials du menu L
bool8 Couronnement_VolPossible(void);
bool8 Couronnement_FlashPossible(void);
void Couronnement_Vol(void);

#endif // GUARD_COURONNEMENT_CS_H
