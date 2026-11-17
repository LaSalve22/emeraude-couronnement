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
#include "script.h"
#include "string_util.h"
#include "task.h"
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
