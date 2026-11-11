GraniteCave_1F_MapScripts::
	.byte 0

GraniteCave_1F_EventScript_Hiker::
	lock
	faceplayer
	goto_if_set FLAG_RECEIVED_HM_FLASH, GraniteCave_1F_EventScript_ReceivedFlash
	msgbox GraniteCave_1F_Text_GetsDarkAheadHereYouGo, MSGBOX_DEFAULT
	giveitem ITEM_HM_FLASH
	setflag FLAG_RECEIVED_HM_FLASH
	msgbox GraniteCave_1F_Text_ExplainFlash, MSGBOX_DEFAULT
	release
	end

GraniteCave_1F_EventScript_ReceivedFlash::
	msgbox GraniteCave_1F_Text_ExplainFlash, MSGBOX_DEFAULT
	release
	end

GraniteCave_1F_Text_GetsDarkAheadHereYouGo:
	.string "Hé, toi. Plus loin, il fait noir comme dans\n"
	.string "un four. Pas facile d'explorer.\p"
	.string "Le type qui est passé tout à l'heure…\n"
	.string "Pierre, je crois.\p"
	.string "Il savait utiliser Flash, alors il devrait\n"
	.string "s'en sortir, mais…\p"
	.string "Nous, les Montagnards, on aide ceux\n"
	.string "qu'on croise. C'est notre règle. La\l"
	.string "montagne ne pardonne pas.\p"
	.string "Tiens, prends ça.$"

GraniteCave_1F_Text_ExplainFlash:
	.string "Apprends Flash à un Pokémon et\n"
	.string "utilise-la.\p"
	.string "Elle éclaire même les ténèbres des\n"
	.string "grottes.\p"
	.string "Mais il te faut le Badge de l'Arène de\n"
	.string "Myokara.$"

