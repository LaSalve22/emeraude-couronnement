#include "global.h"
#include "arenes.h"
#include "societe.h"
#include "bourse.h"
#include "immobilier.h"
#include "commerces.h"
#include "event_data.h"
#include "main.h"
#include "money.h"
#include "naming_screen.h"
#include "overworld.h"
#include "rtc.h"
#include "string_util.h"
#include "constants/characters.h"

// Émeraude Couronnement : société du joueur (v0.35).
// Créée pour 20 000 ¥ avec un nom au choix, elle encaisse les loyers de l'immobilier dans sa trésorerie.
// Valeur = biens + commerces (investissement, v0.36) + trésorerie ; cours d'une action = valeur x humeur du marché / 10 000 actions.
// Une fois la valeur à 100 000 ¥, introduction en Bourse : 25 % des actions vendues au public (argent pour le joueur).
// Ensuite on vend / rachète par paquets de 500 (3 % de frais), sans jamais descendre sous 51 %.
// Retirer la trésorerie verse au joueur sa part (le reste revient aux autres actionnaires).

#define SOC (gSaveBlock2Ptr->pokedex.societe)

STATIC_ASSERT(sizeof(struct BourseSave) + sizeof(struct SocieteSave) <= 0x68, SocieteSaveTropGrande);

static u32 Melanger(u32 x)
{
    x ^= x >> 16;
    x *= 0x7FEB352D;
    x ^= x >> 15;
    x *= 0x846CA68B;
    x ^= x >> 16;
    return x;
}

static u32 Alea(u32 jour, u32 tirage)
{
    const u8 *id = gSaveBlock2Ptr->playerTrainerId;
    u32 graine = id[0] | (id[1] << 8) | (id[2] << 16) | (id[3] << 24);
    return Melanger((jour * 0xC2B2AE35u) ^ graine ^ (tirage << 16) ^ 0x50C1E7E);
}

static u32 Aujourdhui(void)
{
    return RtcGetLocalDayCount();
}

static u32 Borner(u64 v, u32 min, u32 max)
{
    return v < min ? min : (v > max ? max : v);
}

void Societe_Creer(const u8 *nom, u32 jour)
{
    memset(&SOC, 0, sizeof(SOC));
    StringCopyN(SOC.nom, nom, SOCIETE_NOM_LONGUEUR);
    SOC.nom[SOCIETE_NOM_LONGUEUR] = EOS;
    SOC.etat = SOCIETE_ETAT_CREEE;
    SOC.actionsJoueur = SOCIETE_ACTIONS;
    SOC.sentiment = 1000;
    SOC.jour = jour;
}

// Humeur du marché : ±1,5 % par jour, plus 0,3 % si la trésorerie a de l'argent (la boîte tourne).
void Societe_MettreAJourJusqua(u32 aujourdhui)
{
    if (SOC.etat == SOCIETE_ETAT_AUCUNE)
        return;
    if (SOC.sentiment == 0)
        SOC.sentiment = 1000;
    if (aujourdhui < SOC.jour)
        SOC.jour = aujourdhui;
    if (aujourdhui - SOC.jour > BOURSE_RATTRAPAGE_MAX)
        SOC.jour = aujourdhui - BOURSE_RATTRAPAGE_MAX;
    while (SOC.jour < aujourdhui)
    {
        s32 variation;
        SOC.jour++;
        variation = (s32)(Alea(SOC.jour, 0) % 31) - 15;
        if (SOC.tresorerie > 0)
            variation += 3;
        SOC.sentiment = Borner((u64)SOC.sentiment * (1000 + variation) / 1000, SOCIETE_SENTIMENT_MIN, SOCIETE_SENTIMENT_MAX);
    }
}

u32 Societe_Valeur(u32 jour)
{
    if (SOC.etat == SOCIETE_ETAT_AUCUNE)
        return 0;
    return Borner((u64)Immo_ValeurBiens(jour) + Commerce_Valeur() + Arene_Valeur() + SOC.tresorerie, 0, 2000000000);
}

u32 Societe_Cours(u32 jour)
{
    return Borner((u64)Societe_Valeur(jour) * SOC.sentiment / 1000 / SOCIETE_ACTIONS, 1, 9999999);
}

u32 Societe_RetirerTresorerie(void)
{
    u32 part, place;

    if (SOC.etat == SOCIETE_ETAT_AUCUNE || SOC.tresorerie == 0)
        return 0;
    part = (u64)SOC.tresorerie * SOC.actionsJoueur / SOCIETE_ACTIONS;
    place = MAX_MONEY - GetMoney(&gSaveBlock1Ptr->money);
    if (part > place)
    {
        // On ne retire que ce qui tient dans le portefeuille ; le reste attend dans la trésorerie.
        u32 retire = (u64)place * SOCIETE_ACTIONS / SOC.actionsJoueur;
        SOC.tresorerie -= retire;
        AddMoney(&gSaveBlock1Ptr->money, place);
        return place;
    }
    SOC.tresorerie = 0;
    AddMoney(&gSaveBlock1Ptr->money, part);
    return part;
}

static bool32 VendreAuPublic(u32 actions, u32 jour)
{
    u32 net = (u64)actions * Societe_Cours(jour) * (100 - SOCIETE_FRAIS_POURCENT) / 100;

    if (SOC.actionsJoueur < SOCIETE_MIN_JOUEUR + actions)
        return FALSE;
    if (GetMoney(&gSaveBlock1Ptr->money) + (u64)net > MAX_MONEY)
        return FALSE;
    SOC.actionsJoueur -= actions;
    AddMoney(&gSaveBlock1Ptr->money, net);
    return TRUE;
}

bool32 Societe_Introduire(u32 jour)
{
    if (SOC.etat != SOCIETE_ETAT_CREEE || Societe_Valeur(jour) < SOCIETE_INTRO_VALEUR)
        return FALSE;
    if (!VendreAuPublic(SOCIETE_INTRO_ACTIONS, jour))
        return FALSE;
    SOC.etat = SOCIETE_ETAT_COTEE;
    return TRUE;
}

bool32 Societe_VendreActions(u32 jour)
{
    if (SOC.etat != SOCIETE_ETAT_COTEE)
        return FALSE;
    return VendreAuPublic(SOCIETE_PAQUET, jour);
}

bool32 Societe_RacheterActions(u32 jour)
{
    u32 cout = (u64)SOCIETE_PAQUET * Societe_Cours(jour) * (100 + SOCIETE_FRAIS_POURCENT) / 100;

    if (SOC.etat != SOCIETE_ETAT_COTEE || SOC.actionsJoueur + SOCIETE_PAQUET > SOCIETE_ACTIONS)
        return FALSE;
    if (!IsEnoughMoney(&gSaveBlock1Ptr->money, cout))
        return FALSE;
    RemoveMoney(&gSaveBlock1Ptr->money, cout);
    SOC.actionsJoueur += SOCIETE_PAQUET;
    return TRUE;
}

// ---------------------------------------------------------------- textes et specials

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

// Écran de saisie du nom ; le résultat arrive dans gStringVar2 (script : special Societe_SaisirNom, waitstate).
void Societe_SaisirNom(void)
{
    gStringVar2[0] = EOS;
    DoNamingScreen(NAMING_SCREEN_SOCIETE, gStringVar2, 0, 0, 0, CB2_ReturnToFieldContinueScript);
}

static const u8 sText_NomParDefaut[] = _("Hoenn Corp");

// VAR_RESULT = FALSE s'il manque l'argent de la création.
void Societe_ScriptCreer(void)
{
    gSpecialVar_Result = FALSE;
    if (!IsEnoughMoney(&gSaveBlock1Ptr->money, SOCIETE_COUT_CREATION))
        return;
    RemoveMoney(&gSaveBlock1Ptr->money, SOCIETE_COUT_CREATION);
    Societe_Creer(gStringVar2[0] != EOS ? gStringVar2 : sText_NomParDefaut, Aujourdhui());
    StringCopy(gStringVar1, SOC.nom);
    gSpecialVar_Result = TRUE;
}

static const u8 sText_Resume1[] = _(" : valeur ");
static const u8 sText_Resume2[] = _("\nTrésorerie : ");
static const u8 sText_Resume3[] = _("\pTu as ");
static const u8 sText_Resume4[] = _(" % des actions.\nCours : ");
static const u8 sText_Resume5[] = _(" par action.");
static const u8 sText_ResumeCotee[] = _("\lCotée en Bourse.");
static const u8 sText_ResumePrivee[] = _("\pEntrée en Bourse possible à partir\nde 100 000 ¥ de valeur.");

// Texte de l'accueil dans gStringVar4 ; VAR_RESULT = état de la société.
void Societe_Resume(void)
{
    u32 jour = Aujourdhui();
    u8 *fin;

    Immo_MettreAJourJusqua(jour);
    Societe_MettreAJourJusqua(jour);
    gSpecialVar_Result = SOC.etat;
    if (SOC.etat == SOCIETE_ETAT_AUCUNE)
        return;
    StringCopy(gStringVar1, SOC.nom);
    fin = StringCopy(gStringVar4, SOC.nom);
    fin = StringCopy(fin, sText_Resume1);
    fin = AjouterYens(fin, Societe_Valeur(jour));
    fin = StringCopy(fin, sText_Resume2);
    fin = AjouterYens(fin, SOC.tresorerie);
    fin = StringCopy(fin, sText_Resume3);
    fin = AjouterNombre(fin, SOC.actionsJoueur / 100);
    fin = StringCopy(fin, sText_Resume4);
    fin = AjouterYens(fin, Societe_Cours(jour));
    fin = StringCopy(fin, sText_Resume5);
    StringCopy(fin, SOC.etat == SOCIETE_ETAT_COTEE ? sText_ResumeCotee : sText_ResumePrivee);
}

static const u8 sText_Retire1[] = _("Tu te verses ");
static const u8 sText_Retire2[] = _(".\nLe reste revient aux actionnaires.");
static const u8 sText_RetireRien[] = _("La caisse est vide, ou ton\nportefeuille est plein.");

void Societe_ScriptRetirer(void)
{
    u32 m = Societe_RetirerTresorerie();
    u8 *fin;

    gSpecialVar_Result = m != 0;
    if (m == 0)
    {
        StringCopy(gStringVar4, sText_RetireRien);
        return;
    }
    fin = StringCopy(gStringVar4, sText_Retire1);
    fin = AjouterYens(fin, m);
    StringCopy(fin, sText_Retire2);
}

static const u8 sText_Introduite[] = _("Cloche de la Bourse ! 25 % des\nactions vendus au public.\pTa société est cotée. Le cash est\nsur ton compte.");
static const u8 sText_IntroRatee[] = _("Pas encore : il faut 100 000 ¥ de\nvaleur, et de la place dans ton\lportefeuille.");
static const u8 sText_Vendu[] = _("500 actions vendues au public.");
static const u8 sText_VenteRatee[] = _("Impossible : tu dois garder 51 %,\net ton portefeuille a une limite.");
static const u8 sText_Rachete[] = _("500 actions rachetées.");
static const u8 sText_RachatRate[] = _("Pas assez d'argent, ou tu as déjà\ntout racheté.");

void Societe_ScriptIntroduire(void)
{
    gSpecialVar_Result = Societe_Introduire(Aujourdhui());
    StringCopy(gStringVar4, gSpecialVar_Result ? sText_Introduite : sText_IntroRatee);
}

void Societe_ScriptVendre(void)
{
    gSpecialVar_Result = Societe_VendreActions(Aujourdhui());
    StringCopy(gStringVar4, gSpecialVar_Result ? sText_Vendu : sText_VenteRatee);
}

void Societe_ScriptRacheter(void)
{
    gSpecialVar_Result = Societe_RacheterActions(Aujourdhui());
    StringCopy(gStringVar4, gSpecialVar_Result ? sText_Rachete : sText_RachatRate);
}

// Ligne de la société dans la liste de la Bourse (gBourseLignes[BOURSE_NB_ENTREPRISES]). VAR_RESULT = TRUE si cotée.
void Societe_LigneBourse(void)
{
    u32 jour = Aujourdhui();
    u8 *fin;

    Societe_MettreAJourJusqua(jour);
    gSpecialVar_Result = SOC.etat == SOCIETE_ETAT_COTEE;
    if (!gSpecialVar_Result)
        return;
    fin = gBourseLignes[BOURSE_NB_ENTREPRISES];
    *fin++ = CHAR_PLUS;
    fin = StringCopy(fin, SOC.nom);
    *fin++ = CHAR_SPACE;
    fin = AjouterYens(fin, Societe_Cours(jour));
    *fin = EOS;
}
