#ifndef GUARD_COACH_EV_H
#define GUARD_COACH_EV_H

// Émeraude Couronnement : Coach EV (après la Ligue), Centre Pokémon de Clémenti-Ville.
#include "constants/couronnement.h"

struct Pokemon;
u32 CoachEV_Total(struct Pokemon *mon);
u32 CoachEV_Regler(struct Pokemon *mon, u32 stat, u32 valeur);
void CoachEV_ToutRemettre(struct Pokemon *mon);

// Specials (data/specials.inc)
void CoachEV_Afficher(void);
void CoachEV_Appliquer(void);

#endif // GUARD_COACH_EV_H
