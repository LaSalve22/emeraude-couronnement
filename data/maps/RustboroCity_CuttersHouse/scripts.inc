RustboroCity_CuttersHouse_MapScripts::
	.byte 0

RustboroCity_CuttersHouse_EventScript_Cutter::
	lock
	faceplayer
	goto_if_set FLAG_RECEIVED_HM_CUT, RustboroCity_CuttersHouse_EventScript_ExplainCut
	msgbox RustboroCity_CuttersHouse_Text_YouCanPutThisHMToGoodUse, MSGBOX_DEFAULT
	giveitem ITEM_HM_CUT
	setflag FLAG_RECEIVED_HM_CUT
	msgbox RustboroCity_CuttersHouse_Text_ExplainCut, MSGBOX_DEFAULT
	release
	end

RustboroCity_CuttersHouse_EventScript_ExplainCut::
	msgbox RustboroCity_CuttersHouse_Text_ExplainCut, MSGBOX_DEFAULT
	release
	end

RustboroCity_CuttersHouse_EventScript_Lass::
	msgbox RustboroCity_CuttersHouse_Text_DadHelpedClearLandOfTrees, MSGBOX_NPC
	end

RustboroCity_CuttersHouse_Text_YouCanPutThisHMToGoodUse:
	.string "Ce regard déterminé… Cette souplesse…\n"
	.string "Et ces Pokémon bien entraînés…\p"
	.string "Aucun doute, tu sais te battre !\p"
	.string "Non, attends, ne dis rien. Je le vois rien\n"
	.string "qu'à te regarder.\p"
	.string "Tu sauras faire bon usage de cette\n"
	.string "Capsule Secrète.\p"
	.string "Pas de fausse modestie. Allez,\n"
	.string "prends-la !$"

RustboroCity_CuttersHouse_Text_ExplainCut:
	.string "Cette Capsule Secrète, ou CS, contient\n"
	.string "Coupe.\p"
	.string "Une attaque de CS peut être utilisée\n"
	.string "par les Pokémon hors combat.\p"
	.string "Un Pokémon qui connaît Coupe peut\n"
	.string "abattre les petits arbres si son\l"
	.string "Dresseur a le Badge Roche.\p"
	.string "Et comme les CT, une CS s'utilise autant\n"
	.string "de fois que tu veux.$"

RustboroCity_CuttersHouse_Text_DadHelpedClearLandOfTrees:
	.string "Quand Mérouville s'est agrandie, mon\n"
	.string "père a donné un coup de main.\p"
	.string "Ses Pokémon ont rasé la forêt à coups\n"
	.string "de Coupe. Il disait que les arbres\l"
	.string "criaient. Je ne l'ai jamais cru.$"

