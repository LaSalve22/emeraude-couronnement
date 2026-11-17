#ifndef GUARD_TYCOON_H
#define GUARD_TYCOON_H

// Émeraude Couronnement : édition « Carrière & Business » (EDITION_TYCOON_SIM, include/config/general.h), src/tycoon.c.
// Pas de level cap ; Taxi Volant et Taxi Surfant dès le début ; les autres capacités de terrain se débloquent selon le tier
// du club du joueur (Ligue des Clubs) et s'utilisent sans qu'un Pokémon les connaisse ; la Ligue Pokémon s'ouvre une fois
// le rang Champions (Top 32) atteint au Couronnement Mondial. Les fonctions existent dans les deux éditions (tests).

#include "constants/field_move.h"

#define COURONNEMENT_CHAMPIONS_ATTEINT 0x80   // bit de CouronnementSaison.paliers, jamais remis à zéro

bool32 Tycoon_EditionActive(void);
bool32 Tycoon_CapaciteDebloquee(u32 fieldMove);   // règle de l'édition Carrière (tier du club)
bool32 Tycoon_CapaciteSansPokemon(u32 fieldMove); // Carrière : pas besoin qu'un Pokémon connaisse la capacité
u32 Tycoon_TierRequis(u32 fieldMove);             // CLUB_TIER_* ; 0xFF = toujours (taxis)
bool32 Couronnement_ChampionsAtteint(void);
void Tycoon_ChoisirProfil(void);                  // VAR_0x8004 = profil (intro adulte)
u32 Tycoon_Profil(void);

#endif // GUARD_TYCOON_H
