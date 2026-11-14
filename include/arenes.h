#ifndef GUARD_ARENES_H
#define GUARD_ARENES_H

// Émeraude Couronnement : arènes du joueur (v0.40), après la Ligue.
// 6 villes sans arène ; type au choix parmi ceux qu'aucun Champion n'utilise ; 6 titulaires + 3 remplaçants déposés
// (rangés dans le PC avec MON_DATA_ARENE, bloqués) ; challengers de 1 à 8 badges joués par le joueur.

#define ARENE_NB_TYPES        10
#define ARENE_PRIX            500000
#define ARENE_TITULAIRES      6
#define ARENE_REMPLACANTS     3
#define ARENE_PLACES          (ARENE_TITULAIRES + ARENE_REMPLACANTS)
#define ARENE_MIN_DU_TYPE     4       // titulaires du type de l'arène pour accueillir des challengers
#define ARENE_ATTENTE_MAX     3
#define ARENE_REPOS_DEFAITE   2       // jours sans challenger après une défaite
#define ARENE_REPUTATION_DEPART 20
#define ARENE_SPONSORS_PAR_POINT 150  // ¥ par jour et par point de réputation
#define ARENE_PRIME_BADGE     5000    // payée par badge du challenger quand il gagne
#define ARENE_BONUS_BADGE     2000    // sponsors par badge du challenger battu
#define ARENE_RATTRAPAGE_MAX  30

struct BoxPokemon;
struct Trainer;

// Moteur (testable)
void Arene_MettreAJourJusqua(u32 aujourdhui);
bool32 Arene_Achetee(u32 arene);
u32 Arene_Type(u32 arene);                       // type de combat (TYPE_*)
u32 Arene_Pokemon(u32 arene, struct BoxPokemon **places); // remplit places[ARENE_PLACES], renvoie le nombre
bool32 Arene_Valide(u32 arene);                  // assez de titulaires du type
bool32 Arene_Acheter(u32 arene, u32 typeIndex);
bool32 Arene_Deposer(u32 arene, u32 partySlot, bool32 titulaire);
bool32 Arene_Retirer(u32 arene, u32 place);
u32 Arene_Valeur(void);                          // compté dans la valeur de la société
u32 Arene_BadgesChallenger(u32 arene);          // badges du prochain challenger (0 si personne)
bool32 Arene_EstBloque(struct BoxPokemon *boxMon); // PC : Pokémon déposé dans une arène
bool32 Arene_PokemonDansLePC(void);
bool32 Arene_CombatEnCours(void);               // pas d'XP pour l'équipe prêtée
const struct Trainer *Arene_Challenger(void);

// Specials (data/specials.inc), VAR_0x8008 = arène
void Arene_Ouvrir(void);
void Arene_ScriptAcheter(void);
void Arene_PreparerChallenger(void);
void Arene_DebutCombat(void);
void Arene_FinCombat(void);
void Arene_ScriptDeposer(void);
void Arene_PreparerListe(void);
void Arene_ScriptRetirer(void);
void Arene_Resume(void);

extern u8 gAreneLignes[ARENE_PLACES][24];

#endif // GUARD_ARENES_H
