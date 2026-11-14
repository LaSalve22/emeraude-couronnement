#ifndef GUARD_COMMERCES_H
#define GUARD_COMMERCES_H

// Émeraude Couronnement : commerces de la société (v0.36). Appli « Mes commerces » du menu L, une fois la société créée.
// Boutique (une ville au choix, un stock, une politique de prix, 3 niveaux, concurrence) et centre d'entraînement
// (recettes des dresseurs + points pour entraîner son équipe). État dans VAR_COMMERCE_* (include/constants/vars.h).

#define COMMERCE_NB_VILLES          12
#define COMMERCE_NB_STOCKS          4     // Baies, Soins, Poké Balls, Objets rares
#define COMMERCE_NB_PRIX            3     // Bas, Normal, Élevé
#define COMMERCE_NIVEAUX            3
#define COMMERCE_RATTRAPAGE_MAX     30
#define COMMERCE_CONCURRENT_RECETTE 60    // % de recette quand un concurrent est en face
#define COMMERCE_GUERRE_JOURS       5     // jours de prix bas pour faire fermer le concurrent
#define COMMERCE_CENTRE_PRIX        80000
#define COMMERCE_CENTRE_RECETTE     250   // par jour et par niveau
#define COMMERCE_CENTRE_POINTS      30    // points d'entraînement par jour et par niveau
#define COMMERCE_POINTS_MAX         9999
#define COMMERCE_SEANCE_XP          100   // points pour un niveau (sous le level cap)
#define COMMERCE_SEANCE_EV          40    // points pour +12 EV dans une stat
#define COMMERCE_EV_PAR_SEANCE      12

struct Pokemon;

// Moteur (testable)
bool32 Commerce_BoutiqueOuverte(void);
bool32 Commerce_CentreOuvert(void);
u32 Commerce_PrixBoutique(u32 ville);
u32 Commerce_PrixAgrandirBoutique(void);  // 0 si niveau max
u32 Commerce_PrixAgrandirCentre(void);    // 0 si niveau max
u32 Commerce_RecetteBoutique(u32 jour);   // recette d'un jour donné
u32 Commerce_RecetteMoyenneBoutique(void);
bool32 Commerce_OuvrirBoutique(u32 ville, u32 stock);
bool32 Commerce_OuvrirCentre(void);
bool32 Commerce_AgrandirBoutique(void);
bool32 Commerce_AgrandirCentre(void);
void Commerce_ChoisirStock(u32 stock);
void Commerce_ChoisirPrix(u32 prix);
void Commerce_PasserJour(u32 jour);
void Commerce_MettreAJourJusqua(u32 aujourdhui);
u32 Commerce_Valeur(void);                // investissement, compté dans la valeur de la société
bool32 Commerce_SeanceXP(struct Pokemon *mon);
bool32 Commerce_SeanceEV(struct Pokemon *mon, u32 stat);

// Specials (data/specials.inc)
void Commerce_Ouvrir(void);
void Commerce_Resume(void);
void Commerce_PreparerVilles(void);
void Commerce_ScriptOuvrirBoutique(void);
void Commerce_ScriptOuvrirCentre(void);
void Commerce_ScriptStock(void);
void Commerce_ScriptPrix(void);
void Commerce_ScriptAgrandirBoutique(void);
void Commerce_ScriptAgrandirCentre(void);
void Commerce_ScriptSeanceXP(void);
void Commerce_ScriptSeanceEV(void);
void Commerce_ConcurrentBattu(void);
u16 Commerce_NiveauConcurrent(void);
u16 Commerce_EtatBoutique(void);   // 0 fermée, 1 ouverte, 2 ouverte avec un concurrent
u16 Commerce_EtatCentre(void);     // 0 fermé, 1 ouvert
void Commerce_TexteAgrandir(void); // VAR_0x8005 : 0 boutique, 1 centre ; VAR_RESULT = TRUE si possible

#endif // GUARD_COMMERCES_H
