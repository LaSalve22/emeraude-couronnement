@ Émeraude Couronnement : Méga-Collectionneur du Centre Pokémon de Clémenti-Ville.
@ 40 Méga-Gemmes classiques sont cachées dans des Poké Balls du monde (tools/megas/placer.py) ; les autres
@ (Pokémon d'autres régions, légendaires, Légendes Z-A) sont vendues ici après la Ligue. Prix : include/config/item.h.
MegaGemmes_EventScript_Collectionneur::
	lock
	faceplayer
	goto_if_unset FLAG_SYS_GAME_CLEAR, MegaGemmes_EventScript_PasEncore
	message MegaGemmes_Text_Bienvenue
	waitmessage
	pokemart MegaGemmes_Boutique
	msgbox MegaGemmes_Text_AuRevoir, MSGBOX_DEFAULT
	release
	end

MegaGemmes_EventScript_PasEncore::
	msgbox MegaGemmes_Text_PasEncore, MSGBOX_DEFAULT
	release
	end

	.align 2
MegaGemmes_Boutique:
	.2byte ITEM_STRANGE_BALL @ Dark Ball
	.2byte ITEM_BLASTOISINITE
	.2byte ITEM_MEWTWONITE_X
	.2byte ITEM_MEWTWONITE_Y
	.2byte ITEM_SCEPTILITE
	.2byte ITEM_LATIASITE
	.2byte ITEM_LATIOSITE
	.2byte ITEM_DIANCITE
	.2byte ITEM_CLEFABLITE
	.2byte ITEM_VICTREEBELITE
	.2byte ITEM_STARMINITE
	.2byte ITEM_DRAGONINITE
	.2byte ITEM_MEGANIUMITE
	.2byte ITEM_FERALIGITE
	.2byte ITEM_SKARMORITE
	.2byte ITEM_FROSLASSITE
	.2byte ITEM_EMBOARITE
	.2byte ITEM_EXCADRITE
	.2byte ITEM_SCOLIPITE
	.2byte ITEM_SCRAFTINITE
	.2byte ITEM_EELEKTROSSITE
	.2byte ITEM_CHANDELURITE
	.2byte ITEM_CHESNAUGHTITE
	.2byte ITEM_DELPHOXITE
	.2byte ITEM_GRENINJITE
	.2byte ITEM_PYROARITE
	.2byte ITEM_FLOETTITE
	.2byte ITEM_MALAMARITE
	.2byte ITEM_BARBARACITE
	.2byte ITEM_DRAGALGITE
	.2byte ITEM_HAWLUCHANITE
	.2byte ITEM_ZYGARDITE
	.2byte ITEM_DRAMPANITE
	.2byte ITEM_FALINKSITE
	.2byte ITEM_HEATRANITE
	.2byte ITEM_DARKRANITE
	.2byte ITEM_ZERAORITE
	.2byte ITEM_RAICHUNITE_X
	.2byte ITEM_RAICHUNITE_Y
	.2byte ITEM_CHIMECHITE
	.2byte ITEM_ABSOLITE_Z
	.2byte ITEM_STARAPTITE
	.2byte ITEM_GARCHOMPITE_Z
	.2byte ITEM_LUCARIONITE_Z
	.2byte ITEM_GOLURKITE
	.2byte ITEM_MEOWSTICITE
	.2byte ITEM_CRABOMINITE
	.2byte ITEM_GOLISOPITE
	.2byte ITEM_MAGEARNITE
	.2byte ITEM_SCOVILLAINITE
	.2byte ITEM_BAXCALIBRITE
	.2byte ITEM_TATSUGIRINITE
	.2byte ITEM_GLIMMORANITE
	.2byte ITEM_NONE

MegaGemmes_Text_Bienvenue:
	.string "Des Méga-Gemmes de tout le monde,\n"
	.string "même celles qu'on ne trouve pas à\l"
	.string "Hoenn. Choisis, c'est pas donné.$"

MegaGemmes_Text_AuRevoir:
	.string "Les autres, je les ai semées dans\n"
	.string "Hoenn. Ouvre l'œil.$"

MegaGemmes_Text_PasEncore:
	.string "Ma collection de Méga-Gemmes n'est\n"
	.string "pas pour les touristes.\p"
	.string "Il y en a une quarantaine cachées\n"
	.string "dans Hoenn, à partir de la Route 118.\p"
	.string "Bats la Ligue et je t'ouvre ma\n"
	.string "vitrine.$"
