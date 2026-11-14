#ifndef GUARD_BANQUES_H
#define GUARD_BANQUES_H

// Émeraude Couronnement : banques (v0.37). Appli « Banques » du menu L (avec le pass).
// Trois prêteurs avec leur taux, leur plafond et leur sanction en cas de défaut ; échéance chaque lundi.

enum Banque
{
    BANQUE_DEVON,    // Banque Devon (Mérouville) : 0,3 %/jour, plafond selon les badges, défaut = +10 % de pénalité
    BANQUE_FONCIER,  // Crédit Foncier de Nénucrique : 0,5 %/jour, plafond = 60 % des biens et commerces, défaut = saisie d'un bien
    BANQUE_CASINO,   // Prêteur du Casino (Lavandia) : 2 %/jour, sans garantie, défaut = saisie du liquide + 20 %
    BANQUE_NB,
};

#define BANQUE_ECHEANCE_POURCENT 5      // part de la dette prélevée chaque lundi
#define BANQUE_ECHEANCE_MIN      500
#define BANQUE_RATTRAPAGE_MAX    30

// Moteur (testable)
void Banque_MettreAJourJusqua(u32 aujourdhui);
void Banque_PasserJour(u32 jour);
u32 Banque_Dette(u32 banque);
u32 Banque_Plafond(u32 banque);
u32 Banque_Emprunter(u32 banque, u32 montant);   // renvoie le montant prêté
u32 Banque_Rembourser(u32 banque);               // renvoie le montant remboursé

// Specials (data/specials.inc)
void Banque_Ouvrir(void);
void Banque_Resume(void);
void Banque_Fiche(void);
void Banque_ScriptEmprunter(void);
void Banque_ScriptRembourser(void);

#endif // GUARD_BANQUES_H
