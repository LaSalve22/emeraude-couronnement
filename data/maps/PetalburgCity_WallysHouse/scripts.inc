PetalburgCity_WallysHouse_MapScripts::
	map_script MAP_SCRIPT_ON_FRAME_TABLE, PetalburgCity_WallysHouse_OnFrame
	map_script MAP_SCRIPT_ON_WARP_INTO_MAP_TABLE, PetalburgCity_WallysHouse_OnWarp
	.byte 0

PetalburgCity_WallysHouse_OnWarp:
	map_script_2 VAR_PETALBURG_CITY_STATE, 4, PetalburgCity_WallysHouse_EventScript_PlayerWallysDadFaceEachOther
	.2byte 0

PetalburgCity_WallysHouse_EventScript_PlayerWallysDadFaceEachOther::
	turnobject LOCALID_PLAYER, DIR_EAST
	turnobject LOCALID_WALLYS_HOUSE_WALLYS_DAD, DIR_WEST
	end

PetalburgCity_WallysHouse_OnFrame:
	map_script_2 VAR_PETALBURG_CITY_STATE, 4, PetalburgCity_WallysHouse_EventScript_GiveHMSurf
	.2byte 0

PetalburgCity_WallysHouse_EventScript_GiveHMSurf::
	lockall
	msgbox PetalburgCity_WallysHouse_Text_PleaseExcuseUs, MSGBOX_DEFAULT
	giveitem ITEM_HM_SURF
	setflag FLAG_RECEIVED_HM_SURF
	msgbox PetalburgCity_WallysHouse_Text_SurfGoAllSortsOfPlaces, MSGBOX_DEFAULT
	setvar VAR_PETALBURG_CITY_STATE, 5
	releaseall
	end

PetalburgCity_WallysHouse_EventScript_WallysDad::
	lock
	faceplayer
	goto_if_set FLAG_DEFEATED_WALLY_VICTORY_ROAD, PetalburgCity_WallysHouse_EventScript_DefeatedWallyInVictoryRoad
	goto_if_set FLAG_RECEIVED_HM_SURF, PetalburgCity_WallysHouse_EventScript_ReceievedHMSurf
	goto_if_set FLAG_THANKED_FOR_PLAYING_WITH_WALLY, PetalburgCity_WallysHouse_EventScript_PlayedWithWally
	msgbox PetalburgCity_WallysHouse_Text_ThanksForPlayingWithWally, MSGBOX_DEFAULT
	setflag FLAG_THANKED_FOR_PLAYING_WITH_WALLY
	release
	end

PetalburgCity_WallysHouse_EventScript_ReceievedHMSurf::
	msgbox PetalburgCity_WallysHouse_Text_WallyIsComingHomeSoon, MSGBOX_DEFAULT
	release
	end

PetalburgCity_WallysHouse_EventScript_DefeatedWallyInVictoryRoad::
	msgbox PetalburgCity_WallysHouse_Text_YouMetWallyInEverGrandeCity, MSGBOX_DEFAULT
	release
	end

PetalburgCity_WallysHouse_EventScript_PlayedWithWally::
	msgbox PetalburgCity_WallysHouse_Text_WonderHowWallyIsDoing, MSGBOX_DEFAULT
	release
	end

PetalburgCity_WallysHouse_EventScript_WallysMom::
	lock
	faceplayer
	goto_if_set FLAG_RECEIVED_HM_SURF, PetalburgCity_WallysHouse_EventScript_ReceivedHMSurf
	msgbox PetalburgCity_WallysHouse_Text_WallyWasReallyHappy, MSGBOX_DEFAULT
	release
	end

PetalburgCity_WallysHouse_EventScript_ReceivedHMSurf::
	msgbox PetalburgCity_WallysHouse_Text_WallyLeftWithoutTelling, MSGBOX_DEFAULT
	release
	end

PetalburgCity_WallysHouse_Text_ThanksForPlayingWithWally:
	.string "Tu es… Ah, tu dois être {PLAYER}, c'est\n"
	.string "ça ?\p"
	.string "Merci d'avoir tenu compagnie à Timmy,\n"
	.string "tout à l'heure.\p"
	.string "Il est malade depuis sa naissance. Les\n"
	.string "médecins ont arrêté de nous faire des\l"
	.string "promesses quand il avait quatre ans.\p"
	.string "On l'envoie chez des cousins à\n"
	.string "Vergazon.\p"
	.string "L'air y est plus pur. C'est… c'est ce\n"
	.string "qu'il nous reste, comme espoir.\p"
	.string "Comment ? Timmy ? Il est déjà parti.\p"
	.string "Il a souri en partant. Ça faisait\n"
	.string "longtemps.$"

PetalburgCity_WallysHouse_Text_WonderHowWallyIsDoing:
	.string "Je me demande comment va Timmy… Chaque\n"
	.string "fois que le téléphone sonne, j'arrête de\l"
	.string "respirer.$"

PetalburgCity_WallysHouse_Text_PleaseExcuseUs:
	.string "{PLAYER} ! Pardon de t'avoir fait venir\n"
	.string "comme ça.\p"
	.string "Mais notre Timmy va beaucoup mieux\n"
	.string "depuis qu'il est à Vergazon !\p"
	.string "Et c'est grâce à toi !\p"
	.string "Tu l'as aidé à capturer son Pokémon\n"
	.string "avant qu'il parte, n'est-ce pas ?\p"
	.string "Depuis, il se lève le matin. Il mange. Il\n"
	.string "fait des projets.\p"
	.string "On a passé des années à attendre le\n"
	.string "pire. Toi, tu lui as donné une raison\l"
	.string "d'attendre le meilleur.\p"
	.string "C'est pas un pot-de-vin, hein. Mais\n"
	.string "prends ça. J'insiste. Je pleure, là, mais\l"
	.string "j'insiste.$"

PetalburgCity_WallysHouse_Text_SurfGoAllSortsOfPlaces:
	.string "Si ton Pokémon sait utiliser Surf, tu\n"
	.string "pourras aller presque partout.$"

PetalburgCity_WallysHouse_Text_WallyIsComingHomeSoon:
	.string "Timmy rentre bientôt. J'ai déjà acheté\n"
	.string "trois gâteaux. On va tous être malades,\l"
	.string "et ce sera merveilleux.$"

PetalburgCity_WallysHouse_Text_YouMetWallyInEverGrandeCity:
	.string "Hein ? Tu as vu Timmy à Éternara ?\p"
	.string "Voyons, {PLAYER}. Ne dis pas de bêtises.\p"
	.string "Il va mieux, mais il ne pourrait jamais\n"
	.string "aller si loin tout seul. Il… il ne ferait\l"
	.string "pas ça. Pas sans nous le dire.$"

PetalburgCity_WallysHouse_Text_WallyWasReallyHappy:
	.string "Timmy était tellement heureux quand il\n"
	.string "nous a dit qu'il avait capturé un\l"
	.string "Pokémon.\p"
	.string "Ça faisait des années que je ne l'avais\n"
	.string "pas vu sourire comme ça. J'avais oublié\l"
	.string "la forme de son sourire.$"

PetalburgCity_WallysHouse_Text_WallyLeftWithoutTelling:
	.string "Ne dis rien à mon mari…\p"
	.string "Timmy a quitté Vergazon sans prévenir\n"
	.string "personne.\p"
	.string "Il est fragile, mais il a une volonté de\n"
	.string "fer. Plus que nous tous réunis.\p"
	.string "Il reviendra. Il a promis à son Pokémon\n"
	.string "de lui montrer le monde. Timmy tient\l"
	.string "toujours ses promesses.$"

