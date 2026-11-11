OldaleTown_PokemonCenter_1F_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, OldaleTown_PokemonCenter_1F_OnTransition
	map_script MAP_SCRIPT_ON_RESUME, CableClub_OnResume
	.byte 0

OldaleTown_PokemonCenter_1F_OnTransition:
	setrespawn HEAL_LOCATION_OLDALE_TOWN
	call Common_EventScript_UpdateBrineyLocation
	end

OldaleTown_PokemonCenter_1F_EventScript_Nurse::
	setvar VAR_0x800B, LOCALID_OLDALE_NURSE
	call Common_EventScript_PkmnCenterNurse
	waitmessage
	waitbuttonpress
	release
	end

OldaleTown_PokemonCenter_1F_EventScript_Gentleman::
	msgbox OldaleTown_PokemonCenter_1F_Text_TrainersCanUsePC, MSGBOX_NPC
	end

OldaleTown_PokemonCenter_1F_EventScript_Boy::
	msgbox OldaleTown_PokemonCenter_1F_Text_PokemonCentersAreGreat, MSGBOX_NPC
	end

OldaleTown_PokemonCenter_1F_EventScript_Girl::
	lock
	faceplayer
	goto_if_set FLAG_SYS_POKEDEX_GET, OldaleTown_PokemonCenter_1F_EventScript_WirelessClubAvailable
	msgbox OldaleTown_PokemonCenter_1F_Text_WirelessClubNotAvailable, MSGBOX_DEFAULT
	release
	end

OldaleTown_PokemonCenter_1F_EventScript_WirelessClubAvailable::
	msgbox OldaleTown_PokemonCenter_1F_Text_TradedInWirelessClub, MSGBOX_DEFAULT
	release
	end

OldaleTown_PokemonCenter_1F_Text_TrainersCanUsePC:
	.string "Le PC dans le coin est à la disposition\n"
	.string "de tous les Dresseurs.\p"
	.string "Toi aussi. Même si t'as la tête de\n"
	.string "quelqu'un qui débute.$"

OldaleTown_PokemonCenter_1F_Text_PokemonCentersAreGreat:
	.string "Les Centres Pokémon, c'est génial !\p"
	.string "C'est gratuit, illimité, et personne ne\n"
	.string "demande d'où viennent les blessures.\l"
	.string "Personne. Jamais.$"

OldaleTown_PokemonCenter_1F_Text_WirelessClubNotAvailable:
	.string "Le Club Sans-Fil du premier étage vient\n"
	.string "d'ouvrir.\p"
	.string "Mais il paraît qu'ils font encore des\n"
	.string "réglages. Le technicien pleure dans les\l"
	.string "toilettes depuis mardi.$"

OldaleTown_PokemonCenter_1F_Text_TradedInWirelessClub:
	.string "Le Club Sans-Fil du premier étage vient\n"
	.string "d'ouvrir.\p"
	.string "J'ai tout de suite échangé des Pokémon\n"
	.string "! Mon pote m'a filé un Magicarpe contre\l"
	.string "mon meilleur Pokémon. Je le hais.$"

