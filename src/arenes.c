#include "global.h"
#include "arenes.h"
#include "battle.h"
#include "battle_main.h"
#include "battle_util.h"
#include "calendrier.h"
#include "data.h"
#include "event_data.h"
#include "money.h"
#include "pokemon.h"
#include "pokemon_storage_system.h"
#include "rtc.h"
#include "script_pokemon_util.h"
#include "societe.h"
#include "string_util.h"
#include "constants/battle_ai.h"
#include "constants/characters.h"
#include "constants/flags.h"
#include "constants/species.h"
#include "constants/trainers.h"
#include "constants/vars.h"

// Émeraude Couronnement : arènes du joueur (v0.40).
// Les Pokémon déposés restent dans le PC, marqués MON_DATA_ARENE (1-6) et MON_DATA_ARENE_PLACE (0-8) : le PC les bloque.
// Chaque jour (rattrapé à l'ouverture, 30 jours au plus) : XP pour les Pokémon déposés, sponsors selon la réputation,
// challengers qui arrivent (1 à 3 selon la réputation, 3 au plus en attente). Le joueur affronte le premier challenger :
// ses Pokémon d'arène sont prêtés au niveau du challenger, sans XP de combat, puis l'équipe d'origine revient.

#define AR (gSaveBlock3Ptr->arenes)
#define SOC (gSaveBlock2Ptr->pokedex.societe)

EWRAM_DATA u8 gAreneLignes[ARENE_PLACES][24] = {0};
static EWRAM_DATA struct Trainer sChallenger = {0};
static EWRAM_DATA struct TrainerMon sChallengerEquipe[PARTY_SIZE] = {0};
static EWRAM_DATA struct Pokemon sPartieSauvee[PARTY_SIZE] = {0};
static EWRAM_DATA u8 sCombattants[PARTY_SIZE] = {0};
static EWRAM_DATA u8 sNbCombattants = 0;
static EWRAM_DATA u8 sBadgesChallenger = 0;
static EWRAM_DATA bool8 sCombat = FALSE;

static const u8 sTypes[ARENE_NB_TYPES] =
{
    TYPE_GRASS, TYPE_ICE, TYPE_BUG, TYPE_POISON, TYPE_GROUND, TYPE_GHOST, TYPE_DRAGON, TYPE_DARK, TYPE_STEEL, TYPE_FAIRY,
};

static const u8 *const sVilles[ARENE_NB] =
{
    COMPOUND_STRING("Bourg-en-Vol"), COMPOUND_STRING("Rosyères"), COMPOUND_STRING("Poivressel"),
    COMPOUND_STRING("Vergazon"), COMPOUND_STRING("Vermilava"), COMPOUND_STRING("Nénucrique"),
};

// Par nombre de badges du challenger (1 à 8) : niveau (cap de ses badges) et nombre de Pokémon.
static const u8 sNiveaux[8] = {18, 27, 36, 45, 54, 63, 71, 78};
static const u8 sNbPokemon[8] = {2, 2, 3, 3, 4, 5, 6, 6};

static const u16 sEspeces[8][10] =
{
    {SPECIES_ZIGZAGOON, SPECIES_POOCHYENA, SPECIES_TAILLOW, SPECIES_WINGULL, SPECIES_LOTAD,
     SPECIES_SEEDOT, SPECIES_SHROOMISH, SPECIES_MAKUHITA, SPECIES_ARON, SPECIES_RALTS},
    {SPECIES_LINOONE, SPECIES_MIGHTYENA, SPECIES_LOMBRE, SPECIES_NUZLEAF, SPECIES_BRELOOM,
     SPECIES_MACHOKE, SPECIES_KIRLIA, SPECIES_LAIRON, SPECIES_PELIPPER, SPECIES_SWELLOW},
    {SPECIES_MANECTRIC, SPECIES_MAGNETON, SPECIES_GRAVELER, SPECIES_CAMERUPT, SPECIES_MEDICHAM,
     SPECIES_SHARPEDO, SPECIES_WAILMER, SPECIES_TORKOAL, SPECIES_GRUMPIG, SPECIES_HARIYAMA},
    {SPECIES_CACTURNE, SPECIES_ALTARIA, SPECIES_ZANGOOSE, SPECIES_SEVIPER, SPECIES_LUNATONE,
     SPECIES_SOLROCK, SPECIES_WHISCASH, SPECIES_CRAWDAUNT, SPECIES_FLYGON, SPECIES_SWALOT},
    {SPECIES_BANETTE, SPECIES_DUSCLOPS, SPECIES_TROPIUS, SPECIES_ABSOL, SPECIES_GLALIE,
     SPECIES_SEALEO, SPECIES_KECLEON, SPECIES_CLAYDOL, SPECIES_CRADILY, SPECIES_ARMALDO},
    {SPECIES_MILOTIC, SPECIES_GARDEVOIR, SPECIES_AGGRON, SPECIES_LUDICOLO, SPECIES_SHIFTRY,
     SPECIES_WALREIN, SPECIES_STARAPTOR, SPECIES_LUCARIO, SPECIES_SKARMORY, SPECIES_SCIZOR},
    {SPECIES_SALAMENCE, SPECIES_METAGROSS, SPECIES_GARCHOMP, SPECIES_TYRANITAR, SPECIES_GYARADOS,
     SPECIES_DRAGONITE, SPECIES_ARCANINE, SPECIES_GENGAR, SPECIES_ALAKAZAM, SPECIES_MACHAMP},
    {SPECIES_HYDREIGON, SPECIES_VOLCARONA, SPECIES_KINGAMBIT, SPECIES_DRAGAPULT, SPECIES_GARCHOMP,
     SPECIES_METAGROSS, SPECIES_TYRANITAR, SPECIES_SALAMENCE, SPECIES_ROTOM_WASH, SPECIES_GHOLDENGO},
};

struct Classe
{
    u8 classe;
    u16 pic;
    bool8 fille;
};

static const struct Classe sClasses[] =
{
    {TRAINER_CLASS_CAMPER, TRAINER_PIC_CAMPER, FALSE},
    {TRAINER_CLASS_PICNICKER, TRAINER_PIC_PICNICKER, TRUE},
    {TRAINER_CLASS_YOUNGSTER, TRAINER_PIC_YOUNGSTER, FALSE},
    {TRAINER_CLASS_LASS, TRAINER_PIC_LASS, TRUE},
    {TRAINER_CLASS_COOLTRAINER, TRAINER_PIC_COOLTRAINER_M, FALSE},
    {TRAINER_CLASS_COOLTRAINER, TRAINER_PIC_COOLTRAINER_F, TRUE},
    {TRAINER_CLASS_BLACK_BELT, TRAINER_PIC_BLACK_BELT, FALSE},
    {TRAINER_CLASS_BATTLE_GIRL, TRAINER_PIC_BATTLE_GIRL, TRUE},
    {TRAINER_CLASS_PSYCHIC, TRAINER_PIC_PSYCHIC_M, FALSE},
    {TRAINER_CLASS_PSYCHIC, TRAINER_PIC_PSYCHIC_F, TRUE},
    {TRAINER_CLASS_EXPERT, TRAINER_PIC_EXPERT_M, FALSE},
    {TRAINER_CLASS_EXPERT, TRAINER_PIC_EXPERT_F, TRUE},
};

static const u8 *const sNomsGarcons[] =
{
    COMPOUND_STRING("Hugo"), COMPOUND_STRING("Léo"), COMPOUND_STRING("Nathan"), COMPOUND_STRING("Malo"),
    COMPOUND_STRING("Enzo"), COMPOUND_STRING("Yanis"), COMPOUND_STRING("Tom"), COMPOUND_STRING("Noé"),
};
static const u8 *const sNomsFilles[] =
{
    COMPOUND_STRING("Jade"), COMPOUND_STRING("Lina"), COMPOUND_STRING("Chloé"), COMPOUND_STRING("Inès"),
    COMPOUND_STRING("Manon"), COMPOUND_STRING("Zoé"), COMPOUND_STRING("Lou"), COMPOUND_STRING("Emma"),
};

// ---------------------------------------------------------------- outils

static u32 Melanger(u32 x)
{
    x ^= x >> 16;
    x *= 0x7FEB352D;
    x ^= x >> 15;
    x *= 0x846CA68B;
    x ^= x >> 16;
    return x;
}

static u32 Alea(u32 arene, u32 n, u32 tirage)
{
    const u8 *id = gSaveBlock2Ptr->playerTrainerId;
    u32 graine = id[0] | (id[1] << 8) | (id[2] << 16) | (id[3] << 24);
    return Melanger(graine ^ (arene << 24) ^ (n * 0x9E3779B1u) ^ (tirage << 16));
}

static u8 *AjouterNombre(u8 *fin, u32 n)
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

static const u8 sText_Yens[] = _(" ¥");

static u8 *AjouterYens(u8 *fin, u32 n)
{
    fin = AjouterNombre(fin, n);
    return StringCopy(fin, sText_Yens);
}

static void Verifier(u32 jour)
{
    if (AR.magie == ARENE_MAGIE)
        return;
    memset(&AR, 0, sizeof(AR));
    AR.magie = ARENE_MAGIE;
    AR.jour = jour;
}

static u32 Aujourdhui(void)
{
    return RtcGetLocalDayCount();
}

// Recettes : trésorerie de la société si elle existe, sinon l'argent du joueur.
static void Encaisser(u32 montant)
{
    if (SOC.etat != SOCIETE_ETAT_AUCUNE)
        SOC.tresorerie = (u64)SOC.tresorerie + montant > 2000000000 ? 2000000000 : SOC.tresorerie + montant;
    else
        AddMoney(&gSaveBlock1Ptr->money, montant);
    AR.sponsors = (u64)AR.sponsors + montant > 2000000000 ? 2000000000 : AR.sponsors + montant;
}

static u32 Disponible(void)
{
    u64 total = GetMoney(&gSaveBlock1Ptr->money) + (u64)(SOC.etat != SOCIETE_ETAT_AUCUNE ? SOC.tresorerie : 0);
    return total > 2000000000 ? 2000000000 : total;
}

// Paiement : trésorerie d'abord, puis le joueur. Prend au plus ce qui est disponible ; renvoie le montant pris.
static u32 Prendre(u32 montant)
{
    u32 tresorerie = SOC.etat != SOCIETE_ETAT_AUCUNE ? SOC.tresorerie : 0, pris;

    if (montant > Disponible())
        montant = Disponible();
    pris = montant < tresorerie ? montant : tresorerie;
    if (SOC.etat != SOCIETE_ETAT_AUCUNE)
        SOC.tresorerie -= pris;
    RemoveMoney(&gSaveBlock1Ptr->money, montant - pris);
    return montant;
}

static bool32 EstDuType(u32 species, u32 type)
{
    return GetSpeciesType(species, 0) == type || GetSpeciesType(species, 1) == type;
}

// ---------------------------------------------------------------- Pokémon déposés

static void Scanner(struct BoxPokemon *places[ARENE_NB][ARENE_PLACES])
{
    u32 b, p;

    memset(places, 0, sizeof(struct BoxPokemon *) * ARENE_NB * ARENE_PLACES);
    for (b = 0; b < TOTAL_BOXES_COUNT; b++)
    {
        for (p = 0; p < IN_BOX_COUNT; p++)
        {
            struct BoxPokemon *bm = GetBoxedMonPtr(b, p);
            u32 a, place;
            if (!bm->hasSpecies)
                continue;
            a = GetBoxMonData(bm, MON_DATA_ARENE);
            if (a == 0 || a > ARENE_NB)
                continue;
            place = GetBoxMonData(bm, MON_DATA_ARENE_PLACE);
            if (place < ARENE_PLACES && places[a - 1][place] == NULL)
                places[a - 1][place] = bm;
        }
    }
}

u32 Arene_Pokemon(u32 arene, struct BoxPokemon **places)
{
    static EWRAM_DATA struct BoxPokemon *sTout[ARENE_NB][ARENE_PLACES] = {0};
    u32 i, n = 0;

    Scanner(sTout);
    for (i = 0; i < ARENE_PLACES; i++)
    {
        places[i] = arene < ARENE_NB ? sTout[arene][i] : NULL;
        n += places[i] != NULL;
    }
    return n;
}

bool32 Arene_EstBloque(struct BoxPokemon *boxMon)
{
    return boxMon->hasSpecies && GetBoxMonData(boxMon, MON_DATA_ARENE) != 0;
}

bool32 Arene_PokemonDansLePC(void)
{
    u32 b, p;

    if (AR.magie != ARENE_MAGIE)
        return FALSE;
    for (b = 0; b < TOTAL_BOXES_COUNT; b++)
        for (p = 0; p < IN_BOX_COUNT; p++)
            if (Arene_EstBloque(GetBoxedMonPtr(b, p)))
                return TRUE;
    return FALSE;
}

bool32 Arene_Achetee(u32 arene)
{
    return AR.magie == ARENE_MAGIE && arene < ARENE_NB && AR.arenes[arene].achetee;
}

u32 Arene_Type(u32 arene)
{
    return sTypes[AR.arenes[arene].type % ARENE_NB_TYPES];
}

static u32 TitulairesDuType(u32 arene, struct BoxPokemon **places)
{
    u32 i, n = 0;
    for (i = 0; i < ARENE_TITULAIRES; i++)
        if (places[i] && EstDuType(GetBoxMonData(places[i], MON_DATA_SPECIES), Arene_Type(arene)))
            n++;
    return n;
}

bool32 Arene_Valide(u32 arene)
{
    struct BoxPokemon *places[ARENE_PLACES];

    if (!Arene_Achetee(arene))
        return FALSE;
    Arene_Pokemon(arene, places);
    return TitulairesDuType(arene, places) >= ARENE_MIN_DU_TYPE;
}

u32 Arene_Valeur(void)
{
    u32 a, n = 0;
    for (a = 0; a < ARENE_NB; a++)
        n += Arene_Achetee(a);
    return n * ARENE_PRIX;
}

static void DonnerXP(struct BoxPokemon *bm, u32 pourcent)
{
    u32 species = GetBoxMonData(bm, MON_DATA_SPECIES), niveau = GetLevelFromBoxMonExp(bm), exp, gain;
    const u32 *table = gExperienceTables[gSpeciesInfo[species].growthRate];

    if (niveau >= MAX_LEVEL)
        return;
    gain = (u64)(table[niveau + 1] - table[niveau]) * pourcent / 100;
    exp = GetBoxMonData(bm, MON_DATA_EXP) + (gain ? gain : 1);
    if (exp > table[MAX_LEVEL])
        exp = table[MAX_LEVEL];
    SetBoxMonData(bm, MON_DATA_EXP, &exp);
}

// ---------------------------------------------------------------- jours

static void PasserJour(struct BoxPokemon *places[ARENE_NB][ARENE_PLACES])
{
    u32 a, i, arrivees;

    for (a = 0; a < ARENE_NB; a++)
    {
        struct AreneEtat *e = &AR.arenes[a];
        if (!e->achetee)
            continue;
        for (i = 0; i < ARENE_PLACES; i++)
            if (places[a][i])
                DonnerXP(places[a][i], 20 + e->reputation / 5);
        if (TitulairesDuType(a, places[a]) < ARENE_MIN_DU_TYPE)
            continue;
        Encaisser(ARENE_SPONSORS_PAR_POINT * e->reputation);
        if (e->repos)
        {
            e->repos--;
            continue;
        }
        if (e->attente >= ARENE_ATTENTE_MAX && e->reputation > 0)
            e->reputation--; // des challengers repartent sans avoir été reçus
        for (arrivees = 1 + e->reputation / 40; arrivees > 0 && e->attente < ARENE_ATTENTE_MAX; arrivees--)
        {
            e->attente++;
            e->arrivees++;
        }
    }
}

void Arene_MettreAJourJusqua(u32 aujourdhui)
{
    static EWRAM_DATA struct BoxPokemon *sPlaces[ARENE_NB][ARENE_PLACES] = {0};
    u32 a;
    bool32 une = FALSE;

    Verifier(aujourdhui);
    for (a = 0; a < ARENE_NB; a++)
        une |= AR.arenes[a].achetee;
    if (!une || aujourdhui < AR.jour)
    {
        AR.jour = aujourdhui;
        return;
    }
    if (aujourdhui - AR.jour > ARENE_RATTRAPAGE_MAX)
        AR.jour = aujourdhui - ARENE_RATTRAPAGE_MAX;
    if (AR.jour == aujourdhui)
        return;
    Scanner(sPlaces);
    while (AR.jour < aujourdhui)
    {
        AR.jour++;
        PasserJour(sPlaces);
    }
}

bool32 Arene_Acheter(u32 arene, u32 typeIndex)
{
    struct AreneEtat *e;

    Verifier(Aujourdhui());
    if (arene >= ARENE_NB || typeIndex >= ARENE_NB_TYPES || AR.arenes[arene].achetee || !FlagGet(FLAG_SYS_GAME_CLEAR)
     || Disponible() < ARENE_PRIX)
        return FALSE;
    Prendre(ARENE_PRIX);
    e = &AR.arenes[arene];
    memset(e, 0, sizeof(*e));
    e->achetee = TRUE;
    e->type = typeIndex;
    e->reputation = ARENE_REPUTATION_DEPART;
    return TRUE;
}

enum { DEPOT_OK, DEPOT_COMPLET, DEPOT_DERNIER, DEPOT_OEUF, DEPOT_PC_PLEIN, DEPOT_ERREUR };

static u32 Deposer(u32 arene, u32 slot, bool32 titulaire)
{
    struct BoxPokemon *places[ARENE_PLACES], *libre = NULL;
    u32 debut = titulaire ? 0 : ARENE_TITULAIRES, fin = titulaire ? ARENE_TITULAIRES : ARENE_PLACES, place, b, p;
    u8 valeur;

    if (!Arene_Achetee(arene) || slot >= PARTY_SIZE || GetMonData(&gPlayerParty[slot], MON_DATA_SPECIES) == SPECIES_NONE)
        return DEPOT_ERREUR;
    if (GetMonData(&gPlayerParty[slot], MON_DATA_IS_EGG))
        return DEPOT_OEUF;
    if (CalculatePlayerPartyCount() <= 1)
        return DEPOT_DERNIER;
    Arene_Pokemon(arene, places);
    for (place = debut; place < fin && places[place]; place++)
        ;
    if (place >= fin)
        return DEPOT_COMPLET;
    for (b = TOTAL_BOXES_COUNT; b-- > 0 && libre == NULL;)
        for (p = 0; p < IN_BOX_COUNT && libre == NULL; p++)
            if (!GetBoxedMonPtr(b, p)->hasSpecies)
                libre = GetBoxedMonPtr(b, p);
    if (libre == NULL)
        return DEPOT_PC_PLEIN;
    *libre = gPlayerParty[slot].box;
    valeur = arene + 1;
    SetBoxMonData(libre, MON_DATA_ARENE, &valeur);
    valeur = place;
    SetBoxMonData(libre, MON_DATA_ARENE_PLACE, &valeur);
    ZeroMonData(&gPlayerParty[slot]);
    CompactPartySlots();
    CalculatePlayerPartyCount();
    return DEPOT_OK;
}

bool32 Arene_Deposer(u32 arene, u32 partySlot, bool32 titulaire)
{
    return Deposer(arene, partySlot, titulaire) == DEPOT_OK;
}

// Le Pokémon quitte l'arène : dans l'équipe s'il y a de la place, sinon il reste (libre) dans le PC.
static u32 Retirer(u32 arene, u32 place)
{
    struct BoxPokemon *places[ARENE_PLACES];
    u8 zero = 0;
    u32 n;

    if (!Arene_Achetee(arene) || place >= ARENE_PLACES)
        return 0;
    Arene_Pokemon(arene, places);
    if (places[place] == NULL)
        return 0;
    SetBoxMonData(places[place], MON_DATA_ARENE, &zero);
    SetBoxMonData(places[place], MON_DATA_ARENE_PLACE, &zero);
    n = CalculatePlayerPartyCount();
    if (n < PARTY_SIZE)
    {
        BoxMonToMon(places[place], &gPlayerParty[n]);
        ZeroBoxMonData(places[place]);
        CalculatePlayerPartyCount();
        return 1;
    }
    return 2;
}

bool32 Arene_Retirer(u32 arene, u32 place)
{
    return Retirer(arene, place) != 0;
}

// ---------------------------------------------------------------- challengers

static u32 IndexChallenger(u32 arene)
{
    return AR.arenes[arene].arrivees - AR.arenes[arene].attente;
}

u32 Arene_BadgesChallenger(u32 arene)
{
    struct AreneEtat *e;
    u32 max;

    if (!Arene_Achetee(arene) || AR.arenes[arene].attente == 0)
        return 0;
    e = &AR.arenes[arene];
    max = 3 + e->reputation / 20;
    if (max > 8)
        max = 8;
    return 1 + Alea(arene, IndexChallenger(arene), 1) % max;
}

static void ConstruireChallenger(u32 arene, u32 badges)
{
    u32 n = IndexChallenger(arene), i, k, tirage = 10;
    const struct Classe *c = &sClasses[Alea(arene, n, 2) % ARRAY_COUNT(sClasses)];
    const u8 *nom = c->fille ? sNomsFilles[Alea(arene, n, 3) % ARRAY_COUNT(sNomsFilles)]
                             : sNomsGarcons[Alea(arene, n, 3) % ARRAY_COUNT(sNomsGarcons)];
    u32 nb = sNbPokemon[badges - 1];

    memset(&sChallenger, 0, sizeof(sChallenger));
    memset(sChallengerEquipe, 0, sizeof(sChallengerEquipe));
    for (i = 0; i < nb; i++)
    {
        u16 espece;
        do
        {
            espece = sEspeces[badges - 1][Alea(arene, n, tirage++) % 10];
            for (k = 0; k < i && sChallengerEquipe[k].species != espece; k++)
                ;
        } while (k < i && tirage < 60);
        sChallengerEquipe[i].species = espece;
        sChallengerEquipe[i].lvl = sNiveaux[badges - 1];
        sChallengerEquipe[i].iv = TRAINER_PARTY_IVS(20, 20, 20, 20, 20, 20);
        sChallengerEquipe[i].nature = Alea(arene, n, 70 + i) % NUM_NATURES;
    }
    sChallenger.aiFlags = AI_FLAG_BASIC_TRAINER;
    sChallenger.party = sChallengerEquipe;
    sChallenger.partySize = nb;
    sChallenger.trainerClass = c->classe;
    sChallenger.trainerPic = c->pic;
    sChallenger.gender = c->fille;
    sChallenger.encounterMusic = c->fille ? TRAINER_ENCOUNTER_MUSIC_FEMALE : TRAINER_ENCOUNTER_MUSIC_MALE;
    sChallenger.battleType = TRAINER_BATTLE_TYPE_SINGLES;
    StringCopy(sChallenger.trainerName, nom);
}

const struct Trainer *Arene_Challenger(void)
{
    return &sChallenger;
}

bool32 Arene_CombatEnCours(void)
{
    return sCombat;
}

static u32 EfficaciteSur(u32 atk, u32 species)
{
    uq4_12_t mod = GetTypeModifier(atk, GetSpeciesType(species, 0));
    if (GetSpeciesType(species, 1) != GetSpeciesType(species, 0))
        mod = uq4_12_multiply(mod, GetTypeModifier(atk, GetSpeciesType(species, 1)));
    return mod * 4 / UQ_4_12(1.0);
}

// Note d'un Pokémon d'arène face à l'équipe du challenger : ses types en attaque moins ceux du challenger.
static s32 Note(struct BoxPokemon *bm, bool32 titulaire)
{
    u32 species = GetBoxMonData(bm, MON_DATA_SPECIES), i;
    s32 note = titulaire ? 3 : 0;

    for (i = 0; i < sChallenger.partySize; i++)
    {
        u32 c = sChallengerEquipe[i].species;
        note += EfficaciteSur(GetSpeciesType(species, 0), c) + EfficaciteSur(GetSpeciesType(species, 1), c);
        note -= EfficaciteSur(GetSpeciesType(c, 0), species) + EfficaciteSur(GetSpeciesType(c, 1), species);
    }
    return note;
}

static const u8 sText_Defie1[] = _(" (");
static const u8 sText_Defie2[] = _(" badge(s)) te défie !\nIl aligne ");
static const u8 sText_Defie3[] = _(" Pokémon au N.");
static const u8 sText_Defie4[] = _(".\pTon arène envoie :\n");
static const u8 sText_Virgule[] = _(", ");
static const u8 sText_VirguleLigne[] = _(",\l");
static const u8 sText_Point[] = _(".");
static const u8 sText_Para[] = _("\p");
static const u8 sText_VirguleN[] = _(",\n");
static const u8 sText_PasDeChallenger[] = _("Personne n'attend pour l'instant.\nReviens demain.");
static const u8 sText_ArenePasPrete[] = _("Il faut 4 titulaires du type de\nl'arène pour recevoir des\lchallengers.");

// VAR_0x8008 = arène. VAR_RESULT = TRUE si un challenger attend (texte de présentation dans gStringVar4).
void Arene_PreparerChallenger(void)
{
    u32 a = gSpecialVar_0x8008, badges, nb, i, j, n;
    struct BoxPokemon *places[ARENE_PLACES];
    s32 notes[ARENE_PLACES];
    u8 *fin;
    u8 nom[POKEMON_NAME_LENGTH + 1];

    gSpecialVar_Result = FALSE;
    Arene_MettreAJourJusqua(Aujourdhui());
    if (!Arene_Valide(a))
    {
        StringCopy(gStringVar4, sText_ArenePasPrete);
        return;
    }
    badges = Arene_BadgesChallenger(a);
    if (badges == 0)
    {
        StringCopy(gStringVar4, sText_PasDeChallenger);
        return;
    }
    ConstruireChallenger(a, badges);
    sBadgesChallenger = badges;
    nb = sChallenger.partySize;

    // Les meilleurs Pokémon de l'arène face à ce challenger.
    n = Arene_Pokemon(a, places);
    for (i = 0; i < ARENE_PLACES; i++)
        notes[i] = places[i] ? Note(places[i], i < ARENE_TITULAIRES) : -10000;
    if (nb > n)
        nb = n;
    sNbCombattants = 0;
    for (j = 0; j < nb; j++)
    {
        u32 meilleur = 0;
        for (i = 1; i < ARENE_PLACES; i++)
            if (notes[i] > notes[meilleur])
                meilleur = i;
        sCombattants[sNbCombattants++] = meilleur;
        notes[meilleur] = -20000;
    }

    fin = StringCopy(gStringVar4, sChallenger.trainerName);
    fin = StringCopy(fin, sText_Defie1);
    fin = AjouterNombre(fin, badges);
    fin = StringCopy(fin, sText_Defie2);
    fin = AjouterNombre(fin, sChallenger.partySize);
    fin = StringCopy(fin, sText_Defie3);
    fin = AjouterNombre(fin, sNiveaux[badges - 1]);
    fin = StringCopy(fin, sText_Defie4);
    for (i = 0; i < sNbCombattants; i++)
    {
        if (i > 0)
            fin = StringCopy(fin, (i % 2 == 0) ? sText_VirguleLigne : sText_Virgule);
        GetBoxMonData(places[sCombattants[i]], MON_DATA_NICKNAME, nom);
        fin = StringCopy(fin, StringGet_Nickname(nom));
    }
    StringCopy(fin, sText_Point);
    gSpecialVar_Result = TRUE;
}

// Avant le combat : l'équipe est mise de côté, les Pokémon d'arène sont prêtés au niveau du challenger.
void Arene_DebutCombat(void)
{
    struct BoxPokemon *places[ARENE_PLACES];
    u32 i, niveau = sNiveaux[(sBadgesChallenger ? sBadgesChallenger : 1) - 1];

    Arene_Pokemon(gSpecialVar_0x8008, places);
    memcpy(sPartieSauvee, gPlayerParty, sizeof(sPartieSauvee));
    for (i = 0; i < PARTY_SIZE; i++)
        ZeroMonData(&gPlayerParty[i]);
    for (i = 0; i < sNbCombattants; i++)
    {
        struct Pokemon *mon = &gPlayerParty[i];
        u32 species, exp;
        if (places[sCombattants[i]] == NULL)
            continue;
        BoxMonToMon(places[sCombattants[i]], mon);
        species = GetMonData(mon, MON_DATA_SPECIES);
        exp = gExperienceTables[gSpeciesInfo[species].growthRate][niveau];
        SetMonData(mon, MON_DATA_EXP, &exp);
        SetMonData(mon, MON_DATA_LEVEL, &niveau);
        CalculateMonStats(mon);
    }
    CompactPartySlots();
    CalculatePlayerPartyCount();
    HealPlayerParty();
    sCombat = TRUE;
    FlagSet(FLAG_COURONNEMENT_EN_COURS); // pas de blackout
}

static const u8 sText_Gagne1[] = _("Victoire ! ");
static const u8 sText_Gagne2[] = _(" repart\nsans badge. Sponsors : +");
static const u8 sText_Gagne3[] = _(".\pRéputation : ");
static const u8 sText_Gagne4[] = _(". Tes Pokémon\nont gagné de l'expérience.");
static const u8 sText_Perdu1[] = _(" remporte le badge\nde ton arène !\pPrime du badge payée : ");
static const u8 sText_Perdu2[] = _(".\nRéputation : ");
static const u8 sText_Perdu3[] = _(".\pLes challengers boudent ton arène\npendant deux jours.");

// Après le combat : l'équipe d'origine revient. VAR_RESULT = victoire ; bilan dans gStringVar4.
void Arene_FinCombat(void)
{
    u32 a = gSpecialVar_0x8008, i, montant;
    bool32 victoire = (gBattleOutcome == B_OUTCOME_WON);
    struct BoxPokemon *places[ARENE_PLACES];
    struct AreneEtat *e = &AR.arenes[a % ARENE_NB];
    u8 *fin;

    memcpy(gPlayerParty, sPartieSauvee, sizeof(sPartieSauvee));
    CalculatePlayerPartyCount();
    sCombat = FALSE;
    FlagClear(FLAG_COURONNEMENT_EN_COURS);
    if (e->attente)
        e->attente--;
    Arene_Pokemon(a, places);
    if (victoire)
    {
        e->reputation = e->reputation + 3 > 100 ? 100 : e->reputation + 3;
        if (e->victoires < 0xFFFF)
            e->victoires++;
        for (i = 0; i < sNbCombattants; i++)
            if (places[sCombattants[i]])
                DonnerXP(places[sCombattants[i]], 50);
        montant = ARENE_BONUS_BADGE * sBadgesChallenger;
        Encaisser(montant);
        fin = StringCopy(gStringVar4, sText_Gagne1);
        fin = StringCopy(fin, sChallenger.trainerName);
        fin = StringCopy(fin, sText_Gagne2);
        fin = AjouterYens(fin, montant);
        fin = StringCopy(fin, sText_Gagne3);
        fin = AjouterNombre(fin, e->reputation);
        StringCopy(fin, sText_Gagne4);
    }
    else
    {
        e->reputation = e->reputation > 8 ? e->reputation - 8 : 0;
        e->repos = ARENE_REPOS_DEFAITE;
        montant = Prendre(ARENE_PRIME_BADGE * sBadgesChallenger);
        fin = StringCopy(gStringVar4, sChallenger.trainerName);
        fin = StringCopy(fin, sText_Perdu1);
        fin = AjouterYens(fin, montant);
        fin = StringCopy(fin, sText_Perdu2);
        fin = AjouterNombre(fin, e->reputation);
        StringCopy(fin, sText_Perdu3);
    }
    gSpecialVar_Result = victoire;
}

// ---------------------------------------------------------------- specials de l'arène

static const u8 sText_Fiche1[] = _("Arène de ");
static const u8 sText_Fiche2[] = _(" (");
static const u8 sText_Fiche3[] = _(").\nRéputation : ");
static const u8 sText_Fiche4[] = _(". Victoires : ");
static const u8 sText_Fiche5[] = _(".\pTitulaires : ");
static const u8 sText_Fiche6[] = _("/6, dont ");
static const u8 sText_Fiche7[] = _(" du type.\nRemplaçants : ");
static const u8 sText_Fiche8[] = _("/3. En attente : ");
static const u8 sText_Sponsors[] = _("\pSponsors depuis ta dernière\nvisite : ");

// VAR_0x8008 = arène. VAR_RESULT : 0 = pas encore la Ligue, 1 = à vendre, 2 = à toi (fiche dans gStringVar4).
// STR_VAR_1 = ville.
void Arene_Ouvrir(void)
{
    u32 a = gSpecialVar_0x8008 % ARENE_NB, i, titulaires = 0, remplacants = 0;
    struct BoxPokemon *places[ARENE_PLACES];
    struct AreneEtat *e;
    u8 *fin;

    Arene_MettreAJourJusqua(Aujourdhui());
    StringCopy(gStringVar1, sVilles[a]);
    if (!Arene_Achetee(a))
    {
        gSpecialVar_Result = FlagGet(FLAG_SYS_GAME_CLEAR) ? 1 : 0;
        return;
    }
    e = &AR.arenes[a];
    Arene_Pokemon(a, places);
    for (i = 0; i < ARENE_PLACES; i++)
    {
        if (places[i] == NULL)
            continue;
        if (i < ARENE_TITULAIRES)
            titulaires++;
        else
            remplacants++;
    }
    fin = StringCopy(gStringVar4, sText_Fiche1);
    fin = StringCopy(fin, sVilles[a]);
    fin = StringCopy(fin, sText_Fiche2);
    fin = StringCopy(fin, gTypesInfo[Arene_Type(a)].name);
    fin = StringCopy(fin, sText_Fiche3);
    fin = AjouterNombre(fin, e->reputation);
    fin = StringCopy(fin, sText_Fiche4);
    fin = AjouterNombre(fin, e->victoires);
    fin = StringCopy(fin, sText_Fiche5);
    fin = AjouterNombre(fin, titulaires);
    fin = StringCopy(fin, sText_Fiche6);
    fin = AjouterNombre(fin, TitulairesDuType(a, places));
    fin = StringCopy(fin, sText_Fiche7);
    fin = AjouterNombre(fin, remplacants);
    fin = StringCopy(fin, sText_Fiche8);
    fin = AjouterNombre(fin, e->attente);
    fin = StringCopy(fin, sText_Point);
    if (AR.sponsors)
    {
        fin = StringCopy(fin, sText_Sponsors);
        fin = AjouterYens(fin, AR.sponsors);
        StringCopy(fin, sText_Point);
        AR.sponsors = 0;
    }
    gSpecialVar_Result = 2;
}

// VAR_0x8008 = arène, VAR_0x8005 = type (ordre de la liste). VAR_RESULT = achat réussi.
void Arene_ScriptAcheter(void)
{
    gSpecialVar_Result = Arene_Acheter(gSpecialVar_0x8008, gSpecialVar_0x8005);
}

static const u8 *const sTextesDepot[] =
{
    [DEPOT_OK]       = COMPOUND_STRING("Ton Pokémon s'installe dans l'arène.\nIl s'entraîne ici chaque jour."),
    [DEPOT_COMPLET]  = COMPOUND_STRING("Plus de place à ce poste. Retire\nd'abord un Pokémon."),
    [DEPOT_DERNIER]  = COMPOUND_STRING("Tu ne peux pas laisser ton dernier\nPokémon ici."),
    [DEPOT_OEUF]     = COMPOUND_STRING("Un Œuf ne garde pas une arène."),
    [DEPOT_PC_PLEIN] = COMPOUND_STRING("Le PC est plein : plus de place\npour loger ton Pokémon."),
    [DEPOT_ERREUR]   = COMPOUND_STRING("Rien n'a été déposé."),
};

// VAR_0x8008 = arène, VAR_0x8004 = Pokémon de l'équipe, VAR_0x8005 = 0 titulaire / 1 remplaçant.
void Arene_ScriptDeposer(void)
{
    u32 r = Deposer(gSpecialVar_0x8008, gSpecialVar_0x8004, gSpecialVar_0x8005 == 0);
    StringCopy(gStringVar4, sTextesDepot[r]);
    gSpecialVar_Result = (r == DEPOT_OK);
}

static const u8 sText_Vide[] = _("-");

// VAR_0x8008 = arène : gAreneLignes = « T1 Nom N.xx » pour les 9 places.
void Arene_PreparerListe(void)
{
    struct BoxPokemon *places[ARENE_PLACES];
    u8 nom[POKEMON_NAME_LENGTH + 1];
    u32 i;

    Arene_Pokemon(gSpecialVar_0x8008, places);
    for (i = 0; i < ARENE_PLACES; i++)
    {
        u8 *fin = gAreneLignes[i];
        *fin++ = i < ARENE_TITULAIRES ? CHAR_T : CHAR_R;
        fin = AjouterNombre(fin, i < ARENE_TITULAIRES ? i + 1 : i - ARENE_TITULAIRES + 1);
        *fin++ = CHAR_SPACE;
        if (places[i] == NULL)
        {
            StringCopy(fin, sText_Vide);
            continue;
        }
        GetBoxMonData(places[i], MON_DATA_NICKNAME, nom);
        fin = StringCopy(fin, StringGet_Nickname(nom));
        *fin++ = CHAR_SPACE;
        *fin++ = CHAR_N;
        *fin++ = CHAR_PERIOD;
        AjouterNombre(fin, GetLevelFromBoxMonExp(places[i]));
    }
}

static const u8 sText_RetireEquipe[] = _("Ton Pokémon quitte l'arène et\nrejoint ton équipe.");
static const u8 sText_RetirePC[] = _("Ton équipe est pleine : il quitte\nl'arène et t'attend dans le PC.");
static const u8 sText_RetireRien[] = _("Personne à cette place.");

// VAR_0x8008 = arène, VAR_0x8005 = place (0-8).
void Arene_ScriptRetirer(void)
{
    u32 r = Retirer(gSpecialVar_0x8008, gSpecialVar_0x8005);
    StringCopy(gStringVar4, r == 1 ? sText_RetireEquipe : (r == 2 ? sText_RetirePC : sText_RetireRien));
    gSpecialVar_Result = r != 0;
}

static const u8 sText_Resume1[] = _("Arènes à toi : ");
static const u8 sText_Resume2[] = _(".\nChallengers qui attendent : ");
static const u8 sText_ResumeAucune[] = _("Tu n'as pas encore d'arène. Elles\nse vendent sur place, après la Ligue.");

// Menu L : résumé de toutes les arènes (gStringVar4).
void Arene_Resume(void)
{
    u32 a, n = 0, attente = 0;
    u8 *fin;

    Arene_MettreAJourJusqua(Aujourdhui());
    for (a = 0; a < ARENE_NB; a++)
    {
        if (!Arene_Achetee(a))
            continue;
        n++;
        attente += AR.arenes[a].attente;
    }
    if (n == 0)
    {
        StringCopy(gStringVar4, sText_ResumeAucune);
        return;
    }
    fin = StringCopy(gStringVar4, sText_Resume1);
    fin = AjouterNombre(fin, n);
    fin = StringCopy(fin, sText_Resume2);
    fin = AjouterNombre(fin, attente);
    fin = StringCopy(fin, sText_Point);
    for (a = 0, n = 0; a < ARENE_NB; a++)
    {
        if (!Arene_Achetee(a) || AR.arenes[a].attente == 0)
            continue;
        fin = StringCopy(fin, n == 0 ? sText_Para : (n == 2 ? sText_VirguleN : (n == 4 ? sText_VirguleLigne : sText_Virgule)));
        n++;
        fin = StringCopy(fin, sVilles[a]);
        *fin++ = CHAR_SPACE;
        *fin++ = CHAR_COLON;
        *fin++ = CHAR_SPACE;
        fin = AjouterNombre(fin, AR.arenes[a].attente);
        *fin = EOS;
    }
    if (n)
        fin = StringCopy(fin, sText_Point);
    if (AR.sponsors)
    {
        fin = StringCopy(fin, sText_Sponsors);
        fin = AjouterYens(fin, AR.sponsors);
        StringCopy(fin, sText_Point);
        AR.sponsors = 0;
    }
}
