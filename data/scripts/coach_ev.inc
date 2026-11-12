@ Émeraude Couronnement : Coach EV du Centre Pokémon de Clémenti-Ville (src/coach_ev.c).
@ Après la Ligue (FLAG_SYS_GAME_CLEAR) : règle les EV d'un Pokémon stat par stat, gratuitement.
CoachEV_EventScript_Coach::
	lock
	faceplayer
	goto_if_unset FLAG_SYS_GAME_CLEAR, CoachEV_EventScript_PasEncore
	msgbox CoachEV_Text_Bienvenue, MSGBOX_DEFAULT
	call CoachEV_EventScript_Session
	msgbox CoachEV_Text_AuRevoir, MSGBOX_DEFAULT
	release
	end

@ Choix du Pokémon puis réglage stat par stat ; aussi appelé par le menu L (après la Ligue).
CoachEV_EventScript_Session::
	special ChoosePartyMon
	waitstate
	goto_if_eq VAR_0x8004, PARTY_NOTHING_CHOSEN, CoachEV_EventScript_SessionFin
	specialvar VAR_RESULT, ScriptGetPartyMonSpecies
	goto_if_ne VAR_RESULT, SPECIES_EGG, CoachEV_EventScript_Menu
	msgbox CoachEV_Text_Oeuf, MSGBOX_DEFAULT
	goto CoachEV_EventScript_SessionFin
CoachEV_EventScript_Menu::
	special CoachEV_Afficher
	message gStringVar4
	waitmessage
	dynmultichoice 0, 0, FALSE, 8, 0, DYN_MULTICHOICE_CB_NONE, CoachEV_Text_PV, CoachEV_Text_Attaque, CoachEV_Text_Defense, CoachEV_Text_AtqSpe, CoachEV_Text_DefSpe, CoachEV_Text_Vitesse, CoachEV_Text_ToutAZero, CoachEV_Text_Termine
	goto_if_eq VAR_RESULT, MULTI_B_PRESSED, CoachEV_EventScript_SessionFin
	goto_if_eq VAR_RESULT, 7, CoachEV_EventScript_SessionFin
	copyvar VAR_0x8005, VAR_RESULT
	goto_if_eq VAR_RESULT, COACH_EV_TOUT_A_ZERO, CoachEV_EventScript_Appliquer
	message CoachEV_Text_Combien
	waitmessage
	dynmultichoice 0, 0, FALSE, 4, 0, DYN_MULTICHOICE_CB_NONE, CoachEV_Text_252, CoachEV_Text_128, CoachEV_Text_4, CoachEV_Text_0
	goto_if_eq VAR_RESULT, MULTI_B_PRESSED, CoachEV_EventScript_Menu
	copyvar VAR_0x8006, VAR_RESULT
CoachEV_EventScript_Appliquer::
	special CoachEV_Appliquer
	goto_if_eq VAR_RESULT, FALSE, CoachEV_EventScript_Menu
	msgbox CoachEV_Text_PasDePlace, MSGBOX_DEFAULT
	goto CoachEV_EventScript_Menu
CoachEV_EventScript_SessionFin::
	return

CoachEV_EventScript_PasEncore::
	msgbox CoachEV_Text_PasEncore, MSGBOX_DEFAULT
	release
	end

CoachEV_Text_PV:
	.string "PV$"
CoachEV_Text_Attaque:
	.string "Attaque$"
CoachEV_Text_Defense:
	.string "Défense$"
CoachEV_Text_AtqSpe:
	.string "Atq. Spé.$"
CoachEV_Text_DefSpe:
	.string "Déf. Spé.$"
CoachEV_Text_Vitesse:
	.string "Vitesse$"
CoachEV_Text_ToutAZero:
	.string "Tout à 0$"
CoachEV_Text_Termine:
	.string "Terminé$"
CoachEV_Text_252:
	.string "252 (max)$"
CoachEV_Text_128:
	.string "128$"
CoachEV_Text_4:
	.string "4$"
CoachEV_Text_0:
	.string "0$"

CoachEV_Text_Bienvenue:
	.string "Coach EV, pour te servir. Tu as\n"
	.string "battu la Ligue, alors on passe aux\l"
	.string "choses sérieuses : je réécris\l"
	.string "l'entraînement de ton Pokémon.\p"
	.string "252 max par stat, 510 au total.\n"
	.string "C'est gratuit. Lequel ?$"

CoachEV_Text_Combien:
	.string "Combien d'EV dans cette stat ?$"

CoachEV_Text_PasDePlace:
	.string "Pas assez de place : 510 au total,\n"
	.string "pas un de plus. J'ai mis le maximum\l"
	.string "possible. Libère une autre stat.$"

CoachEV_Text_Oeuf:
	.string "Un Œuf ? Reviens quand il aura\n"
	.string "des muscles. Ou au moins des pattes.$"

CoachEV_Text_AuRevoir:
	.string "Allez, file. Et pas de flemme à\n"
	.string "l'entraînement, hein.$"

CoachEV_Text_PasEncore:
	.string "Je règle les EV des Pokémon, au\n"
	.string "point près. Mais je ne bosse qu'avec\l"
	.string "les Champions de la Ligue.\p"
	.string "Reviens quand tu l'auras battue.$"
