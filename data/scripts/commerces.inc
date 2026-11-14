@ Émeraude Couronnement : commerces de la société (v0.36). Appli « Mes commerces » du menu L (src/commerces.c).
@ Gérant concurrent : combat contre un dresseur existant selon les badges (Richard / Gentleman) ; son drapeau de dresseur est
@ remis dans son état d'avant (VAR_COMMERCE_REMETTRE).
Commerce_EventScript_Appli::
	special Commerce_Ouvrir
	goto_if_eq VAR_RESULT, FALSE, Commerce_EventScript_Accueil
	playse SE_SHOP
	msgbox gStringVar4, MSGBOX_DEFAULT
Commerce_EventScript_Accueil::
	special Commerce_Resume
	goto_if_eq VAR_RESULT, 0, Commerce_EventScript_SansSociete
	message gStringVar4
	waitmessage
	dynmultichoice 0, 0, FALSE, 3, 0, DYN_MULTICHOICE_CB_NONE, Commerce_Text_Boutique, Commerce_Text_Centre, Commerce_Text_Fermer
	switch VAR_RESULT
	case 0, Commerce_EventScript_Boutique
	case 1, Commerce_EventScript_Centre
	return
Commerce_EventScript_SansSociete::
	msgbox Commerce_Text_SansSociete, MSGBOX_DEFAULT
	return

@ ---------------------------------------------------------------- boutique
Commerce_EventScript_Boutique::
	specialvar VAR_RESULT, Commerce_EtatBoutique
	goto_if_ne VAR_RESULT, 0, Commerce_EventScript_BoutiqueMenu
	special Commerce_PreparerVilles
	message Commerce_Text_QuelleVille
	waitmessage
	dynmultichoice 0, 0, FALSE, 6, 0, DYN_MULTICHOICE_CB_NONE, gImmoLignes + 0, gImmoLignes + 32, gImmoLignes + 64, gImmoLignes + 96, gImmoLignes + 128, gImmoLignes + 160, gImmoLignes + 192, gImmoLignes + 224, gImmoLignes + 256, gImmoLignes + 288, gImmoLignes + 320, gImmoLignes + 352, Commerce_Text_Retour
	goto_if_eq VAR_RESULT, MULTI_B_PRESSED, Commerce_EventScript_Accueil
	goto_if_eq VAR_RESULT, 12, Commerce_EventScript_Accueil
	copyvar VAR_0x8004, VAR_RESULT
	message Commerce_Text_QuelStock
	waitmessage
	call Commerce_EventScript_ChoixStock
	goto_if_eq VAR_RESULT, 4, Commerce_EventScript_Accueil
	special Commerce_ScriptOuvrirBoutique
	goto Commerce_EventScript_Resultat

Commerce_EventScript_ChoixStock::
	dynmultichoice 0, 0, FALSE, 5, 0, DYN_MULTICHOICE_CB_NONE, Commerce_Text_Baies, Commerce_Text_Soins, Commerce_Text_Balls, Commerce_Text_Rares, Commerce_Text_Retour
	goto_if_eq VAR_RESULT, MULTI_B_PRESSED, Commerce_EventScript_ChoixStockAnnule
	copyvar VAR_0x8005, VAR_RESULT
	return
Commerce_EventScript_ChoixStockAnnule::
	setvar VAR_RESULT, 4
	return

Commerce_EventScript_BoutiqueMenu::
	special Commerce_Resume
	message gStringVar4
	waitmessage
	specialvar VAR_RESULT, Commerce_EtatBoutique
	goto_if_eq VAR_RESULT, 2, Commerce_EventScript_BoutiqueMenuConcurrent
	dynmultichoice 0, 0, FALSE, 4, 0, DYN_MULTICHOICE_CB_NONE, Commerce_Text_Stock, Commerce_Text_Prix, Commerce_Text_Agrandir, Commerce_Text_Retour
	goto Commerce_EventScript_BoutiqueChoix
Commerce_EventScript_BoutiqueMenuConcurrent::
	dynmultichoice 0, 0, FALSE, 5, 0, DYN_MULTICHOICE_CB_NONE, Commerce_Text_Stock, Commerce_Text_Prix, Commerce_Text_Agrandir, Commerce_Text_Retour, Commerce_Text_Defier
Commerce_EventScript_BoutiqueChoix::
	switch VAR_RESULT
	case 0, Commerce_EventScript_Stock
	case 1, Commerce_EventScript_Prix
	case 2, Commerce_EventScript_AgrandirBoutique
	case 4, Commerce_EventScript_Defier
	goto Commerce_EventScript_Accueil

Commerce_EventScript_Stock::
	message Commerce_Text_QuelStock
	waitmessage
	call Commerce_EventScript_ChoixStock
	goto_if_eq VAR_RESULT, 4, Commerce_EventScript_BoutiqueMenu
	special Commerce_ScriptStock
	goto Commerce_EventScript_ResultatBoutique

Commerce_EventScript_Prix::
	message Commerce_Text_QuelsPrix
	waitmessage
	dynmultichoice 0, 0, FALSE, 4, 1, DYN_MULTICHOICE_CB_NONE, Commerce_Text_PrixBas, Commerce_Text_PrixNormaux, Commerce_Text_PrixEleves, Commerce_Text_Retour
	goto_if_eq VAR_RESULT, MULTI_B_PRESSED, Commerce_EventScript_BoutiqueMenu
	goto_if_eq VAR_RESULT, 3, Commerce_EventScript_BoutiqueMenu
	copyvar VAR_0x8005, VAR_RESULT
	special Commerce_ScriptPrix
	goto Commerce_EventScript_ResultatBoutique

Commerce_EventScript_AgrandirBoutique::
	setvar VAR_0x8005, 0
	special Commerce_TexteAgrandir
	goto_if_eq VAR_RESULT, FALSE, Commerce_EventScript_AgrandirMax
	call Commerce_EventScript_OuiNon
	goto_if_ne VAR_RESULT, 0, Commerce_EventScript_BoutiqueMenu
	special Commerce_ScriptAgrandirBoutique
	goto Commerce_EventScript_ResultatBoutique
Commerce_EventScript_AgrandirMax::
	msgbox gStringVar4, MSGBOX_DEFAULT
	goto Commerce_EventScript_Accueil

Commerce_EventScript_Defier::
	msgbox Commerce_Text_Concurrent, MSGBOX_DEFAULT
	closemessage
	specialvar VAR_RESULT, Commerce_NiveauConcurrent
	switch VAR_RESULT
	case 1, Commerce_EventScript_Duel1
	case 2, Commerce_EventScript_Duel2
	case 3, Commerce_EventScript_Duel3
Commerce_EventScript_Duel0::
	setvar VAR_COMMERCE_REMETTRE, 0
	goto_if_defeated TRAINER_WINSTON_1, Commerce_EventScript_Duel0Combat
	setvar VAR_COMMERCE_REMETTRE, 1
Commerce_EventScript_Duel0Combat::
	cleartrainerflag TRAINER_WINSTON_1
	trainerbattle_no_intro TRAINER_WINSTON_1, Commerce_Text_ConcurrentBattu
	goto_if_eq VAR_COMMERCE_REMETTRE, 0, Commerce_EventScript_DuelGagne
	cleartrainerflag TRAINER_WINSTON_1
	goto Commerce_EventScript_DuelGagne

Commerce_EventScript_Duel1::
	setvar VAR_COMMERCE_REMETTRE, 0
	goto_if_defeated TRAINER_WINSTON_2, Commerce_EventScript_Duel1Combat
	setvar VAR_COMMERCE_REMETTRE, 1
Commerce_EventScript_Duel1Combat::
	cleartrainerflag TRAINER_WINSTON_2
	trainerbattle_no_intro TRAINER_WINSTON_2, Commerce_Text_ConcurrentBattu
	goto_if_eq VAR_COMMERCE_REMETTRE, 0, Commerce_EventScript_DuelGagne
	cleartrainerflag TRAINER_WINSTON_2
	goto Commerce_EventScript_DuelGagne

Commerce_EventScript_Duel2::
	setvar VAR_COMMERCE_REMETTRE, 0
	goto_if_defeated TRAINER_WALTER_3, Commerce_EventScript_Duel2Combat
	setvar VAR_COMMERCE_REMETTRE, 1
Commerce_EventScript_Duel2Combat::
	cleartrainerflag TRAINER_WALTER_3
	trainerbattle_no_intro TRAINER_WALTER_3, Commerce_Text_ConcurrentBattu
	goto_if_eq VAR_COMMERCE_REMETTRE, 0, Commerce_EventScript_DuelGagne
	cleartrainerflag TRAINER_WALTER_3
	goto Commerce_EventScript_DuelGagne

Commerce_EventScript_Duel3::
	setvar VAR_COMMERCE_REMETTRE, 0
	goto_if_defeated TRAINER_THOMAS, Commerce_EventScript_Duel3Combat
	setvar VAR_COMMERCE_REMETTRE, 1
Commerce_EventScript_Duel3Combat::
	cleartrainerflag TRAINER_THOMAS
	trainerbattle_no_intro TRAINER_THOMAS, Commerce_Text_ConcurrentBattu
	goto_if_eq VAR_COMMERCE_REMETTRE, 0, Commerce_EventScript_DuelGagne
	cleartrainerflag TRAINER_THOMAS
	goto Commerce_EventScript_DuelGagne

Commerce_EventScript_DuelGagne::
	special Commerce_ConcurrentBattu
	msgbox Commerce_Text_ConcurrentParti, MSGBOX_DEFAULT
	goto Commerce_EventScript_BoutiqueMenu

Commerce_EventScript_ResultatBoutique::
	call Commerce_EventScript_Son
	msgbox gStringVar4, MSGBOX_DEFAULT
	goto Commerce_EventScript_BoutiqueMenu

@ ---------------------------------------------------------------- centre d'entraînement
Commerce_EventScript_Centre::
	specialvar VAR_RESULT, Commerce_EtatCentre
	goto_if_ne VAR_RESULT, 0, Commerce_EventScript_CentreMenu
	message Commerce_Text_OuvrirCentre
	waitmessage
	call Commerce_EventScript_OuiNonSansMessage
	goto_if_ne VAR_RESULT, 0, Commerce_EventScript_Accueil
	special Commerce_ScriptOuvrirCentre
	goto Commerce_EventScript_Resultat

Commerce_EventScript_CentreMenu::
	special Commerce_Resume
	message gStringVar4
	waitmessage
	dynmultichoice 0, 0, FALSE, 4, 0, DYN_MULTICHOICE_CB_NONE, Commerce_Text_SeanceXP, Commerce_Text_SeanceEV, Commerce_Text_Agrandir, Commerce_Text_Retour
	switch VAR_RESULT
	case 0, Commerce_EventScript_SeanceXP
	case 1, Commerce_EventScript_SeanceEV
	case 2, Commerce_EventScript_AgrandirCentre
	goto Commerce_EventScript_Accueil

Commerce_EventScript_SeanceXP::
	special ChoosePartyMon
	waitstate
	goto_if_eq VAR_0x8004, PARTY_NOTHING_CHOSEN, Commerce_EventScript_CentreMenu
	special Commerce_ScriptSeanceXP
	goto Commerce_EventScript_ResultatCentre

Commerce_EventScript_SeanceEV::
	special ChoosePartyMon
	waitstate
	goto_if_eq VAR_0x8004, PARTY_NOTHING_CHOSEN, Commerce_EventScript_CentreMenu
	message Commerce_Text_QuelleStat
	waitmessage
	dynmultichoice 0, 0, FALSE, 7, 0, DYN_MULTICHOICE_CB_NONE, CoachEV_Text_PV, CoachEV_Text_Attaque, CoachEV_Text_Defense, CoachEV_Text_AtqSpe, CoachEV_Text_DefSpe, CoachEV_Text_Vitesse, Commerce_Text_Retour
	goto_if_eq VAR_RESULT, MULTI_B_PRESSED, Commerce_EventScript_CentreMenu
	goto_if_eq VAR_RESULT, 6, Commerce_EventScript_CentreMenu
	copyvar VAR_0x8005, VAR_RESULT
	special Commerce_ScriptSeanceEV
	goto Commerce_EventScript_ResultatCentre

Commerce_EventScript_AgrandirCentre::
	setvar VAR_0x8005, 1
	special Commerce_TexteAgrandir
	goto_if_eq VAR_RESULT, FALSE, Commerce_EventScript_AgrandirMax
	call Commerce_EventScript_OuiNon
	goto_if_ne VAR_RESULT, 0, Commerce_EventScript_CentreMenu
	special Commerce_ScriptAgrandirCentre
Commerce_EventScript_ResultatCentre::
	call Commerce_EventScript_Son
	msgbox gStringVar4, MSGBOX_DEFAULT
	goto Commerce_EventScript_CentreMenu

@ ---------------------------------------------------------------- outils
Commerce_EventScript_Resultat::
	call Commerce_EventScript_Son
	msgbox gStringVar4, MSGBOX_DEFAULT
	goto Commerce_EventScript_Accueil

Commerce_EventScript_Son::
	goto_if_eq VAR_RESULT, FALSE, Commerce_EventScript_SonRate
	playse SE_SHOP
	return
Commerce_EventScript_SonRate::
	playse SE_BOO
	return

@ Affiche gStringVar4 puis Oui / Non (VAR_RESULT : 0 = oui).
Commerce_EventScript_OuiNon::
	message gStringVar4
	waitmessage
Commerce_EventScript_OuiNonSansMessage::
	dynmultichoice 0, 0, FALSE, 2, 1, DYN_MULTICHOICE_CB_NONE, Commerce_Text_Oui, Commerce_Text_Non
	return

Commerce_Text_Boutique:
	.string "Boutique$"
Commerce_Text_Centre:
	.string "Centre d'entraînement$"
Commerce_Text_Fermer:
	.string "Fermer$"
Commerce_Text_Retour:
	.string "Retour$"
Commerce_Text_Oui:
	.string "Oui$"
Commerce_Text_Non:
	.string "Non$"
Commerce_Text_Baies:
	.string "Baies$"
Commerce_Text_Soins:
	.string "Soins$"
Commerce_Text_Balls:
	.string "Poké Balls$"
Commerce_Text_Rares:
	.string "Objets rares$"
Commerce_Text_Stock:
	.string "Changer le stock$"
Commerce_Text_Prix:
	.string "Changer les prix$"
Commerce_Text_Agrandir:
	.string "Agrandir$"
Commerce_Text_Defier:
	.string "Défier le concurrent$"
Commerce_Text_PrixBas:
	.string "Prix bas$"
Commerce_Text_PrixNormaux:
	.string "Prix normaux$"
Commerce_Text_PrixEleves:
	.string "Prix élevés$"
Commerce_Text_SeanceXP:
	.string "Séance XP (100 pts)$"
Commerce_Text_SeanceEV:
	.string "Séance EV (40 pts)$"

Commerce_Text_SansSociete:
	.string "Les commerces passent par ta\n"
	.string "société. Crée-la d'abord dans\l"
	.string "“Ma société”.$"

Commerce_Text_QuelleVille:
	.string "Dans quelle ville ? Prix du local.$"

Commerce_Text_QuelStock:
	.string "Tu vends quoi ? Plus c'est cher,\n"
	.string "plus la recette joue au yo-yo.$"

Commerce_Text_QuelsPrix:
	.string "Bas : moins de marge, chasse les\n"
	.string "concurrents. Élevés : plus de marge,\l"
	.string "mais ça attire la concurrence.$"

Commerce_Text_OuvrirCentre:
	.string "Ouvrir un centre d'entraînement pour\n"
	.string "80 000 ¥ ? Les dresseurs paient, et\l"
	.string "tu gagnes des points pour ton équipe.$"

Commerce_Text_QuelleStat:
	.string "Quelle stat on travaille ?$"

Commerce_Text_Concurrent:
	.string "Gérant concurrent : Ma boutique est\n"
	.string "plus belle, mes prix plus bas. Tu\l"
	.string "veux régler ça ? Allez, combat !$"

Commerce_Text_ConcurrentBattu:
	.string "D'accord, d'accord… Je baisse le\n"
	.string "rideau. Tu as gagné la rue.$"

Commerce_Text_ConcurrentParti:
	.string "Le concurrent a plié boutique. Les\n"
	.string "clients reviennent chez toi.$"
