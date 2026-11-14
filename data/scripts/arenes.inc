@ Émeraude Couronnement : arènes du joueur (v0.40), src/arenes.c. Un responsable par ville (VAR_0x8008 = arène).

Arene_EventScript_Guichet0::
	lock
	faceplayer
	setvar VAR_0x8008, 0
	goto Arene_EventScript_Guichet

Arene_EventScript_Guichet1::
	lock
	faceplayer
	setvar VAR_0x8008, 1
	goto Arene_EventScript_Guichet

Arene_EventScript_Guichet2::
	lock
	faceplayer
	setvar VAR_0x8008, 2
	goto Arene_EventScript_Guichet

Arene_EventScript_Guichet3::
	lock
	faceplayer
	setvar VAR_0x8008, 3
	goto Arene_EventScript_Guichet

Arene_EventScript_Guichet4::
	lock
	faceplayer
	setvar VAR_0x8008, 4
	goto Arene_EventScript_Guichet

Arene_EventScript_Guichet5::
	lock
	faceplayer
	setvar VAR_0x8008, 5
	goto Arene_EventScript_Guichet

Arene_EventScript_Guichet::
	special Arene_Ouvrir
	goto_if_eq VAR_RESULT, 0, Arene_EventScript_PasLigue
	goto_if_eq VAR_RESULT, 1, Arene_EventScript_AVendre
Arene_EventScript_Fiche::
	special Arene_Ouvrir
	message gStringVar4
	waitmessage
	dynmultichoice 0, 0, FALSE, 4, 0, DYN_MULTICHOICE_CB_NONE, Arene_Text_Affronter, Arene_Text_Deposer, Arene_Text_Retirer, Arene_Text_Partir
	switch VAR_RESULT
	case 0, Arene_EventScript_Affronter
	case 1, Arene_EventScript_Deposer
	case 2, Arene_EventScript_Retirer
Arene_EventScript_Fin::
	closemessage
	release
	end

Arene_EventScript_Affronter::
	special Arene_PreparerChallenger
	goto_if_eq VAR_RESULT, FALSE, Arene_EventScript_Info
	msgbox gStringVar4, MSGBOX_DEFAULT
	msgbox Arene_Text_OnYVa, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, Arene_EventScript_Fiche
	closemessage
	special Arene_DebutCombat
	trainerbattle_no_intro TRAINER_COURONNEMENT, Arene_Text_ChallengerBattu
	cleartrainerflag TRAINER_COURONNEMENT
	special Arene_FinCombat
	goto_if_eq VAR_RESULT, FALSE, Arene_EventScript_Defaite
	playfanfare MUS_OBTAIN_ITEM
	msgbox gStringVar4, MSGBOX_DEFAULT
	waitfanfare
	goto Arene_EventScript_Fiche

Arene_EventScript_Defaite::
	playse SE_BOO
	msgbox gStringVar4, MSGBOX_DEFAULT
	goto Arene_EventScript_Fiche

Arene_EventScript_Info::
	msgbox gStringVar4, MSGBOX_DEFAULT
	goto Arene_EventScript_Fiche

Arene_EventScript_Deposer::
	msgbox Arene_Text_QuiDeposer, MSGBOX_DEFAULT
	special ChoosePartyMon
	waitstate
	goto_if_eq VAR_0x8004, PARTY_NOTHING_CHOSEN, Arene_EventScript_Fiche
	message Arene_Text_Poste
	waitmessage
	dynmultichoice 0, 0, FALSE, 3, 0, DYN_MULTICHOICE_CB_NONE, Arene_Text_Titulaire, Arene_Text_Remplacant, Arene_Text_Retour
	goto_if_eq VAR_RESULT, MULTI_B_PRESSED, Arene_EventScript_Fiche
	goto_if_eq VAR_RESULT, 2, Arene_EventScript_Fiche
	copyvar VAR_0x8005, VAR_RESULT
	special Arene_ScriptDeposer
	msgbox gStringVar4, MSGBOX_DEFAULT
	goto Arene_EventScript_Fiche

Arene_EventScript_Retirer::
	special Arene_PreparerListe
	message Arene_Text_QuiRetirer
	waitmessage
	dynmultipush gAreneLignes + 0, 0
	dynmultipush gAreneLignes + 24, 1
	dynmultipush gAreneLignes + 48, 2
	dynmultipush gAreneLignes + 72, 3
	dynmultipush gAreneLignes + 96, 4
	dynmultipush gAreneLignes + 120, 5
	dynmultipush gAreneLignes + 144, 6
	dynmultipush gAreneLignes + 168, 7
	dynmultipush gAreneLignes + 192, 8
	dynmultipush Arene_Text_Retour, 9
	dynmultistack 0, 0, FALSE, 6, FALSE, 0, DYN_MULTICHOICE_CB_NONE
	goto_if_eq VAR_RESULT, MULTI_B_PRESSED, Arene_EventScript_Fiche
	goto_if_eq VAR_RESULT, 9, Arene_EventScript_Fiche
	copyvar VAR_0x8005, VAR_RESULT
	special Arene_ScriptRetirer
	msgbox gStringVar4, MSGBOX_DEFAULT
	goto Arene_EventScript_Fiche

Arene_EventScript_AVendre::
	msgbox Arene_Text_AVendre, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, Arene_EventScript_Fin
	message Arene_Text_QuelType
	waitmessage
	dynmultichoice 0, 0, FALSE, 6, 0, DYN_MULTICHOICE_CB_NONE, Arene_Text_Plante, Arene_Text_Glace, Arene_Text_Insecte, Arene_Text_Poison, Arene_Text_Sol, Arene_Text_Spectre, Arene_Text_Dragon, Arene_Text_Tenebres, Arene_Text_Acier, Arene_Text_Fee, Arene_Text_Retour
	goto_if_eq VAR_RESULT, MULTI_B_PRESSED, Arene_EventScript_Fin
	goto_if_eq VAR_RESULT, 10, Arene_EventScript_Fin
	copyvar VAR_0x8005, VAR_RESULT
	special Arene_ScriptAcheter
	goto_if_eq VAR_RESULT, FALSE, Arene_EventScript_PasAssez
	playfanfare MUS_OBTAIN_SYMBOL
	msgbox Arene_Text_Achetee, MSGBOX_DEFAULT
	waitfanfare
	goto Arene_EventScript_Fiche

Arene_EventScript_PasAssez::
	msgbox Arene_Text_PasAssez, MSGBOX_DEFAULT
	goto Arene_EventScript_Fin

Arene_EventScript_PasLigue::
	msgbox Arene_Text_PasLigue, MSGBOX_DEFAULT
	goto Arene_EventScript_Fin

@ Menu L : résumé de toutes les arènes.
Arene_EventScript_MenuL::
	special Arene_Resume
	msgbox gStringVar4, MSGBOX_DEFAULT
	return

Arene_Text_Affronter:
	.string "Affronter un challenger$"
Arene_Text_Deposer:
	.string "Déposer un Pokémon$"
Arene_Text_Retirer:
	.string "Retirer un Pokémon$"
Arene_Text_Partir:
	.string "Partir$"
Arene_Text_Retour:
	.string "Retour$"
Arene_Text_Titulaire:
	.string "Titulaire$"
Arene_Text_Remplacant:
	.string "Remplaçant$"
Arene_Text_Plante:
	.string "Plante$"
Arene_Text_Glace:
	.string "Glace$"
Arene_Text_Insecte:
	.string "Insecte$"
Arene_Text_Poison:
	.string "Poison$"
Arene_Text_Sol:
	.string "Sol$"
Arene_Text_Spectre:
	.string "Spectre$"
Arene_Text_Dragon:
	.string "Dragon$"
Arene_Text_Tenebres:
	.string "Ténèbres$"
Arene_Text_Acier:
	.string "Acier$"
Arene_Text_Fee:
	.string "Fée$"

Arene_Text_OnYVa:
	.string "Le challenger t'attend sur le\n"
	.string "terrain. On y va ?$"

Arene_Text_ChallengerBattu:
	.string "… Ton arène est solide. Je\n"
	.string "reviendrai plus fort.$"

Arene_Text_QuiDeposer:
	.string "Quel Pokémon s'installe dans\n"
	.string "l'arène ? Il y restera bloqué\l"
	.string "jusqu'à ce que tu le retires.$"

Arene_Text_Poste:
	.string "À quel poste ? 6 titulaires et\n"
	.string "3 remplaçants.$"

Arene_Text_QuiRetirer:
	.string "Qui quitte l'arène ?$"

Arene_Text_AVendre:
	.string "Responsable : L'arène de {STR_VAR_1}\n"
	.string "est à vendre, 500 000 ¥.\p"
	.string "Le type est à toi de choisir. Les\n"
	.string "challengers viennent te défier, tu\l"
	.string "touches les sponsors.\p"
	.string "Tu l'achètes ?$"

Arene_Text_QuelType:
	.string "Quel type pour ton arène ?$"

Arene_Text_Achetee:
	.string "{PLAYER} est Champion de l'arène\n"
	.string "de {STR_VAR_1} !\p"
	.string "Dépose au moins 4 titulaires du\n"
	.string "type choisi pour ouvrir les portes\l"
	.string "aux challengers.$"

Arene_Text_PasAssez:
	.string "Il te manque de l'argent. La\n"
	.string "trésorerie de ta société compte\l"
	.string "aussi.$"

Arene_Text_PasLigue:
	.string "Responsable : Une arène sera à\n"
	.string "vendre ici. Mais seul un vainqueur\l"
	.string "de la Ligue peut en tenir une.$"
