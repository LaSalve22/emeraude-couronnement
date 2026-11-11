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
	.string "Merci d'avoir passé un moment avec\n"
	.string "Timmy, tout à l'heure.\p"
	.string "Il est malade depuis sa naissance. Les\n"
	.string "médecins ne nous ont jamais promis\l"
	.string "grand-chose.\p"
	.string "On l'envoie chez des parents à Vergazon\n"
	.string "pour quelque temps.\p"
	.string "L'air y est bien plus pur qu'ici. C'est\n"
	.string "notre dernier espoir.\p"
	.string "Comment ? Où est Timmy ? Il est déjà\n"
	.string "parti.\p"
	.string "Je me demande où il est, maintenant…$"

PetalburgCity_WallysHouse_Text_WonderHowWallyIsDoing:
	.string "Je me demande comment va notre Timmy…\n"
	.string "Chaque fois que le téléphone sonne, j'ai\l"
	.string "peur.$"

PetalburgCity_WallysHouse_Text_PleaseExcuseUs:
	.string "{PLAYER} ! Pardon de t'avoir fait venir\n"
	.string "comme ça.\p"
	.string "Mais notre Timmy va beaucoup mieux\n"
	.string "depuis qu'il est à Vergazon.\p"
	.string "Et c'est grâce à toi !\p"
	.string "Avant de partir, tu l'as aidé à capturer\n"
	.string "un Pokémon, n'est-ce pas ?\p"
	.string "Ça l'a rendu tellement heureux.\p"
	.string "Et pas seulement lui. Moi, son père,\n"
	.string "aussi.\p"
	.string "On a passé des années à attendre le\n"
	.string "pire. Et toi, tu lui as donné une raison\l"
	.string "de se battre.\p"
	.string "Ce n'est pas un pot-de-vin, mais\n"
	.string "j'aimerais vraiment que tu acceptes\l"
	.string "ceci.$"

PetalburgCity_WallysHouse_Text_SurfGoAllSortsOfPlaces:
	.string "Si ton Pokémon sait utiliser Surf, tu\n"
	.string "pourras aller presque partout.$"

PetalburgCity_WallysHouse_Text_WallyIsComingHomeSoon:
	.string "Timmy rentre bientôt. J'ai hâte. On a\n"
	.string "failli ne jamais le revoir.$"

PetalburgCity_WallysHouse_Text_YouMetWallyInEverGrandeCity:
	.string "Oh ? Tu as vu Timmy à Éternara ?\p"
	.string "Voyons, {PLAYER}, ne dis pas de bêtises.\p"
	.string "Il va mieux, mais il ne pourrait jamais\n"
	.string "aller si loin tout seul. Il… il ne ferait\l"
	.string "pas ça.$"

PetalburgCity_WallysHouse_Text_WallyWasReallyHappy:
	.string "Timmy était tellement heureux quand il\n"
	.string "nous a dit qu'il avait capturé un\l"
	.string "Pokémon.\p"
	.string "Ça faisait des années que je ne l'avais\n"
	.string "pas vu sourire comme ça.$"

PetalburgCity_WallysHouse_Text_WallyLeftWithoutTelling:
	.string "Ne dis rien à mon mari…\p"
	.string "Notre Timmy a quitté Vergazon sans\n"
	.string "prévenir personne.\p"
	.string "Il est fragile, mais il a une volonté de\n"
	.string "fer. Plus que nous tous.\p"
	.string "Je suis sûre qu'il reviendra sain et\n"
	.string "sauf. Il le faut.$"

