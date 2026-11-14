#ifndef GUARD_CALENDRIER_H
#define GUARD_CALENDRIER_H

#include "constants/siirtc.h"

// Émeraude Couronnement : jours de la semaine (v0.37), calculés sur le compteur de jours de l'horloge
// (RtcGetLocalDayCount : 1 = samedi 1er janvier 2000), donc le vrai jour du calendrier.
enum Weekday Calendrier_JourSemaine(u32 jour);
bool32 Calendrier_EstWeekend(u32 jour);
const u8 *Calendrier_NomJour(enum Weekday jourSemaine);
u32 Calendrier_Aujourdhui(void);

#endif // GUARD_CALENDRIER_H
