@ Émeraude Couronnement : immobilier (v0.34). Agence dans le menu L (avec le pass) et panneaux « À vendre » devant une maison
@ de 12 villes (src/immobilier.c). Squatteur Aqua : combat contre un sbire existant selon les badges ; son drapeau de dresseur
@ est remis dans son état d'avant (VAR_IMMO_REMETTRE) pour ne pas perturber le scénario.
Immo_EventScript_Appli::
	special Immo_Ouvrir
	goto_if_eq VAR_RESULT, FALSE, Immo_EventScript_Accueil
	msgbox gStringVar4, MSGBOX_DEFAULT
Immo_EventScript_Accueil::
	special Immo_Resume
	message gStringVar4
	waitmessage
	dynmultichoice 0, 0, FALSE, 2, 0, DYN_MULTICHOICE_CB_NONE, Immo_Text_Catalogue, Immo_Text_Fermer
	goto_if_eq VAR_RESULT, 0, Immo_EventScript_Catalogue
	return
Immo_EventScript_Catalogue::
	special Immo_PreparerListe
	message Immo_Text_QuelBien
	waitmessage
	dynmultichoice 0, 0, FALSE, 6, 0, DYN_MULTICHOICE_CB_NONE, gImmoLignes + 0, gImmoLignes + 32, gImmoLignes + 64, gImmoLignes + 96, gImmoLignes + 128, gImmoLignes + 160, gImmoLignes + 192, gImmoLignes + 224, gImmoLignes + 256, gImmoLignes + 288, gImmoLignes + 320, gImmoLignes + 352, Immo_Text_Retour
	goto_if_eq VAR_RESULT, MULTI_B_PRESSED, Immo_EventScript_Accueil
	goto_if_eq VAR_RESULT, 12, Immo_EventScript_Accueil
	copyvar VAR_0x8004, VAR_RESULT
	call Immo_EventScript_Fiche
	goto Immo_EventScript_Catalogue

@ Fiche d'un bien (VAR_0x8004), appelée par l'agence et par les panneaux.
Immo_EventScript_Fiche::
	special Immo_Fiche
	message gStringVar4
	waitmessage
	switch VAR_RESULT
	case 1, Immo_EventScript_FicheAToi
	case 2, Immo_EventScript_FicheSquat
	dynmultichoice 0, 0, FALSE, 2, 0, DYN_MULTICHOICE_CB_NONE, Immo_Text_Acheter, Immo_Text_Retour
	goto_if_ne VAR_RESULT, 0, Immo_EventScript_FicheFin
	special Immo_ScriptAcheter
	goto Immo_EventScript_Resultat
Immo_EventScript_FicheAToi::
	dynmultichoice 0, 0, FALSE, 2, 0, DYN_MULTICHOICE_CB_NONE, Immo_Text_Vendre, Immo_Text_Retour
	goto_if_ne VAR_RESULT, 0, Immo_EventScript_FicheFin
	goto Immo_EventScript_Vendre
Immo_EventScript_FicheSquat::
	dynmultichoice 0, 0, FALSE, 3, 0, DYN_MULTICHOICE_CB_NONE, Immo_Text_Expulser, Immo_Text_Vendre, Immo_Text_Retour
	goto_if_eq VAR_RESULT, 0, Immo_EventScript_Expulser
	goto_if_eq VAR_RESULT, 1, Immo_EventScript_Vendre
	goto Immo_EventScript_FicheFin
Immo_EventScript_Vendre::
	message Immo_Text_SurDeVendre
	waitmessage
	dynmultichoice 0, 0, FALSE, 2, 1, DYN_MULTICHOICE_CB_NONE, Immo_Text_Oui, Immo_Text_Non
	goto_if_ne VAR_RESULT, 0, Immo_EventScript_FicheFin
	special Immo_ScriptVendre
Immo_EventScript_Resultat::
	goto_if_eq VAR_RESULT, FALSE, Immo_EventScript_ResultatRate
	playse SE_SHOP
	msgbox gStringVar4, MSGBOX_DEFAULT
	goto Immo_EventScript_FicheFin
Immo_EventScript_ResultatRate::
	playse SE_BOO
	msgbox gStringVar4, MSGBOX_DEFAULT
Immo_EventScript_FicheFin::
	return

Immo_EventScript_Expulser::
	copyvar VAR_IMMO_BIEN, VAR_0x8004
	msgbox Immo_Text_Squatteur, MSGBOX_DEFAULT
	closemessage
	specialvar VAR_RESULT, Immo_NiveauSquatteur
	switch VAR_RESULT
	case 1, Immo_EventScript_Squat1
	case 2, Immo_EventScript_Squat2
	case 3, Immo_EventScript_Squat3
Immo_EventScript_Squat0::
	setvar VAR_IMMO_REMETTRE, 0
	goto_if_defeated TRAINER_GRUNT_PETALBURG_WOODS, Immo_EventScript_Squat0Combat
	setvar VAR_IMMO_REMETTRE, 1
Immo_EventScript_Squat0Combat::
	cleartrainerflag TRAINER_GRUNT_PETALBURG_WOODS
	trainerbattle_no_intro TRAINER_GRUNT_PETALBURG_WOODS, Immo_Text_SquatteurBattu
	goto_if_eq VAR_IMMO_REMETTRE, 0, Immo_EventScript_SquatGagne
	cleartrainerflag TRAINER_GRUNT_PETALBURG_WOODS
	goto Immo_EventScript_SquatGagne

Immo_EventScript_Squat1::
	setvar VAR_IMMO_REMETTRE, 0
	goto_if_defeated TRAINER_GRUNT_MUSEUM_1, Immo_EventScript_Squat1Combat
	setvar VAR_IMMO_REMETTRE, 1
Immo_EventScript_Squat1Combat::
	cleartrainerflag TRAINER_GRUNT_MUSEUM_1
	trainerbattle_no_intro TRAINER_GRUNT_MUSEUM_1, Immo_Text_SquatteurBattu
	goto_if_eq VAR_IMMO_REMETTRE, 0, Immo_EventScript_SquatGagne
	cleartrainerflag TRAINER_GRUNT_MUSEUM_1
	goto Immo_EventScript_SquatGagne

Immo_EventScript_Squat2::
	setvar VAR_IMMO_REMETTRE, 0
	goto_if_defeated TRAINER_GRUNT_WEATHER_INST_1, Immo_EventScript_Squat2Combat
	setvar VAR_IMMO_REMETTRE, 1
Immo_EventScript_Squat2Combat::
	cleartrainerflag TRAINER_GRUNT_WEATHER_INST_1
	trainerbattle_no_intro TRAINER_GRUNT_WEATHER_INST_1, Immo_Text_SquatteurBattu
	goto_if_eq VAR_IMMO_REMETTRE, 0, Immo_EventScript_SquatGagne
	cleartrainerflag TRAINER_GRUNT_WEATHER_INST_1
	goto Immo_EventScript_SquatGagne

Immo_EventScript_Squat3::
	setvar VAR_IMMO_REMETTRE, 0
	goto_if_defeated TRAINER_GRUNT_AQUA_HIDEOUT_1, Immo_EventScript_Squat3Combat
	setvar VAR_IMMO_REMETTRE, 1
Immo_EventScript_Squat3Combat::
	cleartrainerflag TRAINER_GRUNT_AQUA_HIDEOUT_1
	trainerbattle_no_intro TRAINER_GRUNT_AQUA_HIDEOUT_1, Immo_Text_SquatteurBattu
	goto_if_eq VAR_IMMO_REMETTRE, 0, Immo_EventScript_SquatGagne
	cleartrainerflag TRAINER_GRUNT_AQUA_HIDEOUT_1
	goto Immo_EventScript_SquatGagne

Immo_EventScript_SquatGagne::
	special Immo_ScriptExpulser
	msgbox Immo_Text_Expulse, MSGBOX_DEFAULT
	return

@ Panneaux « À vendre »
Immo_EventScript_Panneau0::
	setvar VAR_0x8004, 0
	goto Immo_EventScript_Panneau
Immo_EventScript_Panneau1::
	setvar VAR_0x8004, 1
	goto Immo_EventScript_Panneau
Immo_EventScript_Panneau2::
	setvar VAR_0x8004, 2
	goto Immo_EventScript_Panneau
Immo_EventScript_Panneau3::
	setvar VAR_0x8004, 3
	goto Immo_EventScript_Panneau
Immo_EventScript_Panneau4::
	setvar VAR_0x8004, 4
	goto Immo_EventScript_Panneau
Immo_EventScript_Panneau5::
	setvar VAR_0x8004, 5
	goto Immo_EventScript_Panneau
Immo_EventScript_Panneau6::
	setvar VAR_0x8004, 6
	goto Immo_EventScript_Panneau
Immo_EventScript_Panneau7::
	setvar VAR_0x8004, 7
	goto Immo_EventScript_Panneau
Immo_EventScript_Panneau8::
	setvar VAR_0x8004, 8
	goto Immo_EventScript_Panneau
Immo_EventScript_Panneau9::
	setvar VAR_0x8004, 9
	goto Immo_EventScript_Panneau
Immo_EventScript_Panneau10::
	setvar VAR_0x8004, 10
	goto Immo_EventScript_Panneau
Immo_EventScript_Panneau11::
	setvar VAR_0x8004, 11
	goto Immo_EventScript_Panneau
Immo_EventScript_Panneau::
	lock
	faceplayer
	msgbox Immo_Text_Panneau, MSGBOX_DEFAULT
	special Immo_Ouvrir
	goto_if_eq VAR_RESULT, FALSE, Immo_EventScript_PanneauFiche
	msgbox gStringVar4, MSGBOX_DEFAULT
Immo_EventScript_PanneauFiche::
	call Immo_EventScript_Fiche
	closemessage
	release
	end

Immo_Text_Catalogue:
	.string "Catalogue$"
Immo_Text_Fermer:
	.string "Fermer$"
Immo_Text_Retour:
	.string "Retour$"
Immo_Text_Acheter:
	.string "Acheter$"
Immo_Text_Vendre:
	.string "Vendre$"
Immo_Text_Expulser:
	.string "Expulser le squatteur$"
Immo_Text_Oui:
	.string "Oui$"
Immo_Text_Non:
	.string "Non$"

Immo_Text_QuelBien:
	.string "+ à toi, ! squatté, % en promo.$"

Immo_Text_SurDeVendre:
	.string "Tu revends vraiment ce bien ?$"

Immo_Text_Panneau:
	.string "“À VENDRE”\n"
	.string "Agence immobilière de Hoenn.$"

Immo_Text_Squatteur:
	.string "Sbire Aqua : Hé, c'est chez moi ici,\n"
	.string "maintenant. La mer appartient à tout\l"
	.string "le monde, ta baraque aussi. Dégage !$"

Immo_Text_SquatteurBattu:
	.string "Ok, ok, je me casse…\n"
	.string "J'ai même pas fait la vaisselle.$"

Immo_Text_Expulse:
	.string "Le squatteur a filé. Ton locataire\n"
	.string "revient et les loyers reprennent.$"
