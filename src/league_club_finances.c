#include "global.h"
#include "league_club.h"
#include "event_data.h"
#include "item.h"
#include "money.h"
#include "rtc.h"
#include "societe.h"
#include "string_util.h"
#include "constants/characters.h"
#include "constants/flags.h"
#include "constants/items.h"
#include "constants/species.h"
#include "constants/trainers.h"

// Émeraude Couronnement : Ligue des Clubs, phase 2 (v0.43) : le joueur propriétaire.
// Rachat avec la trésorerie de sa société (le « compte pro »), bilan de chaque saison (billetterie, droits TV et sponsors,
// boutique, salaires, entretien du stade, impôt de 20 %) versé à la société, stade en 4 niveaux, recrutement de dresseurs
// déjà battus (vivier de 20) sur les places de Capitaine et de Titulaires, coaching aux Bonbons Exp.

#define LC  (gSaveBlock3Ptr->ligueClubs)
#define SOC (gSaveBlock2Ptr->pokedex.societe)
#define MAXI 2000000000

static const u32 sPrixRachat[CLUB_NB_TIERS] = {250000, 450000, 750000, 1200000};
static const u32 sTVBase[CLUB_NB_TIERS] = {20000, 40000, 60000, 80000};
static const u32 sPlaces[CLUB_NB_STADES] = {1000, 4000, 10000, 25000};
static const u32 sBillet[CLUB_NB_STADES] = {50, 80, 110, 150};
static const u32 sEntretien[CLUB_NB_STADES] = {8000, 15000, 25000, 40000};
static const u32 sTravaux[CLUB_NB_STADES] = {0, 150000, 400000, 900000}; // pour atteindre ce niveau
#define TV_PAR_POINT     1500
#define TV_FINALE        50000
#define MATCHS_DOMICILE  4

static const u8 *const sNomsStades[CLUB_NB_STADES] =
{
    COMPOUND_STRING("Municipal"), COMPOUND_STRING("Régional"), COMPOUND_STRING("Arène Nationale"), COMPOUND_STRING("Colisée Mondial"),
};

// Vivier : Espoirs de l'Académie (55-68, 2e badge), Vétérans (69-79, 5e badge), Champions d'Arène (80-89, leur badge),
// Conseil 4 et Maître (90-96, après la Ligue).
static const struct ClubDresseur sRecrues[CLUB_NB_RECRUES] =
{
    {COMPOUND_STRING("Hugo"), TRAINER_CLASS_COOLTRAINER, TRAINER_PIC_COOLTRAINER_M, FALSE, 58,
     {SPECIES_MACHOP, SPECIES_ELECTRIKE, SPECIES_SNORUNT, SPECIES_BAGON, SPECIES_MAKUHITA, SPECIES_NUMEL}},
    {COMPOUND_STRING("Jade"), TRAINER_CLASS_COOLTRAINER, TRAINER_PIC_COOLTRAINER_F, TRUE, 61,
     {SPECIES_RALTS, SPECIES_SWABLU, SPECIES_SPHEAL, SPECIES_SKITTY, SPECIES_TRAPINCH, SPECIES_CARVANHA}},
    {COMPOUND_STRING("Nathan"), TRAINER_CLASS_BLACK_BELT, TRAINER_PIC_BLACK_BELT, FALSE, 64,
     {SPECIES_MACHOP, SPECIES_MEDITITE, SPECIES_TIMBURR, SPECIES_RIOLU, SPECIES_MAKUHITA, SPECIES_MANKEY}},
    {COMPOUND_STRING("Inès"), TRAINER_CLASS_BATTLE_GIRL, TRAINER_PIC_BATTLE_GIRL, TRUE, 67,
     {SPECIES_MAKUHITA, SPECIES_TYROGUE, SPECIES_MIENFOO, SPECIES_CROAGUNK, SPECIES_SCRAGGY, SPECIES_PANCHAM}},
    {COMPOUND_STRING("Gaspard"), TRAINER_CLASS_EXPERT, TRAINER_PIC_EXPERT_M, FALSE, 70,
     {SPECIES_ARON, SPECIES_LARVITAR, SPECIES_BAGON, SPECIES_GIBLE, SPECIES_BELDUM, SPECIES_DRATINI}},
    {COMPOUND_STRING("Odette"), TRAINER_CLASS_EXPERT, TRAINER_PIC_EXPERT_F, TRUE, 73,
     {SPECIES_SNEASEL, SPECIES_MISDREAVUS, SPECIES_MURKROW, SPECIES_HOUNDOUR, SPECIES_ABSOL, SPECIES_DEINO}},
    {COMPOUND_STRING("Léon"), TRAINER_CLASS_GENTLEMAN, TRAINER_PIC_GENTLEMAN, FALSE, 76,
     {SPECIES_GROWLITHE, SPECIES_PONYTA, SPECIES_HOUNDOUR, SPECIES_SKARMORY, SPECIES_SCYTHER, SPECIES_DRATINI}},
    {COMPOUND_STRING("Mireille"), TRAINER_CLASS_LADY, TRAINER_PIC_LADY, TRUE, 78,
     {SPECIES_RALTS, SPECIES_TOGEPI, SPECIES_CHANSEY, SPECIES_SNORLAX, SPECIES_LAPRAS, SPECIES_GOOMY}},
    {COMPOUND_STRING("Roxanne"), TRAINER_CLASS_LEADER, TRAINER_PIC_LEADER_ROXANNE, TRUE, 80,
     {SPECIES_GEODUDE, SPECIES_NOSEPASS, SPECIES_ARON, SPECIES_LILEEP, SPECIES_ANORITH, SPECIES_ROGGENROLA}},
    {COMPOUND_STRING("Bastien"), TRAINER_CLASS_LEADER, TRAINER_PIC_LEADER_BRAWLY, FALSE, 81,
     {SPECIES_MACHOP, SPECIES_MAKUHITA, SPECIES_MEDITITE, SPECIES_TIMBURR, SPECIES_RIOLU, SPECIES_MANKEY}},
    {COMPOUND_STRING("Voltère"), TRAINER_CLASS_LEADER, TRAINER_PIC_LEADER_WATTSON, FALSE, 82,
     {SPECIES_MAGNEMITE, SPECIES_VOLTORB, SPECIES_ELECTRIKE, SPECIES_SHINX, SPECIES_ROTOM, SPECIES_MAREEP}},
    {COMPOUND_STRING("Adriane"), TRAINER_CLASS_LEADER, TRAINER_PIC_LEADER_FLANNERY, TRUE, 84,
     {SPECIES_NUMEL, SPECIES_SLUGMA, SPECIES_TORKOAL, SPECIES_VULPIX, SPECIES_GROWLITHE, SPECIES_LITWICK}},
    {COMPOUND_STRING("Norman"), TRAINER_CLASS_LEADER, TRAINER_PIC_LEADER_NORMAN, FALSE, 87,
     {SPECIES_SLAKOTH, SPECIES_ZIGZAGOON, SPECIES_KANGASKHAN, SPECIES_SPINDA, SPECIES_TAUROS, SPECIES_SNORLAX}},
    {COMPOUND_STRING("Alizée"), TRAINER_CLASS_LEADER, TRAINER_PIC_LEADER_WINONA, TRUE, 85,
     {SPECIES_SWABLU, SPECIES_TAILLOW, SPECIES_SKARMORY, SPECIES_TROPIUS, SPECIES_WINGULL, SPECIES_STARLY}},
    {COMPOUND_STRING("Juan"), TRAINER_CLASS_LEADER, TRAINER_PIC_LEADER_JUAN, FALSE, 89,
     {SPECIES_SPHEAL, SPECIES_HORSEA, SPECIES_CORPHISH, SPECIES_LAPRAS, SPECIES_POLIWAG, SPECIES_WAILMER}},
    {COMPOUND_STRING("Damien"), TRAINER_CLASS_ELITE_FOUR, TRAINER_PIC_ELITE_FOUR_SIDNEY, FALSE, 90,
     {SPECIES_POOCHYENA, SPECIES_SEEDOT, SPECIES_SCRAGGY, SPECIES_ABSOL, SPECIES_SNEASEL, SPECIES_DEINO}},
    {COMPOUND_STRING("Spectra"), TRAINER_CLASS_ELITE_FOUR, TRAINER_PIC_ELITE_FOUR_PHOEBE, TRUE, 91,
     {SPECIES_DUSKULL, SPECIES_SHUPPET, SPECIES_SABLEYE, SPECIES_GASTLY, SPECIES_LITWICK, SPECIES_DRIFLOON}},
    {COMPOUND_STRING("Glacia"), TRAINER_CLASS_ELITE_FOUR, TRAINER_PIC_ELITE_FOUR_GLACIA, TRUE, 92,
     {SPECIES_SNORUNT, SPECIES_SPHEAL, SPECIES_SNOVER, SPECIES_SWINUB, SPECIES_VANILLITE, SPECIES_FRIGIBAX}},
    {COMPOUND_STRING("Aragon"), TRAINER_CLASS_ELITE_FOUR, TRAINER_PIC_ELITE_FOUR_DRAKE, FALSE, 94,
     {SPECIES_BAGON, SPECIES_GIBLE, SPECIES_DRATINI, SPECIES_AXEW, SPECIES_DEINO, SPECIES_SWABLU}},
    {COMPOUND_STRING("Marc"), TRAINER_CLASS_CHAMPION, TRAINER_PIC_STEVEN, FALSE, 96,
     {SPECIES_BELDUM, SPECIES_SKARMORY, SPECIES_ARON, SPECIES_ANORITH, SPECIES_LILEEP, SPECIES_BALTOY}},
};

static const u16 sConditions[CLUB_NB_RECRUES] =
{
    FLAG_BADGE02_GET, FLAG_BADGE02_GET, FLAG_BADGE02_GET, FLAG_BADGE02_GET,
    FLAG_BADGE05_GET, FLAG_BADGE05_GET, FLAG_BADGE05_GET, FLAG_BADGE05_GET,
    FLAG_BADGE01_GET, FLAG_BADGE02_GET, FLAG_BADGE03_GET, FLAG_BADGE04_GET, FLAG_BADGE05_GET, FLAG_BADGE06_GET, FLAG_BADGE08_GET,
    FLAG_SYS_GAME_CLEAR, FLAG_SYS_GAME_CLEAR, FLAG_SYS_GAME_CLEAR, FLAG_SYS_GAME_CLEAR, FLAG_SYS_GAME_CLEAR,
};

// ---------------------------------------------------------------- outils

static u32 Borne(u64 v)
{
    return v > MAXI ? MAXI : (u32)v;
}

static u32 Disponible(void)
{
    return Borne((u64)GetMoney(&gSaveBlock1Ptr->money) + (SOC.etat != SOCIETE_ETAT_AUCUNE ? SOC.tresorerie : 0));
}

// Paiement : trésorerie de la société d'abord, puis l'argent du joueur. Renvoie le montant pris (au plus le disponible).
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

static void Encaisser(u32 montant)
{
    if (SOC.etat != SOCIETE_ETAT_AUCUNE)
        SOC.tresorerie = Borne((u64)SOC.tresorerie + montant);
    else
        AddMoney(&gSaveBlock1Ptr->money, montant);
}

static void Ajouter(u32 *v, u32 n)
{
    *v = Borne((u64)*v + n);
}

// ---------------------------------------------------------------- vivier

const struct ClubDresseur *Club_Recrue(u32 recrue)
{
    return &sRecrues[recrue % CLUB_NB_RECRUES];
}

bool32 Club_RecrueDisponible(u32 recrue)
{
    return recrue < CLUB_NB_RECRUES && FlagGet(sConditions[recrue]);
}

static bool32 DejaRecrutee(u32 recrue)
{
    u32 p;
    for (p = 0; p < CLUB_PLACES_RECRUTEMENT; p++)
        if (LC.effectif[p] == recrue + 1)
            return TRUE;
    return FALSE;
}

// Salaire par saison : rating² x 5 (rating 58 : 16 820 ¥ ; 90 : 40 500 ¥). Le joueur ne se paie pas.
u32 Club_Salaire(u32 club, u32 slot)
{
    u32 r;
    if (slot >= CLUB_PLACES_RECRUTEMENT)
        return 0;
    r = min(Club_Rating(club, slot), 100);
    return r * r * 5;
}

// ---------------------------------------------------------------- rachat et revente

u32 Club_PrixRachat(u32 club)
{
    return sPrixRachat[Club_Tier(club)];
}

bool32 Club_PeutRacheter(void)
{
    return LC.magie == CLUB_MAGIE && Club_Titres() >= CLUB_TITRES_REQUIS && SOC.etat != SOCIETE_ETAT_AUCUNE
        && Club_Proprio() == CLUB_AUCUN;
}

bool32 Club_Racheter(u32 club)
{
    u32 prix;
    if (club >= CLUB_NB || !Club_PeutRacheter())
        return FALSE;
    prix = Club_PrixRachat(club);
    if (Disponible() < prix)
        return FALSE;
    Prendre(prix);
    if (Club_DuJoueur() != club)
        Club_QuitterClub(); // un duel en attente dans l'ancien club est joué par sa Recrue
    LC.proprio = club + 1;
    LC.club = club + 1;
    LC.stade = 0;
    LC.drapeaux &= ~CLUB_DRAP_BOUTIQUE;
    memset(LC.effectif, 0, sizeof(LC.effectif));
    memset(LC.coaching, 0, sizeof(LC.coaching));
    memset(LC.bilan, 0, sizeof(LC.bilan));
    LC.investi = prix;
    return TRUE;
}

u32 Club_Valeur(void)
{
    return Club_Proprio() == CLUB_AUCUN ? 0 : LC.investi;
}

// Revente : 80 % de ce qui a été investi, versés à la société. Le joueur reste licencié du club pour la saison.
u32 Club_Revendre(void)
{
    u32 montant;
    if (Club_Proprio() == CLUB_AUCUN)
        return 0;
    montant = (u64)LC.investi * CLUB_REVENTE_POURCENT / 100;
    Encaisser(montant);
    LC.proprio = 0;
    LC.investi = 0;
    LC.stade = 0;
    LC.drapeaux &= ~CLUB_DRAP_BOUTIQUE;
    memset(LC.effectif, 0, sizeof(LC.effectif));
    memset(LC.coaching, 0, sizeof(LC.coaching));
    return montant;
}

// ---------------------------------------------------------------- stade et boutique

u32 Club_Places(void)       { return sPlaces[LC.stade % CLUB_NB_STADES]; }
u32 Club_PrixBillet(void)   { return sBillet[LC.stade % CLUB_NB_STADES]; }
u32 Club_Entretien(void)    { return sEntretien[LC.stade % CLUB_NB_STADES]; }

u32 Club_CoutTravaux(void)
{
    return LC.stade + 1 < CLUB_NB_STADES ? sTravaux[LC.stade + 1] : 0;
}

const u8 *Club_NomNiveauStade(void)
{
    return sNomsStades[LC.stade % CLUB_NB_STADES];
}

bool32 Club_AgrandirStade(void)
{
    u32 cout = Club_CoutTravaux();
    if (Club_Proprio() == CLUB_AUCUN || cout == 0 || Disponible() < cout)
        return FALSE;
    Prendre(cout);
    LC.stade++;
    Ajouter(&LC.investi, cout);
    return TRUE;
}

bool32 Club_AcheterBoutique(void)
{
    if (Club_Proprio() == CLUB_AUCUN || (LC.drapeaux & CLUB_DRAP_BOUTIQUE) || Disponible() < CLUB_PRIX_BOUTIQUE)
        return FALSE;
    Prendre(CLUB_PRIX_BOUTIQUE);
    LC.drapeaux |= CLUB_DRAP_BOUTIQUE;
    Ajouter(&LC.investi, CLUB_PRIX_BOUTIQUE);
    return TRUE;
}

// ---------------------------------------------------------------- recrutement et coaching

// Recruter coûte une saison de salaire d'avance (prime de signature). La place garde sa forme, le coaching repart à zéro.
bool32 Club_Recruter(u32 place, u32 recrue)
{
    u32 club = Club_Proprio();
    if (club == CLUB_AUCUN || place >= CLUB_PLACES_RECRUTEMENT)
        return FALSE;
    if (recrue >= CLUB_NB_RECRUES) // retour du dresseur d'origine
    {
        if (LC.effectif[place] == 0)
            return FALSE;
        LC.effectif[place] = 0;
        LC.coaching[place] = 0;
        return TRUE;
    }
    if (!Club_RecrueDisponible(recrue) || DejaRecrutee(recrue))
        return FALSE;
    {
        u32 r = sRecrues[recrue].rating, prime = r * r * 5;
        if (Disponible() < prime)
            return FALSE;
        Prendre(prime);
    }
    LC.effectif[place] = recrue + 1;
    LC.coaching[place] = 0;
    return TRUE;
}

bool32 Club_Coacher(u32 place, u32 bonus)
{
    if (Club_Proprio() == CLUB_AUCUN || place >= CLUB_PLACES_RECRUTEMENT || LC.coaching[place] >= CLUB_COACHING_MAX
     || Club_Rating(Club_Proprio(), place) >= 100)
        return FALSE;
    LC.coaching[place] = min(LC.coaching[place] + bonus, CLUB_COACHING_MAX);
    return TRUE;
}

// ---------------------------------------------------------------- bilan

// Lignes : billetterie, TV et sponsors, boutique, charges (salaires + entretien), impôt, résultat (s32).
void Club_Previsionnel(u32 points, bool32 finale, u32 *lignes)
{
    u32 club = Club_Proprio(), tier, taux, p, charges = 0;
    s64 benefice;

    memset(lignes, 0, 6 * sizeof(u32));
    if (club == CLUB_AUCUN)
        return;
    tier = Club_Tier(club);
    taux = min(50 + points * 50 / 120, 100); // remplissage : 50 % à 100 % selon les points
    lignes[CLUB_BILAN_BILLETTERIE] = Borne((u64)MATCHS_DOMICILE * Club_Places() * taux / 100 * Club_PrixBillet());
    lignes[CLUB_BILAN_TV] = Borne((u64)sTVBase[tier] + TV_PAR_POINT * points + (finale ? TV_FINALE : 0));
    lignes[CLUB_BILAN_BOUTIQUE] = lignes[CLUB_BILAN_BILLETTERIE] * ((LC.drapeaux & CLUB_DRAP_BOUTIQUE) ? 25 : 10) / 100;
    for (p = 0; p < CLUB_PLACES_RECRUTEMENT; p++)
        charges += Club_Salaire(club, p);
    charges += Club_Entretien();
    lignes[CLUB_BILAN_CHARGES] = charges;
    benefice = (s64)lignes[CLUB_BILAN_BILLETTERIE] + lignes[CLUB_BILAN_TV] + lignes[CLUB_BILAN_BOUTIQUE] - charges;
    lignes[CLUB_BILAN_IMPOT] = benefice > 0 ? (u32)(benefice * CLUB_IMPOT_POURCENT / 100) : 0;
    benefice -= lignes[CLUB_BILAN_IMPOT];
    if (benefice > MAXI)
        benefice = MAXI;
    if (benefice < -MAXI)
        benefice = -MAXI;
    lignes[CLUB_BILAN_NET] = (u32)(s32)benefice;
}

// Fin de saison : résultat net versé à la société (dividende) ou perte payée par elle (puis par le joueur).
void Club_Bilan(void)
{
    u8 dom, ext;
    u32 club = Club_Proprio();
    bool32 finale;

    if (club == CLUB_AUCUN)
        return;
    Club_Affiche(CLUB_MATCH_FINALE, &dom, &ext);
    finale = (LC.duels[CLUB_MATCH_FINALE] != 0) && (dom == club || ext == club);
    Club_Previsionnel(Club_Points(club), finale, LC.bilan);
    if ((s32)LC.bilan[CLUB_BILAN_NET] >= 0)
        Encaisser(LC.bilan[CLUB_BILAN_NET]);
    else
        Prendre(-(s32)LC.bilan[CLUB_BILAN_NET]);
    LC.rapport |= CLUB_RAPPORT_BILAN;
}

// ---------------------------------------------------------------- textes et specials

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

static const u8 sText_Yens[] = _(" ¥");
static const u8 sText_Point[] = _(".");

static u8 *Yens(u8 *fin, u32 n)
{
    fin = Nombre(fin, n);
    return StringCopy(fin, sText_Yens);
}

static u8 *YensSignes(u8 *fin, s32 n)
{
    if (n < 0)
    {
        *fin++ = CHAR_HYPHEN;
        n = -n;
    }
    return Yens(fin, n);
}

// Rapport de fin de saison (appelé par Club_Ouvrir).
static const u8 sText_B1[] = _("\pBilan du club : recettes ");
static const u8 sText_B2[] = _(",\ncharges et impôt ");
static const u8 sText_B3[] = _(".\pRésultat versé à ta société :\n");

u8 *Club_TexteBilan(u8 *fin)
{
    u32 recettes = Borne((u64)LC.bilan[CLUB_BILAN_BILLETTERIE] + LC.bilan[CLUB_BILAN_TV] + LC.bilan[CLUB_BILAN_BOUTIQUE]);
    fin = StringCopy(fin, sText_B1);
    fin = Yens(fin, recettes);
    fin = StringCopy(fin, sText_B2);
    fin = Yens(fin, Borne((u64)LC.bilan[CLUB_BILAN_CHARGES] + LC.bilan[CLUB_BILAN_IMPOT]));
    fin = StringCopy(fin, sText_B3);
    fin = YensSignes(fin, (s32)LC.bilan[CLUB_BILAN_NET]);
    return StringCopy(fin, sText_Point);
}

EWRAM_DATA u8 gClubLignes[CLUB_NB_RECRUES + 1][28] = {0};

static const u8 sText_PasProprio1[] = _("Pour racheter un club : ");
static const u8 sText_PasProprio2[] = _(" titre(s)\nde la Ligue des Clubs et une société.\pTes titres : ");
static const u8 sText_PasProprio3[] = _(". Société : ");
static const u8 sText_Oui[] = _("oui");
static const u8 sText_Non[] = _("non");
static const u8 sText_PasProprio4[] = _(".\nLe prix se paie avec ta société.");

// VAR_RESULT = 0 : conditions pas remplies (gStringVar4 explique), 1 : peut racheter, 2 : déjà propriétaire.
void Club_ScriptPropriete(void)
{
    u8 *fin;
    Club_MettreAJourJusqua(RtcGetLocalDayCount());
    if (Club_Proprio() != CLUB_AUCUN)
    {
        gSpecialVar_Result = 2;
        return;
    }
    if (Club_PeutRacheter())
    {
        gSpecialVar_Result = 1;
        return;
    }
    fin = StringCopy(gStringVar4, sText_PasProprio1);
    fin = Nombre(fin, CLUB_TITRES_REQUIS);
    fin = StringCopy(fin, sText_PasProprio2);
    fin = Nombre(fin, Club_Titres());
    fin = StringCopy(fin, sText_PasProprio3);
    fin = StringCopy(fin, SOC.etat != SOCIETE_ETAT_AUCUNE ? sText_Oui : sText_Non);
    StringCopy(fin, sText_PasProprio4);
    gSpecialVar_Result = 0;
}

static const u8 sText_R1[] = _("\nest à vendre : ");
static const u8 sText_R2[] = _(".\pTu le rachètes avec ta société ?");

// VAR_0x8004 = club : fiche de rachat dans gStringVar4.
void Club_FicheRachat(void)
{
    u32 club = gSpecialVar_0x8004 % CLUB_NB;
    u8 *fin = StringCopy(gStringVar4, Club_Nom(club));
    fin = StringCopy(fin, sText_R1);
    fin = Yens(fin, Club_PrixRachat(club));
    StringCopy(fin, sText_R2);
}

static const u8 sText_Rachete[] = _("Félicitations, patron ! Le club est\nà toi. Tu joues toujours la Recrue.");
static const u8 sText_PasAssez[] = _("Pas assez d'argent dans ta société\net sur toi.");

void Club_ScriptRacheter(void)
{
    gSpecialVar_Result = Club_Racheter(gSpecialVar_0x8004 % CLUB_NB);
    StringCopy(gStringVar4, gSpecialVar_Result ? sText_Rachete : sText_PasAssez);
}

static const u8 sText_G2[] = _(".\nRésultat prévu : ");
static const u8 sText_G3[] = _(" / saison.");

// Résumé du propriétaire dans gStringVar4.
void Club_ResumeProprio(void)
{
    u32 lignes[6], club = Club_Proprio();
    u8 *fin;
    if (club == CLUB_AUCUN)
        return;
    Club_Previsionnel(Club_Points(club), FALSE, lignes);
    fin = StringCopy(gStringVar4, Club_Nom(club));
    fin = StringCopy(fin, sText_G2);
    fin = YensSignes(fin, (s32)lignes[CLUB_BILAN_NET]);
    StringCopy(fin, sText_G3);
}

static const u8 sText_Verrou[] = _("??? (pas encore battu)");
static const u8 sText_Pris[] = _(" (déjà au club)");
static const u8 sText_Espace[] = _(" ");
static const u8 sText_K[] = _("k ¥");

// gClubLignes : « Nom R.80 32k ¥ » pour les 20 recrues.
void Club_PreparerVivier(void)
{
    u32 i;
    for (i = 0; i < CLUB_NB_RECRUES; i++)
    {
        u8 *fin = gClubLignes[i];
        u32 r = sRecrues[i].rating;
        if (!Club_RecrueDisponible(i))
        {
            StringCopy(fin, sText_Verrou);
            continue;
        }
        fin = StringCopy(fin, sRecrues[i].nom);
        *fin++ = CHAR_SPACE;
        *fin++ = CHAR_R;
        *fin++ = CHAR_PERIOD;
        fin = Nombre(fin, r);
        if (DejaRecrutee(i))
        {
            StringCopy(fin, sText_Pris);
            continue;
        }
        fin = StringCopy(fin, sText_Espace);
        fin = Nombre(fin, r * r * 5 / 1000);
        StringCopy(fin, sText_K);
    }
}

static const u8 sText_RecruteOK[] = _("Contrat signé ! La prime d'une\nsaison de salaire est payée.");
static const u8 sText_RecruteRetour[] = _("Le dresseur d'origine reprend\nsa place.");
static const u8 sText_RecruteNon[] = _("Impossible : pas encore battu,\ndéjà au club ou pas assez d'argent.");

// VAR_0x8005 = place (0-2), VAR_0x8006 = recrue (20 = rendre la place).
void Club_ScriptRecruter(void)
{
    u32 recrue = gSpecialVar_0x8006;
    gSpecialVar_Result = Club_Recruter(gSpecialVar_0x8005, recrue);
    StringCopy(gStringVar4, !gSpecialVar_Result ? sText_RecruteNon : (recrue >= CLUB_NB_RECRUES ? sText_RecruteRetour : sText_RecruteOK));
}

static const u8 sText_CoachOK[] = _("Séance réussie !\nRating de ");
static const u8 sText_CoachOK2[] = _(" : ");
static const u8 sText_CoachPasBonbon[] = _("Il te faut ce Bonbon Exp. dans\nton sac.");
static const u8 sText_Plus3[] = _("+3 de rating.");
static const u8 sText_CoachMax[] = _("Ce dresseur a atteint son maximum\nde coaching.");

// VAR_0x8005 = place, VAR_0x8006 = 0 : Bonbon Exp. L (+1), 1 : Bonbon Exp. XL (+2).
void Club_ScriptCoacher(void)
{
    u32 item = gSpecialVar_0x8006 ? ITEM_EXP_CANDY_XL : ITEM_EXP_CANDY_L, place = gSpecialVar_0x8005 % CLUB_PLACES_RECRUTEMENT;
    u8 *fin;
    gSpecialVar_Result = FALSE;
    if (!CheckBagHasItem(item, 1))
    {
        StringCopy(gStringVar4, sText_CoachPasBonbon);
        return;
    }
    if (!Club_Coacher(place, gSpecialVar_0x8006 ? 2 : 1))
    {
        StringCopy(gStringVar4, sText_CoachMax);
        return;
    }
    RemoveBagItem(item, 1);
    fin = StringCopy(gStringVar4, sText_CoachOK);
    fin = StringCopy(fin, Club_NomDresseur(Club_Proprio(), place));
    fin = StringCopy(fin, sText_CoachOK2);
    fin = Nombre(fin, Club_Rating(Club_Proprio(), place));
    StringCopy(fin, sText_Point);
    gSpecialVar_Result = TRUE;
}

// Objets compétitifs : le premier de la liste présent dans le sac est donné (+3 de rating de coaching).
static const u16 sObjetsCoaching[] =
{
    ITEM_LEFTOVERS, ITEM_LIFE_ORB, ITEM_CHOICE_SCARF, ITEM_CHOICE_BAND, ITEM_CHOICE_SPECS, ITEM_FOCUS_SASH,
    ITEM_ASSAULT_VEST, ITEM_ROCKY_HELMET, ITEM_WEAKNESS_POLICY, ITEM_EXPERT_BELT, ITEM_MUSCLE_BAND, ITEM_WISE_GLASSES,
};
static const u8 sText_ObjetOK[] = _(" reçoit l'objet :\n");
static const u8 sText_ObjetAucun[] = _("Aucun objet de combat dans ton sac\n(Restes, Orbe Vie, Choix...).");

// VAR_0x8005 = place.
void Club_ScriptCoacherObjet(void)
{
    u32 i, place = gSpecialVar_0x8005 % CLUB_PLACES_RECRUTEMENT;
    u8 *fin;
    gSpecialVar_Result = FALSE;
    for (i = 0; i < ARRAY_COUNT(sObjetsCoaching); i++)
        if (CheckBagHasItem(sObjetsCoaching[i], 1))
            break;
    if (i == ARRAY_COUNT(sObjetsCoaching))
    {
        StringCopy(gStringVar4, sText_ObjetAucun);
        return;
    }
    if (!Club_Coacher(place, 3))
    {
        StringCopy(gStringVar4, sText_CoachMax);
        return;
    }
    RemoveBagItem(sObjetsCoaching[i], 1);
    fin = StringCopy(gStringVar4, Club_NomDresseur(Club_Proprio(), place));
    fin = StringCopy(fin, sText_ObjetOK);
    fin = CopyItemName(sObjetsCoaching[i], fin);
    fin = StringCopy(fin, sText_CoachOK2);
    fin = StringCopy(fin, sText_Plus3);
    gSpecialVar_Result = TRUE;
}

static const u8 sText_T1[] = _("Passer au niveau ");
static const u8 sText_T2[] = _(" :\n");
static const u8 sText_T3[] = _(" places, travaux ");
static const u8 sText_T4[] = _(".\pOn lance les travaux ?");
static const u8 sText_TMax[] = _("Ton stade est déjà un Colisée\nMondial !");

// VAR_RESULT = travaux possibles ; gStringVar4 = devis.
void Club_DevisStade(void)
{
    u8 *fin;
    if (Club_CoutTravaux() == 0)
    {
        StringCopy(gStringVar4, sText_TMax);
        gSpecialVar_Result = FALSE;
        return;
    }
    fin = StringCopy(gStringVar4, sText_T1);
    fin = StringCopy(fin, sNomsStades[LC.stade + 1]);
    fin = StringCopy(fin, sText_T2);
    fin = Nombre(fin, sPlaces[LC.stade + 1]);
    fin = StringCopy(fin, sText_T3);
    fin = Yens(fin, Club_CoutTravaux());
    StringCopy(fin, sText_T4);
    gSpecialVar_Result = TRUE;
}

static const u8 sText_TravauxOK[] = _("Travaux terminés ! Plus de places,\nbillets plus chers.");
static const u8 sText_BoutiqueOK[] = _("Tenues officielles et Balls Devon\nen boutique : 25 % de la billetterie.");
static const u8 sText_BoutiqueDeja[] = _("La boutique officielle est déjà\nouverte, ou il manque de l'argent.");

void Club_ScriptAgrandir(void)
{
    gSpecialVar_Result = Club_AgrandirStade();
    StringCopy(gStringVar4, gSpecialVar_Result ? sText_TravauxOK : sText_PasAssez);
}

void Club_ScriptBoutique(void)
{
    gSpecialVar_Result = Club_AcheterBoutique();
    StringCopy(gStringVar4, gSpecialVar_Result ? sText_BoutiqueOK : sText_BoutiqueDeja);
}

static const u8 sText_Vente1[] = _("Club revendu. Ta société touche\n");
static const u8 sText_Vente2[] = _(". Tu restes licencié du club.");

void Club_ScriptRevendre(void)
{
    u8 *fin = StringCopy(gStringVar4, sText_Vente1);
    fin = Yens(fin, Club_Revendre());
    StringCopy(fin, sText_Vente2);
}
