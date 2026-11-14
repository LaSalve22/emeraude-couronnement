#ifndef GUARD_SOCIETE_H
#define GUARD_SOCIETE_H

// Émeraude Couronnement : société du joueur (v0.35). Appli « Ma société » du menu L (avec le pass) et ligne dans la Bourse
// une fois cotée. Données : gSaveBlock2Ptr->pokedex.societe (struct SocieteSave, include/global.h).

#define SOCIETE_ETAT_AUCUNE     0
#define SOCIETE_ETAT_CREEE      1
#define SOCIETE_ETAT_COTEE      2

#define SOCIETE_COUT_CREATION   20000
#define SOCIETE_ACTIONS         10000   // nombre total d'actions
#define SOCIETE_MIN_JOUEUR      5100    // le joueur garde toujours 51 % (pas d'OPA hostile)
#define SOCIETE_INTRO_ACTIONS   2500    // actions vendues au public lors de l'introduction en Bourse
#define SOCIETE_INTRO_VALEUR    100000  // valeur minimale pour entrer en Bourse
#define SOCIETE_PAQUET          500     // actions par ordre d'achat / de vente
#define SOCIETE_FRAIS_POURCENT  3
#define SOCIETE_SENTIMENT_MIN   300
#define SOCIETE_SENTIMENT_MAX   3000

// Moteur (testable)
void Societe_Creer(const u8 *nom, u32 jour);
void Societe_MettreAJourJusqua(u32 aujourdhui);
u32 Societe_Valeur(u32 jour);           // biens + trésorerie
u32 Societe_Cours(u32 jour);            // prix d'une action
u32 Societe_RetirerTresorerie(void);    // part du joueur versée, renvoie le montant reçu
bool32 Societe_Introduire(u32 jour);
bool32 Societe_VendreActions(u32 jour);
bool32 Societe_RacheterActions(u32 jour);

// Specials (data/specials.inc)
void Societe_SaisirNom(void);
void Societe_ScriptCreer(void);
void Societe_Resume(void);
void Societe_ScriptRetirer(void);
void Societe_ScriptIntroduire(void);
void Societe_ScriptVendre(void);
void Societe_ScriptRacheter(void);
void Societe_LigneBourse(void);

#endif // GUARD_SOCIETE_H
