#ifndef GUARD_BOURSE_H
#define GUARD_BOURSE_H

// Émeraude Couronnement : Bourse de Hoenn (v0.33), appli du menu L dès que le rival a remis le pass.
// Données : gSaveBlock2Ptr->pokedex.bourse (struct BourseSave, include/global.h).

enum BourseEntreprise
{
    BOURSE_DEVON,
    BOURSE_STERN,
    BOURSE_CASINO,
    BOURSE_TELE,
    BOURSE_MAGASINS,
    BOURSE_AQUA,
    BOURSE_MAGMA,
    BOURSE_PENSION,
    BOURSE_SAFARI,
};

#define BOURSE_FRAIS_POURCENT     3       // frais à l'achat et à la vente
#define BOURSE_OR                 BOURSE_NB_ENTREPRISES  // v0.41 : l'or XAU/¥ (index après les 9 entreprises)
#define BOURSE_NB_ACTIFS          (BOURSE_NB_ENTREPRISES + 1)
#define BOURSE_OR_DEPART          20000   // ¥ par once
#define BOURSE_OR_MIN             2000
#define BOURSE_OR_MAX             2000000
#define BOURSE_OR_FRAIS_POURCENT  1
#define BOURSE_PARTS_MAX          9999    // parts maximum par entreprise
#define BOURSE_COURS_MIN          2
#define BOURSE_COURS_MAX          60000
#define BOURSE_RATTRAPAGE_MAX     30      // jours rattrapés au maximum à l'ouverture
#define BOURSE_INTERET_POURMILLE  10      // intérêts du crédit : 1 % par jour
#define BOURSE_CREDIT_BASE        10000   // plafond du crédit : 10 000 + moitié du portefeuille
#define BOURSE_CREDIT_MAX         5000000
#define BOURSE_HUISSIER_MARGE     10000   // l'huissier passe si dette > portefeuille + 10 000
#define BOURSE_HUISSIER_PENALITE  10      // % retenu sur les parts vendues de force
#define BOURSE_DIVIDENDE_POURMILLE 20     // Grands Magasins : 2 % du cours par part et par semaine
#define BOURSE_TUYAU_FIABLE       70      // % de rumeurs vraies

// Moteur (testable)
void Bourse_Initialiser(u32 jour);
s32 Bourse_VariationDuJour(u32 entreprise, u32 jour); // en pour mille, sans les chocs du scénario
s32 Bourse_VariationOr(u32 jour);                     // v0.41, en pour mille
u32 Bourse_Cours(u32 actif);                          // entreprise ou BOURSE_OR
u32 Bourse_Parts(u32 actif);
void Bourse_PasserJour(u32 jour);
void Bourse_AppliquerChocs(void);
void Bourse_VerifierHuissier(void);
void Bourse_MettreAJourJusqua(u32 aujourdhui);
u32 Bourse_ValeurPortefeuille(void);
u32 Bourse_PlafondCredit(void);
u32 Bourse_Acheter(u32 entreprise, u32 quantite); // renvoie le nombre de parts achetées
u32 Bourse_Vendre(u32 entreprise, u32 quantite);  // renvoie le nombre de parts vendues
u32 Bourse_Emprunter(u32 montant);                // renvoie le montant emprunté
u32 Bourse_Rembourser(void);                      // renvoie le montant remboursé

// Historique et graphique (v0.38)
struct BourseBougie
{
    u32 ouverture, haut, bas, cloture;
};
void Bourse_HistoriqueInitialiser(u32 jour);
u32 Bourse_HistoriqueCloture(u32 entreprise, u32 age); // âge 0 = aujourd'hui, 0 si pas d'historique
void Bourse_Bougie(u32 entreprise, u32 age, struct BourseBougie *bougie);
u8 *Bourse_TexteEntete(u8 *dest, u32 entreprise);
void Bourse_GraphiqueDessiner(u8 windowId, u32 entreprise, u32 mode); // mode 0 = bougies, 1 = courbe
void Bourse_Graphique(void); // special + waitstate, VAR_0x8004 = entreprise (src/bourse_graphique.c)

// Specials (data/specials.inc)
void Bourse_Ouvrir(void);
void Bourse_Resume(void);
void Bourse_PreparerListe(void);
void Bourse_Fiche(void);
void Bourse_ScriptAcheter(void);
void Bourse_ScriptVendre(void);
void Bourse_Credit(void);
void Bourse_ScriptEmprunter(void);
void Bourse_ScriptRembourser(void);
void Bourse_Tuyau(void);

extern u8 gBourseLignes[BOURSE_NB_ENTREPRISES + 2][32]; // + la société du joueur (v0.35) + l'or (v0.41)

#endif // GUARD_BOURSE_H
