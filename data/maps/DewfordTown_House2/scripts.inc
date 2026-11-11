DewfordTown_House2_MapScripts::
	.byte 0

DewfordTown_House2_EventScript_Man::
	lock
	faceplayer
	goto_if_set FLAG_RECEIVED_SILK_SCARF, DewfordTown_House2_EventScript_ExplainSilkScarf
	msgbox DewfordTown_House2_Text_WantYouToHaveSilkScarf, MSGBOX_DEFAULT
	giveitem ITEM_SILK_SCARF
	goto_if_eq VAR_RESULT, FALSE, DewfordTown_House2_EventScript_NoRoomForScarf
	setflag FLAG_RECEIVED_SILK_SCARF
	release
	end

DewfordTown_House2_EventScript_NoRoomForScarf::
	msgbox DewfordTown_House2_Text_NoRoom, MSGBOX_DEFAULT
	release
	end

DewfordTown_House2_EventScript_ExplainSilkScarf::
	msgbox DewfordTown_House2_Text_ExplainSilkScarf, MSGBOX_DEFAULT
	release
	end

DewfordTown_House2_EventScript_Boy::
	msgbox DewfordTown_House2_Text_BrawlySoCool, MSGBOX_NPC
	end

DewfordTown_House2_Text_WantYouToHaveSilkScarf:
	.string "Admire un peu ça !\p"
	.string "C'est un Mouchoir Soie. Le top de la\n"
	.string "mode, ouais !\p"
	.string "Oh, je vois tes yeux briller ! Tu\n"
	.string "apprécies mon style éblouissant !\p"
	.string "Tu es un amour ! Tiens, je te l'offre !$"

DewfordTown_House2_Text_NoRoom:
	.string "Oh, t'as pas de place ?\p"
	.string "Écoute bien : ce Mouchoir est\n"
	.string "indispensable ! Je vendrais tous mes\l"
	.string "objets pour l'avoir !$"

DewfordTown_House2_Text_ExplainSilkScarf:
	.string "Le Mouchoir Soie augmente la puissance\n"
	.string "des capacités Normal.\p"
	.string "Un Mouchoir merveilleux qui va avec\n"
	.string "presque tous les Pokémon !$"

DewfordTown_House2_Text_BrawlySoCool:
	.string "Waouh, t'as traversé la mer pour venir à\n"
	.string "Myokara ?\p"
	.string "C'est pour Bastien, hein ?\p"
	.string "Il est tellement cool… Tout le monde\n"
	.string "l'adore. Moi, je l'ai vu sans T-shirt une\l"
	.string "fois. J'en dors plus.$"

