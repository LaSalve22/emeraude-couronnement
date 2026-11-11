OldaleTown_House2_MapScripts::
	.byte 0

OldaleTown_House2_EventScript_Woman::
	msgbox OldaleTown_House2_Text_PokemonLevelUp, MSGBOX_NPC
	end

OldaleTown_House2_EventScript_Man::
	msgbox OldaleTown_House2_Text_YoullGoFurtherWithStrongPokemon, MSGBOX_NPC
	end

OldaleTown_House2_Text_PokemonLevelUp:
	.string "À force de combattre, les Pokémon\n"
	.string "montent de niveau et deviennent plus\l"
	.string "forts. Comme les gens. Ou plus durs, en\n"
	.string "tout cas.$"

OldaleTown_House2_Text_YoullGoFurtherWithStrongPokemon:
	.string "Plus tes Pokémon seront forts, plus tu\n"
	.string "iras loin.\p"
	.string "Mon fils est parti avec un seul Pokémon,\n"
	.string "il y a six ans. Il m'envoie une carte à\l"
	.string "chaque Noël. Ça me suffit.$"

