#include "global.h"
#include "battle.h"
#include "battle_anim.h"
#include "battle_ai_record.h"
#include "battle_controllers.h"
#include "battle_message.h"
#include "battle_setup.h"
#include "battle_special.h"
#include "battle_z_move.h"
#include "data.h"
#include "event_data.h"
#include "frontier_util.h"
#include "graphics.h"
#include "international_string_util.h"
#include "item.h"
#include "link.h"
#include "menu.h"
#include "palette.h"
#include "recorded_battle.h"
#include "string_util.h"
#include "strings.h"
#include "test_runner.h"
#include "text.h"
#include "trainer_hill.h"
#include "trainer_slide.h"
#include "trainer_tower.h"
#include "window.h"
#include "line_break.h"
#include "constants/abilities.h"
#include "constants/battle_dome.h"
#include "constants/battle_string_ids.h"
#include "constants/frontier_util.h"
#include "constants/items.h"
#include "constants/moves.h"
#include "constants/opponents.h"
#include "constants/species.h"
#include "constants/trainers.h"
#include "constants/trainer_hill.h"
#include "constants/weather.h"

struct BattleWindowText
{
    u8 fillValue;
    u8 fontId;
    u8 x;
    u8 y;
    union {
        struct {
            DEPRECATED("Use color.background instead") u8 bgColor;
            DEPRECATED("Use color.foreground instead") u8 fgColor;
            DEPRECATED("Use color.shadow instead") u8 shadowColor;
            DEPRECATED("Use color.accent instead") u8 accentColor;
        };
        union TextColor color;
    };
    u8 letterSpacing;
    u8 lineSpacing;
    u8 speed;
};

#if TESTING
EWRAM_DATA u16 sBattlerAbilities[MAX_BATTLERS_COUNT] = {0};
#else
static EWRAM_DATA u16 sBattlerAbilities[MAX_BATTLERS_COUNT] = {0};
#endif
EWRAM_DATA struct BattleMsgData *gBattleMsgDataPtr = NULL;

// todo: make some of those names less vague: attacker/target vs pkmn, etc.

static const u8 sText_EmptyString4[] = _("");

const u8 gText_PkmnShroudedInMist[] = _("{B_ATK_NAME_WITH_PREFIX} s'entoure d'une brume protectrice !");
const u8 gText_PkmnGettingPumped[] = _("{B_DEF_NAME_WITH_PREFIX} se motive à bloc !");
const u8 gText_PkmnsXPreventsSwitching[] = _("{B_BUFF1} empêche de changer de Pokémon grâce à {B_LAST_ABILITY} !\p");
const u8 gText_StatSharply[] = _(" beaucoup");
const u8 gText_StatRose[] = _("augmente !");
const u8 gText_StatFell[] = _("baisse !");
const u8 gText_DefendersStatRose[] = _("La stat {B_BUFF1} de {B_DEF_NAME_WITH_PREFIX2} augmente{B_BUFF2} !");
static const u8 sText_GotAwaySafely[] = _("{PLAY_SE SE_FLEE}Tu prends la fuite !\p");
static const u8 sText_PlayerDefeatedLinkTrainer[] = _("Tu as battu {B_LINK_OPPONENT1_NAME} !");
static const u8 sText_TwoLinkTrainersDefeated[] = _("Tu as battu {B_LINK_OPPONENT1_NAME} et {B_LINK_OPPONENT2_NAME} !");
static const u8 sText_PlayerLostAgainstLinkTrainer[] = _("Tu as perdu contre {B_LINK_OPPONENT1_NAME} !");
static const u8 sText_PlayerLostToTwo[] = _("Tu as perdu contre {B_LINK_OPPONENT1_NAME} et {B_LINK_OPPONENT2_NAME} !");
static const u8 sText_PlayerBattledToDrawLinkTrainer[] = _("Match nul contre {B_LINK_OPPONENT1_NAME} !");
static const u8 sText_PlayerBattledToDrawVsTwo[] = _("Match nul contre {B_LINK_OPPONENT1_NAME} et {B_LINK_OPPONENT2_NAME} !");
static const u8 sText_WildFled[] = _("{PLAY_SE SE_FLEE}{B_LINK_OPPONENT1_NAME} s'enfuit !"); //not in gen 5+, replaced with match was forfeited text
static const u8 sText_TwoWildFled[] = _("{PLAY_SE SE_FLEE}{B_LINK_OPPONENT1_NAME} et {B_LINK_OPPONENT2_NAME} s'enfuient !"); //not in gen 5+, replaced with match was forfeited text
static const u8 sText_PlayerDefeatedLinkTrainerTrainer1[] = _("Tu as battu {B_TRAINER1_NAME_WITH_CLASS} !\p");
static const u8 sText_OpponentMon1Appeared[] = _("{B_OPPONENT_MON1_NAME} apparaît !\p");
static const u8 sText_WildPkmnAppeared[] = _("Un {B_OPPONENT_MON1_NAME} sauvage apparaît !\p");
static const u8 sText_LegendaryPkmnAppeared[] = _("Un {B_OPPONENT_MON1_NAME} sauvage apparaît !\p");
static const u8 sText_WildPkmnAppearedPause[] = _("Un {B_OPPONENT_MON1_NAME} sauvage apparaît !{PAUSE 127}");
static const u8 sText_TwoWildPkmnAppeared[] = _("Oh ! Un {B_OPPONENT_MON1_NAME} et un {B_OPPONENT_MON2_NAME} sauvages apparaissent !\p");
static const u8 sText_GhostAppearedCantId[] = _("Le SPECTRE apparaît !\pMince !\nImpossible d'identifier le SPECTRE !\p");
static const u8 sText_TheGhostAppeared[] = _("Le SPECTRE apparaît !\p");
static const u8 sText_Trainer1WantsToBattle[] = _("{B_TRAINER1_NAME_WITH_CLASS} veut se battre !\p");
static const u8 sText_LinkTrainerWantsToBattle[] = _("{B_LINK_OPPONENT1_NAME} veut se battre !");
static const u8 sText_TwoLinkTrainersWantToBattle[] = _("{B_LINK_OPPONENT1_NAME} et {B_LINK_OPPONENT2_NAME} veulent se battre !");
static const u8 sText_Trainer1SentOutPkmn[] = _("{B_TRAINER1_NAME_WITH_CLASS} envoie {B_OPPONENT_MON1_NAME} !");
static const u8 sText_Trainer1SentOutTwoPkmn[] = _("{B_TRAINER1_NAME_WITH_CLASS} envoie {B_OPPONENT_MON1_NAME} et {B_OPPONENT_MON2_NAME} !");
static const u8 sText_Trainer1SentOutPkmn2[] = _("{B_TRAINER1_NAME_WITH_CLASS} envoie {B_BUFF1} !");
static const u8 sText_LinkTrainerIntroSendOutPkmn[] = _("{B_LINK_OPPONENT1_NAME} envoie {B_LINK_OPPONENT_MON1_NAME} !");
static const u8 sText_LinkTrainerSentOutPkmn[] = _("{B_LINK_OPPONENT1_NAME} envoie {B_BUFF1} !");
static const u8 sText_LinkTrainer2SentOutPkmn2[] = _("{B_LINK_OPPONENT2_NAME} envoie {B_BUFF1} !");
static const u8 sText_LinkTrainerSentOutTwoPkmn[] = _("{B_LINK_OPPONENT1_NAME} envoie {B_OPPONENT_MON1_NAME} et {B_OPPONENT_MON2_NAME} !");
static const u8 sText_TwoLinkTrainersIntroSendOutPkmn[] = _("{B_LINK_OPPONENT1_NAME} envoie {B_LINK_OPPONENT_MON1_NAME} ! {B_LINK_OPPONENT2_NAME} envoie {B_LINK_OPPONENT_MON2_NAME} !");
static const u8 sText_LinkTrainerSentOutPkmn2[] = _("{B_LINK_OPPONENT1_NAME} envoie {B_LINK_OPPONENT_MON2_NAME} !");
static const u8 sText_LinkTrainerMultiSentOutPkmn[] = _("{B_LINK_SCR_TRAINER_NAME} envoie {B_BUFF1} !");
static const u8 sText_GoPkmn[] = _("Vas-y, {B_PLAYER_MON1_NAME} !");
static const u8 sText_GoTwoPkmn[] = _("Allez, {B_PLAYER_MON1_NAME} et {B_PLAYER_MON2_NAME} !");
static const u8 sText_GoPkmn2[] = _("Vas-y, {B_BUFF1} !");
static const u8 sText_DoItPkmn[] = _("À toi de jouer, {B_BUFF1} !");
static const u8 sText_GoForItPkmn[] = _("Fonce, {B_BUFF1} !");
static const u8 sText_BeCarefulPkmn[] = _("Fais gaffe, {B_PLAYER_MON1_NAME} !");
static const u8 sText_JustALittleMorePkmn[] = _("Encore un effort ! Tiens bon, {B_BUFF1} !"); //currently unused, will require code changes
static const u8 sText_YourFoesWeakGetEmPkmn[] = _("Il est presque à terre ! Achève-le, {B_BUFF1} !");
static const u8 sText_LinkPartnerSentOutPkmn1GoPkmn[] = _("{B_LINK_PARTNER_NAME} envoie {B_LINK_PLAYER_MON1_NAME} ! Vas-y, {B_LINK_PLAYER_MON2_NAME} !");
static const u8 sText_LinkPartnerSentOutPkmn2GoPkmn[] = _("{B_LINK_PARTNER_NAME} envoie {B_LINK_PLAYER_MON2_NAME} ! Vas-y, {B_LINK_PLAYER_MON1_NAME} !");
static const u8 sText_LinkPartnerSentOutPkmn1[] = _("{B_LINK_PARTNER_NAME} envoie {B_BUFF1} !");
static const u8 sText_LinkPartnerSentOutPkmn2[] = _("{B_LINK_PARTNER_NAME} envoie {B_BUFF1} !");
static const u8 sText_LinkPartnerWithdrewPkmn1[] = _("{B_LINK_PARTNER_NAME} rappelle {B_LINK_PLAYER_MON1_NAME} !");
static const u8 sText_LinkPartnerWithdrewPkmn2[] = _("{B_LINK_PARTNER_NAME} rappelle {B_LINK_PLAYER_MON2_NAME} !");
static const u8 sText_PkmnSwitchOut[] = _("{B_BUFF1}, on change ! Reviens !"); //currently unused, I believe its used for when you switch on a Pokémon in shift mode
static const u8 sText_PkmnThatsEnough[] = _("{B_BUFF1}, ça suffit ! Reviens !");
static const u8 sText_PkmnComeBack[] = _("{B_BUFF1}, reviens !");
static const u8 sText_PkmnOkComeBack[] = _("OK, {B_BUFF1} ! Reviens !");
static const u8 sText_PkmnGoodComeBack[] = _("Bien joué, {B_BUFF1} ! Reviens !");
static const u8 sText_Trainer1WithdrewPkmn[] = _("{B_TRAINER1_NAME_WITH_CLASS} rappelle {B_BUFF1} !");
static const u8 sText_Trainer2WithdrewPkmn[] = _("{B_TRAINER2_NAME_WITH_CLASS} rappelle {B_BUFF1} !");
static const u8 sText_LinkTrainer1WithdrewPkmn[] = _("{B_LINK_OPPONENT1_NAME} rappelle {B_BUFF1} !");
static const u8 sText_LinkTrainer2WithdrewPkmn[] = _("{B_LINK_OPPONENT2_NAME} rappelle {B_BUFF1} !");
static const u8 sText_WildPkmnPrefix[] = _(" sauvage");
static const u8 sText_FoePkmnPrefix[] = _(" ennemi");
static const u8 sText_WildPkmnPrefixLower[] = _(" sauvage");
static const u8 sText_FoePkmnPrefixLower[] = _(" ennemi");
static const u8 sText_EmptyString8[] = _("");
static const u8 sText_FoePkmnPrefix2[] = _("Ennemi");
static const u8 sText_AllyPkmnPrefix[] = _("Allié");
static const u8 sText_FoePkmnPrefix3[] = _("ennemi");
static const u8 sText_AllyPkmnPrefix2[] = _("allié");
static const u8 sText_FoePkmnPrefix4[] = _("ennemi");
static const u8 sText_AllyPkmnPrefix3[] = _("allié");
static const u8 sText_AttackerUsedX[] = _("{B_ATK_NAME_WITH_PREFIX} utilise {B_BUFF3} !");
static const u8 sText_ExclamationMark[] = _("!");
static const u8 sText_ExclamationMark2[] = _("!");
static const u8 sText_ExclamationMark3[] = _("!");
static const u8 sText_ExclamationMark4[] = _("!");
static const u8 sText_ExclamationMark5[] = _("!");
static const u8 sText_HP[] = _("HP");
static const u8 sText_Attack[] = _("Attaque");
static const u8 sText_Defense[] = _("Défense");
static const u8 sText_Speed[] = _("Vitesse");
static const u8 sText_SpAttack[] = _("Atq. Spé.");
static const u8 sText_SpDefense[] = _("Déf. Spé.");
static const u8 sText_Accuracy[] = _("Précision");
static const u8 sText_Evasiveness[] = _("Esquive");

const u8 *const gStatNamesTable[NUM_BATTLE_STATS] =
{
    [STAT_HP]      = sText_HP,
    [STAT_ATK]     = sText_Attack,
    [STAT_DEF]     = sText_Defense,
    [STAT_SPEED]   = sText_Speed,
    [STAT_SPATK]   = sText_SpAttack,
    [STAT_SPDEF]   = sText_SpDefense,
    [STAT_ACC]     = sText_Accuracy,
    [STAT_EVASION] = sText_Evasiveness,
};
const u8 *const gPokeblockWasTooXStringTable[FLAVOR_COUNT] =
{
    [FLAVOR_SPICY]  = COMPOUND_STRING("was too spicy!"),
    [FLAVOR_DRY]    = COMPOUND_STRING("was too dry!"),
    [FLAVOR_SWEET]  = COMPOUND_STRING("was too sweet!"),
    [FLAVOR_BITTER] = COMPOUND_STRING("was too bitter!"),
    [FLAVOR_SOUR]   = COMPOUND_STRING("was too sour!"),
};

static const u8 sText_Someones[] = _("de quelqu'un");
static const u8 sText_Lanettes[] = _("LANETTE's"); //no decapitalize until it is everywhere
static const u8 sText_Bills[] = _("BILL's");
static const u8 sText_EnigmaBerry[] = _("ENIGMA BERRY"); //no decapitalize until it is everywhere
static const u8 sText_BerrySuffix[] = _(" BERRY"); //no decapitalize until it is everywhere
const u8 gText_EmptyString3[] = _("");

static const u8 sText_TwoInGameTrainersDefeated[] = _("Tu as battu {B_TRAINER1_NAME_WITH_CLASS} et {B_TRAINER2_NAME_WITH_CLASS} !\p");

// New battle strings.
const u8 gText_drastically[] = _(" énormément");
const u8 gText_severely[] = _("énormément ");
static const u8 sText_TerrainReturnedToNormal[] = _("Le terrain redevient normal !"); // Unused

const u8 *const gBattleStringsTable[STRINGID_COUNT] =
{
    [STRINGID_TRAINER1LOSETEXT]                     = COMPOUND_STRING("{B_TRAINER1_LOSE_TEXT}"),
    [STRINGID_PKMNGAINEDEXP]                        = COMPOUND_STRING("{B_BUFF1} gagne{B_BUFF2} {B_BUFF3} points d'Exp. !\p"),
    [STRINGID_PKMNGREWTOLV]                         = COMPOUND_STRING("{B_BUFF1} monte au N. {B_BUFF2} !{WAIT_SE}\p"),
    [STRINGID_PKMNLEARNEDMOVE]                      = COMPOUND_STRING("{B_BUFF1} apprend {B_BUFF2} !{WAIT_SE}\p"),
    [STRINGID_TRYTOLEARNMOVE1]                      = COMPOUND_STRING("{B_BUFF1} veut apprendre {B_BUFF2}.\p"),
    [STRINGID_TRYTOLEARNMOVE2]                      = COMPOUND_STRING("Mais {B_BUFF1} connaît déjà quatre capacités.\p"),
    [STRINGID_TRYTOLEARNMOVE3]                      = COMPOUND_STRING("Oublier une capacité pour la remplacer par {B_BUFF2} ?"),
    [STRINGID_PKMNFORGOTMOVE]                       = COMPOUND_STRING("{B_BUFF1} oublie {B_BUFF2}…\p"),
    [STRINGID_STOPLEARNINGMOVE]                     = COMPOUND_STRING("{PAUSE 32}Renoncer à faire apprendre {B_BUFF2} à {B_BUFF1} ?"),
    [STRINGID_DIDNOTLEARNMOVE]                      = COMPOUND_STRING("{B_BUFF1} n'a pas appris {B_BUFF2}.\p"),
    [STRINGID_PKMNLEARNEDMOVE2]                     = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} apprend {B_BUFF1} !"),
    [STRINGID_PKMNPROTECTEDITSELF]                  = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX} se protège !"),
    [STRINGID_ITDOESNTAFFECT]                       = COMPOUND_STRING("Ça n'affecte pas {B_DEF_NAME_WITH_PREFIX2}…"),
    [STRINGID_ITDOESNTAFFECTSCR]                    = COMPOUND_STRING("Ça n'affecte pas {B_SCR_NAME_WITH_PREFIX2}…"),
    [STRINGID_BATTLERFAINTED]                       = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX} est K.O. !\p"),
    [STRINGID_PLAYERGOTMONEY]                       = COMPOUND_STRING("Tu remportes {B_BUFF1} ¥ !\p"),
    [STRINGID_PLAYERWHITEOUT]                       = COMPOUND_STRING("Tu n'as plus de Pokémon en état de se battre !\p"),
    [STRINGID_PLAYERWHITEOUT2_WILD]                 = COMPOUND_STRING("Dans la panique, tu perds {B_BUFF1} ¥…"),
    [STRINGID_PLAYERWHITEOUT2_TRAINER]              = COMPOUND_STRING("Tu donnes {B_BUFF1} ¥ au vainqueur…"),
    [STRINGID_PLAYERWHITEOUT3]                      = COMPOUND_STRING("La défaite t'écrase. Tout devient noir…"),
    [STRINGID_PREVENTSESCAPE]                       = COMPOUND_STRING("{B_SCR_ABILITY} de {B_SCR_NAME_WITH_PREFIX} empêche la fuite !\p"),
    [STRINGID_HITXTIMES]                            = COMPOUND_STRING("Touché {B_BUFF1} fois !"),
    [STRINGID_PKMNFELLASLEEP]                       = COMPOUND_STRING("{B_EFF_NAME_WITH_PREFIX} s'endort !"),
    [STRINGID_PKMNMADESLEEP]                        = COMPOUND_STRING("{B_BUFF1} de {B_SCR_NAME_WITH_PREFIX} endort {B_EFF_NAME_WITH_PREFIX2} !"), //not in gen 5+, ability popup
    [STRINGID_PKMNALREADYASLEEP]                    = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX} dort déjà !"),
    [STRINGID_PKMNALREADYASLEEP2]                   = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} dort déjà !"),
    [STRINGID_PKMNWASPOISONED]                      = COMPOUND_STRING("{B_EFF_NAME_WITH_PREFIX} est empoisonné !"),
    [STRINGID_PKMNPOISONEDBY]                       = COMPOUND_STRING("{B_EFF_NAME_WITH_PREFIX} est empoisonné par {B_BUFF1} de {B_SCR_NAME_WITH_PREFIX2} !"), //not in gen 5+, ability popup
    [STRINGID_PKMNHURTBYPOISON]                     = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} souffre du poison !"),
    [STRINGID_PKMNALREADYPOISONED]                  = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX} est déjà empoisonné !"),
    [STRINGID_PKMNBADLYPOISONED]                    = COMPOUND_STRING("{B_EFF_NAME_WITH_PREFIX} est gravement empoisonné !"),
    [STRINGID_PKMNENERGYDRAINED]                    = COMPOUND_STRING("L'énergie de {B_SCR_NAME_WITH_PREFIX} est aspirée !"),
    [STRINGID_PKMNWASBURNED]                        = COMPOUND_STRING("{B_EFF_NAME_WITH_PREFIX} est brûlé !"),
    [STRINGID_PKMNBURNEDBY]                         = COMPOUND_STRING("{B_BUFF1} de {B_SCR_NAME_WITH_PREFIX} brûle {B_EFF_NAME_WITH_PREFIX2} !"), //not in gen 5+, ability popup
    [STRINGID_PKMNHURTBYBURN]                       = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} souffre de sa brûlure !"),
    [STRINGID_PKMNWASFROZEN]                        = COMPOUND_STRING("{B_EFF_NAME_WITH_PREFIX} est gelé !"),
    [STRINGID_PKMNFROZENBY]                         = COMPOUND_STRING("{B_BUFF1} de {B_SCR_NAME_WITH_PREFIX} gèle {B_EFF_NAME_WITH_PREFIX2} !"), //not in gen 5+, ability popup
    [STRINGID_PKMNISFROZEN]                         = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} est gelé !"),
    [STRINGID_PKMNWASDEFROSTED]                     = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX} dégèle !"),
    [STRINGID_PKMNWASDEFROSTEDBY]                   = COMPOUND_STRING("{B_CURRENT_MOVE} de {B_SCR_NAME_WITH_PREFIX} fait fondre la glace !"),
    [STRINGID_PKMNWASPARALYZED]                     = COMPOUND_STRING("{B_EFF_NAME_WITH_PREFIX} est paralysé ! Il aura du mal à bouger !"),
    [STRINGID_PKMNWASPARALYZEDBY]                   = COMPOUND_STRING("{B_BUFF1} de {B_SCR_NAME_WITH_PREFIX} paralyse {B_EFF_NAME_WITH_PREFIX2} ! Il aura du mal à bouger !"), //not in gen 5+, ability popup
    [STRINGID_PKMNISPARALYZED]                      = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} est paralysé ! Il ne peut pas bouger !"),
    [STRINGID_PKMNISALREADYPARALYZED]               = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX} est déjà paralysé !"),
    [STRINGID_PKMNHEALEDPARALYSIS]                  = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX} n'est plus paralysé !"),
    [STRINGID_STATSWONTINCREASE]                    = COMPOUND_STRING("La stat {B_BUFF1} de {B_SCR_NAME_WITH_PREFIX} ne peut plus monter !"),
    [STRINGID_STATSWONTDECREASE]                    = COMPOUND_STRING("La stat {B_BUFF1} de {B_SCR_NAME_WITH_PREFIX} ne peut plus baisser !"),
    [STRINGID_PKMNISCONFUSED]                       = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} est confus !"),
    [STRINGID_PKMNHEALEDCONFUSION]                  = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} n'est plus confus !"),
    [STRINGID_PKMNWASCONFUSED]                      = COMPOUND_STRING("{B_EFF_NAME_WITH_PREFIX} devient confus !"),
    [STRINGID_PKMNALREADYCONFUSED]                  = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX} est déjà confus !"),
    [STRINGID_PKMNFELLINLOVE]                       = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX} tombe amoureux !"),
    [STRINGID_PKMNINLOVE]                           = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} est amoureux de {B_SCR_NAME_WITH_PREFIX2} !"),
    [STRINGID_PKMNIMMOBILIZEDBYLOVE]                = COMPOUND_STRING("L'amour paralyse {B_ATK_NAME_WITH_PREFIX2} !"),
    [STRINGID_PKMNCHANGEDTYPE]                      = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} devient de type {B_BUFF1} !"),
    [STRINGID_PKMNFLINCHED]                         = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} a la trouille ! Il ne peut pas attaquer !"),
    [STRINGID_PKMNREGAINEDHEALTH]                   = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX} récupère des PV."),
    [STRINGID_PKMNHPFULL]                           = COMPOUND_STRING("Les PV de {B_DEF_NAME_WITH_PREFIX} sont au max !"),
    [STRINGID_PKMNRAISEDSPDEF]                      = COMPOUND_STRING("Mur Lumière protège {B_ATK_TEAM2} contre les attaques spéciales !"),
    [STRINGID_PKMNRAISEDDEF]                        = COMPOUND_STRING("Protection protège {B_ATK_TEAM2} contre les attaques physiques !"),
    [STRINGID_PKMNAURORAVEIL]                       = COMPOUND_STRING("Voile Aurore protège {B_ATK_TEAM2} contre les attaques physiques et spéciales !"),
    [STRINGID_PKMNCOVEREDBYVEIL]                    = COMPOUND_STRING("{B_ATK_TEAM1} est enveloppée d'un voile mystique !"),
    [STRINGID_PKMNUSEDSAFEGUARD]                    = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX} est protégé par Rune Protect !"),
    [STRINGID_PKMNSAFEGUARDEXPIRED]                 = COMPOUND_STRING("{B_ATK_TEAM1} n'est plus protégée par le voile mystique !"),
    [STRINGID_PKMNWENTTOSLEEP]                      = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} s'endort !"), //not in gen 5+
    [STRINGID_PKMNSLEPTHEALTHY]                     = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} pique un somme et récupère ses PV !"),
    [STRINGID_PKMNWHIPPEDWHIRLWIND]                 = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} déclenche un tourbillon !"),
    [STRINGID_PKMNTOOKSUNLIGHT]                     = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} absorbe la lumière !"),
    [STRINGID_PKMNLOWEREDHEAD]                      = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} rentre la tête !"),
    [STRINGID_PKMNFLEWHIGH]                         = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} s'envole très haut !"),
    [STRINGID_PKMNDUGHOLE]                          = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} s'enfonce sous terre !"),
    [STRINGID_PKMNSQUEEZEDBYBIND]                   = COMPOUND_STRING("{B_EFF_NAME_WITH_PREFIX} est ligoté par {B_SCR_NAME_WITH_PREFIX2} !"),
    [STRINGID_PKMNTRAPPEDINVORTEX]                  = COMPOUND_STRING("{B_EFF_NAME_WITH_PREFIX} est piégé dans le tourbillon !"),
    [STRINGID_PKMNWRAPPEDBY]                        = COMPOUND_STRING("{B_EFF_NAME_WITH_PREFIX} est enserré par {B_SCR_NAME_WITH_PREFIX2} !"),
    [STRINGID_PKMNCLAMPED]                          = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX} broie {B_EFF_NAME_WITH_PREFIX2} entre ses mâchoires !"),
    [STRINGID_PKMNHURTBY]                           = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} souffre de {B_BUFF1} !"),
    [STRINGID_PKMNFREEDFROM]                        = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} se libère de {B_BUFF1} !"),
    [STRINGID_PKMNCRASHED]                          = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} rate son coup et s'écrase par terre !"),
    [STRINGID_PKMNSHROUDEDINMIST]                   = gText_PkmnShroudedInMist,
    [STRINGID_PKMNPROTECTEDBYMIST]                  = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX} est protégé par la brume !"),
    [STRINGID_PKMNGETTINGPUMPED]                    = gText_PkmnGettingPumped,
    [STRINGID_PKMNHITWITHRECOIL]                    = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} subit le contrecoup !"),
    [STRINGID_PKMNPROTECTEDITSELF2]                 = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} se protège !"),
    [STRINGID_PKMNBUFFETEDBYSANDSTORM]              = COMPOUND_STRING("La tempête de sable fouette {B_ATK_NAME_WITH_PREFIX2} !"),
    [STRINGID_PKMNPELTEDBYHAIL]                     = COMPOUND_STRING("La grêle frappe {B_ATK_NAME_WITH_PREFIX2} !"),
    [STRINGID_PKMNSEEDED]                           = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX} est infecté !"),
    [STRINGID_PKMNAVOIDEDATTACK]                    = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX} esquive l'attaque !"),
    [STRINGID_BATTLERAVOIDEDATTACK]                 = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX} esquive l'attaque !"),
    [STRINGID_PKMNSAPPEDBYLEECHSEED]                = COMPOUND_STRING("Vampigraine draine l'énergie de {B_SCR_NAME_WITH_PREFIX2} !"),
    [STRINGID_PKMNFASTASLEEP]                       = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} dort comme un loir."),
    [STRINGID_PKMNWOKEUP]                           = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} se réveille !"),
    [STRINGID_PKMNWOKEUPINUPROAR]                   = COMPOUND_STRING("Le Brouhaha réveille {B_EFF_NAME_WITH_PREFIX2} !"),
    [STRINGID_PKMNCAUSEDUPROAR]                     = COMPOUND_STRING("{B_EFF_NAME_WITH_PREFIX} fait du boucan !"),
    [STRINGID_PKMNMAKINGUPROAR]                     = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} fait un boucan d'enfer !"),
    [STRINGID_PKMNCALMEDDOWN]                       = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} se calme."),
    [STRINGID_PKMNSTOCKPILED]                       = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} stocke {B_BUFF1} !"),
    [STRINGID_PKMNCANTSLEEPINUPROAR2]               = COMPOUND_STRING("Mais {B_DEF_NAME_WITH_PREFIX2} ne peut pas dormir avec ce boucan !"),
    [STRINGID_UPROARKEPTPKMNAWAKE]                  = COMPOUND_STRING("Mais le boucan empêche {B_DEF_NAME_WITH_PREFIX2} de dormir !"),
    [STRINGID_PKMNSTAYEDAWAKEUSING]                 = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX} reste éveillé !"),
    [STRINGID_PKMNSTORINGENERGY]                    = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} accumule de l'énergie !"),
    [STRINGID_PKMNUNLEASHEDENERGY]                  = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} libère son énergie !"),
    [STRINGID_PKMNFATIGUECONFUSION]                 = COMPOUND_STRING("L'épuisement rend {B_SCR_NAME_WITH_PREFIX2} confus !"),
    [STRINGID_PLAYERPICKEDUPMONEY]                  = COMPOUND_STRING("Tu ramasses {B_BUFF1} ¥ !\p"),
    [STRINGID_PKMNUNAFFECTED]                       = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX} n'est pas affecté !"),
    [STRINGID_PKMNTRANSFORMEDINTO]                  = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} se transforme en {B_BUFF1} !"),
    [STRINGID_PKMNMADESUBSTITUTE]                   = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} crée un clone !"),
    [STRINGID_PKMNHASSUBSTITUTE]                    = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} a déjà un clone !"),
    [STRINGID_SUBSTITUTEDAMAGED]                    = COMPOUND_STRING("Le clone encaisse les dégâts à la place de {B_DEF_NAME_WITH_PREFIX2} !\p"),
    [STRINGID_PKMNSUBSTITUTEFADED]                  = COMPOUND_STRING("Le clone de {B_SCR_NAME_WITH_PREFIX2} disparaît !\p"),
    [STRINGID_PKMNMUSTRECHARGE]                     = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} doit se recharger !"),
    [STRINGID_PKMNRAGEBUILDING]                     = COMPOUND_STRING("La rage de {B_DEF_NAME_WITH_PREFIX2} monte !"),
    [STRINGID_PKMNMOVEWASDISABLED]                  = COMPOUND_STRING("{B_BUFF1} de {B_DEF_NAME_WITH_PREFIX2} est bloquée !"),
    [STRINGID_PKMNMOVEISDISABLED]                   = COMPOUND_STRING("{B_CURRENT_MOVE} de {B_ATK_NAME_WITH_PREFIX2} est bloquée !\p"),
    [STRINGID_PKMNMOVEDISABLEDNOMORE]               = COMPOUND_STRING("La capacité de {B_SCR_NAME_WITH_PREFIX2} n'est plus bloquée !"),
    [STRINGID_PKMNGOTENCORE]                        = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX} subit Encore !"),
    [STRINGID_PKMNGOTENCOREDMOVE]                   = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX} ne peut utiliser que {B_CURRENT_MOVE} !\p"),
    [STRINGID_PKMNENCOREENDED]                      = COMPOUND_STRING("L'effet Encore sur {B_SCR_NAME_WITH_PREFIX2} prend fin !"),
    [STRINGID_PKMNTOOKAIM]                          = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} vise {B_DEF_NAME_WITH_PREFIX2} !"),
    [STRINGID_PKMNSKETCHEDMOVE]                     = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} copie {B_BUFF1} !"),
    [STRINGID_PKMNTRYINGTOTAKEFOE]                  = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} veut entraîner son adversaire dans sa chute !"),
    [STRINGID_PKMNTOOKFOE]                          = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX} entraîne son adversaire dans la tombe !"),
    [STRINGID_PKMNREDUCEDPP]                        = COMPOUND_STRING("{B_BUFF1} de {B_DEF_NAME_WITH_PREFIX2} perd {B_BUFF2} PP !"),
    [STRINGID_PKMNSTOLEITEM]                        = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} vole l'objet {B_LAST_ITEM} de {B_BUFF2} !"),
    [STRINGID_TARGETCANTESCAPENOW]                  = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX} ne peut plus s'enfuir !"),
    [STRINGID_PKMNFELLINTONIGHTMARE]                = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX} commence à cauchemarder !"),
    [STRINGID_PKMNLOCKEDINNIGHTMARE]                = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} est prisonnier d'un cauchemar !"),
    [STRINGID_PKMNLAIDCURSE]                        = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} sacrifie des PV et maudit {B_DEF_NAME_WITH_PREFIX2} !"),
    [STRINGID_PKMNAFFLICTEDBYCURSE]                 = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} est rongé par la malédiction !"),
    [STRINGID_SPIKESSCATTERED]                      = COMPOUND_STRING("Des picots jonchent le sol autour de {B_DEF_TEAM2} !"),
    [STRINGID_PKMNHURTBYSPIKES]                     = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX} est blessé par les picots !"),
    [STRINGID_PKMNIDENTIFIED]                       = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX} est identifié !"),
    [STRINGID_PKMNPERISHCOUNTFELL]                  = COMPOUND_STRING("Le compte à rebours de {B_ATK_NAME_WITH_PREFIX2} tombe à {B_BUFF1} !"),
    [STRINGID_PKMNBRACEDITSELF]                     = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} se prépare au choc !"),
    [STRINGID_PKMNENDUREDHIT]                       = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX} encaisse le coup !"),
    [STRINGID_MAGNITUDESTRENGTH]                    = COMPOUND_STRING("Ampleur {B_BUFF1} !"),
    [STRINGID_PKMNCUTHPMAXEDATTACK]                 = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} sacrifie des PV et maximise son Attaque !"),
    [STRINGID_PKMNCOPIEDSTATCHANGES]                = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX} copie les changements de stats de {B_EFF_NAME_WITH_PREFIX2} !"),
    [STRINGID_PKMNGOTFREE]                          = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} se libère de {B_BUFF1} !"),
    [STRINGID_PKMNSHEDLEECHSEED]                    = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} se débarrasse de Vampigraine !"),
    [STRINGID_PKMNBLEWAWAYSPIKES]                   = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} balaie les Picots !"), // Not in gen 5+
    [STRINGID_PKMNFLEDFROMBATTLE]                   = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} prend la fuite !"),
    [STRINGID_PKMNFORESAWATTACK]                    = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} prévoit une attaque !"),
    [STRINGID_PKMNTOOKATTACK]                       = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX} subit l'attaque {B_BUFF1} !"),
    [STRINGID_PKMNATTACK]                           = COMPOUND_STRING("Attaque de {B_BUFF1} !"), // Not in gen 5+
    [STRINGID_PKMNCENTERATTENTION]                  = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX} attire toute l'attention !"),
    [STRINGID_PKMNCHARGINGPOWER]                    = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} se charge en énergie !"),
    [STRINGID_NATUREPOWERTURNEDINTO]                = COMPOUND_STRING("Force Nature de {B_ATK_NAME_WITH_PREFIX2} se change en {B_CURRENT_MOVE} !"),
    [STRINGID_PKMNSTATUSNORMAL]                     = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} retrouve un état normal !"),
    [STRINGID_PKMNHASNOMOVESLEFT]                   = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} n'a plus aucune capacité utilisable !\p"),
    [STRINGID_PKMNSUBJECTEDTOTORMENT]               = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX} est tourmenté !"),
    [STRINGID_PKMNCANTUSEMOVETORMENT]               = COMPOUND_STRING("Tourmenté, {B_ATK_NAME_WITH_PREFIX2} ne peut pas utiliser deux fois de suite la même capacité !\p"),
    [STRINGID_PKMNTIGHTENINGFOCUS]                  = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX} se concentre !"),
    [STRINGID_PKMNFELLFORTAUNT]                     = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX} tombe dans la provoc' !"),
    [STRINGID_PKMNCANTUSEMOVETAUNT]                 = COMPOUND_STRING("Provoqué, {B_ATK_NAME_WITH_PREFIX2} ne peut pas utiliser {B_CURRENT_MOVE} !\p"),
    [STRINGID_PKMNREADYTOHELP]                      = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} est prêt à aider {B_DEF_NAME_WITH_PREFIX2} !"),
    [STRINGID_PKMNSWITCHEDITEMS]                    = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} échange les objets avec sa cible !"),
    [STRINGID_PKMNCOPIEDFOE]                        = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} copie le talent de {B_DEF_NAME_WITH_PREFIX2} !"),
    [STRINGID_PKMNWISHCAMETRUE]                     = COMPOUND_STRING("Le vœu de {B_BUFF1} se réalise !"),
    [STRINGID_PKMNPLANTEDROOTS]                     = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} plante ses racines !"),
    [STRINGID_PKMNABSORBEDNUTRIENTS]                = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} puise des nutriments par ses racines !"),
    [STRINGID_PKMNANCHOREDITSELF]                   = COMPOUND_STRING("{B_EFF_NAME_WITH_PREFIX} est ancré au sol par ses racines !"),
    [STRINGID_PKMNWASMADEDROWSY]                    = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX} s'assoupit !"),
    [STRINGID_PKMNKNOCKEDOFF]                       = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} fait tomber l'objet {B_LAST_ITEM} de {B_EFF_NAME_WITH_PREFIX2} !"),
    [STRINGID_PKMNSWAPPEDABILITIES]                 = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} échange son talent avec sa cible !"),
    [STRINGID_PKMNSEALEDOPPONENTMOVE]               = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} scelle les capacités qu'il partage avec sa cible !"),
    [STRINGID_PKMNCANTUSEMOVESEALED]                = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} ne peut pas utiliser {B_CURRENT_MOVE}, elle est scellée !\p"),
    [STRINGID_PKMNWANTSGRUDGE]                      = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} veut que sa cible lui garde rancune !"),
    [STRINGID_PKMNLOSTPPGRUDGE]                     = COMPOUND_STRING("{B_BUFF1} de {B_ATK_NAME_WITH_PREFIX2} perd tous ses PP à cause de la rancune !"),
    [STRINGID_PKMNSHROUDEDITSELF]                   = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} s'enveloppe de Reflet Magik !"),
    [STRINGID_PKMNMOVEBOUNCED]                      = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} renvoie {B_CURRENT_MOVE} !"),
    [STRINGID_PKMNWAITSFORTARGET]                   = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} attend que sa cible agisse !"),
    [STRINGID_PKMNSNATCHEDMOVE]                     = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} vole la capacité de {B_SCR_NAME_WITH_PREFIX2} !"),
    [STRINGID_PKMNMADEITRAIN]                       = COMPOUND_STRING("{B_SCR_ABILITY} de {B_SCR_NAME_WITH_PREFIX2} fait tomber la pluie !"), //not in gen 5+, ability popup
    [STRINGID_PKMNPROTECTEDBY]                      = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX} est protégé par {B_DEF_ABILITY} !"), //not in gen 5+, ability popup
    [STRINGID_PKMNPREVENTSUSAGE]                    = COMPOUND_STRING("{B_DEF_ABILITY} de {B_DEF_NAME_WITH_PREFIX2} empêche {B_ATK_NAME_WITH_PREFIX2} d'utiliser {B_CURRENT_MOVE} !"), //not in gen 5+, ability popup
    [STRINGID_PKMNRESTOREDHPUSING]                  = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX} récupère des PV."),
    [STRINGID_PKMNCHANGEDTYPEWITH]                  = COMPOUND_STRING("{B_EFF_NAME_WITH_PREFIX} devient de type {B_BUFF1} !"),
    [STRINGID_PKMNPREVENTSROMANCEWITH]              = COMPOUND_STRING("{B_DEF_ABILITY} de {B_DEF_NAME_WITH_PREFIX2} empêche toute romance !"), //not in gen 5+, ability popup
    [STRINGID_PKMNPREVENTSCONFUSIONWITH]            = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX} ne peut pas être confus !"),
    [STRINGID_PKMNRAISEDFIREPOWERWITH]              = COMPOUND_STRING("La puissance des capacités Feu de {B_SCR_NAME_WITH_PREFIX2} augmente !"),
    [STRINGID_PKMNANCHORSITSELFWITH]                = COMPOUND_STRING("{B_EFF_NAME_WITH_PREFIX} s'agrippe avec ses ventouses !"),
    [STRINGID_PKMNPREVENTSSTATLOSSWITH]             = COMPOUND_STRING("Les stats de {B_SCR_NAME_WITH_PREFIX2} ne baissent pas !"),
    [STRINGID_PKMNHURTSWITH]                        = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} est blessé par {B_BUFF1} de {B_DEF_NAME_WITH_PREFIX2} !"),
    [STRINGID_PKMNTRACED]                           = COMPOUND_STRING("Il copie {B_BUFF2} de {B_BUFF1} !"),
    [STRINGID_STATSHARPLY]                          = gText_StatSharply,
    [STRINGID_STATHARSHLY]                          = COMPOUND_STRING("beaucoup"),
    [STRINGID_STATROSE]                             = COMPOUND_STRING("La stat {B_BUFF1} de {B_SCR_NAME_WITH_PREFIX2} augmente{B_BUFF2} !"),
    [STRINGID_STATFELL]                             = COMPOUND_STRING("La stat {B_BUFF1} de {B_SCR_NAME_WITH_PREFIX2} baisse {B_BUFF2}!"),
    [STRINGID_CRITICALHIT]                          = COMPOUND_STRING("Coup critique !"),
    [STRINGID_ONEHITKO]                             = COMPOUND_STRING("K.O. en un coup !"),
    [STRINGID_123POOF]                              = COMPOUND_STRING("Un…{PAUSE 10}deux…{PAUSE 10}et…{PAUSE 10}{PAUSE 20}{PLAY_SE SE_BALL_BOUNCE_1}tadaaa !\p"),
    [STRINGID_ANDELLIPSIS]                          = COMPOUND_STRING("Et…\p"),
    [STRINGID_NOTVERYEFFECTIVE]                     = COMPOUND_STRING("Ce n'est pas très efficace…"),
    [STRINGID_SUPEREFFECTIVE]                       = COMPOUND_STRING("C'est super efficace !"),
    [STRINGID_GOTAWAYSAFELY]                        = sText_GotAwaySafely,
    [STRINGID_WILDPKMNFLED]                         = COMPOUND_STRING("{PLAY_SE SE_FLEE}{B_BUFF1} sauvage s'enfuit !"),
    [STRINGID_NORUNNINGFROMTRAINERS]                = COMPOUND_STRING("Non ! On ne fuit pas un combat de Dresseurs !\p"),
    [STRINGID_CANTESCAPE]                           = COMPOUND_STRING("Impossible de fuir !\p"),
    [STRINGID_DONTLEAVEBIRCH]                       = COMPOUND_STRING("Prof. Seko : Ne me laisse pas crever ici !\p"), //no decapitalize until it is everywhere
    [STRINGID_BUTNOTHINGHAPPENED]                   = COMPOUND_STRING("Mais rien ne se passe !"),
    [STRINGID_BUTITFAILED]                          = COMPOUND_STRING("Mais cela échoue !"),
    [STRINGID_ITHURTCONFUSION]                      = COMPOUND_STRING("Il se blesse dans sa confusion !"),
    [STRINGID_STARTEDTORAIN]                        = COMPOUND_STRING("Il commence à pleuvoir !"),
    [STRINGID_DOWNPOURSTARTED]                      = COMPOUND_STRING("Une pluie battante s'abat !"), // corresponds to DownpourText in pokegold and pokecrystal and is used by Rain Dance in GSC
    [STRINGID_RAINCONTINUES]                        = COMPOUND_STRING("La pluie continue de tomber."), //not in gen 5+
    [STRINGID_DOWNPOURCONTINUES]                    = COMPOUND_STRING("La pluie battante continue."), // unused
    [STRINGID_RAINSTOPPED]                          = COMPOUND_STRING("La pluie s'arrête."),
    [STRINGID_SANDSTORMBREWED]                      = COMPOUND_STRING("Une tempête de sable se lève !"),
    [STRINGID_SANDSTORMRAGES]                       = COMPOUND_STRING("La tempête de sable fait rage."),
    [STRINGID_SANDSTORMSUBSIDED]                    = COMPOUND_STRING("La tempête de sable se calme."),
    [STRINGID_SUNLIGHTGOTBRIGHT]                    = COMPOUND_STRING("Le soleil tape fort !"),
    [STRINGID_SUNLIGHTSTRONG]                       = COMPOUND_STRING("Le soleil brille intensément."), //not in gen 5+
    [STRINGID_SUNLIGHTFADED]                        = COMPOUND_STRING("Le soleil s'affaiblit."),
    [STRINGID_STARTEDHAIL]                          = COMPOUND_STRING("Il commence à grêler !"),
    [STRINGID_HAILCONTINUES]                        = COMPOUND_STRING("La grêle continue de tomber."),
    [STRINGID_HAILSTOPPED]                          = COMPOUND_STRING("La grêle s'arrête."),
    [STRINGID_STATCHANGESGONE]                      = COMPOUND_STRING("Tous les changements de stats sont annulés !"),
    [STRINGID_COINSSCATTERED]                       = COMPOUND_STRING("Des pièces s'éparpillent partout !"),
    [STRINGID_TOOWEAKFORSUBSTITUTE]                 = COMPOUND_STRING("Mais il n'a pas assez de PV pour créer un clone !"),
    [STRINGID_SHAREDPAIN]                           = COMPOUND_STRING("Les Pokémon partagent leur douleur !"),
    [STRINGID_BELLCHIMED]                           = COMPOUND_STRING("Une cloche sonne !"),
    [STRINGID_FAINTINTHREE]                         = COMPOUND_STRING("Les Pokémon qui ont entendu le chant seront K.O. dans trois tours !"),
    [STRINGID_NOPPLEFT]                             = COMPOUND_STRING("Plus de PP pour cette capacité !\p"), //not in gen 5+
    [STRINGID_BUTNOPPLEFT]                          = COMPOUND_STRING("Mais cette capacité n'a plus de PP !"),
    [STRINGID_PLAYERUSEDITEM]                       = COMPOUND_STRING("Tu utilises {B_LAST_ITEM} !"),
    [STRINGID_TRAINERBLOCKEDBALL]                   = COMPOUND_STRING("Le Dresseur dévie ta Poké Ball !"),
    [STRINGID_DONTBEATHIEF]                         = COMPOUND_STRING("Pas touche, voleur !"),
    [STRINGID_ITDODGEDBALL]                         = COMPOUND_STRING("Il esquive ta Poké Ball ! Ce Pokémon ne peut pas être capturé !"),
    [STRINGID_PKMNBROKEFREE]                        = COMPOUND_STRING("Oh non ! Le Pokémon s'est libéré !"),
    [STRINGID_ITAPPEAREDCAUGHT]                     = COMPOUND_STRING("Raaah ! Presque !"),
    [STRINGID_AARGHALMOSTHADIT]                     = COMPOUND_STRING("Mince ! C'était tout près !"),
    [STRINGID_SHOOTSOCLOSE]                         = COMPOUND_STRING("Argh ! À un cheveu !"),
    [STRINGID_GOTCHAPKMNCAUGHTPLAYER]               = COMPOUND_STRING("Et hop ! {B_DEF_NAME} est capturé !{WAIT_SE}{PLAY_BGM MUS_CAUGHT}\p"),
    [STRINGID_GOTCHAPKMNCAUGHTWALLY]                = COMPOUND_STRING("Et hop ! {B_DEF_NAME} est capturé !{WAIT_SE}{PLAY_BGM MUS_CAUGHT}{PAUSE 127}"),
    [STRINGID_GIVENICKNAMECAPTURED]                 = COMPOUND_STRING("Donner un surnom à {B_DEF_NAME} ?"),
    [STRINGID_PKMNDATAADDEDTODEX]                   = COMPOUND_STRING("Les données de {B_DEF_NAME} sont ajoutées au Pokédex !\p"),
    [STRINGID_ITISRAINING]                          = COMPOUND_STRING("Il pleut !"),
    [STRINGID_SANDSTORMISRAGING]                    = COMPOUND_STRING("Une tempête de sable fait rage !"),
    [STRINGID_CANTESCAPE2]                          = COMPOUND_STRING("Tu n'as pas réussi à fuir !\p"),
    [STRINGID_PKMNIGNORESASLEEP]                    = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} ignore tes ordres et continue de dormir !"),
    [STRINGID_PKMNIGNOREDORDERS]                    = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} ignore tes ordres !"),
    [STRINGID_PKMNBEGANTONAP]                       = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} fait la sieste !"),
    [STRINGID_PKMNLOAFING]                          = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} se tourne les pouces !"),
    [STRINGID_PKMNWONTOBEY]                         = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} refuse d'obéir !"),
    [STRINGID_PKMNTURNEDAWAY]                       = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} tourne le dos !"),
    [STRINGID_PKMNPRETENDNOTNOTICE]                 = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} fait semblant de ne rien voir !"),
    [STRINGID_ENEMYABOUTTOSWITCHPKMN]               = COMPOUND_STRING("{B_TRAINER1_NAME_WITH_CLASS} va envoyer {B_BUFF2}. Changer de Pokémon ?"),
    [STRINGID_CREPTCLOSER]                          = COMPOUND_STRING("{B_PLAYER_NAME} s'approche de {B_OPPONENT_MON1_NAME} !"), //safari
    [STRINGID_CANTGETCLOSER]                        = COMPOUND_STRING("{B_PLAYER_NAME} ne peut pas s'approcher plus !"), //safari
    [STRINGID_PKMNWATCHINGCAREFULLY]                = COMPOUND_STRING("{B_OPPONENT_MON1_NAME} observe attentivement !"), //safari
    [STRINGID_PKMNCURIOUSABOUTX]                    = COMPOUND_STRING("{B_OPPONENT_MON1_NAME} est intrigué par {B_BUFF1} !"), //safari
    [STRINGID_PKMNENTHRALLEDBYX]                    = COMPOUND_STRING("{B_OPPONENT_MON1_NAME} est fasciné par {B_BUFF1} !"), //safari
    [STRINGID_PKMNIGNOREDX]                         = COMPOUND_STRING("{B_OPPONENT_MON1_NAME} ignore complètement {B_BUFF1} !"), //safari
    [STRINGID_THREWPOKEBLOCKATPKMN]                 = COMPOUND_STRING("{B_PLAYER_NAME} lance un {POKEBLOCK} à {B_OPPONENT_MON1_NAME} !"), //safari
    [STRINGID_OUTOFSAFARIBALLS]                     = COMPOUND_STRING("{PLAY_SE SE_DING_DONG}Speaker : Plus de Safari Balls ! C'est fini !\p"), //safari
    [STRINGID_PKMNSITEMCUREDPARALYSIS]              = COMPOUND_STRING("{B_LAST_ITEM} de {B_SCR_NAME_WITH_PREFIX2} soigne sa paralysie !"),
    [STRINGID_PKMNSITEMCUREDPOISON]                 = COMPOUND_STRING("{B_LAST_ITEM} de {B_SCR_NAME_WITH_PREFIX2} soigne son empoisonnement !"),
    [STRINGID_PKMNSITEMHEALEDBURN]                  = COMPOUND_STRING("{B_LAST_ITEM} de {B_SCR_NAME_WITH_PREFIX2} soigne sa brûlure !"),
    [STRINGID_PKMNSITEMDEFROSTEDIT]                 = COMPOUND_STRING("{B_LAST_ITEM} de {B_SCR_NAME_WITH_PREFIX2} le dégèle !"),
    [STRINGID_PKMNSITEMWOKEIT]                      = COMPOUND_STRING("{B_LAST_ITEM} de {B_SCR_NAME_WITH_PREFIX2} le réveille !"),
    [STRINGID_PKMNSITEMSNAPPEDOUT]                  = COMPOUND_STRING("{B_LAST_ITEM} de {B_SCR_NAME_WITH_PREFIX2} dissipe sa confusion !"),
    [STRINGID_PKMNSITEMCUREDPROBLEM]                = COMPOUND_STRING("{B_LAST_ITEM} de {B_SCR_NAME_WITH_PREFIX2} soigne son problème de {B_BUFF1} !"), // Not in Gen 5+
    [STRINGID_PKMNSITEMRESTOREDHEALTH]              = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX} récupère des PV grâce à {B_LAST_ITEM} !"),
    [STRINGID_PKMNSITEMRESTOREDPP]                  = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX} restaure les PP de {B_BUFF1} grâce à {B_LAST_ITEM} !"),
    [STRINGID_PKMNSITEMRESTOREDSTATUS]              = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX} rétablit ses stats grâce à {B_LAST_ITEM} !"),
    [STRINGID_PKMNSITEMRESTOREDHPALITTLE]           = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX} récupère un peu de PV grâce à {B_LAST_ITEM} !"),
    [STRINGID_ITEMALLOWSONLYYMOVE]                  = COMPOUND_STRING("{B_LAST_ITEM} ne permet d'utiliser que {B_CURRENT_MOVE} !\p"),
    [STRINGID_PKMNHUNGONWITHX]                      = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX} tient bon grâce à {B_LAST_ITEM} !"),
    [STRINGID_EMPTYSTRING3]                         = gText_EmptyString3,
    [STRINGID_PKMNSXRESTOREDHPALITTLE2]             = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} récupère des PV."),
    [STRINGID_PKMNSXWHIPPEDUPSANDSTORM]             = COMPOUND_STRING("{B_SCR_ABILITY} de {B_SCR_NAME_WITH_PREFIX2} déclenche une tempête de sable !"), //not in gen 5+, ability popup
    [STRINGID_PKMNSXPREVENTSYLOSS]                  = COMPOUND_STRING("La stat {B_BUFF1} de {B_SCR_NAME_WITH_PREFIX2} ne baisse pas !"),
    [STRINGID_PKMNSXINFATUATEDY]                    = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} tombe amoureux !"),
    [STRINGID_PKMNSXMADEYINEFFECTIVE]               = COMPOUND_STRING("L'objet de {B_SCR_NAME_WITH_PREFIX2} ne peut pas être retiré !"),
    [STRINGID_ITSUCKEDLIQUIDOOZE]                   = COMPOUND_STRING("{B_EFF_NAME_WITH_PREFIX} aspire le Suintement ! Beurk !"),
    [STRINGID_PKMNTRANSFORMED]                      = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX} se transforme !"),
    [STRINGID_ELECTRICITYWEAKENED]                  = COMPOUND_STRING("La puissance des capacités Électrik diminue !"),
    [STRINGID_FIREWEAKENED]                         = COMPOUND_STRING("La puissance des capacités Feu diminue !"),
    [STRINGID_PKMNHIDUNDERWATER]                    = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} se cache sous l'eau !"),
    [STRINGID_PKMNSPRANGUP]                         = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} bondit !"),
    [STRINGID_HMMOVESCANTBEFORGOTTEN]               = COMPOUND_STRING("Impossible d'oublier une capacité de CS maintenant.\p"),
    [STRINGID_XFOUNDONEY]                           = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} trouve {B_LAST_ITEM} !"),
    [STRINGID_PLAYERDEFEATEDTRAINER1]               = sText_PlayerDefeatedLinkTrainerTrainer1,
    [STRINGID_SOOTHINGAROMA]                        = COMPOUND_STRING("Un parfum apaisant flotte dans l'air !"),
    [STRINGID_ITEMSCANTBEUSEDNOW]                   = COMPOUND_STRING("Impossible d'utiliser des objets maintenant.{PAUSE 64}"), // Not present in Gen 5+
    [STRINGID_USINGITEMSTATOFPKMNROSE]              = COMPOUND_STRING("{B_LAST_ITEM}{B_BUFF2} augmente la stat {B_BUFF1} de {B_SCR_NAME_WITH_PREFIX2} !"),
    [STRINGID_USINGITEMSTATOFPKMNFELL]              = COMPOUND_STRING("{B_LAST_ITEM}{B_BUFF2} baisse la stat {B_BUFF1} de {B_SCR_NAME_WITH_PREFIX2} !"), // This string does not exist in Gen 5+. Used to print more info that's otherwise obscured such as using Room Service
    [STRINGID_PKMNUSEDXTOGETPUMPED]                 = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX} utilise {B_LAST_ITEM} et se motive à bloc !"),
    [STRINGID_PKMNSXMADEYUSELESS]                   = COMPOUND_STRING("{B_SCR_ABILITY} de {B_SCR_NAME_WITH_PREFIX2} rend {B_CURRENT_MOVE} inutile !"), //not in gen 5+, ability popup
    [STRINGID_PKMNTRAPPEDBYSANDTOMB]                = COMPOUND_STRING("{B_EFF_NAME_WITH_PREFIX} est piégé dans les sables mouvants !"),
    [STRINGID_EMPTYSTRING4]                         = COMPOUND_STRING(""),
    [STRINGID_ABOOSTED]                             = COMPOUND_STRING(" (boosté)"),
    [STRINGID_PKMNSXINTENSIFIEDSUN]                 = COMPOUND_STRING("{B_SCR_ABILITY} de {B_SCR_NAME_WITH_PREFIX2} intensifie les rayons du soleil !"), //not in gen 5+, ability popup
    [STRINGID_YOUTHROWABALLNOWRIGHT]                = COMPOUND_STRING("Tu lances une Ball, c'est ça ? Je… je vais faire de mon mieux !"),
    [STRINGID_PKMNSXTOOKATTACK]                     = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX} encaisse l'attaque !"),
    [STRINGID_PKMNCHOSEXASDESTINY]                  = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} confie son destin à Carnareket !"),
    [STRINGID_PKMNLOSTFOCUS]                        = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} perd sa concentration et ne peut pas attaquer !"),
    [STRINGID_USENEXTPKMN]                          = COMPOUND_STRING("Envoyer un autre Pokémon ?"),
    [STRINGID_PKMNFLEDUSINGITS]                     = COMPOUND_STRING("{PLAY_SE SE_FLEE}{B_ATK_NAME_WITH_PREFIX} s'enfuit grâce à {B_LAST_ITEM} !\p"),
    [STRINGID_PKMNFLEDUSING]                        = COMPOUND_STRING("{PLAY_SE SE_FLEE}{B_ATK_NAME_WITH_PREFIX} s'enfuit grâce à {B_ATK_ABILITY} !\p"), //not in gen 5+
    [STRINGID_PKMNWASDRAGGEDOUT]                    = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX} est traîné au combat !\p"),
    [STRINGID_PKMNSITEMNORMALIZEDSTATUS]            = COMPOUND_STRING("{B_LAST_ITEM} de {B_SCR_NAME_WITH_PREFIX2} soigne son statut !"), // Not in Gen 5+
    [STRINGID_TRAINER1USEDITEM]                     = COMPOUND_STRING("{B_ATK_TRAINER_NAME_WITH_CLASS} utilise {B_LAST_ITEM} !"),
    [STRINGID_BOXISFULL]                            = COMPOUND_STRING("La Boîte est pleine ! Tu ne peux plus capturer de Pokémon !\p"),
    [STRINGID_PKMNSXMADEITINEFFECTIVE]              = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX} rend l'attaque inefficace !"),
    [STRINGID_PKMNSXPREVENTSFLINCHING]              = COMPOUND_STRING("{B_EFF_ABILITY} de {B_EFF_NAME_WITH_PREFIX2} l'empêche d'avoir peur !"), //not in gen 5+, ability popup
    [STRINGID_PKMNALREADYHASBURN]                   = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX} est déjà brûlé !"),
    [STRINGID_PKMNSXBLOCKSY]                        = COMPOUND_STRING("{B_SCR_ABILITY} de {B_SCR_NAME_WITH_PREFIX2} bloque {B_CURRENT_MOVE} !"), //not in gen 5+, ability popup
    [STRINGID_PKMNSXWOREOFF]                        = COMPOUND_STRING("{B_BUFF1} de {B_ATK_TEAM2} prend fin !"),
    [STRINGID_THEWALLSHATTERED]                     = COMPOUND_STRING("La barrière vole en éclats !"), //not in gen5+, uses "your teams light screen wore off!" etc instead
    [STRINGID_PKMNSXCUREDITSYPROBLEM]               = COMPOUND_STRING("{B_SCR_ABILITY} de {B_SCR_NAME_WITH_PREFIX2} soigne son problème de {B_BUFF1} !"), //not in gen 5+, ability popup
    [STRINGID_ATTACKERCANTESCAPE]                   = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} ne peut pas s'enfuir !"),
    [STRINGID_PKMNOBTAINEDX]                        = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} obtient {B_BUFF1}."),
    [STRINGID_PKMNOBTAINEDX2]                       = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX} obtient {B_BUFF2}."),
    [STRINGID_PKMNOBTAINEDXYOBTAINEDZ]              = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} obtient {B_BUFF1}.\p{B_DEF_NAME_WITH_PREFIX} obtient {B_BUFF2}."),
    [STRINGID_BUTNOEFFECT]                          = COMPOUND_STRING("Mais ça n'a aucun effet !"),
    [STRINGID_TWOENEMIESDEFEATED]                   = sText_TwoInGameTrainersDefeated,
    [STRINGID_TRAINER2LOSETEXT]                     = COMPOUND_STRING("{B_TRAINER2_LOSE_TEXT}"),
    [STRINGID_PKMNINCAPABLEOFPOWER]                 = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} semble incapable d'utiliser sa puissance !"),
    [STRINGID_GLINTAPPEARSINEYE]                    = COMPOUND_STRING("Une lueur traverse les yeux de {B_SCR_NAME_WITH_PREFIX2} !"),
    [STRINGID_PKMNGETTINGINTOPOSITION]              = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX} se met en position !"),
    [STRINGID_PKMNBEGANGROWLINGDEEPLY]              = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX} se met à grogner sourdement !"),
    [STRINGID_PKMNEAGERFORMORE]                     = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX} en redemande !"),
    [STRINGID_DEFEATEDOPPONENTBYREFEREE]            = COMPOUND_STRING("{B_PLAYER_MON1_NAME} bat {B_OPPONENT_MON1_NAME} sur décision de l'arbitre !"),
    [STRINGID_LOSTTOOPPONENTBYREFEREE]              = COMPOUND_STRING("{B_PLAYER_MON1_NAME} perd contre {B_OPPONENT_MON1_NAME} sur décision de l'arbitre !"),
    [STRINGID_TIEDOPPONENTBYREFEREE]                = COMPOUND_STRING("{B_PLAYER_MON1_NAME} et {B_OPPONENT_MON1_NAME} font match nul sur décision de l'arbitre !"),
    [STRINGID_QUESTIONFORFEITMATCH]                 = COMPOUND_STRING("Abandonner le match maintenant ?"),
    [STRINGID_FORFEITEDMATCH]                       = COMPOUND_STRING("Le match est abandonné."),
    [STRINGID_PKMNTRANSFERREDSOMEONESPC]            = gText_PkmnTransferredSomeonesPC,
    [STRINGID_PKMNTRANSFERREDLANETTESPC]            = gText_PkmnTransferredLanettesPC,
    [STRINGID_PKMNBOXSOMEONESPCFULL]                = gText_PkmnTransferredSomeonesPCBoxFull,
    [STRINGID_PKMNBOXLANETTESPCFULL]                = gText_PkmnTransferredLanettesPCBoxFull,
    [STRINGID_TRAINER1WINTEXT]                      = COMPOUND_STRING("{B_TRAINER1_WIN_TEXT}"),
    [STRINGID_TRAINER2WINTEXT]                      = COMPOUND_STRING("{B_TRAINER2_WIN_TEXT}"),
    [STRINGID_ENDUREDSTURDY]                        = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX} encaisse le coup grâce à {B_DEF_ABILITY} !"),
    [STRINGID_POWERHERB]                            = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} se charge instantanément grâce à {B_LAST_ITEM} !"),
    [STRINGID_HURTBYITEM]                           = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} est blessé par {B_LAST_ITEM} !"),
    [STRINGID_GRAVITYINTENSIFIED]                   = COMPOUND_STRING("La gravité s'intensifie !"),
    [STRINGID_TARGETWOKEUP]                         = COMPOUND_STRING("{B_EFF_NAME_WITH_PREFIX} se réveille !"),
    [STRINGID_TAILWINDBLEW]                         = COMPOUND_STRING("Un vent arrière souffle derrière {B_ATK_TEAM2} !"),
    [STRINGID_PKMNWENTBACK]                         = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} revient vers {B_ATK_TRAINER_NAME} !"),
    [STRINGID_PKMNCANTUSEITEMSANYMORE]              = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX} ne peut plus utiliser d'objets !"),
    [STRINGID_PKMNFLUNG]                            = COMPOUND_STRING("{B_EFF_NAME_WITH_PREFIX} lance {B_LAST_ITEM} !"),
    [STRINGID_PKMNPREVENTEDFROMHEALING]             = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX} ne peut plus se soigner !"),
    [STRINGID_PKMNSWITCHEDATKANDDEF]                = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} échange son Attaque et sa Défense !"),
    [STRINGID_PKMNSABILITYSUPPRESSED]               = COMPOUND_STRING("Le talent de {B_DEF_NAME_WITH_PREFIX2} est neutralisé !"),
    [STRINGID_SHIELDEDFROMCRITICALHITS]             = COMPOUND_STRING("Air Veinard protège {B_ATK_TEAM2} des coups critiques !"), // Currently not present in Champions
    [STRINGID_PKMNACQUIREDABILITY]                  = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX} acquiert {B_DEF_ABILITY} !"),
    [STRINGID_POISONSPIKESSCATTERED]                = COMPOUND_STRING("Des pics toxiques jonchent le sol autour de {B_DEF_TEAM2} !"),
    [STRINGID_PKMNSWITCHEDSTATCHANGES]              = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} échange ses changements de stats avec sa cible !"),
    [STRINGID_PKMNSURROUNDEDWITHVEILOFWATER]        = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} s'entoure d'un voile d'eau !"),
    [STRINGID_PKMNLEVITATEDONELECTROMAGNETISM]      = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} lévite grâce à l'électromagnétisme !"),
    [STRINGID_PKMNTWISTEDDIMENSIONS]                = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} distord les dimensions !"),
    [STRINGID_POINTEDSTONESFLOAT]                   = COMPOUND_STRING("Des pierres pointues flottent autour de {B_DEF_TEAM2} !"),
    [STRINGID_TRAPPEDBYSWIRLINGMAGMA]               = COMPOUND_STRING("{B_EFF_NAME_WITH_PREFIX} est piégé dans un tourbillon de magma !"),
    [STRINGID_VANISHEDINSTANTLY]                    = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} disparaît en un éclair !"),
    [STRINGID_PROTECTEDTEAM]                        = COMPOUND_STRING("{B_CURRENT_MOVE} protège {B_ATK_TEAM2} !"),
    [STRINGID_SHAREDITSGUARD]                       = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} partage sa garde avec sa cible !"),
    [STRINGID_SHAREDITSPOWER]                       = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} partage sa force avec sa cible !"),
    [STRINGID_SWAPSDEFANDSPDEFOFALLPOKEMON]         = COMPOUND_STRING("Une zone étrange inverse la Défense et la Défense Spéciale !"),
    [STRINGID_BECAMENIMBLE]                         = COMPOUND_STRING("{B_EFF_NAME_WITH_PREFIX} devient plus agile !"),
    [STRINGID_HURLEDINTOTHEAIR]                     = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX} est projeté dans les airs !"),
    [STRINGID_HELDITEMSLOSEEFFECTS]                 = COMPOUND_STRING("Une zone étrange annule l'effet des objets tenus !"),
    [STRINGID_FELLSTRAIGHTDOWN]                     = COMPOUND_STRING("{B_EFF_NAME_WITH_PREFIX} tombe comme une pierre !"),
    [STRINGID_TARGETCHANGEDTYPE]                    = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX} devient de type {B_BUFF1} !"),
    [STRINGID_KINDOFFER]                            = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX} accepte gentiment l'offre !"),
    [STRINGID_RESETSTARGETSSTATLEVELS]              = COMPOUND_STRING("Les changements de stats de {B_EFF_NAME_WITH_PREFIX2} sont annulés !"),
    [STRINGID_ALLYSWITCHPOSITION]                   = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} et {B_SCR_NAME_WITH_PREFIX2} échangent leurs places !"),
    [STRINGID_REFLECTTARGETSTYPE]                   = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} prend le type de {B_DEF_NAME_WITH_PREFIX2} !"),
    [STRINGID_EMBARGOENDS]                          = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} peut de nouveau utiliser des objets !"),
    [STRINGID_ELECTROMAGNETISM]                     = COMPOUND_STRING("électromagnétisme"),
    [STRINGID_BUFFERENDS]                           = COMPOUND_STRING("{B_BUFF1} de {B_SCR_NAME_WITH_PREFIX2} prend fin !"),
    [STRINGID_TELEKINESISENDS]                      = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} est libéré de la télékinésie !"),
    [STRINGID_TAILWINDENDS]                         = COMPOUND_STRING("Le vent arrière de {B_ATK_TEAM2} retombe !"),
    [STRINGID_LUCKYCHANTENDS]                       = COMPOUND_STRING("Air Veinard de {B_ATK_TEAM2} prend fin !"),
    [STRINGID_TRICKROOMENDS]                        = COMPOUND_STRING("Les dimensions reprennent leur forme normale !"),
    [STRINGID_WONDERROOMENDS]                       = COMPOUND_STRING("Zone Étrange prend fin. La Défense et la Défense Spéciale redeviennent normales !"),
    [STRINGID_MAGICROOMENDS]                        = COMPOUND_STRING("Zone Magique prend fin. Les objets tenus retrouvent leur effet !"),
    [STRINGID_MUDSPORTENDS]                         = COMPOUND_STRING("L'effet de Lance-Boue se dissipe."),
    [STRINGID_WATERSPORTENDS]                       = COMPOUND_STRING("L'effet de Tourniquet se dissipe."),
    [STRINGID_GRAVITYENDS]                          = COMPOUND_STRING("La gravité redevient normale !"),
    [STRINGID_AQUARINGHEAL]                         = COMPOUND_STRING("Le voile d'eau soigne {B_ATK_NAME_WITH_PREFIX2} !"),
    [STRINGID_ELECTRICTERRAINENDS]                  = COMPOUND_STRING("L'électricité quitte le terrain."),
    [STRINGID_MISTYTERRAINENDS]                     = COMPOUND_STRING("La brume quitte le terrain."),
    [STRINGID_PSYCHICTERRAINENDS]                   = COMPOUND_STRING("Le terrain redevient normal !"),
    [STRINGID_GRASSYTERRAINENDS]                    = COMPOUND_STRING("L'herbe disparaît du terrain."),
    [STRINGID_TARGETABILITYSTATRAISE]               = COMPOUND_STRING("{B_DEF_ABILITY} de {B_DEF_NAME_WITH_PREFIX2} augmente{B_BUFF2} sa stat {B_BUFF1} !"), // Not in Gen 5+
    [STRINGID_STATWASMAXEDOUT]                      = COMPOUND_STRING("La stat {B_BUFF1} de {B_SCR_NAME_WITH_PREFIX2} est au maximum !"),
    [STRINGID_ATTACKERABILITYSTATRAISE]             = COMPOUND_STRING("{B_ATK_ABILITY} de {B_ATK_NAME_WITH_PREFIX2} augmente{B_BUFF2} sa stat {B_BUFF1} !"), // Not in Gen 5+
    [STRINGID_POISONHEALHPUP]                       = COMPOUND_STRING("Le poison soigne un peu {B_ATK_NAME_WITH_PREFIX2} !"), // Not in Gen 5+
    [STRINGID_BADDREAMSDMG]                         = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX} est tourmenté par ses cauchemars !"),
    [STRINGID_MOLDBREAKERENTERS]                    = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX} brise le moule !"),
    [STRINGID_TERAVOLTENTERS]                       = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX} dégage une aura électrique !"),
    [STRINGID_TURBOBLAZEENTERS]                     = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX} dégage une aura brûlante !"),
    [STRINGID_SLOWSTARTENTERS]                      = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX} a du mal à démarrer !"),
    [STRINGID_SLOWSTARTEND]                         = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} se met enfin en route !"),
    [STRINGID_SOLARPOWERHPDROP]                     = COMPOUND_STRING("{B_ATK_ABILITY} de {B_ATK_NAME_WITH_PREFIX2} lui coûte des PV !"), // Not in Gen 5+
    [STRINGID_PKMNWASHURT]                          = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} est blessé !"),
    [STRINGID_ANTICIPATIONACTIVATES]                = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX} frissonne !"),
    [STRINGID_FOREWARNACTIVATES]                    = COMPOUND_STRING("{B_SCR_ABILITY} prévient {B_SCR_NAME_WITH_PREFIX2} : {B_EFF_NAME_WITH_PREFIX2} a {B_BUFF1} !"),
    [STRINGID_ICEBODYHPGAIN]                        = COMPOUND_STRING("{B_ATK_ABILITY} de {B_ATK_NAME_WITH_PREFIX2} le soigne un peu !"), // Not in Gen 5+
    [STRINGID_SNOWWARNINGHAIL]                      = COMPOUND_STRING("Il commence à grêler !"),
    [STRINGID_FRISKACTIVATES]                       = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX} est fouillé : il tient {B_LAST_ITEM} !"),
    [STRINGID_UNNERVEENTERS]                        = COMPOUND_STRING("{B_EFF_TEAM1} est trop stressée pour manger des Baies !"),
    [STRINGID_HARVESTBERRY]                         = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} récolte {B_LAST_ITEM} !"),
    [STRINGID_PROTEANTYPECHANGE]                    = COMPOUND_STRING("{B_ATK_ABILITY} de {B_ATK_NAME_WITH_PREFIX2} le change en type {B_BUFF1} !"),
    [STRINGID_SYMBIOSISITEMPASS]                    = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX} partage {B_LAST_ITEM} avec {B_EFF_NAME_WITH_PREFIX2} !"),
    [STRINGID_STEALTHROCKDMG]                       = COMPOUND_STRING("Des pierres pointues transpercent {B_SCR_NAME_WITH_PREFIX2} !"),
    [STRINGID_TOXICSPIKESABSORBED]                  = COMPOUND_STRING("Les pics toxiques disparaissent autour de {B_EFF_TEAM2} !"),
    [STRINGID_TOXICSPIKESPOISONED]                  = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX} est empoisonné !"),
    [STRINGID_TOXICSPIKESBADLYPOISONED]             = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX} est gravement empoisonné !"),
    [STRINGID_STICKYWEBSWITCHIN]                    = COMPOUND_STRING("{B_EFF_NAME_WITH_PREFIX} est pris dans une toile gluante !"),
    [STRINGID_HEALINGWISHCAMETRUE]                  = COMPOUND_STRING("Le Vœu Soin de {B_SCR_NAME_WITH_PREFIX2} se réalise !"),
    [STRINGID_HEALINGWISHHEALED]                    = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX} récupère toutes ses forces !"),
    [STRINGID_LUNARDANCECAMETRUE]                   = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX} est baigné d'un clair de lune mystique !"),
    [STRINGID_CURSEDBODYDISABLED]                   = COMPOUND_STRING("{B_BUFF1} de {B_ATK_NAME_WITH_PREFIX2} est bloquée !"),
    [STRINGID_ATTACKERACQUIREDABILITY]              = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} acquiert {B_ATK_ABILITY} !"), // Not in Gen 5+
    [STRINGID_TARGETABILITYSTATLOWER]               = COMPOUND_STRING("{B_DEF_ABILITY} de {B_DEF_NAME_WITH_PREFIX2} baisse {B_BUFF2}sa stat {B_BUFF1} !"), // Not in Gen 5+
    [STRINGID_TARGETSTATWONTGOHIGHER]               = COMPOUND_STRING("La stat {B_BUFF1} de {B_DEF_NAME_WITH_PREFIX2} ne peut plus monter !"),
    [STRINGID_PKMNMOVEBOUNCEDABILITY]               = COMPOUND_STRING("{B_CURRENT_MOVE} de {B_DEF_NAME_WITH_PREFIX2} est renvoyée !"),
    [STRINGID_IMPOSTERTRANSFORM]                    = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} se transforme en {B_DEF_NAME_WITH_PREFIX2} !"),
    [STRINGID_ASSAULTVESTDOESNTALLOW]               = COMPOUND_STRING("{B_LAST_ITEM} empêche d'utiliser les capacités de statut !\p"),
    [STRINGID_GRAVITYPREVENTSUSAGE]                 = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} ne peut pas utiliser {B_CURRENT_MOVE} à cause de la gravité !\p"),
    [STRINGID_HEALBLOCKPREVENTSUSAGE]               = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} ne peut pas se soigner !\p"),
    [STRINGID_NOTDONEYET]                           = COMPOUND_STRING("Cet effet n'est pas encore programmé !\p"),
    [STRINGID_STICKYWEBUSED]                        = COMPOUND_STRING("Une toile gluante s'étend sous les pieds de {B_DEF_TEAM2} !"),
    [STRINGID_QUASHSUCCESS]                         = COMPOUND_STRING("L'action de {B_DEF_NAME_WITH_PREFIX2} est repoussée !"),
    [STRINGID_PKMNBLEWAWAYTOXICSPIKES]              = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} balaie les Pics Toxik !"),
    [STRINGID_PKMNBLEWAWAYSTICKYWEB]                = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} balaie la Toile Gluante !"),
    [STRINGID_PKMNBLEWAWAYSTEALTHROCK]              = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} balaie le Piège de Roc !"),
    [STRINGID_IONDELUGEON]                          = COMPOUND_STRING("Une pluie d'ions s'abat sur le terrain !"),
    [STRINGID_TOPSYTURVYSWITCHEDSTATS]              = COMPOUND_STRING("Les changements de stats de {B_DEF_NAME_WITH_PREFIX2} sont inversés !"),
    [STRINGID_TERRAINBECOMESMISTY]                  = COMPOUND_STRING("La brume enveloppe le terrain !"),
    [STRINGID_TERRAINBECOMESGRASSY]                 = COMPOUND_STRING("L'herbe recouvre le terrain !"),
    [STRINGID_TERRAINBECOMESELECTRIC]               = COMPOUND_STRING("Un courant électrique parcourt le terrain !"),
    [STRINGID_TERRAINBECOMESPSYCHIC]                = COMPOUND_STRING("Le terrain devient bizarre !"),
    [STRINGID_TARGETELECTRIFIED]                    = COMPOUND_STRING("Les capacités de {B_DEF_NAME_WITH_PREFIX2} sont électrifiées !"),
    [STRINGID_MEGAEVOREACTING]                      = COMPOUND_STRING("{B_LAST_ITEM} de {B_ATK_NAME_WITH_PREFIX2} réagit au Méga-Anneau de {B_ATK_TRAINER_NAME} !"), //actually displays the type of mega ring in inventory, but we didnt implement them :(
    [STRINGID_MEGAEVOEVOLVED]                       = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} méga-évolue en Méga-{B_BUFF1} !"),
    [STRINGID_DRASTICALLY]                          = gText_drastically,
    [STRINGID_SEVERELY]                             = gText_severely,
    [STRINGID_INFESTATION]                          = COMPOUND_STRING("{B_EFF_NAME_WITH_PREFIX} est infesté par {B_SCR_NAME_WITH_PREFIX2} !"),
    [STRINGID_NOEFFECTONTARGET]                     = COMPOUND_STRING("Ça n'aura aucun effet sur {B_DEF_NAME_WITH_PREFIX2} !"),
    [STRINGID_BURSTINGFLAMESHIT]                    = COMPOUND_STRING("Les flammes touchent {B_EFF_NAME_WITH_PREFIX2} !"),
    [STRINGID_BESTOWITEMGIVING]                     = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX} reçoit {B_LAST_ITEM} de {B_ATK_NAME_WITH_PREFIX2} !"),
    [STRINGID_THIRDTYPEADDED]                       = COMPOUND_STRING("Le type {B_BUFF1} s'ajoute à {B_DEF_NAME_WITH_PREFIX2} !"),
    [STRINGID_FELLFORFEINT]                         = COMPOUND_STRING("{B_EFF_NAME_WITH_PREFIX} tombe dans la feinte !"),
    [STRINGID_POKEMONCANNOTUSEMOVE]                 = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} ne peut pas utiliser {B_CURRENT_MOVE} !"),
    [STRINGID_COVEREDINPOWDER]                      = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX} est couvert de poudre !"),
    [STRINGID_POWDEREXPLODES]                       = COMPOUND_STRING("La flamme touche la poudre… et BOUM, ça explose !"),
    [STRINGID_BELCHCANTSELECT]                      = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} n'a mangé aucune Baie, il ne peut pas roter !\p"),
    [STRINGID_SPECTRALTHIEFSTEAL]                   = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} vole les bonus de stats de sa cible !"),
    [STRINGID_GRAVITYGROUNDING]                     = COMPOUND_STRING("La gravité fait tomber {B_DEF_NAME_WITH_PREFIX2} du ciel !"),
    [STRINGID_MISTYTERRAINPREVENTS]                 = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX} s'entoure d'une brume protectrice !"),
    [STRINGID_GRASSYTERRAINHEALS]                   = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} est soigné par le Champ Herbu !"),
    [STRINGID_ELECTRICTERRAINPREVENTS]              = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX} est protégé par le Champ Électrifié !"),
    [STRINGID_PSYCHICTERRAINPREVENTS]               = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX} est protégé par le Champ Psychique !"),
    [STRINGID_SAFETYGOGGLESPROTECTED]               = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX} n'est pas affecté grâce à {B_LAST_ITEM} !"),
    [STRINGID_FLOWERVEILPROTECTED]                  = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX} s'entoure d'un voile de pétales !"),
    [STRINGID_FLOWERVEILPROTECTEDTARGET]            = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX} s'entoure d'un voile de pétales !"),
    [STRINGID_AROMAVEILPROTECTED]                   = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX} est protégé par un voile aromatique !"),
    [STRINGID_CELEBRATEMESSAGE]                     = COMPOUND_STRING("Félicitations, {B_PLAYER_NAME} !"),
    [STRINGID_USEDINSTRUCTEDMOVE]                   = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX} suit les instructions de {B_ATK_NAME_WITH_PREFIX2} !"),
    [STRINGID_THROATCHOPENDS]                       = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} peut de nouveau utiliser des capacités sonores !"),
    [STRINGID_PKMNCANTUSEMOVETHROATCHOP]            = COMPOUND_STRING("Exécu-Gorge empêche {B_ATK_NAME_WITH_PREFIX2} d'utiliser certaines capacités !\p"),
    [STRINGID_LASERFOCUS]                           = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} se concentre intensément !"),
    [STRINGID_GEMACTIVATES]                         = COMPOUND_STRING("{B_LAST_ITEM} renforce la puissance de {B_ATK_NAME_WITH_PREFIX2} !"),
    [STRINGID_BERRYDMGREDUCES]                      = COMPOUND_STRING("{B_LAST_ITEM} de {B_SCR_NAME_WITH_PREFIX2} réduit les dégâts subis !"),
    [STRINGID_AIRBALLOONFLOAT]                      = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX} flotte grâce à son Ballon !"),
    [STRINGID_AIRBALLOONPOP]                        = COMPOUND_STRING("Le Ballon de {B_DEF_NAME_WITH_PREFIX2} éclate !"),
    [STRINGID_INCINERATEBURN]                       = COMPOUND_STRING("{B_LAST_ITEM} de {B_EFF_NAME_WITH_PREFIX2} est réduit en cendres !"),
    [STRINGID_BUGBITE]                              = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} vole et dévore {B_LAST_ITEM} de sa cible !"),
    [STRINGID_ILLUSIONWOREOFF]                      = COMPOUND_STRING("L'illusion de {B_SCR_NAME_WITH_PREFIX2} se dissipe !"),
    [STRINGID_ATTACKERCUREDTARGETSTATUS]            = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} soigne le problème de {B_DEF_NAME_WITH_PREFIX2} !"),
    [STRINGID_ATTACKERLOSTFIRETYPE]                 = COMPOUND_STRING("{B_EFF_NAME_WITH_PREFIX} s'est consumé entièrement !"),
    [STRINGID_HEALERCURE]                           = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} est soigné par {B_SCR_NAME_WITH_PREFIX2} !"),
    [STRINGID_SCRIPTINGABILITYSTATRAISE]            = COMPOUND_STRING("{B_SCR_ABILITY} de {B_SCR_NAME_WITH_PREFIX2} augmente{B_BUFF2} sa stat {B_BUFF1} !"),
    [STRINGID_RECEIVERABILITYTAKEOVER]              = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX} récupère {B_SCR_ABILITY} !"),
    [STRINGID_PKNMABSORBINGPOWER]                   = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} absorbe de la puissance !"),
    [STRINGID_NOONEWILLBEABLETORUNAWAY]             = COMPOUND_STRING("Personne ne pourra quitter le combat au prochain tour !"),
    [STRINGID_DESTINYKNOTACTIVATES]                 = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX} tombe amoureux à cause de {B_LAST_ITEM} !"),
    [STRINGID_CLOAKEDINAFREEZINGLIGHT]              = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} est enveloppé d'une lumière glaciale !"),
    [STRINGID_CLEARAMULETWONTLOWERSTATS]            = COMPOUND_STRING("{B_LAST_ITEM} de {B_SCR_NAME_WITH_PREFIX2} empêche ses stats de baisser !"),
    [STRINGID_FERVENTWISHREACHED]                   = COMPOUND_STRING("Le vœu ardent de {B_ATK_TRAINER_NAME} atteint {B_ATK_NAME_WITH_PREFIX2} !"),
    [STRINGID_AIRLOCKACTIVATES]                     = COMPOUND_STRING("Les effets de la météo disparaissent."),
    [STRINGID_PRESSUREENTERS]                       = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX} exerce une forte pression !"),
    [STRINGID_DARKAURAENTERS]                       = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX} dégage une aura ténébreuse !"),
    [STRINGID_FAIRYAURAENTERS]                      = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX} dégage une aura féerique !"),
    [STRINGID_AURABREAKENTERS]                      = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX} inverse les auras de tous les autres Pokémon !"),
    [STRINGID_COMATOSEENTERS]                       = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX} somnole !"),
    [STRINGID_SCREENCLEANERENTERS]                  = COMPOUND_STRING("Tous les écrans du terrain sont dissipés !"),
    [STRINGID_FETCHEDPOKEBALL]                      = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX} trouve {B_LAST_ITEM} !"),
    [STRINGID_ASANDSTORMKICKEDUP]                   = COMPOUND_STRING("Une tempête de sable se lève !"),
    [STRINGID_PKMNSWILLPERISHIN3TURNS]              = COMPOUND_STRING("Les deux Pokémon seront K.O. dans trois tours !"),
    [STRINGID_AURAFLAREDTOLIFE]                     = COMPOUND_STRING("L'aura de {B_ATK_NAME_WITH_PREFIX2} s'embrase !"),
    [STRINGID_ASONEENTERS]                          = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX} possède deux talents !"),
    [STRINGID_CURIOUSMEDICINEENTERS]                = COMPOUND_STRING("Les changements de stats de {B_EFF_NAME_WITH_PREFIX2} sont annulés !"),
    [STRINGID_CANACTFASTERTHANKSTO]                 = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} agit plus vite grâce à {B_BUFF1} !"),
    [STRINGID_MICLEBERRYACTIVATES]                  = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX} augmente la précision de sa prochaine capacité grâce à {B_LAST_ITEM} !"),
    [STRINGID_PKMNSHOOKOFFTHETAUNT]                 = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX} se débarrasse de la provocation !"),
    [STRINGID_PKMNGOTOVERITSINFATUATION]            = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX} n'est plus amoureux !"),
    [STRINGID_ITEMCANNOTBEREMOVED]                  = COMPOUND_STRING("L'objet de {B_ATK_NAME_WITH_PREFIX2} ne peut pas être retiré !"),
    [STRINGID_STICKYBARBTRANSFER]                   = COMPOUND_STRING("{B_LAST_ITEM} s'accroche à {B_ATK_NAME_WITH_PREFIX2} !"),
    [STRINGID_PKMNBURNHEALED]                       = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX} n'est plus brûlé !"),
    [STRINGID_REDCARDACTIVATE]                      = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX} brandit son Carton Rouge face à {B_ATK_NAME_WITH_PREFIX2} !"),
    [STRINGID_EJECTBUTTONACTIVATE]                  = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX} est rappelé grâce à {B_LAST_ITEM} !"),
    [STRINGID_ATKGOTOVERINFATUATION]                = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX} se soigne de son amour grâce à {B_LAST_ITEM} !"),
    [STRINGID_TORMENTEDNOMORE]                      = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX} n'est plus tourmenté !"),
    [STRINGID_HEALBLOCKEDNOMORE]                    = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX} peut de nouveau se soigner !"),
    [STRINGID_ATTACKERBECAMEFULLYCHARGED]           = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} se charge à bloc grâce au lien avec son Dresseur !\p"),
    [STRINGID_ATTACKERBECAMEASHSPECIES]             = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} devient Amphinobi-Sacha !\p"),
    [STRINGID_EXTREMELYHARSHSUNLIGHT]               = COMPOUND_STRING("Le soleil devient brûlant !"),
    [STRINGID_EXTREMESUNLIGHTFADED]                 = COMPOUND_STRING("Le soleil brûlant disparaît !"),
    [STRINGID_MOVEEVAPORATEDINTHEHARSHSUNLIGHT]     = COMPOUND_STRING("L'attaque Eau s'évapore sous le soleil brûlant !"),
    [STRINGID_EXTREMELYHARSHSUNLIGHTWASNOTLESSENED] = COMPOUND_STRING("Le soleil brûlant ne faiblit pas !"),
    [STRINGID_HEAVYRAIN]                            = COMPOUND_STRING("Un déluge s'abat !"),
    [STRINGID_HEAVYRAINLIFTED]                      = COMPOUND_STRING("Le déluge s'arrête !"),
    [STRINGID_MOVEFIZZLEDOUTINTHEHEAVYRAIN]         = COMPOUND_STRING("L'attaque Feu s'éteint sous le déluge !"),
    [STRINGID_NORELIEFROMHEAVYRAIN]                 = COMPOUND_STRING("Le déluge ne faiblit pas !"),
    [STRINGID_MYSTERIOUSAIRCURRENT]                 = COMPOUND_STRING("Un mystérieux courant aérien protège les Pokémon Vol !"),
    [STRINGID_STRONGWINDSDISSIPATED]                = COMPOUND_STRING("Le mystérieux courant aérien se dissipe !"),
    [STRINGID_MYSTERIOUSAIRCURRENTBLOWSON]          = COMPOUND_STRING("Le mystérieux courant aérien souffle toujours !"),
    [STRINGID_ATTACKWEAKENEDBSTRONGWINDS]           = COMPOUND_STRING("Le mystérieux courant aérien affaiblit l'attaque !"),
    [STRINGID_STUFFCHEEKSCANTSELECT]                = COMPOUND_STRING("Impossible d'utiliser cette capacité sans Baie !\p"),
    [STRINGID_PKMNREVERTEDTOPRIMAL]                 = COMPOUND_STRING("Primo-Résurgence de {B_SCR_NAME_WITH_PREFIX2} ! Il retrouve sa forme originelle !"),
    [STRINGID_BUTPOKEMONCANTUSETHEMOVE]             = COMPOUND_STRING("Mais {B_ATK_NAME_WITH_PREFIX2} ne peut pas utiliser cette capacité !"),
    [STRINGID_BUTHOOPACANTUSEIT]                    = COMPOUND_STRING("Mais {B_ATK_NAME_WITH_PREFIX2} ne peut pas l'utiliser sous cette forme !"),
    [STRINGID_BROKETHROUGHPROTECTION]               = COMPOUND_STRING("L'attaque perce la protection de {B_EFF_NAME_WITH_PREFIX2} !"),
    [STRINGID_ABILITYALLOWSONLYMOVE]                = COMPOUND_STRING("{B_ATK_ABILITY} ne permet d'utiliser que {B_CURRENT_MOVE} !\p"),
    [STRINGID_SWAPPEDABILITIES]                     = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX} échange son talent avec sa cible !"),
    [STRINGID_PKMNHEALEDPOISON]                     = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX} n'est plus empoisonné !"),
    [STRINGID_BATTLERTYPECHANGEDTO]                 = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX} devient de type {B_BUFF1} !"),
    [STRINGID_BOTHCANNOLONGERESCAPE]                = COMPOUND_STRING("Aucun des deux Pokémon ne peut fuir !"),
    [STRINGID_CANTESCAPEDUETOUSEDMOVE]              = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} ne peut plus fuir à cause d'Ultime Bastion !"),
    [STRINGID_PKMNBECAMEWEAKERTOFIRE]               = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX} devient vulnérable au feu !"),
    [STRINGID_ABOUTTOUSEPOLTERGEIST]                = COMPOUND_STRING("{B_EFF_NAME_WITH_PREFIX} va être attaqué par {B_LAST_ITEM} !"),
    [STRINGID_CANTESCAPEBECAUSEOFCURRENTMOVE]       = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX} ne peut plus fuir à cause de Prise de Tête !"),
    [STRINGID_NEUTRALIZINGGASENTERS]                = COMPOUND_STRING("Un gaz inhibiteur envahit le terrain !"),
    [STRINGID_NEUTRALIZINGGASOVER]                  = COMPOUND_STRING("Le gaz inhibiteur se dissipe !"),
    [STRINGID_TARGETTOOHEAVY]                       = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX} est trop lourd pour être soulevé !"),
    [STRINGID_PKMNTOOKTARGETHIGH]                   = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} emporte {B_DEF_NAME_WITH_PREFIX2} dans les airs !"),
    [STRINGID_PKMNINSNAPTRAP]                       = COMPOUND_STRING("{B_EFF_NAME_WITH_PREFIX} est pris dans un piège à mâchoires !"),
    [STRINGID_METEORBEAMCHARGING]                   = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} déborde d'énergie cosmique !"),
    [STRINGID_HEATUPBEAK]                           = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX} fait chauffer son bec !"),
    [STRINGID_COURTCHANGE]                          = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} inverse les effets des deux camps !"),
    [STRINGID_ZPOWERSURROUNDS]                      = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} s'entoure de sa Force Z !"),
    [STRINGID_ZMOVEUNLEASHED]                       = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} déchaîne sa capacité Z !"),
    [STRINGID_ZMOVERESETSSTATS]                     = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX} rétablit ses stats grâce à la Force Z !"),
    [STRINGID_ZMOVEALLSTATSUP]                      = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX} augmente ses stats grâce à la Force Z !"),
    [STRINGID_ZMOVEZBOOSTCRIT]                      = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX} augmente son taux de critique grâce à la Force Z !"),
    [STRINGID_ZMOVERESTOREHP]                       = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX} récupère ses PV grâce à la Force Z !"),
    [STRINGID_ZMOVESTATUP]                          = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX} augmente ses stats grâce à la Force Z !"),
    [STRINGID_ZMOVEHPTRAP]                          = COMPOUND_STRING("La Force Z restaure les PV de {B_SCR_NAME_WITH_PREFIX2} !"),
    [STRINGID_ATTACKEREXPELLEDTHEPOISON]            = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} expulse le poison pour ne pas t'inquiéter !"),
    [STRINGID_ATTACKERSHOOKITSELFAWAKE]             = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} se réveille tout seul pour ne pas t'inquiéter !"),
    [STRINGID_ATTACKERBROKETHROUGHPARALYSIS]        = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} rassemble ses forces et brise sa paralysie pour ne pas t'inquiéter !"),
    [STRINGID_ATTACKERHEALEDITSBURN]                = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} guérit sa brûlure à force de volonté pour ne pas t'inquiéter !"),
    [STRINGID_ATTACKERMELTEDTHEICE]                 = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} fait fondre la glace à force de détermination pour ne pas t'inquiéter !"),
    [STRINGID_TARGETTOUGHEDITOUT]                   = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX} serre les dents pour ne pas te rendre triste !"),
    [STRINGID_ATTACKERLOSTELECTRICTYPE]             = COMPOUND_STRING("{B_EFF_NAME_WITH_PREFIX} a épuisé toute son électricité !"),
    [STRINGID_ATTACKERSWITCHEDSTATWITHTARGET]       = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} échange sa stat {B_BUFF1} avec sa cible !"),
    [STRINGID_BEINGHITCHARGEDPKMNWITHPOWER]         = COMPOUND_STRING("{B_CURRENT_MOVE} recharge {B_EFF_NAME_WITH_PREFIX2} en énergie !"),
    [STRINGID_SUNLIGHTACTIVATEDABILITY]             = COMPOUND_STRING("Le soleil active Paléosynthèse de {B_SCR_NAME_WITH_PREFIX2} !"),
    [STRINGID_ORICHALCUMPULSEACTIVATES]             = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX} fait briller le soleil, et son pouls ancestral s'emballe !"),
    [STRINGID_ORICHALCUMPULSEACTIVATESINSUN]        = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX} se gorge de soleil, et son pouls ancestral s'emballe !"),
    [STRINGID_STATWASHEIGHTENED]                    = COMPOUND_STRING("La stat {B_BUFF1} de {B_SCR_NAME_WITH_PREFIX2} augmente !"),
    [STRINGID_ELECTRICTERRAINACTIVATEDABILITY]      = COMPOUND_STRING("Le Champ Électrifié active Charge Quantique de {B_SCR_NAME_WITH_PREFIX2} !"),
    [STRINGID_HADRONENGINEACTIVATES]                = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX} électrifie le terrain, et son moteur futuriste s'emballe !"),
    [STRINGID_HADRONENGINEACTIVATESINTERRAIN]       = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX} utilise le Champ Électrifié pour alimenter son moteur futuriste !"),
    [STRINGID_ABILITYWEAKENEDSURROUNDINGMONSSTAT]   = COMPOUND_STRING("{B_SCR_ABILITY} de {B_SCR_NAME_WITH_PREFIX2} affaiblit la stat {B_BUFF1} de tous les Pokémon alentour !\p"),
    [STRINGID_ATTACKERGAINEDSTRENGTHFROMTHEFALLEN]  = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX} puise sa force dans ses camarades tombés !"),
    [STRINGID_PKMNSABILITYPREVENTSABILITY]          = COMPOUND_STRING("{B_SCR_ABILITY} de {B_SCR_NAME_WITH_PREFIX2} empêche {B_DEF_ABILITY} de {B_DEF_NAME_WITH_PREFIX2} de fonctionner !"), //not in gen 5+, ability popup
    [STRINGID_PREPARESHELLTRAP]                     = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX} pose une Carapiège !"),
    [STRINGID_SHELLTRAPDIDNTWORK]                   = COMPOUND_STRING("La Carapiège de {B_ATK_NAME_WITH_PREFIX2} n'a pas fonctionné !"),
    [STRINGID_SPIKESDISAPPEAREDFROMTEAM]            = COMPOUND_STRING("Les picots disparaissent autour de {B_ATK_TEAM2} !"),
    [STRINGID_TOXICSPIKESDISAPPEAREDFROMTEAM]       = COMPOUND_STRING("Les pics toxiques disparaissent autour de {B_ATK_TEAM2} !"),
    [STRINGID_STICKYWEBDISAPPEAREDFROMTEAM]         = COMPOUND_STRING("La toile gluante disparaît sous les pieds de {B_ATK_TEAM2} !"),
    [STRINGID_STEALTHROCKDISAPPEAREDFROMTEAM]       = COMPOUND_STRING("Les pierres pointues disparaissent autour de {B_ATK_TEAM2} !"),
    [STRINGID_COULDNTFULLYPROTECT]                  = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX} ne peut pas se protéger entièrement et se blesse !"),
    [STRINGID_STOCKPILEDEFFECTWOREOFF]              = COMPOUND_STRING("L'effet de Stockage de {B_ATK_NAME_WITH_PREFIX2} prend fin !"),
    [STRINGID_PKMNREVIVEDREADYTOFIGHT]              = COMPOUND_STRING("{B_BUFF1} est ranimé et prêt à reprendre le combat !"),
    [STRINGID_ITEMRESTOREDSPECIESHEALTH]            = COMPOUND_STRING("{B_BUFF1} récupère des PV."),
    [STRINGID_ITEMCUREDSPECIESSTATUS]               = COMPOUND_STRING("{B_BUFF1} est soigné !"), // Not in Gen 5+
    [STRINGID_ITEMRESTOREDSPECIESPP]                = COMPOUND_STRING("Les PP de {B_BUFF2} de {B_BUFF1} sont restaurés !"),
    [STRINGID_THUNDERCAGETRAPPED]                   = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX} emprisonne {B_EFF_NAME_WITH_PREFIX2} !"),
    [STRINGID_PKMNHURTBYFROSTBITE]                  = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} souffre de ses engelures !"),
    [STRINGID_PKMNGOTFROSTBITE]                     = COMPOUND_STRING("{B_EFF_NAME_WITH_PREFIX} a des engelures !"),
    [STRINGID_PKMNSITEMHEALEDFROSTBITE]             = COMPOUND_STRING("{B_LAST_ITEM} de {B_SCR_NAME_WITH_PREFIX2} soigne ses engelures !"),
    [STRINGID_ATTACKERHEALEDITSFROSTBITE]           = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} guérit ses engelures à force de volonté pour ne pas t'inquiéter !"),
    [STRINGID_PKMNFROSTBITEHEALED]                  = COMPOUND_STRING("Les engelures de {B_SCR_NAME_WITH_PREFIX2} sont soignées !"),
    [STRINGID_PKMNFROSTBITEHEALEDBY]                = COMPOUND_STRING("{B_CURRENT_MOVE} de {B_SCR_NAME_WITH_PREFIX2} soigne ses engelures !"),
    [STRINGID_MIRRORHERBCOPIED]                     = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX} copie les changements de stats adverses grâce à son Herbe Miroir !"),
    [STRINGID_STARTEDSNOW]                          = COMPOUND_STRING("Il commence à neiger !"),
    [STRINGID_SNOWCONTINUES]                        = COMPOUND_STRING("La neige continue de tomber."), //not in gen 5+ (lol)
    [STRINGID_SNOWSTOPPED]                          = COMPOUND_STRING("La neige s'arrête."),
    [STRINGID_SNOWWARNINGSNOW]                      = COMPOUND_STRING("Il commence à neiger !"),
    [STRINGID_PKMNITEMMELTED]                       = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} fait fondre {B_LAST_ITEM} de {B_DEF_NAME_WITH_PREFIX2} !"),
    [STRINGID_ULTRABURSTREACTING]                   = COMPOUND_STRING("Une lumière éclatante va jaillir de {B_ATK_NAME_WITH_PREFIX2} !"),
    [STRINGID_ULTRABURSTCOMPLETED]                  = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} retrouve sa vraie puissance grâce à l'Ultra-Explosion !"),
    [STRINGID_TEAMGAINEDEXP]                        = COMPOUND_STRING("Le reste de ton équipe gagne de l'Exp. grâce au Multi Exp !\p"),
    [STRINGID_CURRENTMOVECANTSELECT]                = COMPOUND_STRING("Impossible d'utiliser {B_BUFF1} !\p"),
    [STRINGID_TARGETISBEINGSALTCURED]               = COMPOUND_STRING("{B_EFF_NAME_WITH_PREFIX} est mis en salaison !"),
    [STRINGID_TARGETISHURTBYSALTCURE]               = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} souffre de {B_BUFF1} !"),
    [STRINGID_TARGETCOVEREDINSTICKYCANDYSYRUP]      = COMPOUND_STRING("{B_EFF_NAME_WITH_PREFIX} est couvert de sirop collant !"),
    [STRINGID_SHARPSTEELFLOATS]                     = COMPOUND_STRING("Des éclats d'acier tranchants flottent autour de {B_DEF_TEAM2} !"),
    [STRINGID_SHARPSTEELDMG]                        = COMPOUND_STRING("Les éclats d'acier entaillent {B_DEF_NAME_WITH_PREFIX2} !"),
    [STRINGID_PKMNBLEWAWAYSHARPSTEEL]               = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} balaie les éclats d'acier !"),
    [STRINGID_SHARPSTEELDISAPPEAREDFROMTEAM]        = COMPOUND_STRING("Les éclats d'acier autour de {B_ATK_TEAM2} disparaissent !"),
    [STRINGID_TEAMTRAPPEDWITHVINES]                 = COMPOUND_STRING("{B_EFF_TEAM1} est prise dans des lianes !"),
    [STRINGID_PKMNHURTBYVINES]                      = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} est fouetté sans pitié par les lianes !"),
    [STRINGID_TEAMCAUGHTINVORTEX]                   = COMPOUND_STRING("{B_EFF_TEAM1} est prise dans un tourbillon d'eau !"),
    [STRINGID_PKMNHURTBYVORTEX]                     = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} est blessé par le tourbillon !"),
    [STRINGID_TEAMSURROUNDEDBYFIRE]                 = COMPOUND_STRING("{B_EFF_TEAM1} est encerclée par les flammes !"),
    [STRINGID_PKMNBURNINGUP]                        = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} brûle dans les flammes !"),
    [STRINGID_TEAMSURROUNDEDBYROCKS]                = COMPOUND_STRING("{B_EFF_TEAM1} est encerclée de rochers !"),
    [STRINGID_PKMNHURTBYROCKSTHROWN]                = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} est frappé par les rochers !"),
    [STRINGID_MOVEBLOCKEDBYDYNAMAX]                 = COMPOUND_STRING("La capacité est bloquée par la puissance Dynamax !"),
    [STRINGID_ZEROTOHEROTRANSFORMATION]             = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX} subit une transformation héroïque !"),
    [STRINGID_THETWOMOVESBECOMEONE]                 = COMPOUND_STRING("Les deux capacités fusionnent ! C'est une attaque combinée !{PAUSE 16}"),
    [STRINGID_ARAINBOWAPPEAREDONSIDE]               = COMPOUND_STRING("Un arc-en-ciel apparaît au-dessus de {B_EFF_TEAM2} !"),
    [STRINGID_THERAINBOWDISAPPEARED]                = COMPOUND_STRING("L'arc-en-ciel au-dessus de {B_ATK_TEAM2} disparaît !"),
    [STRINGID_WAITINGFORPARTNERSMOVE]               = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} attend l'action de {B_ATK_PARTNER_NAME}…{PAUSE 16}"),
    [STRINGID_SEAOFFIREENVELOPEDSIDE]               = COMPOUND_STRING("Une mer de flammes engloutit {B_EFF_TEAM2} !"),
    [STRINGID_HURTBYTHESEAOFFIRE]                   = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} est brûlé par la mer de flammes !"),
    [STRINGID_THESEAOFFIREDISAPPEARED]              = COMPOUND_STRING("La mer de flammes autour de {B_ATK_TEAM2} disparaît !"),
    [STRINGID_SWAMPENVELOPEDSIDE]                   = COMPOUND_STRING("Un marécage engloutit {B_EFF_TEAM2} !"),
    [STRINGID_THESWAMPDISAPPEARED]                  = COMPOUND_STRING("Le marécage autour de {B_ATK_TEAM2} disparaît !"),
    [STRINGID_PKMNTELLCHILLINGRECEPTIONJOKE]        = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} s'apprête à raconter une blague affreusement nulle !"),
    [STRINGID_HOSPITALITYRESTORATION]               = COMPOUND_STRING("{B_EFF_NAME_WITH_PREFIX} boit tout le matcha préparé par {B_SCR_NAME_WITH_PREFIX2} !"),
    [STRINGID_ELECTROSHOTCHARGING]                  = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} absorbe de l'électricité !"),
    [STRINGID_ITEMWASUSEDUP]                        = COMPOUND_STRING("{B_LAST_ITEM} est épuisé…"),
    [STRINGID_ATTACKERLOSTITSTYPE]                  = COMPOUND_STRING("{B_EFF_NAME_WITH_PREFIX} perd son type {B_BUFF1} !"),
    [STRINGID_SHEDITSTAIL]                          = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} se coupe la queue pour créer un leurre !"),
    [STRINGID_CLOAKEDINAHARSHLIGHT]                 = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} est enveloppé d'une lumière aveuglante !"),
    [STRINGID_SUPERSWEETAROMAWAFTS]                 = COMPOUND_STRING("Un parfum ultra-sucré émane du sirop qui couvre {B_EFF_NAME_WITH_PREFIX2} !"),
    [STRINGID_DIMENSIONSWERETWISTED]                = COMPOUND_STRING("Les dimensions sont distordues !"),
    [STRINGID_BIZARREARENACREATED]                  = COMPOUND_STRING("Une zone étrange annule l'effet des objets tenus !"),
    [STRINGID_BIZARREAREACREATED]                   = COMPOUND_STRING("Une zone étrange inverse la Défense et la Défense Spéciale !"),
    [STRINGID_TIDYINGUPCOMPLETE]                    = COMPOUND_STRING("Grand ménage terminé !"),
    [STRINGID_PKMNTERASTALLIZEDINTO]                = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} se téracristallise en type {B_BUFF1} !"), // Does not exist, meant to mimic form change strings
    [STRINGID_BOOSTERENERGYACTIVATES]               = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX} utilise {B_LAST_ITEM} pour activer {B_SCR_ABILITY} !"),
    [STRINGID_FOGCREPTUP]                           = COMPOUND_STRING("Un brouillard épais comme de la soupe se lève !"),
    [STRINGID_FOGISDEEP]                            = COMPOUND_STRING("Le brouillard est épais…"),
    [STRINGID_FOGLIFTED]                            = COMPOUND_STRING("Le brouillard se dissipe."),
    [STRINGID_PKMNMADESHELLGLEAM]                   = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX} fait briller sa carapace ! Les affinités de types sont faussées !"),
    [STRINGID_FICKLEBEAMDOUBLED]                    = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} met toute sa puissance dans cette attaque !"),
    [STRINGID_COMMANDERACTIVATES]                   = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX} est avalé par {B_BUFF1} et le commande de l'intérieur !"),
    [STRINGID_POKEFLUTECATCHY]                      = COMPOUND_STRING("{B_PLAYER_NAME} joue de {B_LAST_ITEM}.\pC'est un air entraînant !"),
    [STRINGID_POKEFLUTE]                            = COMPOUND_STRING("{B_PLAYER_NAME} joue de {B_LAST_ITEM}."),
    [STRINGID_MONHEARINGFLUTEAWOKE]                 = COMPOUND_STRING("Les Pokémon qui entendent la flûte se réveillent !"),
    [STRINGID_SUNLIGHTISHARSH]                      = COMPOUND_STRING("Le soleil tape fort !"),
    [STRINGID_ITISHAILING]                          = COMPOUND_STRING("Il grêle !"),
    [STRINGID_ITISSNOWING]                          = COMPOUND_STRING("Il neige !"),
    [STRINGID_ISCOVEREDWITHGRASS]                   = COMPOUND_STRING("Le terrain est couvert d'herbe !"),
    [STRINGID_MISTSWIRLSAROUND]                     = COMPOUND_STRING("La brume enveloppe le terrain !"),
    [STRINGID_ELECTRICCURRENTISRUNNING]             = COMPOUND_STRING("Un courant électrique parcourt le terrain !"),
    [STRINGID_SEEMSWEIRD]                           = COMPOUND_STRING("Le terrain semble bizarre !"),
    [STRINGID_WAGGLINGAFINGER]                      = COMPOUND_STRING("Métronome déclenche {B_CURRENT_MOVE} !"),
    [STRINGID_BLOCKEDBYSLEEPCLAUSE]                 = COMPOUND_STRING("La Clause Sommeil garde {B_DEF_NAME_WITH_PREFIX2} éveillé !"),
    [STRINGID_SUPEREFFECTIVETWOFOES]                = COMPOUND_STRING("C'est super efficace sur {B_DEF_NAME_WITH_PREFIX2} et {B_DEF_PARTNER_NAME} !"),
    [STRINGID_NOTVERYEFFECTIVETWOFOES]              = COMPOUND_STRING("Ce n'est pas très efficace sur {B_DEF_NAME_WITH_PREFIX2} et {B_DEF_PARTNER_NAME}."),
    [STRINGID_ITDOESNTAFFECTTWOFOES]                = COMPOUND_STRING("Ça n'affecte pas {B_DEF_NAME_WITH_PREFIX2} et {B_DEF_PARTNER_NAME}…"),
    [STRINGID_SENDCAUGHTMONPARTYORBOX]              = COMPOUND_STRING("Ajouter {B_DEF_NAME} à ton équipe ?"),
    [STRINGID_PKMNSENTTOPCAFTERCATCH]               = gText_PkmnSentToPCAfterCatch,
    [STRINGID_PKMNDYNAMAXED]                        = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} devient gigantesque sous sa forme Dynamax !"),
    [STRINGID_PKMNGIGANTAMAXED]                     = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} devient gigantesque sous sa forme Gigamax !"),
    [STRINGID_TIMETODYNAMAX]                        = COMPOUND_STRING("C'est l'heure du Dynamax !"),
    [STRINGID_TIMETOGIGANTAMAX]                     = COMPOUND_STRING("C'est l'heure du Gigamax !"),
    [STRINGID_QUESTIONFORFEITBATTLE]                = COMPOUND_STRING("Abandonner ce combat ? Abandonner revient à perdre."),
    [STRINGID_POWERCONSTRUCTPRESENCEOFMANY]         = COMPOUND_STRING("Tu sens la présence d'une multitude !"),
    [STRINGID_POWERCONSTRUCTTRANSFORM]              = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} prend sa Forme Parfaite !"),
    [STRINGID_ABILITYSHIELDPROTECTS]                = COMPOUND_STRING("Le talent de {B_ATK_NAME_WITH_PREFIX2} est protégé par {B_LAST_ITEM} !"),
    [STRINGID_MONTOOSCAREDTOMOVE]                   = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} est trop terrifié pour bouger !"),
    [STRINGID_GHOSTGETOUTGETOUT]                    = COMPOUND_STRING("SPECTRE : Va-t'en…… Va-t'en……"),
    [STRINGID_SILPHSCOPEUNVEILED]                   = COMPOUND_STRING("Le Scope Sylphe révèle l'identité\ndu SPECTRE !"),
    [STRINGID_GHOSTWASMAROWAK]                      = COMPOUND_STRING("Le SPECTRE était Ossatueur !\p"),
    [STRINGID_TRAINER1MON1COMEBACK]                 = COMPOUND_STRING("{B_TRAINER1_NAME} : {B_OPPONENT_MON1_NAME}, reviens !"),
    [STRINGID_THREWROCK]                            = COMPOUND_STRING("{B_PLAYER_NAME} lance un CAILLOU\nsur {B_OPPONENT_MON1_NAME} !"),
    [STRINGID_THREWBAIT]                            = COMPOUND_STRING("{B_PLAYER_NAME} lance un APPÂT\nà {B_OPPONENT_MON1_NAME} !"),
    [STRINGID_PKMNANGRY]                            = COMPOUND_STRING("{B_OPPONENT_MON1_NAME} est en colère !"),
    [STRINGID_PKMNEATING]                           = COMPOUND_STRING("{B_OPPONENT_MON1_NAME} mange !"),
    [STRINGID_PKMNDISGUISEWASBUSTED]                = COMPOUND_STRING("Le déguisement de {B_SCR_NAME_WITH_PREFIX2} tombe !"),
    [STRINGID_ZENMODETRIGGERED]                     = COMPOUND_STRING("{B_SCR_ABILITY} s'active !"),
    [STRINGID_ZENMODEENDED]                         = COMPOUND_STRING("{B_SCR_ABILITY} prend fin !"),
    [STRINGID_SCRCUREDPARALYSIS]                    = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX} n'est plus paralysé !"),
    [STRINGID_SCRCUREDPOISON]                       = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX} n'est plus empoisonné !"),
    [STRINGID_SCRCUREDBURN]                         = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX} n'est plus brûlé !"),
    [STRINGID_SCRCUREDSLEEP]                        = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX} se réveille !"),
    [STRINGID_SCRCUREDCONFUSION]                    = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX} n'est plus confus !"),
    [STRINGID_PARTYCUREDPARALYSIS]                  = COMPOUND_STRING("{B_BUFF1} n'est plus paralysé !"),
    [STRINGID_PARTYCUREDPOISON]                     = COMPOUND_STRING("{B_BUFF1} n'est plus empoisonné !"),
    [STRINGID_PARTYCUREDBURN]                       = COMPOUND_STRING("{B_BUFF1} n'est plus brûlé !"),
    [STRINGID_PARTYCUREDSLEEP]                      = COMPOUND_STRING("{B_BUFF1} se réveille !"),
    [STRINGID_PARTYCUREDFREEZE]                     = COMPOUND_STRING("{B_BUFF1} dégèle !"),
    [STRINGID_PARTYCUREDFROSTBITE]                  = COMPOUND_STRING("Les engelures de {B_BUFF1} sont soignées !"),
    [STRINGID_PKMNATKNOTLOWERED]                    = COMPOUND_STRING("L'Attaque de {B_SCR_NAME_WITH_PREFIX2} ne baisse pas !"),
    [STRINGID_VICTORYCATCH]                         = COMPOUND_STRING("{B_DEF_NAME} est affaibli !\nLance une Poké Ball !"),
    [STRINGID_CANTUSEMOVE]                          = COMPOUND_STRING("Impossible d'utiliser cette capacité !\p"),
    [STRINGID_REFLECTWOREOFF]                       = COMPOUND_STRING("Protection de {B_DEF_TEAM2} prend fin !"),
    [STRINGID_LIGHTSCREENWOREOFF]                   = COMPOUND_STRING("Mur Lumière de {B_DEF_TEAM2} prend fin !"),
    [STRINGID_AURORAVEILWOREOFF]                    = COMPOUND_STRING("Voile Aurore de {B_DEF_TEAM2} prend fin !"),
    [STRINGID_MOSTLYINEFFECTIVE]                    = COMPOUND_STRING("C'est quasiment inefficace…"),
    [STRINGID_EXTREMELYEFFECTIVE]                   = COMPOUND_STRING("C'est extrêmement efficace !"),
    [STRINGID_NOTVERYEFFECTIVEONDEF]                = COMPOUND_STRING("Ce n'est pas très efficace sur {B_DEF_NAME_WITH_PREFIX2}."),
    [STRINGID_SUPEREFFECTIVEONDEF]                  = COMPOUND_STRING("C'est super efficace sur {B_DEF_NAME_WITH_PREFIX2} !"),
    [STRINGID_MOSTLYINEFFECTIVEONDEF]               = COMPOUND_STRING("C'est quasiment inefficace sur {B_DEF_NAME_WITH_PREFIX2}."),
    [STRINGID_EXTREMELYEFFECTIVEONDEF]              = COMPOUND_STRING("C'est extrêmement efficace sur {B_DEF_NAME_WITH_PREFIX2} !"),
    [STRINGID_EXTREMELYEFFECTIVETWOFOES]            = COMPOUND_STRING("C'est extrêmement efficace sur {B_DEF_NAME_WITH_PREFIX2} et {B_DEF_PARTNER_NAME} !"),
    [STRINGID_MOSTLYINEFFECTIVETWOFOES]             = COMPOUND_STRING("C'est quasiment inefficace sur {B_DEF_NAME_WITH_PREFIX2} et {B_DEF_PARTNER_NAME}."),
    [STRINGID_CRITICALHITONDEF]                     = COMPOUND_STRING("Coup critique sur {B_DEF_NAME_WITH_PREFIX2} !"),
    [STRINGID_S]                                    = COMPOUND_STRING("s"),
    [STRINGID_LOSTSOMEOFITSHP]                      = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} perd une partie de ses PV !"),
    [STRINGID_BELCHCANTUSE]                         = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} n'a mangé aucune Baie, il ne peut pas roter !\p"),
};

const u16 gTrainerUsedItemStringIds[] =
{
    STRINGID_PLAYERUSEDITEM, STRINGID_TRAINER1USEDITEM
};

const u16 gZEffectStringIds[] =
{
    [B_MSG_Z_RESET_STATS] = STRINGID_ZMOVERESETSSTATS,
    [B_MSG_Z_ALL_STATS_UP]= STRINGID_ZMOVEALLSTATSUP,
    [B_MSG_Z_BOOST_CRITS] = STRINGID_ZMOVEZBOOSTCRIT,
    [B_MSG_Z_FOLLOW_ME]   = STRINGID_PKMNCENTERATTENTION,
    [B_MSG_Z_RECOVER_HP]  = STRINGID_ZMOVERESTOREHP,
    [B_MSG_Z_STAT_UP]     = STRINGID_ZMOVESTATUP,
    [B_MSG_Z_HP_TRAP]     = STRINGID_ZMOVEHPTRAP,
};

const u16 gMentalHerbCureStringIds[] =
{
    [B_MSG_MENTALHERBCURE_INFATUATION] = STRINGID_ATKGOTOVERINFATUATION,
    [B_MSG_MENTALHERBCURE_TORMENT]     = STRINGID_TORMENTEDNOMORE,
    [B_MSG_MENTALHERBCURE_DISABLE]     = STRINGID_PKMNMOVEDISABLEDNOMORE,
    [B_MSG_MENTALHERBCURE_HEALBLOCK]   = STRINGID_HEALBLOCKEDNOMORE,
    [B_MSG_MENTALHERBCURE_ENCORE]      = STRINGID_PKMNENCOREENDED,
    [B_MSG_MENTALHERBCURE_TAUNT]       = STRINGID_PKMNSHOOKOFFTHETAUNT,
};

const u16 gStartingStatusStringIds[B_MSG_STARTING_STATUS_COUNT] =
{
    [B_MSG_TERRAIN_SET_MISTY]    = STRINGID_TERRAINBECOMESMISTY,
    [B_MSG_TERRAIN_SET_ELECTRIC] = STRINGID_TERRAINBECOMESELECTRIC,
    [B_MSG_TERRAIN_SET_PSYCHIC]  = STRINGID_TERRAINBECOMESPSYCHIC,
    [B_MSG_TERRAIN_SET_GRASSY]   = STRINGID_TERRAINBECOMESGRASSY,
    [B_MSG_SET_TRICK_ROOM]       = STRINGID_DIMENSIONSWERETWISTED,
    [B_MSG_SET_MAGIC_ROOM]       = STRINGID_BIZARREARENACREATED,
    [B_MSG_SET_WONDER_ROOM]      = STRINGID_BIZARREAREACREATED,
    [B_MSG_SET_TAILWIND]         = STRINGID_TAILWINDBLEW,
    [B_MSG_SET_RAINBOW]          = STRINGID_ARAINBOWAPPEAREDONSIDE,
    [B_MSG_SET_SEA_OF_FIRE]      = STRINGID_SEAOFFIREENVELOPEDSIDE,
    [B_MSG_SET_SWAMP]            = STRINGID_SWAMPENVELOPEDSIDE,
    [B_MSG_SET_SPIKES]           = STRINGID_SPIKESSCATTERED,
    [B_MSG_SET_POISON_SPIKES]    = STRINGID_POISONSPIKESSCATTERED,
    [B_MSG_SET_STICKY_WEB]       = STRINGID_STICKYWEBUSED,
    [B_MSG_SET_STEALTH_ROCK]     = STRINGID_POINTEDSTONESFLOAT,
    [B_MSG_SET_SHARP_STEEL]      = STRINGID_SHARPSTEELFLOATS,
};

const u16 gTerrainStringIds[B_MSG_TERRAIN_COUNT] =
{
    [B_MSG_TERRAIN_SET_MISTY] = STRINGID_TERRAINBECOMESMISTY,
    [B_MSG_TERRAIN_SET_ELECTRIC] = STRINGID_TERRAINBECOMESELECTRIC,
    [B_MSG_TERRAIN_SET_PSYCHIC] = STRINGID_TERRAINBECOMESPSYCHIC,
    [B_MSG_TERRAIN_SET_GRASSY] = STRINGID_TERRAINBECOMESGRASSY,
    [B_MSG_TERRAIN_END_MISTY] = STRINGID_MISTYTERRAINENDS,
    [B_MSG_TERRAIN_END_ELECTRIC] = STRINGID_ELECTRICTERRAINENDS,
    [B_MSG_TERRAIN_END_PSYCHIC] = STRINGID_PSYCHICTERRAINENDS,
    [B_MSG_TERRAIN_END_GRASSY] = STRINGID_GRASSYTERRAINENDS,
};

const u16 gTerrainPreventsStringIds[] =
{
    [B_MSG_TERRAINPREVENTS_MISTY]    = STRINGID_MISTYTERRAINPREVENTS,
    [B_MSG_TERRAINPREVENTS_ELECTRIC] = STRINGID_ELECTRICTERRAINPREVENTS,
    [B_MSG_TERRAINPREVENTS_PSYCHIC]  = STRINGID_PSYCHICTERRAINPREVENTS
};

const u16 gHealingWishStringIds[] =
{
    STRINGID_HEALINGWISHCAMETRUE,
    STRINGID_LUNARDANCECAMETRUE
};

const u16 gDmgHazardsStringIds[] =
{
    [B_MSG_PKMNHURTBYSPIKES]   = STRINGID_PKMNHURTBYSPIKES,
    [B_MSG_STEALTHROCKDMG]     = STRINGID_STEALTHROCKDMG,
    [B_MSG_SHARPSTEELDMG]      = STRINGID_SHARPSTEELDMG,
    [B_MSG_POINTEDSTONESFLOAT] = STRINGID_POINTEDSTONESFLOAT,
    [B_MSG_SPIKESSCATTERED]    = STRINGID_SPIKESSCATTERED,
    [B_MSG_SHARPSTEELFLOATS]   = STRINGID_SHARPSTEELFLOATS,
};

const u16 gSwitchInAbilityStringIds[] =
{
    [B_MSG_SWITCHIN_MOLDBREAKER] = STRINGID_MOLDBREAKERENTERS,
    [B_MSG_SWITCHIN_TERAVOLT] = STRINGID_TERAVOLTENTERS,
    [B_MSG_SWITCHIN_TURBOBLAZE] = STRINGID_TURBOBLAZEENTERS,
    [B_MSG_SWITCHIN_SLOWSTART] = STRINGID_SLOWSTARTENTERS,
    [B_MSG_SWITCHIN_UNNERVE] = STRINGID_UNNERVEENTERS,
    [B_MSG_SWITCHIN_ANTICIPATION] = STRINGID_ANTICIPATIONACTIVATES,
    [B_MSG_SWITCHIN_FOREWARN] = STRINGID_FOREWARNACTIVATES,
    [B_MSG_SWITCHIN_PRESSURE] = STRINGID_PRESSUREENTERS,
    [B_MSG_SWITCHIN_DARKAURA] = STRINGID_DARKAURAENTERS,
    [B_MSG_SWITCHIN_FAIRYAURA] = STRINGID_FAIRYAURAENTERS,
    [B_MSG_SWITCHIN_AURABREAK] = STRINGID_AURABREAKENTERS,
    [B_MSG_SWITCHIN_COMATOSE] = STRINGID_COMATOSEENTERS,
    [B_MSG_SWITCHIN_SCREENCLEANER] = STRINGID_SCREENCLEANERENTERS,
    [B_MSG_SWITCHIN_ASONE] = STRINGID_ASONEENTERS,
    [B_MSG_SWITCHIN_CURIOUS_MEDICINE] = STRINGID_CURIOUSMEDICINEENTERS,
    [B_MSG_SWITCHIN_PASTEL_VEIL] = STRINGID_PKMNHEALEDPOISON,
    [B_MSG_SWITCHIN_NEUTRALIZING_GAS] = STRINGID_NEUTRALIZINGGASENTERS,
};

const u16 gNoEscapeStringIds[] =
{
    [B_MSG_CANT_ESCAPE]          = STRINGID_CANTESCAPE,
    [B_MSG_DONT_LEAVE_BIRCH]     = STRINGID_DONTLEAVEBIRCH,
    [B_MSG_PREVENTS_ESCAPE]      = STRINGID_PREVENTSESCAPE,
    [B_MSG_CANT_ESCAPE_2]        = STRINGID_CANTESCAPE2,
    [B_MSG_ATTACKER_CANT_ESCAPE] = STRINGID_ATTACKERCANTESCAPE
};

const u16 gMoveWeatherChangeStringIds[] =
{
    [B_MSG_STARTED_RAIN]      = STRINGID_STARTEDTORAIN,
    [B_MSG_STARTED_DOWNPOUR]  = STRINGID_DOWNPOURSTARTED, // Unused
    [B_MSG_WEATHER_FAILED]    = STRINGID_BUTITFAILED,
    [B_MSG_STARTED_SANDSTORM] = STRINGID_SANDSTORMBREWED,
    [B_MSG_STARTED_SUNLIGHT]  = STRINGID_SUNLIGHTGOTBRIGHT,
    [B_MSG_STARTED_HAIL]      = STRINGID_STARTEDHAIL,
    [B_MSG_STARTED_SNOW]      = STRINGID_STARTEDSNOW,
    [B_MSG_STARTED_FOG]       = STRINGID_FOGCREPTUP, // Unused, can use for custom moves that set fog
};

const u16 gAbilityWeatherChangeStringId[] =
{
    [B_MSG_STARTED_DRIZZLE]        = STRINGID_STARTEDTORAIN,
    [B_MSG_STARTED_SAND_STREAM]    = STRINGID_SANDSTORMBREWED,
    [B_MSG_STARTED_DROUGHT]        = STRINGID_SUNLIGHTGOTBRIGHT,
    [B_MSG_STARTED_HAIL_WARNING]   = STRINGID_STARTEDHAIL,
    [B_MSG_STARTED_SNOW_WARNING]   = STRINGID_STARTEDSNOW,
    [B_MSG_STARTED_DESOLATE_LAND]  = STRINGID_EXTREMELYHARSHSUNLIGHT,
    [B_MSG_STARTED_PRIMORDIAL_SEA] = STRINGID_HEAVYRAIN,
    [B_MSG_STARTED_STRONG_WINDS]   = STRINGID_MYSTERIOUSAIRCURRENT,
};

const u16 gWeatherEndsStringIds[B_MSG_WEATHER_END_COUNT] =
{
    [B_MSG_WEATHER_END_RAIN]                       = STRINGID_RAINSTOPPED,
    [B_MSG_WEATHER_END_SUN]                        = STRINGID_SUNLIGHTFADED,
    [B_MSG_WEATHER_END_SANDSTORM]                  = STRINGID_SANDSTORMSUBSIDED,
    [B_MSG_WEATHER_END_HAIL]                       = STRINGID_HAILSTOPPED,
    [B_MSG_WEATHER_END_SNOW]                       = STRINGID_SNOWSTOPPED,
    [B_MSG_WEATHER_END_FOG]                        = STRINGID_FOGLIFTED,
    [B_MSG_WEATHER_END_EXTREMELY_HARSH_SUNLIGHT]   = STRINGID_EXTREMESUNLIGHTFADED,
    [B_MSG_WEATHER_END_HEAVY_RAIN]                 = STRINGID_HEAVYRAINLIFTED,
    [B_MSG_WEATHER_END_STRONG_WINDS]               = STRINGID_STRONGWINDSDISSIPATED,
};

const u16 gWeatherTurnStringIds[] =
{
    [B_MSG_WEATHER_TURN_RAIN]         = STRINGID_RAINCONTINUES,
    [B_MSG_WEATHER_TURN_DOWNPOUR]     = STRINGID_DOWNPOURCONTINUES,
    [B_MSG_WEATHER_TURN_SUN]          = STRINGID_SUNLIGHTSTRONG,
    [B_MSG_WEATHER_TURN_SANDSTORM]    = STRINGID_SANDSTORMRAGES,
    [B_MSG_WEATHER_TURN_HAIL]         = STRINGID_HAILCONTINUES,
    [B_MSG_WEATHER_TURN_SNOW]         = STRINGID_SNOWCONTINUES,
    [B_MSG_WEATHER_TURN_FOG]          = STRINGID_FOGISDEEP,
    [B_MSG_WEATHER_TURN_STRONG_WINDS] = STRINGID_MYSTERIOUSAIRCURRENTBLOWSON,
};

const u16 gSandStormHailDmgStringIds[] =
{
    [B_MSG_SANDSTORM] = STRINGID_PKMNBUFFETEDBYSANDSTORM,
    [B_MSG_HAIL]      = STRINGID_PKMNPELTEDBYHAIL
};

const u16 gProtectLikeUsedStringIds[] =
{
    [B_MSG_PROTECTED_ITSELF] = STRINGID_PKMNPROTECTEDITSELF2,
    [B_MSG_BRACED_ITSELF]    = STRINGID_PKMNBRACEDITSELF,
    [B_MSG_PROTECTED_TEAM]   = STRINGID_PROTECTEDTEAM,
};

const u16 gBrokeProtectionStringIds[] =
{
    [B_MSG_FEINT]                 = STRINGID_FELLFORFEINT,
    [B_MSG_BROKE_THROUGH_PROTECT] = STRINGID_BROKETHROUGHPROTECTION,
};

const u16 gReflectLightScreenSafeguardStringIds[] =
{
    [B_MSG_SIDE_STATUS_FAILED]     = STRINGID_BUTITFAILED,
    [B_MSG_SET_REFLECT_SINGLE]     = STRINGID_PKMNRAISEDDEF,
    [B_MSG_SET_REFLECT_DOUBLE]     = STRINGID_PKMNRAISEDDEF,
    [B_MSG_SET_LIGHTSCREEN_SINGLE] = STRINGID_PKMNRAISEDSPDEF,
    [B_MSG_SET_LIGHTSCREEN_DOUBLE] = STRINGID_PKMNRAISEDSPDEF,
    [B_MSG_SET_SAFEGUARD]          = STRINGID_PKMNCOVEREDBYVEIL,
    [B_MSG_SET_AURORA_VEIL]        = STRINGID_PKMNAURORAVEIL,
};

const u16 gLeechSeedStringIds[] =
{
    [B_MSG_LEECH_SEED_SET]   = STRINGID_PKMNSEEDED,
    [B_MSG_LEECH_SEED_MISS]  = STRINGID_PKMNAVOIDEDATTACK,
    [B_MSG_LEECH_SEED_FAIL]  = STRINGID_ITDOESNTAFFECT,
    [B_MSG_LEECH_SEED_DRAIN] = STRINGID_PKMNSAPPEDBYLEECHSEED,
    [B_MSG_LEECH_SEED_OOZE]  = STRINGID_ITSUCKEDLIQUIDOOZE,
};

const u16 gRestUsedStringIds[] =
{
    [B_MSG_REST]          = STRINGID_PKMNWENTTOSLEEP,
    [B_MSG_REST_STATUSED] = STRINGID_PKMNSLEPTHEALTHY
};

const u16 gUproarOverTurnStringIds[] =
{
    [B_MSG_UPROAR_CONTINUES] = STRINGID_PKMNMAKINGUPROAR,
    [B_MSG_UPROAR_ENDS]      = STRINGID_PKMNCALMEDDOWN
};

const u16 gWokeUpStringIds[] =
{
    [B_MSG_WOKE_UP]        = STRINGID_PKMNWOKEUP,
    [B_MSG_WOKE_UP_UPROAR] = STRINGID_PKMNWOKEUPINUPROAR
};

const u16 gUproarAwakeStringIds[] =
{
    [B_MSG_CANT_SLEEP_UPROAR]  = STRINGID_PKMNCANTSLEEPINUPROAR2,
    [B_MSG_UPROAR_KEPT_AWAKE]  = STRINGID_UPROARKEPTPKMNAWAKE,
};

const u16 gStatUpStringIds[] =
{
    [B_MSG_STAT_CHANGED]            = STRINGID_STATROSE,
    [B_MSG_STAT_WONT_CHANGE]        = STRINGID_STATSWONTINCREASE,
    [B_MSG_STAT_MAXED]              = STRINGID_STATWASMAXEDOUT,
    [B_MSG_STAT_CHANGE_EMPTY]       = STRINGID_EMPTYSTRING3,
    [B_MSG_STAT_CHANGED_ITEM]       = STRINGID_USINGITEMSTATOFPKMNROSE,
    [B_MSG_STAT_CHANGED_BELLY_DRUM] = STRINGID_PKMNCUTHPMAXEDATTACK,
    [B_MSG_USED_DIRE_HIT]           = STRINGID_PKMNUSEDXTOGETPUMPED,
};

// Mostly redundant, combine with above in a future pr
const u16 gStatDownStringIds[] =
{
    [B_MSG_STAT_CHANGED]            = STRINGID_STATFELL,
    [B_MSG_STAT_WONT_CHANGE]        = STRINGID_STATSWONTDECREASE,
    [B_MSG_STAT_CHANGE_EMPTY]       = STRINGID_EMPTYSTRING3,
    [B_MSG_STAT_CHANGED_ITEM]       = STRINGID_USINGITEMSTATOFPKMNFELL,
    [B_MSG_STAT_CHANGED_BELLY_DRUM] = STRINGID_PKMNCUTHPMAXEDATTACK, // Message for contrary is still printed
};

// Index copied from move's index in sTrappingMoves
const u16 gWrappedStringIds[NUM_TRAPPING_MOVES] =
{
    [B_MSG_WRAPPED_BIND]        = STRINGID_PKMNSQUEEZEDBYBIND,     // MOVE_BIND
    [B_MSG_WRAPPED_WRAP]        = STRINGID_PKMNWRAPPEDBY,          // MOVE_WRAP
    [B_MSG_WRAPPED_FIRE_SPIN]   = STRINGID_PKMNTRAPPEDINVORTEX,    // MOVE_FIRE_SPIN
    [B_MSG_WRAPPED_CLAMP]       = STRINGID_PKMNCLAMPED,            // MOVE_CLAMP
    [B_MSG_WRAPPED_WHIRLPOOL]   = STRINGID_PKMNTRAPPEDINVORTEX,    // MOVE_WHIRLPOOL
    [B_MSG_WRAPPED_SAND_TOMB]   = STRINGID_PKMNTRAPPEDBYSANDTOMB,  // MOVE_SAND_TOMB
    [B_MSG_WRAPPED_MAGMA_STORM] = STRINGID_TRAPPEDBYSWIRLINGMAGMA, // MOVE_MAGMA_STORM
    [B_MSG_WRAPPED_INFESTATION] = STRINGID_INFESTATION,            // MOVE_INFESTATION
    [B_MSG_WRAPPED_SNAP_TRAP]   = STRINGID_PKMNINSNAPTRAP,         // MOVE_SNAP_TRAP
    [B_MSG_WRAPPED_THUNDER_CAGE]= STRINGID_THUNDERCAGETRAPPED,     // MOVE_THUNDER_CAGE
};

const u16 gMistUsedStringIds[] =
{
    [B_MSG_SET_MIST]    = STRINGID_PKMNSHROUDEDINMIST,
    [B_MSG_MIST_FAILED] = STRINGID_BUTITFAILED
};

const u16 gFocusEnergyUsedStringIds[] =
{
    [B_MSG_GETTING_PUMPED]      = STRINGID_PKMNGETTINGPUMPED,
    [B_MSG_FOCUS_ENERGY_FAILED] = STRINGID_BUTITFAILED
};

const u16 gTransformUsedStringIds[] =
{
    [B_MSG_TRANSFORMED]      = STRINGID_PKMNTRANSFORMEDINTO,
    [B_MSG_TRANSFORM_FAILED] = STRINGID_BUTITFAILED
};

const u16 gSubstituteUsedStringIds[] =
{
    [B_MSG_SET_SUBSTITUTE]    = STRINGID_PKMNMADESUBSTITUTE,
    [B_MSG_SUBSTITUTE_FAILED] = STRINGID_TOOWEAKFORSUBSTITUTE
};

const u16 gGotPoisonedStringIds[] =
{
    [B_MSG_STATUSED]            = STRINGID_PKMNWASPOISONED,
    [B_MSG_STATUSED_BY_ABILITY] = STRINGID_PKMNPOISONEDBY
};

const u16 gGotParalyzedStringIds[] =
{
    [B_MSG_STATUSED]            = STRINGID_PKMNWASPARALYZED,
    [B_MSG_STATUSED_BY_ABILITY] = STRINGID_PKMNWASPARALYZEDBY
};

const u16 gFellAsleepStringIds[] =
{
    [B_MSG_STATUSED]            = STRINGID_PKMNFELLASLEEP,
    [B_MSG_STATUSED_BY_ABILITY] = STRINGID_PKMNMADESLEEP,
};

const u16 gGotBurnedStringIds[] =
{
    [B_MSG_STATUSED]            = STRINGID_PKMNWASBURNED,
    [B_MSG_STATUSED_BY_ABILITY] = STRINGID_PKMNBURNEDBY
};

const u16 gGotFrostbiteStringIds[] =
{
    [B_MSG_STATUSED]            = STRINGID_PKMNGOTFROSTBITE,
    [B_MSG_STATUSED_BY_ABILITY] = STRINGID_PKMNGOTFROSTBITE,
};

const u16 gFrostbiteHealedStringIds[] =
{
    [B_MSG_FROSTBITE_HEALED]         = STRINGID_PKMNFROSTBITEHEALED,
    [B_MSG_FROSTBITE_HEALED_BY_MOVE] = STRINGID_PKMNFROSTBITEHEALEDBY
};

const u16 gGotFrozenStringIds[] =
{
    [B_MSG_STATUSED]            = STRINGID_PKMNWASFROZEN,
    [B_MSG_STATUSED_BY_ABILITY] = STRINGID_PKMNFROZENBY
};

const u16 gGotDefrostedStringIds[] =
{
    [B_MSG_DEFROSTED]         = STRINGID_PKMNWASDEFROSTED,
    [B_MSG_DEFROSTED_BY_MOVE] = STRINGID_PKMNWASDEFROSTEDBY
};

const u16 gKOFailedStringIds[] =
{
    [B_MSG_KO_MISS]       = STRINGID_PKMNAVOIDEDATTACK,
    [B_MSG_KO_UNAFFECTED] = STRINGID_PKMNUNAFFECTED
};

const u16 gAttractUsedStringIds[] =
{
    [B_MSG_STATUSED]            = STRINGID_PKMNFELLINLOVE,
    [B_MSG_STATUSED_BY_ABILITY] = STRINGID_PKMNSXINFATUATEDY
};

const u16 gAbsorbDrainStringIds[] =
{
    [B_MSG_ABSORB]      = STRINGID_PKMNENERGYDRAINED,
    [B_MSG_ABSORB_OOZE] = STRINGID_ITSUCKEDLIQUIDOOZE
};

const u16 gSportsUsedStringIds[] =
{
    [B_MSG_WEAKEN_ELECTRIC] = STRINGID_ELECTRICITYWEAKENED,
    [B_MSG_WEAKEN_FIRE]     = STRINGID_FIREWEAKENED
};

const u16 gPartyStatusHealStringIds[] =
{
    [B_MSG_BELL]                     = STRINGID_BELLCHIMED,
    [B_MSG_BELL_SOUNDPROOF_ATTACKER] = STRINGID_BELLCHIMED,
    [B_MSG_BELL_SOUNDPROOF_PARTNER]  = STRINGID_BELLCHIMED,
    [B_MSG_BELL_BOTH_SOUNDPROOF]     = STRINGID_BELLCHIMED,
    [B_MSG_SOOTHING_AROMA]           = STRINGID_SOOTHINGAROMA
};

const u16 gFutureMoveUsedStringIds[] =
{
    [B_MSG_FUTURE_SIGHT] = STRINGID_PKMNFORESAWATTACK,
    [B_MSG_DOOM_DESIRE]  = STRINGID_PKMNCHOSEXASDESTINY
};

const u16 gBallEscapeStringIds[] =
{
    [BALL_NO_SHAKES]     = STRINGID_PKMNBROKEFREE,
    [BALL_1_SHAKE]       = STRINGID_ITAPPEAREDCAUGHT,
    [BALL_2_SHAKES]      = STRINGID_AARGHALMOSTHADIT,
    [BALL_3_SHAKES_FAIL] = STRINGID_SHOOTSOCLOSE
};

// Overworld weathers that don't have an associated battle weather default to "It is raining."
const u16 gWeatherStartsStringIds[] =
{
    [WEATHER_NONE]               = STRINGID_ITISRAINING,
    [WEATHER_SUNNY_CLOUDS]       = STRINGID_ITISRAINING,
    [WEATHER_SUNNY]              = STRINGID_ITISRAINING,
    [WEATHER_RAIN]               = STRINGID_ITISRAINING,
    [WEATHER_SNOW]               = (B_OVERWORLD_SNOW >= GEN_9 ? STRINGID_ITISSNOWING : STRINGID_ITISHAILING),
    [WEATHER_RAIN_THUNDERSTORM]  = STRINGID_ITISRAINING,
    [WEATHER_FOG_HORIZONTAL]     = STRINGID_FOGISDEEP,
    [WEATHER_VOLCANIC_ASH]       = STRINGID_ITISRAINING,
    [WEATHER_SANDSTORM]          = STRINGID_SANDSTORMISRAGING,
    [WEATHER_FOG_DIAGONAL]       = STRINGID_FOGISDEEP,
    [WEATHER_UNDERWATER]         = STRINGID_ITISRAINING,
    [WEATHER_SHADE]              = STRINGID_ITISRAINING,
    [WEATHER_DROUGHT]            = STRINGID_SUNLIGHTISHARSH,
    [WEATHER_DOWNPOUR]           = STRINGID_ITISRAINING,
    [WEATHER_UNDERWATER_BUBBLES] = STRINGID_ITISRAINING,
    [WEATHER_ABNORMAL]           = STRINGID_ITISRAINING
};

const u16 gTerrainStartsStringIds[] =
{
    [B_MSG_TERRAIN_SET_MISTY]    = STRINGID_MISTSWIRLSAROUND,
    [B_MSG_TERRAIN_SET_ELECTRIC] = STRINGID_ELECTRICCURRENTISRUNNING,
    [B_MSG_TERRAIN_SET_PSYCHIC]  = STRINGID_SEEMSWEIRD,
    [B_MSG_TERRAIN_SET_GRASSY]   = STRINGID_ISCOVEREDWITHGRASS,
};

const u16 gPrimalWeatherBlocksStringIds[] =
{
    [B_MSG_PRIMAL_WEATHER_FIZZLED_BY_RAIN]      = STRINGID_MOVEFIZZLEDOUTINTHEHEAVYRAIN,
    [B_MSG_PRIMAL_WEATHER_EVAPORATED_IN_SUN]    = STRINGID_MOVEEVAPORATEDINTHEHARSHSUNLIGHT,
};

const u16 gInobedientStringIds[] =
{
    [B_MSG_LOAFING]            = STRINGID_PKMNLOAFING,
    [B_MSG_WONT_OBEY]          = STRINGID_PKMNWONTOBEY,
    [B_MSG_TURNED_AWAY]        = STRINGID_PKMNTURNEDAWAY,
    [B_MSG_PRETEND_NOT_NOTICE] = STRINGID_PKMNPRETENDNOTNOTICE,
    [B_MSG_INCAPABLE_OF_POWER] = STRINGID_PKMNINCAPABLEOFPOWER
};

const u16 gSafariReactionStringIds[NUM_SAFARI_REACTIONS] =
{
    [B_MSG_MON_WATCHING] = STRINGID_PKMNWATCHINGCAREFULLY,
    [B_MSG_MON_ANGRY]    = STRINGID_PKMNANGRY,
    [B_MSG_MON_EATING]   = STRINGID_PKMNEATING
};

const u16 gSafariGetNearStringIds[] =
{
    [B_MSG_CREPT_CLOSER]    = STRINGID_CREPTCLOSER,
    [B_MSG_CANT_GET_CLOSER] = STRINGID_CANTGETCLOSER
};

const u16 gSafariPokeblockResultStringIds[] =
{
    [B_MSG_MON_CURIOUS]    = STRINGID_PKMNCURIOUSABOUTX,
    [B_MSG_MON_ENTHRALLED] = STRINGID_PKMNENTHRALLEDBYX,
    [B_MSG_MON_IGNORED]    = STRINGID_PKMNIGNOREDX
};

const u16 CureStatusBerryEffectStringID[] =
{
    [B_MSG_CURED_PARALYSIS] = STRINGID_PKMNSITEMCUREDPARALYSIS,
    [B_MSG_CURED_POISON] = STRINGID_PKMNSITEMCUREDPOISON,
    [B_MSG_CURED_BURN] = STRINGID_PKMNSITEMHEALEDBURN,
    [B_MSG_CURED_FREEZE] = STRINGID_PKMNSITEMDEFROSTEDIT,
    [B_MSG_CURED_FROSTBITE] = STRINGID_PKMNSITEMHEALEDFROSTBITE,
    [B_MSG_CURED_SLEEP] = STRINGID_PKMNSITEMWOKEIT,
    [B_MSG_CURED_CONFUSION] = STRINGID_PKMNSITEMSNAPPEDOUT,
};

const u16 gItemSwapStringIds[] =
{
    [B_MSG_ITEM_SWAP_TAKEN] = STRINGID_PKMNOBTAINEDX,
    [B_MSG_ITEM_SWAP_GIVEN] = STRINGID_PKMNOBTAINEDX2,
    [B_MSG_ITEM_SWAP_BOTH]  = STRINGID_PKMNOBTAINEDXYOBTAINEDZ
};

const u16 gFlashFireStringIds[] =
{
    [B_MSG_FLASH_FIRE_BOOST]    = STRINGID_PKMNRAISEDFIREPOWERWITH,
    [B_MSG_FLASH_FIRE_NO_BOOST] = STRINGID_PKMNSXMADEITINEFFECTIVE
};

const u16 gCaughtMonStringIds[] =
{
    [B_MSG_SENT_SOMEONES_PC]   = STRINGID_PKMNTRANSFERREDSOMEONESPC,
    [B_MSG_SENT_LANETTES_PC]   = STRINGID_PKMNTRANSFERREDLANETTESPC,
    [B_MSG_SOMEONES_BOX_FULL]  = STRINGID_PKMNBOXSOMEONESPCFULL,
    [B_MSG_LANETTES_BOX_FULL]  = STRINGID_PKMNBOXLANETTESPCFULL,
    [B_MSG_SWAPPED_INTO_PARTY] = STRINGID_PKMNSENTTOPCAFTERCATCH,
};

const u16 gRoomsStringIds[] =
{
    STRINGID_PKMNTWISTEDDIMENSIONS, STRINGID_TRICKROOMENDS,
    STRINGID_SWAPSDEFANDSPDEFOFALLPOKEMON, STRINGID_WONDERROOMENDS,
    STRINGID_HELDITEMSLOSEEFFECTS, STRINGID_MAGICROOMENDS,
    STRINGID_EMPTYSTRING3
};

const u16 gStatusConditionsStringIds[] =
{
    STRINGID_PKMNWASPOISONED, STRINGID_PKMNBADLYPOISONED, STRINGID_PKMNWASBURNED, STRINGID_PKMNWASPARALYZED, STRINGID_PKMNFELLASLEEP, STRINGID_PKMNGOTFROSTBITE
};

const u16 gDamageNonTypesStartStringIds[] =
{
    [B_MSG_TRAPPED_WITH_VINES]  = STRINGID_TEAMTRAPPEDWITHVINES,
    [B_MSG_CAUGHT_IN_VORTEX]    = STRINGID_TEAMCAUGHTINVORTEX,
    [B_MSG_SURROUNDED_BY_FIRE]  = STRINGID_TEAMSURROUNDEDBYFIRE,
    [B_MSG_SURROUNDED_BY_ROCKS] = STRINGID_TEAMSURROUNDEDBYROCKS,
};

const u16 gDamageNonTypesDmgStringIds[] =
{
    [B_MSG_HURT_BY_VINES]        = STRINGID_PKMNHURTBYVINES,
    [B_MSG_HURT_BY_VORTEX]       = STRINGID_PKMNHURTBYVORTEX,
    [B_MSG_BURNING_UP]           = STRINGID_PKMNBURNINGUP,
    [B_MSG_HURT_BY_ROCKS_THROWN] = STRINGID_PKMNHURTBYROCKSTHROWN,
};

const u16 gRemoveHazardsStringIds[] =
{
    [HAZARDS_SPIKES] = STRINGID_SPIKESDISAPPEAREDFROMTEAM,
    [HAZARDS_STICKY_WEB] = STRINGID_STICKYWEBDISAPPEAREDFROMTEAM,
    [HAZARDS_TOXIC_SPIKES] = STRINGID_TOXICSPIKESDISAPPEAREDFROMTEAM,
    [HAZARDS_STEALTH_ROCK] = STRINGID_STEALTHROCKDISAPPEAREDFROMTEAM,
    [HAZARDS_STEELSURGE] = STRINGID_SHARPSTEELDISAPPEAREDFROMTEAM,
};

const u16 gZenModeStringIds[] =
{
    [B_MSG_ZEN_MODE_TRIGGERED] = STRINGID_ZENMODETRIGGERED,
    [B_MSG_ZEN_MODE_ENDED] = STRINGID_ZENMODEENDED
};

const u16 gCureStatusStringIds[] =
{
    [B_MSG_CURED_PARALYSIS] = STRINGID_SCRCUREDPARALYSIS,
    [B_MSG_CURED_POISON] = STRINGID_SCRCUREDPOISON,
    [B_MSG_CURED_BURN] = STRINGID_SCRCUREDBURN,
    [B_MSG_CURED_SLEEP] = STRINGID_SCRCUREDSLEEP,
    [B_MSG_CURED_FREEZE] = STRINGID_PKMNWASDEFROSTED,
    [B_MSG_CURED_FROSTBITE] = STRINGID_PKMNFROSTBITEHEALED,
    [B_MSG_CURED_CONFUSION] = STRINGID_SCRCUREDCONFUSION,
    [B_MSG_CURED_INFATUATION] = STRINGID_PKMNGOTOVERITSINFATUATION,
    [B_MSG_CURED_TAUNT] = STRINGID_PKMNSHOOKOFFTHETAUNT,
};

const u16 gPartyCureStatusStringIds[] =
{
    [B_MSG_CURED_PARALYSIS] = STRINGID_PARTYCUREDPARALYSIS,
    [B_MSG_CURED_POISON] = STRINGID_PARTYCUREDPOISON,
    [B_MSG_CURED_BURN] = STRINGID_PARTYCUREDBURN,
    [B_MSG_CURED_SLEEP] = STRINGID_PARTYCUREDSLEEP,
    [B_MSG_CURED_FREEZE] = STRINGID_PARTYCUREDFREEZE,
    [B_MSG_CURED_FROSTBITE] = STRINGID_PARTYCUREDFROSTBITE,
    [B_MSG_CURED_CONFUSION] = STRINGID_SCRCUREDCONFUSION,
    [B_MSG_CURED_INFATUATION] = STRINGID_PKMNGOTOVERITSINFATUATION,
    [B_MSG_CURED_TAUNT] = STRINGID_PKMNSHOOKOFFTHETAUNT,
};

const u16 gHurtByStringIds[] =
{
    [B_MSG_HURT] = STRINGID_PKMNWASHURT,
    [B_MSG_HURT_BY_ITEM] = STRINGID_PKMNHURTSWITH,
};

const u16 gBreakScreensStringIds[] =
{
    [B_MSG_BREAK_REFLECT] = STRINGID_REFLECTWOREOFF,
    [B_MSG_BREAK_LIGHT_SCREEN] = STRINGID_LIGHTSCREENWOREOFF,
    [B_MSG_BREAK_AURORA_VEIL] = STRINGID_AURORAVEILWOREOFF,
};

const u8 gText_PkmnIsEvolving[] = _("Quoi ?\n{STR_VAR_1} évolue !");
const u8 gText_CongratsPkmnEvolved[] = _("Félicitations ! Ton {STR_VAR_1}\névolue en {STR_VAR_2} !{WAIT_SE}\p");
const u8 gText_PkmnStoppedEvolving[] = _("Hein ? {STR_VAR_1}\na arrêté d'évoluer !\p");
const u8 gText_EllipsisQuestionMark[] = _("……?\p");
const u8 gText_WhatWillPkmnDo[] = _("Que doit faire\n{B_BUFF1} ?");
const u8 gText_WhatWillPkmnDo2[] = _("Que doit faire\n{B_PLAYER_NAME} ?");
const u8 gText_WhatWillWallyDo[] = _("Que doit faire\nTimmy ?");
const u8 gText_LinkStandby[] = _("{PAUSE 16}Connexion en attente…");
const u8 gText_BattleMenu[] = _("Attaque{CLEAR_TO 56}Sac\nPokémon{CLEAR_TO 56}Fuite");
const u8 gText_SafariZoneMenu[] = _("Ball{CLEAR_TO 56}{POKEBLOCK}\nApprocher{CLEAR_TO 56}Fuite");
const u8 gText_SafariZoneMenuFrlg[] = _("{PALETTE 5}{COLOR_HIGHLIGHT_SHADOW 13 14 15}BALL{CLEAR_TO 56}BAIT\nROCK{CLEAR_TO 56}RUN");
const u8 gText_MoveInterfacePP[] = _("PP ");
const u8 gText_MoveInterfaceType[] = _("TYPE/");
const u8 gText_MoveInterfacePPType[] = _("{PALETTE 5}{BACKGROUND DYNAMIC_COLOR5}{TEXT_COLORS DYNAMIC_COLOR4 DYNAMIC_COLOR6 DYNAMIC_COLOR5}PP\nTYPE/");
const u8 gText_MoveInterfaceDynamicColors[] = _("{PALETTE 5}{BACKGROUND DYNAMIC_COLOR5}{TEXT_COLORS DYNAMIC_COLOR4 DYNAMIC_COLOR6 DYNAMIC_COLOR5}");
const u8 gText_WhichMoveToForget4[] = _("{PALETTE 5}{BACKGROUND DYNAMIC_COLOR5}{TEXT_COLORS DYNAMIC_COLOR4 DYNAMIC_COLOR6 DYNAMIC_COLOR5}Quelle capacité\noublier ?");
const u8 gText_BattleCatchOrNot[] = _("{PALETTE 5}{BACKGROUND DYNAMIC_COLOR5}{TEXT_COLORS DYNAMIC_COLOR4 DYNAMIC_COLOR6 DYNAMIC_COLOR5}Capturer\nLaisser");
const u8 gText_BattleYesNoChoice[] = _("{PALETTE 5}{BACKGROUND DYNAMIC_COLOR5}{TEXT_COLORS DYNAMIC_COLOR4 DYNAMIC_COLOR6 DYNAMIC_COLOR5}Oui\nNon");
const u8 gText_BattleSwitchWhich[] = _("{PALETTE 5}{BACKGROUND DYNAMIC_COLOR5}{TEXT_COLORS DYNAMIC_COLOR4 DYNAMIC_COLOR6 DYNAMIC_COLOR5}Échanger\navec ?");
const u8 gText_BattleSwitchWhich2[] = _("{PALETTE 5}{BACKGROUND DYNAMIC_COLOR5}{TEXT_COLORS DYNAMIC_COLOR4 DYNAMIC_COLOR6 DYNAMIC_COLOR5}");
const u8 gText_BattleSwitchWhich3[] = _("{UP_ARROW}");
const u8 gText_BattleSwitchWhich4[] = _("{ESCAPE 4}");
const u8 gText_BattleSwitchWhich5[] = _("-");
const u8 gText_SafariBalls[] = _("Safari Balls");
const u8 gText_SafariBallLeft[] = _("Restant : $");
const u8 gText_Sleep[] = _("sommeil");
const u8 gText_Poison[] = _("poison");
const u8 gText_Burn[] = _("brûlure");
const u8 gText_Paralysis[] = _("paralysie");
const u8 gText_Ice[] = _("gel");
const u8 gText_Confusion[] = _("confusion");
const u8 gText_Love[] = _("amour");
const u8 gText_SpaceAndSpace[] = _(" et ");
const u8 gText_CommaSpace[] = _(", ");
const u8 gText_Space2[] = _(" ");
const u8 gText_LineBreak[] = _("\l");
const u8 gText_NewLine[] = _("\n");
const u8 gText_Are[] = _("sont");
const u8 gText_Are2[] = _("sont");
const u8 gText_BadEgg[] = _("Œuf Mauvais");
const u8 gText_BattleWallyName[] = _("WALLY");
const u8 gText_Win[] = _("{BACKGROUND TRANSPARENT}{ACCENT TRANSPARENT}Victoire");
const u8 gText_Loss[] = _("{BACKGROUND TRANSPARENT}{ACCENT TRANSPARENT}Défaite");
const u8 gText_Draw[] = _("{BACKGROUND TRANSPARENT}{ACCENT TRANSPARENT}Nul");
static const u8 sText_SpaceIs[] = _(" est");
static const u8 sText_ApostropheS[] = _("'s");
const u8 gText_BattleTourney[] = _("BATTLE TOURNEY");

const u8 *const gRoundsStringTable[DOME_ROUNDS_COUNT] =
{
    [DOME_ROUND1]    = COMPOUND_STRING("Round 1"),
    [DOME_ROUND2]    = COMPOUND_STRING("Round 2"),
    [DOME_SEMIFINAL] = COMPOUND_STRING("Semifinal"),
    [DOME_FINAL]     = COMPOUND_STRING("Final"),
};

const u8 gText_TheGreatNewHope[] = _("Le grand espoir montant !\p");
const u8 gText_WillChampionshipDreamComeTrue[] = _("Le rêve du titre va-t-il se réaliser ?!\p");
const u8 gText_AFormerChampion[] = _("Un ancien Maître !\p");
const u8 gText_ThePreviousChampion[] = _("Le Maître précédent !\p");
const u8 gText_TheUnbeatenChampion[] = _("Le Maître invaincu !\p");
const u8 gText_PlayerMon1Name[] = _("{B_PLAYER_MON1_NAME}");
const u8 gText_Vs[] = _("VS");
const u8 gText_OpponentMon1Name[] = _("{B_OPPONENT_MON1_NAME}");
const u8 gText_Mind[] = _("Esprit");
const u8 gText_Skill[] = _("Technique");
const u8 gText_Body[] = _("Physique");
const u8 gText_Judgment[] = _("{B_BUFF1}{CLEAR 13}Jugement{CLEAR 13}{B_BUFF2}");
static const u8 sText_TwoTrainersSentPkmn[] = _("{B_TRAINER1_NAME_WITH_CLASS} envoie {B_OPPONENT_MON1_NAME} !\p{B_TRAINER2_NAME_WITH_CLASS} envoie {B_OPPONENT_MON2_NAME} !");
static const u8 sText_Trainer2SentOutPkmn[] = _("{B_TRAINER2_NAME_WITH_CLASS} envoie {B_BUFF1} !");
static const u8 sText_TwoTrainersWantToBattle[] = _("{B_TRAINER1_NAME_WITH_CLASS} et {B_TRAINER2_NAME_WITH_CLASS} veulent se battre !\p");
static const u8 sText_InGamePartnerSentOutZGoN[] = _("{B_PARTNER_NAME_WITH_CLASS} envoie {B_PLAYER_MON2_NAME} ! Vas-y, {B_PLAYER_MON1_NAME} !");
static const u8 sText_InGamePartnerSentOutNGoZ[] = _("{B_PARTNER_NAME_WITH_CLASS} envoie {B_PLAYER_MON1_NAME} ! Vas-y, {B_PLAYER_MON2_NAME} !");
static const u8 sText_InGamePartnerSentOutPkmn1[] = _("{B_PARTNER_NAME_WITH_CLASS} envoie {B_PLAYER_MON1_NAME} !");
static const u8 sText_InGamePartnerSentOutPkmn2[] = _("{B_PARTNER_NAME_WITH_CLASS} envoie {B_PLAYER_MON2_NAME} !");
static const u8 sText_InGamePartnerWithdrewPkmn1[] = _("{B_PARTNER_NAME_WITH_CLASS} rappelle {B_PLAYER_MON1_NAME} !");
static const u8 sText_InGamePartnerWithdrewPkmn2[] = _("{B_PARTNER_NAME_WITH_CLASS} rappelle {B_PLAYER_MON2_NAME} !");

const u16 gBattlePalaceFlavorTextTable[] =
{
    [B_MSG_GLINT_IN_EYE]   = STRINGID_GLINTAPPEARSINEYE,
    [B_MSG_GETTING_IN_POS] = STRINGID_PKMNGETTINGINTOPOSITION,
    [B_MSG_GROWL_DEEPLY]   = STRINGID_PKMNBEGANGROWLINGDEEPLY,
    [B_MSG_EAGER_FOR_MORE] = STRINGID_PKMNEAGERFORMORE,
};

const u8 *const gRefereeStringsTable[] =
{
    [B_MSG_REF_NOTHING_IS_DECIDED] = COMPOUND_STRING("REFEREE: If nothing is decided in 3 turns, we will go to judging!"),
    [B_MSG_REF_THATS_IT]           = COMPOUND_STRING("REFEREE: That's it! We will now go to judging to determine the winner!"),
    [B_MSG_REF_JUDGE_MIND]         = COMPOUND_STRING("REFEREE: Judging category 1, Mind! The POKéMON showing the most guts!\p"),
    [B_MSG_REF_JUDGE_SKILL]        = COMPOUND_STRING("REFEREE: Judging category 2, Skill! The POKéMON using moves the best!\p"),
    [B_MSG_REF_JUDGE_BODY]         = COMPOUND_STRING("REFEREE: Judging category 3, Body! The POKéMON with the most vitality!\p"),
    [B_MSG_REF_PLAYER_WON]         = COMPOUND_STRING("REFEREE: Judgment: {B_BUFF1} to {B_BUFF2}! The winner is {B_PLAYER_NAME}'s {B_PLAYER_MON1_NAME}!\p"),
    [B_MSG_REF_OPPONENT_WON]       = COMPOUND_STRING("REFEREE: Judgment: {B_BUFF1} to {B_BUFF2}! The winner is {B_TRAINER1_NAME}'s {B_OPPONENT_MON1_NAME}!\p"),
    [B_MSG_REF_DRAW]               = COMPOUND_STRING("REFEREE: Judgment: 3 to 3! We have a draw!\p"),
    [B_MSG_REF_COMMENCE_BATTLE]    = COMPOUND_STRING("REFEREE: {B_PLAYER_MON1_NAME} VS {B_OPPONENT_MON1_NAME}! Commence battling!"),
};

static const u8 sText_Trainer1Fled[] = _( "{PLAY_SE SE_FLEE}{B_TRAINER1_NAME_WITH_CLASS} fled!");
static const u8 sText_PlayerLostAgainstTrainer1[] = _("Tu as perdu contre {B_TRAINER1_NAME_WITH_CLASS} !");
static const u8 sText_PlayerBattledToDrawTrainer1[] = _("Match nul contre {B_TRAINER1_NAME_WITH_CLASS} !");
const u8 gText_RecordBattleToPass[] = _("Enregistrer ce combat\nsur ton Passe Zone ?");
const u8 gText_BattleRecordedOnPass[] = _("Le résultat du combat de {B_PLAYER_NAME}\nest enregistré sur le Passe Zone.");
static const u8 sText_LinkTrainerWantsToBattlePause[] = _("{B_LINK_OPPONENT1_NAME} veut se battre !\p");
static const u8 sText_TwoLinkTrainersWantToBattlePause[] = _("{B_LINK_OPPONENT1_NAME} et {B_LINK_OPPONENT2_NAME} veulent se battre !\p");
static const u8 sText_Your1[] = _("Votre équipe");
static const u8 sText_Opposing1[] = _("L'équipe ennemie");
static const u8 sText_Your2[] = _("votre équipe");
static const u8 sText_Opposing2[] = _("l'équipe ennemie");
static const u8 sText_EmptyStatus[] = _("$$$$$$$");

static const struct BattleWindowText sTextOnWindowsInfo_Normal[] =
{
    [B_WIN_MSG] = {
        .fillValue = PIXEL_FILL(0xF),
        .fontId = FONT_NORMAL,
        .x = 0,
        .y = 1,
        .speed = 1,
        .color.foreground = 1,
        .color.background = 15,
        .color.accent = 15,
        .color.shadow = 6,
    },
    [B_WIN_ACTION_PROMPT] = {
        .fillValue = PIXEL_FILL(0xF),
        .fontId = FONT_NORMAL,
        .x = 1,
        .y = 1,
        .speed = 0,
        .color.foreground = 1,
        .color.background = 15,
        .color.accent = 15,
        .color.shadow = 6,
    },
    [B_WIN_ACTION_MENU] = {
        .fillValue = PIXEL_FILL(0xE),
        .fontId = FONT_NORMAL,
        .x = 0,
        .y = 1,
        .speed = 0,
        .color.foreground = 13,
        .color.background = 14,
        .color.accent = 14,
        .color.shadow = 15,
    },
    [B_WIN_MOVE_NAME_1] = {
        .fillValue = PIXEL_FILL(0xE),
        .fontId = FONT_NARROW,
        .x = 0,
        .y = 1,
        .speed = 0,
        .color.foreground = 13,
        .color.background = 14,
        .color.accent = 14,
        .color.shadow = 15,
    },
    [B_WIN_MOVE_NAME_2] = {
        .fillValue = PIXEL_FILL(0xE),
        .fontId = FONT_NARROW,
        .x = 0,
        .y = 1,
        .speed = 0,
        .color.foreground = 13,
        .color.background = 14,
        .color.accent = 14,
        .color.shadow = 15,
    },
    [B_WIN_MOVE_NAME_3] = {
        .fillValue = PIXEL_FILL(0xE),
        .fontId = FONT_NARROW,
        .x = 0,
        .y = 1,
        .speed = 0,
        .color.foreground = 13,
        .color.background = 14,
        .color.accent = 14,
        .color.shadow = 15,
    },
    [B_WIN_MOVE_NAME_4] = {
        .fillValue = PIXEL_FILL(0xE),
        .fontId = FONT_NARROW,
        .x = 0,
        .y = 1,
        .speed = 0,
        .color.foreground = 13,
        .color.background = 14,
        .color.accent = 14,
        .color.shadow = 15,
    },
    [B_WIN_PP] = {
        .fillValue = PIXEL_FILL(0xE),
        .fontId = FONT_NARROW,
        .x = 0,
        .y = 1,
        .speed = 0,
        .color.foreground = B_SHOW_EFFECTIVENESS != SHOW_EFFECTIVENESS_NEVER ? 13 : 12,
        .color.background = 14,
        .color.accent = 14,
        .color.shadow = B_SHOW_EFFECTIVENESS != SHOW_EFFECTIVENESS_NEVER ? 15 : 11,
    },
    [B_WIN_DUMMY] = {
        .fillValue = PIXEL_FILL(0xE),
        .fontId = FONT_NORMAL,
        .x = 0,
        .y = 1,
        .speed = 0,
        .color.foreground = 13,
        .color.background = 14,
        .color.accent = 14,
        .color.shadow = 15,
    },
    [B_WIN_PP_REMAINING] = {
        .fillValue = PIXEL_FILL(0xE),
        .fontId = FONT_NORMAL,
        .x = 2,
        .y = 1,
        .speed = 0,
        .color.foreground = 12,
        .color.background = 14,
        .color.accent = 14,
        .color.shadow = 11,
    },
    [B_WIN_MOVE_TYPE] = {
        .fillValue = PIXEL_FILL(0xE),
        .fontId = FONT_NARROW,
        .x = 0,
        .y = 1,
        .speed = 0,
        .color.foreground = 13,
        .color.background = 14,
        .color.accent = 14,
        .color.shadow = 15,
    },
    [B_WIN_SWITCH_PROMPT] = {
        .fillValue = PIXEL_FILL(0xE),
        .fontId = FONT_NARROW,
        .x = 0,
        .y = 1,
        .speed = 0,
        .color.foreground = 13,
        .color.background = 14,
        .color.accent = 14,
        .color.shadow = 15,
    },
    [B_WIN_YESNO] = {
        .fillValue = PIXEL_FILL(0xE),
        .fontId = FONT_NORMAL,
        .x = 0,
        .y = 1,
        .speed = 0,
        .color.foreground = 13,
        .color.background = 14,
        .color.accent = 14,
        .color.shadow = 15,
    },
    [B_WIN_LEVEL_UP_BOX] = {
        .fillValue = PIXEL_FILL(0xE),
        .fontId = FONT_NORMAL,
        .x = 0,
        .y = 1,
        .speed = 0,
        .color.foreground = 13,
        .color.background = 14,
        .color.accent = 14,
        .color.shadow = 15,
    },
    [B_WIN_LEVEL_UP_BANNER] = {
        .fillValue = PIXEL_FILL(0),
        .fontId = FONT_NORMAL,
        .x = 32,
        .y = 1,
        .speed = 0,
        .color.foreground = 1,
        .color.shadow = 2,
    },
    [B_WIN_VS_PLAYER] = {
        .fillValue = PIXEL_FILL(0xE),
        .fontId = FONT_NORMAL,
        .x = -1,
        .y = 1,
        .speed = 0,
        .color.foreground = 13,
        .color.background = 14,
        .color.accent = 14,
        .color.shadow = 15,
    },
    [B_WIN_VS_OPPONENT] = {
        .fillValue = PIXEL_FILL(0xE),
        .fontId = FONT_NORMAL,
        .x = -1,
        .y = 1,
        .speed = 0,
        .color.foreground = 13,
        .color.background = 14,
        .color.accent = 14,
        .color.shadow = 15,
    },
    [B_WIN_VS_MULTI_PLAYER_1] = {
        .fillValue = PIXEL_FILL(0xE),
        .fontId = FONT_NORMAL,
        .x = -1,
        .y = 1,
        .speed = 0,
        .color.foreground = 13,
        .color.background = 14,
        .color.accent = 14,
        .color.shadow = 15,
    },
    [B_WIN_VS_MULTI_PLAYER_2] = {
        .fillValue = PIXEL_FILL(0xE),
        .fontId = FONT_NORMAL,
        .x = -1,
        .y = 1,
        .speed = 0,
        .color.foreground = 13,
        .color.background = 14,
        .color.accent = 14,
        .color.shadow = 15,
    },
    [B_WIN_VS_MULTI_PLAYER_3] = {
        .fillValue = PIXEL_FILL(0xE),
        .fontId = FONT_NORMAL,
        .x = -1,
        .y = 1,
        .speed = 0,
        .color.foreground = 13,
        .color.background = 14,
        .color.accent = 14,
        .color.shadow = 15,
    },
    [B_WIN_VS_MULTI_PLAYER_4] = {
        .fillValue = PIXEL_FILL(0xE),
        .fontId = FONT_NORMAL,
        .x = -1,
        .y = 1,
        .speed = 0,
        .color.foreground = 13,
        .color.background = 14,
        .color.accent = 14,
        .color.shadow = 15,
    },
    [B_WIN_VS_OUTCOME_DRAW] = {
        .fillValue = PIXEL_FILL(0),
        .fontId = FONT_NORMAL,
        .x = -1,
        .y = 1,
        .speed = 0,
        .color.foreground = 1,
        .color.shadow = 6,
    },
    [B_WIN_VS_OUTCOME_LEFT] = {
        .fillValue = PIXEL_FILL(0),
        .fontId = FONT_NORMAL,
        .x = -1,
        .y = 1,
        .speed = 0,
        .color.foreground = 1,
        .color.shadow = 6,
    },
    [B_WIN_VS_OUTCOME_RIGHT] = {
        .fillValue = PIXEL_FILL(0x0),
        .fontId = FONT_NORMAL,
        .x = -1,
        .y = 1,
        .speed = 0,
        .color.foreground = 1,
        .color.shadow = 6,
    },
    [B_WIN_MOVE_DESCRIPTION] = {
        .fillValue = PIXEL_FILL(0xE),
        .fontId = FONT_NARROW,
        .x = 0,
        .y = 1,
        .letterSpacing = 0,
        .lineSpacing = 0,
        .speed = 0,
        .color.foreground = TEXT_DYNAMIC_COLOR_4,
        .color.background = TEXT_DYNAMIC_COLOR_5,
        .color.accent = TEXT_DYNAMIC_COLOR_5,
        .color.shadow = TEXT_DYNAMIC_COLOR_6,
    },
    [B_CATCH_OR_NOT] = {
        .fillValue = PIXEL_FILL(0xE),
        .fontId = FONT_NORMAL,
        .x = 0,
        .y = 1,
        .speed = 0,
        .color.foreground = 13,
        .color.background = 14,
        .color.accent = 14,
        .color.shadow = 15,
    },
};

static const struct BattleWindowText sTextOnWindowsInfo_KantoTutorial[] =
{
    [B_WIN_MSG] = {
        .fillValue = PIXEL_FILL(0xF),
        .fontId = FONT_NORMAL,
        .x = 0,
        .y = 1,
        .speed = 1,
        .color.foreground = 1,
        .color.background = 15,
        .color.accent = 15,
        .color.shadow = 6,
    },
    [B_WIN_ACTION_PROMPT] = {
        .fillValue = PIXEL_FILL(0xF),
        .fontId = FONT_NORMAL,
        .x = 1,
        .y = 1,
        .speed = 0,
        .color.foreground = 1,
        .color.background = 15,
        .color.accent = 15,
        .color.shadow = 6,
    },
    [B_WIN_ACTION_MENU] = {
        .fillValue = PIXEL_FILL(0xE),
        .fontId = FONT_NORMAL,
        .x = 0,
        .y = 1,
        .speed = 0,
        .color.foreground = 13,
        .color.background = 14,
        .color.accent = 14,
        .color.shadow = 15,
    },
    [B_WIN_MOVE_NAME_1] = {
        .fillValue = PIXEL_FILL(0xE),
        .fontId = FONT_NARROW,
        .x = 0,
        .y = 1,
        .speed = 0,
        .color.foreground = 13,
        .color.background = 14,
        .color.accent = 14,
        .color.shadow = 15,
    },
    [B_WIN_MOVE_NAME_2] = {
        .fillValue = PIXEL_FILL(0xE),
        .fontId = FONT_NARROW,
        .x = 0,
        .y = 1,
        .speed = 0,
        .color.foreground = 13,
        .color.background = 14,
        .color.accent = 14,
        .color.shadow = 15,
    },
    [B_WIN_MOVE_NAME_3] = {
        .fillValue = PIXEL_FILL(0xE),
        .fontId = FONT_NARROW,
        .x = 0,
        .y = 1,
        .speed = 0,
        .color.foreground = 13,
        .color.background = 14,
        .color.accent = 14,
        .color.shadow = 15,
    },
    [B_WIN_MOVE_NAME_4] = {
        .fillValue = PIXEL_FILL(0xE),
        .fontId = FONT_NARROW,
        .x = 0,
        .y = 1,
        .speed = 0,
        .color.foreground = 13,
        .color.background = 14,
        .color.accent = 14,
        .color.shadow = 15,
    },
    [B_WIN_PP] = {
        .fillValue = PIXEL_FILL(0xE),
        .fontId = FONT_NARROW,
        .x = 0,
        .y = 1,
        .speed = 0,
        .color.foreground = B_SHOW_EFFECTIVENESS != SHOW_EFFECTIVENESS_NEVER ? 13 : 12,
        .color.background = 14,
        .color.accent = 14,
        .color.shadow = B_SHOW_EFFECTIVENESS != SHOW_EFFECTIVENESS_NEVER ? 15 : 11,
    },
    [B_WIN_DUMMY] = {
        .fillValue = PIXEL_FILL(0xE),
        .fontId = FONT_NORMAL,
        .x = 0,
        .y = 1,
        .speed = 0,
        .color.foreground = 13,
        .color.background = 14,
        .color.accent = 14,
        .color.shadow = 15,
    },
    [B_WIN_PP_REMAINING] = {
        .fillValue = PIXEL_FILL(0xE),
        .fontId = FONT_NORMAL,
        .x = 2,
        .y = 1,
        .speed = 0,
        .color.foreground = 12,
        .color.background = 14,
        .color.accent = 14,
        .color.shadow = 11,
    },
    [B_WIN_MOVE_TYPE] = {
        .fillValue = PIXEL_FILL(0xE),
        .fontId = FONT_NARROW,
        .x = 0,
        .y = 1,
        .speed = 0,
        .color.foreground = 13,
        .color.background = 14,
        .color.accent = 14,
        .color.shadow = 15,
    },
    [B_WIN_SWITCH_PROMPT] = {
        .fillValue = PIXEL_FILL(0xE),
        .fontId = FONT_NARROW,
        .x = 0,
        .y = 1,
        .speed = 0,
        .color.foreground = 13,
        .color.background = 14,
        .color.accent = 14,
        .color.shadow = 15,
    },
    [B_WIN_YESNO] = {
        .fillValue = PIXEL_FILL(0xE),
        .fontId = FONT_NORMAL,
        .x = 0,
        .y = 1,
        .speed = 0,
        .color.foreground = 13,
        .color.background = 14,
        .color.accent = 14,
        .color.shadow = 15,
    },
    [B_WIN_LEVEL_UP_BOX] = {
        .fillValue = PIXEL_FILL(0xE),
        .fontId = FONT_NORMAL,
        .x = 0,
        .y = 1,
        .speed = 0,
        .color.foreground = 13,
        .color.background = 14,
        .color.accent = 14,
        .color.shadow = 15,
    },
    [B_WIN_LEVEL_UP_BANNER] = {
        .fillValue = PIXEL_FILL(0),
        .fontId = FONT_NORMAL,
        .x = 32,
        .y = 1,
        .speed = 0,
        .color.foreground = 1,
        .color.shadow = 2,
    },
    [B_WIN_VS_PLAYER] = {
        .fillValue = PIXEL_FILL(0xE),
        .fontId = FONT_NORMAL,
        .x = -1,
        .y = 1,
        .speed = 0,
        .color.foreground = 13,
        .color.background = 14,
        .color.accent = 14,
        .color.shadow = 15,
    },
    [B_WIN_VS_OPPONENT] = {
        .fillValue = PIXEL_FILL(0xE),
        .fontId = FONT_NORMAL,
        .x = -1,
        .y = 1,
        .speed = 0,
        .color.foreground = 13,
        .color.background = 14,
        .color.accent = 14,
        .color.shadow = 15,
    },
    [B_WIN_VS_MULTI_PLAYER_1] = {
        .fillValue = PIXEL_FILL(0xE),
        .fontId = FONT_NORMAL,
        .x = -1,
        .y = 1,
        .speed = 0,
        .color.foreground = 13,
        .color.background = 14,
        .color.accent = 14,
        .color.shadow = 15,
    },
    [B_WIN_VS_MULTI_PLAYER_2] = {
        .fillValue = PIXEL_FILL(0xE),
        .fontId = FONT_NORMAL,
        .x = -1,
        .y = 1,
        .speed = 0,
        .color.foreground = 13,
        .color.background = 14,
        .color.accent = 14,
        .color.shadow = 15,
    },
    [B_WIN_VS_MULTI_PLAYER_3] = {
        .fillValue = PIXEL_FILL(0xE),
        .fontId = FONT_NORMAL,
        .x = -1,
        .y = 1,
        .speed = 0,
        .color.foreground = 13,
        .color.background = 14,
        .color.accent = 14,
        .color.shadow = 15,
    },
    [B_WIN_VS_MULTI_PLAYER_4] = {
        .fillValue = PIXEL_FILL(0xE),
        .fontId = FONT_NORMAL,
        .x = -1,
        .y = 1,
        .speed = 0,
        .color.foreground = 13,
        .color.background = 14,
        .color.accent = 14,
        .color.shadow = 15,
    },
    [B_WIN_VS_OUTCOME_DRAW] = {
        .fillValue = PIXEL_FILL(0),
        .fontId = FONT_NORMAL,
        .x = -1,
        .y = 1,
        .speed = 0,
        .color.foreground = 1,
        .color.shadow = 6,
    },
    [B_WIN_VS_OUTCOME_LEFT] = {
        .fillValue = PIXEL_FILL(0),
        .fontId = FONT_NORMAL,
        .x = -1,
        .y = 1,
        .speed = 0,
        .color.foreground = 1,
        .color.shadow = 6,
    },
    [B_WIN_VS_OUTCOME_RIGHT] = {
        .fillValue = PIXEL_FILL(0x0),
        .fontId = FONT_NORMAL,
        .x = -1,
        .y = 1,
        .speed = 0,
        .color.foreground = 1,
        .color.shadow = 6,
    },
    [B_WIN_MOVE_DESCRIPTION] = {
        .fillValue = PIXEL_FILL(0xE),
        .fontId = FONT_NARROW,
        .x = 0,
        .y = 1,
        .letterSpacing = 0,
        .lineSpacing = 0,
        .speed = 0,
        .color.foreground = TEXT_DYNAMIC_COLOR_4,
        .color.background = TEXT_DYNAMIC_COLOR_5,
        .color.accent = TEXT_DYNAMIC_COLOR_5,
        .color.shadow = TEXT_DYNAMIC_COLOR_6,
    },
    [B_WIN_OAK_OLD_MAN] = {
        .fillValue = PIXEL_FILL(0x1),
        .fontId = FONT_NORMAL,
        .x = 0,
        .y = 1,
        .letterSpacing = 0,
        .lineSpacing = 1,
        .speed = 1,
        .fgColor = 2,
        .bgColor = 1,
        .shadowColor = 3,
    },
};

static const struct BattleWindowText sTextOnWindowsInfo_Arena[] =
{
    [B_WIN_MSG] = {
        .fillValue = PIXEL_FILL(0xF),
        .fontId = FONT_NORMAL,
        .x = 0,
        .y = 1,
        .speed = 1,
        .color.foreground = 1,
        .color.background = 15,
        .color.accent = 15,
        .color.shadow = 6,
    },
    [B_WIN_ACTION_PROMPT] = {
        .fillValue = PIXEL_FILL(0xF),
        .fontId = FONT_NORMAL,
        .x = 1,
        .y = 1,
        .speed = 0,
        .color.foreground = 1,
        .color.background = 15,
        .color.accent = 15,
        .color.shadow = 6,
    },
    [B_WIN_ACTION_MENU] = {
        .fillValue = PIXEL_FILL(0xE),
        .fontId = FONT_NORMAL,
        .x = 0,
        .y = 1,
        .speed = 0,
        .color.foreground = 13,
        .color.background = 14,
        .color.accent = 14,
        .color.shadow = 15,
    },
    [B_WIN_MOVE_NAME_1] = {
        .fillValue = PIXEL_FILL(0xE),
        .fontId = FONT_NARROW,
        .x = 0,
        .y = 1,
        .speed = 0,
        .color.foreground = 13,
        .color.background = 14,
        .color.accent = 14,
        .color.shadow = 15,
    },
    [B_WIN_MOVE_NAME_2] = {
        .fillValue = PIXEL_FILL(0xE),
        .fontId = FONT_NARROW,
        .x = 0,
        .y = 1,
        .speed = 0,
        .color.foreground = 13,
        .color.background = 14,
        .color.accent = 14,
        .color.shadow = 15,
    },
    [B_WIN_MOVE_NAME_3] = {
        .fillValue = PIXEL_FILL(0xE),
        .fontId = FONT_NARROW,
        .x = 0,
        .y = 1,
        .speed = 0,
        .color.foreground = 13,
        .color.background = 14,
        .color.accent = 14,
        .color.shadow = 15,
    },
    [B_WIN_MOVE_NAME_4] = {
        .fillValue = PIXEL_FILL(0xE),
        .fontId = FONT_NARROW,
        .x = 0,
        .y = 1,
        .speed = 0,
        .color.foreground = 13,
        .color.background = 14,
        .color.accent = 14,
        .color.shadow = 15,
    },
    [B_WIN_PP] = {
        .fillValue = PIXEL_FILL(0xE),
        .fontId = FONT_NARROW,
        .x = 0,
        .y = 1,
        .speed = 0,
        .color.foreground = B_SHOW_EFFECTIVENESS != SHOW_EFFECTIVENESS_NEVER ? 13 : 12,
        .color.background = 14,
        .color.accent = 14,
        .color.shadow = B_SHOW_EFFECTIVENESS != SHOW_EFFECTIVENESS_NEVER ? 15 : 11,
    },
    [B_WIN_DUMMY] = {
        .fillValue = PIXEL_FILL(0xE),
        .fontId = FONT_NORMAL,
        .x = 0,
        .y = 1,
        .speed = 0,
        .color.foreground = 13,
        .color.background = 14,
        .color.accent = 14,
        .color.shadow = 15,
    },
    [B_WIN_PP_REMAINING] = {
        .fillValue = PIXEL_FILL(0xE),
        .fontId = FONT_NORMAL,
        .x = 2,
        .y = 1,
        .speed = 0,
        .color.foreground = 12,
        .color.background = 14,
        .color.accent = 14,
        .color.shadow = 11,
    },
    [B_WIN_MOVE_TYPE] = {
        .fillValue = PIXEL_FILL(0xE),
        .fontId = FONT_NARROW,
        .x = 0,
        .y = 1,
        .speed = 0,
        .color.foreground = 13,
        .color.background = 14,
        .color.accent = 14,
        .color.shadow = 15,
    },
    [B_WIN_SWITCH_PROMPT] = {
        .fillValue = PIXEL_FILL(0xE),
        .fontId = FONT_NARROW,
        .x = 0,
        .y = 1,
        .speed = 0,
        .color.foreground = 13,
        .color.background = 14,
        .color.accent = 14,
        .color.shadow = 15,
    },
    [B_WIN_YESNO] = {
        .fillValue = PIXEL_FILL(0xE),
        .fontId = FONT_NORMAL,
        .x = 0,
        .y = 1,
        .speed = 0,
        .color.foreground = 13,
        .color.background = 14,
        .color.accent = 14,
        .color.shadow = 15,
    },
    [B_WIN_LEVEL_UP_BOX] = {
        .fillValue = PIXEL_FILL(0xE),
        .fontId = FONT_NORMAL,
        .x = 0,
        .y = 1,
        .speed = 0,
        .color.foreground = 13,
        .color.background = 14,
        .color.accent = 14,
        .color.shadow = 15,
    },
    [B_WIN_LEVEL_UP_BANNER] = {
        .fillValue = PIXEL_FILL(0),
        .fontId = FONT_NORMAL,
        .x = 32,
        .y = 1,
        .speed = 0,
        .color.foreground = 1,
        .color.shadow = 2,
    },
    [ARENA_WIN_PLAYER_NAME] = {
        .fillValue = PIXEL_FILL(0xE),
        .fontId = FONT_NORMAL,
        .x = -1,
        .y = 1,
        .speed = 0,
        .color.foreground = 1,
        .color.background = 14,
        .color.accent = 14,
        .color.shadow = 15,
    },
    [ARENA_WIN_VS] = {
        .fillValue = PIXEL_FILL(0xE),
        .fontId = FONT_NORMAL,
        .x = -1,
        .y = 1,
        .speed = 0,
        .color.foreground = 13,
        .color.background = 14,
        .color.accent = 14,
        .color.shadow = 15,
    },
    [ARENA_WIN_OPPONENT_NAME] = {
        .fillValue = PIXEL_FILL(0xE),
        .fontId = FONT_NORMAL,
        .x = -1,
        .y = 1,
        .speed = 0,
        .color.foreground = 13,
        .color.background = 14,
        .color.accent = 14,
        .color.shadow = 15,
    },
    [ARENA_WIN_MIND] = {
        .fillValue = PIXEL_FILL(0xE),
        .fontId = FONT_NORMAL,
        .x = -1,
        .y = 1,
        .speed = 0,
        .color.foreground = 13,
        .color.background = 14,
        .color.accent = 14,
        .color.shadow = 15,
    },
    [ARENA_WIN_SKILL] = {
        .fillValue = PIXEL_FILL(0xE),
        .fontId = FONT_NORMAL,
        .x = -1,
        .y = 1,
        .speed = 0,
        .color.foreground = 13,
        .color.background = 14,
        .color.accent = 14,
        .color.shadow = 15,
    },
    [ARENA_WIN_BODY] = {
        .fillValue = PIXEL_FILL(0xE),
        .fontId = FONT_NORMAL,
        .x = -1,
        .y = 1,
        .speed = 0,
        .color.foreground = 13,
        .color.background = 14,
        .color.accent = 14,
        .color.shadow = 15,
    },
    [ARENA_WIN_JUDGMENT_TITLE] = {
        .fillValue = PIXEL_FILL(0xE),
        .fontId = FONT_NORMAL,
        .x = -1,
        .y = 1,
        .speed = 0,
        .color.foreground = 13,
        .color.background = 14,
        .color.accent = 14,
        .color.shadow = 15,
    },
    [ARENA_WIN_JUDGMENT_TEXT] = {
        .fillValue = PIXEL_FILL(0x1),
        .fontId = FONT_NORMAL,
        .x = 0,
        .y = 1,
        .speed = 1,
        .color.foreground = 2,
        .color.background = 1,
        .color.accent = 1,
        .color.shadow = 3,
    },
    [B_WIN_MOVE_DESCRIPTION] = {
        .fillValue = PIXEL_FILL(0xE),
        .fontId = FONT_NARROW,
        .x = 0,
        .y = 1,
        .letterSpacing = 0,
        .lineSpacing = 0,
        .speed = 0,
        .color.foreground = TEXT_DYNAMIC_COLOR_4,
        .color.background = TEXT_DYNAMIC_COLOR_5,
        .color.accent = TEXT_DYNAMIC_COLOR_5,
        .color.shadow = TEXT_DYNAMIC_COLOR_6,
    },
};

static const struct BattleWindowText *const sBattleTextOnWindowsInfo[] =
{
    [B_WIN_TYPE_NORMAL] = sTextOnWindowsInfo_Normal,
    [B_WIN_TYPE_ARENA]  = sTextOnWindowsInfo_Arena,
    [B_WIN_TYPE_KANTO_TUTORIAL] = sTextOnWindowsInfo_KantoTutorial,
};

static const u8 sRecordedBattleTextSpeeds[] = {8, 4, 1, 0};

void BufferStringBattle(enum StringID stringID, enum BattlerId battler)
{
    s32 i;
    const u8 *stringPtr = NULL;

    gBattleMsgDataPtr = (struct BattleMsgData *)(&gBattleResources->bufferA[battler][4]);
    gLastUsedItem = gBattleMsgDataPtr->lastItem;
    gLastUsedAbility = gBattleMsgDataPtr->lastAbility;
    gBattleScripting.battler = gBattleMsgDataPtr->scrActive;
    gBattleStruct->scriptPartyIdx = gBattleMsgDataPtr->bakScriptPartyIdx;
    gBattleStruct->hpScale = gBattleMsgDataPtr->hpScale;
    gPotentialItemEffectBattler = gBattleMsgDataPtr->itemEffectBattler;
    gBattleStruct->stringMoveType = gBattleMsgDataPtr->moveType;

    for (i = 0; i < MAX_BATTLERS_COUNT; i++)
    {
        sBattlerAbilities[i] = gBattleMsgDataPtr->abilities[i];
    }
    for (i = 0; i < TEXT_BUFF_ARRAY_COUNT; i++)
    {
        gBattleTextBuff1[i] = gBattleMsgDataPtr->textBuffs[0][i];
        gBattleTextBuff2[i] = gBattleMsgDataPtr->textBuffs[1][i];
        gBattleTextBuff3[i] = gBattleMsgDataPtr->textBuffs[2][i];
    }

    switch (stringID)
    {
    case STRINGID_INTROMSG: // first battle msg
        if (gBattleTypeFlags & BATTLE_TYPE_TRAINER)
        {
            if (gBattleTypeFlags & (BATTLE_TYPE_LINK | BATTLE_TYPE_RECORDED_LINK))
            {
                if (gBattleTypeFlags & BATTLE_TYPE_TOWER_LINK_MULTI)
                {
                    stringPtr = sText_TwoTrainersWantToBattle;
                }
                else if (gBattleTypeFlags & BATTLE_TYPE_TWO_OPPONENTS)
                {
                    stringPtr = sText_TwoLinkTrainersWantToBattle;
                }
                else if (gBattleTypeFlags & BATTLE_TYPE_MULTI)
                {
                    if (gBattleTypeFlags & BATTLE_TYPE_RECORDED)
                    {
                        if (gBattleTypeFlags & BATTLE_TYPE_RECORDED_LINK)
                            stringPtr = sText_LinkTrainerWantsToBattle;
                        else if (gBattleTypeFlags & BATTLE_TYPE_TWO_OPPONENTS)
                            stringPtr = sText_TwoTrainersWantToBattle;
                        else if (!(gBattleTypeFlags & BATTLE_TYPE_TWO_OPPONENTS))
                            stringPtr = sText_LinkTrainerWantsToBattlePause;
                        else
                            stringPtr = sText_TwoLinkTrainersWantToBattlePause;
                    }
                    else
                    {
                        stringPtr = sText_TwoLinkTrainersWantToBattle;
                    }
                }
                else
                {
                    if (TRAINER_BATTLE_PARAM.opponentA == TRAINER_UNION_ROOM)
                        stringPtr = sText_Trainer1WantsToBattle;
                    else if (gBattleTypeFlags & BATTLE_TYPE_RECORDED)
                        stringPtr = sText_LinkTrainerWantsToBattlePause;
                    else
                        stringPtr = sText_LinkTrainerWantsToBattle;
                }
            }
            else
            {
                if (BATTLE_TWO_VS_ONE_OPPONENT)
                    stringPtr = sText_Trainer1WantsToBattle;
                else if (gBattleTypeFlags & (BATTLE_TYPE_MULTI | BATTLE_TYPE_INGAME_PARTNER))
                    stringPtr = sText_TwoTrainersWantToBattle;
                else if (gBattleTypeFlags & BATTLE_TYPE_TWO_OPPONENTS)
                    stringPtr = sText_TwoTrainersWantToBattle;
                else
                    stringPtr = sText_Trainer1WantsToBattle;
            }
        }
        else
        {
            if (IsGhostBattleWithoutScope())
                stringPtr = sText_GhostAppearedCantId;
            else if (gBattleTypeFlags & BATTLE_TYPE_GHOST)
                stringPtr = sText_TheGhostAppeared;
            else if (gBattleTypeFlags & BATTLE_TYPE_LEGENDARY)
                stringPtr = sText_LegendaryPkmnAppeared;
            else if (IsDoubleBattle() && IsValidForBattle(GetBattlerMon(GetBattlerAtPosition(B_POSITION_OPPONENT_RIGHT))))
                stringPtr = sText_TwoWildPkmnAppeared;
            else if (gBattleTypeFlags & BATTLE_TYPE_CATCH_TUTORIAL)
                stringPtr = sText_WildPkmnAppearedPause;
            else
                stringPtr = sText_WildPkmnAppeared;
        }
        break;
    case STRINGID_INTROSENDOUT: // poke first send-out
        if (IsOnPlayerSide(battler))
        {
            if (IsDoubleBattle() && IsValidForBattle(GetBattlerMon(GetPartnerBattler(battler))))
            {
                if (gBattleTypeFlags & BATTLE_TYPE_MULTI && (gBattleTypeFlags & BATTLE_TYPE_RECORDED_LINK || gBattleTypeFlags & BATTLE_TYPE_LINK))
                {
                    if (BattlerIsPlayer(battler))
                        stringPtr = sText_LinkPartnerSentOutPkmn2GoPkmn; // Player is on left
                    else
                        stringPtr = sText_LinkPartnerSentOutPkmn1GoPkmn; // Link Partner on left
                }
                else if (gBattleTypeFlags & BATTLE_TYPE_INGAME_PARTNER)
                {
                    if (BattlerIsPlayer(battler) && (battler & BIT_FLANK) == B_FLANK_LEFT)
                        stringPtr = sText_InGamePartnerSentOutZGoN; // Player is on left
                    else
                        stringPtr = sText_InGamePartnerSentOutNGoZ; // Partner on left
                }
                else if (gBattleTypeFlags & BATTLE_TYPE_RAID)
                {
                    stringPtr = sText_BeCarefulPkmn;
                }
                else
                {
                    stringPtr = sText_GoTwoPkmn;
                }
            }
            else
            {
                stringPtr = sText_GoPkmn;
            }
        }
        else
        {
            if (IsDoubleBattle() && IsValidForBattle(GetBattlerMon(GetPartnerBattler(battler))))
            {
                if (gBattleTypeFlags & BATTLE_TYPE_RECORDED_LINK && BATTLE_TWO_VS_ONE_OPPONENT)
                    stringPtr = sText_LinkTrainerSentOutTwoPkmn;
                else if (BATTLE_TWO_VS_ONE_OPPONENT)
                    stringPtr = sText_Trainer1SentOutTwoPkmn;
                else if ((gBattleTypeFlags & BATTLE_TYPE_RECORDED_LINK && gBattleTypeFlags & BATTLE_TYPE_TWO_OPPONENTS) || (gBattleTypeFlags & BATTLE_TYPE_MULTI && (gBattleTypeFlags & BATTLE_TYPE_RECORDED_LINK || gBattleTypeFlags & BATTLE_TYPE_LINK)))
                    stringPtr = sText_TwoLinkTrainersIntroSendOutPkmn;
                else if (gBattleTypeFlags & BATTLE_TYPE_TWO_OPPONENTS || gBattleTypeFlags & BATTLE_TYPE_TOWER_LINK_MULTI)
                    stringPtr = sText_TwoTrainersSentPkmn;
                else if (BattlerIsLink(battler) || (BattlerIsRecorded(battler) && BattlerIsOpponent(battler))) // Link Opponent doubles and test opponent
                    stringPtr = sText_LinkTrainerSentOutTwoPkmn;
                else
                    stringPtr = sText_Trainer1SentOutTwoPkmn;
            }
            else if (BattlerIsLink(battler) || (BattlerIsRecorded(battler) && BattlerIsOpponent(battler)))
            {
                if (TRAINER_BATTLE_PARAM.opponentA == TRAINER_UNION_ROOM)
                    stringPtr = sText_Trainer1SentOutPkmn;
                else
                    stringPtr = sText_LinkTrainerIntroSendOutPkmn;
            }
            else
            {
                stringPtr = sText_Trainer1SentOutPkmn;
            }
        }
        break;
    case STRINGID_RETURNMON: // sending poke to ball msg
        if ((GetBattlerPosition(battler) & BIT_FLANK) == B_FLANK_LEFT) // battler 0 and 1
        {
            if (BattlerIsPlayer(battler) || BattlerIsWally(battler)) // Player
            {
                if (*(&gBattleStruct->hpScale) == 0)
                    stringPtr = sText_PkmnThatsEnough;
                else if (*(&gBattleStruct->hpScale) == 1 || IsDoubleBattle())
                    stringPtr = sText_PkmnComeBack;
                else if (*(&gBattleStruct->hpScale) == 2)
                    stringPtr = sText_PkmnOkComeBack;
                else
                    stringPtr = sText_PkmnGoodComeBack;
            }
            else if (BattlerIsPartner(battler)) // Link or Ingame Partner
            {
                if (BattlerIsLink(battler) || gBattleTypeFlags & BATTLE_TYPE_RECORDED_LINK)
                    stringPtr = sText_LinkPartnerWithdrewPkmn1;
                else
                    stringPtr = sText_InGamePartnerWithdrewPkmn1;
            }
            else if (TRAINER_BATTLE_PARAM.opponentA == TRAINER_LINK_OPPONENT
             || gBattleTypeFlags & BATTLE_TYPE_RECORDED_LINK) // Link Opponent A and test opponent
            {
                stringPtr = sText_LinkTrainer1WithdrewPkmn;
            }
            else // Opponent A
            {
                stringPtr = sText_Trainer1WithdrewPkmn;
            }
        }
        else // battler 2 and 3
        {
            if (BattlerIsPlayer(battler)) // Player
            {
                if (*(&gBattleStruct->hpScale) == 0)
                    stringPtr = sText_PkmnThatsEnough;
                else if (*(&gBattleStruct->hpScale) == 1 || IsDoubleBattle())
                    stringPtr = sText_PkmnComeBack;
                else if (*(&gBattleStruct->hpScale) == 2)
                    stringPtr = sText_PkmnOkComeBack;
                else
                    stringPtr = sText_PkmnGoodComeBack;
            }
            else if (BattlerIsPartner(battler)) // Link or Ingame Partner
            {
                if (BattlerIsLink(battler) || gBattleTypeFlags & BATTLE_TYPE_RECORDED_LINK)
                    stringPtr = sText_LinkPartnerWithdrewPkmn2;
                else
                    stringPtr = sText_InGamePartnerWithdrewPkmn2;
            }
            else if (TRAINER_BATTLE_PARAM.opponentA == TRAINER_LINK_OPPONENT
             || gBattleTypeFlags & BATTLE_TYPE_RECORDED_LINK) // Link Opponent B and test opponent
            {
                if (BattleSideHasTwoTrainers(B_SIDE_OPPONENT))
                    stringPtr = sText_LinkTrainer2WithdrewPkmn;
                else
                    stringPtr = sText_LinkTrainer1WithdrewPkmn;
            }
            else if (gBattleTypeFlags & BATTLE_TYPE_TWO_OPPONENTS) // Opponent B
            {
                stringPtr = sText_Trainer2WithdrewPkmn;
            }
            else // Opponent A
            {
                stringPtr = sText_Trainer1WithdrewPkmn;
            }
        }
        break;
    case STRINGID_SWITCHINMON: // switch-in msg
        if ((GetBattlerPosition(gBattleScripting.battler) & BIT_FLANK) == B_FLANK_LEFT) // battler 0 and 1
        {
            if (BattlerIsPlayer(gBattleScripting.battler)) // Player
            {
                if (*(&gBattleStruct->hpScale) == 0)
                    stringPtr = sText_GoPkmn2;
                else if (*(&gBattleStruct->hpScale) == 1 || IsDoubleBattle())
                    stringPtr = sText_DoItPkmn;
                else if (*(&gBattleStruct->hpScale) == 2)
                    stringPtr = sText_GoForItPkmn;
                else
                    stringPtr = sText_YourFoesWeakGetEmPkmn;
            }
            else if (BattlerIsPartner(gBattleScripting.battler)) // Link or Ingame Partner
            {
                if (BattlerIsLink(gBattleScripting.battler) || gBattleTypeFlags & BATTLE_TYPE_RECORDED_LINK)
                    stringPtr = sText_LinkPartnerSentOutPkmn1;
                else
                    stringPtr = sText_InGamePartnerSentOutPkmn1;
            }
            else if (TRAINER_BATTLE_PARAM.opponentA == TRAINER_LINK_OPPONENT
             || gBattleTypeFlags & BATTLE_TYPE_RECORDED_LINK) // Link Opponent A and test opponent
            {
                stringPtr = sText_LinkTrainerSentOutPkmn;
            }
            else // Opponent A
            {
                stringPtr = sText_Trainer1SentOutPkmn;
            }
        }
        else // battler 2 and 3
        {
            if (BattlerIsPlayer(gBattleScripting.battler)) // Player
            {
                if (*(&gBattleStruct->hpScale) == 0)
                    stringPtr = sText_GoPkmn2;
                else if (*(&gBattleStruct->hpScale) == 1 || IsDoubleBattle())
                    stringPtr = sText_DoItPkmn;
                else if (*(&gBattleStruct->hpScale) == 2)
                    stringPtr = sText_GoForItPkmn;
                else
                    stringPtr = sText_YourFoesWeakGetEmPkmn;
            }
            else if (BattlerIsPartner(gBattleScripting.battler)) // Link or Ingame Partner
            {
                if (BattlerIsLink(gBattleScripting.battler) || gBattleTypeFlags & BATTLE_TYPE_RECORDED_LINK)
                    stringPtr = sText_LinkPartnerSentOutPkmn2;
                else
                    stringPtr = sText_InGamePartnerSentOutPkmn2;
            }
            else if (TRAINER_BATTLE_PARAM.opponentA == TRAINER_LINK_OPPONENT
             || gBattleTypeFlags & BATTLE_TYPE_RECORDED_LINK) // Link Opponent B and test opponent
            {
                if (BattleSideHasTwoTrainers(B_SIDE_OPPONENT))
                    stringPtr = sText_LinkTrainer2SentOutPkmn2;
                else
                    stringPtr = sText_LinkTrainerSentOutPkmn2;
            }
            else if (gBattleTypeFlags & BATTLE_TYPE_TWO_OPPONENTS) // Opponent B
            {
                stringPtr = sText_Trainer2SentOutPkmn;
            }
            else // Single trainer double Opponent A
            {
                stringPtr = sText_Trainer1SentOutPkmn2;
            }
        }
        break;
    case STRINGID_USEDMOVE: // Pokémon used a move msg
        if (gBattleMsgDataPtr->currentMove >= MOVES_COUNT
         && !IsZMove(gBattleMsgDataPtr->currentMove)
         && !IsMaxMove(gBattleMsgDataPtr->currentMove))
            StringCopy(gBattleTextBuff3, gTypesInfo[*(&gBattleStruct->stringMoveType)].generic);
        else
            StringCopy(gBattleTextBuff3, GetMoveName(gBattleMsgDataPtr->currentMove));
        stringPtr = sText_AttackerUsedX;
        break;
    case STRINGID_BATTLEEND: // battle end
        if (gBattleTextBuff1[0] & B_OUTCOME_LINK_BATTLE_RAN)
        {
            gBattleTextBuff1[0] &= ~(B_OUTCOME_LINK_BATTLE_RAN);
            if (!(BattlerIsPlayer(battler) || BattlerIsPlayer(GetPartnerBattler(battler))) && gBattleTextBuff1[0] != B_OUTCOME_DREW)
                gBattleTextBuff1[0] ^= (B_OUTCOME_LOST | B_OUTCOME_WON);

            if (gBattleTextBuff1[0] == B_OUTCOME_LOST || gBattleTextBuff1[0] == B_OUTCOME_DREW)
                stringPtr = sText_GotAwaySafely;
            else if (gBattleTypeFlags & BATTLE_TYPE_MULTI)
                stringPtr = sText_TwoWildFled;
            else
                stringPtr = sText_WildFled;
        }
        else
        {
            if (!(BattlerIsPlayer(battler) || BattlerIsPlayer(GetPartnerBattler(battler))) && gBattleTextBuff1[0] != B_OUTCOME_DREW)
                gBattleTextBuff1[0] ^= (B_OUTCOME_LOST | B_OUTCOME_WON);

            if (gBattleTypeFlags & BATTLE_TYPE_MULTI)
            {
                switch (gBattleTextBuff1[0])
                {
                case B_OUTCOME_WON:
                    if (gBattleTypeFlags & BATTLE_TYPE_TOWER_LINK_MULTI)
                        stringPtr = sText_TwoInGameTrainersDefeated;
                    else
                        stringPtr = sText_TwoLinkTrainersDefeated;
                    break;
                case B_OUTCOME_LOST:
                    stringPtr = sText_PlayerLostToTwo;
                    break;
                case B_OUTCOME_DREW:
                    stringPtr = sText_PlayerBattledToDrawVsTwo;
                    break;
                }
            }
            else if (TRAINER_BATTLE_PARAM.opponentA == TRAINER_UNION_ROOM)
            {
                switch (gBattleTextBuff1[0])
                {
                case B_OUTCOME_WON:
                    stringPtr = sText_PlayerDefeatedLinkTrainerTrainer1;
                    break;
                case B_OUTCOME_LOST:
                    stringPtr = sText_PlayerLostAgainstTrainer1;
                    break;
                case B_OUTCOME_DREW:
                    stringPtr = sText_PlayerBattledToDrawTrainer1;
                    break;
                }
            }
            else
            {
                switch (gBattleTextBuff1[0])
                {
                case B_OUTCOME_WON:
                    stringPtr = sText_PlayerDefeatedLinkTrainer;
                    break;
                case B_OUTCOME_LOST:
                    stringPtr = sText_PlayerLostAgainstLinkTrainer;
                    break;
                case B_OUTCOME_DREW:
                    stringPtr = sText_PlayerBattledToDrawLinkTrainer;
                    break;
                }
            }
        }
        break;
    case STRINGID_TRAINERSLIDE:
        stringPtr = gBattleStruct->trainerSlideMsg;
        break;
    default: // load a string from the table
        if (stringID >= STRINGID_COUNT)
        {
            gDisplayedStringBattle[0] = EOS;
            return;
        }
        else
        {
            stringPtr = gBattleStringsTable[stringID];
        }
        break;
    }

    BattleStringExpandPlaceholdersToDisplayedString(stringPtr);
}

u32 BattleStringExpandPlaceholdersToDisplayedString(const u8 *src)
{
#ifndef NDEBUG
    u32 j, strWidth;
    u32 dstID = BattleStringExpandPlaceholders(src, gDisplayedStringBattle, sizeof(gDisplayedStringBattle));
    for (j = 1;; j++)
    {
        strWidth = GetStringLineWidth(0, gDisplayedStringBattle, 0, j, sizeof(gDisplayedStringBattle));
        if (strWidth == 0)
            break;
    }
    return dstID;
#else
    return BattleStringExpandPlaceholders(src, gDisplayedStringBattle, sizeof(gDisplayedStringBattle));
#endif
}

static const u8 *TryGetStatusString(u8 *src)
{
    u32 i;
    u8 status[8];
    u32 chars1, chars2;
    u8 *statusPtr;

    memcpy(status, sText_EmptyStatus, min(ARRAY_COUNT(status), ARRAY_COUNT(sText_EmptyStatus)));

    statusPtr = status;
    for (i = 0; i < ARRAY_COUNT(status); i++)
    {
        if (*src == EOS) break; // one line required to match -g
        *statusPtr = *src;
        src++;
        statusPtr++;
    }

    chars1 = *(u32 *)(&status[0]);
    chars2 = *(u32 *)(&status[4]);

    for (i = 0; i < ARRAY_COUNT(gStatusConditionStringsTable); i++)
    {
        if (chars1 == *(u32 *)(&gStatusConditionStringsTable[i][0][0])
            && chars2 == *(u32 *)(&gStatusConditionStringsTable[i][0][4]))
            return gStatusConditionStringsTable[i][1];
    }
    return NULL;
}

static void GetBattlerNick(enum BattlerId battler, u8 *dst)
{
    struct Pokemon *illusionMon = GetIllusionMonPtr(battler);
    struct Pokemon *mon = GetBattlerMon(battler);

    if (illusionMon != NULL)
        mon = illusionMon;
    GetMonData(mon, MON_DATA_NICKNAME, dst);
    StringGet_Nickname(dst);
}

#define HANDLE_NICKNAME_STRING_CASE(battler)                                              \
    GetBattlerNick(battler, text);                                      \
    toCpy = text;                                                       \
    if (!IsOnPlayerSide(battler))                                       \
    {                                                                   \
        /* Émeraude Couronnement : en français, le qualificatif suit le nom (Zigzaton sauvage). */ \
        while (*toCpy != EOS)                                           \
        {                                                               \
            dst[dstID] = *toCpy;                                        \
            dstID++;                                                    \
            toCpy++;                                                    \
        }                                                               \
        if (gBattleTypeFlags & BATTLE_TYPE_TRAINER)                     \
            toCpy = sText_FoePkmnPrefix;                                \
        else                                                            \
            toCpy = sText_WildPkmnPrefix;                               \
    }

#define HANDLE_NICKNAME_STRING_LOWERCASE(battler)                                              \
    GetBattlerNick(battler, text);                                      \
    toCpy = text;                                                       \
    if (!IsOnPlayerSide(battler))                                       \
    {                                                                   \
        /* Émeraude Couronnement : en français, le qualificatif suit le nom (Zigzaton sauvage). */ \
        while (*toCpy != EOS)                                           \
        {                                                               \
            dst[dstID] = *toCpy;                                        \
            dstID++;                                                    \
            toCpy++;                                                    \
        }                                                               \
        if (gBattleTypeFlags & BATTLE_TYPE_TRAINER)                     \
            toCpy = sText_FoePkmnPrefix;                                \
        else                                                            \
            toCpy = sText_WildPkmnPrefix;                               \
    }

static const u8 *BattleStringGetOpponentNameByTrainerId(u16 trainerId, u8 *text, u8 multiplayerId, enum BattlerId battler)
{
    const u8 *toCpy = NULL;

    if (gBattleTypeFlags & BATTLE_TYPE_SECRET_BASE)
    {
        u32 i;
        for (i = 0; i < ARRAY_COUNT(gBattleResources->secretBase->trainerName); i++)
            text[i] = gBattleResources->secretBase->trainerName[i];
        text[i] = EOS;
        ConvertInternationalString(text, gBattleResources->secretBase->language);
        toCpy = text;
    }
    else if (trainerId == TRAINER_UNION_ROOM)
    {
        toCpy = gLinkPlayers[multiplayerId ^ BIT_SIDE].name;
    }
    else if (trainerId == TRAINER_LINK_OPPONENT)
    {
        if (gBattleTypeFlags & BATTLE_TYPE_MULTI)
            toCpy = gLinkPlayers[GetBattlerMultiplayerId(battler)].name;
        else
            toCpy = gLinkPlayers[GetBattlerMultiplayerId(battler) & BIT_SIDE].name;
    }
    else if (trainerId == TRAINER_FRONTIER_BRAIN)
    {
        CopyFrontierBrainTrainerName(text);
        toCpy = text;
    }
    else if (gBattleTypeFlags & BATTLE_TYPE_FRONTIER)
    {
        GetFrontierTrainerName(text, trainerId);
        toCpy = text;
    }
    else if (gBattleTypeFlags & BATTLE_TYPE_TRAINER_TOWER && gMapHeader.regionMapSectionId == MAPSEC_TRAINER_TOWER_2)
    {
        GetTrainerTowerOpponentName(text);
        toCpy = text;
    }
    else if (gBattleTypeFlags & BATTLE_TYPE_TRAINER_HILL)
    {
        GetTrainerHillTrainerName(text, trainerId);
        toCpy = text;
    }
    else if (gBattleTypeFlags & BATTLE_TYPE_EREADER_TRAINER)
    {
        GetEreaderTrainerName(text);
        toCpy = text;
    }
    else
    {
        enum TrainerClassID trainerClass = GetTrainerClassFromId(TRAINER_BATTLE_PARAM.opponentA);

        if (trainerClass == TRAINER_CLASS_RIVAL_EARLY_FRLG || trainerClass == TRAINER_CLASS_RIVAL_LATE_FRLG || trainerClass == TRAINER_CLASS_CHAMPION_FRLG)
            toCpy = GetExpandedPlaceholder(PLACEHOLDER_ID_RIVAL);
        else
            toCpy = GetTrainerNameFromId(trainerId);
    }

    assertf(DoesStringProperlyTerminate(toCpy, TRAINER_NAME_LENGTH + 1),"Opponent needs a valid name")
    {
        return sText_EmptyString4;
    }

    return toCpy;
}

static const u8 *BattleStringGetOpponentName(u8 *text, u8 multiplayerId, enum BattlerId battler)
{
    const u8 *toCpy = NULL;

    switch (GetBattlerPosition(battler))
    {
    case B_POSITION_OPPONENT_LEFT:
        toCpy = BattleStringGetOpponentNameByTrainerId(TRAINER_BATTLE_PARAM.opponentA, text, multiplayerId, battler);
        break;
    case B_POSITION_OPPONENT_RIGHT:
        if (gBattleTypeFlags & (BATTLE_TYPE_TWO_OPPONENTS | BATTLE_TYPE_MULTI) && !BATTLE_TWO_VS_ONE_OPPONENT)
            toCpy = BattleStringGetOpponentNameByTrainerId(TRAINER_BATTLE_PARAM.opponentB, text, multiplayerId, battler);
        else
            toCpy = BattleStringGetOpponentNameByTrainerId(TRAINER_BATTLE_PARAM.opponentA, text, multiplayerId, battler);
        break;
    default:
        break;
    }

    return toCpy;
}

static const u8 *BattleStringGetPlayerName(u8 *text, enum BattlerId battler)
{
    const u8 *toCpy = NULL;

    switch (GetBattlerPosition(battler))
    {
    case B_POSITION_PLAYER_LEFT:
        if (gBattleTypeFlags & BATTLE_TYPE_RECORDED)
            toCpy = gLinkPlayers[0].name;
        else
            toCpy = gSaveBlock2Ptr->playerName;
        break;
    case B_POSITION_PLAYER_RIGHT:
        if (((gBattleTypeFlags & BATTLE_TYPE_RECORDED) && !(gBattleTypeFlags & (BATTLE_TYPE_MULTI | BATTLE_TYPE_INGAME_PARTNER)))
            || gTestRunnerEnabled)
        {
            toCpy = gLinkPlayers[0].name;
        }
        else if ((gBattleTypeFlags & BATTLE_TYPE_LINK) && gBattleTypeFlags & (BATTLE_TYPE_RECORDED | BATTLE_TYPE_MULTI))
        {
            toCpy = gLinkPlayers[2].name;
        }
        else if (gBattleTypeFlags & BATTLE_TYPE_INGAME_PARTNER)
        {
            GetFrontierTrainerName(text, gPartnerTrainerId);
            toCpy = text;
        }
        else
        {
            toCpy = gSaveBlock2Ptr->playerName;
        }
        break;
    default:
        break;
    }

    return toCpy;
}

static const u8 *BattleStringGetTrainerName(u8 *text, u8 multiplayerId, enum BattlerId battler)
{
    if (IsOnPlayerSide(battler))
        return BattleStringGetPlayerName(text, battler);
    else
        return BattleStringGetOpponentName(text, multiplayerId, battler);
}

static const u8 *BattleStringGetOpponentClassByTrainerId(u16 trainerId)
{
    const u8 *toCpy;

    if (gBattleTypeFlags & BATTLE_TYPE_SECRET_BASE)
        toCpy = gTrainerClasses[GetSecretBaseTrainerClass()].name;
    else if (trainerId == TRAINER_UNION_ROOM)
        toCpy = gTrainerClasses[GetUnionRoomTrainerClass()].name;
    else if (trainerId == TRAINER_FRONTIER_BRAIN)
        toCpy = gTrainerClasses[GetFrontierBrainTrainerClass()].name;
    else if (gBattleTypeFlags & BATTLE_TYPE_FRONTIER)
        toCpy = gTrainerClasses[GetFrontierOpponentClass(trainerId)].name;
    else if (gBattleTypeFlags & BATTLE_TYPE_TRAINER_TOWER && gMapHeader.regionMapSectionId == MAPSEC_TRAINER_TOWER_2)
        toCpy = gTrainerClasses[GetTrainerTowerOpponentClass()].name;
    else if (gBattleTypeFlags & BATTLE_TYPE_TRAINER_HILL)
        toCpy = gTrainerClasses[GetTrainerHillOpponentClass(trainerId)].name;
    else if (gBattleTypeFlags & BATTLE_TYPE_EREADER_TRAINER)
        toCpy = gTrainerClasses[GetEreaderTrainerClassId()].name;
    else if (trainerId == TRAINER_LINK_OPPONENT)
        toCpy = gTrainerClasses[TRAINER_NONE].name;
    else
        toCpy = gTrainerClasses[GetTrainerClassFromId(trainerId)].name;

    return toCpy;
}

// Ensure the defined length for an item name can contain the full defined length of a berry name.
// This ensures that custom Enigma Berry names will fit in the text buffer at the top of BattleStringExpandPlaceholders.
STATIC_ASSERT(BERRY_NAME_LENGTH + ARRAY_COUNT(sText_BerrySuffix) <= ITEM_NAME_LENGTH, BerryNameTooLong);

u32 BattleStringExpandPlaceholders(const u8 *src, u8 *dst, u32 dstSize)
{
    u32 dstID = 0; // if they used dstID, why not use srcID as well?
    const u8 *toCpy = NULL;
    u8 text[max(max(max(32, TRAINER_NAME_LENGTH + 1), POKEMON_NAME_LENGTH + 1), ITEM_NAME_LENGTH)];
    u8 *textStart = &text[0];
    u8 multiplayerId;
    u8 fontId = FONT_NORMAL;

    if (gBattleTypeFlags & BATTLE_TYPE_RECORDED_LINK)
        multiplayerId = gRecordedBattleMultiplayerId;
    else
        multiplayerId = GetMultiplayerId();

    // Clear destination first
    while (dstID < dstSize)
    {
        dst[dstID] = EOS;
        dstID++;
    }

    dstID = 0;
    while (*src != EOS)
    {
        toCpy = NULL;

        if (*src == PLACEHOLDER_BEGIN)
        {
            src++;
            u32 classLength = 0;
            u32 nameLength = 0;
            const u8 *classString;
            const u8 *nameString;
            switch (*src)
            {
            case B_TXT_BUFF1:
                if (gBattleTextBuff1[0] == B_BUFF_PLACEHOLDER_BEGIN)
                {
                    ExpandBattleTextBuffPlaceholders(gBattleTextBuff1, gStringVar1);
                    toCpy = gStringVar1;
                }
                else
                {
                    toCpy = TryGetStatusString(gBattleTextBuff1);
                    if (toCpy == NULL)
                        toCpy = gBattleTextBuff1;
                }
                break;
            case B_TXT_BUFF2:
                if (gBattleTextBuff2[0] == B_BUFF_PLACEHOLDER_BEGIN)
                {
                    ExpandBattleTextBuffPlaceholders(gBattleTextBuff2, gStringVar2);
                    toCpy = gStringVar2;
                }
                else
                {
                    toCpy = gBattleTextBuff2;
                }
                break;
            case B_TXT_BUFF3:
                if (gBattleTextBuff3[0] == B_BUFF_PLACEHOLDER_BEGIN)
                {
                    ExpandBattleTextBuffPlaceholders(gBattleTextBuff3, gStringVar3);
                    toCpy = gStringVar3;
                }
                else
                {
                    toCpy = gBattleTextBuff3;
                }
                break;
            case B_TXT_COPY_VAR_1:
                toCpy = gStringVar1;
                break;
            case B_TXT_COPY_VAR_2:
                toCpy = gStringVar2;
                break;
            case B_TXT_COPY_VAR_3:
                toCpy = gStringVar3;
                break;
            case B_TXT_PLAYER_MON1_NAME: // first player poke name
                GetBattlerNick(GetBattlerAtPosition(B_POSITION_PLAYER_LEFT), text);
                toCpy = text;
                break;
            case B_TXT_OPPONENT_MON1_NAME: // first enemy poke name
                GetBattlerNick(GetBattlerAtPosition(B_POSITION_OPPONENT_LEFT), text);
                toCpy = text;
                break;
            case B_TXT_PLAYER_MON2_NAME: // second player poke name
                GetBattlerNick(GetBattlerAtPosition(B_POSITION_PLAYER_RIGHT), text);
                toCpy = text;
                break;
            case B_TXT_OPPONENT_MON2_NAME: // second enemy poke name
                GetBattlerNick(GetBattlerAtPosition(B_POSITION_OPPONENT_RIGHT), text);
                toCpy = text;
                break;
            case B_TXT_LINK_PLAYER_MON1_NAME: // link first player poke name
                GetBattlerNick(GetBattlerAtPosition(B_POSITION_PLAYER_LEFT), text);
                toCpy = text;
                break;
            case B_TXT_LINK_OPPONENT_MON1_NAME: // link first opponent poke name
                GetBattlerNick(GetBattlerAtPosition(B_POSITION_OPPONENT_LEFT), text);
                toCpy = text;
                break;
            case B_TXT_LINK_PLAYER_MON2_NAME: // link second player poke name
                GetBattlerNick(GetBattlerAtPosition(B_POSITION_PLAYER_RIGHT), text);
                toCpy = text;
                break;
            case B_TXT_LINK_OPPONENT_MON2_NAME: // link second opponent poke name
                GetBattlerNick(GetBattlerAtPosition(B_POSITION_OPPONENT_RIGHT), text);
                toCpy = text;
                break;
            case B_TXT_ATK_NAME_WITH_PREFIX_MON1: // Unused, to change into sth else.
                break;
            case B_TXT_ATK_PARTNER_NAME: // attacker partner name
                GetBattlerNick(GetPartnerBattler(gBattlerAttacker), text);
                toCpy = text;
                break;
            case B_TXT_ATK_NAME_WITH_PREFIX: // attacker name with prefix
                HANDLE_NICKNAME_STRING_CASE(gBattlerAttacker)
                break;
            case B_TXT_DEF_NAME_WITH_PREFIX: // target name with prefix
                HANDLE_NICKNAME_STRING_CASE(gBattlerTarget)
                break;
            case B_TXT_DEF_NAME: // target name
                GetBattlerNick(gBattlerTarget, text);
                toCpy = text;
                break;
            case B_TXT_DEF_PARTNER_NAME: // partner target name
                GetBattlerNick(GetPartnerBattler(gBattlerTarget), text);
                toCpy = text;
                break;
            case B_TXT_EFF_NAME_WITH_PREFIX: // effect battler name with prefix
                HANDLE_NICKNAME_STRING_CASE(gEffectBattler)
                break;
            case B_TXT_SCR_ACTIVE_NAME_WITH_PREFIX: // scripting active battler name with prefix
                HANDLE_NICKNAME_STRING_CASE(gBattleScripting.battler)
                break;
            case B_TXT_CURRENT_MOVE: // current move name
                if (gBattleMsgDataPtr->currentMove >= MOVES_COUNT
                 && !IsZMove(gBattleMsgDataPtr->currentMove)
                 && !IsMaxMove(gBattleMsgDataPtr->currentMove))
                    toCpy = gTypesInfo[gBattleStruct->stringMoveType].generic;
                else
                    toCpy = GetMoveName(gBattleMsgDataPtr->currentMove);
                break;
            case B_TXT_LAST_MOVE: // originally used move name
                if (gBattleMsgDataPtr->originallyUsedMove >= MOVES_COUNT
                 && !IsZMove(gBattleMsgDataPtr->currentMove)
                 && !IsMaxMove(gBattleMsgDataPtr->currentMove))
                    toCpy = gTypesInfo[gBattleStruct->stringMoveType].generic;
                else
                    toCpy = GetMoveName(gBattleMsgDataPtr->originallyUsedMove);
                break;
            case B_TXT_LAST_ITEM: // last used item
                if (gBattleTypeFlags & (BATTLE_TYPE_LINK | BATTLE_TYPE_RECORDED_LINK))
                {
                    if (gLastUsedItem == ITEM_ENIGMA_BERRY_E_READER)
                    {
                        if (!(gBattleTypeFlags & BATTLE_TYPE_MULTI))
                        {
                            if ((gBattleScripting.multiplayerId != 0 && (gPotentialItemEffectBattler & BIT_SIDE))
                                || (gBattleScripting.multiplayerId == 0 && !(gPotentialItemEffectBattler & BIT_SIDE)))
                            {
                                StringCopy(text, gEnigmaBerries[gPotentialItemEffectBattler].name);
                                StringAppend(text, sText_BerrySuffix);
                                toCpy = text;
                            }
                            else
                            {
                                toCpy = sText_EnigmaBerry;
                            }
                        }
                        else
                        {
                            if (gLinkPlayers[gBattleScripting.multiplayerId].id == gPotentialItemEffectBattler)
                            {
                                StringCopy(text, gEnigmaBerries[gPotentialItemEffectBattler].name);
                                StringAppend(text, sText_BerrySuffix);
                                toCpy = text;
                            }
                            else
                            {
                                toCpy = sText_EnigmaBerry;
                            }
                        }
                    }
                    else
                    {
                        CopyItemName(gLastUsedItem, text);
                        toCpy = text;
                    }
                }
                else
                {
                    CopyItemName(gLastUsedItem, text);
                    toCpy = text;
                }
                break;
            case B_TXT_LAST_ABILITY: // last used ability
                toCpy = gAbilitiesInfo[gLastUsedAbility].name;
                break;
            case B_TXT_ATK_ABILITY: // attacker ability
                toCpy = gAbilitiesInfo[sBattlerAbilities[gBattlerAttacker]].name;
                break;
            case B_TXT_DEF_ABILITY: // target ability
                toCpy = gAbilitiesInfo[sBattlerAbilities[gBattlerTarget]].name;
                break;
            case B_TXT_SCR_ACTIVE_ABILITY: // scripting active ability
                toCpy = gAbilitiesInfo[sBattlerAbilities[gBattleScripting.battler]].name;
                break;
            case B_TXT_EFF_ABILITY: // effect battler ability
                toCpy = gAbilitiesInfo[sBattlerAbilities[gEffectBattler]].name;
                break;
            case B_TXT_TRAINER1_CLASS: // trainer class name
                toCpy = BattleStringGetOpponentClassByTrainerId(TRAINER_BATTLE_PARAM.opponentA);
                break;
            case B_TXT_TRAINER1_NAME: // trainer1 name
                toCpy = BattleStringGetOpponentNameByTrainerId(TRAINER_BATTLE_PARAM.opponentA, text, multiplayerId, GetBattlerAtPosition(B_POSITION_OPPONENT_LEFT));
                break;
            case B_TXT_TRAINER1_NAME_WITH_CLASS: // trainer1 name with trainer class
                toCpy = textStart;
                classString = BattleStringGetOpponentClassByTrainerId(TRAINER_BATTLE_PARAM.opponentA);
                while (classString[classLength] != EOS)
                {
                    textStart[classLength] = classString[classLength];
                    classLength++;
                }
                textStart[classLength] = CHAR_SPACE;
                textStart += classLength + 1;
                nameString = BattleStringGetOpponentNameByTrainerId(TRAINER_BATTLE_PARAM.opponentA, textStart, multiplayerId, GetBattlerAtPosition(B_POSITION_OPPONENT_LEFT));
                if (nameString != textStart)
                {
                    while (nameString[nameLength] != EOS)
                    {
                        textStart[nameLength] = nameString[nameLength];
                        nameLength++;
                    }
                    textStart[nameLength] = EOS;
                }
                break;
            case B_TXT_LINK_PLAYER_NAME: // link player name
                toCpy = gLinkPlayers[multiplayerId].name;
                break;
            case B_TXT_LINK_PARTNER_NAME: // link partner name
                toCpy = gLinkPlayers[GetBattlerMultiplayerId(GetPartnerPosition(gLinkPlayers[multiplayerId].id))].name;
                break;
            case B_TXT_LINK_OPPONENT1_NAME: // link opponent 1 name
                toCpy = gLinkPlayers[GetBattlerMultiplayerId(GetBattlerLeftFoe(gLinkPlayers[multiplayerId].id))].name;
                break;
            case B_TXT_LINK_OPPONENT2_NAME: // link opponent 2 name
                toCpy = gLinkPlayers[GetBattlerMultiplayerId(GetBattlerRightFoe(gLinkPlayers[multiplayerId].id))].name;
                break;
            case B_TXT_LINK_SCR_TRAINER_NAME: // link scripting active name
                toCpy = gLinkPlayers[GetBattlerMultiplayerId(gBattleScripting.battler)].name;
                break;
            case B_TXT_PLAYER_NAME: // player name
                toCpy = BattleStringGetPlayerName(text, GetBattlerAtPosition(B_POSITION_PLAYER_LEFT));
                break;
            case B_TXT_TRAINER1_LOSE_TEXT: // trainerA lose text
                if (gBattleTypeFlags & BATTLE_TYPE_FRONTIER)
                {
                    CopyFrontierTrainerText(FRONTIER_PLAYER_WON_TEXT, TRAINER_BATTLE_PARAM.opponentA);
                    toCpy = gStringVar4;
                }
                else if (gBattleTypeFlags & BATTLE_TYPE_TRAINER_TOWER && gMapHeader.regionMapSectionId == MAPSEC_TRAINER_TOWER_2)
                {
                    GetTrainerTowerOpponentLoseText(gStringVar4, 0);
                    toCpy = gStringVar4;
                }
                else if (gBattleTypeFlags & BATTLE_TYPE_TRAINER_HILL)
                {
                    CopyTrainerHillTrainerText(TRAINER_HILL_TEXT_PLAYER_WON, TRAINER_BATTLE_PARAM.opponentA);
                    toCpy = gStringVar4;
                }
                else
                {
                    toCpy = GetTrainerALoseText();
                }
                break;
            case B_TXT_TRAINER1_WIN_TEXT: // trainerA win text
                if (gBattleTypeFlags & BATTLE_TYPE_FRONTIER)
                {
                    CopyFrontierTrainerText(FRONTIER_PLAYER_LOST_TEXT, TRAINER_BATTLE_PARAM.opponentA);
                    toCpy = gStringVar4;
                }
                else if (gBattleTypeFlags & BATTLE_TYPE_TRAINER_TOWER && gMapHeader.regionMapSectionId == MAPSEC_TRAINER_TOWER_2)
                {
                    GetTrainerTowerOpponentWinText(gStringVar4, 0);
                    toCpy = gStringVar4;
                }
                else if (gBattleTypeFlags & BATTLE_TYPE_TRAINER_HILL)
                {
                    CopyTrainerHillTrainerText(TRAINER_HILL_TEXT_PLAYER_LOST, TRAINER_BATTLE_PARAM.opponentA);
                    toCpy = gStringVar4;
                }
                else
                {
                    toCpy = GetTrainerWonSpeech();
                }
                break;
            case B_TXT_26: // ?
                GetMonData(&GetBattlerParty(gBattleScripting.battler)[gBattleStruct->scriptPartyIdx], MON_DATA_NICKNAME, text);
                StringGet_Nickname(text);
                toCpy = text;
                if (!IsOnPlayerSide(gBattleScripting.battler))
                {
                    while (*toCpy != EOS)
                    {
                        dst[dstID] = *toCpy;
                        dstID++;
                        toCpy++;
                    }
                    if (gBattleTypeFlags & BATTLE_TYPE_TRAINER)
                        toCpy = sText_FoePkmnPrefix;
                    else
                        toCpy = sText_WildPkmnPrefix;
                }
                break;
            case B_TXT_PC_CREATOR_NAME: // lanette pc
                if (FlagGet(FLAG_SYS_PC_LANETTE))
                    toCpy = IS_FRLG ? sText_Bills : sText_Lanettes;
                else
                    toCpy = sText_Someones;
                break;
            case B_TXT_ATK_PREFIX2:
                if (IsOnPlayerSide(gBattlerAttacker))
                    toCpy = sText_AllyPkmnPrefix2;
                else
                    toCpy = sText_FoePkmnPrefix3;
                break;
            case B_TXT_DEF_PREFIX2:
                if (IsOnPlayerSide(gBattlerTarget))
                    toCpy = sText_AllyPkmnPrefix2;
                else
                    toCpy = sText_FoePkmnPrefix3;
                break;
            case B_TXT_ATK_PREFIX1:
                if (IsOnPlayerSide(gBattlerAttacker))
                    toCpy = sText_AllyPkmnPrefix;
                else
                    toCpy = sText_FoePkmnPrefix2;
                break;
            case B_TXT_DEF_PREFIX1:
                if (IsOnPlayerSide(gBattlerTarget))
                    toCpy = sText_AllyPkmnPrefix;
                else
                    toCpy = sText_FoePkmnPrefix2;
                break;
            case B_TXT_ATK_PREFIX3:
                if (IsOnPlayerSide(gBattlerAttacker))
                    toCpy = sText_AllyPkmnPrefix3;
                else
                    toCpy = sText_FoePkmnPrefix4;
                break;
            case B_TXT_DEF_PREFIX3:
                if (IsOnPlayerSide(gBattlerTarget))
                    toCpy = sText_AllyPkmnPrefix3;
                else
                    toCpy = sText_FoePkmnPrefix4;
                break;
            case B_TXT_TRAINER2_CLASS:
                toCpy = BattleStringGetOpponentClassByTrainerId(TRAINER_BATTLE_PARAM.opponentB);
                break;
            case B_TXT_TRAINER2_NAME:
                toCpy = BattleStringGetOpponentNameByTrainerId(TRAINER_BATTLE_PARAM.opponentB, text, multiplayerId, GetBattlerAtPosition(B_POSITION_OPPONENT_RIGHT));
                break;
            case B_TXT_TRAINER2_NAME_WITH_CLASS:
                toCpy = textStart;
                classString = BattleStringGetOpponentClassByTrainerId(TRAINER_BATTLE_PARAM.opponentB);
                while (classString[classLength] != EOS)
                {
                    textStart[classLength] = classString[classLength];
                    classLength++;
                }
                textStart[classLength] = CHAR_SPACE;
                textStart += classLength + 1;
                nameString = BattleStringGetOpponentNameByTrainerId(TRAINER_BATTLE_PARAM.opponentB, textStart, multiplayerId, GetBattlerAtPosition(B_POSITION_OPPONENT_RIGHT));
                if (nameString != textStart)
                {
                    while (nameString[nameLength] != EOS)
                    {
                        textStart[nameLength] = nameString[nameLength];
                        nameLength++;
                    }
                    textStart[nameLength] = EOS;
                }
                break;
            case B_TXT_TRAINER2_LOSE_TEXT:
                if (gBattleTypeFlags & BATTLE_TYPE_FRONTIER)
                {
                    CopyFrontierTrainerText(FRONTIER_PLAYER_WON_TEXT, TRAINER_BATTLE_PARAM.opponentB);
                    toCpy = gStringVar4;
                }
                else if (gBattleTypeFlags & BATTLE_TYPE_TRAINER_TOWER && gMapHeader.regionMapSectionId == MAPSEC_TRAINER_TOWER_2)
                {
                    GetTrainerTowerOpponentLoseText(gStringVar4, 1);
                    toCpy = gStringVar4;
                }
                else if (gBattleTypeFlags & BATTLE_TYPE_TRAINER_HILL)
                {
                    CopyTrainerHillTrainerText(TRAINER_HILL_TEXT_PLAYER_WON, TRAINER_BATTLE_PARAM.opponentB);
                    toCpy = gStringVar4;
                }
                else
                {
                    toCpy = GetTrainerBLoseText();
                }
                break;
            case B_TXT_TRAINER2_WIN_TEXT:
                if (gBattleTypeFlags & BATTLE_TYPE_FRONTIER)
                {
                    CopyFrontierTrainerText(FRONTIER_PLAYER_LOST_TEXT, TRAINER_BATTLE_PARAM.opponentB);
                    toCpy = gStringVar4;
                }
                else if (gBattleTypeFlags & BATTLE_TYPE_TRAINER_TOWER && gMapHeader.regionMapSectionId == MAPSEC_TRAINER_TOWER_2)
                {
                    GetTrainerTowerOpponentWinText(gStringVar4, 1);
                    toCpy = gStringVar4;
                }
                else if (gBattleTypeFlags & BATTLE_TYPE_TRAINER_HILL)
                {
                    CopyTrainerHillTrainerText(TRAINER_HILL_TEXT_PLAYER_LOST, TRAINER_BATTLE_PARAM.opponentB);
                    toCpy = gStringVar4;
                }
                break;
            case B_TXT_PARTNER_CLASS:
                toCpy = gTrainerClasses[GetFrontierOpponentClass(gPartnerTrainerId)].name;
                break;
            case B_TXT_PARTNER_NAME:
                toCpy = BattleStringGetPlayerName(text, GetBattlerAtPosition(B_POSITION_PLAYER_RIGHT));
                break;
            case B_TXT_PARTNER_NAME_WITH_CLASS:
                toCpy = textStart;
                classString = gTrainerClasses[GetFrontierOpponentClass(gPartnerTrainerId)].name;
                while (classString[classLength] != EOS)
                {
                    textStart[classLength] = classString[classLength];
                    classLength++;
                }
                textStart[classLength] = CHAR_SPACE;
                textStart += classLength + 1;
                nameString = BattleStringGetPlayerName(textStart, GetBattlerAtPosition(B_POSITION_PLAYER_RIGHT));
                if (nameString != textStart)
                {
                    while (nameString[nameLength] != EOS)
                    {
                        textStart[nameLength] = nameString[nameLength];
                        nameLength++;
                    }
                    textStart[nameLength] = EOS;
                }
                break;
            case B_TXT_ATK_TRAINER_NAME:
                toCpy = BattleStringGetTrainerName(text, multiplayerId, gBattlerAttacker);
                break;
            case B_TXT_ATK_TRAINER_CLASS:
                switch (GetBattlerPosition(gBattlerAttacker))
                {
                case B_POSITION_PLAYER_RIGHT:
                    if (gBattleTypeFlags & BATTLE_TYPE_INGAME_PARTNER)
                        toCpy = gTrainerClasses[GetFrontierOpponentClass(gPartnerTrainerId)].name;
                    break;
                case B_POSITION_OPPONENT_LEFT:
                    toCpy = BattleStringGetOpponentClassByTrainerId(TRAINER_BATTLE_PARAM.opponentA);
                    break;
                case B_POSITION_OPPONENT_RIGHT:
                    if (gBattleTypeFlags & BATTLE_TYPE_TWO_OPPONENTS && !BATTLE_TWO_VS_ONE_OPPONENT)
                        toCpy = BattleStringGetOpponentClassByTrainerId(TRAINER_BATTLE_PARAM.opponentB);
                    else
                        toCpy = BattleStringGetOpponentClassByTrainerId(TRAINER_BATTLE_PARAM.opponentA);
                    break;
                default:
                    break;
                }
                break;
            case B_TXT_ATK_TRAINER_NAME_WITH_CLASS:
                toCpy = textStart;
                if (gBattleTypeFlags & BATTLE_TYPE_CATCH_TUTORIAL)
                {
                    if (IS_FRLG)
                        textStart = StringCopy(textStart, COMPOUND_STRING("The old man"));
                    else
                        textStart = StringCopy(textStart, COMPOUND_STRING("WALLY"));
                }
                else if (GetBattlerPosition(gBattlerAttacker) == B_POSITION_PLAYER_LEFT)
                {
                    textStart = StringCopy(textStart, BattleStringGetTrainerName(textStart, multiplayerId, gBattlerAttacker));
                }
                else
                {
                    classString = NULL;
                    switch (GetBattlerPosition(gBattlerAttacker))
                    {
                    case B_POSITION_PLAYER_RIGHT:
                        if (gBattleTypeFlags & BATTLE_TYPE_INGAME_PARTNER)
                            classString = gTrainerClasses[GetFrontierOpponentClass(gPartnerTrainerId)].name;
                        break;
                    case B_POSITION_OPPONENT_LEFT:
                        classString = BattleStringGetOpponentClassByTrainerId(TRAINER_BATTLE_PARAM.opponentA);
                        break;
                    case B_POSITION_OPPONENT_RIGHT:
                        if (gBattleTypeFlags & BATTLE_TYPE_TWO_OPPONENTS && !BATTLE_TWO_VS_ONE_OPPONENT)
                            classString = BattleStringGetOpponentClassByTrainerId(TRAINER_BATTLE_PARAM.opponentB);
                        else
                            classString = BattleStringGetOpponentClassByTrainerId(TRAINER_BATTLE_PARAM.opponentA);
                        break;
                    default:
                        break;
                    }
                    classLength = 0;
                    nameLength = 0;
                    while (classString[classLength] != EOS)
                    {
                        textStart[classLength] = classString[classLength];
                        classLength++;
                    }
                    textStart[classLength] = CHAR_SPACE;
                    textStart += 1 + classLength;
                    nameString = BattleStringGetTrainerName(textStart, multiplayerId, gBattlerAttacker);
                    if (nameString != textStart)
                    {
                        while (nameString[nameLength] != EOS)
                        {
                            textStart[nameLength] = nameString[nameLength];
                            nameLength++;
                        }
                        textStart[nameLength] = EOS;
                    }
                }
                break;
            case B_TXT_ATK_TEAM1:
                if (IsOnPlayerSide(gBattlerAttacker))
                    toCpy = sText_Your1;
                else
                    toCpy = sText_Opposing1;
                break;
            case B_TXT_ATK_TEAM2:
                if (IsOnPlayerSide(gBattlerAttacker))
                    toCpy = sText_Your2;
                else
                    toCpy = sText_Opposing2;
                break;
            case B_TXT_DEF_TEAM1:
                if (IsOnPlayerSide(gBattlerTarget))
                    toCpy = sText_Your1;
                else
                    toCpy = sText_Opposing1;
                break;
            case B_TXT_DEF_TEAM2:
                if (IsOnPlayerSide(gBattlerTarget))
                    toCpy = sText_Your2;
                else
                    toCpy = sText_Opposing2;
                break;
            case B_TXT_EFF_TEAM1:
                if (IsOnPlayerSide(gEffectBattler))
                    toCpy = sText_Your1;
                else
                    toCpy = sText_Opposing1;
                break;
            case B_TXT_EFF_TEAM2:
                if (IsOnPlayerSide(gEffectBattler))
                    toCpy = sText_Your2;
                else
                    toCpy = sText_Opposing2;
                break;
            case B_TXT_ATK_NAME_WITH_PREFIX2:
                HANDLE_NICKNAME_STRING_LOWERCASE(gBattlerAttacker)
                break;
            case B_TXT_DEF_NAME_WITH_PREFIX2:
                HANDLE_NICKNAME_STRING_LOWERCASE(gBattlerTarget)
                break;
            case B_TXT_EFF_NAME_WITH_PREFIX2:
                HANDLE_NICKNAME_STRING_LOWERCASE(gEffectBattler)
                break;
            case B_TXT_SCR_ACTIVE_NAME_WITH_PREFIX2:
                HANDLE_NICKNAME_STRING_LOWERCASE(gBattleScripting.battler)
                break;
            }

            if (toCpy != NULL)
            {
                while (*toCpy != EOS)
                {
                    if (*toCpy == CHAR_SPACE)
                        dst[dstID] = CHAR_NBSP;
                    else
                        dst[dstID] = *toCpy;
                    dstID++;
                    toCpy++;
                }
            }

            if (*src == B_TXT_TRAINER1_LOSE_TEXT || *src == B_TXT_TRAINER2_LOSE_TEXT
                || *src == B_TXT_TRAINER1_WIN_TEXT || *src == B_TXT_TRAINER2_WIN_TEXT)
            {
                dst[dstID] = EXT_CTRL_CODE_BEGIN;
                dstID++;
                dst[dstID] = EXT_CTRL_CODE_PAUSE_UNTIL_PRESS;
                dstID++;
            }
        }
        else
        {
            dst[dstID] = *src;
            dstID++;
        }
        src++;
    }

    dst[dstID] = *src;
    dstID++;

    BreakStringAutomatic(dst, BATTLE_MSG_MAX_WIDTH, BATTLE_MSG_MAX_LINES, fontId, SHOW_SCROLL_PROMPT);

    return dstID;
}

static void IllusionNickHack(enum BattlerId battler, u32 partyId, u8 *dst)
{
    u32 id = PARTY_SIZE;
    struct Pokemon *party = GetBattlerParty(battler);
    struct Pokemon *mon = &party[partyId], *partnerMon;

    if (GetMonAbility(mon) == ABILITY_ILLUSION)
    {
        if (IsBattlerAlive(GetPartnerBattler(battler)))
            partnerMon = GetBattlerMon(GetPartnerBattler(battler));
        else
            partnerMon = mon;

        id = GetIllusionMonPartyId(party, mon, partnerMon, battler);
    }

    if (id != PARTY_SIZE)
        GetMonData(&party[id], MON_DATA_NICKNAME, dst);
    else
        GetMonData(mon, MON_DATA_NICKNAME, dst);
}

void ExpandBattleTextBuffPlaceholders(const u8 *src, u8 *dst)
{
    u32 srcID = 1;
    u32 value = 0;
    u8 nickname[POKEMON_NAME_LENGTH + 1];
    u16 hword;

    *dst = EOS;
    while (src[srcID] != B_BUFF_EOS)
    {
        switch (src[srcID])
        {
        case B_BUFF_STRING: // battle string
            hword = T1_READ_16(&src[srcID + 1]);
            StringAppend(dst, gBattleStringsTable[hword]);
            srcID += 3;
            break;
        case B_BUFF_NUMBER: // int to string
            switch (src[srcID + 1])
            {
            case 1:
                value = src[srcID + 3];
                break;
            case 2:
                value = T1_READ_16(&src[srcID + 3]);
                break;
            case 4:
                value = T1_READ_32(&src[srcID + 3]);
                break;
            }
            ConvertIntToDecimalStringN(dst, value, STR_CONV_MODE_LEFT_ALIGN, src[srcID + 2]);
            srcID += src[srcID + 1] + 3;
            break;
        case B_BUFF_MOVE: // move name
            StringAppend(dst, GetMoveName(T1_READ_16(&src[srcID + 1])));
            srcID += 3;
            break;
        case B_BUFF_TYPE: // type name
            StringAppend(dst, gTypesInfo[src[srcID + 1]].name);
            srcID += 2;
            break;
        case B_BUFF_MON_NICK_WITH_PREFIX: // poke nick with prefix
        case B_BUFF_MON_NICK_WITH_PREFIX_LOWER: // poke nick with lowercase prefix
            if (!IsOnPlayerSide(src[srcID + 1]))
            {
                if (src[srcID] == B_BUFF_MON_NICK_WITH_PREFIX_LOWER)
                {
                    if (gBattleTypeFlags & BATTLE_TYPE_TRAINER)
                        StringAppend(dst, sText_FoePkmnPrefixLower);
                    else
                        StringAppend(dst, sText_WildPkmnPrefixLower);
                }
                else
                {
                    if (gBattleTypeFlags & BATTLE_TYPE_TRAINER)
                        StringAppend(dst, sText_FoePkmnPrefix);
                    else
                        StringAppend(dst, sText_WildPkmnPrefix);
                }
            }
            GetMonData(&GetBattlerParty(src[srcID + 1])[src[srcID + 2]], MON_DATA_NICKNAME, nickname);
            StringGet_Nickname(nickname);
            StringAppend(dst, nickname);
            srcID += 3;
            break;
        case B_BUFF_STAT: // stats
            StringAppend(dst, gStatNamesTable[src[srcID + 1]]);
            srcID += 2;
            break;
        case B_BUFF_SPECIES: // species name
            StringCopy(dst, GetSpeciesName(T1_READ_16(&src[srcID + 1])));
            srcID += 3;
            break;
        case B_BUFF_MON_NICK: // poke nick without prefix
            if (src[srcID + 2] == gBattlerPartyIndexes[src[srcID + 1]])
            {
                GetBattlerNick(src[srcID + 1], dst);
            }
            else if (gBattleScripting.illusionNickHack) // for STRINGID_ENEMYABOUTTOSWITCHPKMN
            {
                gBattleScripting.illusionNickHack = 0;
                IllusionNickHack(src[srcID + 1], src[srcID + 2], dst);
                StringGet_Nickname(dst);
            }
            else
            {
                GetMonData(&GetBattlerParty(src[srcID + 1])[src[srcID + 2]], MON_DATA_NICKNAME, dst);
                StringGet_Nickname(dst);
            }
            srcID += 3;
            break;
        case B_BUFF_NEGATIVE_FLAVOR: // flavor table
            StringAppend(dst, gPokeblockWasTooXStringTable[src[srcID + 1]]);
            srcID += 2;
            break;
        case B_BUFF_ABILITY: // ability names
            StringAppend(dst, gAbilitiesInfo[T1_READ_16(&src[srcID + 1])].name);
            srcID += 3;
            break;
        case B_BUFF_ITEM: // item name
            hword = T1_READ_16(&src[srcID + 1]);
            if (gBattleTypeFlags & (BATTLE_TYPE_LINK | BATTLE_TYPE_RECORDED_LINK))
            {
                if (hword == ITEM_ENIGMA_BERRY_E_READER)
                {
                    if (gLinkPlayers[gBattleScripting.multiplayerId].id == gPotentialItemEffectBattler)
                    {
                        StringCopy(dst, gEnigmaBerries[gPotentialItemEffectBattler].name);
                        StringAppend(dst, sText_BerrySuffix);
                    }
                    else
                    {
                        StringAppend(dst, sText_EnigmaBerry);
                    }
                }
                else
                {
                    CopyItemName(hword, dst);
                }
            }
            else
            {
                CopyItemName(hword, dst);
            }
            srcID += 3;
            break;
        }
    }
}

void BattlePutTextOnWindow(const u8 *text, u8 windowId)
{
    const struct BattleWindowText *textInfo = sBattleTextOnWindowsInfo[gBattleScripting.windowsType];
    bool32 copyToVram;
    struct TextPrinterTemplate printerTemplate;
    u8 speed;

    if (windowId & B_WIN_COPYTOVRAM)
    {
        windowId &= ~B_WIN_COPYTOVRAM;
        copyToVram = FALSE;
    }
    else
    {
        FillWindowPixelBuffer(windowId, textInfo[windowId].fillValue);
        copyToVram = TRUE;
    }

    printerTemplate.currentChar = text;
    printerTemplate.type = WINDOW_TEXT_PRINTER;
    printerTemplate.windowId = windowId;
    printerTemplate.fontId = textInfo[windowId].fontId;
    printerTemplate.x = textInfo[windowId].x;
    printerTemplate.y = textInfo[windowId].y;
    printerTemplate.currentX = printerTemplate.x;
    printerTemplate.currentY = printerTemplate.y;
    printerTemplate.letterSpacing = textInfo[windowId].letterSpacing;
    printerTemplate.lineSpacing = textInfo[windowId].lineSpacing;
    printerTemplate.color = textInfo[windowId].color;

    if (B_WIN_MOVE_NAME_1 <= windowId && windowId <= B_WIN_MOVE_NAME_4)
    {
        // We cannot check the actual width of the window because
        // B_WIN_MOVE_NAME_1 and B_WIN_MOVE_NAME_3 are 16 wide for
        // Z-move details.
        if (gBattleStruct->zmove.viewing && windowId == B_WIN_MOVE_NAME_1)
            printerTemplate.fontId = GetFontIdToFit(text, printerTemplate.fontId, printerTemplate.letterSpacing, 16 * TILE_WIDTH);
        else
            printerTemplate.fontId = GetFontIdToFit(text, printerTemplate.fontId, printerTemplate.letterSpacing, 8 * TILE_WIDTH);
    }

    if (printerTemplate.x == 0xFF)
    {
        u32 width = GetBattleWindowTemplatePixelWidth(gBattleScripting.windowsType, windowId);
        s32 alignX = GetStringCenterAlignXOffsetWithLetterSpacing(printerTemplate.fontId, printerTemplate.currentChar, width, printerTemplate.letterSpacing);
        printerTemplate.x = printerTemplate.currentX = alignX;
    }

    if (windowId == ARENA_WIN_JUDGMENT_TEXT || windowId == B_WIN_OAK_OLD_MAN)
        gTextFlags.useAlternateDownArrow = FALSE;
    else
        gTextFlags.useAlternateDownArrow = TRUE;

    if ((gBattleTypeFlags & (BATTLE_TYPE_LINK | BATTLE_TYPE_RECORDED)) || gTestRunnerEnabled || ((gBattleTypeFlags & BATTLE_TYPE_POKEDUDE) && windowId != B_WIN_OAK_OLD_MAN))
        gTextFlags.autoScroll = TRUE;
    else
        gTextFlags.autoScroll = FALSE;

    if (windowId == B_WIN_MSG || windowId == ARENA_WIN_JUDGMENT_TEXT || windowId == B_WIN_OAK_OLD_MAN)
    {
        if (gBattleTypeFlags & (BATTLE_TYPE_LINK | BATTLE_TYPE_RECORDED_LINK))
            speed = 1;
        else if (gBattleTypeFlags & BATTLE_TYPE_RECORDED)
            speed = sRecordedBattleTextSpeeds[GetTextSpeedInRecordedBattle()];
        else
            speed = GetPlayerTextSpeedDelay();

        gTextFlags.canABSpeedUpPrint = 1;
    }
    else
    {
        speed = textInfo[windowId].speed;
        gTextFlags.canABSpeedUpPrint = 0;
    }

    AddTextPrinter(&printerTemplate, speed, NULL);

    if (copyToVram)
    {
        PutWindowTilemap(windowId);
        CopyWindowToVram(windowId, COPYWIN_FULL);
    }
}

void SetPPNumbersPaletteInMoveSelection(enum BattlerId battler)
{
    struct ChooseMoveStruct *chooseMoveStruct = (struct ChooseMoveStruct *)(&gBattleResources->bufferA[battler][4]);
    const u16 *palPtr = gPPTextPalette;
    u8 var;

    if (!gBattleStruct->zmove.viewing)
        var = GetCurrentPPToMaxPPState(chooseMoveStruct->currentPP[gMoveSelectionCursor[battler]],
                         chooseMoveStruct->maxPP[gMoveSelectionCursor[battler]]);
    else
        var = 3;

    gPlttBufferUnfaded[BG_PLTT_ID(5) + 12] = palPtr[(var * 2) + 0];
    gPlttBufferUnfaded[BG_PLTT_ID(5) + 11] = palPtr[(var * 2) + 1];

    CpuCopy16(&gPlttBufferUnfaded[BG_PLTT_ID(5) + 12], &gPlttBufferFaded[BG_PLTT_ID(5) + 12], PLTT_SIZEOF(1));
    CpuCopy16(&gPlttBufferUnfaded[BG_PLTT_ID(5) + 11], &gPlttBufferFaded[BG_PLTT_ID(5) + 11], PLTT_SIZEOF(1));
}

u8 GetCurrentPPToMaxPPState(u8 currentPP, u8 maxPP)
{
    if (maxPP == currentPP)
    {
        return 3;
    }
    else if (maxPP <= 2)
    {
        if (currentPP > 1)
            return 3;
        else
            return 2 - currentPP;
    }
    else if (maxPP <= 7)
    {
        if (currentPP > 2)
            return 3;
        else
            return 2 - currentPP;
    }
    else
    {
        if (currentPP == 0)
            return 2;
        if (currentPP <= maxPP / 4)
            return 1;
        if (currentPP > maxPP / 2)
            return 3;
    }

    return 0;
}
