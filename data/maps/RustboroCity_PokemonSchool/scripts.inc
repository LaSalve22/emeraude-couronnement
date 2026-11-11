RustboroCity_PokemonSchool_MapScripts::
	.byte 0

RustboroCity_PokemonSchool_EventScript_Blackboard::
	lockall
	msgbox RustboroCity_PokemonSchool_Text_BlackboardListsStatusChanges, MSGBOX_DEFAULT
	goto RustboroCity_PokemonSchool_EventScript_ChooseBlackboardTopic
	end

RustboroCity_PokemonSchool_EventScript_ChooseBlackboardTopic::
	message RustboroCity_PokemonSchool_Text_ReadWhichTopic
	waitmessage
	multichoicegrid 8, 1, MULTI_STATUS_INFO, 3, FALSE
	switch VAR_RESULT
	case 0, RustboroCity_PokemonSchool_EventScript_Poison
	case 1, RustboroCity_PokemonSchool_EventScript_Paralysis
	case 2, RustboroCity_PokemonSchool_EventScript_Sleep
	case 3, RustboroCity_PokemonSchool_EventScript_Burn
	case 4, RustboroCity_PokemonSchool_EventScript_Freeze
	case 5, RustboroCity_PokemonSchool_EventScript_ExitTopicSelect
	case MULTI_B_PRESSED, RustboroCity_PokemonSchool_EventScript_ExitTopicSelect
	end

RustboroCity_PokemonSchool_EventScript_Poison::
	msgbox RustboroCity_PokemonSchool_Text_ExplainPoison, MSGBOX_DEFAULT
	goto RustboroCity_PokemonSchool_EventScript_ChooseBlackboardTopic
	end

RustboroCity_PokemonSchool_EventScript_Paralysis::
	msgbox RustboroCity_PokemonSchool_Text_ExplainParalysis, MSGBOX_DEFAULT
	goto RustboroCity_PokemonSchool_EventScript_ChooseBlackboardTopic
	end

RustboroCity_PokemonSchool_EventScript_Sleep::
	msgbox RustboroCity_PokemonSchool_Text_ExplainSleep, MSGBOX_DEFAULT
	goto RustboroCity_PokemonSchool_EventScript_ChooseBlackboardTopic
	end

RustboroCity_PokemonSchool_EventScript_Burn::
	msgbox RustboroCity_PokemonSchool_Text_ExplainBurn, MSGBOX_DEFAULT
	goto RustboroCity_PokemonSchool_EventScript_ChooseBlackboardTopic
	end

RustboroCity_PokemonSchool_EventScript_Freeze::
	msgbox RustboroCity_PokemonSchool_Text_ExplainFreeze, MSGBOX_DEFAULT
	goto RustboroCity_PokemonSchool_EventScript_ChooseBlackboardTopic
	end

RustboroCity_PokemonSchool_EventScript_ExitTopicSelect::
	releaseall
	end

RustboroCity_PokemonSchool_EventScript_GameboyKid1::
	msgbox RustboroCity_PokemonSchool_Text_TradingRightNow, MSGBOX_NPC
	end

RustboroCity_PokemonSchool_EventScript_GameboyKid2::
	msgbox RustboroCity_PokemonSchool_Text_AlwaysWantedSeedot, MSGBOX_NPC
	end

RustboroCity_PokemonSchool_EventScript_RichBoy::
	msgbox RustboroCity_PokemonSchool_Text_PokemontCantUseManMadeItems, MSGBOX_NPC
	end

RustboroCity_PokemonSchool_EventScript_Lass::
	msgbox RustboroCity_PokemonSchool_Text_ConfusedPokemonAttacksItself, MSGBOX_NPC
	end

RustboroCity_PokemonSchool_EventScript_SchoolKidM::
	msgbox RustboroCity_PokemonSchool_Text_PokemonHealItselfWithBerry, MSGBOX_NPC
	end

RustboroCity_PokemonSchool_EventScript_StudentNotebook::
	msgbox RustboroCity_PokemonSchool_Text_StudentsNotes, MSGBOX_SIGN
	end

RustboroCity_PokemonSchool_EventScript_Teacher::
	lock
	faceplayer
	goto_if_set FLAG_RECEIVED_QUICK_CLAW, RustboroCity_PokemonSchool_EventScript_GaveQuickClaw
	call_if_eq VAR_FACING, DIR_EAST, RustboroCity_PokemonSchool_EventScript_TeacherCheckOnStudentsEast
	call_if_eq VAR_FACING, DIR_WEST, RustboroCity_PokemonSchool_EventScript_TeacherCheckOnStudentsWest
	msgbox RustboroCity_PokemonSchool_Text_StudentsWhoDontStudyGetQuickClaw, MSGBOX_DEFAULT
	giveitem ITEM_QUICK_CLAW
	goto_if_eq VAR_RESULT, 0, Common_EventScript_ShowBagIsFull
	closemessage
	applymovement VAR_LAST_TALKED, Common_Movement_WalkInPlaceFasterDown
	waitmovement 0
	setflag FLAG_RECEIVED_QUICK_CLAW
	release
	end

RustboroCity_PokemonSchool_EventScript_TeacherCheckOnStudentsEast::
	applymovement VAR_LAST_TALKED, RustboroCity_PokemonSchool_Movement_TeacherCheckOnStudentsEast
	waitmovement 0
	return

RustboroCity_PokemonSchool_EventScript_TeacherCheckOnStudentsWest::
	applymovement VAR_LAST_TALKED, RustboroCity_PokemonSchool_Movement_TeacherCheckOnStudentsWest
	waitmovement 0
	return

RustboroCity_PokemonSchool_EventScript_GaveQuickClaw::
	msgbox RustboroCity_PokemonSchool_Text_ExplainQuickClaw, MSGBOX_DEFAULT
	closemessage
	applymovement VAR_LAST_TALKED, Common_Movement_WalkInPlaceFasterDown
	waitmovement 0
	release
	end

RustboroCity_PokemonSchool_Movement_TeacherCheckOnStudentsWest:
	walk_left
	walk_down
	walk_down
	walk_right
	walk_in_place_faster_down
	delay_16
	delay_16
	delay_16
	walk_down
	walk_in_place_faster_left
	delay_16
	delay_16
	walk_right
	delay_16
	delay_16
	delay_8
	walk_up
	walk_left
	walk_left
	walk_up
	walk_up
	walk_right
	step_end

RustboroCity_PokemonSchool_Movement_TeacherCheckOnStudentsEast:
	walk_right
	walk_right
	walk_down
	walk_down
	walk_left
	walk_left
	walk_in_place_faster_down
	delay_16
	delay_16
	delay_16
	walk_down
	walk_in_place_faster_left
	delay_16
	delay_16
	walk_right
	delay_16
	delay_16
	delay_8
	walk_up
	walk_right
	walk_up
	walk_up
	walk_left
	walk_left
	step_end

RustboroCity_PokemonSchool_EventScript_Scott::
	lock
	faceplayer
	goto_if_set FLAG_MET_SCOTT_AFTER_OBTAINING_STONE_BADGE, RustboroCity_PokemonSchool_EventScript_ScottWatchStudents
	goto_if_set FLAG_MET_SCOTT_RUSTBORO, RustboroCity_PokemonSchool_EventScript_ScottSpokeAlready
	goto_if_set FLAG_BADGE01_GET, RustboroCity_PokemonSchool_EventScript_ScottGreetHasBadge
	msgbox RustboroCity_PokemonSchool_Text_ScottMetAlreadyCut, MSGBOX_DEFAULT
	addvar VAR_SCOTT_STATE, 1
	setflag FLAG_MET_SCOTT_RUSTBORO
	release
	end

RustboroCity_PokemonSchool_EventScript_ScottSpokeAlready::
	goto_if_set FLAG_BADGE01_GET, RustboroCity_PokemonSchool_EventScript_ScottNoticeBadge
	msgbox RustboroCity_PokemonSchool_Text_StudentTalentLevelUnknown, MSGBOX_DEFAULT
	release
	end

RustboroCity_PokemonSchool_EventScript_ScottGreetHasBadge::
	msgbox RustboroCity_PokemonSchool_Text_ScottMetAlreadyStoneBadge, MSGBOX_DEFAULT
	goto RustboroCity_PokemonSchool_EventScript_MetScottAfterBadge
	end

RustboroCity_PokemonSchool_EventScript_ScottNoticeBadge::
	msgbox RustboroCity_PokemonSchool_Text_ScottStoneBadge, MSGBOX_DEFAULT
	goto RustboroCity_PokemonSchool_EventScript_MetScottAfterBadge
	end

RustboroCity_PokemonSchool_EventScript_MetScottAfterBadge::
	addvar VAR_SCOTT_STATE, 1
	setflag FLAG_MET_SCOTT_AFTER_OBTAINING_STONE_BADGE
	release
	end

RustboroCity_PokemonSchool_EventScript_ScottWatchStudents::
	msgbox RustboroCity_PokemonSchool_Text_StudentTalentLevelUnknown, MSGBOX_DEFAULT
	release
	end

RustboroCity_PokemonSchool_Text_BlackboardListsStatusChanges:
	.string "Le tableau liste les altérations d'état\n"
	.string "qui peuvent toucher les Pokémon en\l"
	.string "combat.$"

RustboroCity_PokemonSchool_Text_ReadWhichTopic:
	.string "Quel sujet veux-tu lire ?$"

RustboroCity_PokemonSchool_Text_ExplainPoison:
	.string "Un Pokémon empoisonné perd des PV en\n"
	.string "continu.\p"
	.string "Le poison persiste après le combat.\p"
	.string "Il continue de faire effet pendant les\n"
	.string "déplacements. Lentement.\l"
	.string "Douloureusement.\p"
	.string "Soignez-le avec un Antidote.$"

RustboroCity_PokemonSchool_Text_ExplainParalysis:
	.string "Un Pokémon paralysé voit sa Vitesse\n"
	.string "chuter.\p"
	.string "Il peut aussi rester figé pendant le\n"
	.string "combat, incapable de bouger.\p"
	.string "La paralysie persiste après le combat.\n"
	.string "Soignez-la avec un Anti-Para.$"

RustboroCity_PokemonSchool_Text_ExplainSleep:
	.string "Un Pokémon endormi ne peut pas\n"
	.string "attaquer.\p"
	.string "Il peut se réveiller seul, mais s'il dort\n"
	.string "à la fin du combat, il restera endormi.\p"
	.string "Réveillez-le avec un Réveil.$"

RustboroCity_PokemonSchool_Text_ExplainBurn:
	.string "Une brûlure réduit l'Attaque et ronge\n"
	.string "les PV de la victime.\p"
	.string "Elle persiste après le combat.\n"
	.string "Soignez-la avec un Anti-Brûle.$"

RustboroCity_PokemonSchool_Text_ExplainFreeze:
	.string "Un Pokémon gelé est totalement sans\n"
	.string "défense.\p"
	.string "Il reste gelé après le combat.\n"
	.string "Dégelez-le avec un Antigel.$"

RustboroCity_PokemonSchool_Text_StudentsWhoDontStudyGetQuickClaw:
	.string "Les élèves qui ne travaillent pas\n"
	.string "goûtent à ma Vive Griffe.\p"
	.string "Ta façon d'utiliser cet objet dira si tu\n"
	.string "es un bon élève. Ou un cas désespéré.$"

RustboroCity_PokemonSchool_Text_ExplainQuickClaw:
	.string "Un Pokémon qui tient une Vive Griffe\n"
	.string "peut parfois accélérer et agir avant\l"
	.string "son adversaire.\p"
	.string "Il existe beaucoup d'autres objets à\n"
	.string "faire tenir aux Pokémon.\p"
	.string "Rien qu'avec ça, tu as de quoi réviser\n"
	.string "jusqu'à la fin de tes jours !$"

RustboroCity_PokemonSchool_Text_TradingRightNow:
	.string "Je suis en train d'échanger des Pokémon\n"
	.string "avec mon pote.$"

RustboroCity_PokemonSchool_Text_AlwaysWantedSeedot:
	.string "J'ai toujours voulu un Grainipiot, et je\n"
	.string "vais enfin en avoir un !$"

RustboroCity_PokemonSchool_Text_PokemontCantUseManMadeItems:
	.string "Les Pokémon peuvent tenir des objets,\n"
	.string "mais ils ne savent pas se servir des\l"
	.string "objets humains comme les Potions ou les\n"
	.string "Antidotes.$"

RustboroCity_PokemonSchool_Text_ConfusedPokemonAttacksItself:
	.string "Tu sais que certaines attaques rendent\n"
	.string "les Pokémon confus ?\p"
	.string "Un Pokémon confus peut se blesser\n"
	.string "lui-même sans le vouloir.\p"
	.string "Mais une fois sorti du combat, il\n"
	.string "redevient normal.$"

RustboroCity_PokemonSchool_Text_PokemonHealItselfWithBerry:
	.string "Un Pokémon qui tient une Baie se soigne\n"
	.string "tout seul…\p"
	.string "Il y a plein d'objets à faire tenir…\p"
	.string "Pfff, c'est dur de tout noter…$"

RustboroCity_PokemonSchool_Text_StudentsNotes:
	.string "Le cahier d'un élève…\p"
	.string "On capture les Pokémon avec des Poké\n"
	.string "Balls.\p"
	.string "Un Dresseur peut avoir jusqu'à six\n"
	.string "Pokémon avec lui.\p"
	.string "Un Dresseur capture des Pokémon, les\n"
	.string "entraîne et les fait combattre.\p"
	.string "La mission d'un Dresseur : vaincre les\n"
	.string "Dresseurs redoutables qui l'attendent\l"
	.string "dans les Arènes.\p"
	.string "Dans la marge, quelqu'un a écrit : “Et\n"
	.string "revenir vivant.”$"

RustboroCity_PokemonSchool_Text_ScottMetAlreadyCut:
	.string "Tiens ? On ne s'est pas déjà vus ? À\n"
	.string "Clémenti-Ville, je crois.\p"
	.string "Je me présente. Je m'appelle Scott.\p"
	.string "Je parcours le pays à la recherche de\n"
	.string "Dresseurs exceptionnels.\p"
	.string "Plus précisément, des experts du\n"
	.string "combat Pokémon. Des vrais. Ceux qui\l"
	.string "n'ont plus peur de rien.\p"
	.string "Qu'est-ce qui t'amène dans cette école\n"
	.string "? Toi aussi, tu es Dresseur ?\p"
	.string "D'abord, apprends Coupe à un de tes\n"
	.string "Pokémon.\p"
	.string "Si je me souviens bien, quelqu'un en\n"
	.string "ville possède cette CS.$"

RustboroCity_PokemonSchool_Text_StudentTalentLevelUnknown:
	.string "Scott : Hmm…\p"
	.string "Le niveau des élèves ici est encore un\n"
	.string "mystère. Le potentiel est là. Reste à\l"
	.string "voir qui tiendra.$"

RustboroCity_PokemonSchool_Text_ScottStoneBadge:
	.string "Scott : Oh, qu'est-ce que c'est ?\p"
	.string "C'est un Badge Roche, non ? Plutôt\n"
	.string "impressionnant.\p"
	.string "Mais tu sais, j'aurais adoré te voir\n"
	.string "combattre.\p"
	.string "Un Badge ne dit pas grand-chose du\n"
	.string "Dresseur qui le porte.$"

RustboroCity_PokemonSchool_Text_ScottMetAlreadyStoneBadge:
	.string "Tiens ? On ne s'est pas déjà vus ? À\n"
	.string "Clémenti-Ville, je crois.\p"
	.string "Je me présente. Je m'appelle Scott.\p"
	.string "Je parcours le pays à la recherche de\n"
	.string "Dresseurs exceptionnels.\p"
	.string "Plus précisément, des experts du\n"
	.string "combat Pokémon.\p"
	.string "Oh, qu'est-ce que c'est ? Un Badge\n"
	.string "Roche ?\p"
	.string "Plutôt impressionnant, pour quelqu'un\n"
	.string "qui débute.\p"
	.string "Mais tu sais, j'aurais adoré te voir\n"
	.string "combattre.\p"
	.string "Un Badge ne dit pas grand-chose du\n"
	.string "Dresseur qui le porte.$"

