@ Émeraude Couronnement : société du joueur (v0.35). Appli « Ma société » du menu L (avec le pass), aussi ouverte depuis sa ligne
@ dans la Bourse une fois cotée (src/societe.c).
Societe_EventScript_Appli::
	special Societe_Resume
	goto_if_ne VAR_RESULT, 0, Societe_EventScript_Accueil
	msgbox Societe_Text_Presentation, MSGBOX_DEFAULT
	message Societe_Text_OnCree
	waitmessage
	dynmultichoice 0, 0, FALSE, 2, 0, DYN_MULTICHOICE_CB_NONE, Societe_Text_Creer, Societe_Text_PasMaintenant
	goto_if_ne VAR_RESULT, 0, Societe_EventScript_Fin
	checkmoney 20000
	goto_if_eq VAR_RESULT, FALSE, Societe_EventScript_PasLesMoyens
	msgbox Societe_Text_ChoisisNom, MSGBOX_DEFAULT
	closemessage
	special Societe_SaisirNom
	waitstate
	special Societe_ScriptCreer
	goto_if_eq VAR_RESULT, FALSE, Societe_EventScript_PasLesMoyens
	playfanfare MUS_OBTAIN_ITEM
	msgbox Societe_Text_Creee, MSGBOX_DEFAULT
	waitfanfare
	special Societe_Resume
Societe_EventScript_Accueil::
	message gStringVar4
	waitmessage
	goto_if_eq VAR_RESULT, 2, Societe_EventScript_MenuCotee
	dynmultichoice 0, 0, FALSE, 3, 0, DYN_MULTICHOICE_CB_NONE, Societe_Text_Retirer, Societe_Text_Introduire, Societe_Text_Fermer
	switch VAR_RESULT
	case 0, Societe_EventScript_Retirer
	case 1, Societe_EventScript_Introduire
	goto Societe_EventScript_Fin
Societe_EventScript_MenuCotee::
	dynmultichoice 0, 0, FALSE, 4, 0, DYN_MULTICHOICE_CB_NONE, Societe_Text_Retirer, Societe_Text_Vendre, Societe_Text_Racheter, Societe_Text_Fermer
	switch VAR_RESULT
	case 0, Societe_EventScript_Retirer
	case 1, Societe_EventScript_Vendre
	case 2, Societe_EventScript_Racheter
	goto Societe_EventScript_Fin

Societe_EventScript_Retirer::
	special Societe_ScriptRetirer
	goto Societe_EventScript_Resultat
Societe_EventScript_Introduire::
	message Societe_Text_SurIntro
	waitmessage
	dynmultichoice 0, 0, FALSE, 2, 1, DYN_MULTICHOICE_CB_NONE, Societe_Text_Oui, Societe_Text_Non
	goto_if_ne VAR_RESULT, 0, Societe_EventScript_Retour
	special Societe_ScriptIntroduire
	goto Societe_EventScript_Resultat
Societe_EventScript_Vendre::
	special Societe_ScriptVendre
	goto Societe_EventScript_Resultat
Societe_EventScript_Racheter::
	special Societe_ScriptRacheter
Societe_EventScript_Resultat::
	goto_if_eq VAR_RESULT, FALSE, Societe_EventScript_Rate
	playse SE_SHOP
	msgbox gStringVar4, MSGBOX_DEFAULT
	goto Societe_EventScript_Retour
Societe_EventScript_Rate::
	playse SE_BOO
	msgbox gStringVar4, MSGBOX_DEFAULT
Societe_EventScript_Retour::
	special Societe_Resume
	goto Societe_EventScript_Accueil

Societe_EventScript_PasLesMoyens::
	msgbox Societe_Text_PasLesMoyens, MSGBOX_DEFAULT
Societe_EventScript_Fin::
	return

Societe_Text_Creer:
	.string "Créer (20 000 ¥)$"
Societe_Text_PasMaintenant:
	.string "Pas maintenant$"
Societe_Text_Retirer:
	.string "Retirer la trésorerie$"
Societe_Text_Introduire:
	.string "Entrer en Bourse$"
Societe_Text_Vendre:
	.string "Vendre 500 actions$"
Societe_Text_Racheter:
	.string "Racheter 500 actions$"
Societe_Text_Fermer:
	.string "Fermer$"
Societe_Text_Oui:
	.string "Oui$"
Societe_Text_Non:
	.string "Non$"

Societe_Text_Presentation:
	.string "Monter ta propre boîte ? Ta société\n"
	.string "encaisse les loyers de tes biens\l"
	.string "dans sa trésorerie.\p"
	.string "Quand elle vaut 100 000 ¥, tu peux\n"
	.string "l'introduire en Bourse et vendre un\l"
	.string "quart des actions au public.\p"
	.string "Tu gardes toujours 51 % : personne\n"
	.string "ne te la piquera.$"

Societe_Text_OnCree:
	.string "On crée ta société ?$"

Societe_Text_ChoisisNom:
	.string "Choisis son nom. Laisse vide pour\n"
	.string "“Hoenn Corp”.$"

Societe_Text_Creee:
	.string "{STR_VAR_1} est née ! Les loyers vont\n"
	.string "maintenant dans sa trésorerie.$"

Societe_Text_PasLesMoyens:
	.string "Il faut 20 000 ¥ pour les statuts et\n"
	.string "le capital. Reviens plus riche.$"

Societe_Text_SurIntro:
	.string "Vendre 25 % des actions au public ?\n"
	.string "Le cours bougera tous les jours.$"
