RustboroCity_House3_MapScripts::
	.byte 0

RustboroCity_House3_EventScript_OldMan::
	msgbox RustboroCity_House3_Text_IGivePerfectlySuitedNicknames, MSGBOX_NPC
	end

RustboroCity_House3_EventScript_OldWoman::
	msgbox RustboroCity_House3_Text_NamingPikachuPekachu, MSGBOX_NPC
	end

@ Misspelling on purpose, see nickname
RustboroCity_House3_EventScript_Pekachu::
	lock
	faceplayer
	waitse
	playmoncry SPECIES_PIKACHU, CRY_MODE_NORMAL
	msgbox RustboroCity_House3_Text_Pekachu, MSGBOX_DEFAULT
	waitmoncry
	release
	end

RustboroCity_House3_Text_IGivePerfectlySuitedNicknames:
	.string "Je donne à mes Pokémon des surnoms\n"
	.string "parfaitement adaptés !\p"
	.string "C'est ma façon d'exprimer mon, euh…\n"
	.string "originalité, voilà !$"

RustboroCity_House3_Text_NamingPikachuPekachu:
	.string "Mais appeler un Pikachu “Pekachu” ? Ça\n"
	.string "n'a aucun sens.\p"
	.string "Un nom facile à comprendre, c'est bien,\n"
	.string "mais…$"

RustboroCity_House3_Text_Pekachu:
	.string "Pekachu : Peka !$"

