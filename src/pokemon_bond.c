#include "global.h"
#include "pokemon.h"
#include "pokemon_bond.h"

// Émeraude Couronnement : liens avec les Pokémon.
// - Lien (amitié >= 150) : +5 % d'XP, +25 % d'EV gagnés.
// - Lien fort (amitié >= 220) : +10 % d'XP, +50 % d'EV, et les EV d'une stat comptent jusqu'à 302 (252 -> 302).
// - Ancienneté : +1 % d'XP tous les 6 K.O. obtenus dans l'équipe (max +10 %, compteur 0-63).

u32 GetMonBondLevel(struct Pokemon *mon)
{
    u32 friendship = GetMonData(mon, MON_DATA_FRIENDSHIP);

    if (GetMonData(mon, MON_DATA_IS_EGG))
        return BOND_NONE;
    if (friendship >= BOND_FRIENDSHIP_STRONG)
        return BOND_STRONG;
    if (friendship >= BOND_FRIENDSHIP_NORMAL)
        return BOND_NORMAL;
    return BOND_NONE;
}

u32 GetMonBondExpBonusPercent(struct Pokemon *mon)
{
    static const u8 sBondExpBonus[] = {[BOND_NONE] = 0, [BOND_NORMAL] = 5, [BOND_STRONG] = 10};
    u32 seniority = GetMonData(mon, MON_DATA_BOND_BATTLES) / 6;

    if (seniority > 10)
        seniority = 10;
    return sBondExpBonus[GetMonBondLevel(mon)] + seniority;
}

u32 GetMonBondEVMultiplierPercent(struct Pokemon *mon)
{
    static const u8 sBondEVMultiplier[] = {[BOND_NONE] = 100, [BOND_NORMAL] = 125, [BOND_STRONG] = 150};
    return sBondEVMultiplier[GetMonBondLevel(mon)];
}

s32 GetBondEffectiveEV(struct Pokemon *mon, s32 ev)
{
    if (GetMonBondLevel(mon) != BOND_STRONG)
        return ev;
    return ev + ev * (BOND_STRONG_MAX_EFFECTIVE_EV - MAX_PER_STAT_EVS) / MAX_PER_STAT_EVS;
}

void IncrementMonBondBattles(struct Pokemon *mon)
{
    u8 battles = GetMonData(mon, MON_DATA_BOND_BATTLES);

    if (battles < 63)
    {
        battles++;
        SetMonData(mon, MON_DATA_BOND_BATTLES, &battles);
    }
}
