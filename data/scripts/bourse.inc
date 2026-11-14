@ Émeraude Couronnement : Bourse de Hoenn (v0.33), appli du menu L dès que le rival a remis le pass (src/bourse.c).
Bourse_EventScript_Appli::
	special Bourse_Ouvrir
	goto_if_eq VAR_RESULT, FALSE, Bourse_EventScript_Accueil
	playse SE_FAILURE
	msgbox gStringVar4, MSGBOX_DEFAULT
Bourse_EventScript_Accueil::
	special Bourse_Resume
	message gStringVar4
	waitmessage
	dynmultichoice 0, 0, FALSE, 5, 0, DYN_MULTICHOICE_CB_NONE, Bourse_Text_Marche, Bourse_Text_Tuyau, Bourse_Text_Credit, Bourse_Text_Fermer
	switch VAR_RESULT
	case 0, Bourse_EventScript_Marche
	case 1, Bourse_EventScript_Tuyau
	case 2, Bourse_EventScript_Credit
	return

Bourse_EventScript_Marche::
	special Bourse_PreparerListe
	message Bourse_Text_QuelleEntreprise
	waitmessage
	dynmultipush gBourseLignes + 0, 0
	dynmultipush gBourseLignes + 32, 1
	dynmultipush gBourseLignes + 64, 2
	dynmultipush gBourseLignes + 96, 3
	dynmultipush gBourseLignes + 128, 4
	dynmultipush gBourseLignes + 160, 5
	dynmultipush gBourseLignes + 192, 6
	dynmultipush gBourseLignes + 224, 7
	dynmultipush gBourseLignes + 256, 8
	special Societe_LigneBourse
	goto_if_eq VAR_RESULT, FALSE, Bourse_EventScript_MarcheRetour
	dynmultipush gBourseLignes + 288, 9
Bourse_EventScript_MarcheRetour::
	dynmultipush Bourse_Text_Retour, 10
	dynmultistack 0, 0, FALSE, 6, FALSE, 0, DYN_MULTICHOICE_CB_NONE
	goto_if_eq VAR_RESULT, MULTI_B_PRESSED, Bourse_EventScript_Accueil
	goto_if_eq VAR_RESULT, 10, Bourse_EventScript_Accueil
	goto_if_eq VAR_RESULT, 9, Bourse_EventScript_MaSociete
	copyvar VAR_0x8004, VAR_RESULT
Bourse_EventScript_Fiche::
	special Bourse_Fiche
	message gStringVar4
	waitmessage
	dynmultichoice 0, 0, FALSE, 3, 0, DYN_MULTICHOICE_CB_NONE, Bourse_Text_Acheter, Bourse_Text_Vendre, Bourse_Text_Retour
	switch VAR_RESULT
	case 0, Bourse_EventScript_Acheter
	case 1, Bourse_EventScript_Vendre
	goto Bourse_EventScript_Marche

Bourse_EventScript_MaSociete::
	call Societe_EventScript_Appli
	goto Bourse_EventScript_Marche

Bourse_EventScript_Acheter::
	message Bourse_Text_CombienAcheter
	waitmessage
	dynmultichoice 0, 0, FALSE, 5, 0, DYN_MULTICHOICE_CB_NONE, Bourse_Text_1, Bourse_Text_10, Bourse_Text_100, Bourse_Text_Max, Bourse_Text_Retour
	goto_if_eq VAR_RESULT, MULTI_B_PRESSED, Bourse_EventScript_Fiche
	goto_if_eq VAR_RESULT, 4, Bourse_EventScript_Fiche
	copyvar VAR_0x8005, VAR_RESULT
	special Bourse_ScriptAcheter
	call Bourse_EventScript_SonOrdre
	msgbox gStringVar4, MSGBOX_DEFAULT
	goto Bourse_EventScript_Fiche

Bourse_EventScript_Vendre::
	message Bourse_Text_CombienVendre
	waitmessage
	dynmultichoice 0, 0, FALSE, 5, 0, DYN_MULTICHOICE_CB_NONE, Bourse_Text_1, Bourse_Text_10, Bourse_Text_100, Bourse_Text_Tout, Bourse_Text_Retour
	goto_if_eq VAR_RESULT, MULTI_B_PRESSED, Bourse_EventScript_Fiche
	goto_if_eq VAR_RESULT, 4, Bourse_EventScript_Fiche
	copyvar VAR_0x8005, VAR_RESULT
	special Bourse_ScriptVendre
	call Bourse_EventScript_SonOrdre
	msgbox gStringVar4, MSGBOX_DEFAULT
	goto Bourse_EventScript_Fiche

Bourse_EventScript_SonOrdre::
	goto_if_eq VAR_RESULT, FALSE, Bourse_EventScript_SonRate
	playse SE_SHOP
	return
Bourse_EventScript_SonRate::
	playse SE_BOO
	return

Bourse_EventScript_Tuyau::
	special Bourse_Tuyau
	msgbox gStringVar4, MSGBOX_DEFAULT
	goto Bourse_EventScript_Accueil

Bourse_EventScript_Credit::
	special Bourse_Credit
	message gStringVar4
	waitmessage
	dynmultichoice 0, 0, FALSE, 5, 0, DYN_MULTICHOICE_CB_NONE, Bourse_Text_Emprunter5000, Bourse_Text_Emprunter20000, Bourse_Text_Emprunter50000, Bourse_Text_Rembourser, Bourse_Text_Retour
	goto_if_eq VAR_RESULT, MULTI_B_PRESSED, Bourse_EventScript_Accueil
	goto_if_eq VAR_RESULT, 4, Bourse_EventScript_Accueil
	goto_if_eq VAR_RESULT, 3, Bourse_EventScript_Rembourser
	copyvar VAR_0x8005, VAR_RESULT
	special Bourse_ScriptEmprunter
	playse SE_SHOP
	msgbox gStringVar4, MSGBOX_DEFAULT
	goto Bourse_EventScript_Credit
Bourse_EventScript_Rembourser::
	special Bourse_ScriptRembourser
	playse SE_SHOP
	msgbox gStringVar4, MSGBOX_DEFAULT
	goto Bourse_EventScript_Credit

Bourse_Text_Marche:
	.string "Le marché$"
Bourse_Text_Tuyau:
	.string "Tuyau du jour$"
Bourse_Text_Credit:
	.string "Crédit$"
Bourse_Text_Fermer:
	.string "Fermer$"
Bourse_Text_Retour:
	.string "Retour$"
Bourse_Text_Acheter:
	.string "Acheter$"
Bourse_Text_Vendre:
	.string "Vendre$"
Bourse_Text_1:
	.string "1 part$"
Bourse_Text_10:
	.string "10 parts$"
Bourse_Text_100:
	.string "100 parts$"
Bourse_Text_Max:
	.string "Le maximum$"
Bourse_Text_Tout:
	.string "Tout$"
Bourse_Text_Emprunter5000:
	.string "Emprunter 5 000 ¥$"
Bourse_Text_Emprunter20000:
	.string "Emprunter 20 000 ¥$"
Bourse_Text_Emprunter50000:
	.string "Emprunter 50 000 ¥$"
Bourse_Text_Rembourser:
	.string "Rembourser$"

Bourse_Text_QuelleEntreprise:
	.string "Cours du jour. Laquelle ?$"

Bourse_Text_CombienAcheter:
	.string "Combien de parts ? Frais : 3 %.$"

Bourse_Text_CombienVendre:
	.string "Combien de parts tu lâches ?$"
