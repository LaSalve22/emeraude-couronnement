#ifndef GUARD_CONSTANTS_COURONNEMENT_H
#define GUARD_CONSTANTS_COURONNEMENT_H

// Émeraude Couronnement : Couronnement Mondial (classement Elo façon Pokémon Champions).

#include "constants/couronnement_adversaires.h"

#define COURONNEMENT_ELO_DEPART     1500
#define COURONNEMENT_ELO_MIN        100
#define COURONNEMENT_ELO_MAX        3000
#define COURONNEMENT_K              32      // gain/perte max par combat
#define COURONNEMENT_RANG_DEPART    10000   // rang à 1500 d'Elo
#define COURONNEMENT_TOP_CHAMPIONS  32      // rang <= 32 : palier Champions

// Paliers
#define PALIER_POKE        0   // Elo < 1300
#define PALIER_SUPER       1   // 1300 <= Elo < 1600
#define PALIER_HYPER       2   // 1600 <= Elo < 1800
#define PALIER_MASTER      3   // Elo >= 1800
#define PALIER_CHAMPIONS   4   // Top 32

#define COURONNEMENT_SEUIL_SUPER   1300
#define COURONNEMENT_SEUIL_HYPER   1600
#define COURONNEMENT_SEUIL_MASTER  1800

// Tournois des villes d'Arène (tournoi t accessible après le badge t+1).
#define COURONNEMENT_TOURNOI_MEROUVILLE    0
#define COURONNEMENT_TOURNOI_MYOKARA       1
#define COURONNEMENT_TOURNOI_LAVANDIA      2
#define COURONNEMENT_TOURNOI_VERMILAVA     3
#define COURONNEMENT_TOURNOI_CLEMENTI      4
#define COURONNEMENT_TOURNOI_CIMETRONELLE  5
#define COURONNEMENT_TOURNOI_ALGATIA       6
#define COURONNEMENT_TOURNOI_ATALANOPOLIS  7
#define COURONNEMENT_NB_TOURNOIS_VILLES    8
// Salles de Concours d'Émeraude : Tentes de Combat de Poivressel, Vergazon, Autéquia et salle de Nénucrique.
#define COURONNEMENT_TOURNOI_POIVRESSEL    8
#define COURONNEMENT_TOURNOI_VERGAZON      9
#define COURONNEMENT_TOURNOI_AUTEQUIA      10
#define COURONNEMENT_TOURNOI_NENUCRIQUE    11
#define COURONNEMENT_NB_TOURNOIS           12  // villes + salles
#define COURONNEMENT_TOURNOI_FINALE        12  // Tour de Combat, après la Ligue, Top 32
#define COURONNEMENT_NB_TOURNOIS_TOTAL     13
#define COURONNEMENT_MANCHES               3
#define COURONNEMENT_MANCHES_FINALE        5   // 32 joueurs -> 5 tours
#define COURONNEMENT_DEMI_FINALE           3   // manche (0..4) précédée d'un soin
#define COURONNEMENT_TAILLE_EQUIPE         4   // 6 montrés, 4 choisis
#define COURONNEMENT_TAILLE_ADVERSE        6

// Résultat de Couronnement_EtatTournoi
#define TOURNOI_PAS_DE_PASS   0
#define TOURNOI_BADGE_MANQUANT 1
#define TOURNOI_OUVERT        2
#define TOURNOI_DEJA_GAGNE    3   // rejouable pour l'Elo, sans récompense
#define TOURNOI_LIGUE_MANQUANTE 4 // finale : Ligue pas encore battue
#define TOURNOI_PAS_TOP32     5   // finale : rang > 32

// Valeur de VAR_FRONTIER_FACILITY pendant le choix des 4 Pokémon.
#define FACILITY_COURONNEMENT 10

// Coach EV (src/coach_ev.c) : stats 0 PV, 1 Attaque, 2 Défense, 3 Atq. Spé., 4 Déf. Spé., 5 Vitesse ; 6 = tout à 0.
#define COACH_EV_TOUT_A_ZERO 6

#endif // GUARD_CONSTANTS_COURONNEMENT_H
