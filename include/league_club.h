#ifndef GUARD_LEAGUE_CLUB_H
#define GUARD_LEAGUE_CLUB_H

// Émeraude Couronnement : Ligue des Clubs (v0.42, 1re partie : joueur licencié).
// 10 clubs de villes en 2 poules de 5, tirées au sort à chaque saison de 30 jours. Chaque rencontre : 4 duels en 6 contre 6
// (Capitaine, Titulaire, Titulaire, Recrue). Barème « modèle 1 » : victoire 4, nul 2, défaite 0, bonus offensif +1 (6-0),
// bonus défensif +1 (l'adversaire gagne avec 1 seul Pokémon). Le joueur licencié prend la place de la Recrue et joue son
// duel en vrai ; tous les autres duels sont simulés (rating 1-100, +6 à domicile, +4 si 3 Pokémon profitent du stade).

#define CLUB_NB                 10
#define CLUB_PAR_POULE          5
#define CLUB_DRESSEURS          4       // par club
#define CLUB_SLOT_JOUEUR        3       // le joueur licencié remplace la Recrue (4e place)
#define CLUB_TAILLE_EQUIPE      6
#define CLUB_JOURS_SAISON       30
#define CLUB_NB_JOURNEES        10      // 5 clubs en aller-retour : 10 journées, chaque club exempt 2 fois
#define CLUB_MATCHS_PAR_JOURNEE 4       // 2 par poule
#define CLUB_NB_MATCHS          (CLUB_NB_JOURNEES * CLUB_MATCHS_PAR_JOURNEE) // 40 matchs de poule
#define CLUB_MATCH_FINALE       CLUB_NB_MATCHS                                 // 41e : la Grande Finale
#define CLUB_JOUR_FINALE        30
#define CLUB_JOURS_MERCATO      2       // jours 1 et 2
#define CLUB_AUCUN              0xFF
#define CLUB_MAGIE              0xC1B5

enum ClubTier { CLUB_TIER_C, CLUB_TIER_B, CLUB_TIER_A, CLUB_TIER_S, CLUB_NB_TIERS };

// Effets de stade, lancés au 1er tour de chaque combat de ligue (pas en finale) : 5 tours (Vent Arrière : 4 tours).
enum ClubStade
{
    CLUB_STADE_HERBU,       // Champ Herbu
    CLUB_STADE_GRAVITE,     // Gravité
    CLUB_STADE_SOLEIL,
    CLUB_STADE_VENT,        // Vent Arrière des deux côtés
    CLUB_STADE_SABLE,
    CLUB_STADE_ELECTRIQUE,  // Champ Électrifié
    CLUB_STADE_PLUIE,
    CLUB_STADE_DISTORSION,  // Distorsion
    CLUB_STADE_PSYCHIQUE,   // Champ Psychique
    CLUB_STADE_NEIGE,
    CLUB_STADE_NEUTRE,      // finale à la Tour de Combat
};

// Codage d'un duel sur 4 bits : 0 = pas joué, 1-6 = victoire à domicile avec n survivants,
// 7-12 = victoire à l'extérieur avec (n - 6) survivants, 13 = nul (double K.O.).
#define CLUB_DUEL_PAS_JOUE      0
#define CLUB_DUEL_NUL           13
#define CLUB_DUEL_DOMICILE(n)   (n)
#define CLUB_DUEL_EXTERIEUR(n)  (6 + (n))

// Primes et licences (¥), par tier C / B / A / S.
#define CLUB_PRIME_FINALISTE    15000
#define CLUB_PRIME_VAINQUEUR    30000   // + Patch Talent
#define CLUB_CLAUSE_POURCENT    50      // hors mercato : +50 % de l'ancienne licence

struct ClubDuel { u8 domicile, exterieur; }; // points d'un duel

// Moteur (testable)
void CalculateDuelPoints(u8 homeSurvivors, u8 awaySurvivors, u8 *outHomePts, u8 *outAwayPts);
u32 Club_PointsDuel(u32 code, bool32 domicile);       // points d'un duel codé, pour le club à domicile ou à l'extérieur
u32 Club_KOInfliges(u32 code, bool32 domicile);
void Club_MettreAJourJusqua(u32 aujourdhui);          // rattrape les jours (journées, finale, fin de saison)
u32 Club_Saison(void);
u32 Club_JourDeSaison(void);                          // 1 à 30
u32 Club_JourDeJournee(u32 journee);                  // jour de la saison où se joue la journée (0-9)
void Club_Tirage(u32 saison, u8 *clubs);              // clubs[0-4] = poule A, clubs[5-9] = poule B
void Club_Affiche(u32 match, u8 *domicile, u8 *exterieur); // clubs d'un match de poule (0-39) ou de la finale (40)
u32 Club_Duel(u32 match, u32 slot);
u32 Club_Classement(u32 poule, u8 *ordre);            // ordre[5] ; renvoie le nombre de matchs déjà joués
u32 Club_Points(u32 club);
s32 Club_DiffKO(u32 club);
u32 Club_BonusOffensifs(u32 club);
u32 Club_MatchsJoues(u32 club);
u32 Club_Poule(u32 club);
u32 Club_Rating(u32 club, u32 slot);                  // rating de base + forme de la saison
u32 Club_RatingEffectif(u32 club, u32 slot, u32 stade, bool32 domicile);
u32 Club_SimulerDuel(u32 match, u32 slot);            // résultat codé d'un duel simulé (déterministe)
u32 Club_DuJoueur(void);                              // CLUB_AUCUN si pas de licence
u32 Club_Tier(u32 club);
u32 Club_PrixLicence(u32 club);
u32 Club_CoutSignature(u32 club);                     // licence + clause libératoire hors mercato
bool32 Club_Signer(u32 club);
u32 Club_MatchDuJoueur(void);                         // match où le joueur doit jouer son duel (0xFF si aucun)
void Club_EnregistrerDuelJoueur(u32 code);            // code vu du club à domicile
u32 Club_Titres(void);
u32 Club_Stade(u32 club);
const u8 *Club_Nom(u32 club);
const u8 *Club_NomCourt(u32 club);
const u8 *Club_NomStade(u32 club);
const u8 *Club_NomDresseur(u32 club, u32 slot);
u32 Club_CouleurClub(u32 club);
u32 Club_Vainqueur(void);
const u8 *Club_NomSlot(u32 slot);                     // Capitaine, Titulaire, Recrue
const u8 *Club_NomEffet(u32 stade);                             // club vainqueur de la finale de la saison (CLUB_AUCUN si pas jouée)

// Tableau de bord (src/club_dashboard.c) : onglets 0 Classement, 1 Feuille de match, 2 Effectif
void Club_TableauDessiner(u8 fen, u32 onglet, u32 poule, u32 journee, u32 match, u32 club);

// Combat
bool32 Club_CombatEnCours(void);
const struct Trainer *Club_Adversaire(void);

#endif // GUARD_LEAGUE_CLUB_H
