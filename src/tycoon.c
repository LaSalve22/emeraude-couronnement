#include "global.h"
#include "tycoon.h"
#include "league_club.h"
#include "event_data.h"
#include "field_player_avatar.h"
#include "field_screen_effect.h"
#include "main.h"
#include "overworld.h"
#include "palette.h"
#include "party_menu.h"
#include "pokemon.h"
#include "region_map.h"
#include "immobilier.h"
#include "money.h"
#include "rtc.h"
#include "script.h"
#include "string_util.h"
#include "task.h"
#include "battle_setup.h"
#include "constants/flags.h"
#include "constants/rgb.h"

// Émeraude Couronnement : édition « Carrière & Business » (v0.46), règles de terrain. Voir include/tycoon.h.

#define TOUJOURS       0xFF
#define REGLE_ORIGINE  0xFE   // capacité non concernée (Téléport, Tunnel, Force Cachée...) : règle du jeu de base

bool32 Tycoon_EditionActive(void)
{
    return EDITION_TYCOON_SIM;
}

// Taxis dès le début ; ensuite par tier du club : C Coupe / Flash, B Éclate-Roc / Force, A Plongée / Cascade, S Escalade / Anti-Brume.
u32 Tycoon_TierRequis(u32 fieldMove)
{
    switch (fieldMove)
    {
    case FIELD_MOVE_SURF:
    case FIELD_MOVE_FLY:        return TOUJOURS;
    case FIELD_MOVE_CUT:
    case FIELD_MOVE_FLASH:      return CLUB_TIER_C;
    case FIELD_MOVE_ROCK_SMASH:
    case FIELD_MOVE_STRENGTH:   return CLUB_TIER_B;
    case FIELD_MOVE_DIVE:
    case FIELD_MOVE_WATERFALL:  return CLUB_TIER_A;
    case FIELD_MOVE_ROCK_CLIMB:
    case FIELD_MOVE_DEFOG:      return CLUB_TIER_S;
    default:                    return REGLE_ORIGINE;
    }
}

bool32 Tycoon_CapaciteDebloquee(u32 fieldMove)
{
    u32 tier = Tycoon_TierRequis(fieldMove), club = Club_DuJoueur();
    if (tier == TOUJOURS)
        return TRUE;
    if (tier == REGLE_ORIGINE || club == CLUB_AUCUN)
        return FALSE;
    return Club_Tier(club) >= tier;
}

bool32 Tycoon_CapaciteSansPokemon(u32 fieldMove)
{
    return EDITION_TYCOON_SIM && Tycoon_TierRequis(fieldMove) != REGLE_ORIGINE && Tycoon_CapaciteDebloquee(fieldMove)
        && GetMonData(&gParties[B_TRAINER_PLAYER][0], MON_DATA_SPECIES) != SPECIES_NONE;
}

bool32 Couronnement_ChampionsAtteint(void)
{
    return (gSaveBlock3Ptr->couronnementSaison.paliers & COURONNEMENT_CHAMPIONS_ATTEINT) || gSaveBlock3Ptr->couronnementSaison.titres;
}

// Porte de la Ligue (Éternara) : édition classique = badges (le jeu de base ne vérifie que celui d'Alizée) ;
// Carrière = rang Champions atteint une fois au Couronnement Mondial. VAR_RESULT = la porte s'ouvre.
void Tycoon_LigueOuverte(void)
{
    gSpecialVar_Result = EDITION_TYCOON_SIM ? Couronnement_ChampionsAtteint() : FlagGet(FLAG_BADGE06_GET);
}

void Tycoon_EstActif(void)
{
    gSpecialVar_Result = EDITION_TYCOON_SIM;
}

// ---------------------------------------------------------------- taxis

EWRAM_DATA bool8 gTaxiVolant = FALSE; // la carte de vol s'ouvre depuis le menu L : annuler ramène sur le terrain

static const u16 sVillesTaxi[] =
{
    FLAG_VISITED_LITTLEROOT_TOWN, FLAG_VISITED_PETALBURG_CITY, FLAG_VISITED_RUSTBORO_CITY, FLAG_VISITED_SLATEPORT_CITY,
    FLAG_VISITED_MAUVILLE_CITY, FLAG_VISITED_FALLARBOR_TOWN, FLAG_VISITED_LAVARIDGE_TOWN, FLAG_VISITED_FORTREE_CITY,
    FLAG_VISITED_LILYCOVE_CITY, FLAG_VISITED_MOSSDEEP_CITY, FLAG_VISITED_SOOTOPOLIS_CITY,
};

static void Task_TaxiVolant(u8 taskId)
{
    if (gPaletteFade.active)
        return;
    DestroyTask(taskId);
    CleanupOverworldWindowsAndTilemaps();
    SetMainCallback2(CB2_OpenFlyMap);
}

// Le réseau dessert Bourg-en-Vol et les 10 villes de club dès le premier jour. Le script fait ensuite releaseall / end.
void Tycoon_TaxiVolant(void)
{
    u32 i;
    for (i = 0; i < ARRAY_COUNT(sVillesTaxi); i++)
        FlagSet(sVillesTaxi[i]);
    gPartyMenu.slotId = 0; // Pokémon montré par l'animation de vol
    gTaxiVolant = TRUE;
    BeginNormalPaletteFade(PALETTES_ALL, 0, 0, 16, RGB_BLACK);
    CreateTask(Task_TaxiVolant, 10);
}

// VAR_RESULT = on fait face à une eau où surfer.
void Tycoon_PeutSurfer(void)
{
    gSpecialVar_Result = !TestPlayerAvatarFlags(PLAYER_AVATAR_FLAG_SURFING) && IsPlayerFacingSurfableFishableWater()
                      && GetMonData(&gParties[B_TRAINER_PLAYER][0], MON_DATA_SPECIES) != SPECIES_NONE;
}

// VAR_RESULT = on peut prendre le Taxi Volant ici (en extérieur).
void Tycoon_PeutVoler(void)
{
    gSpecialVar_Result = Overworld_MapTypeAllowsTeleportAndFly(gMapHeader.mapType)
                      && GetMonData(&gParties[B_TRAINER_PLAYER][0], MON_DATA_SPECIES) != SPECIES_NONE;
}

static const u8 *const sNomsCapacites[] =
{
    [FIELD_MOVE_CUT] = COMPOUND_STRING("Coupe"), [FIELD_MOVE_FLASH] = COMPOUND_STRING("Flash"),
    [FIELD_MOVE_ROCK_SMASH] = COMPOUND_STRING("Éclate-Roc"), [FIELD_MOVE_STRENGTH] = COMPOUND_STRING("Force"),
    [FIELD_MOVE_DIVE] = COMPOUND_STRING("Plongée"), [FIELD_MOVE_WATERFALL] = COMPOUND_STRING("Cascade"),
    [FIELD_MOVE_ROCK_CLIMB] = COMPOUND_STRING("Escalade"), [FIELD_MOVE_DEFOG] = COMPOUND_STRING("Anti-Brume"),
};
static const u8 sOrdre[] = {FIELD_MOVE_CUT, FIELD_MOVE_FLASH, FIELD_MOVE_ROCK_SMASH, FIELD_MOVE_STRENGTH,
                            FIELD_MOVE_DIVE, FIELD_MOVE_WATERFALL, FIELD_MOVE_ROCK_CLIMB, FIELD_MOVE_DEFOG};
static const u8 sText_Cap1[] = _("Taxis Volant et Surfant : toujours.\nAvec ton club : ");
static const u8 sText_CapAucune[] = _("rien encore.");
static const u8 sText_CapSuite[] = _(".\pTier C : Coupe, Flash. B : Éclate-Roc,\nForce. A : Plongée, Cascade.\lS : Escalade, Anti-Brume.\pLes obstacles se franchissent sans\nqu'un Pokémon connaisse la capacité.");
static const u8 sText_Virgule[] = _(", ");

void Tycoon_TexteCapacites(void)
{
    u32 i, n = 0;
    u8 *fin = StringCopy(gStringVar4, sText_Cap1);
    for (i = 0; i < ARRAY_COUNT(sOrdre); i++)
    {
        if (!Tycoon_CapaciteDebloquee(sOrdre[i]))
            continue;
        if (n++)
            fin = StringCopy(fin, n == 3 || n == 6 ? (const u8 *)COMPOUND_STRING(",\l") : sText_Virgule);
        fin = StringCopy(fin, sNomsCapacites[sOrdre[i]]);
    }
    if (n == 0)
        fin = StringCopy(fin, sText_CapAucune);
    StringCopy(fin, sText_CapSuite);
}

// ---------------------------------------------------------------- intro adulte et loyer (v0.47)

#define TY (gSaveBlock3Ptr->tycoon)
#define TYCOON_LOYER        2500
#define TYCOON_JOURS_LOYER  30

static const u32 sArgentProfil[3] = {12000, 8000, 6000};
static const u8 *const sNomsProfils[3] =
{
    COMPOUND_STRING("Épargnant"), COMPOUND_STRING("Trader"), COMPOUND_STRING("Espoir Sportif"),
};
static const u8 sText_Profil1[] = _("Profil ");
static const u8 sText_Profil2[] = _(" validé.\n");
static const u8 sText_Profil3[] = _(" ¥ sur ton compte.");

static u8 *Nombre(u8 *fin, u32 n)
{
    u32 chiffres = 1;
    u64 p = 10;
    while (n >= p && chiffres < 10)
    {
        chiffres++;
        p *= 10;
    }
    return ConvertIntToDecimalStringN(fin, n, STR_CONV_MODE_LEFT_ALIGN, chiffres);
}

// VAR_0x8004 = profil (0 Épargnant, 1 Trader, 2 Espoir Sportif). Les Bonbons Exp. S de l'Espoir sont donnés par le script.
void Tycoon_ChoisirProfil(void)
{
    u32 p = gSpecialVar_0x8004 % 3;
    u8 *fin;
    memset(&TY, 0, sizeof(TY));
    TY.magie = TYCOON_MAGIE;
    TY.profil = p;
    TY.jourLoyer = RtcGetLocalDayCount();
    SetMoney(&gSaveBlock1Ptr->money, sArgentProfil[p]);
    fin = StringCopy(gStringVar4, sText_Profil1);
    fin = StringCopy(fin, sNomsProfils[p]);
    fin = StringCopy(fin, sText_Profil2);
    fin = Nombre(fin, sArgentProfil[p]);
    StringCopy(fin, sText_Profil3);
}

u32 Tycoon_Profil(void)
{
    return TY.magie == TYCOON_MAGIE ? TY.profil : 0;
}

static bool32 ProprietaireDUneMaison(void)
{
    u32 i;
    for (i = 0; i < IMMO_NB_BIENS; i++)
        if (Immo_Possede(i))
            return TRUE;
    return FALSE;
}

static const u8 sText_Loyer1[] = _("Le propriétaire est passé : loyer\nde ");
static const u8 sText_Loyer2[] = _(" mois, ");
static const u8 sText_Loyer3[] = _(" ¥ payés.");
static const u8 sText_LoyerImpaye[] = _("\pIl manquait de l'argent : impayé\nnoté sur ton dossier.");

// Loyer de l'appartement tous les 30 jours tant que le joueur n'a pas de maison (édition Carrière). VAR_RESULT = rapport.
void Tycoon_Loyer(void)
{
    u32 aujourdhui = RtcGetLocalDayCount(), mois, du, paye;
    u8 *fin;

    gSpecialVar_Result = FALSE;
    if (!EDITION_TYCOON_SIM || TY.magie != TYCOON_MAGIE || aujourdhui < TY.jourLoyer + TYCOON_JOURS_LOYER)
        return;
    mois = (aujourdhui - TY.jourLoyer) / TYCOON_JOURS_LOYER;
    TY.jourLoyer += mois * TYCOON_JOURS_LOYER;
    if (ProprietaireDUneMaison())
        return;
    if (mois > 12)
        mois = 12;
    du = mois * TYCOON_LOYER;
    paye = min(du, GetMoney(&gSaveBlock1Ptr->money));
    RemoveMoney(&gSaveBlock1Ptr->money, paye);
    fin = StringCopy(gStringVar4, sText_Loyer1);
    fin = Nombre(fin, mois);
    fin = StringCopy(fin, sText_Loyer2);
    fin = Nombre(fin, paye);
    fin = StringCopy(fin, sText_Loyer3);
    if (paye < du)
    {
        if (TY.impayes < 0xFF)
            TY.impayes++;
        StringCopy(fin, sText_LoyerImpaye);
    }
    gSpecialVar_Result = TRUE;
}

// ---------------------------------------------------------------- sièges des clubs (v0.48)

// Édition Carrière : le champion d'arène est le président du club de sa ville. Lui parler la première fois remet
// l'accréditation (tout ce que donnait la victoire : badge, CT, objets, suite du scénario) sans combat.
// VAR_0x8004 = dresseur du champion. VAR_RESULT = l'accréditation est à remettre (le drapeau du dresseur est posé).
void Tycoon_Accreditation(void)
{
    gSpecialVar_Result = FALSE;
    if (!EDITION_TYCOON_SIM || HasTrainerBeenFought(gSpecialVar_0x8004))
        return;
    SetTrainerFlag(gSpecialVar_0x8004);
    gSpecialVar_Result = TRUE;
}
