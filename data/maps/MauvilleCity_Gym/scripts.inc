MauvilleCity_Gym_MapScripts::
	map_script MAP_SCRIPT_ON_LOAD, MauvilleCity_Gym_OnLoad
	.byte 0

MauvilleCity_Gym_OnLoad:
	goto_if_set FLAG_DEFEATED_MAUVILLE_GYM, MauvilleCity_Gym_EventScript_DeactivatePuzzle
	switch VAR_MAUVILLE_GYM_STATE
	case 0, MauvilleCity_Gym_EventScript_UpdateBarriers
	case 1, MauvilleCity_Gym_EventScript_Switch1Pressed
	case 2, MauvilleCity_Gym_EventScript_Switch2Pressed
	case 3, MauvilleCity_Gym_EventScript_Switch3Pressed
	case 4, MauvilleCity_Gym_EventScript_Switch4Pressed
	end

MauvilleCity_Gym_EventScript_UpdateBarriers::
	goto_if_set FLAG_MAUVILLE_GYM_BARRIERS_STATE, MauvilleCity_Gym_EventScript_SetAltBarriers
	end

MauvilleCity_Gym_EventScript_SetAltBarriers::
	setmetatile 3, 11, METATILE_MauvilleGym_RedBeamV1_On, TRUE
	setmetatile 3, 12, METATILE_MauvilleGym_RedBeamV2_On, TRUE
	setmetatile 3, 13, METATILE_MauvilleGym_PoleTop_On, TRUE
	setmetatile 4, 10, METATILE_MauvilleGym_RedBeamH1_On, FALSE
	setmetatile 5, 10, METATILE_MauvilleGym_RedBeamH2_On, FALSE
	setmetatile 4, 11, METATILE_MauvilleGym_RedBeamH3_On, TRUE
	setmetatile 5, 11, METATILE_MauvilleGym_RedBeamH4_On, TRUE
	setmetatile 7, 10, METATILE_MauvilleGym_RedBeamH1_On, FALSE
	setmetatile 8, 10, METATILE_MauvilleGym_RedBeamH2_On, FALSE
	setmetatile 7, 11, METATILE_MauvilleGym_RedBeamH3_On, TRUE
	setmetatile 8, 11, METATILE_MauvilleGym_RedBeamH4_On, TRUE
	setmetatile 4, 13, METATILE_MauvilleGym_GreenBeamH1_Off, FALSE
	setmetatile 5, 13, METATILE_MauvilleGym_GreenBeamH2_Off, FALSE
	setmetatile 4, 14, METATILE_MauvilleGym_GreenBeamH3_Off, FALSE
	setmetatile 5, 14, METATILE_MauvilleGym_GreenBeamH4_Off, FALSE
	setmetatile 1, 10, METATILE_MauvilleGym_GreenBeamH1_Off, FALSE
	setmetatile 2, 10, METATILE_MauvilleGym_GreenBeamH2_Off, FALSE
	setmetatile 1, 11, METATILE_MauvilleGym_GreenBeamH3_Off, FALSE
	setmetatile 2, 11, METATILE_MauvilleGym_GreenBeamH4_Off, FALSE
	setmetatile 6, 8, METATILE_MauvilleGym_PoleBottom_On, TRUE
	setmetatile 6, 9, METATILE_MauvilleGym_FloorTile, FALSE
	setmetatile 6, 10, METATILE_MauvilleGym_PoleTop_Off, FALSE
	setmetatile 4, 6, METATILE_MauvilleGym_GreenBeamH1_Off, FALSE
	setmetatile 5, 6, METATILE_MauvilleGym_GreenBeamH2_Off, FALSE
	setmetatile 4, 7, METATILE_MauvilleGym_GreenBeamH3_Off, FALSE
	setmetatile 5, 7, METATILE_MauvilleGym_GreenBeamH4_Off, FALSE
	end

MauvilleCity_Gym_EventScript_Switch1Pressed::
	setvar VAR_0x8004, 0
	special MauvilleGymPressSwitch
	goto MauvilleCity_Gym_EventScript_UpdateBarriers
	end

MauvilleCity_Gym_EventScript_Switch2Pressed::
	setvar VAR_0x8004, 1
	special MauvilleGymPressSwitch
	goto MauvilleCity_Gym_EventScript_UpdateBarriers
	end

MauvilleCity_Gym_EventScript_Switch3Pressed::
	setvar VAR_0x8004, 2
	special MauvilleGymPressSwitch
	goto MauvilleCity_Gym_EventScript_UpdateBarriers
	end

MauvilleCity_Gym_EventScript_Switch4Pressed::
	setvar VAR_0x8004, 3
	special MauvilleGymPressSwitch
	goto MauvilleCity_Gym_EventScript_UpdateBarriers
	end

MauvilleCity_Gym_EventScript_DeactivatePuzzle::
	special MauvilleGymDeactivatePuzzle
	end

MauvilleCity_Gym_EventScript_Wattson::
	trainerbattle_single TRAINER_WATTSON_1, MauvilleCity_Gym_Text_WattsonIntro, MauvilleCity_Gym_Text_WattsonDefeat, MauvilleCity_Gym_EventScript_WattsonDefeated, NO_MUSIC
	specialvar VAR_RESULT, ShouldTryRematchBattle
	goto_if_eq VAR_RESULT, TRUE, MauvilleCity_Gym_EventScript_WattsonRematch
	goto_if_unset FLAG_RECEIVED_TM_SHOCK_WAVE, MauvilleCity_Gym_EventScript_GiveShockWave2
	goto_if_eq VAR_NEW_MAUVILLE_STATE, 2, MauvilleCity_Gym_EventScript_CompletedNewMauville
	msgbox MauvilleCity_Gym_Text_WattsonPostBattle, MSGBOX_DEFAULT
	release
	end

MauvilleCity_Gym_EventScript_WattsonDefeated::
	message MauvilleCity_Gym_Text_ReceivedDynamoBadge
	waitmessage
	call Common_EventScript_PlayGymBadgeFanfare
	msgbox MauvilleCity_Gym_Text_ExplainDynamoBadgeTakeThis, MSGBOX_DEFAULT
	setvar VAR_SLATEPORT_OUTSIDE_MUSEUM_STATE, 3
	clearflag FLAG_HIDE_VERDANTURF_TOWN_SCOTT
	setflag FLAG_DEFEATED_MAUVILLE_GYM
	setflag FLAG_BADGE03_GET
	addvar VAR_PETALBURG_GYM_STATE, 1
	call_if_eq VAR_PETALBURG_GYM_STATE, 6, Common_EventScript_ReadyPetalburgGymForBattle
	setvar VAR_0x8008, 3
	call Common_EventScript_SetGymTrainers
	special MauvilleGymDeactivatePuzzle
	special DrawWholeMapView
	playse SE_UNLOCK
	call MauvilleCity_Gym_EventScript_GiveShockWave
	closemessage
	delay 30
	playfanfare MUS_REGISTER_MATCH_CALL
	msgbox MauvilleCity_Gym_Text_RegisteredWattson, MSGBOX_DEFAULT
	waitfanfare
	closemessage
	delay 30
	setflag FLAG_ENABLE_WATTSON_MATCH_CALL
	release
	end

MauvilleCity_Gym_EventScript_GiveShockWave2::
	giveitem ITEM_TM_SHOCK_WAVE
	goto_if_eq VAR_RESULT, FALSE, Common_EventScript_ShowBagIsFull
	msgbox MauvilleCity_Gym_Text_ExplainShockWave, MSGBOX_DEFAULT
	setflag FLAG_RECEIVED_TM_SHOCK_WAVE
	release
	end

MauvilleCity_Gym_EventScript_GiveShockWave::
	giveitem ITEM_TM_SHOCK_WAVE
	goto_if_eq VAR_RESULT, FALSE, Common_EventScript_BagIsFull
	msgbox MauvilleCity_Gym_Text_ExplainShockWave, MSGBOX_DEFAULT
	setflag FLAG_RECEIVED_TM_SHOCK_WAVE
	return

MauvilleCity_Gym_EventScript_CompletedNewMauville::
	msgbox MauvilleCity_Gym_Text_WattsonGoForthAndEndeavor, MSGBOX_DEFAULT
	release
	end

MauvilleCity_Gym_EventScript_WattsonRematch::
	trainerbattle_rematch_double TRAINER_WATTSON_1, MauvilleCity_Gym_Text_WattsonPreRematch, MauvilleCity_Gym_Text_WattsonRematchDefeat, MauvilleCity_Gym_Text_WattsonRematchNeedTwoMons
	msgbox MauvilleCity_Gym_Text_WattsonPostRematch, MSGBOX_AUTOCLOSE
	end

MauvilleCity_Gym_EventScript_Switch1::
	lockall
	goto_if_set FLAG_DEFEATED_MAUVILLE_GYM, MauvilleCity_Gym_EventScript_SwitchDoNothing
	goto_if_eq VAR_MAUVILLE_GYM_STATE, 1, MauvilleCity_Gym_EventScript_SwitchDoNothing
	setvar VAR_MAUVILLE_GYM_STATE, 1
	setvar VAR_0x8004, 0
	goto MauvilleCity_Gym_EventScript_PressFloorSwitch
	end

MauvilleCity_Gym_EventScript_Switch2::
	lockall
	goto_if_set FLAG_DEFEATED_MAUVILLE_GYM, MauvilleCity_Gym_EventScript_SwitchDoNothing
	goto_if_eq VAR_MAUVILLE_GYM_STATE, 2, MauvilleCity_Gym_EventScript_SwitchDoNothing
	setvar VAR_MAUVILLE_GYM_STATE, 2
	setvar VAR_0x8004, 1
	goto MauvilleCity_Gym_EventScript_PressFloorSwitch
	end

MauvilleCity_Gym_EventScript_Switch3::
	lockall
	goto_if_set FLAG_DEFEATED_MAUVILLE_GYM, MauvilleCity_Gym_EventScript_SwitchDoNothing
	goto_if_eq VAR_MAUVILLE_GYM_STATE, 3, MauvilleCity_Gym_EventScript_SwitchDoNothing
	setvar VAR_MAUVILLE_GYM_STATE, 3
	setvar VAR_0x8004, 2
	goto MauvilleCity_Gym_EventScript_PressFloorSwitch
	end

MauvilleCity_Gym_EventScript_Switch4::
	lockall
	goto_if_set FLAG_DEFEATED_MAUVILLE_GYM, MauvilleCity_Gym_EventScript_SwitchDoNothing
	goto_if_eq VAR_MAUVILLE_GYM_STATE, 4, MauvilleCity_Gym_EventScript_SwitchDoNothing
	setvar VAR_MAUVILLE_GYM_STATE, 4
	setvar VAR_0x8004, 3
	goto MauvilleCity_Gym_EventScript_PressFloorSwitch
	end

MauvilleCity_Gym_EventScript_PressFloorSwitch::
	special MauvilleGymSetDefaultBarriers
	special MauvilleGymPressSwitch
	special DrawWholeMapView
	playse SE_UNLOCK
	goto_if_set FLAG_MAUVILLE_GYM_BARRIERS_STATE, MauvilleCity_Gym_EventScript_ClearBarriersAltState
	goto_if_unset FLAG_MAUVILLE_GYM_BARRIERS_STATE, MauvilleCity_Gym_EventScript_SetBarriersAltState
	releaseall
	end

MauvilleCity_Gym_EventScript_SwitchDoNothing::
	releaseall
	end

MauvilleCity_Gym_EventScript_SetBarriersAltState::
	setflag FLAG_MAUVILLE_GYM_BARRIERS_STATE
	releaseall
	end

MauvilleCity_Gym_EventScript_ClearBarriersAltState::
	clearflag FLAG_MAUVILLE_GYM_BARRIERS_STATE
	releaseall
	end

MauvilleCity_Gym_EventScript_Kirk::
	trainerbattle_single TRAINER_KIRK, MauvilleCity_Gym_Text_KirkIntro, MauvilleCity_Gym_Text_KirkDefeat
	msgbox MauvilleCity_Gym_Text_KirkPostBattle, MSGBOX_AUTOCLOSE
	end

MauvilleCity_Gym_EventScript_Shawn::
	trainerbattle_single TRAINER_SHAWN, MauvilleCity_Gym_Text_ShawnIntro, MauvilleCity_Gym_Text_ShawnDefeat
	msgbox MauvilleCity_Gym_Text_ShawnPostBattle, MSGBOX_AUTOCLOSE
	end

MauvilleCity_Gym_EventScript_Ben::
	trainerbattle_single TRAINER_BEN, MauvilleCity_Gym_Text_BenIntro, MauvilleCity_Gym_Text_BenDefeat
	msgbox MauvilleCity_Gym_Text_BenPostBattle, MSGBOX_AUTOCLOSE
	end

MauvilleCity_Gym_EventScript_Vivian::
	trainerbattle_single TRAINER_VIVIAN, MauvilleCity_Gym_Text_VivianIntro, MauvilleCity_Gym_Text_VivianDefeat
	msgbox MauvilleCity_Gym_Text_VivianPostBattle, MSGBOX_AUTOCLOSE
	end

MauvilleCity_Gym_EventScript_Angelo::
	trainerbattle_single TRAINER_ANGELO, MauvilleCity_Gym_Text_AngeloIntro, MauvilleCity_Gym_Text_AngeloDefeat
	msgbox MauvilleCity_Gym_Text_AngeloPostBattle, MSGBOX_AUTOCLOSE
	end

MauvilleCity_Gym_EventScript_GymGuide::
	lock
	faceplayer
	goto_if_set FLAG_DEFEATED_MAUVILLE_GYM, MauvilleCity_Gym_EventScript_GymGuidePostVictory
	msgbox MauvilleCity_Gym_Text_GymGuideAdvice, MSGBOX_DEFAULT
	release
	end

MauvilleCity_Gym_EventScript_GymGuidePostVictory::
	msgbox MauvilleCity_Gym_Text_GymGuidePostVictory, MSGBOX_DEFAULT
	release
	end

MauvilleCity_Gym_EventScript_LeftGymStatue::
	lockall
	goto_if_set FLAG_BADGE03_GET, MauvilleCity_Gym_EventScript_GymStatueCertified
	goto MauvilleCity_Gym_EventScript_GymStatue
	end

MauvilleCity_Gym_EventScript_RightGymStatue::
	lockall
	goto_if_set FLAG_BADGE03_GET, MauvilleCity_Gym_EventScript_GymStatueCertified
	goto MauvilleCity_Gym_EventScript_GymStatue
	end

MauvilleCity_Gym_EventScript_GymStatueCertified::
	msgbox MauvilleCity_Gym_Text_GymStatueCertified, MSGBOX_DEFAULT
	releaseall
	end

MauvilleCity_Gym_EventScript_GymStatue::
	msgbox MauvilleCity_Gym_Text_GymStatue, MSGBOX_DEFAULT
	releaseall
	end

MauvilleCity_Gym_Text_GymGuideAdvice:
	.string "Yo, futur Maître {PLAYER} !\p"
	.string "Voltère, le Champion de Lavandia,\n"
	.string "utilise des Pokémon Électrik.\p"
	.string "Si tu viens avec des Pokémon Eau, il va\n"
	.string "les griller ! Bzzt !\p"
	.string "Et il a mis des portes à interrupteurs\n"
	.string "partout dans son Arène ! Un vrai\l"
	.string "maniaque !\p"
	.string "Allez, fonce !$"

MauvilleCity_Gym_Text_GymGuidePostVictory:
	.string "Waouh, c'est électrisant ! T'as ouvert\n"
	.string "la porte à la force des volts !$"

MauvilleCity_Gym_Text_KirkIntro:
	.string "Mon âme électrique va pulvériser tes\n"
	.string "rêves, ouais-ouais !$"

MauvilleCity_Gym_Text_KirkDefeat:
	.string "C'était branché, amplifié, saturé, mec !$"

MauvilleCity_Gym_Text_KirkPostBattle:
	.string "Les Pokémon et le rock, c'est une\n"
	.string "histoire de cœur, ouais !$"

MauvilleCity_Gym_Text_ShawnIntro:
	.string "Je suis un élève de Voltère ! Je vais pas\n"
	.string "perdre si facilement !$"

MauvilleCity_Gym_Text_ShawnDefeat:
	.string "Débranché et éteint…$"

MauvilleCity_Gym_Text_ShawnPostBattle:
	.string "Voltère, notre Champion, est là depuis\n"
	.string "toujours.\p"
	.string "Il combattait déjà avant la naissance\n"
	.string "de ton père. Ce vieux briscard est\l"
	.string "increvable.$"

MauvilleCity_Gym_Text_BenIntro:
	.string "Cette Arène, c'est plein d'énigmes !\n"
	.string "C'est trop marrant, non ?$"

MauvilleCity_Gym_Text_BenDefeat:
	.string "C'est pas marrant de perdre…$"

MauvilleCity_Gym_Text_BenPostBattle:
	.string "Voltère dit qu'il adore poser des petits\n"
	.string "pièges à interrupteurs.$"

MauvilleCity_Gym_Text_VivianIntro:
	.string "Avec mon charme et les attaques de mes\n"
	.string "Pokémon, tu vas prendre la châtaigne de\l"
	.string "ta vie !$"

MauvilleCity_Gym_Text_VivianDefeat:
	.string "C'est ta puissance qui m'a électrocutée\n"
	.string "!$"

MauvilleCity_Gym_Text_VivianPostBattle:
	.string "Il paraît que Lavandia a été fondée par\n"
	.string "Voltère.\p"
	.string "Il était Dresseur bien avant nous. Il\n"
	.string "doit en savoir, des choses. Et en\l"
	.string "cacher, aussi.$"

MauvilleCity_Gym_Text_AngeloIntro:
	.string "J'adore ce qui brille !$"

MauvilleCity_Gym_Text_AngeloDefeat:
	.string "Oh… Mes yeux sont grillés…$"

MauvilleCity_Gym_Text_AngeloPostBattle:
	.string "Voltère a un front qui brille. Ça me rend\n"
	.string "heureux ! Je pourrais le regarder\l"
	.string "pendant des heures.$"

MauvilleCity_Gym_Text_WattsonIntro:
	.string "J'ai abandonné mon projet de\n"
	.string "reconstruire la ville, oui.\p"
	.string "Le conseil municipal m'a dit que j'avais\n"
	.string "déjà assez électrocuté de monde.\p"
	.string "Alors je passe mon temps à piéger les\n"
	.string "portes de mon Arène.\p"
	.string "Oh ? Mais qu'est-ce que tu fais là ?\p"
	.string "Quoi ? Tu as passé toutes mes portes\n"
	.string "piégées ?\p"
	.string "Wahahahah ! Voilà qui est amusant !\p"
	.string "Alors moi, Voltère, Champion de\n"
	.string "Lavandia, je vais t'électriser !$"

MauvilleCity_Gym_Text_WattsonDefeat:
	.string "Wahahahah ! Bon, j'ai perdu !\p"
	.string "Tu m'as filé un sacré frisson ! Prends ce\n"
	.string "Badge !$"

MauvilleCity_Gym_Text_ReceivedDynamoBadge:
	.string "{PLAYER} reçoit le Badge Dynamo de la\n"
	.string "part de Voltère.$"

MauvilleCity_Gym_Text_ExplainDynamoBadgeTakeThis:
	.string "Avec le Badge Dynamo, tes Pokémon\n"
	.string "peuvent utiliser Éclate-Roc hors\l"
	.string "combat.\p"
	.string "Et ils seront un peu plus rapides.\p"
	.string "Hmm… Prends aussi ça !$"

MauvilleCity_Gym_Text_ExplainShockWave:
	.string "La CT34 contient Onde de Choc.\p"
	.string "Une capacité fiable qui ne rate jamais !\n"
	.string "Tu peux compter dessus !\p"
	.string "… … … … … …$"

MauvilleCity_Gym_Text_RegisteredWattson:
	.string "Le Champion Voltère a été enregistré\n"
	.string "dans le PokéNav.$"

MauvilleCity_Gym_Text_WattsonPostBattle:
	.string "Je déborde d'optimisme en voyant un\n"
	.string "jeune Dresseur aussi prometteur !$"

MauvilleCity_Gym_Text_WattsonGoForthAndEndeavor:
	.string "Wahahahah ! Va de l'avant, jeunesse !$"

MauvilleCity_Gym_Text_GymStatue:
	.string "Arène Pokémon de Lavandia$"

MauvilleCity_Gym_Text_GymStatueCertified:
	.string "Arène Pokémon de Lavandia\p"
	.string "Dresseurs reconnus par Voltère :\n"
	.string "{PLAYER}$"

MauvilleCity_Gym_Text_WattsonPreRematch:
	.string "Voltère : Ah-ha ! Enfin ! Je sais ce que\n"
	.string "tu veux. Tu veux affronter mes Pokémon\l"
	.string "!\p"
	.string "Wahahahaha !\p"
	.string "Je vais te faire jaillir des étincelles !\n"
	.string "On ne pourra pas dire que je n'ai rien\l"
	.string "dit !$"

MauvilleCity_Gym_Text_WattsonRematchDefeat:
	.string "Ouf… Nos batteries sont à plat…$"

MauvilleCity_Gym_Text_WattsonPostRematch:
	.string "Voltère : Il va falloir recharger nos\n"
	.string "batteries.\p"
	.string "Quand on sera à bloc, on acceptera ton\n"
	.string "défi avec plaisir.\p"
	.string "Alors reviens un de ces jours, d'accord\n"
	.string "?$"

MauvilleCity_Gym_Text_WattsonRematchNeedTwoMons:
	.string "Voltère : Ah-ha ! Enfin ! Je sais ce que\n"
	.string "tu veux. Tu veux affronter mes Pokémon\l"
	.string "!\p"
	.string "Wahahahaha !\p"
	.string "Oups ! Attends ! Tu n'as qu'un seul\n"
	.string "Pokémon !\p"
	.string "Reviens avec au moins deux Pokémon,\n"
	.string "d'accord ?$"

