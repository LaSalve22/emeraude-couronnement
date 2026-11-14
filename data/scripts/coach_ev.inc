@ Émeraude Couronnement : Coach EV du Centre Pokémon de Clémenti-Ville (src/coach_ev.c).
@ Après la Ligue (FLAG_SYS_GAME_CLEAR) : règle les EV d'un Pokémon stat par stat, gratuitement.
CoachEV_EventScript_Coach::
	lock
	faceplayer
	goto_if_unset FLAG_SYS_GAME_CLEAR, CoachEV_EventScript_PasEncore
	msgbox CoachEV_Text_Bienvenue, MSGBOX_DEFAULT
	call CoachEV_EventScript_Choix
	msgbox CoachEV_Text_AuRevoir, MSGBOX_DEFAULT
	release
	end

@ EV ou IV ? Aussi appelé par le menu L (après la Ligue).
CoachEV_EventScript_Choix::
	message CoachEV_Text_EVouIV
	waitmessage
	dynmultichoice 0, 0, FALSE, 3, 0, DYN_MULTICHOICE_CB_NONE, CoachEV_Text_LesEV, CoachEV_Text_LesIV, CoachEV_Text_Termine
	goto_if_eq VAR_RESULT, 0, CoachEV_EventScript_Session
	goto_if_eq VAR_RESULT, 1, CoachIV_EventScript_Session
	return

@ Choix du Pokémon puis réglage stat par stat.
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

@ ---------------------------------------------------------------- Coach IV (v0.32) : IV de 0 à 31, stat par stat, gratuit.
CoachIV_EventScript_Session::
	special ChoosePartyMon
	waitstate
	goto_if_eq VAR_0x8004, PARTY_NOTHING_CHOSEN, CoachIV_EventScript_SessionFin
	specialvar VAR_RESULT, ScriptGetPartyMonSpecies
	goto_if_ne VAR_RESULT, SPECIES_EGG, CoachIV_EventScript_Menu
	msgbox CoachEV_Text_Oeuf, MSGBOX_DEFAULT
	goto CoachIV_EventScript_SessionFin
CoachIV_EventScript_Menu::
	setvar VAR_0x8007, 0
	special CoachIV_Afficher
	message gStringVar4
	waitmessage
	dynmultichoice 0, 0, FALSE, 8, 0, DYN_MULTICHOICE_CB_NONE, CoachEV_Text_PV, CoachEV_Text_Attaque, CoachEV_Text_Defense, CoachEV_Text_AtqSpe, CoachEV_Text_DefSpe, CoachEV_Text_Vitesse, CoachIV_Text_ToutA31, CoachEV_Text_Termine
	goto_if_eq VAR_RESULT, MULTI_B_PRESSED, CoachIV_EventScript_SessionFin
	goto_if_eq VAR_RESULT, 7, CoachIV_EventScript_SessionFin
	copyvar VAR_0x8005, VAR_RESULT
	setvar VAR_0x8006, 0
	goto_if_eq VAR_RESULT, COACH_IV_TOUT_A_31, CoachIV_EventScript_Appliquer
	message CoachIV_Text_Combien
	waitmessage
	dynmultichoice 0, 0, FALSE, 6, 0, DYN_MULTICHOICE_CB_NONE, CoachIV_Text_31, CoachIV_Text_30, CoachIV_Text_29, CoachIV_Text_28, CoachIV_Text_27, CoachIV_Text_26, CoachIV_Text_25, CoachIV_Text_24, CoachIV_Text_23, CoachIV_Text_22, CoachIV_Text_21, CoachIV_Text_20, CoachIV_Text_19, CoachIV_Text_18, CoachIV_Text_17, CoachIV_Text_16, CoachIV_Text_15, CoachIV_Text_14, CoachIV_Text_13, CoachIV_Text_12, CoachIV_Text_11, CoachIV_Text_10, CoachIV_Text_9, CoachIV_Text_8, CoachIV_Text_7, CoachIV_Text_6, CoachIV_Text_5, CoachIV_Text_4, CoachIV_Text_3, CoachIV_Text_2, CoachIV_Text_1, CoachIV_Text_0
	goto_if_eq VAR_RESULT, MULTI_B_PRESSED, CoachIV_EventScript_Menu
	copyvar VAR_0x8006, VAR_RESULT
CoachIV_EventScript_Appliquer::
	special CoachIV_Appliquer
	playse SE_SELECT
	goto CoachIV_EventScript_Menu
CoachIV_EventScript_SessionFin::
	return

@ ---------------------------------------------------------------- Capsules d'Argent / d'Or (v0.32), lancées depuis le sac
@ (ItemUseOutOfBattle_Capsule, src/item_use.c). gSpecialVar_ItemId = la Capsule utilisée.
Capsule_EventScript_Utiliser::
	lockall
	msgbox Capsule_Text_QuelPokemon, MSGBOX_DEFAULT
	special ChoosePartyMon
	waitstate
	goto_if_eq VAR_0x8004, PARTY_NOTHING_CHOSEN, Capsule_EventScript_Fin
	specialvar VAR_RESULT, ScriptGetPartyMonSpecies
	goto_if_eq VAR_RESULT, SPECIES_EGG, Capsule_EventScript_Oeuf
	specialvar VAR_RESULT, Capsule_EstOr
	goto_if_eq VAR_RESULT, TRUE, Capsule_EventScript_Appliquer
	setvar VAR_0x8007, 1
	special CoachIV_Afficher
	message gStringVar4
	waitmessage
	dynmultichoice 0, 0, FALSE, 7, 0, DYN_MULTICHOICE_CB_NONE, CoachEV_Text_PV, CoachEV_Text_Attaque, CoachEV_Text_Defense, CoachEV_Text_AtqSpe, CoachEV_Text_DefSpe, CoachEV_Text_Vitesse, Capsule_Text_Annuler
	goto_if_eq VAR_RESULT, MULTI_B_PRESSED, Capsule_EventScript_Fin
	goto_if_eq VAR_RESULT, 6, Capsule_EventScript_Fin
	copyvar VAR_0x8005, VAR_RESULT
Capsule_EventScript_Appliquer::
	specialvar VAR_RESULT, Capsule_Utiliser
	goto_if_eq VAR_RESULT, FALSE, Capsule_EventScript_DejaMax
	bufferpartymonnick STR_VAR_1, VAR_0x8004
	playse SE_USE_ITEM
	msgbox Capsule_Text_Fait, MSGBOX_DEFAULT
	goto Capsule_EventScript_Fin
Capsule_EventScript_DejaMax::
	msgbox Capsule_Text_DejaMax, MSGBOX_DEFAULT
	goto Capsule_EventScript_Fin
Capsule_EventScript_Oeuf::
	msgbox Capsule_Text_Oeuf, MSGBOX_DEFAULT
Capsule_EventScript_Fin::
	closemessage
	releaseall
	end

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
CoachEV_Text_LesEV:
	.string "Les EV$"
CoachEV_Text_LesIV:
	.string "Les IV$"
CoachIV_Text_ToutA31:
	.string "Tout à 31$"
Capsule_Text_Annuler:
	.string "Annuler$"
CoachIV_Text_31:
	.string "31$"
CoachIV_Text_30:
	.string "30$"
CoachIV_Text_29:
	.string "29$"
CoachIV_Text_28:
	.string "28$"
CoachIV_Text_27:
	.string "27$"
CoachIV_Text_26:
	.string "26$"
CoachIV_Text_25:
	.string "25$"
CoachIV_Text_24:
	.string "24$"
CoachIV_Text_23:
	.string "23$"
CoachIV_Text_22:
	.string "22$"
CoachIV_Text_21:
	.string "21$"
CoachIV_Text_20:
	.string "20$"
CoachIV_Text_19:
	.string "19$"
CoachIV_Text_18:
	.string "18$"
CoachIV_Text_17:
	.string "17$"
CoachIV_Text_16:
	.string "16$"
CoachIV_Text_15:
	.string "15$"
CoachIV_Text_14:
	.string "14$"
CoachIV_Text_13:
	.string "13$"
CoachIV_Text_12:
	.string "12$"
CoachIV_Text_11:
	.string "11$"
CoachIV_Text_10:
	.string "10$"
CoachIV_Text_9:
	.string "9$"
CoachIV_Text_8:
	.string "8$"
CoachIV_Text_7:
	.string "7$"
CoachIV_Text_6:
	.string "6$"
CoachIV_Text_5:
	.string "5$"
CoachIV_Text_4:
	.string "4$"
CoachIV_Text_3:
	.string "3$"
CoachIV_Text_2:
	.string "2$"
CoachIV_Text_1:
	.string "1$"
CoachIV_Text_0:
	.string "0$"
CoachEV_Text_252:
	.string "252 (max)$"
CoachEV_Text_128:
	.string "128$"
CoachEV_Text_4:
	.string "4$"
CoachEV_Text_0:
	.string "0$"

CoachEV_Text_Bienvenue:
	.string "Coach EV et IV, pour te servir. Tu\n"
	.string "as battu la Ligue, alors on passe\l"
	.string "aux choses sérieuses : je réécris\l"
	.string "l'entraînement de ton Pokémon.\p"
	.string "EV : 252 max par stat, 510 au total.\n"
	.string "IV : de 0 à 31. C'est gratuit.$"

CoachEV_Text_EVouIV:
	.string "On travaille quoi ? Les EV, ou les\n"
	.string "IV, de 0 à 31 ?$"

CoachIV_Text_Combien:
	.string "Quel IV dans cette stat ?$"

Capsule_Text_QuelPokemon:
	.string "Sur quel Pokémon utiliser la\n"
	.string "Capsule ?$"

Capsule_Text_Fait:
	.string "{STR_VAR_1} se sent plus fort !\n"
	.string "IV montés à 31.$"

Capsule_Text_DejaMax:
	.string "Cet IV est déjà à 31. La Capsule\n"
	.string "n'a pas été utilisée.$"

Capsule_Text_Oeuf:
	.string "Ça ne marche pas sur un Œuf.$"

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
	.string "Je règle les EV et les IV des\n"
	.string "Pokémon, au point près. Mais je ne\l"
	.string "bosse qu'avec les Champions de la\l"
	.string "Ligue.\p"
	.string "Reviens quand tu l'auras battue.$"
