@ Émeraude Couronnement : édition Carrière & Business, intro adulte (v0.47), src/tycoon.c.
@ Nouvelle partie : pas de camion ni de maman. On se réveille seul dans sa chambre (appartement loué, 2 500 ¥ par mois tant
@ qu'on n'a pas acheté de maison), on choisit la difficulté, on règle l'horloge et on choisit son profil sur le terminal.
@ Seko est sauvé sur la Route 101 comme d'habitude ; au labo il donne aussitôt le Pokédex (pas de rival sur la Route 103).

Tycoon_EventScript_Reveil::
	lockall
	call Tycoon_EventScript_PoserDecor
	msgbox Tycoon_Text_Reveil, MSGBOX_DEFAULT
	call InsideOfTruck_EventScript_DifficultyChoice
	msgbox Tycoon_Text_Horloge, MSGBOX_DEFAULT
	call PlayersHouse_2F_EventScript_SetWallClock
	delay 30
	setflag FLAG_SET_WALL_CLOCK
	msgbox Tycoon_Text_Terminal, MSGBOX_DEFAULT
Tycoon_EventScript_Profil::
	message Tycoon_Text_QuelProfil
	waitmessage
	dynmultichoice 0, 0, TRUE, 3, 0, DYN_MULTICHOICE_CB_NONE, Tycoon_Text_Epargnant, Tycoon_Text_Trader, Tycoon_Text_Espoir
	switch VAR_RESULT
	case 0, Tycoon_EventScript_ProfilEpargnant
	case 1, Tycoon_EventScript_ProfilTrader
	case 2, Tycoon_EventScript_ProfilEspoir
	goto Tycoon_EventScript_Profil
Tycoon_EventScript_ProfilEpargnant::
	msgbox Tycoon_Text_ExpliqueEpargnant, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, Tycoon_EventScript_Profil
	setvar VAR_0x8004, 0
	goto Tycoon_EventScript_ProfilChoisi
Tycoon_EventScript_ProfilTrader::
	msgbox Tycoon_Text_ExpliqueTrader, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, Tycoon_EventScript_Profil
	setvar VAR_0x8004, 1
	goto Tycoon_EventScript_ProfilChoisi
Tycoon_EventScript_ProfilEspoir::
	msgbox Tycoon_Text_ExpliqueEspoir, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, Tycoon_EventScript_Profil
	setvar VAR_0x8004, 2
Tycoon_EventScript_ProfilChoisi::
	special Tycoon_ChoisirProfil
	playfanfare MUS_OBTAIN_ITEM
	msgbox gStringVar4, MSGBOX_DEFAULT
	waitfanfare
	goto_if_ne VAR_0x8004, 2, Tycoon_EventScript_PassEtPokeNav
	giveitem ITEM_EXP_CANDY_S, 3
Tycoon_EventScript_PassEtPokeNav::
	msgbox Tycoon_Text_PassCourrier, MSGBOX_DEFAULT
	playfanfare MUS_OBTAIN_ITEM
	message Couronnement_Text_PassRecu
	waitfanfare
	special Couronnement_DonnerPass
	setflag FLAG_SYS_POKENAV_GET
	setflag FLAG_RECEIVED_POKENAV
	call Couronnement_EventScript_DonnerRepousseInfinie
	msgbox Tycoon_Text_Dehors, MSGBOX_DEFAULT
	setvar VAR_LITTLEROOT_INTRO_STATE, 7
	releaseall
	end

@ Appartement vide : pas de maman, pas de camion, pas de cartons ; le rival est déjà parti (Route 103 sautée).
Tycoon_EventScript_PoserDecor::
	setflag FLAG_HIDE_MAP_NAME_POPUP
	setflag FLAG_HIDE_LITTLEROOT_TOWN_BRENDANS_HOUSE_MOM
	setflag FLAG_HIDE_LITTLEROOT_TOWN_MAYS_HOUSE_MOM
	setflag FLAG_HIDE_LITTLEROOT_TOWN_PLAYERS_BEDROOM_MOM
	setflag FLAG_HIDE_LITTLEROOT_TOWN_MOM_OUTSIDE
	setflag FLAG_HIDE_LITTLEROOT_TOWN_BRENDANS_HOUSE_TRUCK
	setflag FLAG_HIDE_LITTLEROOT_TOWN_MAYS_HOUSE_TRUCK
	setflag FLAG_HIDE_LITTLEROOT_TOWN_PLAYERS_HOUSE_VIGOROTH_1
	setflag FLAG_HIDE_LITTLEROOT_TOWN_PLAYERS_HOUSE_VIGOROTH_2
	setflag FLAG_HIDE_LITTLEROOT_TOWN_BRENDANS_HOUSE_2F_POKE_BALL
	setflag FLAG_HIDE_LITTLEROOT_TOWN_MAYS_HOUSE_2F_POKE_BALL
	setflag FLAG_HIDE_LITTLEROOT_TOWN_FAT_MAN
	setflag FLAG_HIDE_ROUTE_103_RIVAL
	setflag FLAG_RIVAL_LEFT_FOR_ROUTE103
	setvar VAR_LITTLEROOT_TOWN_STATE, 1
	setvar VAR_LITTLEROOT_RIVAL_STATE, 3
	checkplayergender
	goto_if_eq VAR_RESULT, FEMALE, Tycoon_EventScript_PoserDecorFille
	setrespawn HEAL_LOCATION_LITTLEROOT_TOWN_BRENDANS_HOUSE_2F
	setvar VAR_LITTLEROOT_HOUSES_STATE_BRENDAN, 1
	clearflag FLAG_HIDE_MAP_NAME_POPUP
	return
Tycoon_EventScript_PoserDecorFille::
	setrespawn HEAL_LOCATION_LITTLEROOT_TOWN_MAYS_HOUSE_2F
	setvar VAR_LITTLEROOT_HOUSES_STATE_MAY, 1
	clearflag FLAG_HIDE_MAP_NAME_POPUP
	return

@ Labo de Seko, après le choix du starter : Pokédex, Poké Balls et Dark Ball tout de suite, pas de rival à aller voir.
Tycoon_EventScript_LaboApresStarter::
	msgbox Tycoon_Text_SekoPokedex, MSGBOX_DEFAULT
	call LittlerootTown_ProfessorBirchsLab_EventScript_ReceivePokedex
	msgbox LittlerootTown_ProfessorBirchsLab_Text_ExplainPokedex, MSGBOX_DEFAULT
	giveitem ITEM_POKE_BALL, 5
	call LittlerootTown_ProfessorBirchsLab_EventScript_DarkBall
	msgbox Tycoon_Text_SekoAuRevoir, MSGBOX_DEFAULT
	setflag FLAG_DEFEATED_RIVAL_ROUTE103
	setvar VAR_BIRCH_LAB_STATE, 5
	setflag FLAG_ADVENTURE_STARTED
	setvar VAR_OLDALE_TOWN_STATE, 1
	setvar VAR_LITTLEROOT_RIVAL_STATE, 4
	setvar VAR_LITTLEROOT_TOWN_STATE, 4
	releaseall
	end

@ Loyer : appelé à l'ouverture du menu L.
Tycoon_EventScript_Loyer::
	special Tycoon_Loyer
	goto_if_eq VAR_RESULT, FALSE, Tycoon_EventScript_LoyerFin
	playse SE_SHOP
	msgbox gStringVar4, MSGBOX_DEFAULT
Tycoon_EventScript_LoyerFin::
	return

Tycoon_Text_Reveil:
	.string "Le réveil hurle. Une chambre louée,\n"
	.string "des murs nus, un frigo presque vide.\p"
	.string "Pas de maman pour le petit-déj.\n"
	.string "Juste toi, ton loyer et tes rêves.\p"
	.string "2 500 ¥ par mois, tant que tu n'as\n"
	.string "pas de maison à toi. Bienvenue chez\l"
	.string "les adultes.$"

Tycoon_Text_Horloge:
	.string "L'horloge murale s'est arrêtée.\n"
	.string "Comme ta vie sociale.$"

Tycoon_Text_Terminal:
	.string "Le terminal sur le bureau s'allume :\n"
	.string "“Profil de départ à confirmer.”$"

Tycoon_Text_QuelProfil:
	.string "Quel profil pour démarrer ?$"

Tycoon_Text_Epargnant:
	.string "Épargnant$"
Tycoon_Text_Trader:
	.string "Trader$"
Tycoon_Text_Espoir:
	.string "Espoir Sportif$"

Tycoon_Text_ExpliqueEpargnant:
	.string "Épargnant : 12 000 ¥ sur le compte.\n"
	.string "Le plus gros matelas pour démarrer.\p"
	.string "Choisir Épargnant ?$"

Tycoon_Text_ExpliqueTrader:
	.string "Trader : 8 000 ¥ et un compte à la\n"
	.string "Bourse de Hoenn. La vente à découvert\l"
	.string "s'ouvrira pour toi en premier.\p"
	.string "Choisir Trader ?$"

Tycoon_Text_ExpliqueEspoir:
	.string "Espoir Sportif : 6 000 ¥ et trois\n"
	.string "Bonbons Exp. S. Les clubs aiment\l"
	.string "les jeunes qui en veulent.\p"
	.string "Choisir Espoir Sportif ?$"

Tycoon_Text_PassCourrier:
	.string "Une enveloppe glissée sous la porte :\n"
	.string "ton pass du Couronnement Mondial.\p"
	.string "Et le PokéNav que tu as payé en trois\n"
	.string "fois sans frais.$"

Tycoon_Text_Dehors:
	.string "Menu L : taxis, Bourse, immobilier,\n"
	.string "clubs. Le monde est à toi.\p"
	.string "Il paraît qu'un professeur crie\n"
	.string "souvent sur la Route 101...$"

Tycoon_Text_SekoPokedex:
	.string "SEKO : Tu m'as sauvé la mise. Garde\n"
	.string "ce Pokémon, il t'a choisi.\p"
	.string "Et prends ça : un Pokédex. Pas de\n"
	.string "devoirs, remplis-le à ton rythme.$"

Tycoon_Text_SekoAuRevoir:
	.string "SEKO : Les clubs de la Ligue cherchent\n"
	.string "des recrues. Passe à un guichet de\l"
	.string "ligue dans un Centre Pokémon !$"

@ Sièges des clubs (v0.48) : le champion d'arène est le président du club de sa ville.
Tycoon_Text_President:
	.string "Bienvenue au siège du club ! Ici,\n"
	.string "plus d'arène : on gère une équipe.\p"
	.string "Tu es des nôtres maintenant. Voici\n"
	.string "ton accréditation et de quoi bosser.$"

Tycoon_Text_PresidentNorman:
	.string "NORMAN : Le siège de Clémenti, c'est\n"
	.string "moi qui le dirige. Pas de combat.\p"
	.string "Tu as fait ton chemin. Voici ton\n"
	.string "accréditation, et le reste avec.$"
