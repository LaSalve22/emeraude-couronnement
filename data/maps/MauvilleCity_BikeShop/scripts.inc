MauvilleCity_BikeShop_MapScripts::
	.byte 0

MauvilleCity_BikeShop_EventScript_Rydel::
	lock
	faceplayer
	goto_if_set FLAG_RECEIVED_BIKE, MauvilleCity_BikeShop_EventScript_AskSwitchBikes
	goto_if_set FLAG_DECLINED_BIKE, MauvilleCity_BikeShop_EventScript_SkipGreeting
	msgbox MauvilleCity_BikeShop_Text_RydelGreeting, MSGBOX_DEFAULT
	msgbox MauvilleCity_BikeShop_Text_DidYouComeFromFarAway, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, YES, MauvilleCity_BikeShop_EventScript_YesFar
	goto_if_eq VAR_RESULT, NO, MauvilleCity_BikeShop_EventScript_NotFar
	end

MauvilleCity_BikeShop_EventScript_SkipGreeting::
	msgbox MauvilleCity_BikeShop_Text_DidYouComeFromFarAway, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, YES, MauvilleCity_BikeShop_EventScript_YesFar
	goto_if_eq VAR_RESULT, NO, MauvilleCity_BikeShop_EventScript_NotFar
	end

MauvilleCity_BikeShop_EventScript_ChooseBike::
	message MauvilleCity_BikeShop_Text_ExplainBikesChooseWhichOne
	waitmessage
	multichoice 21, 8, MULTI_BIKE, TRUE
	switch VAR_RESULT
	case 0, MauvilleCity_BikeShop_EventScript_GetMachBike
	case 1, MauvilleCity_BikeShop_EventScript_GetAcroBike
	end

MauvilleCity_BikeShop_EventScript_NotFar::
	setflag FLAG_DECLINED_BIKE
	msgbox MauvilleCity_BikeShop_Text_GuessYouDontNeedBike, MSGBOX_DEFAULT
	release
	end

MauvilleCity_BikeShop_EventScript_YesFar::
	setflag FLAG_RECEIVED_BIKE
	goto MauvilleCity_BikeShop_EventScript_ChooseBike
	end

MauvilleCity_BikeShop_EventScript_GetMachBike::
	msgbox MauvilleCity_BikeShop_Text_ChoseMachBike, MSGBOX_DEFAULT
	giveitem ITEM_MACH_BIKE
	goto_if_eq VAR_RESULT, FALSE, Common_EventScript_ShowBagIsFull
	goto MauvilleCity_BikeShop_EventScript_ComeBackToSwitchBikes
	end

MauvilleCity_BikeShop_EventScript_GetAcroBike::
	msgbox MauvilleCity_BikeShop_Text_ChoseAcroBike, MSGBOX_DEFAULT
	giveitem ITEM_ACRO_BIKE
	goto_if_eq VAR_RESULT, FALSE, Common_EventScript_ShowBagIsFull
	goto MauvilleCity_BikeShop_EventScript_ComeBackToSwitchBikes
	end

MauvilleCity_BikeShop_EventScript_ComeBackToSwitchBikes::
	msgbox MauvilleCity_BikeShop_Text_ComeBackToSwitchBikes, MSGBOX_DEFAULT
	special SwapRegisteredBike
	release
	end

MauvilleCity_BikeShop_EventScript_AskSwitchBikes::
	msgbox MauvilleCity_BikeShop_Text_WantToSwitchBikes, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, YES, MauvilleCity_BikeShop_EventScript_SwitchBikes
	goto_if_eq VAR_RESULT, NO, MauvilleCity_BikeShop_EventScript_KeepBike
	end

@ If the player does not have a bike on them Rydel assumes its stored in the PC
MauvilleCity_BikeShop_EventScript_SwitchBikes::
	msgbox MauvilleCity_BikeShop_Text_IllSwitchBikes, MSGBOX_DEFAULT
	checkitem ITEM_ACRO_BIKE
	goto_if_eq VAR_RESULT, TRUE, MauvilleCity_BikeShop_EventScript_SwitchAcroForMach
	checkitem ITEM_MACH_BIKE
	goto_if_eq VAR_RESULT, TRUE, MauvilleCity_BikeShop_EventScript_SwitchMachForAcro
	msgbox MauvilleCity_BikeShop_Text_OhYourBikeIsInPC, MSGBOX_DEFAULT
	release
	end

MauvilleCity_BikeShop_EventScript_KeepBike::
	msgbox MauvilleCity_BikeShop_Text_HappyYouLikeIt, MSGBOX_DEFAULT
	release
	end

MauvilleCity_BikeShop_EventScript_SwitchAcroForMach::
	incrementgamestat GAME_STAT_TRADED_BIKES
	msgbox MauvilleCity_BikeShop_Text_ExchangedAcroForMach, MSGBOX_DEFAULT
	removeitem ITEM_ACRO_BIKE
	giveitem ITEM_MACH_BIKE
	goto MauvilleCity_BikeShop_EventScript_ComeBackToSwitchBikes
	end

MauvilleCity_BikeShop_EventScript_SwitchMachForAcro::
	incrementgamestat GAME_STAT_TRADED_BIKES
	msgbox MauvilleCity_BikeShop_Text_ExchangedMachForAcro, MSGBOX_DEFAULT
	removeitem ITEM_MACH_BIKE
	giveitem ITEM_ACRO_BIKE
	goto MauvilleCity_BikeShop_EventScript_ComeBackToSwitchBikes
	end

MauvilleCity_BikeShop_EventScript_Assistant::
	msgbox MauvilleCity_BikeShop_Text_HandbooksAreInBack, MSGBOX_NPC
	end

MauvilleCity_BikeShop_EventScript_MachBikeHandbook::
	message MauvilleCity_BikeShop_Text_MachHandbookWhichPage
	waitmessage
	goto MauvilleCity_BikeShop_EventScript_ChooseMachHandbookPage
	end

MauvilleCity_BikeShop_EventScript_ChooseMachHandbookPage::
	multichoice 0, 0, MULTI_MACH_BIKE_INFO, FALSE
	switch VAR_RESULT
	case 0, MauvilleCity_BikeShop_EventScript_HowToRide
	case 1, MauvilleCity_BikeShop_EventScript_HowToTurn
	case 2, MauvilleCity_BikeShop_EventScript_SandySlopes
	case 3, MauvilleCity_BikeShop_EventScript_ExitMachHandbook
	case MULTI_B_PRESSED, MauvilleCity_BikeShop_EventScript_ExitMachHandbook
	end

MauvilleCity_BikeShop_EventScript_HowToRide::
	message MauvilleCity_BikeShop_Text_HowToRideMachBike
	waitmessage
	goto MauvilleCity_BikeShop_EventScript_ChooseMachHandbookPage
	end

MauvilleCity_BikeShop_EventScript_HowToTurn::
	message MauvilleCity_BikeShop_Text_HowToTurnMachBike
	waitmessage
	goto MauvilleCity_BikeShop_EventScript_ChooseMachHandbookPage
	end

MauvilleCity_BikeShop_EventScript_SandySlopes::
	message MauvilleCity_BikeShop_Text_SandySlopes
	waitmessage
	goto MauvilleCity_BikeShop_EventScript_ChooseMachHandbookPage
	end

MauvilleCity_BikeShop_EventScript_ExitMachHandbook::
	release
	end

MauvilleCity_BikeShop_EventScript_AcroBikeHandbook::
	message MauvilleCity_BikeShop_Text_AcroHandbookWhichPage
	waitmessage
	goto MauvilleCity_BikeShop_EventScript_ChooseAcroHandbookPage
	end

MauvilleCity_BikeShop_EventScript_ChooseAcroHandbookPage::
	multichoice 0, 0, MULTI_ACRO_BIKE_INFO, FALSE
	switch VAR_RESULT
	case 0, MauvilleCity_BikeShop_EventScript_Wheelies
	case 1, MauvilleCity_BikeShop_EventScript_BunnyHops
	case 2, MauvilleCity_BikeShop_EventScript_Jumps
	case 3, MauvilleCity_BikeShop_EventScript_ExitAcroHandbook
	case MULTI_B_PRESSED, MauvilleCity_BikeShop_EventScript_ExitAcroHandbook
	end

MauvilleCity_BikeShop_EventScript_Wheelies::
	message MauvilleCity_BikeShop_Text_Wheelies
	waitmessage
	goto MauvilleCity_BikeShop_EventScript_ChooseAcroHandbookPage
	end

MauvilleCity_BikeShop_EventScript_BunnyHops::
	message MauvilleCity_BikeShop_Text_BunnyHops
	waitmessage
	goto MauvilleCity_BikeShop_EventScript_ChooseAcroHandbookPage
	end

MauvilleCity_BikeShop_EventScript_Jumps::
	message MauvilleCity_BikeShop_Text_Jumps
	waitmessage
	goto MauvilleCity_BikeShop_EventScript_ChooseAcroHandbookPage
	end

MauvilleCity_BikeShop_EventScript_ExitAcroHandbook::
	release
	end

MauvilleCity_BikeShop_Text_RydelGreeting:
	.string "Eh bien, eh bien, qu'avons-nous là ? Un\n"
	.string "client plein d'énergie !\p"
	.string "Moi ? Appelle-moi Rydel. Je suis le\n"
	.string "patron de cette boutique de vélos.$"

MauvilleCity_BikeShop_Text_DidYouComeFromFarAway:
	.string "Rydel : Tes Chaussures de sport… Elles\n"
	.string "sont dans un état lamentable.\p"
	.string "Tu viens de loin ?$"

MauvilleCity_BikeShop_Text_GuessYouDontNeedBike:
	.string "Rydel : Ah bon ?\p"
	.string "Alors tu n'as pas besoin de mes Vélos.\n"
	.string "Tant pis pour tes mollets.$"

MauvilleCity_BikeShop_Text_ExplainBikesChooseWhichOne:
	.string "Rydel : Hm, hm… … … … …\p"
	.string "T'as fait tout le chemin à pied depuis\n"
	.string "Bourg-en-Vol ?\p"
	.string "Mais c'est de la folie ! T'as des pieds en\n"
	.string "acier ?\p"
	.string "Avec un de mes Vélos, tu irais partout\n"
	.string "sans effort, la brise sur le visage !\p"
	.string "Tu sais quoi ? Je t'en offre un !\p"
	.string "Ah, attends une seconde !\p"
	.string "J'ai oublié de te dire qu'il existe deux\n"
	.string "sortes de Vélos !\p"
	.string "Le Vélo de Course et le Vélo Cross !\p"
	.string "Le Vélo de Course, c'est pour ceux qui\n"
	.string "veulent sentir le vent leur gifler le\l"
	.string "visage !\p"
	.string "Le Vélo Cross, c'est pour ceux qui\n"
	.string "préfèrent la technique et les figures !\p"
	.string "Je suis un vrai cœur d'artichaut, alors\n"
	.string "prends celui que tu veux !\p"
	.string "Lequel choisis-tu ?$"

MauvilleCity_BikeShop_Text_ChoseMachBike:
	.string "{PLAYER} choisit le Vélo de Course.$"

MauvilleCity_BikeShop_Text_ChoseAcroBike:
	.string "{PLAYER} choisit le Vélo Cross.$"

MauvilleCity_BikeShop_Text_ComeBackToSwitchBikes:
	.string "Rydel : Si tu as envie de changer de\n"
	.string "Vélo, reviens me voir !$"

MauvilleCity_BikeShop_Text_WantToSwitchBikes:
	.string "Rydel : Oh ? Tu veux changer de Vélo ?$"

MauvilleCity_BikeShop_Text_IllSwitchBikes:
	.string "Rydel : Pas de souci ! Je te l'échange !$"

MauvilleCity_BikeShop_Text_ExchangedMachForAcro:
	.string "{PLAYER} échange le Vélo de Course\n"
	.string "contre un Vélo Cross.$"

MauvilleCity_BikeShop_Text_ExchangedAcroForMach:
	.string "{PLAYER} échange le Vélo Cross contre\n"
	.string "un Vélo de Course.$"

MauvilleCity_BikeShop_Text_HappyYouLikeIt:
	.string "Rydel : Parfait, parfait ! Ravi qu'il te\n"
	.string "plaise !$"

MauvilleCity_BikeShop_Text_OhYourBikeIsInPC:
	.string "Oh ? Qu'est-ce que t'as fait du Vélo que\n"
	.string "je t'ai donné ?\p"
	.string "Ah, je vois, tu l'as rangé dans ton PC.\p"
	.string "Sors-le du PC, et je te l'échange avec\n"
	.string "plaisir !\p"
	.string "Que le vent souffle toujours dans ton\n"
	.string "dos !$"

MauvilleCity_BikeShop_Text_HandbooksAreInBack:
	.string "J'apprends le métier ici, en bossant.\p"
	.string "Si t'as besoin de conseils pour rouler, il\n"
	.string "y a des manuels au fond.$"

MauvilleCity_BikeShop_Text_MachHandbookWhichPage:
	.string "C'est un manuel du Vélo de Course.\p"
	.string "Quelle page veux-tu lire ?$"

MauvilleCity_BikeShop_Text_HowToRideMachBike:
	.string "Le Vélo avance dans la direction\n"
	.string "indiquée par la croix directionnelle.\p"
	.string "Il accélère une fois lancé.\p"
	.string "Pour t'arrêter, relâche la croix. Le Vélo\n"
	.string "ralentit jusqu'à l'arrêt.\p"
	.string "Lire une autre page ?$"

MauvilleCity_BikeShop_Text_HowToTurnMachBike:
	.string "Le Vélo de Course est rapide, mais il\n"
	.string "freine mal.\p"
	.string "Prendre un virage devient vite\n"
	.string "acrobatique.\p"
	.string "Relâche la croix un peu avant le virage\n"
	.string "pour ralentir. Ou embrasse le mur.\p"
	.string "Lire une autre page ?$"

MauvilleCity_BikeShop_Text_SandySlopes:
	.string "Il y a de petites pentes de sable un peu\n"
	.string "partout à Hoenn.\p"
	.string "Le sable meuble empêche de les gravir à\n"
	.string "pied.\p"
	.string "Mais avec un Vélo de Course, tu les\n"
	.string "grimpes à toute allure.\p"
	.string "Lire une autre page ?$"

MauvilleCity_BikeShop_Text_AcroHandbookWhichPage:
	.string "C'est un manuel du Vélo Cross.\p"
	.string "Quelle page veux-tu lire ?$"

MauvilleCity_BikeShop_Text_Wheelies:
	.string "Appuie sur B en roulant : la roue avant\n"
	.string "se soulève.\p"
	.string "Tu peux te déplacer roue levée avec la\n"
	.string "croix directionnelle.\p"
	.string "Cette technique s'appelle le wheeling.\p"
	.string "Lire une autre page ?$"

MauvilleCity_BikeShop_Text_BunnyHops:
	.string "En maintenant B, ton Vélo saute sur\n"
	.string "place.\p"
	.string "Cette technique s'appelle le bunny hop.\p"
	.string "Tu peux aussi avancer en sautant.\p"
	.string "Lire une autre page ?$"

MauvilleCity_BikeShop_Text_Jumps:
	.string "Appuie sur B et la croix en même temps\n"
	.string "pour sauter.\p"
	.string "Croix sur le côté : saut de côté.\p"
	.string "Croix vers l'arrière : le Vélo fait\n"
	.string "demi-tour en plein saut.\p"
	.string "Lire une autre page ?$"

