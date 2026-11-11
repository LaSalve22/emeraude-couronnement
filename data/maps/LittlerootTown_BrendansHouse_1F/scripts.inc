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
	.string "Maman : Regarde, {PLAYER}. C'est mignon,\n"
	.string "non ? Ça sent la peinture fraîche et les\l"
	.string "nouveaux départs.$"

PlayersHouse_1F_Text_MoversPokemonGoSetClock:
	.string "Les Machoc des déménageurs ont tout\n"
	.string "monté en dix minutes. Moi, j'ai juste\l"
	.string "pointé du doigt et bu un café.\p"
	.string "{PLAYER}, ta chambre est en haut. Va\n"
	.string "voir !\p"
	.string "Ton père t'a offert une horloge pour\n"
	.string "l'emménagement. Il n'est pas là, mais il a\l"
	.string "envoyé une horloge. Tout lui ressemble.\p"
	.string "Va la régler. Que le temps reparte, au\n"
	.string "moins ici.$"

PlayersHouse_1F_Text_ArentYouInterestedInRoom:
	.string "Maman : Alors, {PLAYER} ?\p"
	.string "Ta chambre ne t'intéresse pas ? Tu m'as\n"
	.string "saoulée avec ça pendant tout le trajet.$"

PlayersHouse_1F_Text_GoSetTheClock:
	.string "Maman : {PLAYER}.\p"
	.string "L'horloge. Dans ta chambre. Maintenant,\n"
	.string "s'il te plaît.$"

PlayersHouse_1F_Text_OhComeQuickly:
	.string "Maman : {PLAYER} ! {PLAYER} ! Vite ! Viens\n"
	.string "vite !$"

PlayersHouse_1F_Text_MaybeDadWillBeOn:
	.string "Maman : Regarde ! C'est l'Arène de\n"
	.string "Clémenti-Ville ! On va peut-être voir\l"
	.string "ton père !$"

PlayersHouse_1F_Text_ItsOverWeMissedHim:
	.string "Maman : Oh… C'est fini.\p"
	.string "Il était là, je crois. Et on l'a raté.\p"
	.string "…Comme les anniversaires. Et Noël. Et à\n"
	.string "peu près tout le reste.$"

PlayersHouse_1F_Text_GoIntroduceYourselfNextDoor:
	.string "Ah, oui. Un ami de ton père habite ici.\p"
	.string "Le Prof. Seko. Juste à côté.\p"
	.string "Va te présenter. Il est gentil. Un peu\n"
	.string "fou. Il sent souvent le Pokémon mouillé,\l"
	.string "mais il est gentil.$"

PlayersHouse_1F_Text_SeeYouHoney:
	.string "Maman : À tout à l'heure, mon cœur !$"

PlayersHouse_1F_Text_DidYouMeetProfBirch:
	.string "Maman : Tu as vu le Prof. Seko ? Il avait\n"
	.string "encore ses deux bras ?$"

PlayersHouse_1F_Text_YouShouldRestABit:
	.string "Maman : Ça va, {PLAYER} ? Tu as une mine\n"
	.string "de zombie.\p"
	.string "Repose-toi. Le monde ne va pas exploser\n"
	.string "pendant ta sieste. Enfin, j'espère.$"

PlayersHouse_1F_Text_TakeCareHoney:
	.string "Maman : Fais attention à toi !$"

PlayersHouse_1F_Text_GotDadsBadgeHeresSomethingFromMom:
	.string "Maman : Oh ? C'est ton père qui t'a donné\n"
	.string "ce Badge ?\p"
	.string "Il a fini par te regarder dans les yeux,\n"
	.string "alors.\p"
	.string "Tiens. Ça, c'est de ma part. Les mères\n"
	.string "aussi distribuent des récompenses.$"

PlayersHouse_1F_Text_DontPushYourselfTooHard:
	.string "Ne te tue pas à la tâche. J'ai déjà un\n"
	.string "homme dans cette famille qui fait ça\l"
	.string "très bien.\p"
	.string "Tu peux toujours rentrer. Allez, fonce.$"

PlayersHouse_1F_Text_IsThatAPokenav:
	.string "Maman : C'est quoi, ce machin ? Un\n"
	.string "PokéNav ? Quelqu'un de Devon te l'a\l"
	.string "donné ?\p"
	.string "Parfait. Enregistre-moi. Comme ça,\n"
	.string "quand tu ne m'appelles pas, je saurai\l"
	.string "que c'est exprès.\p"
	.string "… … …$"

PlayersHouse_1F_Text_RegisteredMom:
	.string "Maman a été enregistrée dans le\n"
	.string "PokéNav.$"

PlayersHouse_1F_Text_Vigoroth1:
	.string "Fugiiiiih !$"

PlayersHouse_1F_Text_Vigoroth2:
	.string "Huggoh, uggo uggo… (Il a cassé une\n"
	.string "assiette et fait semblant de rien.)$"

PlayersHouse_1F_Text_ReportFromPetalburgGym:
	.string "Journaliste : …Nous sommes en direct\n"
	.string "devant l'Arène de Clémenti-Ville.$"

PlayersHouse_1F_Text_TheresAMovieOnTV:
	.string "Un film passe à la télé.\p"
	.string "Deux types se hurlent dessus sous la\n"
	.string "pluie, puis s'embrassent… non, se\l"
	.string "mettent un coup de boule. Classique.\p"
	.string "Pas le temps de voir la fin.$"

PlayersHouse_1F_Text_RunningShoesManual:
	.string "La notice des Chaussures de sport.\p"
	.string "“Maintenez le Bouton B pour courir.”\p"
	.string "“Courir ne résout pas vos problèmes.\n"
	.string "Mais ça les fait passer plus vite.”$"

PlayersHouse_1F_Text_TicketFromBrineyCameForYou:
	.string "Papa : Hm ?\p"
	.string "Tiens. {PLAYER}.\p"
	.string "…\p"
	.string "Tu as changé. Tu tiens debout\n"
	.string "autrement. Comme quelqu'un qui a déjà\l"
	.string "pris des coups.\p"
	.string "Ça me plaît. Mais ton vieux n'est pas\n"
	.string "encore à la retraite.\p"
	.string "Ah. Ceci est arrivé pour toi. De la part\n"
	.string "d'un certain M. Marco.$"

PlayersHouse_1F_Text_PortsInSlateportLilycove:
	.string "Papa : Hm. Un billet de ferry.\p"
	.string "Il y a des ports à Poivressel et à\n"
	.string "Nénucrique.$"

PlayersHouse_1F_Text_BetterGetBackToGym:
	.string "Je dois retourner à l'Arène.\p"
	.string "Merci de tenir la maison. Je sais. Je sais\n"
	.string "que je dis ça à chaque fois.$"

PlayersHouse_1F_Text_DadShouldStayLonger:
	.string "Maman : Ton père…\p"
	.string "Il rentre pour la première fois en trois\n"
	.string "mois et il me parle de taux de coups\l"
	.string "critiques pendant le dîner.\p"
	.string "…Un jour, il rentrera et la maison sera\n"
	.string "vide. Et il ne comprendra même pas\l"
	.string "pourquoi.$"

PlayersHouse_1F_Text_IsThatABreakingStory:
	.string "Maman : C'est un flash spécial ?$"

PlayersHouse_1F_Text_LatiEmergencyNewsFlash:
	.string "Flash spécial !\p"
	.string "Partout dans Hoenn, des témoins\n"
	.string "signalent un Pokémon BZZT… volant à\l"
	.string "une vitesse folle.\p"
	.string "Son identité est inconnue. Trois\n"
	.string "personnes ont déjà fini dans un arbre\l"
	.string "en essayant de le prendre en photo.\p"
	.string "Nous reprenons le cours normal de nos\n"
	.string "programmes.$"

PlayersHouse_1F_Text_WhatColorDidTheySay:
	.string "Maman : {PLAYER}, tu as entendu ?\p"
	.string "Il a dit quelle couleur, ce Pokémon ?$"

PlayersHouse_1F_Text_StillUnknownPokemon:
	.string "Maman : Il reste des Pokémon que\n"
	.string "personne ne connaît… Le monde est\l"
	.string "encore plus grand que ce qu'on nous\n"
	.string "raconte. Ça fait du bien à entendre.$"
