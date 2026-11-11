RustboroCity_Flat1_1F_MapScripts::
	.byte 0

RustboroCity_Flat1_1F_EventScript_Man::
	msgbox RustboroCity_Flat1_1F_Text_EveryPokemonHasAbility, MSGBOX_NPC
	end

RustboroCity_Flat1_1F_EventScript_Woman::
	msgbox RustboroCity_Flat1_1F_Text_PokemonStrange, MSGBOX_NPC
	end

RustboroCity_Flat1_1F_Text_EveryPokemonHasAbility:
	.string "Chaque Pokémon possède un talent\n"
	.string "spécial.$"

RustboroCity_Flat1_1F_Text_PokemonStrange:
	.string "Les Pokémon sont vraiment des\n"
	.string "créatures étranges. Des fois, la nuit,\l"
	.string "je les entends parler entre eux.$"

