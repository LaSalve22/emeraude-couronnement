DewfordTown_Gym_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, DewfordTown_Gym_OnTransition
	.byte 0

DewfordTown_Gym_OnTransition:
	call DewfordTown_Gym_EventScript_SetFlashLevel
	end

DewfordTown_Gym_EventScript_SetFlashLevel::
	goto_if_defeated TRAINER_BRAWLY_1, DewfordTown_Gym_EventScript_SetLightsOn
	call DewfordTown_Gym_EventScript_CountTrainersDefeated
	copyvar VAR_0x8001, VAR_0x8000
	goto_if_eq VAR_0x8000, 0, DewfordTown_Gym_EventScript_SetFlashLevel7
	goto_if_eq VAR_0x8000, 1, DewfordTown_Gym_EventScript_SetFlashLevel6
	goto_if_eq VAR_0x8000, 2, DewfordTown_Gym_EventScript_SetFlashLevel5
	goto_if_eq VAR_0x8000, 3, DewfordTown_Gym_EventScript_SetFlashLevel4
	goto_if_eq VAR_0x8000, 4, DewfordTown_Gym_EventScript_SetFlashLevel3
	goto_if_eq VAR_0x8000, 5, DewfordTown_Gym_EventScript_SetFlashLevel2
	goto DewfordTown_Gym_EventScript_SetFlashLevel1

DewfordTown_Gym_EventScript_SetLightsOn::
	setflashlevel 0
	return

@ Brightest
DewfordTown_Gym_EventScript_SetFlashLevel1::
	setflashlevel 1
	return

DewfordTown_Gym_EventScript_SetFlashLevel2::
	setflashlevel 2
	return

DewfordTown_Gym_EventScript_SetFlashLevel3::
	setflashlevel 3
	return

DewfordTown_Gym_EventScript_SetFlashLevel4::
	setflashlevel 4
	return

DewfordTown_Gym_EventScript_SetFlashLevel5::
	setflashlevel 5
	return

DewfordTown_Gym_EventScript_SetFlashLevel6::
	setflashlevel 6
	return

@ Darkest
DewfordTown_Gym_EventScript_SetFlashLevel7::
	setflashlevel 7
	return

DewfordTown_Gym_EventScript_BrightenRoom::
	call DewfordTown_Gym_EventScript_CountTrainersDefeated
	nop1
	goto_if_eq VAR_0x8000, VAR_0x8001, DewfordTown_Gym_EventScript_NoLightChange
	copyvar VAR_0x8001, VAR_0x8000
	goto_if_eq VAR_0x8000, 1, DewfordTown_Gym_EventScript_AnimateFlash1Trainer
	goto_if_eq VAR_0x8000, 2, DewfordTown_Gym_EventScript_AnimateFlash2Trainers
	goto_if_eq VAR_0x8000, 3, DewfordTown_Gym_EventScript_AnimateFlash3Trainers
	goto_if_eq VAR_0x8000, 4, DewfordTown_Gym_EventScript_AnimateFlash4Trainers
	goto_if_eq VAR_0x8000, 5, DewfordTown_Gym_EventScript_AnimateFlash5Trainers
	goto_if_eq VAR_0x8000, 6, DewfordTown_Gym_EventScript_AnimateFlash6Trainers

DewfordTown_Gym_EventScript_NoLightChange::
	return

@ NOTE: A little confusingly, a larger animateflash param value is a smaller flash radius
DewfordTown_Gym_EventScript_AnimateFlash1Trainer::
	playse SE_SWITCH
	animateflash 6
	call DewfordTown_Gym_EventScript_SetFlashLevel
	return

DewfordTown_Gym_EventScript_AnimateFlash2Trainers::
	playse SE_SWITCH
	animateflash 5
	call DewfordTown_Gym_EventScript_SetFlashLevel
	return

DewfordTown_Gym_EventScript_AnimateFlash3Trainers::
	playse SE_SWITCH
	animateflash 4
	call DewfordTown_Gym_EventScript_SetFlashLevel
	return

DewfordTown_Gym_EventScript_AnimateFlash4Trainers::
	playse SE_SWITCH
	animateflash 3
	call DewfordTown_Gym_EventScript_SetFlashLevel
	return

DewfordTown_Gym_EventScript_AnimateFlash5Trainers::
	playse SE_SWITCH
	animateflash 2
	call DewfordTown_Gym_EventScript_SetFlashLevel
	return

DewfordTown_Gym_EventScript_AnimateFlash6Trainers::
	playse SE_SWITCH
	animateflash 1
	call DewfordTown_Gym_EventScript_SetFlashLevel
	return

DewfordTown_Gym_EventScript_AnimateFlashFullBrightness::
	playse SE_SWITCH
	animateflash 0
	call DewfordTown_Gym_EventScript_SetFlashLevel
	return

DewfordTown_Gym_EventScript_CountTrainersDefeated::
	setvar VAR_0x8000, 0
	goto_if_not_defeated TRAINER_TAKAO, DewfordTown_Gym_EventScript_CheckJocelyn
	addvar VAR_0x8000, 1
DewfordTown_Gym_EventScript_CheckJocelyn::
	goto_if_not_defeated TRAINER_JOCELYN, DewfordTown_Gym_EventScript_CheckLaura
	addvar VAR_0x8000, 1
DewfordTown_Gym_EventScript_CheckLaura::
	goto_if_not_defeated TRAINER_LAURA, DewfordTown_Gym_EventScript_CheckBrenden
	addvar VAR_0x8000, 1
DewfordTown_Gym_EventScript_CheckBrenden::
	goto_if_not_defeated TRAINER_BRENDEN, DewfordTown_Gym_EventScript_CheckCristian
	addvar VAR_0x8000, 1
DewfordTown_Gym_EventScript_CheckCristian::
	goto_if_not_defeated TRAINER_CRISTIAN, DewfordTown_Gym_EventScript_CheckLilith
	addvar VAR_0x8000, 1
DewfordTown_Gym_EventScript_CheckLilith::
	goto_if_not_defeated TRAINER_LILITH, DewfordTown_Gym_EventScript_StopCountingTrainers
	addvar VAR_0x8000, 1
DewfordTown_Gym_EventScript_StopCountingTrainers::
	return

DewfordTown_Gym_EventScript_Brawly::
	trainerbattle_single TRAINER_BRAWLY_1, DewfordTown_Gym_Text_BrawlyIntro, DewfordTown_Gym_Text_BrawlyDefeat, DewfordTown_Gym_EventScript_BrawlyDefeated, NO_MUSIC
	specialvar VAR_RESULT, ShouldTryRematchBattle
	goto_if_eq VAR_RESULT, TRUE, DewfordTown_Gym_EventScript_BrawlyRematch
	goto_if_unset FLAG_RECEIVED_TM_BULK_UP, DewfordTown_Gym_EventScript_GiveBulkUp2
	msgbox DewfordTown_Gym_Text_BrawlyPostBattle, MSGBOX_DEFAULT
	release
	end

DewfordTown_Gym_EventScript_BrawlyDefeated::
	call DewfordTown_Gym_EventScript_AnimateFlashFullBrightness
	message DewfordTown_Gym_Text_ReceivedKnuckleBadge
	waitmessage
	call Common_EventScript_PlayGymBadgeFanfare
	msgbox DewfordTown_Gym_Text_KnuckleBadgeInfoTakeThis, MSGBOX_DEFAULT
	setflag FLAG_DEFEATED_DEWFORD_GYM
	setflag FLAG_BADGE02_GET
	addvar VAR_PETALBURG_GYM_STATE, 1
	call_if_eq VAR_PETALBURG_GYM_STATE, 6, Common_EventScript_ReadyPetalburgGymForBattle
	setvar VAR_0x8008, 2
	call Common_EventScript_SetGymTrainers
	call DewfordTown_Gym_EventScript_GiveBulkUp
	call DewfordTown_Gym_EventScript_GiveTeraOrb
	closemessage
	delay 30
	playfanfare MUS_REGISTER_MATCH_CALL
	msgbox DewfordTown_Gym_Text_RegisteredBrawly, MSGBOX_DEFAULT
	waitfanfare
	closemessage
	delay 30
	setflag FLAG_ENABLE_BRAWLY_MATCH_CALL
	setvar VAR_ROXANNE_CALL_STEP_COUNTER, 0
	setflag FLAG_ENABLE_ROXANNE_FIRST_CALL
	release
	end

DewfordTown_Gym_EventScript_GiveBulkUp::
	giveitem ITEM_TM_BULK_UP
	goto_if_eq VAR_RESULT, 0, Common_EventScript_BagIsFull
	msgbox DewfordTown_Gym_Text_ExplainBulkUp, MSGBOX_DEFAULT
	setflag FLAG_RECEIVED_TM_BULK_UP
	return

@ Émeraude Couronnement : Bastien remet l'Orbe Téracristal (Téracristallisation débloquée après le 2e badge).
DewfordTown_Gym_EventScript_GiveTeraOrb::
	checkitem ITEM_TERA_ORB
	goto_if_eq VAR_RESULT, TRUE, DewfordTown_Gym_EventScript_AlreadyHasTeraOrb
	msgbox DewfordTown_Gym_Text_TakeTeraOrb, MSGBOX_DEFAULT
	giveitem ITEM_TERA_ORB
	setflag FLAG_TERA_ORB_CHARGED
	msgbox DewfordTown_Gym_Text_ExplainTeraOrb, MSGBOX_DEFAULT
	return

DewfordTown_Gym_EventScript_AlreadyHasTeraOrb::
	return

DewfordTown_Gym_Text_TakeTeraOrb:
	.string "Bastien : Attends, attends ! J'ai un\n"
	.string "dernier truc pour toi.\p"
	.string "Je l'ai trouvé en surfant au large,\n"
	.string "coincé dans un récif. Il brillait comme\l"
	.string "un soleil sous l'eau.\p"
	.string "Moi, je préfère mes poings. Toi, t'en\n"
	.string "feras meilleur usage.$"

DewfordTown_Gym_Text_ExplainTeraOrb:
	.string "Bastien : C'est un Orbe Téracristal. En\n"
	.string "combat, il fait briller ton Pokémon\l"
	.string "comme un cristal et change son type.\p"
	.string "Une fois par combat. Après, il faut le\n"
	.string "recharger dans un Centre Pokémon.\p"
	.string "Bien utilisé, ça retourne un combat\n"
	.string "perdu. Mal utilisé… bah, tu t'en mords\l"
	.string "les doigts. Comme moi.$"

DewfordTown_Gym_EventScript_GiveBulkUp2:
	giveitem ITEM_TM_BULK_UP
	goto_if_eq VAR_RESULT, 0, Common_EventScript_ShowBagIsFull
	msgbox DewfordTown_Gym_Text_ExplainBulkUp, MSGBOX_DEFAULT
	setflag FLAG_RECEIVED_TM_BULK_UP
	release
	end

DewfordTown_Gym_EventScript_BrawlyRematch::
	trainerbattle_rematch_double TRAINER_BRAWLY_1, DewfordTown_Gym_Text_BrawlyPreRematch, DewfordTown_Gym_Text_BrawlyRematchDefeat, DewfordTown_Gym_Text_BrawlyRematchNeedTwoMons
	msgbox DewfordTown_Gym_Text_BrawlyPostRematch, MSGBOX_AUTOCLOSE
	end

DewfordTown_Gym_EventScript_Takao::
	trainerbattle_single TRAINER_TAKAO, DewfordTown_Gym_Text_TakaoIntro, DewfordTown_Gym_Text_TakaoDefeat, DewfordTown_Gym_EventScript_TakaoBrightenRoom
	msgbox DewfordTown_Gym_Text_TakaoPostBattle, MSGBOX_AUTOCLOSE
	end

DewfordTown_Gym_EventScript_TakaoBrightenRoom::
	call DewfordTown_Gym_EventScript_BrightenRoom
	release
	end

DewfordTown_Gym_EventScript_Jocelyn::
	trainerbattle_single TRAINER_JOCELYN, DewfordTown_Gym_Text_JocelynIntro, DewfordTown_Gym_Text_JocelynDefeat, DewfordTown_Gym_EventScript_JocelynBrightenRoom
	msgbox DewfordTown_Gym_Text_JocelynPostBattle, MSGBOX_AUTOCLOSE
	end

DewfordTown_Gym_EventScript_JocelynBrightenRoom::
	call DewfordTown_Gym_EventScript_BrightenRoom
	release
	end

DewfordTown_Gym_EventScript_Laura::
	trainerbattle_single TRAINER_LAURA, DewfordTown_Gym_Text_LauraIntro, DewfordTown_Gym_Text_LauraDefeat, DewfordTown_Gym_EventScript_LauraBrightenRoom
	msgbox DewfordTown_Gym_Text_LauraPostBattle, MSGBOX_AUTOCLOSE
	end

DewfordTown_Gym_EventScript_LauraBrightenRoom::
	call DewfordTown_Gym_EventScript_BrightenRoom
	release
	end

DewfordTown_Gym_EventScript_Brenden::
	trainerbattle_single TRAINER_BRENDEN, DewfordTown_Gym_Text_BrendenIntro, DewfordTown_Gym_Text_BrendenDefeat, DewfordTown_Gym_EventScript_BrendenBrightenRoom
	msgbox DewfordTown_Gym_Text_BrendenPostBattle, MSGBOX_AUTOCLOSE
	end

DewfordTown_Gym_EventScript_BrendenBrightenRoom::
	call DewfordTown_Gym_EventScript_BrightenRoom
	release
	end

DewfordTown_Gym_EventScript_Cristian::
	trainerbattle_single TRAINER_CRISTIAN, DewfordTown_Gym_Text_CristianIntro, DewfordTown_Gym_Text_CristianDefeat, DewfordTown_Gym_EventScript_CristianBrightenRoom
	msgbox DewfordTown_Gym_Text_CristianPostBattle, MSGBOX_AUTOCLOSE
	end

DewfordTown_Gym_EventScript_CristianBrightenRoom::
	call DewfordTown_Gym_EventScript_BrightenRoom
	release
	end

DewfordTown_Gym_EventScript_Lilith::
	trainerbattle_single TRAINER_LILITH, DewfordTown_Gym_Text_LilithIntro, DewfordTown_Gym_Text_LilithDefeat, DewfordTown_Gym_EventScript_LilithBrightenRoom
	msgbox DewfordTown_Gym_Text_LilithPostBattle, MSGBOX_AUTOCLOSE
	end

DewfordTown_Gym_EventScript_LilithBrightenRoom::
	call DewfordTown_Gym_EventScript_BrightenRoom
	release
	end

DewfordTown_Gym_EventScript_GymGuide::
	lock
	faceplayer
	goto_if_set FLAG_DEFEATED_DEWFORD_GYM, DewfordTown_Gym_EventScript_GymGuidePostVictory
	msgbox DewfordTown_Gym_Text_GymGuideAdvice, MSGBOX_DEFAULT
	release
	end

DewfordTown_Gym_EventScript_GymGuidePostVictory::
	msgbox DewfordTown_Gym_Text_GymGuidePostVictory, MSGBOX_DEFAULT
	release
	end

DewfordTown_Gym_EventScript_LeftGymStatue::
	lockall
	goto_if_set FLAG_BADGE02_GET, DewfordTown_Gym_EventScript_GymStatueCertified
	goto DewfordTown_Gym_EventScript_GymStatue
	end

DewfordTown_Gym_EventScript_RightGymStatue::
	lockall
	goto_if_set FLAG_BADGE02_GET, DewfordTown_Gym_EventScript_GymStatueCertified
	goto DewfordTown_Gym_EventScript_GymStatue
	end

DewfordTown_Gym_EventScript_GymStatueCertified::
	msgbox DewfordTown_Gym_Text_GymStatueCertified, MSGBOX_DEFAULT
	releaseall
	end

DewfordTown_Gym_EventScript_GymStatue::
	msgbox DewfordTown_Gym_Text_GymStatue, MSGBOX_DEFAULT
	releaseall
	end

DewfordTown_Gym_Text_GymGuideAdvice:
	.string "Yo, futur Maître {PLAYER} ! Ça roule ?\p"
	.string "Bastien, le Champion de Myokara, utilise\n"
	.string "des Pokémon Combat.\p"
	.string "Y aller avec des Pokémon Normal, c'est\n"
	.string "chercher les baffes.\p"
	.string "Le Combat écrase le Normal. Fais gaffe à\n"
	.string "tes dents.\p"
	.string "Pire : l'Arène est noire comme le fond de\n"
	.string "l'océan.\p"
	.string "Elle s'éclaire à mesure que tu bats les\n"
	.string "Dresseurs. Ou que tu te prends des\l"
	.string "murs.\p"
	.string "Allez, fonce !$"

DewfordTown_Gym_Text_GymGuidePostVictory:
	.string "Waouh ! Il fait trop clair d'un coup !\p"
	.string "Ton avenir aussi, il brille !$"

DewfordTown_Gym_Text_TakaoIntro:
	.string "La force fait le droit ! Viens !$"

DewfordTown_Gym_Text_TakaoDefeat:
	.string "Pas assez de puissance…$"

DewfordTown_Gym_Text_TakaoPostBattle:
	.string "Ta technique a battu ma force. Respect.$"

DewfordTown_Gym_Text_JocelynIntro:
	.string "Pas besoin de déranger Bastien !\p"
	.string "Je vais t'écraser moi-même !$"

DewfordTown_Gym_Text_JocelynDefeat:
	.string "Mais… comment ? Comment j'ai pu perdre\n"
	.string "si facilement ?$"

DewfordTown_Gym_Text_JocelynPostBattle:
	.string "Je vais pas gâcher cette défaite. Elle\n"
	.string "va me servir de carburant.\p"
	.string "Un jour, je serai Championne d'Arène…\p"
	.string "Non. Je serai Maître de la Ligue !$"

DewfordTown_Gym_Text_LauraIntro:
	.string "Me snobe pas ! Si tu pleures après, c'est\n"
	.string "pas ma faute !$"

DewfordTown_Gym_Text_LauraDefeat:
	.string "Ouiiiin ! J'ai perdu !$"

DewfordTown_Gym_Text_LauraPostBattle:
	.string "Je me bats dans le noir pour endurcir\n"
	.string "mon cœur.\p"
	.string "Mais à cause du noir, j'ai jamais vu le\n"
	.string "visage de Bastien… Il paraît qu'il est\l"
	.string "beau.$"

DewfordTown_Gym_Text_LilithIntro:
	.string "Hu hu… Tu trébuches dans le noir ?$"

DewfordTown_Gym_Text_LilithDefeat:
	.string "Oh, toi… Je veux pas te connaître !$"

DewfordTown_Gym_Text_LilithPostBattle:
	.string "Tu mérites de te perdre et de retomber\n"
	.string "à l'entrée !$"

DewfordTown_Gym_Text_BrendenIntro:
	.string "Je vais te montrer le cran d'un marin !$"

DewfordTown_Gym_Text_BrendenDefeat:
	.string "Comment c'est arrivé ? Moi, perdre comme\n"
	.string "ça ?$"

DewfordTown_Gym_Text_BrendenPostBattle:
	.string "Ouais ! T'as mérité de voir notre\n"
	.string "Champion.$"

DewfordTown_Gym_Text_CristianIntro:
	.string "Si tu veux passer, faudra me passer\n"
	.string "dessus !$"

DewfordTown_Gym_Text_CristianDefeat:
	.string "Grrrr ! Complètement dépassé !$"

DewfordTown_Gym_Text_CristianPostBattle:
	.string "T'as gagné ! Va chercher ce Badge !$"

DewfordTown_Gym_Text_BrawlyIntro:
	.string "Moi, c'est Bastien ! Champion de Myokara\n"
	.string "!\p"
	.string "J'ai grandi dans les rouleaux de ces\n"
	.string "eaux, et je me suis forgé dans la grotte\l"
	.string "la plus noire de l'île !\p"
	.string "Une nuit, la mer m'a avalé. Trois minutes\n"
	.string "sous l'eau. Je suis revenu. Depuis, j'ai\l"
	.string "plus peur de rien.\p"
	.string "Tu veux me défier ? Montre-moi ce que\n"
	.string "t'as dans le ventre !$"

DewfordTown_Gym_Text_BrawlyDefeat:
	.string "Waouh ! T'as fait une vague bien plus\n"
	.string "grosse que prévu !\p"
	.string "Tu m'as noyé !\p"
	.string "OK, t'as gagné. Prends ce Badge !$"

DewfordTown_Gym_Text_ReceivedKnuckleBadge:
	.string "{PLAYER} reçoit le Badge Poing de la\n"
	.string "part de Bastien.$"

DewfordTown_Gym_Text_KnuckleBadgeInfoTakeThis:
	.string "Avec le Badge Poing, tous les Pokémon\n"
	.string "jusqu'au N.30 t'obéissent, même ceux\l"
	.string "obtenus par échange.\p"
	.string "Et tu peux utiliser la CS Flash hors\n"
	.string "combat.\p"
	.string "Tiens, prends aussi cette Capsule\n"
	.string "Technique.$"

DewfordTown_Gym_Text_ExplainBulkUp:
	.string "La CT08 contient Gonflette.\p"
	.string "Elle augmente l'Attaque et la Défense.\n"
	.string "Comme moi, le matin.\p"
	.string "… … … … … …$"

DewfordTown_Gym_Text_RegisteredBrawly:
	.string "Le Champion Bastien a été enregistré\n"
	.string "dans le PokéNav.$"

DewfordTown_Gym_Text_BrawlyPostBattle:
	.string "Ton talent, c'est une vague géante. Un\n"
	.string "jour, elle va tout emporter chez les\l"
	.string "Dresseurs !$"

DewfordTown_Gym_Text_GymStatue:
	.string "Arène Pokémon de Myokara$"

DewfordTown_Gym_Text_GymStatueCertified:
	.string "Arène Pokémon de Myokara\p"
	.string "Dresseurs reconnus par Bastien :\n"
	.string "{PLAYER}$"

DewfordTown_Gym_Text_BrawlyPreRematch:
	.string "Bastien : Une vague recule, mais elle\n"
	.string "revient toujours sur le rivage.\p"
	.string "Une vague géante comme toi… Je savais\n"
	.string "que tu reviendrais !\p"
	.string "Montre-moi tes progrès !$"

DewfordTown_Gym_Text_BrawlyRematchDefeat:
	.string "Waouh ! Noyé, encore !$"

DewfordTown_Gym_Text_BrawlyPostRematch:
	.string "Bastien : Le combat, c'est vaste et\n"
	.string "profond. On n'en voit jamais le fond.\l"
	.string "Comme la mer de Hoenn !$"

DewfordTown_Gym_Text_BrawlyRematchNeedTwoMons:
	.string "Bastien : Une vague recule, mais elle\n"
	.string "revient toujours sur le rivage.\p"
	.string "Une vague géante comme toi… Je savais\n"
	.string "que tu reviendrais !\p"
	.string "Oh ? T'as qu'un seul Pokémon en état de\n"
	.string "se battre ?\p"
	.string "Désolé, reviens avec au moins deux\n"
	.string "Pokémon.$"
