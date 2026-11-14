#ifndef GUARD_COACH_EV_H
#define GUARD_COACH_EV_H

// Émeraude Couronnement : Coach EV (après la Ligue), Centre Pokémon de Clémenti-Ville.
#include "constants/couronnement.h"

struct Pokemon;
u32 CoachEV_Total(struct Pokemon *mon);
u32 CoachEV_Regler(struct Pokemon *mon, u32 stat, u32 valeur);
void CoachEV_ToutRemettre(struct Pokemon *mon);

// IV (Coach après la Ligue, Capsules d'Argent / d'Or). stat : ordre d'affichage PV, ATQ, DÉF, A.SPÉ, D.SPÉ, VIT.
void CoachIV_Regler(struct Pokemon *mon, u32 stat, u32 valeur);
u32 Capsule_Maximiser(struct Pokemon *mon, u32 stat);

// Specials (data/specials.inc)
void CoachEV_Afficher(void);
void CoachEV_Appliquer(void);
void CoachIV_Afficher(void);
void CoachIV_Appliquer(void);
u16 Capsule_EstOr(void);
u16 Capsule_Utiliser(void);

#endif // GUARD_COACH_EV_H
