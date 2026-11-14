#ifndef GUARD_IMMOBILIER_H
#define GUARD_IMMOBILIER_H

// Émeraude Couronnement : immobilier (v0.34). Agence dans le menu L (avec le pass) et panneaux « À vendre » devant
// une maison de 12 villes. Données : gSaveBlock1Ptr->immo (struct ImmoSave, include/global.h).

#define IMMO_NOTAIRE_POURCENT    8     // frais à l'achat
#define IMMO_AGENCE_POURCENT     5     // frais à la revente
#define IMMO_BONNE_AFFAIRE       25    // % de remise sur le bien de la semaine
#define IMMO_RATTRAPAGE_MAX      30    // jours rattrapés au maximum
#define IMMO_INDICE_MIN          400
#define IMMO_INDICE_MAX          3000
#define IMMO_SQUAT_POURMILLE     15    // par bien et par jour, tant que la Team Aqua sévit
#define IMMO_IMPAYE_POURMILLE    50
#define IMMO_TRAVAUX_POURMILLE   30
#define IMMO_TRAVAUX_POURCENT    3     // coût des travaux : 3 % du prix de base

// Moteur (testable)
void Immo_Initialiser(u32 jour);
void Immo_PasserJour(u32 jour);
void Immo_MettreAJourJusqua(u32 aujourdhui);
u32 Immo_Prix(u32 bien, u32 jour);          // prix du marché (sans frais)
u32 Immo_LoyerJour(u32 bien);               // loyer moyen par jour
u32 Immo_BonneAffaire(u32 jour);            // bien soldé cette semaine
bool32 Immo_Possede(u32 bien);
bool32 Immo_Acheter(u32 bien, u32 jour);
bool32 Immo_Vendre(u32 bien, u32 jour);
void Immo_Expulser(u32 bien);

// Specials (data/specials.inc)
void Immo_Ouvrir(void);
void Immo_Resume(void);
void Immo_PreparerListe(void);
void Immo_Fiche(void);
void Immo_ScriptAcheter(void);
void Immo_ScriptVendre(void);
void Immo_ScriptExpulser(void);
u16 Immo_NiveauSquatteur(void);

extern u8 gImmoLignes[IMMO_NB_BIENS][32];

#endif // GUARD_IMMOBILIER_H
