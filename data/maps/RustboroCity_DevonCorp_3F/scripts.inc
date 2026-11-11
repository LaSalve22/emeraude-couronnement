RustboroCity_DevonCorp_3F_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, RustboroCity_DevonCorp_3F_OnTransition
	map_script MAP_SCRIPT_ON_WARP_INTO_MAP_TABLE, RustboroCity_DevonCorp_3F_OnWarp
	map_script MAP_SCRIPT_ON_FRAME_TABLE, RustboroCity_DevonCorp_3F_OnFrame
	.byte 0

RustboroCity_DevonCorp_3F_OnTransition:
	call_if_eq VAR_DEVON_CORP_3F_STATE, 0, RustboroCity_DevonCorp_3F_EventScript_SetEmployeePos
	end

RustboroCity_DevonCorp_3F_EventScript_SetEmployeePos::
	setobjectxyperm LOCALID_DEVON_CORP_3F_EMPLOYEE, 3, 2
	setobjectmovementtype LOCALID_DEVON_CORP_3F_EMPLOYEE, MOVEMENT_TYPE_FACE_LEFT
	return

RustboroCity_DevonCorp_3F_OnWarp:
	map_script_2 VAR_DEVON_CORP_3F_STATE, 0, RustboroCity_DevonCorp_3F_EventScript_PlayerFaceEast
	.2byte 0

RustboroCity_DevonCorp_3F_EventScript_PlayerFaceEast::
	turnobject LOCALID_PLAYER, DIR_EAST
	end

RustboroCity_DevonCorp_3F_OnFrame:
	map_script_2 VAR_DEVON_CORP_3F_STATE, 0, RustboroCity_DevonCorp_3F_EventScript_MeetPresident
	.2byte 0

RustboroCity_DevonCorp_3F_EventScript_MeetPresident::
	lockall
	msgbox RustboroCity_DevonCorp_3F_Text_ThisIs3rdFloorWaitHere, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_DEVON_CORP_3F_EMPLOYEE, RustboroCity_DevonCorp_3F_Movement_EmployeeWalkOffscreen
	waitmovement 0
	delay 80
	applymovement LOCALID_DEVON_CORP_3F_EMPLOYEE, RustboroCity_DevonCorp_3F_Movement_EmployeeReturnToPlayer
	waitmovement 0
	msgbox RustboroCity_DevonCorp_3F_Text_WordWithPresidentComeWithMe, MSGBOX_DEFAULT
	closemessage
	playbgm MUS_FOLLOW_ME, FALSE
	applymovement LOCALID_DEVON_CORP_3F_EMPLOYEE, RustboroCity_DevonCorp_3F_Movement_LeadPlayerToPresident
	applymovement LOCALID_PLAYER, RustboroCity_DevonCorp_3F_Movement_PlayerFollowToPresident
	waitmovement 0
	msgbox RustboroCity_DevonCorp_3F_Text_PleaseGoAhead, MSGBOX_DEFAULT
	closemessage
	fadedefaultbgm
	applymovement LOCALID_DEVON_CORP_3F_EMPLOYEE, RustboroCity_DevonCorp_3F_Movement_EmployeeFaceDesk
	applymovement LOCALID_PLAYER, RustboroCity_DevonCorp_3F_Movement_PlayerApproachDesk
	waitmovement 0
	msgbox RustboroCity_DevonCorp_3F_Text_MrStoneIHaveFavor, MSGBOX_DEFAULT
	giveitem ITEM_LETTER
	msgbox RustboroCity_DevonCorp_3F_Text_MrStoneWantYouToHaveThis, MSGBOX_DEFAULT
	playfanfare MUS_OBTAIN_ITEM
	message RustboroCity_DevonCorp_3F_Text_ReceivedPokenav
	waitfanfare
	setflag FLAG_SYS_POKENAV_GET
	setflag FLAG_RECEIVED_POKENAV
	setflag FLAG_HIDE_RUSTBORO_CITY_POKEMON_SCHOOL_SCOTT
	msgbox RustboroCity_DevonCorp_3F_Text_MrStoneExplainPokenavRestUp, MSGBOX_DEFAULT
	closemessage
	call Common_EventScript_OutOfCenterPartyHeal
	msgbox RustboroCity_DevonCorp_3F_Text_MrStoneGoWithCautionAndCare, MSGBOX_DEFAULT
	setflag FLAG_HIDE_ROUTE_116_WANDAS_BOYFRIEND
	clearflag FLAG_HIDE_RUSTURF_TUNNEL_WANDAS_BOYFRIEND
	clearflag FLAG_HIDE_RUSTURF_TUNNEL_WANDA
	clearflag FLAG_HIDE_BRINEYS_HOUSE_MR_BRINEY
	clearflag FLAG_HIDE_BRINEYS_HOUSE_PEEKO
	setvar VAR_BRINEY_LOCATION, 1
	clearflag FLAG_HIDE_RUSTBORO_CITY_RIVAL
	setvar VAR_DEVON_CORP_3F_STATE, 1
	setvar VAR_RUSTBORO_CITY_STATE, 6
	releaseall
	end

RustboroCity_DevonCorp_3F_Movement_Unused:
	walk_up
	walk_up
	walk_up
	walk_in_place_faster_left
	step_end

RustboroCity_DevonCorp_3F_Movement_LeadPlayerToPresident:
	walk_right
	walk_right
	walk_right
	walk_down
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	walk_in_place_faster_left
	step_end

RustboroCity_DevonCorp_3F_Movement_EmployeeFaceDesk:
	delay_16
	walk_in_place_faster_down
	step_end

RustboroCity_DevonCorp_3F_Movement_EmployeeWalkOffscreen:
	walk_right
	walk_right
	walk_right
	walk_down
	walk_right
	walk_right
	walk_right
	walk_right
	step_end

RustboroCity_DevonCorp_3F_Movement_EmployeeReturnToPlayer:
	walk_left
	walk_left
	walk_left
	walk_left
	walk_up
	walk_left
	walk_left
	walk_left
	step_end

RustboroCity_DevonCorp_3F_Movement_PlayerFollowToPresident:
	walk_right
	walk_right
	walk_right
	walk_right
	walk_down
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	step_end

RustboroCity_DevonCorp_3F_Movement_PlayerApproachDesk:
	delay_16
	delay_16
	walk_down
	walk_down
	walk_in_place_faster_right
	step_end

RustboroCity_DevonCorp_3F_EventScript_MrStone::
	lock
	faceplayer
	goto_if_set FLAG_RECEIVED_EXP_SHARE, RustboroCity_DevonCorp_3F_EventScript_MrStoneAfterFavor
	goto_if_set FLAG_DELIVERED_STEVEN_LETTER, RustboroCity_DevonCorp_3F_EventScript_GiveExpShare
	msgbox RustboroCity_DevonCorp_3F_Text_CountingOnYou, MSGBOX_DEFAULT
	closemessage
	applymovement VAR_LAST_TALKED, Common_Movement_FaceOriginalDirection
	waitmovement 0
	release
	end

RustboroCity_DevonCorp_3F_EventScript_GiveExpShare::
	msgbox RustboroCity_DevonCorp_3F_Text_ThankYouForDeliveringLetter, MSGBOX_DEFAULT
	giveitem ITEM_EXP_SHARE
	goto_if_eq VAR_RESULT, FALSE, Common_EventScript_ShowBagIsFull
	setflag FLAG_RECEIVED_EXP_SHARE
	msgbox RustboroCity_DevonCorp_3F_Text_ExplainExpShare, MSGBOX_DEFAULT
	closemessage
	applymovement VAR_LAST_TALKED, Common_Movement_FaceOriginalDirection
	waitmovement 0
	release
	end

RustboroCity_DevonCorp_3F_EventScript_MrStoneAfterFavor::
	msgbox RustboroCity_DevonCorp_3F_Text_NotFamiliarWithTrends, MSGBOX_DEFAULT
	closemessage
	applymovement VAR_LAST_TALKED, Common_Movement_FaceOriginalDirection
	waitmovement 0
	release
	end

RustboroCity_DevonCorp_3F_EventScript_Employee::
	lock
	faceplayer
	goto_if_set FLAG_RECEIVED_REPEAT_BALL, RustboroCity_DevonCorp_3F_EventScript_EmployeeBalls
	msgbox RustboroCity_DevonCorp_3F_Text_VisitCaptSternShipyard, MSGBOX_DEFAULT
	release
	end

RustboroCity_DevonCorp_3F_EventScript_EmployeeBalls::
	msgbox RustboroCity_DevonCorp_3F_Text_RepeatAndTimerHugelyPopular, MSGBOX_DEFAULT
	release
	end

RustboroCity_DevonCorp_3F_EventScript_RareRocksDisplay::
	msgbox RustboroCity_DevonCorp_3F_Text_RareRocksDisplay, MSGBOX_SIGN
	end

RustboroCity_DevonCorp_3F_Text_MrStoneIHaveFavor:
	.string "Je suis M. Rochard, Président de Devon\n"
	.string "SA.\p"
	.string "On vient de me parler de toi !\p"
	.string "Tu as sauvé mon employé, pas une fois,\n"
	.string "mais deux !\p"
	.string "J'ai un service à demander à quelqu'un\n"
	.string "d'aussi remarquable.\p"
	.string "Tu livres un colis au chantier naval de\n"
	.string "Poivressel, c'est bien ça ?\p"
	.string "En chemin, pourrais-tu faire escale au\n"
	.string "Village Myokara ?\p"
	.string "J'aimerais que tu remettes une Lettre à\n"
	.string "Pierre, à Myokara. Mon fils. Il ne répond\l"
	.string "plus à mes appels.$"

RustboroCity_DevonCorp_3F_Text_MrStoneWantYouToHaveThis:
	.string "M. Rochard : Sache que je suis un grand\n"
	.string "Président.\p"
	.string "Je ne serais jamais assez pingre pour\n"
	.string "demander un service sans rien offrir en\l"
	.string "retour.\p"
	.string "Voilà pourquoi je veux que tu aies ceci !$"

RustboroCity_DevonCorp_3F_Text_ReceivedPokenav:
	.string "{PLAYER} reçoit un PokéNav.$"

RustboroCity_DevonCorp_3F_Text_MrStoneExplainPokenavRestUp:
	.string "M. Rochard : Cet appareil…\p"
	.string "C'est un Navigateur Pokémon, ou\n"
	.string "PokéNav.\p"
	.string "Un outil indispensable pour tout\n"
	.string "Dresseur en voyage.\p"
	.string "Il contient une carte de Hoenn.\p"
	.string "Tu y trouveras facilement Myokara et\n"
	.string "Poivressel !\p"
	.string "Au fait, il paraît que des criminels,\n"
	.string "Magma et Aqua je crois, sèment le chaos\l"
	.string "un peu partout. Il y a déjà eu des\n"
	.string "disparitions.\p"
	.string "Repose-toi avant de partir. C'est un\n"
	.string "conseil.$"

RustboroCity_DevonCorp_3F_Text_MrStoneGoWithCautionAndCare:
	.string "M. Rochard : Bien, {PLAYER}. Sois prudent.\n"
	.string "Vraiment.$"

RustboroCity_DevonCorp_3F_Text_CountingOnYou:
	.string "M. Rochard : Je compte sur toi !$"

RustboroCity_DevonCorp_3F_Text_ThankYouForDeliveringLetter:
	.string "M. Rochard : Tu as remis ma Lettre ?\n"
	.string "Merci infiniment !\p"
	.string "Voici pour te remercier. Ça devrait\n"
	.string "servir à un Dresseur comme toi.$"

RustboroCity_DevonCorp_3F_Text_ExplainExpShare:
	.string "M. Rochard : Un Pokémon qui tient ce\n"
	.string "Multi Exp reçoit une partie des points\l"
	.string "d'expérience du combat.\p"
	.string "Même s'il n'a pas combattu.\p"
	.string "C'est très utile pour entraîner les\n"
	.string "Pokémon faibles.$"

RustboroCity_DevonCorp_3F_Text_NotFamiliarWithTrends:
	.string "M. Rochard : Depuis ma jeunesse, je ne\n"
	.string "vis que pour le travail.\p"
	.string "Du coup, je ne connais rien aux modes. Ni\n"
	.string "à mon propre fils, d'ailleurs.\p"
	.string "Mais dis-moi, tous les jeunes veulent\n"
	.string "devenir Dresseurs à la Ligue Pokémon ?$"

RustboroCity_DevonCorp_3F_Text_ThisIs3rdFloorWaitHere:
	.string "Nous sommes au deuxième étage de Devon\n"
	.string "SA.\p"
	.string "Le bureau du Président se trouve ici.\p"
	.string "Je ne te remercierai jamais assez pour\n"
	.string "ce que tu as fait.\p"
	.string "Euh… Au fait, le colis que tu nous as\n"
	.string "rapporté…\p"
	.string "Tu pourrais le livrer au chantier naval\n"
	.string "de Poivressel ?\p"
	.string "Ce serait terrible si ces voleurs\n"
	.string "remettaient la main dessus. Des gens\l"
	.string "pourraient mourir.\p"
	.string "Euh… Hé hé… Je peux compter sur toi ?\p"
	.string "Ah, c'est vrai. Attends ici une seconde,\n"
	.string "tu veux ?$"

RustboroCity_DevonCorp_3F_Text_WordWithPresidentComeWithMe:
	.string "Notre Président souhaite te parler.\p"
	.string "Suis-moi, s'il te plaît.$"

RustboroCity_DevonCorp_3F_Text_PleaseGoAhead:
	.string "Je t'en prie, entre.$"

RustboroCity_DevonCorp_3F_Text_VisitCaptSternShipyard:
	.string "Au chantier naval de Poivressel, va voir\n"
	.string "le Capitaine Stern.$"

RustboroCity_DevonCorp_3F_Text_RepeatAndTimerHugelyPopular:
	.string "Nos nouveaux produits, la Bis Ball et la\n"
	.string "Chrono Ball, font un tabac chez les\l"
	.string "Dresseurs.$"

RustboroCity_DevonCorp_3F_Text_RareRocksDisplay:
	.string "Une collection de pierres rares réunie\n"
	.string "par le Président. Certaines ont été\l"
	.string "payées plus cher que des vies.$"

