DewfordTown_House1_MapScripts::
	.byte 0

DewfordTown_House1_EventScript_Man::
	msgbox DewfordTown_House1_Text_LotToBeSaidForLivingOnIsland, MSGBOX_NPC
	end

DewfordTown_House1_EventScript_Woman::
	msgbox DewfordTown_House1_Text_LifeGoesSlowlyOnIsland, MSGBOX_NPC
	end

DewfordTown_House1_EventScript_Zigzagoon::
	lock
	faceplayer
	waitse
	playmoncry SPECIES_ZIGZAGOON, CRY_MODE_NORMAL
	msgbox DewfordTown_House1_Text_Zigzagoon, MSGBOX_DEFAULT
	waitmoncry
	release
	end

DewfordTown_House1_Text_LotToBeSaidForLivingOnIsland:
	.string "Vivre sur une petite île comme celle-ci,\n"
	.string "en harmonie avec les Pokémon et la\l"
	.string "famille… ça a du bon.$"

DewfordTown_House1_Text_LifeGoesSlowlyOnIsland:
	.string "J'ai quitté le grand port de Poivressel\n"
	.string "pour épouser mon mari, ici.\p"
	.string "La vie est lente sur cette petite île.\n"
	.string "Mais être entourée par la mer… c'est\l"
	.string "aussi ça, le bonheur. Même les jours de\n"
	.string "tempête.$"

DewfordTown_House1_Text_Zigzagoon:
	.string "Zigzaton : Guguuh !$"

