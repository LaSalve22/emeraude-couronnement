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

// Jour de jeu : l'horloge de la cartouche, mais jamais avant le dernier jour déjà traité par l'économie
// (Bourse, immobilier, banques, société, arènes). Reculer l'horloge ne fait donc ni rejouer les jours (loyers, dividendes,
// sponsors, cours), ni choisir une semaine plus favorable pour les prix de l'immobilier.
u32 Calendrier_Aujourdhui(void)
{
    u32 jour = RtcGetLocalDayCount();
    const struct SaveBlock2 *sb2 = gSaveBlock2Ptr;
    const struct SaveBlock1 *sb1 = gSaveBlock1Ptr;
    const struct SaveBlock3 *sb3 = gSaveBlock3Ptr;

    if (sb2->pokedex.bourse.cours[0] != 0 && sb2->pokedex.bourse.jour > jour)
        jour = sb2->pokedex.bourse.jour;
    if (sb2->pokedex.societe.etat != 0 && sb2->pokedex.societe.jour > jour)
        jour = sb2->pokedex.societe.jour;
    if (sb1->immo.magie == IMMO_MAGIE && sb1->immo.jour > jour)
        jour = sb1->immo.jour;
    if (sb1->banque.magie == BANQUE_MAGIE && sb1->banque.jour > jour)
        jour = sb1->banque.jour;
    if (sb3->arenes.jour > jour && sb3->arenes.magie == ARENE_MAGIE)
        jour = sb3->arenes.jour;
    return jour;
}
