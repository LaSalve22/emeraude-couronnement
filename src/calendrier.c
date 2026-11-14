#include "global.h"
#include "calendrier.h"
#include "rtc.h"

// Émeraude Couronnement : jours de la semaine (v0.37).
// Effets : Bourse sans cotation le samedi et le dimanche, dividende des Grands Magasins le vendredi,
// échéances des banques le lundi, boutique +30 % le samedi et -50 % le dimanche, centre d'entraînement x2 points le dimanche.

static const u8 *const sNomsJours[WEEKDAY_COUNT] =
{
    [WEEKDAY_SUN] = COMPOUND_STRING("Dimanche"),
    [WEEKDAY_MON] = COMPOUND_STRING("Lundi"),
    [WEEKDAY_TUE] = COMPOUND_STRING("Mardi"),
    [WEEKDAY_WED] = COMPOUND_STRING("Mercredi"),
    [WEEKDAY_THU] = COMPOUND_STRING("Jeudi"),
    [WEEKDAY_FRI] = COMPOUND_STRING("Vendredi"),
    [WEEKDAY_SAT] = COMPOUND_STRING("Samedi"),
};

enum Weekday Calendrier_JourSemaine(u32 jour)
{
    return (WEEKDAY_SAT + jour + WEEKDAY_COUNT - 1) % WEEKDAY_COUNT;
}

bool32 Calendrier_EstWeekend(u32 jour)
{
    enum Weekday j = Calendrier_JourSemaine(jour);
    return j == WEEKDAY_SAT || j == WEEKDAY_SUN;
}

const u8 *Calendrier_NomJour(enum Weekday jourSemaine)
{
    return sNomsJours[jourSemaine % WEEKDAY_COUNT];
}

u32 Calendrier_Aujourdhui(void)
{
    return RtcGetLocalDayCount();
}
