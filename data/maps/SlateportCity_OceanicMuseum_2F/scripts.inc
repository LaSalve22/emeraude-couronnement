SlateportCity_OceanicMuseum_2F_MapScripts::
	.byte 0

SlateportCity_OceanicMuseum_2F_EventScript_CaptStern::
	lock
	faceplayer
	msgbox SlateportCity_OceanicMuseum_2F_Text_ThankYouForTheParts, MSGBOX_DEFAULT
	closemessage
	playbgm MUS_ENCOUNTER_AQUA, TRUE
	addobject LOCALID_OCEANIC_MUSEUM_2F_GRUNT_1
	applymovement LOCALID_OCEANIC_MUSEUM_2F_GRUNT_1, SlateportCity_OceanicMuseum_2F_Movement_FirstGruntEnter
	waitmovement 0
	addobject LOCALID_OCEANIC_MUSEUM_2F_GRUNT_2
	applymovement LOCALID_OCEANIC_MUSEUM_2F_GRUNT_2, SlateportCity_OceanicMuseum_2F_Movement_SecondGruntEnter
	waitmovement 0
	applymovement LOCALID_OCEANIC_MUSEUM_2F_GRUNT_1, SlateportCity_OceanicMuseum_2F_Movement_FirstGruntApproach
	applymovement LOCALID_OCEANIC_MUSEUM_2F_GRUNT_2, SlateportCity_OceanicMuseum_2F_Movement_SecondGruntApproach
	waitmovement 0
	call_if_eq VAR_FACING, DIR_SOUTH, SlateportCity_OceanicMuseum_2F_EventScript_PlayerFaceGrunts
	call_if_eq VAR_FACING, DIR_EAST, SlateportCity_OceanicMuseum_2F_EventScript_PlayerFaceGrunts
	msgbox SlateportCity_OceanicMuseum_2F_Text_WellTakeThoseParts, MSGBOX_DEFAULT
	call_if_ne VAR_FACING, DIR_EAST, SlateportCity_OceanicMuseum_2F_EventScript_SternFaceGrunts
	msgbox SlateportCity_OceanicMuseum_2F_Text_SternWhoAreYou, MSGBOX_DEFAULT
	msgbox SlateportCity_OceanicMuseum_2F_Text_WereTeamAqua, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_OCEANIC_MUSEUM_2F_GRUNT_2, SlateportCity_OceanicMuseum_2F_Movement_GruntApproachToBattle
	waitmovement 0
	call_if_eq VAR_FACING, DIR_SOUTH, SlateportCity_OceanicMuseum_2F_EventScript_PlayerApproachGruntSouth
	call_if_eq VAR_FACING, DIR_WEST, SlateportCity_OceanicMuseum_2F_EventScript_PlayerApproachGruntWest
	trainerbattle_no_intro TRAINER_GRUNT_MUSEUM_1, SlateportCity_OceanicMuseum_2F_Text_Grunt1Defeat
	msgbox SlateportCity_OceanicMuseum_2F_Text_BossGoingToBeFurious, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_OCEANIC_MUSEUM_2F_GRUNT_2, SlateportCity_OceanicMuseum_2F_Movement_GruntDefeated
	waitmovement 0
	applymovement LOCALID_OCEANIC_MUSEUM_2F_GRUNT_1, SlateportCity_OceanicMuseum_2F_Movement_GruntApproachToBattle
	waitmovement 0
	msgbox SlateportCity_OceanicMuseum_2F_Text_LetMeTakeCareOfThis, MSGBOX_DEFAULT
	trainerbattle_no_intro TRAINER_GRUNT_MUSEUM_2, SlateportCity_OceanicMuseum_2F_Text_Grunt2Defeat
	applymovement LOCALID_OCEANIC_MUSEUM_2F_GRUNT_1, SlateportCity_OceanicMuseum_2F_Movement_GruntDefeated
	waitmovement 0
	applymovement LOCALID_OCEANIC_MUSEUM_2F_GRUNT_1, Common_Movement_WalkInPlaceFasterDown
	applymovement LOCALID_OCEANIC_MUSEUM_2F_GRUNT_2, Common_Movement_WalkInPlaceFasterUp
	waitmovement 0
	msgbox SlateportCity_OceanicMuseum_2F_Text_MeddlingKid, MSGBOX_DEFAULT
	closemessage
	delay 35
	addobject LOCALID_OCEANIC_MUSEUM_2F_ARCHIE
	applymovement LOCALID_OCEANIC_MUSEUM_2F_ARCHIE, SlateportCity_OceanicMuseum_2F_Movement_ArchieEnter
	applymovement LOCALID_OCEANIC_MUSEUM_2F_GRUNT_2, SlateportCity_OceanicMuseum_2F_Movement_GruntMoveForArchie
	waitmovement 0
	msgbox SlateportCity_OceanicMuseum_2F_Text_CameToSeeWhatsTakingSoLong, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_OCEANIC_MUSEUM_2F_ARCHIE, SlateportCity_OceanicMuseum_2F_Movement_ArchieApproachPlayer
	waitmovement 0
	msgbox SlateportCity_OceanicMuseum_2F_Text_ArchieWarning, MSGBOX_DEFAULT
	closemessage
	savebgm MUS_DUMMY
	fadedefaultbgm
	fadescreen FADE_TO_BLACK
	removeobject LOCALID_OCEANIC_MUSEUM_2F_ARCHIE
	removeobject LOCALID_OCEANIC_MUSEUM_2F_GRUNT_1
	removeobject LOCALID_OCEANIC_MUSEUM_2F_GRUNT_2
	fadescreen FADE_FROM_BLACK
	delay 30
	setflag FLAG_HIDE_SLATEPORT_CITY_OCEANIC_MUSEUM_AQUA_GRUNTS
	applymovement LOCALID_PLAYER, Common_Movement_WalkInPlaceFasterRight
	waitmovement 0
	msgbox SlateportCity_OceanicMuseum_2F_Text_SternThankYouForSavingUs, MSGBOX_DEFAULT
	setvar VAR_0x8004, ITEM_DEVON_PARTS
	call Common_EventScript_PlayerHandedOverTheItem
	msgbox SlateportCity_OceanicMuseum_2F_Text_SternIveGotToGo, MSGBOX_DEFAULT
	closemessage
	fadescreen FADE_TO_BLACK
	playfanfare MUS_HEAL
	waitfanfare
	special HealPlayerParty
	callnative UpdateFollowingPokemon
	removeobject LOCALID_OCEANIC_MUSEUM_2F_CAPT_STERN
	setflag FLAG_HIDE_ROUTE_110_TEAM_AQUA
	call_if_eq VAR_REGISTER_BIRCH_STATE, 0, SlateportCity_OceanicMuseum_2F_EventScript_ReadyRegisterBirch
	setflag FLAG_DELIVERED_DEVON_GOODS
	clearflag FLAG_HIDE_ROUTE_116_DEVON_EMPLOYEE
	setflag FLAG_HIDE_RUSTBORO_CITY_DEVON_CORP_3F_EMPLOYEE
	setvar VAR_SLATEPORT_OUTSIDE_MUSEUM_STATE, 1
	fadescreen FADE_FROM_BLACK
	release
	end

SlateportCity_OceanicMuseum_2F_EventScript_ReadyRegisterBirch::
	setvar VAR_REGISTER_BIRCH_STATE, 1
	return

SlateportCity_OceanicMuseum_2F_EventScript_PlayerFaceGrunts::
	applymovement LOCALID_PLAYER, Common_Movement_WalkInPlaceFasterLeft
	waitmovement 0
	return

SlateportCity_OceanicMuseum_2F_EventScript_SternFaceGrunts::
	applymovement LOCALID_OCEANIC_MUSEUM_2F_CAPT_STERN, Common_Movement_WalkInPlaceFasterLeft
	waitmovement 0
	return

SlateportCity_OceanicMuseum_2F_EventScript_PlayerApproachGruntSouth::
	applymovement LOCALID_PLAYER, SlateportCity_OceanicMuseum_2F_Movement_PlayerApproachGruntSouth
	waitmovement 0
	return

SlateportCity_OceanicMuseum_2F_EventScript_PlayerApproachGruntWest::
	applymovement LOCALID_PLAYER, SlateportCity_OceanicMuseum_2F_Movement_PlayerApproachGruntWest
	waitmovement 0
	return

SlateportCity_OceanicMuseum_2F_Movement_PlayerApproachGruntSouth:
	walk_left
	walk_down
	walk_in_place_faster_left
	step_end

SlateportCity_OceanicMuseum_2F_Movement_PlayerApproachGruntWest:
	walk_up
	walk_left
	walk_left
	walk_down
	walk_in_place_faster_left
	step_end

SlateportCity_OceanicMuseum_2F_Movement_Unused:
	walk_up
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_up
	walk_up
	walk_left
	walk_left
	walk_up
	walk_up
	delay_8
	step_end

SlateportCity_OceanicMuseum_2F_Movement_ArchieApproachPlayer:
	walk_right
	step_end

SlateportCity_OceanicMuseum_2F_Movement_ArchieEnter:
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	walk_right
	walk_right
	walk_right
	walk_right
	step_end

SlateportCity_OceanicMuseum_2F_Movement_GruntApproachToBattle:
	walk_right
	step_end

SlateportCity_OceanicMuseum_2F_Movement_FirstGruntEnter:
	walk_down
	walk_right
	walk_in_place_faster_down
	step_end

SlateportCity_OceanicMuseum_2F_Movement_FirstGruntApproach:
	walk_down
	walk_down
	walk_down
	walk_right
	walk_right
	walk_right
	step_end

SlateportCity_OceanicMuseum_2F_Movement_GruntDefeated:
	lock_facing_direction
	walk_left
	unlock_facing_direction
	step_end

SlateportCity_OceanicMuseum_2F_Movement_SecondGruntEnter:
	walk_down
	step_end

SlateportCity_OceanicMuseum_2F_Movement_SecondGruntApproach:
	walk_down
	walk_down
	walk_down
	walk_down
	walk_right
	walk_right
	walk_right
	walk_right
	step_end

SlateportCity_OceanicMuseum_2F_Movement_GruntMoveForArchie:
	delay_16
	delay_16
	delay_16
	delay_16
	delay_16
	delay_16
	delay_16
	delay_16
	delay_8
	walk_fast_down
	walk_in_place_faster_up
	step_end

SlateportCity_OceanicMuseum_2F_EventScript_WaterQualitySample1::
	msgbox SlateportCity_OceanicMuseum_2F_Text_WaterQualitySample1, MSGBOX_SIGN
	end

SlateportCity_OceanicMuseum_2F_EventScript_WaterQualitySample2::
	msgbox SlateportCity_OceanicMuseum_2F_Text_WaterQualitySample2, MSGBOX_SIGN
	end

SlateportCity_OceanicMuseum_2F_EventScript_PressureExperiment::
	msgbox SlateportCity_OceanicMuseum_2F_Text_PressureExperiment, MSGBOX_SIGN
	end

SlateportCity_OceanicMuseum_2F_EventScript_HoennModel::
	msgbox SlateportCity_OceanicMuseum_2F_Text_HoennModel, MSGBOX_SIGN
	end

SlateportCity_OceanicMuseum_2F_EventScript_DeepSeawaterDisplay::
	msgbox SlateportCity_OceanicMuseum_2F_Text_DeepSeawaterDisplay, MSGBOX_SIGN
	end

SlateportCity_OceanicMuseum_2F_EventScript_SurfaceSeawaterDisplay::
	msgbox SlateportCity_OceanicMuseum_2F_Text_SurfaceSeawaterDisplay, MSGBOX_SIGN
	end

SlateportCity_OceanicMuseum_2F_EventScript_SSTidalReplica::
	msgbox SlateportCity_OceanicMuseum_2F_Text_SSTidalReplica, MSGBOX_SIGN
	end

SlateportCity_OceanicMuseum_2F_EventScript_SubmarineReplica::
	msgbox SlateportCity_OceanicMuseum_2F_Text_SubmarineReplica, MSGBOX_SIGN
	end

SlateportCity_OceanicMuseum_2F_EventScript_SubmersibleReplica::
	msgbox SlateportCity_OceanicMuseum_2F_Text_SumbersibleReplica, MSGBOX_SIGN
	end

SlateportCity_OceanicMuseum_2F_EventScript_SSAnneReplica::
	msgbox SlateportCity_OceanicMuseum_2F_Text_SSAnneReplica, MSGBOX_SIGN
	end

SlateportCity_OceanicMuseum_2F_EventScript_MuseumPatron1::
	msgbox SlateportCity_OceanicMuseum_2F_Text_RemindsMeOfAbandonedShip, MSGBOX_NPC
	end

SlateportCity_OceanicMuseum_2F_EventScript_MuseumPatron2::
	msgbox SlateportCity_OceanicMuseum_2F_Text_DontRunInMuseum, MSGBOX_NPC
	end

SlateportCity_OceanicMuseum_2F_EventScript_MuseumPatron3::
	lock
	faceplayer
	msgbox SlateportCity_OceanicMuseum_2F_Text_WantToRideSubmarine, MSGBOX_DEFAULT
	closemessage
	applymovement VAR_LAST_TALKED, Common_Movement_FaceOriginalDirection
	waitmovement 0
	release
	end

SlateportCity_OceanicMuseum_2F_Text_ThankYouForTheParts:
	.string "Oui ? Si vous cherchez Stern, c'est moi.\p"
	.string "Ah ! Ce sont les pièces que j'ai\n"
	.string "commandées à M. Rochard, de Devon ?\p"
	.string "Merci ! Parfait ! On va pouvoir préparer\n"
	.string "l'expédition.$"

SlateportCity_OceanicMuseum_2F_Text_WellTakeThoseParts:
	.string "Hé hé hé, pas si vite ! Ces pièces, on les\n"
	.string "prend !$"

SlateportCity_OceanicMuseum_2F_Text_SternWhoAreYou:
	.string "Capitaine Stern : Qu… quoi ? Qui\n"
	.string "êtes-vous ?$"

SlateportCity_OceanicMuseum_2F_Text_WereTeamAqua:
	.string "On est la Team Aqua !\p"
	.string "Le Boss veut ces pièces ! Ferme ton\n"
	.string "clapet et file-les !$"

SlateportCity_OceanicMuseum_2F_Text_Grunt1Defeat:
	.string "Awaaah ! Battu par un gamin !$"

SlateportCity_OceanicMuseum_2F_Text_BossGoingToBeFurious:
	.string "Oh, la catastrophe… Le Boss va nous\n"
	.string "jeter aux requins…$"

SlateportCity_OceanicMuseum_2F_Text_LetMeTakeCareOfThis:
	.string "Pff, espèce de mauviette ! Laisse-moi\n"
	.string "faire !$"

SlateportCity_OceanicMuseum_2F_Text_Grunt2Defeat:
	.string "Quoi ?! Moi aussi ?!$"

SlateportCity_OceanicMuseum_2F_Text_MeddlingKid:
	.string "On fait quoi, maintenant ? Sans les\n"
	.string "pièces, on est morts !\p"
	.string "Argh, j'avais pas prévu qu'un gosse\n"
	.string "vienne fourrer son nez partout !$"

SlateportCity_OceanicMuseum_2F_Text_CameToSeeWhatsTakingSoLong:
	.string "Je viens voir pourquoi vous mettez\n"
	.string "trois heures à voler des pièces, et je\l"
	.string "vous trouve tenus en échec par… un\l"
	.string "enfant ?$"

SlateportCity_OceanicMuseum_2F_Text_ArchieWarning:
	.string "Nous sommes la Team Aqua, et nous\n"
	.string "aimons la mer !\p"
	.string "Et moi, je suis Arthur, le chef de la Team\n"
	.string "Aqua !\p"
	.string "Pourquoi tu te mets en travers de notre\n"
	.string "chemin ?\p"
	.string "…Non ! Ne me dis pas que tu fais partie\n"
	.string "de la Team Magma ?\p"
	.string "Non, impossible. Tu n'as pas la dégaine.\p"
	.string "… … … … … … … … … … … …\p"
	.string "Les Pokémon, les humains… Toute vie\n"
	.string "vient de la mer.\p"
	.string "La terre a tout pris. Les forêts, les\n"
	.string "villes, le béton. Elle étouffe la mer, et\l"
	.string "la vie avec.\p"
	.string "La Team Aqua va rendre au monde ce qui\n"
	.string "lui appartient. Plus de mer. Moins de\l"
	.string "terre.\p"
	.string "Tu ne trouves pas ça magnifique ?\p"
	.string "Bah… Tu es encore trop jeune pour\n"
	.string "comprendre notre noble cause.\p"
	.string "Mais je te préviens : ne te mêle plus\n"
	.string "jamais de nos plans.\p"
	.string "Sinon, la mer te reprendra. Et personne\n"
	.string "ne retrouvera ton corps.\p"
	.string "Ne l'oublie pas.$"

SlateportCity_OceanicMuseum_2F_Text_SternThankYouForSavingUs:
	.string "Capitaine Stern : Tu es… Ah, d'accord, tu\n"
	.string "es {PLAYER}…\p"
	.string "En tout cas, c'était tendu ! Merci de\n"
	.string "nous avoir sauvés !\p"
	.string "Ah oui, et tu as même apporté les pièces\n"
	.string "de Devon !$"

SlateportCity_OceanicMuseum_2F_Text_SternIveGotToGo:
	.string "Capitaine Stern : Oups ! Pas de temps à\n"
	.string "perdre !\p"
	.string "Notre expédition sous-marine part très\n"
	.string "bientôt.\p"
	.string "Merci encore, mais je dois filer !\p"
	.string "Fais donc le tour du Musée, si tu veux.$"

SlateportCity_OceanicMuseum_2F_Text_RemindsMeOfAbandonedShip:
	.string "J'ai vu une maquette de bateau ici.\p"
	.string "Ça m'a rappelé le Bateau Abandonné,\n"
	.string "près du Village Myokara… Il paraît qu'on\l"
	.string "y entend des pleurs, la nuit.$"

SlateportCity_OceanicMuseum_2F_Text_DontRunInMuseum:
	.string "Ne cours pas dans le Musée !$"

SlateportCity_OceanicMuseum_2F_Text_WantToRideSubmarine:
	.string "Waouh, le sous-marin est trop beau ! Je\n"
	.string "veux monter dedans !$"

SlateportCity_OceanicMuseum_2F_Text_WaterQualitySample1:
	.string "“Échantillon d'eau no 1”, indique\n"
	.string "l'étiquette.\p"
	.string "Toutes les mers sont reliées, mais l'eau\n"
	.string "semble différente d'une région à\l"
	.string "l'autre.$"

SlateportCity_OceanicMuseum_2F_Text_WaterQualitySample2:
	.string "“Échantillon d'eau no 2”, indique\n"
	.string "l'étiquette.\p"
	.string "La mer est-elle plus ou moins salée\n"
	.string "selon la région ?$"

SlateportCity_OceanicMuseum_2F_Text_PressureExperiment:
	.string "Une balle en caoutchouc gonfle et\n"
	.string "rétrécit.\p"
	.string "“Dans la mer, le poids de l'eau exerce\n"
	.string "une pression.\p"
	.string "“Près de la surface, elle est faible.\p"
	.string "“Mais dans les abysses, elle peut\n"
	.string "atteindre des dizaines de milliers de\l"
	.string "tonnes sur une petite surface. De quoi\l"
	.string "écraser un sous-marin comme une\l"
	.string "canette.”$"

SlateportCity_OceanicMuseum_2F_Text_HoennModel:
	.string "“Maquette de la région de Hoenn”\p"
	.string "Une maquette miniature de Hoenn.\p"
	.string "Où est Bourg-en-Vol, là-dessus ? Ah.\n"
	.string "C'est le point minuscule.$"

SlateportCity_OceanicMuseum_2F_Text_DeepSeawaterDisplay:
	.string "Un panneau sur les courants marins.\p"
	.string "“Au fond de la mer, l'eau circule à cause\n"
	.string "des différences de température et de\l"
	.string "salinité.”$"

SlateportCity_OceanicMuseum_2F_Text_SurfaceSeawaterDisplay:
	.string "Un panneau sur les courants marins.\p"
	.string "“En surface, l'eau circule en courants\n"
	.string "poussés par les vents.”$"

SlateportCity_OceanicMuseum_2F_Text_SSTidalReplica:
	.string "“Le ferry M.S. Marée\p"
	.string "“Maquette du navire en construction au\n"
	.string "chantier naval de Stern.”$"

SlateportCity_OceanicMuseum_2F_Text_SubmarineReplica:
	.string "“Explorateur Sous-Marin 1\p"
	.string "“Maquette du sous-marin d'exploration\n"
	.string "des fonds marins.”$"

SlateportCity_OceanicMuseum_2F_Text_SumbersibleReplica:
	.string "“Capsule submersible\p"
	.string "“Maquette d'une petite capsule sans\n"
	.string "équipage pour explorer les fonds.”$"

SlateportCity_OceanicMuseum_2F_Text_SSAnneReplica:
	.string "“M.S. Anne\p"
	.string "“Maquette du paquebot de luxe qui fait\n"
	.string "le tour du monde.”$"
