Route109_SeashoreHouse_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, Route109_SeashoreHouse_OnTransition
	.byte 0

Route109_SeashoreHouse_OnTransition:
	setflag FLAG_LANDMARK_SEASHORE_HOUSE
	end

Route109_SeashoreHouse_EventScript_Owner::
	lock
	faceplayer
	goto_if_set FLAG_RECEIVED_6_SODA_POP, Route109_SeashoreHouse_EventScript_AlreadyReceivedSodaPop
	goto_if_set FLAG_DEFEATED_SEASHORE_HOUSE, Route109_SeashoreHouse_EventScript_DefeatedTrainers
	goto_if_set FLAG_TEMP_2, Route109_SeashoreHouse_EventScript_AlreadyGaveIntroduction
	msgbox Route109_SeashoreHouse_Text_SeashoreHouseIntro, MSGBOX_DEFAULT
	setflag FLAG_TEMP_2
	release
	end

Route109_SeashoreHouse_EventScript_AlreadyGaveIntroduction::
	msgbox Route109_SeashoreHouse_Text_ShowMeSomeHotMatches, MSGBOX_DEFAULT
	release
	end

Route109_SeashoreHouse_EventScript_DefeatedTrainers::
	msgbox Route109_SeashoreHouse_Text_TakeTheseSodaPopBottles, MSGBOX_DEFAULT
	giveitem ITEM_SODA_POP, 6
	goto_if_eq VAR_RESULT, FALSE, Route109_SeashoreHouse_EventScript_BagFull
	setflag FLAG_RECEIVED_6_SODA_POP
	release
	end

Route109_SeashoreHouse_EventScript_BagFull::
	msgbox Route109_SeashoreHouse_Text_BagFull, MSGBOX_DEFAULT
	release
	end

Route109_SeashoreHouse_EventScript_AlreadyReceivedSodaPop::
	showmoneybox 0, 0
	msgbox Route109_SeashoreHouse_Text_WantToBuySodaPop, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, YES, Route109_SeashoreHouse_EventScript_BuySodaPop
	msgbox Route109_SeashoreHouse_Text_ThatsTooBad, MSGBOX_DEFAULT
	hidemoneybox
	release
	end

Route109_SeashoreHouse_EventScript_BuySodaPop::
	checkmoney 300
	goto_if_eq VAR_RESULT, FALSE, Route109_SeashoreHouse_EventScript_NotEnoughMoney
	checkitemspace ITEM_SODA_POP
	goto_if_eq VAR_RESULT, FALSE, Route109_SeashoreHouse_EventScript_NotEnoughSpace
	msgbox Route109_SeashoreHouse_Text_HereYouGo, MSGBOX_DEFAULT
	removemoney 300
	updatemoneybox
.if OW_SHOW_ITEM_DESCRIPTIONS != OW_ITEM_DESCRIPTIONS_OFF
	hidemoneybox
	giveitem ITEM_SODA_POP
.else
	giveitem ITEM_SODA_POP
	hidemoneybox
.endif
	release
	end

Route109_SeashoreHouse_EventScript_NotEnoughMoney::
	msgbox Route109_SeashoreHouse_Text_NotEnoughMoney, MSGBOX_DEFAULT
	hidemoneybox
	release
	end

Route109_SeashoreHouse_EventScript_NotEnoughSpace::
	msgbox gText_TooBadBagIsFull, MSGBOX_DEFAULT
	hidemoneybox
	release
	end

Route109_SeashoreHouse_EventScript_Dwayne::
	trainerbattle_single TRAINER_DWAYNE, Route109_SeashoreHouse_Text_DwayneIntro, Route109_SeashoreHouse_Text_DwayneDefeated, Route109_SeashoreHouse_EventScript_CheckTrainersCompletion
	msgbox Route109_SeashoreHouse_Text_DwaynePostBattle, MSGBOX_AUTOCLOSE
	end

Route109_SeashoreHouse_EventScript_Johanna::
	trainerbattle_single TRAINER_JOHANNA, Route109_SeashoreHouse_Text_JohannaIntro, Route109_SeashoreHouse_Text_JohannaDefeated, Route109_SeashoreHouse_EventScript_CheckTrainersCompletion
	msgbox Route109_SeashoreHouse_Text_JohannaPostBattle, MSGBOX_AUTOCLOSE
	end

Route109_SeashoreHouse_EventScript_Simon::
	trainerbattle_single TRAINER_SIMON, Route109_SeashoreHouse_Text_SimonIntro, Route109_SeashoreHouse_Text_SimonDefeated, Route109_SeashoreHouse_EventScript_CheckTrainersCompletion
	msgbox Route109_SeashoreHouse_Text_SimonPostBattle, MSGBOX_AUTOCLOSE
	end

Route109_SeashoreHouse_EventScript_CheckTrainersCompletion::
	goto_if_not_defeated TRAINER_DWAYNE, Route109_SeashoreHouse_EventScript_TrainersNotCompleted
	goto_if_not_defeated TRAINER_JOHANNA, Route109_SeashoreHouse_EventScript_TrainersNotCompleted
	goto_if_not_defeated TRAINER_SIMON, Route109_SeashoreHouse_EventScript_TrainersNotCompleted
	setflag FLAG_DEFEATED_SEASHORE_HOUSE
	release
	end

Route109_SeashoreHouse_EventScript_TrainersNotCompleted::
	release
	end

Route109_SeashoreHouse_Text_SeashoreHouseIntro:
	.string "Je suis le patron de la Maison de la\n"
	.string "Plage. Mais appelle-moi M. Mer !\p"
	.string "Ce que j'aime par-dessus tout, ce sont\n"
	.string "les combats Pokémon brûlants.\p"
	.string "Montre-moi que ton cœur est en feu !\p"
	.string "Si tu bats tous les Dresseurs ici, je\n"
	.string "récompenserai tes efforts.$"

Route109_SeashoreHouse_Text_ShowMeSomeHotMatches:
	.string "Montre-moi des combats brûlants !\p"
	.string "Je tiens cette Maison de la Plage\n"
	.string "uniquement pour ça !$"

Route109_SeashoreHouse_Text_TakeTheseSodaPopBottles:
	.string "T'es en fusion ! Ces combats étaient\n"
	.string "incandescents ! Je suis plus que\l"
	.string "satisfait !\p"
	.string "Pour te remercier de ce spectacle,\n"
	.string "prends ça.\p"
	.string "Une demi-douzaine de bouteilles de\n"
	.string "Soda Cool !$"

Route109_SeashoreHouse_Text_BagFull:
	.string "Oh, mais ton Sac est plein à craquer. Je\n"
	.string "te les garde de côté.$"

Route109_SeashoreHouse_Text_WantToBuySodaPop:
	.string "Tu veux du Soda Cool ? Les Pokémon\n"
	.string "adorent ça !\p"
	.string "Seulement 300 ¥ la bouteille ! Achète !$"

Route109_SeashoreHouse_Text_HereYouGo:
	.string "Et voilà !$"

Route109_SeashoreHouse_Text_NotEnoughMoney:
	.string "T'as pas assez d'argent.$"

Route109_SeashoreHouse_Text_ThatsTooBad:
	.string "Non ? Dommage.$"

Route109_SeashoreHouse_Text_DwayneIntro:
	.string "Si tu cherches un combat dans cette\n"
	.string "Maison, tu trouveras pas plus bouillant\l"
	.string "que moi, moussaillon !$"

Route109_SeashoreHouse_Text_DwayneDefeated:
	.string "Ça, c'était un combat brûlant !\n"
	.string "J'accepte la défaite, moussaillon !$"

Route109_SeashoreHouse_Text_DwaynePostBattle:
	.string "Quand je suis à Poivressel, je savoure\n"
	.string "des combats brûlants et du Soda Cool\l"
	.string "glacé !$"

Route109_SeashoreHouse_Text_JohannaIntro:
	.string "Les combats mous, ça vaut pas la peine.\p"
	.string "Ce sont les combats de feu qui forgent\n"
	.string "les Dresseurs et les Pokémon !$"

Route109_SeashoreHouse_Text_JohannaDefeated:
	.string "Ça, c'est chaud !$"

Route109_SeashoreHouse_Text_JohannaPostBattle:
	.string "Pfiou, j'ai soif. Je vais peut-être\n"
	.string "prendre un Soda Cool.$"

Route109_SeashoreHouse_Text_SimonIntro:
	.string "Je vais te montrer à quel point mes\n"
	.string "Pokémon sont géniaux, mais pleure pas !$"

Route109_SeashoreHouse_Text_SimonDefeated:
	.string "…J'ai perdu, mais je pleure pas… Je\n"
	.string "pleure pas…$"

Route109_SeashoreHouse_Text_SimonPostBattle:
	.string "Si un de mes Pokémon savait porter les\n"
	.string "gens sur l'eau, je pourrais jeter cette\l"
	.string "bouée.$"

