#ifndef GUARD_POKEMON_BOND_H
#define GUARD_POKEMON_BOND_H

// Émeraude Couronnement : liens avec les Pokémon (amitié + ancienneté dans l'équipe).
#define BOND_NONE   0
#define BOND_NORMAL 1 // amitié >= BOND_FRIENDSHIP_NORMAL
#define BOND_STRONG 2 // amitié >= BOND_FRIENDSHIP_STRONG

#define BOND_FRIENDSHIP_NORMAL 150
#define BOND_FRIENDSHIP_STRONG 220
#define BOND_STRONG_MAX_EFFECTIVE_EV 302 // une stat à 252 EV compte comme 302 avec un lien fort

struct Pokemon;
u32 GetMonBondLevel(struct Pokemon *mon);
u32 GetMonBondExpBonusPercent(struct Pokemon *mon);
u32 GetMonBondEVMultiplierPercent(struct Pokemon *mon);
s32 GetBondEffectiveEV(struct Pokemon *mon, s32 ev);
void IncrementMonBondBattles(struct Pokemon *mon);

#endif // GUARD_POKEMON_BOND_H
