@ Émeraude Couronnement : herboriste du Centre Pokémon de Clémenti-Ville.
@ Vend les 21 Aromates (changent la nature d'un Pokémon) après le badge de Norman. Prix : include/config/item.h.
Aromates_EventScript_Herboriste::
	lock
	faceplayer
	goto_if_unset FLAG_BADGE05_GET, Aromates_EventScript_PasEncore
	message Aromates_Text_Bienvenue
	waitmessage
	pokemart Aromates_Boutique
	msgbox Aromates_Text_AuRevoir, MSGBOX_DEFAULT
	release
	end

Aromates_EventScript_PasEncore::
	msgbox Aromates_Text_PasEncore, MSGBOX_DEFAULT
	release
	end

	.align 2
Aromates_Boutique:
	.2byte ITEM_LONELY_MINT
	.2byte ITEM_ADAMANT_MINT
	.2byte ITEM_NAUGHTY_MINT
	.2byte ITEM_BRAVE_MINT
	.2byte ITEM_BOLD_MINT
	.2byte ITEM_IMPISH_MINT
	.2byte ITEM_LAX_MINT
	.2byte ITEM_RELAXED_MINT
	.2byte ITEM_MODEST_MINT
	.2byte ITEM_MILD_MINT
	.2byte ITEM_RASH_MINT
	.2byte ITEM_QUIET_MINT
	.2byte ITEM_CALM_MINT
	.2byte ITEM_GENTLE_MINT
	.2byte ITEM_CAREFUL_MINT
	.2byte ITEM_SASSY_MINT
	.2byte ITEM_TIMID_MINT
	.2byte ITEM_HASTY_MINT
	.2byte ITEM_JOLLY_MINT
	.2byte ITEM_NAIVE_MINT
	.2byte ITEM_SERIOUS_MINT
	.2byte ITEM_NONE

Aromates_Text_Bienvenue:
	.string "Des Aromates, tout frais ! Un coup\n"
	.string "de nez et ton Pokémon change de\l"
	.string "nature. Garanti sans effets\l"
	.string "secondaires… presque.$"

Aromates_Text_AuRevoir:
	.string "Reviens quand ton Pokémon a encore\n"
	.string "une sale mentalité.$"

Aromates_Text_PasEncore:
	.string "Mes Aromates changent la nature\n"
	.string "d'un Pokémon. Ça ne se vend pas\l"
	.string "au premier venu.\p"
	.string "Reviens avec le Badge Balance de\n"
	.string "Norman, et on causera.$"
