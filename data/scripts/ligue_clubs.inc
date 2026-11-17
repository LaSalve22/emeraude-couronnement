@ Émeraude Couronnement : Ligue des Clubs (v0.42). Appli « Ligue des Clubs » du menu L (avec le pass), src/league_club.c,
@ tableau de bord src/club_dashboard.c. Le duel du joueur utilise l'emplacement TRAINER_COURONNEMENT.
Club_EventScript_Appli::
	special Club_Ouvrir
	goto_if_eq VAR_RESULT, FALSE, Club_EventScript_Accueil
	playse SE_PC_LOGIN
	msgbox gStringVar4, MSGBOX_DEFAULT
Club_EventScript_Accueil::
	special Club_Resume
	message gStringVar4
	waitmessage
	dynmultichoice 0, 0, FALSE, 5, 0, DYN_MULTICHOICE_CB_NONE, Club_Text_Tableau, Club_Text_Jouer, Club_Text_Licence, Club_Text_Regles, Club_Text_Fermer
	switch VAR_RESULT
	case 0, Club_EventScript_Tableau
	case 1, Club_EventScript_Jouer
	case 2, Club_EventScript_Licence
	case 3, Club_EventScript_Regles
Club_EventScript_Fin::
	closemessage
	return

Club_EventScript_Tableau::
	closemessage
	special Club_TableauDeBord
	waitstate
	goto Club_EventScript_Accueil

Club_EventScript_Regles::
	msgbox Club_Text_ReglesTexte, MSGBOX_DEFAULT
	goto Club_EventScript_Accueil

Club_EventScript_Jouer::
	special Club_PreparerMatch
	goto_if_eq VAR_RESULT, FALSE, Club_EventScript_Info
	msgbox gStringVar4, MSGBOX_DEFAULT
	msgbox Club_Text_OnYVa, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, Club_EventScript_Accueil
	closemessage
	special Club_DebutMatch
	trainerbattle_no_intro TRAINER_COURONNEMENT, Club_Text_AdversaireBattu
	cleartrainerflag TRAINER_COURONNEMENT
	special Club_FinMatch
	goto_if_eq VAR_RESULT, FALSE, Club_EventScript_Defaite
	playfanfare MUS_OBTAIN_ITEM
	msgbox gStringVar4, MSGBOX_DEFAULT
	waitfanfare
	goto Club_EventScript_Accueil

Club_EventScript_Defaite::
	playse SE_BOO
	msgbox gStringVar4, MSGBOX_DEFAULT
	goto Club_EventScript_Accueil

Club_EventScript_Info::
	msgbox gStringVar4, MSGBOX_DEFAULT
	goto Club_EventScript_Accueil

Club_EventScript_Licence::
	message Club_Text_QuelClub
	waitmessage
	dynmultichoice 0, 0, FALSE, 6, 0, DYN_MULTICHOICE_CB_NONE, Club_Text_Club0, Club_Text_Club1, Club_Text_Club2, Club_Text_Club3, Club_Text_Club4, Club_Text_Club5, Club_Text_Club6, Club_Text_Club7, Club_Text_Club8, Club_Text_Club9, Club_Text_Retour
	goto_if_eq VAR_RESULT, MULTI_B_PRESSED, Club_EventScript_Accueil
	goto_if_eq VAR_RESULT, 10, Club_EventScript_Accueil
	copyvar VAR_0x8004, VAR_RESULT
	special Club_FicheLicence
	goto_if_eq VAR_RESULT, FALSE, Club_EventScript_Info
	msgbox gStringVar4, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, Club_EventScript_Licence
	special Club_ScriptSigner
	goto_if_eq VAR_RESULT, FALSE, Club_EventScript_Info
	playfanfare MUS_OBTAIN_ITEM
	msgbox gStringVar4, MSGBOX_DEFAULT
	waitfanfare
	goto Club_EventScript_Accueil

Club_Text_Tableau:
	.string "Tableau de bord$"
Club_Text_Jouer:
	.string "Jouer mon duel$"
Club_Text_Licence:
	.string "Licence et transferts$"
Club_Text_Regles:
	.string "Règlement$"
Club_Text_Fermer:
	.string "Fermer$"
Club_Text_Retour:
	.string "Retour$"
Club_Text_Club0:
	.string "AC Clémenti (C)$"
Club_Text_Club1:
	.string "AS Autéquia (C)$"
Club_Text_Club2:
	.string "SC Vermilava (B)$"
Club_Text_Club3:
	.string "Aigles Cimetronelle (B)$"
Club_Text_Club4:
	.string "Olympique Mérouville (B)$"
Club_Text_Club5:
	.string "Dynamo Lavandia (B)$"
Club_Text_Club6:
	.string "FC Poivressel (A)$"
Club_Text_Club7:
	.string "Cosmos Algatia (A)$"
Club_Text_Club8:
	.string "Stade Nénucrique (S)$"
Club_Text_Club9:
	.string "Royale Atalanopolis (S)$"

Club_Text_QuelClub:
	.string "Quel club ?$"

Club_Text_OnYVa:
	.string "Ton équipe sera soignée avant et\n"
	.string "après le duel. On y va ?$"

Club_Text_AdversaireBattu:
	.string "Bien joué. Le club retiendra\n"
	.string "ton nom.$"

Club_Text_ReglesTexte:
	.string "10 clubs, 2 poules de 5 tirées au\n"
	.string "sort à chaque saison de 30 jours.\p"
	.string "Mercato les jours 1 et 2. Dix\n"
	.string "journées en aller-retour, finale\l"
	.string "le jour 30 entre les deux premiers.\p"
	.string "Chaque rencontre : 4 duels en 6\n"
	.string "contre 6, sans objets du sac.\p"
	.string "Victoire 4 pts, nul 2, défaite 0.\n"
	.string "Gagner 6-0 : +1. Perdre 5-6 : +1.\p"
	.string "Toi, tu joues le duel des Recrues.\n"
	.string "Tu as jusqu'à la journée suivante.\p"
	.string "Sinon : forfait, défaite 0-6. Les\n"
	.string "autres duels se jouent sans toi.\p"
	.string "Le stade qui reçoit lance son\n"
	.string "effet au 1er tour, pour 5 tours.$"
