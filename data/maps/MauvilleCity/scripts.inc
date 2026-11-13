MauvilleCity_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, MauvilleCity_OnTransition
	.byte 0

MauvilleCity_OnTransition:
	setflag FLAG_VISITED_MAUVILLE_CITY
	clearflag FLAG_FORCE_MIRAGE_TOWER_VISIBLE
	clearflag FLAG_HIDE_SLATEPORT_MUSEUM_POPULATION
	setflag FLAG_SYS_TV_START
	clearflag FLAG_MAUVILLE_GYM_BARRIERS_STATE
	setvar VAR_MAUVILLE_GYM_STATE, 0
	call_if_set FLAG_GOT_TM_THUNDERBOLT_FROM_WATTSON, MauvilleCity_EventScript_MoveWattsonBackToGym
	end

MauvilleCity_EventScript_MoveWattsonBackToGym::
	clearflag FLAG_HIDE_MAUVILLE_GYM_WATTSON
	setflag FLAG_HIDE_MAUVILLE_CITY_WATTSON
	setflag FLAG_WATTSON_REMATCH_AVAILABLE
	return

MauvilleCity_EventScript_Boy::
	msgbox MauvilleCity_Text_NurseHurtMonBackToHealth, MSGBOX_NPC
	end

MauvilleCity_EventScript_Maniac::
	msgbox MauvilleCity_Text_AllSortsOfPeopleComeThrough, MSGBOX_NPC
	end

MauvilleCity_EventScript_Woman::
	msgbox MauvilleCity_Text_RydelVeryGenerous, MSGBOX_NPC
	end

MauvilleCity_EventScript_RichBoy::
	msgbox MauvilleCity_Text_PokemonCanJumpYouOnBike, MSGBOX_NPC
	end

MauvilleCity_EventScript_CitySign::
	msgbox MauvilleCity_Text_CitySign, MSGBOX_SIGN
	end

MauvilleCity_EventScript_GymSign::
	msgbox MauvilleCity_Text_GymSign, MSGBOX_SIGN
	end

MauvilleCity_EventScript_BikeShopSign::
	msgbox MauvilleCity_Text_BikeShopSign, MSGBOX_SIGN
	end

MauvilleCity_EventScript_GameCornerSign::
	msgbox MauvilleCity_Text_GameCornerSign, MSGBOX_SIGN
	end

MauvilleCity_EventScript_SchoolKidM::
	lock
	faceplayer
	goto_if_set FLAG_TV_EXPLAINED, MauvilleCity_EventScript_TVExplained
	msgbox MauvilleCity_Text_ExplainTV, MSGBOX_DEFAULT
	setflag FLAG_TV_EXPLAINED
	release
	end

MauvilleCity_EventScript_TVExplained::
	msgbox MauvilleCity_Text_BeenCheckingOutTV, MSGBOX_DEFAULT
	release
	end

MauvilleCity_EventScript_WallysUncle::
	lock
	faceplayer
	goto_if_set FLAG_DECLINED_WALLY_BATTLE_MAUVILLE, MauvilleCity_EventScript_UncleAskPlayerToBattleWally
	msgbox MauvilleCity_Text_UncleHesTooPeppy, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_MAUVILLE_WALLYS_UNCLE, Common_Movement_FaceOriginalDirection
	waitmovement 0
	release
	end

MauvilleCity_EventScript_UncleAskPlayerToBattleWally::
	msgbox MauvilleCity_Text_UncleCanYouBattleWally, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_MAUVILLE_WALLYS_UNCLE, Common_Movement_FaceOriginalDirection
	waitmovement 0
	release
	end

MauvilleCity_EventScript_Wally::
	lockall
	goto_if_set FLAG_DECLINED_WALLY_BATTLE_MAUVILLE, MauvilleCity_EventScript_WallyRequestBattleAgain
	applymovement LOCALID_MAUVILLE_WALLY, Common_Movement_WalkInPlaceFasterRight
	waitmovement 0
	msgbox MauvilleCity_Text_WallyWantToChallengeGym, MSGBOX_DEFAULT
	msgbox MauvilleCity_Text_UncleYourePushingIt, MSGBOX_DEFAULT
	msgbox MauvilleCity_Text_WallyWeCanBeatAnyone, MSGBOX_DEFAULT
	applymovement LOCALID_MAUVILLE_WALLY, Common_Movement_FacePlayer
	waitmovement 0
	playse SE_PIN
	applymovement LOCALID_MAUVILLE_WALLY, Common_Movement_ExclamationMark
	waitmovement 0
	applymovement LOCALID_MAUVILLE_WALLY, Common_Movement_Delay48
	waitmovement 0
	msgbox MauvilleCity_Text_WallyWillYouBattleMe, MSGBOX_YESNO
	goto MauvilleCity_EventScript_BattleWallyPrompt
	end

MauvilleCity_EventScript_BattleWallyPrompt::
	call_if_eq VAR_RESULT, YES, MauvilleCity_EventScript_BattleWally
	goto_if_eq VAR_RESULT, NO, MauvilleCity_EventScript_DeclineWallyBattle
	closemessage
	switch VAR_FACING
	case DIR_NORTH, MauvilleCity_EventScript_WallyAndUncleExitNorth
	case DIR_EAST, MauvilleCity_EventScript_WallyAndUncleExitEast
	end

MauvilleCity_EventScript_WallyAndUncleExitNorth::
	applymovement LOCALID_PLAYER, MauvilleCity_Movement_PlayerWatchWallyExitNorth1
	applymovement LOCALID_MAUVILLE_WALLY, MauvilleCity_Movement_WallyExitNorth1
	applymovement LOCALID_MAUVILLE_WALLYS_UNCLE, MauvilleCity_Movement_WallysUncleExitNorth1
	waitmovement 0
	applymovement LOCALID_MAUVILLE_WALLY, Common_Movement_WalkInPlaceFasterRight
	waitmovement 0
	delay 30
	applymovement LOCALID_PLAYER, MauvilleCity_Movement_PlayerFaceUncleNorth
	applymovement LOCALID_MAUVILLE_WALLYS_UNCLE, MauvilleCity_Movement_WallysUncleApproachPlayerNorth
	waitmovement 0
	msgbox MauvilleCity_Text_UncleVisitUsSometime, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_PLAYER, MauvilleCity_Movement_PlayerWatchWallyExitNorth2
	applymovement LOCALID_MAUVILLE_WALLYS_UNCLE, MauvilleCity_Movement_WallysUncleExitNorth2
	applymovement LOCALID_MAUVILLE_WALLY, MauvilleCity_Movement_WallyExitNorth2
	waitmovement 0
	goto MauvilleCity_EventScript_DefeatedWally
	end

MauvilleCity_EventScript_WallyAndUncleExitEast::
	applymovement LOCALID_PLAYER, MauvilleCity_Movement_PlayerWatchWallyExitEast1
	applymovement LOCALID_MAUVILLE_WALLY, MauvilleCity_Movement_WallyExitEast1
	applymovement LOCALID_MAUVILLE_WALLYS_UNCLE, MauvilleCity_Movement_WallysUncleExitEast1
	waitmovement 0
	applymovement LOCALID_MAUVILLE_WALLY, Common_Movement_WalkInPlaceFasterRight
	waitmovement 0
	delay 30
	applymovement LOCALID_MAUVILLE_WALLYS_UNCLE, MauvilleCity_Movement_WallysUncleApproachPlayerEast
	waitmovement 0
	msgbox MauvilleCity_Text_UncleVisitUsSometime, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_PLAYER, MauvilleCity_Movement_PlayerWatchWallyExitEast2
	applymovement LOCALID_MAUVILLE_WALLYS_UNCLE, MauvilleCity_Movement_WallysUncleExitEast2
	applymovement LOCALID_MAUVILLE_WALLY, MauvilleCity_Movement_WallyExitEast2
	waitmovement 0
	goto MauvilleCity_EventScript_DefeatedWally
	end

MauvilleCity_EventScript_DefeatedWally::
	removeobject LOCALID_MAUVILLE_WALLY
	removeobject LOCALID_MAUVILLE_WALLYS_UNCLE
	clearflag FLAG_HIDE_VERDANTURF_TOWN_WANDAS_HOUSE_WALLY
	clearflag FLAG_HIDE_VERDANTURF_TOWN_WANDAS_HOUSE_WALLYS_UNCLE
	setflag FLAG_DEFEATED_WALLY_MAUVILLE
	setvar VAR_WALLY_CALL_STEP_COUNTER, 0
	setflag FLAG_ENABLE_FIRST_WALLY_POKENAV_CALL
	call_if_eq VAR_FACING, DIR_NORTH, MauvilleCity_EventScript_ScottApproachPlayerNorth
	call_if_eq VAR_FACING, DIR_EAST, MauvilleCity_EventScript_ScottApproachPlayerEast
	applymovement LOCALID_PLAYER, Common_Movement_WalkInPlaceFasterDown
	waitmovement 0
	msgbox MauvilleCity_Text_ScottYouDidntHoldBack, MSGBOX_DEFAULT
	closemessage
	addvar VAR_SCOTT_STATE, 1
	call_if_eq VAR_FACING, DIR_NORTH, MauvilleCity_EventScript_ScottExitNorth
	call_if_eq VAR_FACING, DIR_EAST, MauvilleCity_EventScript_ScottExitEast
	removeobject LOCALID_MAUVILLE_SCOTT
	releaseall
	end

MauvilleCity_EventScript_ScottApproachPlayerNorth::
	addobject LOCALID_MAUVILLE_SCOTT
	applymovement LOCALID_MAUVILLE_SCOTT, MauvilleCity_Movement_ScottApproachPlayerNorth
	waitmovement 0
	return

MauvilleCity_EventScript_ScottApproachPlayerEast::
	setobjectxyperm LOCALID_MAUVILLE_SCOTT, 12, 13
	addobject LOCALID_MAUVILLE_SCOTT
	applymovement LOCALID_MAUVILLE_SCOTT, MauvilleCity_Movement_ScottApproachPlayerEast
	waitmovement 0
	return

MauvilleCity_EventScript_ScottExitNorth::
	applymovement LOCALID_PLAYER, MauvilleCity_Movement_PlayerWatchScottExitNorth
	applymovement LOCALID_MAUVILLE_SCOTT, MauvilleCity_Movement_ScottExitNorth
	waitmovement 0
	return

MauvilleCity_EventScript_ScottExitEast::
	applymovement LOCALID_PLAYER, MauvilleCity_Movement_PlayerWatchScottExitEast
	applymovement LOCALID_MAUVILLE_SCOTT, MauvilleCity_Movement_ScottExitEast
	waitmovement 0
	return

MauvilleCity_EventScript_BattleWally::
	msgbox MauvilleCity_Text_WallyHereICome, MSGBOX_DEFAULT
	trainerbattle_no_intro TRAINER_WALLY_MAUVILLE, MauvilleCity_Text_WallyDefeat
	applymovement LOCALID_MAUVILLE_WALLY, Common_Movement_WalkInPlaceFasterRight
	waitmovement 0
	msgbox MauvilleCity_Text_WallyIllGoBackToVerdanturf, MSGBOX_DEFAULT
	applymovement LOCALID_MAUVILLE_WALLY, Common_Movement_FacePlayer
	waitmovement 0
	msgbox MauvilleCity_Text_ThankYouNotEnoughToBattle, MSGBOX_DEFAULT
	applymovement LOCALID_MAUVILLE_WALLY, Common_Movement_WalkInPlaceFasterRight
	waitmovement 0
	msgbox MauvilleCity_Text_UncleNoNeedToBeDown, MSGBOX_DEFAULT
	return

MauvilleCity_EventScript_DeclineWallyBattle::
	setflag FLAG_DECLINED_WALLY_BATTLE_MAUVILLE
	msgbox MauvilleCity_Text_WallyMyUncleWontKnowImStrong, MSGBOX_DEFAULT
	release
	end

MauvilleCity_EventScript_WallyRequestBattleAgain::
	applymovement LOCALID_MAUVILLE_WALLY, Common_Movement_FacePlayer
	waitmovement 0
	msgbox MauvilleCity_Text_WallyPleaseBattleMe, MSGBOX_YESNO
	goto MauvilleCity_EventScript_BattleWallyPrompt
	end

MauvilleCity_Movement_WallyExitNorth1:
	walk_left
	walk_left
	walk_down
	walk_down
	walk_left
	step_end

MauvilleCity_Movement_WallyExitEast1:
	walk_down
	walk_down
	walk_left
	walk_left
	walk_left
	step_end

MauvilleCity_Movement_WallyExitNorth2:
	delay_16
	delay_16
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	delay_8
	step_end

MauvilleCity_Movement_WallyExitEast2:
	delay_16
	delay_16
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	delay_8
	step_end

MauvilleCity_Movement_PlayerWatchWallyExitNorth2:
	delay_16
	delay_8
	walk_in_place_faster_left
	step_end

MauvilleCity_Movement_PlayerWatchWallyExitEast2:
	delay_16
	delay_16
	delay_16
	walk_in_place_faster_left
	step_end

MauvilleCity_Movement_PlayerWatchScottExitNorth:
	delay_16
	walk_in_place_faster_left
	step_end

MauvilleCity_Movement_PlayerWatchScottExitEast:
	delay_16
	delay_16
	walk_in_place_faster_left
	step_end

MauvilleCity_Movement_PlayerWatchWallyExitEast1:
	delay_16
	walk_in_place_faster_down
	step_end

MauvilleCity_Movement_PlayerWatchWallyExitNorth1:
	delay_16
	walk_in_place_faster_left
	step_end

MauvilleCity_Movement_WallysUncleExitNorth1:
	walk_left
	walk_left
	walk_left
	walk_down
	walk_down
	step_end

MauvilleCity_Movement_WallysUncleExitEast1:
	walk_left
	walk_down
	walk_down
	walk_left
	walk_left
	step_end

MauvilleCity_Movement_PlayerFaceUncleNorth:
	delay_16
	delay_8
	delay_4
	walk_in_place_faster_down
	step_end

MauvilleCity_Movement_WallysUncleApproachPlayerNorth:
	walk_right
	walk_right
	walk_in_place_faster_up
	step_end

MauvilleCity_Movement_WallysUncleApproachPlayerEast:
	walk_right
	walk_up
	step_end

MauvilleCity_Movement_WallysUncleExitNorth2:
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	step_end

MauvilleCity_Movement_WallysUncleExitEast2:
	walk_down
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	step_end

MauvilleCity_Movement_ScottApproachPlayerNorth:
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_left
	walk_left
	walk_left
	walk_left
	walk_in_place_faster_up
	step_end

MauvilleCity_Movement_ScottApproachPlayerEast:
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_up
	step_end

MauvilleCity_Movement_ScottExitNorth:
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	step_end

MauvilleCity_Movement_ScottExitEast:
	walk_down
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	step_end

MauvilleCity_EventScript_Wattson::
	lock
	faceplayer
	goto_if_set FLAG_GOT_TM_THUNDERBOLT_FROM_WATTSON, MauvilleCity_EventScript_ReceivedThunderbolt
	goto_if_eq VAR_NEW_MAUVILLE_STATE, 2, MauvilleCity_EventScript_CompletedNewMauville
	goto_if_set FLAG_GOT_BASEMENT_KEY_FROM_WATTSON, MauvilleCity_EventScript_BegunNewMauville
	msgbox MauvilleCity_Text_WattsonNeedFavorTakeKey, MSGBOX_DEFAULT
	giveitem ITEM_BASEMENT_KEY
	goto_if_eq VAR_RESULT, FALSE, Common_EventScript_ShowBagIsFull
	setflag FLAG_GOT_BASEMENT_KEY_FROM_WATTSON
	msgbox MauvilleCity_Text_WattsonWontBeChallenge, MSGBOX_DEFAULT
	release
	end

MauvilleCity_EventScript_BegunNewMauville::
	msgbox MauvilleCity_Text_WattsonWontBeChallenge, MSGBOX_DEFAULT
	release
	end

MauvilleCity_EventScript_CompletedNewMauville::
	msgbox MauvilleCity_Text_WattsonThanksTakeTM, MSGBOX_DEFAULT
	giveitem ITEM_TM_THUNDERBOLT
	goto_if_eq VAR_RESULT, FALSE, Common_EventScript_ShowBagIsFull
	setflag FLAG_GOT_TM_THUNDERBOLT_FROM_WATTSON
	msgbox MauvilleCity_Text_WattsonYoungTakeCharge, MSGBOX_DEFAULT
	release
	end

MauvilleCity_EventScript_ReceivedThunderbolt::
	msgbox MauvilleCity_Text_WattsonYoungTakeCharge, MSGBOX_DEFAULT
	release
	end

MauvilleCity_EventScript_RegisterWallyCall::
	lockall
	pokenavcall MauvilleCity_Text_WallyPokenavCall
	waitmessage
	delay 30
	playfanfare MUS_REGISTER_MATCH_CALL
	msgbox MauvilleCity_Text_RegisteredWally, MSGBOX_DEFAULT
	waitfanfare
	closemessage
	delay 30
	setflag FLAG_ENABLE_WALLY_MATCH_CALL
	clearflag FLAG_ENABLE_FIRST_WALLY_POKENAV_CALL
	releaseall
	end

MauvilleCity_Text_UncleHesTooPeppy:
	.string "Oncle : C'est grâce aux Pokémon que ce\n"
	.string "gamin a retrouvé de l'énergie, je\l"
	.string "suppose… Mais là, il en a un peu trop.$"

MauvilleCity_Text_WallyWantToChallengeGym:
	.string "Timmy : Allez, tonton, s'il te plaît !\p"
	.string "Je veux défier cette Arène pour voir à\n"
	.string "quel point j'ai progressé.\p"
	.string "S'il te plaît ? Je peux ?$"

MauvilleCity_Text_UncleYourePushingIt:
	.string "Oncle : Doucement, Timmy.\p"
	.string "Depuis que tu vis avec des Pokémon, tu\n"
	.string "as beaucoup changé. Tu manges, tu dors,\l"
	.string "tu ris.\p"
	.string "Mais défier une Arène du jour au\n"
	.string "lendemain… Tu ne crois pas que tu vas\l"
	.string "trop vite ? Ton cœur…$"

MauvilleCity_Text_WallyWeCanBeatAnyone:
	.string "Timmy : Je ne vais pas trop vite.\p"
	.string "J'ai déjà perdu assez de temps dans un\n"
	.string "lit d'hôpital. Avec Tarsal, on peut\l"
	.string "battre n'importe qui !$"

MauvilleCity_Text_WallyWillYouBattleMe:
	.string "Timmy : Oh ! Salut, {PLAYER} !\p"
	.string "Je suis devenu beaucoup plus fort\n"
	.string "depuis notre rencontre.\p"
	.string "{PLAYER}, je veux que toi et mon oncle,\n"
	.string "vous le voyiez.\p"
	.string "{PLAYER}, s'il te plaît, bats-toi contre\n"
	.string "moi.$"

MauvilleCity_Text_WallyMyUncleWontKnowImStrong:
	.string "Timmy : Oh… Si tu refuses, mon oncle ne\n"
	.string "saura jamais que je suis vraiment\l"
	.string "devenu fort. Il continuera à me\l"
	.string "regarder comme si j'allais casser.$"

MauvilleCity_Text_UncleCanYouBattleWally:
	.string "Oncle : {PLAYER}, c'est ça ? Pour Timmy,\n"
	.string "tu veux bien l'affronter, juste une fois\l"
	.string "?\p"
	.string "Dans cet état, il n'écoutera personne.$"

MauvilleCity_Text_WallyPleaseBattleMe:
	.string "Timmy : {PLAYER}, s'il te plaît ! Bats-toi\n"
	.string "contre moi.$"

MauvilleCity_Text_WallyHereICome:
	.string "Timmy : {PLAYER}, merci.\p"
	.string "OK… J'arrive ! Ne me ménage pas !$"

MauvilleCity_Text_WallyDefeat:
	.string "Timmy : … … … … … … …\p"
	.string "… … … … … … … …\p"
	.string "J'ai perdu…$"

MauvilleCity_Text_WallyIllGoBackToVerdanturf:
	.string "Timmy : Tonton… Je rentre à Vergazon…$"

MauvilleCity_Text_ThankYouNotEnoughToBattle:
	.string "{PLAYER}, merci. C'est dur, d'être\n"
	.string "Dresseur, hein ?\p"
	.string "Avoir des Pokémon et les faire se\n"
	.string "battre, ça ne suffit pas. Ce n'est pas\l"
	.string "ça, être un vrai Dresseur.\p"
	.string "Mais je vais devenir plus fort. J'ai un\n"
	.string "corps fragile, pas un cœur fragile.$"

MauvilleCity_Text_UncleNoNeedToBeDown:
	.string "Oncle : Timmy, inutile de te rabaisser.\p"
	.string "Rien ne t'empêche de devenir de plus en\n"
	.string "plus fort.\p"
	.string "Allez, on rentre. Tout le monde t'attend.$"

MauvilleCity_Text_UncleVisitUsSometime:
	.string "Oncle : {PLAYER}, je viens de comprendre\n"
	.string ": c'est toi qui as accompagné Timmy\l"
	.string "quand il a capturé son Pokémon.\p"
	.string "Viens nous voir à Vergazon un jour.\n"
	.string "Timmy serait ravi. Il parle de toi tous\l"
	.string "les soirs.$"


MauvilleCity_Text_WallyPokenavCall:
	.string "… … … … … … … … … … … Bip !\p"
	.string "Timmy : Oh, salut, {PLAYER} !\p"
	.string "Je voulais te dire : mon oncle m'a acheté\n"
	.string "un PokéNav !\p"
	.string "Maintenant, je peux t'appeler quand je\n"
	.string "veux !\p"
	.string "… … … … … … … … … … … Clic !$"

MauvilleCity_Text_RegisteredWally:
	.string "Timmy a été enregistré dans le PokéNav.$"

MauvilleCity_Text_ScottYouDidntHoldBack:
	.string "Scott : Hé hé… J'ai vu ce combat !\p"
	.string "Ce petit Timmy, c'est ton ami, non ?\p"
	.string "Et pourtant, tu ne l'as pas ménagé. Tu\n"
	.string "l'as battu sans pitié.\p"
	.string "Ouais ! C'est ça, un vrai combat Pokémon\n"
	.string "! Le respect, c'est de tout donner.\p"
	.string "J'admire les Dresseurs comme toi !\p"
	.string "… … … … … … Je suis à fond derrière toi !$"

MauvilleCity_Text_WattsonNeedFavorTakeKey:
	.string "Voltère : Oh, {PLAYER} ! T'as l'air plein\n"
	.string "d'énergie ! C'est bien, ça, wahahahaha !\p"
	.string "C'est décidé ! J'ai un service à te\n"
	.string "demander !\p"
	.string "Sous Lavandia, il y a un quartier\n"
	.string "souterrain : Nouvelle Lavandia.\p"
	.string "J'aimerais que tu y ailles couper le\n"
	.string "Générateur.\p"
	.string "Il déraille depuis un moment. Ça devient\n"
	.string "dangereux. Un technicien y a laissé\l"
	.string "deux doigts la semaine dernière.\p"
	.string "Tiens, la Clé de Nouvelle Lavandia.$"

MauvilleCity_Text_WattsonWontBeChallenge:
	.string "Voltère : T'inquiète pas. Pour toi, ce\n"
	.string "sera une promenade.\p"
	.string "L'entrée de Nouvelle Lavandia est à\n"
	.string "deux brasses de la Route 110.\p"
	.string "Voilà, je te fais confiance ! Wahahahaha\n"
	.string "!$"

MauvilleCity_Text_WattsonThanksTakeTM:
	.string "Voltère : Wahahahaha !\p"
	.string "Je le savais, {PLAYER} ! J'ai eu raison de\n"
	.string "te le demander !\p"
	.string "Voilà mon merci : une CT qui contient\n"
	.string "Tonnerre !\p"
	.string "Allez, tu l'as mérité !$"

MauvilleCity_Text_WattsonYoungTakeCharge:
	.string "Voltère : Wahahahaha !\p"
	.string "Ça me fait chaud au cœur de voir les\n"
	.string "jeunes prendre les choses en main ! Moi,\l"
	.string "je commence à rouiller.$"

MauvilleCity_Text_NurseHurtMonBackToHealth:
	.string "Les combats Pokémon, c'est cool…\p"
	.string "Mais si ton Pokémon est blessé, faut le\n"
	.string "soigner. Sinon, t'es juste un bourreau.$"

MauvilleCity_Text_AllSortsOfPeopleComeThrough:
	.string "Les routes de cette ville vont au nord,\n"
	.string "au sud, à l'est et à l'ouest.\p"
	.string "Du coup, on voit passer toutes sortes\n"
	.string "de gens. Des bons et des moins bons.$"

MauvilleCity_Text_RydelVeryGenerous:
	.string "Tu connais Rydel Cycles ?\p"
	.string "Rydel, le patron, est très généreux. Un\n"
	.string "peu trop, même. Je me demande comment il\l"
	.string "paie son loyer.$"

MauvilleCity_Text_PokemonCanJumpYouOnBike:
	.string "Même à Vélo, les Pokémon sauvages\n"
	.string "peuvent te sauter dessus.$"

MauvilleCity_Text_CitySign:
	.string "Lavandia\n"
	.string "“La ville brillante et pleine de joie !”$"

MauvilleCity_Text_GymSign:
	.string "Arène Pokémon de Lavandia\n"
	.string "Champion : Voltère\l"
	.string "“L'homme joyeusement électrisant !”$"

MauvilleCity_Text_BikeShopSign:
	.string "“Roulez dans la caillasse et\n"
	.string "secouez-vous l'âme !”\p"
	.string "Rydel Cycles$"

MauvilleCity_Text_GameCornerSign:
	.string "“Le lieu de jeu pour tous !”\p"
	.string "Casino de Lavandia$"

MauvilleCity_Text_ExplainTV:
	.string "Salut, tu regardes la télé ?\p"
	.string "Ils ont ajouté plein de nouvelles\n"
	.string "émissions.\p"
	.string "La télé, c'est génial. J'ai déjà vu mes\n"
	.string "potes passer dedans, sans prévenir.\p"
	.string "Et parfois, ils te mettent à la télé sans\n"
	.string "te demander ton avis.\p"
	.string "J'aimerais bien passer à la télé un jour.\p"
	.string "Alors regarde la télé dès que tu peux.$"

MauvilleCity_Text_BeenCheckingOutTV:
	.string "Salut, tu as regardé la télé ?$"
