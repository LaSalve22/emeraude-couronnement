#include "global.h"
#include "coach_ev.h"
#include "event_data.h"
#include "pokemon.h"
#include "string_util.h"
#include "constants/pokemon.h"
#include "constants/characters.h"
#include "constants/items.h"
#include "item.h"
#include "item_menu.h"

// Émeraude Couronnement : Coach EV. Après la Ligue, règle les EV d'un Pokémon stat par stat
// (252, 128, 4 ou 0), dans la limite de 252 par stat et 510 au total. Gratuit.

static const u8 sStatEv[NUM_STATS] =
{
    MON_DATA_HP_EV, MON_DATA_ATK_EV, MON_DATA_DEF_EV, MON_DATA_SPATK_EV, MON_DATA_SPDEF_EV, MON_DATA_SPEED_EV,
};

static const u8 sValeurs[] = {MAX_PER_STAT_EVS, 128, 4, 0};

static const u8 sText_PV[] = _("PV ");
static const u8 sText_Atq[] = _("  ATQ ");
static const u8 sText_Def[] = _("  DÉF ");
static const u8 sText_AtqSpe[] = _("\nA.SPÉ ");
static const u8 sText_DefSpe[] = _("  D.SPÉ ");
static const u8 sText_Vit[] = _("  VIT ");
static const u8 sText_Total[] = _("\pTotal : ");
static const u8 sText_Sur510[] = _(" / 510. Je change quoi ?");

static const u8 *const sLibelles[NUM_STATS] = {sText_PV, sText_Atq, sText_Def, sText_AtqSpe, sText_DefSpe, sText_Vit};

u32 CoachEV_Total(struct Pokemon *mon)
{
    u32 i, total = 0;
    for (i = 0; i < NUM_STATS; i++)
        total += GetMonData(mon, sStatEv[i]);
    return total;
}

u32 CoachEV_Regler(struct Pokemon *mon, u32 stat, u32 valeur)
{
    u32 actuel, place;
    u8 ev;

    if (stat >= NUM_STATS)
        return 0;
    actuel = GetMonData(mon, sStatEv[stat]);
    place = MAX_TOTAL_EVS - (CoachEV_Total(mon) - actuel);
    if (valeur > place)
        valeur = place;
    if (valeur > MAX_PER_STAT_EVS)
        valeur = MAX_PER_STAT_EVS;
    ev = valeur;
    SetMonData(mon, sStatEv[stat], &ev);
    CalculateMonStats(mon);
    return valeur;
}

void CoachEV_ToutRemettre(struct Pokemon *mon)
{
    u32 i;
    u8 zero = 0;
    for (i = 0; i < NUM_STATS; i++)
        SetMonData(mon, sStatEv[i], &zero);
    CalculateMonStats(mon);
}

void CoachEV_Afficher(void)
{
    struct Pokemon *mon = &gPlayerParty[gSpecialVar_0x8004];
    u32 i;

    gStringVar4[0] = EOS;
    for (i = 0; i < NUM_STATS; i++)
    {
        StringAppend(gStringVar4, sLibelles[i]);
        ConvertIntToDecimalStringN(gStringVar4 + StringLength(gStringVar4), GetMonData(mon, sStatEv[i]), STR_CONV_MODE_LEFT_ALIGN, 3);
    }
    StringAppend(gStringVar4, sText_Total);
    ConvertIntToDecimalStringN(gStringVar4 + StringLength(gStringVar4), CoachEV_Total(mon), STR_CONV_MODE_LEFT_ALIGN, 3);
    StringAppend(gStringVar4, sText_Sur510);
}

// VAR_0x8004 : Pokémon, VAR_0x8005 : stat (ou COACH_EV_TOUT_A_ZERO), VAR_0x8006 : choix de valeur.
// VAR_RESULT = TRUE si la valeur a été réduite faute de place.
void CoachEV_Appliquer(void)
{
    struct Pokemon *mon = &gPlayerParty[gSpecialVar_0x8004];
    u32 voulu;

    gSpecialVar_Result = FALSE;
    if (gSpecialVar_0x8005 == COACH_EV_TOUT_A_ZERO)
    {
        CoachEV_ToutRemettre(mon);
        return;
    }
    if (gSpecialVar_0x8006 >= ARRAY_COUNT(sValeurs))
        return;
    voulu = sValeurs[gSpecialVar_0x8006];
    gSpecialVar_Result = (CoachEV_Regler(mon, gSpecialVar_0x8005, voulu) < voulu);
}

// ---------------------------------------------------------------- IV (v0.32)
// Coach IV (après la Ligue) : IV réglé de 0 à 31, stat par stat, gratuit.
// Capsule d'Argent : une stat à 31 ; Capsule d'Or : les 6. Vendues par le Méga-Collectionneur dès le 6e badge.
// On écrit l'IV réel (visible dans le résumé, compte pour Puissance Cachée) et on retire l'Entraînement Ultime éventuel.

static const u8 sStatIv[NUM_STATS] =
{
    MON_DATA_HP_IV, MON_DATA_ATK_IV, MON_DATA_DEF_IV, MON_DATA_SPATK_IV, MON_DATA_SPDEF_IV, MON_DATA_SPEED_IV,
};

static const u8 sStatHyper[NUM_STATS] =
{
    MON_DATA_HYPER_TRAINED_HP, MON_DATA_HYPER_TRAINED_ATK, MON_DATA_HYPER_TRAINED_DEF,
    MON_DATA_HYPER_TRAINED_SPATK, MON_DATA_HYPER_TRAINED_SPDEF, MON_DATA_HYPER_TRAINED_SPEED,
};

static const u8 sText_IvChange[] = _("\pJe change quoi ?");
static const u8 sText_IvQuelle[] = _("\pQuelle stat monter à 31 ?");

static void EcrireIv(struct Pokemon *mon, u32 stat, u32 valeur)
{
    u8 iv = valeur, faux = FALSE;
    SetMonData(mon, sStatIv[stat], &iv);
    SetMonData(mon, sStatHyper[stat], &faux);
}

void CoachIV_Regler(struct Pokemon *mon, u32 stat, u32 valeur)
{
    if (valeur > MAX_PER_STAT_IVS)
        valeur = MAX_PER_STAT_IVS;
    if (stat == COACH_IV_TOUT_A_31)
    {
        for (stat = 0; stat < NUM_STATS; stat++)
            EcrireIv(mon, stat, MAX_PER_STAT_IVS);
    }
    else if (stat < NUM_STATS)
    {
        EcrireIv(mon, stat, valeur);
    }
    CalculateMonStats(mon);
}

// Renvoie le nombre de stats montées à 31 (0 = rien à faire, la Capsule n'est pas consommée).
u32 Capsule_Maximiser(struct Pokemon *mon, u32 stat)
{
    u32 i, n = 0;
    for (i = 0; i < NUM_STATS; i++)
    {
        if (stat != CAPSULE_TOUTES_STATS && i != stat)
            continue;
        if (GetMonData(mon, sStatIv[i]) < MAX_PER_STAT_IVS)
        {
            EcrireIv(mon, i, MAX_PER_STAT_IVS);
            n++;
        }
    }
    if (n)
        CalculateMonStats(mon);
    return n;
}

static void AfficherIv(struct Pokemon *mon, const u8 *fin)
{
    u32 i;
    gStringVar4[0] = EOS;
    for (i = 0; i < NUM_STATS; i++)
    {
        StringAppend(gStringVar4, sLibelles[i]);
        ConvertIntToDecimalStringN(gStringVar4 + StringLength(gStringVar4), GetMonData(mon, sStatIv[i]), STR_CONV_MODE_LEFT_ALIGN, 2);
    }
    StringAppend(gStringVar4, fin);
}

// VAR_0x8004 : Pokémon. Affiche ses IV ; la fin du texte dépend de l'appelant (VAR_0x8007 = 1 : Capsule).
void CoachIV_Afficher(void)
{
    AfficherIv(&gPlayerParty[gSpecialVar_0x8004], gSpecialVar_0x8007 == 1 ? sText_IvQuelle : sText_IvChange);
}

// VAR_0x8004 : Pokémon, VAR_0x8005 : stat (ou COACH_IV_TOUT_A_31), VAR_0x8006 : index du choix (0 = 31 ... 31 = 0).
void CoachIV_Appliquer(void)
{
    if (gSpecialVar_0x8006 > MAX_PER_STAT_IVS)
        gSpecialVar_0x8006 = 0;
    CoachIV_Regler(&gPlayerParty[gSpecialVar_0x8004], gSpecialVar_0x8005, MAX_PER_STAT_IVS - gSpecialVar_0x8006);
}

u16 Capsule_EstOr(void)
{
    return gSpecialVar_ItemId == ITEM_GOLD_BOTTLE_CAP;
}

// VAR_0x8004 : Pokémon, VAR_0x8005 : stat (CAPSULE_TOUTES_STATS pour l'Or). Consomme la Capsule si un IV a changé.
u16 Capsule_Utiliser(void)
{
    u32 stat = Capsule_EstOr() ? CAPSULE_TOUTES_STATS : gSpecialVar_0x8005;
    if (Capsule_Maximiser(&gPlayerParty[gSpecialVar_0x8004], stat) == 0)
        return FALSE;
    RemoveBagItem(gSpecialVar_ItemId, 1);
    return TRUE;
}
