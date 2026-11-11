RustboroCity_PokemonCenter_1F_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, RustboroCity_PokemonCenter_1F_OnTransition
	map_script MAP_SCRIPT_ON_RESUME, CableClub_OnResume
	.byte 0

RustboroCity_PokemonCenter_1F_OnTransition:
	setrespawn HEAL_LOCATION_RUSTBORO_CITY
	call Common_EventScript_UpdateBrineyLocation
	end

RustboroCity_PokemonCenter_1F_EventScript_Nurse::
	setvar VAR_0x800B, LOCALID_RUSTBORO_NURSE
	call Common_EventScript_PkmnCenterNurse
	waitmessage
	waitbuttonpress
	release
	end

RustboroCity_PokemonCenter_1F_EventScript_Man::
	msgbox RustboroCity_PokemonCenter_1F_Text_PokemonHavePersonalities, MSGBOX_NPC
	end

RustboroCity_PokemonCenter_1F_EventScript_Boy::
	msgbox RustboroCity_PokemonCenter_1F_Text_MaleAndFemalePokemon, MSGBOX_NPC
	end

RustboroCity_PokemonCenter_1F_EventScript_Girl::
	msgbox RustboroCity_PokemonCenter_1F_Text_HMCutNextDoor, MSGBOX_NPC
	end

RustboroCity_PokemonCenter_1F_Text_PokemonHavePersonalities:
	.string "Mon Pokémon est de nature Naïf, celui\n"
	.string "de mon ami est Jovial.\p"
	.string "C'est fascinant, les Pokémon ont une\n"
	.string "personnalité !$"

RustboroCity_PokemonCenter_1F_Text_MaleAndFemalePokemon:
	.string "Comme les humains, il y a des Pokémon\n"
	.string "mâles et femelles.\p"
	.string "Mais personne ne sait vraiment ce qui\n"
	.string "les différencie.$"

RustboroCity_PokemonCenter_1F_Text_HMCutNextDoor:
	.string "Le monsieur d'à côté m'a donné une CS !\p"
	.string "Mon Pokémon a appris Coupe et peut\n"
	.string "abattre les petits arbres.$"

