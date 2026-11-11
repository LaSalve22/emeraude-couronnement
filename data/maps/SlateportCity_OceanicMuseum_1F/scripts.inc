SlateportCity_OceanicMuseum_1F_MapScripts::
	.byte 0

SlateportCity_OceanicMuseum_1F_EventScript_EntranceAttendant::
	msgbox SlateportCity_OceanicMuseum_1F_Text_PleaseEnjoyYourself, MSGBOX_NPC
	end

SlateportCity_OceanicMuseum_1F_EventScript_PayEntranceFeeLeft::
	lockall
	applymovement LOCALID_PLAYER, Common_Movement_WalkInPlaceFasterLeft
	waitmovement 0
	goto SlateportCity_OceanicMuseum_1F_EventScript_PayEntranceFee
	end

SlateportCity_OceanicMuseum_1F_EventScript_PayEntranceFeeRight::
	lockall
	applymovement LOCALID_PLAYER, Common_Movement_WalkInPlaceFasterRight
	waitmovement 0
	goto SlateportCity_OceanicMuseum_1F_EventScript_PayEntranceFee
	end

SlateportCity_OceanicMuseum_1F_EventScript_PayEntranceFee::
	showmoneybox 0, 0
	msgbox SlateportCity_OceanicMuseum_1F_Text_WouldYouLikeToEnter, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, YES, SlateportCity_OceanicMuseum_1F_EventScript_CheckMoneyForFee
	closemessage
	hidemoneybox
	applymovement LOCALID_PLAYER, SlateportCity_OceanicMuseum_1F_Movement_PushPlayerBackFromCounter
	waitmovement 0
	releaseall
	end

SlateportCity_OceanicMuseum_1F_EventScript_CheckMoneyForFee::
	checkmoney 50
	goto_if_eq VAR_RESULT, FALSE, SlateportCity_OceanicMuseum_1F_EventScript_NotEnoughMoney
	playse SE_SHOP
	removemoney 50
	updatemoneybox
	msgbox SlateportCity_OceanicMuseum_1F_Text_PleaseEnjoyYourself, MSGBOX_DEFAULT
	setvar VAR_SLATEPORT_MUSEUM_1F_STATE, 1
	hidemoneybox
	releaseall
	end

SlateportCity_OceanicMuseum_1F_EventScript_NotEnoughMoney::
	goto_if_unset FLAG_DELIVERED_DEVON_GOODS, SlateportCity_OceanicMuseum_1F_EventScript_AllowEntranceAnyway
	msgbox SlateportCity_OceanicMuseum_1F_Text_NotEnoughMoney, MSGBOX_DEFAULT
	closemessage
	hidemoneybox
	applymovement LOCALID_PLAYER, SlateportCity_OceanicMuseum_1F_Movement_PushPlayerBackFromCounter
	waitmovement 0
	releaseall
	end

SlateportCity_OceanicMuseum_1F_EventScript_AllowEntranceAnyway::
	msgbox SlateportCity_OceanicMuseum_1F_Text_CatchUpWithYourGroup, MSGBOX_DEFAULT
	setvar VAR_SLATEPORT_MUSEUM_1F_STATE, 1
	hidemoneybox
	releaseall
	end

SlateportCity_OceanicMuseum_1F_Movement_PushPlayerBackFromCounter:
	walk_down
	step_end

SlateportCity_OceanicMuseum_1F_EventScript_MuseumGrunt1::
	msgbox SlateportCity_OceanicMuseum_1F_Text_AquaExistForGoodOfAll, MSGBOX_NPC
	end

SlateportCity_OceanicMuseum_1F_EventScript_MuseumGrunt2::
	msgbox SlateportCity_OceanicMuseum_1F_Text_OurBossIsntHere, MSGBOX_NPC
	end

SlateportCity_OceanicMuseum_1F_EventScript_MuseumGrunt3::
	msgbox SlateportCity_OceanicMuseum_1F_Text_WouldStuffHereMakeMeRich, MSGBOX_NPC
	end

SlateportCity_OceanicMuseum_1F_EventScript_MuseumGrunt4::
	msgbox SlateportCity_OceanicMuseum_1F_Text_CanLearnForNefariousDeeds, MSGBOX_SIGN
	end

SlateportCity_OceanicMuseum_1F_EventScript_MuseumGrunt5::
	msgbox SlateportCity_OceanicMuseum_1F_Text_RustboroBungled, MSGBOX_NPC
	end

SlateportCity_OceanicMuseum_1F_EventScript_MuseumGrunt6::
	msgbox SlateportCity_OceanicMuseum_1F_Text_DidntHaveMoney, MSGBOX_NPC
	end

SlateportCity_OceanicMuseum_1F_EventScript_WhirlpoolExperiment::
	msgbox SlateportCity_OceanicMuseum_1F_Text_WhirlpoolExperiment, MSGBOX_SIGN
	end

SlateportCity_OceanicMuseum_1F_EventScript_WaterfallExperiment::
	msgbox SlateportCity_OceanicMuseum_1F_Text_WaterfallExperiment, MSGBOX_SIGN
	end

SlateportCity_OceanicMuseum_1F_EventScript_OceanSoilDisplay::
	msgbox SlateportCity_OceanicMuseum_1F_Text_OceanSoilDisplay, MSGBOX_SIGN
	end

SlateportCity_OceanicMuseum_1F_EventScript_BeachSandDisplay::
	msgbox SlateportCity_OceanicMuseum_1F_Text_BeachSandDisplay, MSGBOX_SIGN
	end

SlateportCity_OceanicMuseum_1F_EventScript_OceanicMinifact1::
	msgbox SlateportCity_OceanicMuseum_1F_Text_OceanicMinifact1, MSGBOX_SIGN
	end

SlateportCity_OceanicMuseum_1F_EventScript_OceanicMinifact2::
	msgbox SlateportCity_OceanicMuseum_1F_Text_OceanicMinifact2, MSGBOX_SIGN
	end

SlateportCity_OceanicMuseum_1F_EventScript_OceanicMinifact3::
	msgbox SlateportCity_OceanicMuseum_1F_Text_OceanicMinifact3, MSGBOX_SIGN
	end

SlateportCity_OceanicMuseum_1F_EventScript_FossilDisplay::
	msgbox SlateportCity_OceanicMuseum_1F_Text_FossilDisplay, MSGBOX_SIGN
	end

SlateportCity_OceanicMuseum_1F_EventScript_DepthMeasuringMachine::
	msgbox SlateportCity_OceanicMuseum_1F_Text_DepthMeasuringMachine, MSGBOX_SIGN
	end

SlateportCity_OceanicMuseum_1F_EventScript_MuseumPatron1::
	msgbox SlateportCity_OceanicMuseum_1F_Text_LearnAboutSeaForBattling, MSGBOX_NPC
	end

SlateportCity_OceanicMuseum_1F_EventScript_MuseumPatron2::
	msgbox SlateportCity_OceanicMuseum_1F_Text_SternIsRoleModel, MSGBOX_NPC
	end

SlateportCity_OceanicMuseum_1F_EventScript_MuseumPatron3::
	msgbox SlateportCity_OceanicMuseum_1F_Text_MustBePokemonWeDontKnow, MSGBOX_NPC
	end

SlateportCity_OceanicMuseum_1F_EventScript_MuseumPatron4::
	msgbox SlateportCity_OceanicMuseum_1F_Text_WantSeaPokemon, MSGBOX_NPC
	end

SlateportCity_OceanicMuseum_1F_EventScript_FamiliarGrunt::
	lock
	faceplayer
	delay 8
	playse SE_PIN
	applymovement LOCALID_OCEANIC_MUSEUM_FAMILIAR_GRUNT, Common_Movement_ExclamationMark
	waitmovement 0
	applymovement LOCALID_OCEANIC_MUSEUM_FAMILIAR_GRUNT, Common_Movement_Delay48
	waitmovement 0
	msgbox SlateportCity_OceanicMuseum_1F_Text_RememberMeTakeThis, MSGBOX_DEFAULT
	giveitem ITEM_TM_THIEF
	goto_if_eq VAR_RESULT, 0, SlateportCity_OceanicMuseum_1F_EventScript_NoRoomForThief
	setflag FLAG_RECEIVED_TM_THIEF
	msgbox SlateportCity_OceanicMuseum_1F_Text_HopeINeverSeeYouAgain, MSGBOX_DEFAULT
	closemessage
	goto_if_eq VAR_FACING, DIR_NORTH, SlateportCity_OceanicMuseum_1F_EventScript_FamiliarGruntExitNorth
	goto_if_eq VAR_FACING, DIR_SOUTH, SlateportCity_OceanicMuseum_1F_EventScript_FamiliarGruntExitSouth
	goto_if_eq VAR_FACING, DIR_WEST, SlateportCity_OceanicMuseum_1F_EventScript_FamiliarGruntExitWestEast
	goto_if_eq VAR_FACING, DIR_EAST, SlateportCity_OceanicMuseum_1F_EventScript_FamiliarGruntExitWestEast
	end

SlateportCity_OceanicMuseum_1F_EventScript_FamiliarGruntExitNorth::
	applymovement LOCALID_PLAYER, SlateportCity_OceanicMuseum_1F_Movement_PlayerWatchGruntExitNorth
	applymovement LOCALID_OCEANIC_MUSEUM_FAMILIAR_GRUNT, SlateportCity_OceanicMuseum_1F_Movement_FamiliarGruntExitNorth
	waitmovement 0
	goto SlateportCity_OceanicMuseum_1F_EventScript_FamiliarGruntExited
	end

SlateportCity_OceanicMuseum_1F_EventScript_FamiliarGruntExitSouth::
	applymovement LOCALID_OCEANIC_MUSEUM_FAMILIAR_GRUNT, SlateportCity_OceanicMuseum_1F_Movement_FamiliarGruntExit
	waitmovement 0
	goto SlateportCity_OceanicMuseum_1F_EventScript_FamiliarGruntExited
	end

SlateportCity_OceanicMuseum_1F_EventScript_FamiliarGruntExitWestEast::
	applymovement LOCALID_PLAYER, SlateportCity_OceanicMuseum_1F_Movement_PlayerWatchGruntExitWestEast
	applymovement LOCALID_OCEANIC_MUSEUM_FAMILIAR_GRUNT, SlateportCity_OceanicMuseum_1F_Movement_FamiliarGruntExit
	waitmovement 0
	goto SlateportCity_OceanicMuseum_1F_EventScript_FamiliarGruntExited
	end

SlateportCity_OceanicMuseum_1F_EventScript_FamiliarGruntExited::
	setflag FLAG_HIDE_SLATEPORT_CITY_OCEANIC_MUSEUM_FAMILIAR_AQUA_GRUNT
	playse SE_EXIT
	removeobject LOCALID_OCEANIC_MUSEUM_FAMILIAR_GRUNT
	release
	end

SlateportCity_OceanicMuseum_1F_EventScript_NoRoomForThief::
	msgbox SlateportCity_OceanicMuseum_1F_Text_YouHaveToTakeThis, MSGBOX_DEFAULT
	release
	end

SlateportCity_OceanicMuseum_1F_Movement_PlayerWatchGruntExitNorth:
	delay_16
	delay_8
	delay_4
	walk_in_place_faster_down
	step_end

SlateportCity_OceanicMuseum_1F_Movement_PlayerWatchGruntExitWestEast:
	delay_16
	walk_in_place_faster_down
	step_end

SlateportCity_OceanicMuseum_1F_Movement_FamiliarGruntExit:
	face_down
	walk_fast_down
	walk_fast_down
	walk_fast_down
	walk_fast_down
	delay_8
	step_end

SlateportCity_OceanicMuseum_1F_Movement_FamiliarGruntExitNorth:
	walk_fast_right
	walk_fast_down
	walk_fast_down
	walk_fast_down
	walk_fast_down
	delay_8
	step_end

SlateportCity_OceanicMuseum_1F_Text_WouldYouLikeToEnter:
	.string "Bienvenue au Musée Océanographique.\p"
	.string "L'entrée coûte 50 ¥. Vous souhaitez\n"
	.string "entrer ?$"

SlateportCity_OceanicMuseum_1F_Text_PleaseEnjoyYourself:
	.string "Bonne visite.$"

SlateportCity_OceanicMuseum_1F_Text_NotEnoughMoney:
	.string "Oh ? Désolée, vous n'avez pas assez\n"
	.string "d'argent.$"

SlateportCity_OceanicMuseum_1F_Text_CatchUpWithYourGroup:
	.string "Oh ? Vous êtes avec le groupe en\n"
	.string "bandanas qui vient d'entrer ?\p"
	.string "Il ne manque plus que vous.\n"
	.string "Dépêchez-vous de les rejoindre !$"

SlateportCity_OceanicMuseum_1F_Text_AquaExistForGoodOfAll:
	.string "Nous, la Team Aqua, existons pour le\n"
	.string "bien de tous ! Même si ça doit faire\l"
	.string "quelques morts.$"

SlateportCity_OceanicMuseum_1F_Text_OurBossIsntHere:
	.string "On nous a dit de nous rassembler ici,\n"
	.string "alors on est là, mais…\p"
	.string "Le Boss, la pièce maîtresse, il est pas\n"
	.string "là. On fait quoi, on attend ?$"

SlateportCity_OceanicMuseum_1F_Text_WouldStuffHereMakeMeRich:
	.string "Si je pique les trucs exposés ici, je\n"
	.string "deviens riche ?$"

SlateportCity_OceanicMuseum_1F_Text_CanLearnForNefariousDeeds:
	.string "Tout ce que j'apprends ici, je pourrai le\n"
	.string "mettre au service du crime…$"

SlateportCity_OceanicMuseum_1F_Text_RustboroBungled:
	.string "Si nos crétins avaient pas foiré à\n"
	.string "Mérouville, on serait pas là !$"

SlateportCity_OceanicMuseum_1F_Text_DidntHaveMoney:
	.string "J'avais pas 50 ¥, alors j'ai mis une\n"
	.string "plombe à passer l'accueil. J'ai dû\l"
	.string "pleurer.$"

SlateportCity_OceanicMuseum_1F_Text_LearnAboutSeaForBattling:
	.string "Je veux apprendre la mer pour mieux me\n"
	.string "battre.$"

SlateportCity_OceanicMuseum_1F_Text_SternIsRoleModel:
	.string "Quand je vois la mer, je fonds !\p"
	.string "Pour moi, le Capitaine Stern, c'est le\n"
	.string "modèle absolu !$"

SlateportCity_OceanicMuseum_1F_Text_MustBePokemonWeDontKnow:
	.string "La mer est sans fin et infiniment\n"
	.string "profonde…\p"
	.string "Il doit y avoir plein de Pokémon qu'on ne\n"
	.string "connaît pas. Et qui nous regardent d'en\l"
	.string "bas.$"

SlateportCity_OceanicMuseum_1F_Text_WantSeaPokemon:
	.string "Je veux un Pokémon de la mer.\p"
	.string "Ça doit être tout frais et tout doux à\n"
	.string "câliner.$"

SlateportCity_OceanicMuseum_1F_Text_RememberMeTakeThis:
	.string "Aïeeeuh ! Qu'est-ce que tu fiches ici ?\p"
	.string "Moi ? Le type de la Team Aqua que t'as\n"
	.string "démonté au Tunnel Mérazon, tu te\l"
	.string "souviens ?\p"
	.string "Tiens, prends ça ! Pardonne-moi, par\n"
	.string "pitié ! Le Boss m'a déjà assez puni !$"

SlateportCity_OceanicMuseum_1F_Text_HopeINeverSeeYouAgain:
	.string "Cette CT te va mieux qu'à moi.\p"
	.string "J'espère ne jamais te revoir ! Wahahaha\n"
	.string "! …Sérieusement.$"

SlateportCity_OceanicMuseum_1F_Text_YouHaveToTakeThis:
	.string "Allez ! Prends-la et laisse-moi partir !$"

SlateportCity_OceanicMuseum_1F_Text_WhirlpoolExperiment:
	.string "Un liquide bleu tourbillonne dans un\n"
	.string "récipient en verre.\p"
	.string "“Expérience : créer un SIPHON artificiel\n"
	.string "avec du vent.”$"

SlateportCity_OceanicMuseum_1F_Text_WaterfallExperiment:
	.string "Une balle rouge monte et descend dans\n"
	.string "un récipient en verre.\p"
	.string "“Expérience : simuler une CASCADE grâce\n"
	.string "à la flottabilité.”$"

SlateportCity_OceanicMuseum_1F_Text_OceanSoilDisplay:
	.string "Un échantillon de sol des fonds marins.\p"
	.string "“Au fil des ans, les restes des êtres\n"
	.string "vivants se déposent au fond de la mer\l"
	.string "en couches.\p"
	.string "“En analysant ces couches, on lit le\n"
	.string "passé. Et ses cimetières.”$"

SlateportCity_OceanicMuseum_1F_Text_BeachSandDisplay:
	.string "Un échantillon de sable de plage.\p"
	.string "“Les pierres des montagnes sont\n"
	.string "emportées par les rivières, brisées,\l"
	.string "broyées.\p"
	.string "“Elles finissent en grains de sable sur\n"
	.string "nos plages.”$"

SlateportCity_OceanicMuseum_1F_Text_OceanicMinifact1:
	.string "“Infos Océan no 1 : pourquoi la mer\n"
	.string "est-elle bleue ?\p"
	.string "“La lumière est faite de nombreuses\n"
	.string "couleurs.\p"
	.string "“En traversant l'eau, la plupart\n"
	.string "perdent leur éclat.\p"
	.string "“Le bleu, lui, résiste. Voilà pourquoi la\n"
	.string "mer paraît bleue.”$"

SlateportCity_OceanicMuseum_1F_Text_OceanicMinifact2:
	.string "“Infos Océan no 2 : pourquoi la mer\n"
	.string "est-elle salée ?\p"
	.string "“L'eau de mer contient du sel dissous,\n"
	.string "sous forme d'ions sodium et chlore.\p"
	.string "“La pluie les arrache aux roches et les\n"
	.string "emporte jusqu'à la mer.\p"
	.string "“C'est ce sel dissous qui rend la mer\n"
	.string "salée.”$"

SlateportCity_OceanicMuseum_1F_Text_OceanicMinifact3:
	.string "“Infos Océan no 3 : qui est le plus\n"
	.string "grand, la mer ou la terre ?\p"
	.string "“La mer couvre environ 70 % de la\n"
	.string "planète.\p"
	.string "“Elle est donc plus de deux fois plus\n"
	.string "grande que les terres.”$"

SlateportCity_OceanicMuseum_1F_Text_FossilDisplay:
	.string "Un fossile couvert de petites crêtes\n"
	.string "ondulées.\p"
	.string "“Le sol des fonds marins est raclé par\n"
	.string "la marée.\p"
	.string "“L'eau y dessine de petites crêtes et\n"
	.string "vallées.\p"
	.string "“Fossilisé, ce motif s'appelle une ride\n"
	.string "de courant.”$"

SlateportCity_OceanicMuseum_1F_Text_DepthMeasuringMachine:
	.string "Une étrange machine tourne sous un\n"
	.string "dôme de verre.\p"
	.string "Elle mesure peut-être la profondeur de\n"
	.string "quelque chose…$"

