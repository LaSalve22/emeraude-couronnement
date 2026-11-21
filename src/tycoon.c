#include "global.h"
#include "pokedex.h"
#include "constants/items.h"
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
#include "data.h"
#include "random.h"
#include "constants/trainers.h"
#include "battle_setup.h"
#include "constants/flags.h"
#include "constants/maps.h"
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

// Boutique Pro (v0.52) : 0 = Bonbons Exp. XS / S / M + Fil de Liaison, 1 = + L (tier B), 2 = + XL (tier A, S ou propriétaire).
void Tycoon_BoutiqueNiveau(void)
{
    u32 niveau = Tycoon_NiveauCible();
    gSpecialVar_Result = niveau >= TYCOON_NIVEAU_A ? 2 : (niveau >= TYCOON_NIVEAU_B ? 1 : 0);
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

// ---------------------------------------------------------------- niveaux par tier (v0.50)

// Édition Carrière : pas de badges ni de level cap, ce sont les Pokémon sauvages et les dresseurs qui suivent la carrière du
// joueur : N.10 sans club ou en tier C la 1re saison, N.18 en tier C ensuite, N.30 en B, N.44 en A, N.60 en S ou propriétaire.
// Les espèces évoluent au besoin (Club_EvoluerPourNiveau).
u32 Tycoon_NiveauCible(void)
{
    u32 club = Club_DuJoueur();
    if (Club_Proprio() != CLUB_AUCUN)
        return TYCOON_NIVEAU_S;
    if (club == CLUB_AUCUN)
        return TYCOON_NIVEAU_DEPART;
    switch (Club_Tier(club))
    {
    case CLUB_TIER_C: // v0.55 : N.10 seulement jusqu'à la 1re journée de la 1re saison
        return (Club_Saison() == 0 && Club_JourDeSaison() < Club_JourDeJournee(0)) ? TYCOON_NIVEAU_DEPART : TYCOON_NIVEAU_C;
    case CLUB_TIER_B: return TYCOON_NIVEAU_B;
    case CLUB_TIER_A: return TYCOON_NIVEAU_A;
    default:          return TYCOON_NIVEAU_S;
    }
}

// Pokémon sauvage : niveau cible ±2, espèce évoluée en conséquence.
void Tycoon_AjusterSauvage(u16 *species, u8 *niveau)
{
    s32 n;
    if (!EDITION_TYCOON_SIM)
        return;
    n = (s32)Tycoon_NiveauCible() - 2 + (s32)(Random() % 5);
    *niveau = n < 2 ? 2 : (n > MAX_LEVEL ? MAX_LEVEL : n);
    *species = Club_EvoluerPourNiveau(*species, *niveau);
}

#define TYCOON_MAX_MONS 32
static EWRAM_DATA struct Trainer sDresseurAjuste = {0};
static EWRAM_DATA struct TrainerMon sEquipeAjustee[TYCOON_MAX_MONS] = {0};

static u32 BonusDeClasse(u32 classe)
{
    switch (classe)
    {
    case TRAINER_CLASS_ELITE_FOUR:
    case TRAINER_CLASS_CHAMPION:     return 5;
    case TRAINER_CLASS_LEADER:
    case TRAINER_CLASS_AQUA_LEADER:
    case TRAINER_CLASS_MAGMA_LEADER:
    case TRAINER_CLASS_RIVAL:
    case TRAINER_CLASS_AQUA_ADMIN:
    case TRAINER_CLASS_MAGMA_ADMIN:  return 3;
    default:                         return 0;
    }
}

// Dresseur : son meilleur Pokémon passe au niveau cible (+3 pour les chefs, +5 pour le Conseil 4 et le Maître), les autres
// gardent leur écart avec lui (5 niveaux au plus). Renvoie une copie ajustée, ou le dresseur tel quel hors édition Carrière.
const struct Trainer *Tycoon_AjusterDresseur(const struct Trainer *dresseur)
{
    u32 i, n, maxi = 0, cible;
    if (!EDITION_TYCOON_SIM || dresseur->party == NULL)
        return dresseur;
    n = max(dresseur->partySize, dresseur->poolSize);
    if (n == 0 || n > TYCOON_MAX_MONS)
        return dresseur;
    sDresseurAjuste = *dresseur;
    memcpy(sEquipeAjustee, dresseur->party, n * sizeof(struct TrainerMon));
    for (i = 0; i < n; i++)
        maxi = max(maxi, sEquipeAjustee[i].lvl);
    cible = Tycoon_NiveauCible() + BonusDeClasse(dresseur->trainerClass);
    for (i = 0; i < n; i++)
    {
        s32 lvl = (s32)cible - (s32)min(maxi - sEquipeAjustee[i].lvl, 5);
        lvl = lvl < 2 ? 2 : (lvl > MAX_LEVEL ? MAX_LEVEL : lvl);
        sEquipeAjustee[i].lvl = lvl;
        sEquipeAjustee[i].species = Club_EvoluerPourNiveau(sEquipeAjustee[i].species, lvl);
    }
    sDresseurAjuste.party = sEquipeAjustee;
    return &sDresseurAjuste;
}

// ---------------------------------------------------------------- Arènes fermées (v0.53, choix d'Adrien)
// Édition Carrière : les Arènes sont fermées (sièges des clubs), on ne combat plus les champions. Les badges restent des drapeaux
// internes (« accréditations ») posés automatiquement, sans message, dans l'ordre et aux moments où le scénario d'origine les
// donnerait : les systèmes qui comptent les badges et les suites du scénario (Centre Spatial, Atalanopolis…) continuent de marcher.
// Les objets des champions arrivent par courrier à l'ouverture du menu L (Tycoon_EventScript_Courrier).
static bool32 AccreditationDue(u32 n)
{
    if (VarGet(VAR_PETALBURG_GYM_STATE) < 2) // tutoriel de Timmy pas encore fait
        return FALSE;
    if (n > 1 && !FlagGet(FLAG_BADGE01_GET + n - 2))
        return FALSE;
    switch (n)
    {
    case 1: return FlagGet(FLAG_VISITED_RUSTBORO_CITY);
    case 2: return FlagGet(FLAG_VISITED_DEWFORD_TOWN);
    case 3: return FlagGet(FLAG_VISITED_MAUVILLE_CITY);
    case 4: return FlagGet(FLAG_VISITED_LAVARIDGE_TOWN) && FlagGet(FLAG_DEFEATED_EVIL_TEAM_MT_CHIMNEY);
    case 5: return VarGet(VAR_PETALBURG_GYM_STATE) >= 6;
    case 6: return FlagGet(FLAG_VISITED_FORTREE_CITY) && VarGet(VAR_WEATHER_INSTITUTE_STATE) != 0;
    case 7: return FlagGet(FLAG_VISITED_MOSSDEEP_CITY) && FlagGet(FLAG_TEAM_AQUA_ESCAPED_IN_SUBMARINE);
    case 8: return VarGet(VAR_SOOTOPOLIS_CITY_STATE) >= 5;
    }
    return FALSE;
}

static void PetalburgUnBadgeDePlus(void)
{
    u32 etat = VarGet(VAR_PETALBURG_GYM_STATE);
    if (etat >= 2 && etat < 6)
        VarSet(VAR_PETALBURG_GYM_STATE, ++etat);
    if (etat == 6)
    {
        FlagClear(FLAG_HIDE_PETALBURG_GYM_GREETER);
        FlagSet(FLAG_PETALBURG_MART_EXPANDED_ITEMS);
    }
}

static void Accrediter(u32 n)
{
    FlagSet(FLAG_BADGE01_GET + n - 1);
    switch (n)
    {
    case 1:
        FlagSet(FLAG_DEFEATED_RUSTBORO_GYM);
        if (VarGet(VAR_RUSTBORO_CITY_STATE) == 0)
            VarSet(VAR_RUSTBORO_CITY_STATE, 1);
        PetalburgUnBadgeDePlus();
        break;
    case 2:
        FlagSet(FLAG_DEFEATED_DEWFORD_GYM);
        PetalburgUnBadgeDePlus();
        FlagSet(FLAG_ENABLE_BRAWLY_MATCH_CALL);
        break;
    case 3:
        if (VarGet(VAR_SLATEPORT_OUTSIDE_MUSEUM_STATE) < 3)
            VarSet(VAR_SLATEPORT_OUTSIDE_MUSEUM_STATE, 3);
        FlagClear(FLAG_HIDE_VERDANTURF_TOWN_SCOTT);
        FlagSet(FLAG_DEFEATED_MAUVILLE_GYM);
        PetalburgUnBadgeDePlus();
        FlagSet(FLAG_ENABLE_WATTSON_MATCH_CALL);
        break;
    case 4:
        FlagSet(FLAG_DEFEATED_LAVARIDGE_GYM);
        PetalburgUnBadgeDePlus();
        FlagSet(FLAG_HIDE_VERDANTURF_TOWN_WANDAS_HOUSE_WALLY);
        if (VarGet(VAR_LAVARIDGE_TOWN_STATE) == 0)
            VarSet(VAR_LAVARIDGE_TOWN_STATE, 1);
        FlagSet(FLAG_ENABLE_FLANNERY_MATCH_CALL);
        break;
    case 5: // Norman, puis la scène du père de Timmy (Timmy part pour Vergazon)
        FlagSet(FLAG_DEFEATED_PETALBURG_GYM);
        VarSet(VAR_PETALBURG_GYM_STATE, 7);
        FlagSet(FLAG_HIDE_MR_BRINEY_DEWFORD_TOWN);
        FlagSet(FLAG_HIDE_MR_BRINEY_BOAT_DEWFORD_TOWN);
        FlagSet(FLAG_HIDE_ROUTE_109_MR_BRINEY);
        FlagSet(FLAG_HIDE_ROUTE_109_MR_BRINEY_BOAT);
        FlagSet(FLAG_HIDE_ROUTE_104_MR_BRINEY);
        FlagSet(FLAG_HIDE_ROUTE_104_MR_BRINEY_BOAT);
        FlagSet(FLAG_HIDE_BRINEYS_HOUSE_MR_BRINEY);
        FlagSet(FLAG_HIDE_BRINEYS_HOUSE_PEEKO);
        VarSet(VAR_BRINEY_LOCATION, 0);
        FlagSet(FLAG_HIDE_MAUVILLE_GYM_WATTSON);
        FlagClear(FLAG_HIDE_MAUVILLE_CITY_WATTSON);
        FlagClear(FLAG_HIDE_DEWFORD_HALL_SLUDGE_BOMB_MAN);
        if (VarGet(VAR_PETALBURG_CITY_STATE) < 4)
            VarSet(VAR_PETALBURG_CITY_STATE, 4);
        FlagClear(FLAG_HIDE_PETALBURG_CITY_WALLYS_DAD);
        break;
    case 6:
        FlagSet(FLAG_DEFEATED_FORTREE_GYM);
        FlagSet(FLAG_ENABLE_WINONA_MATCH_CALL);
        VarSet(VAR_SCOTT_FORTREE_CALL_STEP_COUNTER, 0);
        FlagSet(FLAG_SCOTT_CALL_FORTREE_GYM);
        break;
    case 7:
        FlagSet(FLAG_DEFEATED_MOSSDEEP_GYM);
        FlagSet(FLAG_HIDE_AQUA_HIDEOUT_GRUNTS);
        FlagClear(FLAG_HIDE_SLATEPORT_CITY_STERNS_SHIPYARD_MR_BRINEY);
        FlagClear(FLAG_UNKNOWN_0x393);
        FlagClear(FLAG_HIDE_SLATEPORT_CITY_HARBOR_PATRONS);
        FlagClear(FLAG_HIDE_MOSSDEEP_CITY_TEAM_MAGMA);
        FlagClear(FLAG_HIDE_MOSSDEEP_CITY_SPACE_CENTER_1F_TEAM_MAGMA);
        FlagClear(FLAG_HIDE_MOSSDEEP_CITY_SPACE_CENTER_2F_TEAM_MAGMA);
        FlagClear(FLAG_HIDE_MOSSDEEP_CITY_SPACE_CENTER_2F_STEVEN);
        VarSet(VAR_MOSSDEEP_CITY_STATE, 1);
        FlagSet(FLAG_HIDE_MOSSDEEP_CITY_SPACE_CENTER_1F_STEVEN);
        VarSet(VAR_MOSSDEEP_SPACE_CENTER_STATE, 1);
        FlagSet(FLAG_ENABLE_TATE_AND_LIZA_MATCH_CALL);
        break;
    case 8:
        FlagSet(FLAG_DEFEATED_SOOTOPOLIS_GYM);
        FlagSet(FLAG_HIDE_SOOTOPOLIS_CITY_RESIDENTS);
        FlagSet(FLAG_HIDE_SOOTOPOLIS_CITY_STEVEN);
        FlagSet(FLAG_HIDE_SOOTOPOLIS_CITY_WALLACE);
        VarSet(VAR_SOOTOPOLIS_CITY_STATE, 6);
        FlagClear(FLAG_HIDE_SOOTOPOLIS_CITY_MAN_1);
        FlagSet(FLAG_ENABLE_JUAN_MATCH_CALL);
        break;
    }
}

// Appelé à chaque changement de carte (src/overworld.c), avant les scripts de transition de la carte.
void Tycoon_AccreditationsAuto(void)
{
    u32 n;
    if (!EDITION_TYCOON_SIM)
        return;
    for (n = 1; n <= 8; n++)
    {
        if (FlagGet(FLAG_BADGE01_GET + n - 1))
            continue;
        if (!AccreditationDue(n))
            break;
        Accrediter(n);
    }
}

// Special : 1 si l'Arène de la carte actuelle est fermée (édition Carrière ; Clémenti reste ouverte pour le tutoriel de Timmy).
void Tycoon_AreneFermee(void)
{
    gSpecialVar_Result = EDITION_TYCOON_SIM
        && !(gSaveBlock1Ptr->location.mapGroup == MAP_GROUP(MAP_PETALBURG_CITY_GYM)
             && gSaveBlock1Ptr->location.mapNum == MAP_NUM(MAP_PETALBURG_CITY_GYM)
             && VarGet(VAR_PETALBURG_GYM_STATE) < 2);
}

// Special : ressort de l'Arène par le passage sur lequel le joueur vient d'arriver (la porte de la ville). Suivi de waitstate.
void Tycoon_SortirArene(void)
{
    const struct MapEvents *ev = gMapHeader.events;
    s16 x = gSaveBlock1Ptr->pos.x, y = gSaveBlock1Ptr->pos.y;
    u32 i;
    for (i = 0; i < ev->warpCount; i++)
    {
        if (ev->warps[i].x == x && ev->warps[i].y == y)
        {
            SetWarpDestinationToMapWarp(ev->warps[i].mapGroup, ev->warps[i].mapNum, ev->warps[i].warpId);
            DoWarp();
            ResetInitialPlayerAvatarState();
            return;
        }
    }
    ScriptContext_Enable(); // pas de passage trouvé : on laisse le joueur sur place
}

// ---------------------------------------------------------------- récompenses du Pokédex (v0.54, édition Carrière)
// Tous les 25 Pokémon capturés (Pokédex national), le Prof. Seko envoie un objet rare de combat, par courrier (menu L).
static const u16 sRecompensesDex[] =
{
    ITEM_LEFTOVERS, ITEM_LIFE_ORB, ITEM_CHOICE_SCARF, ITEM_FOCUS_SASH, ITEM_CHOICE_BAND, ITEM_CHOICE_SPECS,
    ITEM_ASSAULT_VEST, ITEM_ABILITY_PATCH, ITEM_ROCKY_HELMET, ITEM_WEAKNESS_POLICY, ITEM_HEAVY_DUTY_BOOTS,
    ITEM_EVIOLITE, ITEM_LOADED_DICE, ITEM_COVERT_CLOAK, ITEM_CLEAR_AMULET, ITEM_MASTER_BALL,
};

u32 Tycoon_PaliersDexAtteints(u32 captures)
{
    u32 n = captures / TYCOON_DEX_PALIER;
    return n > ARRAY_COUNT(sRecompensesDex) ? ARRAY_COUNT(sRecompensesDex) : n;
}

// Special : VAR_RESULT = TRUE s'il y a une récompense à donner ; VAR_0x8004 = objet, VAR_0x8005 = captures du palier.
void Tycoon_RecompenseDex(void)
{
    u32 palier;
    gSpecialVar_Result = FALSE;
    if (!EDITION_TYCOON_SIM || TY.magie != TYCOON_MAGIE)
        return;
    palier = TY.paliersDex;
    if (palier >= Tycoon_PaliersDexAtteints(GetNationalPokedexCount(FLAG_GET_CAUGHT)))
        return;
    gSpecialVar_0x8004 = sRecompensesDex[palier];
    gSpecialVar_0x8005 = (palier + 1) * TYCOON_DEX_PALIER;
    gSpecialVar_Result = TRUE;
}

// Special : la récompense a bien été rangée dans le sac.
void Tycoon_RecompenseDexDonnee(void)
{
    if (TY.magie == TYCOON_MAGIE && TY.paliersDex < ARRAY_COUNT(sRecompensesDex))
        TY.paliersDex++;
}
