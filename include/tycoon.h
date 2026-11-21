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
// Niveaux par tier (v0.50) : N.10 sans club ou tier C 1re saison, 18 en C ensuite, 30 en B, 44 en A, 60 en S ou propriétaire.
#define TYCOON_NIVEAU_DEPART 10
#define TYCOON_NIVEAU_C      18
#define TYCOON_NIVEAU_B      30
#define TYCOON_NIVEAU_A      44
#define TYCOON_NIVEAU_S      60
struct Trainer;
u32 Tycoon_NiveauCible(void);
void Tycoon_AjusterSauvage(u16 *species, u8 *niveau);
const struct Trainer *Tycoon_AjusterDresseur(const struct Trainer *dresseur);
void Tycoon_ChoisirProfil(void);                  // VAR_0x8004 = profil (intro adulte)
u32 Tycoon_Profil(void);
void Tycoon_BoutiqueNiveau(void);                 // special : 0, 1 (L) ou 2 (XL) selon le tier
void Tycoon_AccreditationsAuto(void);              // Arènes fermées : badges posés selon le scénario (v0.53)
void Tycoon_AreneFermee(void);                     // special
void Tycoon_SortirArene(void);
#define TYCOON_DEX_PALIER 25                           // récompense du Pokédex toutes les 25 espèces capturées
u32 Tycoon_PaliersDexAtteints(u32 captures);
void Tycoon_RecompenseDex(void);                   // special
void Tycoon_RecompenseDexDonnee(void);
void Tycoon_DessinerHall(void);                   // halls du Couronnement Mondial (v0.56)
void Tycoon_HallEntree(void);                     // special
void Tycoon_HallTournoi(void);                    // special             // special                     // special (+ waitstate)

#endif // GUARD_TYCOON_H
