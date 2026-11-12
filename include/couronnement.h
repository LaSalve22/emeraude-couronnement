#ifndef GUARD_COURONNEMENT_H
#define GUARD_COURONNEMENT_H

#include "constants/couronnement.h"

struct Trainer;

u16 Couronnement_GetElo(void);
void Couronnement_SetElo(u16 elo);
u32 Couronnement_RangDepuisElo(u16 elo);
u32 Couronnement_PalierDepuisElo(u16 elo);
s32 Couronnement_VariationElo(u16 eloJoueur, u16 eloAdversaire, bool32 victoire);
u16 Couronnement_EloAdversaire(u32 indexAdversaire);
u32 Couronnement_PremierAdversaire(u32 tournoi);
u32 Couronnement_NbManches(u32 tournoi);
u32 Couronnement_TournoiDepuisAdversaire(u32 indexAdversaire);
struct TrainerMon;
struct Trainer;
struct Pokemon;
s32 Couronnement_ScoreAdverse(const struct TrainerMon *mon, struct Pokemon *joueur, u32 nbJoueur);
u32 Couronnement_NbTournoisGagnes(void);
const struct Trainer *GetCouronnementTrainer(void);
bool32 Couronnement_EquipeValide(const u16 *species, u32 count);

// Specials (data/specials.inc)
void Couronnement_DonnerPass(void);
void Couronnement_BufferStatut(void);
void Couronnement_EtatTournoi(void);
void Couronnement_CompterPokemonAptes(void);
void Couronnement_DebutTournoi(void);
void Couronnement_PreparerManche(void);
void Couronnement_EnregistrerResultat(void);
void Couronnement_MancheSuivante(void);
void Couronnement_TournoiGagne(void);
void Couronnement_ChoixAdverse(void);
void Couronnement_FinTournoi(void);
void Couronnement_EstChampionDuMonde(void);
void Couronnement_BufferMenuL(void);

#endif // GUARD_COURONNEMENT_H
