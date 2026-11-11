RustboroCity_DevonCorp_1F_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, RustboroCity_DevonCorp_1F_OnTransition
	.byte 0

RustboroCity_DevonCorp_1F_OnTransition:
	call_if_unset FLAG_RETURNED_DEVON_GOODS, RustboroCity_DevonCorp_1F_EventScript_BlockStairs
	end

RustboroCity_DevonCorp_1F_EventScript_BlockStairs::
	setobjectxyperm LOCALID_DEVON_CORP_STAIR_GUARD, 14, 2
	setobjectmovementtype LOCALID_DEVON_CORP_STAIR_GUARD, MOVEMENT_TYPE_FACE_DOWN
	return

RustboroCity_DevonCorp_1F_EventScript_Employee::
	lock
	faceplayer
	goto_if_set FLAG_RETURNED_DEVON_GOODS, RustboroCity_DevonCorp_1F_EventScript_GoodsRecovered
	goto_if_set FLAG_DEVON_GOODS_STOLEN, RustboroCity_DevonCorp_1F_EventScript_RobberWasntBright
	msgbox RustboroCity_DevonCorp_1F_Text_ThoseShoesAreOurProduct, MSGBOX_DEFAULT
	release
	end

RustboroCity_DevonCorp_1F_EventScript_RobberWasntBright::
	msgbox RustboroCity_DevonCorp_1F_Text_RobberWasntVeryBright, MSGBOX_DEFAULT
	release
	end

RustboroCity_DevonCorp_1F_EventScript_GoodsRecovered::
	msgbox RustboroCity_DevonCorp_1F_Text_SoundsLikeStolenGoodsRecovered, MSGBOX_DEFAULT
	release
	end

RustboroCity_DevonCorp_1F_EventScript_StairGuard::
	lock
	faceplayer
	goto_if_set FLAG_RETURNED_DEVON_GOODS, RustboroCity_DevonCorp_1F_EventScript_AlwaysWelcome
	goto_if_set FLAG_RECOVERED_DEVON_GOODS, RustboroCity_DevonCorp_1F_EventScript_GotRobbed
	goto_if_set FLAG_DEVON_GOODS_STOLEN, RustboroCity_DevonCorp_1F_EventScript_GotRobbed
	msgbox RustboroCity_DevonCorp_1F_Text_OnlyAuthorizedPeopleEnter, MSGBOX_DEFAULT
	release
	end

RustboroCity_DevonCorp_1F_EventScript_AlwaysWelcome::
	msgbox RustboroCity_DevonCorp_1F_Text_YoureAlwaysWelcomeHere, MSGBOX_DEFAULT
	release
	end

RustboroCity_DevonCorp_1F_EventScript_GotRobbed::
	msgbox RustboroCity_DevonCorp_1F_Text_HowCouldWeGetRobbed, MSGBOX_DEFAULT
	release
	end

RustboroCity_DevonCorp_1F_EventScript_Greeter::
	lock
	faceplayer
	goto_if_set FLAG_RETURNED_DEVON_GOODS, RustboroCity_DevonCorp_1F_EventScript_WelcomeToDevonCorp
	goto_if_set FLAG_RECOVERED_DEVON_GOODS, RustboroCity_DevonCorp_1F_EventScript_StaffGotRobbed
	goto_if_set FLAG_DEVON_GOODS_STOLEN, RustboroCity_DevonCorp_1F_EventScript_StaffGotRobbed
	msgbox RustboroCity_DevonCorp_1F_Text_WelcomeToDevonCorp, MSGBOX_DEFAULT
	release
	end

RustboroCity_DevonCorp_1F_EventScript_WelcomeToDevonCorp::
	msgbox RustboroCity_DevonCorp_1F_Text_WelcomeToDevonCorp, MSGBOX_DEFAULT
	release
	end

RustboroCity_DevonCorp_1F_EventScript_StaffGotRobbed::
	msgbox RustboroCity_DevonCorp_1F_Text_StaffGotRobbed, MSGBOX_DEFAULT
	release
	end

RustboroCity_DevonCorp_1F_EventScript_RocksMetalDisplay::
	msgbox RustboroCity_DevonCorp_1F_Text_RocksMetalDisplay, MSGBOX_SIGN
	end

RustboroCity_DevonCorp_1F_EventScript_ProductsDisplay::
	msgbox RustboroCity_DevonCorp_1F_Text_ProductDisplay, MSGBOX_SIGN
	end

RustboroCity_DevonCorp_1F_Text_WelcomeToDevonCorp:
	.string "Bonjour et bienvenue chez Devon SA.\p"
	.string "Nous sommes fiers de produire des\n"
	.string "objets et des médicaments qui\l"
	.string "améliorent votre vie.$"

RustboroCity_DevonCorp_1F_Text_StaffGotRobbed:
	.string "Un de nos chercheurs s'est fait voler\n"
	.string "un colis important. Comme un idiot.$"

RustboroCity_DevonCorp_1F_Text_ThoseShoesAreOurProduct:
	.string "Hé, ces Chaussures de sport ! C'est un\n"
	.string "de nos produits !\p"
	.string "Ça fait plaisir de voir quelqu'un\n"
	.string "utiliser ce qu'on fabrique.$"

RustboroCity_DevonCorp_1F_Text_RobberWasntVeryBright:
	.string "Ce colis volé…\p"
	.string "Il est important, oui, mais personne ne\n"
	.string "peut s'en servir. Pas sans nos\l"
	.string "laboratoires.\p"
	.string "À mon avis, ce voleur n'était pas une\n"
	.string "lumière. Ou alors, il savait exactement\l"
	.string "ce qu'il faisait.$"

RustboroCity_DevonCorp_1F_Text_SoundsLikeStolenGoodsRecovered:
	.string "Il paraît qu'on a récupéré les Pièces\n"
	.string "Devon volées.$"

RustboroCity_DevonCorp_1F_Text_OnlyAuthorizedPeopleEnter:
	.string "Désolé, l'accès est réservé au\n"
	.string "personnel autorisé.$"

RustboroCity_DevonCorp_1F_Text_HowCouldWeGetRobbed:
	.string "C'est d'une connerie sans nom. Comment\n"
	.string "on a pu se faire voler ?$"

RustboroCity_DevonCorp_1F_Text_YoureAlwaysWelcomeHere:
	.string "Bonjour ! Ici, tu es toujours chez toi !$"

RustboroCity_DevonCorp_1F_Text_RocksMetalDisplay:
	.string "Des échantillons de roches et de\n"
	.string "métaux sont exposés dans la vitrine.\p"
	.string "Un panneau explique…\p"
	.string "“Devon a débuté comme exploitant de\n"
	.string "carrières.\p"
	.string "“L'entreprise a ensuite extrait le fer\n"
	.string "du sable.\p"
	.string "“De ces humbles débuts dans les\n"
	.string "matières premières, Devon s'est\l"
	.string "développée.\p"
	.string "“Aujourd'hui, Devon fabrique une vaste\n"
	.string "gamme de produits industriels.”$"

RustboroCity_DevonCorp_1F_Text_ProductDisplay:
	.string "Des prototypes et des produits en test\n"
	.string "remplissent la vitrine.\p"
	.string "Un panneau explique…\p"
	.string "“En plus de ses produits industriels,\n"
	.string "Devon commercialise des articles du\l"
	.string "quotidien et des médicaments.\p"
	.string "“Depuis peu, Devon propose aussi des\n"
	.string "outils pour Dresseurs, dont les Poké\l"
	.string "Balls et le PokéNav.”$"

