#include "global.h"
#include "league_club.h"
#include "event_data.h"
#include "money.h"
#include "pokemon.h"
#include "constants/species.h"

// Émeraude Couronnement : Pokémon de prêt du club (v0.49, édition Carrière ; réglage CLUB_PRET_CLASSIQUE pour l'autre).
// Si le joueur a moins de 6 Pokémon avant son duel de ligue, son club complète l'équipe avec des espèces de son thème
// (tirées des équipes de ses dresseurs, évoluées au niveau de l'adversaire). Gratuit en tier C la 1re saison, puis 500 ¥
// par Pokémon. Après le combat, l'équipe d'origine revient, avec l'XP gagnée par ses propres Pokémon.

#define PERSONNALITE_PRET 0xC1B50000   // + n° de la place : repère des Pokémon prêtés

static EWRAM_DATA struct Pokemon sEquipeAvant[PARTY_SIZE] = {0};
static EWRAM_DATA u8 sNbPretes = 0;

static u32 Valides(void)
{
    u32 i, n = 0;
    for (i = 0; i < PARTY_SIZE; i++)
        if (GetMonData(&gParties[B_TRAINER_PLAYER][i], MON_DATA_SPECIES) != SPECIES_NONE
         && !GetMonData(&gParties[B_TRAINER_PLAYER][i], MON_DATA_IS_EGG))
            n++;
    return n;
}

bool32 Club_PretDisponible(void)
{
    return EDITION_TYCOON_SIM || CLUB_PRET_CLASSIQUE;
}

// Nombre de Pokémon à prêter (places vides de l'équipe, jusqu'à 6 Pokémon aptes au combat).
u32 Club_PretManquants(void)
{
    u32 libres = PARTY_SIZE - CalculatePlayerPartyCount(), manque = CLUB_TAILLE_EQUIPE - Valides();
    if (!Club_PretDisponible())
        return 0;
    return manque < libres ? manque : libres;
}

u32 Club_PretCout(u32 nb)
{
    u32 club = Club_DuJoueur();
    if (club != CLUB_AUCUN && Club_Tier(club) == CLUB_TIER_C && Club_Saison() == 0)
        return 0;
    return nb * CLUB_PRET_PRIX;
}

static bool32 DejaDansLEquipe(u32 species)
{
    u32 i;
    for (i = 0; i < PARTY_SIZE; i++)
        if (GetMonData(&gParties[B_TRAINER_PLAYER][i], MON_DATA_SPECIES) == species)
            return TRUE;
    return FALSE;
}

// Met l'équipe de côté et ajoute nb Pokémon prêtés au niveau donné.
void Club_PretPreparer(u32 club, u32 niveau, u32 nb)
{
    u32 s, k, ajoutes = 0;

    memcpy(sEquipeAvant, gParties[B_TRAINER_PLAYER], sizeof(sEquipeAvant));
    sNbPretes = 0;
    if (club == CLUB_AUCUN)
        return;
    // Les espèces des dresseurs du club, Capitaine d'abord, sans doublon avec l'équipe.
    for (k = 0; k < CLUB_TAILLE_EQUIPE && ajoutes < nb; k++)
    {
        for (s = 0; s < CLUB_DRESSEURS && ajoutes < nb; s++)
        {
            u32 species = Club_EvoluerPourNiveau(Club_Dresseur(club, s)->especes[k], niveau), place;
            if (species == SPECIES_NONE || DejaDansLEquipe(species))
                continue;
            place = CalculatePlayerPartyCount();
            if (place >= PARTY_SIZE)
                return;
            CreateMon(&gParties[B_TRAINER_PLAYER][place], species, niveau, PERSONNALITE_PRET + place, OTID_STRUCT_PLAYER_ID);
            CalculatePlayerPartyCount();
            ajoutes++;
        }
    }
    sNbPretes = ajoutes;
}

u32 Club_PretNombre(void)
{
    return sNbPretes;
}

// Après le combat : les Pokémon du joueur reviennent tels qu'ils sont sortis du combat (XP, évolution) ; les prêtés repartent.
void Club_PretRendre(void)
{
    struct Pokemon *equipe = gParties[B_TRAINER_PLAYER];
    u32 i, j;
    static EWRAM_DATA struct Pokemon sApres[PARTY_SIZE] = {0};

    if (sNbPretes == 0)
        return;
    memcpy(sApres, equipe, sizeof(sApres));
    for (i = 0; i < PARTY_SIZE; i++)
    {
        u32 perso = GetMonData(&sEquipeAvant[i], MON_DATA_PERSONALITY);
        equipe[i] = sEquipeAvant[i];
        if (GetMonData(&sEquipeAvant[i], MON_DATA_SPECIES) == SPECIES_NONE)
            continue;
        for (j = 0; j < PARTY_SIZE; j++)
            if (GetMonData(&sApres[j], MON_DATA_SPECIES) != SPECIES_NONE && GetMonData(&sApres[j], MON_DATA_PERSONALITY) == perso)
                equipe[i] = sApres[j];
    }
    CalculatePlayerPartyCount();
    sNbPretes = 0;
}
