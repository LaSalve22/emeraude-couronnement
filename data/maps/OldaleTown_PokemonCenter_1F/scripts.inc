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
	.string "Toi aussi, bien sûr.$"

OldaleTown_PokemonCenter_1F_Text_PokemonCentersAreGreat:
	.string "Les Centres Pokémon, c'est génial !\p"
	.string "C'est gratuit, autant de fois qu'on\n"
	.string "veut. Personne ne demande d'où\l"
	.string "viennent les blessures.$"

OldaleTown_PokemonCenter_1F_Text_WirelessClubNotAvailable:
	.string "Le Club Sans-Fil du premier étage vient\n"
	.string "d'ouvrir.\p"
	.string "Mais il paraît qu'ils font encore des\n"
	.string "réglages.$"

OldaleTown_PokemonCenter_1F_Text_TradedInWirelessClub:
	.string "Le Club Sans-Fil du premier étage vient\n"
	.string "d'ouvrir.\p"
	.string "J'ai tout de suite échangé des Pokémon.$"

