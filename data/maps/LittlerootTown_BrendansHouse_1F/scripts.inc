LittlerootTown_BrendansHouse_1F_MapScripts::
	map_script MAP_SCRIPT_ON_LOAD, LittlerootTown_BrendansHouse_1F_OnLoad
	map_script MAP_SCRIPT_ON_TRANSITION, LittlerootTown_BrendansHouse_1F_OnTransition
	map_script MAP_SCRIPT_ON_FRAME_TABLE, LittlerootTown_BrendansHouse_1F_OnFrame
	.byte 0

LittlerootTown_BrendansHouse_1F_OnLoad:
	call_if_lt VAR_LITTLEROOT_INTRO_STATE, 6, LittlerootTown_BrendansHouse_1F_EventScript_SetMovingBoxes
	call_if_set FLAG_RECEIVED_RUNNING_SHOES, LittlerootTown_BrendansHouse_1F_EventScript_CheckShowShoesManual
	end

LittlerootTown_BrendansHouse_1F_EventScript_SetMovingBoxes::
	setmetatile 5, 4, METATILE_BrendansMaysHouse_MovingBox_Open, TRUE
	setmetatile 5, 2, METATILE_BrendansMaysHouse_MovingBox_Closed, TRUE
	return

LittlerootTown_BrendansHouse_1F_EventScript_CheckShowShoesManual::
	checkplayergender
	goto_if_eq VAR_RESULT, MALE, LittlerootTown_BrendansHouse_1F_EventScript_ShowRunningShoesManual
	return

LittlerootTown_BrendansHouse_1F_EventScript_ShowRunningShoesManual::
	setmetatile 3, 7, METATILE_BrendansMaysHouse_BookOnTable, TRUE
	return

LittlerootTown_BrendansHouse_1F_OnTransition:
	call_if_eq VAR_LITTLEROOT_INTRO_STATE, 3, LittlerootTown_BrendansHouse_1F_EventScript_MoveMomToDoor
	call_if_eq VAR_LITTLEROOT_INTRO_STATE, 5, LittlerootTown_BrendansHouse_1F_EventScript_MoveMomToStairs
	call_if_eq VAR_LITTLEROOT_INTRO_STATE, 6, LittlerootTown_BrendansHouse_1F_EventScript_MoveMomToTV
	end

LittlerootTown_BrendansHouse_1F_EventScript_MoveMomToStairs::
	setobjectxyperm LOCALID_PLAYERS_HOUSE_1F_MOM, 8, 4
	setobjectmovementtype LOCALID_PLAYERS_HOUSE_1F_MOM, MOVEMENT_TYPE_FACE_UP
	return

LittlerootTown_BrendansHouse_1F_EventScript_MoveMomToTV::
	setobjectxyperm LOCALID_PLAYERS_HOUSE_1F_MOM, 4, 5
	setobjectmovementtype LOCALID_PLAYERS_HOUSE_1F_MOM, MOVEMENT_TYPE_FACE_UP
	return

LittlerootTown_BrendansHouse_1F_EventScript_MoveMomToDoor::
	setobjectxyperm LOCALID_PLAYERS_HOUSE_1F_MOM, 9, 8
	setobjectmovementtype LOCALID_PLAYERS_HOUSE_1F_MOM, MOVEMENT_TYPE_FACE_UP
	return

@ Many of the below scripts have no gender check because they assume youre in the correct house
@ The below SS Ticket script uses Mays house state by accident(?), but theyre both set identically after the intro
LittlerootTown_BrendansHouse_1F_OnFrame:
	map_script_2 VAR_LITTLEROOT_INTRO_STATE, 3, LittlerootTown_BrendansHouse_1F_EventScript_EnterHouseMovingIn
	map_script_2 VAR_LITTLEROOT_INTRO_STATE, 5, LittlerootTown_BrendansHouse_1F_EventScript_GoUpstairsToSetClock
	map_script_2 VAR_LITTLEROOT_INTRO_STATE, 6, LittlerootTown_BrendansHouse_1F_EventScript_PetalburgGymReport
	map_script_2 VAR_LITTLEROOT_HOUSES_STATE_MAY, 1, LittlerootTown_BrendansHouse_1F_EventScript_YoureNewNeighbor
	map_script_2 VAR_LITTLEROOT_HOUSES_STATE_MAY, 3, PlayersHouse_1F_EventScript_GetSSTicketAndSeeLatiTV
	.2byte 0

LittlerootTown_BrendansHouse_1F_EventScript_GoUpstairsToSetClock::
	lockall
	msgbox PlayersHouse_1F_Text_GoSetTheClock, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_PLAYER, LittlerootTown_BrendansHouse_1F_Movement_PushTowardStairs
	applymovement LOCALID_PLAYERS_HOUSE_1F_MOM, LittlerootTown_BrendansHouse_1F_Movement_PushTowardStairs
	waitmovement 0
	warp MAP_LITTLEROOT_TOWN_BRENDANS_HOUSE_2F, 7, 1
	waitstate
	releaseall
	end

LittlerootTown_BrendansHouse_1F_Movement_PushTowardStairs:
	walk_up
	step_end

LittlerootTown_BrendansHouse_1F_EventScript_EnterHouseMovingIn::
	lockall
	setvar VAR_0x8004, LOCALID_PLAYERS_HOUSE_1F_MOM
	setvar VAR_0x8005, MALE
	goto PlayersHouse_1F_EventScript_EnterHouseMovingIn
	end

LittlerootTown_BrendansHouse_1F_EventScript_PetalburgGymReport::
	lockall
	setvar VAR_0x8004, MALE
	setvar VAR_0x8005, LOCALID_PLAYERS_HOUSE_1F_MOM
	goto PlayersHouse_1F_EventScript_PetalburgGymReportMale
	end

LittlerootTown_BrendansHouse_1F_EventScript_YoureNewNeighbor::
	lockall
	playse SE_PIN
	applymovement LOCALID_RIVALS_HOUSE_1F_MOM, Common_Movement_ExclamationMark
	waitmovement 0
	applymovement LOCALID_RIVALS_HOUSE_1F_MOM, Common_Movement_Delay48
	waitmovement 0
	applymovement LOCALID_PLAYER, Common_Movement_WalkInPlaceFasterLeft
	applymovement LOCALID_RIVALS_HOUSE_1F_MOM, LittlerootTown_BrendansHouse_1F_Movement_RivalMomApproach
	waitmovement 0
	special GetRivalSonDaughterString
	msgbox RivalsHouse_1F_Text_OhYoureTheNewNeighbor, MSGBOX_DEFAULT
	setflag FLAG_MET_RIVAL_MOM
	setvar VAR_LITTLEROOT_HOUSES_STATE_MAY, 2
	releaseall
	end

LittlerootTown_BrendansHouse_1F_Movement_RivalMomApproach:
	walk_down
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	step_end

LittlerootTown_BrendansHouse_1F_EventScript_GoSeeRoom::
	lockall
	setvar VAR_0x8004, LOCALID_PLAYERS_HOUSE_1F_MOM
	setvar VAR_0x8005, MALE
	applymovement VAR_0x8004, Common_Movement_WalkInPlaceFasterLeft
	waitmovement 0
	goto PlayersHouse_1F_EventScript_MomGoSeeRoom
	end

LittlerootTown_BrendansHouse_1F_EventScript_MeetRival0::
	lockall
	setvar VAR_0x8008, 0
	goto LittlerootTown_BrendansHouse_1F_EventScript_MeetRival
	end

LittlerootTown_BrendansHouse_1F_EventScript_MeetRival1::
	lockall
	setvar VAR_0x8008, 1
	goto LittlerootTown_BrendansHouse_1F_EventScript_MeetRival
	end

LittlerootTown_BrendansHouse_1F_EventScript_MeetRival2::
	lockall
	setvar VAR_0x8008, 2
	goto LittlerootTown_BrendansHouse_1F_EventScript_MeetRival
	end

LittlerootTown_BrendansHouse_1F_EventScript_MeetRival::
	playse SE_EXIT
	delay 10
	addobject LOCALID_RIVALS_HOUSE_1F_RIVAL
	delay 30
	playse SE_PIN
	applymovement LOCALID_RIVALS_HOUSE_1F_RIVAL, Common_Movement_ExclamationMark
	waitmovement 0
	applymovement LOCALID_RIVALS_HOUSE_1F_RIVAL, Common_Movement_Delay48
	waitmovement 0
	call_if_ne VAR_0x8008, 1, LittlerootTown_BrendansHouse_1F_EventScript_PlayerFaceBrendan
	playbgm MUS_ENCOUNTER_BRENDAN, TRUE
	call_if_eq VAR_0x8008, 0, LittlerootTown_BrendansHouse_1F_EventScript_BrendanApproachPlayer0
	call_if_eq VAR_0x8008, 1, LittlerootTown_BrendansHouse_1F_EventScript_BrendanApproachPlayer1
	call_if_eq VAR_0x8008, 2, LittlerootTown_BrendansHouse_1F_EventScript_BrendanApproachPlayer2
	msgbox RivalsHouse_1F_Text_BrendanWhoAreYou, MSGBOX_DEFAULT
	closemessage
	call_if_eq VAR_0x8008, 0, LittlerootTown_BrendansHouse_1F_EventScript_BrendanGoUpstairs0
	call_if_eq VAR_0x8008, 1, LittlerootTown_BrendansHouse_1F_EventScript_BrendanGoUpstairs1
	call_if_eq VAR_0x8008, 2, LittlerootTown_BrendansHouse_1F_EventScript_BrendanGoUpstairs2
	playse SE_EXIT
	removeobject LOCALID_RIVALS_HOUSE_1F_RIVAL
	setflag FLAG_HIDE_LITTLEROOT_TOWN_BRENDANS_HOUSE_BRENDAN
	setflag FLAG_HIDE_LITTLEROOT_TOWN_BRENDANS_HOUSE_2F_POKE_BALL
	clearflag FLAG_HIDE_LITTLEROOT_TOWN_BRENDANS_HOUSE_RIVAL_BEDROOM
	delay 30
	setvar VAR_LITTLEROOT_RIVAL_STATE, 3
	setvar VAR_LITTLEROOT_TOWN_STATE, 1
	savebgm MUS_DUMMY
	fadedefaultbgm
	releaseall
	end

LittlerootTown_BrendansHouse_1F_EventScript_PlayerFaceBrendan::
	applymovement LOCALID_PLAYER, Common_Movement_WalkInPlaceFasterDown
	waitmovement 0
	return

LittlerootTown_BrendansHouse_1F_EventScript_BrendanApproachPlayer0::
	applymovement LOCALID_RIVALS_HOUSE_1F_RIVAL, LittlerootTown_BrendansHouse_1F_Movement_BrendanApproachPlayer0
	waitmovement 0
	return

LittlerootTown_BrendansHouse_1F_EventScript_BrendanApproachPlayer1::
	applymovement LOCALID_RIVALS_HOUSE_1F_RIVAL, LittlerootTown_BrendansHouse_1F_Movement_BrendanApproachPlayer1
	waitmovement 0
	return

LittlerootTown_BrendansHouse_1F_EventScript_BrendanApproachPlayer2::
	applymovement LOCALID_RIVALS_HOUSE_1F_RIVAL, LittlerootTown_BrendansHouse_1F_Movement_BrendanApproachPlayer2
	waitmovement 0
	return

LittlerootTown_BrendansHouse_1F_Movement_BrendanApproachPlayer0:
	walk_in_place_faster_left
	walk_left
	walk_in_place_faster_up
	walk_up
	walk_up
	walk_up
	walk_up
	step_end

LittlerootTown_BrendansHouse_1F_Movement_BrendanApproachPlayer1:
	walk_up
	walk_up
	walk_up
	step_end

LittlerootTown_BrendansHouse_1F_Movement_BrendanApproachPlayer2:
	walk_in_place_faster_right
	walk_right
	walk_in_place_faster_up
	walk_up
	walk_up
	walk_up
	walk_up
	step_end

LittlerootTown_BrendansHouse_1F_EventScript_BrendanGoUpstairs0::
	applymovement LOCALID_PLAYER, LittlerootTown_BrendansHouse_1F_Movement_PlayerWatchBrendanExit0
	applymovement LOCALID_RIVALS_HOUSE_1F_RIVAL, LittlerootTown_BrendansHouse_1F_Movement_BrendanGoUpstairs0
	waitmovement 0
	return

LittlerootTown_BrendansHouse_1F_EventScript_BrendanGoUpstairs1::
	applymovement LOCALID_PLAYER, LittlerootTown_BrendansHouse_1F_Movement_PlayerWatchBrendanExit1
	applymovement LOCALID_RIVALS_HOUSE_1F_RIVAL, LittlerootTown_BrendansHouse_1F_Movement_BrendanGoUpstairs1
	waitmovement 0
	return

LittlerootTown_BrendansHouse_1F_EventScript_BrendanGoUpstairs2::
	applymovement LOCALID_PLAYER, LittlerootTown_BrendansHouse_1F_Movement_PlayerWatchBrendanExit2
	applymovement LOCALID_RIVALS_HOUSE_1F_RIVAL, LittlerootTown_BrendansHouse_1F_Movement_BrendanGoUpstairs2
	waitmovement 0
	return

LittlerootTown_BrendansHouse_1F_Movement_PlayerWatchBrendanExit0:
	delay_16
	delay_8
	walk_in_place_faster_right
	step_end

LittlerootTown_BrendansHouse_1F_Movement_PlayerWatchBrendanExit1:
	delay_16
	delay_8
	walk_in_place_faster_right
	delay_16
	delay_16
	walk_in_place_faster_up
	step_end

LittlerootTown_BrendansHouse_1F_Movement_PlayerWatchBrendanExit2:
	delay_16
	delay_8
	walk_in_place_faster_left
	step_end

LittlerootTown_BrendansHouse_1F_Movement_BrendanGoUpstairs0:
	walk_in_place_faster_right
	walk_right
	walk_in_place_faster_up
	walk_up
	walk_up
	step_end

LittlerootTown_BrendansHouse_1F_Movement_BrendanGoUpstairs1:
	walk_in_place_faster_right
	walk_right
	walk_in_place_faster_up
	walk_up
	walk_up
	walk_in_place_faster_left
	walk_left
	walk_in_place_faster_up
	walk_up
	step_end

LittlerootTown_BrendansHouse_1F_Movement_BrendanGoUpstairs2:
	walk_in_place_faster_left
	walk_left
	walk_in_place_faster_up
	walk_up
	walk_up
	step_end

PlayersHouse_1F_Text_IsntItNiceInHere:
	.string "Maman : Regarde, {PLAYER}. C'est joli, non\n"
	.string "? Ça sent le neuf.$"

PlayersHouse_1F_Text_MoversPokemonGoSetClock:
	.string "Les Pokémon des déménageurs ont tout\n"
	.string "porté et tout nettoyé. Moi, je n'ai eu\l"
	.string "qu'à pointer du doigt.\p"
	.string "{PLAYER}, ta chambre est en haut. Va\n"
	.string "jeter un œil.\p"
	.string "Ton père t'a offert une horloge pour le\n"
	.string "déménagement. C'est sa façon de dire\l"
	.string "“je pense à toi” sans être là. Règle-la.$"

PlayersHouse_1F_Text_ArentYouInterestedInRoom:
	.string "Maman : Alors, {PLAYER} ?\p"
	.string "Ta chambre ne t'intéresse pas ? Tu as\n"
	.string "attendu ça pendant des années.$"

PlayersHouse_1F_Text_GoSetTheClock:
	.string "Maman : {PLAYER}.\p"
	.string "Va régler l'horloge dans ta chambre, mon\n"
	.string "cœur.$"

PlayersHouse_1F_Text_OhComeQuickly:
	.string "Maman : Oh ! {PLAYER}, {PLAYER} ! Vite !\n"
	.string "Viens vite !$"

PlayersHouse_1F_Text_MaybeDadWillBeOn:
	.string "Maman : Regarde ! L'Arène de\n"
	.string "Clémenti-Ville ! On va peut-être voir\l"
	.string "ton père !$"

PlayersHouse_1F_Text_ItsOverWeMissedHim:
	.string "Maman : Oh… C'est fini.\p"
	.string "Il est passé, je crois. Et on l'a raté.\n"
	.string "Comme d'habitude.$"

PlayersHouse_1F_Text_GoIntroduceYourselfNextDoor:
	.string "Ah, oui. Un ami de ton père vit ici.\p"
	.string "Le Prof. Seko.\p"
	.string "C'est juste à côté. Va te présenter. Il\n"
	.string "sait des choses sur ton père que même\l"
	.string "moi j'ignore.$"

PlayersHouse_1F_Text_SeeYouHoney:
	.string "Maman : À tout à l'heure, mon cœur !$"

PlayersHouse_1F_Text_DidYouMeetProfBirch:
	.string "Maman : Tu es allé te présenter au Prof.\n"
	.string "Seko ?$"

PlayersHouse_1F_Text_YouShouldRestABit:
	.string "Maman : Ça va, {PLAYER} ? Tu as une mine\n"
	.string "affreuse.\p"
	.string "Repose-toi un peu. Le monde ne va pas\n"
	.string "s'écrouler en une nuit.$"

PlayersHouse_1F_Text_TakeCareHoney:
	.string "Maman : Fais attention à toi !$"

PlayersHouse_1F_Text_GotDadsBadgeHeresSomethingFromMom:
	.string "Maman : Oh ? C'est ton père qui t'a donné\n"
	.string "ce Badge ?\p"
	.string "Il t'a enfin regardé en face, alors.\n"
	.string "Tiens, voilà quelque chose de la part de\l"
	.string "ta mère.$"

PlayersHouse_1F_Text_DontPushYourselfTooHard:
	.string "Ne te tue pas à la tâche. Je ne le\n"
	.string "supporterais pas.\p"
	.string "Tu peux toujours rentrer. Allez, fonce.$"

PlayersHouse_1F_Text_IsThatAPokenav:
	.string "Maman : C'est quoi, ce truc ? Un PokéNav\n"
	.string "? Quelqu'un de Devon te l'a donné ?\p"
	.string "Alors enregistre ta mère dedans. Comme\n"
	.string "ça, je saurai que tu es en vie.\p"
	.string "… … …$"

PlayersHouse_1F_Text_RegisteredMom:
	.string "Maman a été enregistrée dans le\n"
	.string "PokéNav.$"

PlayersHouse_1F_Text_Vigoroth1:
	.string "Fugiiiiih !$"

PlayersHouse_1F_Text_Vigoroth2:
	.string "Huggoh, uggo uggo…$"

PlayersHouse_1F_Text_ReportFromPetalburgGym:
	.string "Journaliste : …Nous sommes en direct\n"
	.string "devant l'Arène de Clémenti-Ville.$"

PlayersHouse_1F_Text_TheresAMovieOnTV:
	.string "Un film passe à la télé.\p"
	.string "Deux types se battent à mains nues sur\n"
	.string "un pont qui s'effondre.\p"
	.string "Pas le temps de regarder la fin.$"

PlayersHouse_1F_Text_RunningShoesManual:
	.string "La notice des Chaussures de sport.\p"
	.string "“Maintenez le Bouton B pour courir.”\p"
	.string "“Lacez-les bien. Ce qui vous poursuit\n"
	.string "ne s'arrêtera pas pour un lacet défait.”$"

PlayersHouse_1F_Text_TicketFromBrineyCameForYou:
	.string "Papa : Hm ?\p"
	.string "Tiens, {PLAYER}.\p"
	.string "Ça fait un bail. Tu as… changé. Tu as\n"
	.string "l'air plus solide.\p"
	.string "C'est l'impression que tu me donnes.\n"
	.string "Mais ton vieux n'a pas encore dit son\l"
	.string "dernier mot.\p"
	.string "Ah, oui. Ceci est arrivé pour toi. De la\n"
	.string "part d'un certain M. Marco.$"

PlayersHouse_1F_Text_PortsInSlateportLilycove:
	.string "Papa : Hm, un billet de ferry ?\p"
	.string "Si je me souviens bien, il y a des ports à\n"
	.string "Poivressel et à Nénucrique.$"

PlayersHouse_1F_Text_BetterGetBackToGym:
	.string "Je dois retourner à l'Arène de\n"
	.string "Clémenti-Ville.\p"
	.string "Merci de tenir la maison pendant que je\n"
	.string "ne suis pas là. Encore une fois.$"

PlayersHouse_1F_Text_DadShouldStayLonger:
	.string "Maman : Ton père…\p"
	.string "Il rentre pour la première fois depuis\n"
	.string "des mois et il ne parle que de Pokémon.\p"
	.string "Un jour, il rentrera et il n'y aura plus\n"
	.string "personne pour l'écouter.$"

PlayersHouse_1F_Text_IsThatABreakingStory:
	.string "Maman : C'est un flash spécial ?$"

PlayersHouse_1F_Text_LatiEmergencyNewsFlash:
	.string "Flash d'information urgent !\p"
	.string "Partout dans Hoenn, des témoins\n"
	.string "signalent un Pokémon BZZT… en plein\l"
	.string "vol.\p"
	.string "Son identité reste inconnue. Les\n"
	.string "autorités demandent de ne pas\l"
	.string "l'approcher.\p"
	.string "Nous reprenons le cours normal de nos\n"
	.string "programmes.$"

PlayersHouse_1F_Text_WhatColorDidTheySay:
	.string "Maman : {PLAYER}, tu as entendu ?\p"
	.string "Il a dit quelle couleur, ce Pokémon ?$"

PlayersHouse_1F_Text_StillUnknownPokemon:
	.string "Maman : Eh ben… Il reste encore des\n"
	.string "Pokémon que personne ne connaît. Ça me\l"
	.string "rassure et ça m'inquiète à la fois.$"
