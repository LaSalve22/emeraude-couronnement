@ Émeraude Couronnement : banques (v0.37). Appli « Banques » du menu L (avec le pass), src/banques.c.
Banque_EventScript_Appli::
	special Banque_Ouvrir
	goto_if_eq VAR_RESULT, FALSE, Banque_EventScript_Accueil
	playse SE_FAILURE
	msgbox gStringVar4, MSGBOX_DEFAULT
Banque_EventScript_Accueil::
	special Banque_Resume
	message gStringVar4
	waitmessage
	dynmultichoice 0, 0, FALSE, 4, 0, DYN_MULTICHOICE_CB_NONE, Banque_Text_Devon, Banque_Text_Foncier, Banque_Text_Casino, Banque_Text_Fermer
	goto_if_eq VAR_RESULT, MULTI_B_PRESSED, Banque_EventScript_Fin
	goto_if_eq VAR_RESULT, 3, Banque_EventScript_Fin
	copyvar VAR_0x8004, VAR_RESULT
Banque_EventScript_Fiche::
	special Banque_Fiche
	message gStringVar4
	waitmessage
	dynmultichoice 0, 0, FALSE, 5, 0, DYN_MULTICHOICE_CB_NONE, Banque_Text_Emprunter10, Banque_Text_Emprunter100, Banque_Text_Emprunter1000, Banque_Text_Rembourser, Banque_Text_Retour
	goto_if_eq VAR_RESULT, MULTI_B_PRESSED, Banque_EventScript_Accueil
	goto_if_eq VAR_RESULT, 4, Banque_EventScript_Accueil
	goto_if_eq VAR_RESULT, 3, Banque_EventScript_Rembourser
	copyvar VAR_0x8005, VAR_RESULT
	special Banque_ScriptEmprunter
	goto Banque_EventScript_Resultat
Banque_EventScript_Rembourser::
	special Banque_ScriptRembourser
Banque_EventScript_Resultat::
	goto_if_eq VAR_RESULT, FALSE, Banque_EventScript_Rate
	playse SE_SHOP
	msgbox gStringVar4, MSGBOX_DEFAULT
	goto Banque_EventScript_Fiche
Banque_EventScript_Rate::
	playse SE_BOO
	msgbox gStringVar4, MSGBOX_DEFAULT
	goto Banque_EventScript_Fiche
Banque_EventScript_Fin::
	return

Banque_Text_Devon:
	.string "Banque Devon (0,3 %)$"
Banque_Text_Foncier:
	.string "Crédit Foncier (0,5 %)$"
Banque_Text_Casino:
	.string "Prêteur du Casino (2 %)$"
Banque_Text_Fermer:
	.string "Fermer$"
Banque_Text_Retour:
	.string "Retour$"
Banque_Text_Emprunter10:
	.string "Emprunter 10 000 ¥$"
Banque_Text_Emprunter100:
	.string "Emprunter 100 000 ¥$"
Banque_Text_Emprunter1000:
	.string "Emprunter 1 000 000 ¥$"
Banque_Text_Rembourser:
	.string "Rembourser$"
