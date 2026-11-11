@ Émeraude Couronnement : Couronnement Mondial (pass, statut, tournois des villes d'Arène).
@ Logique et données : src/couronnement.c ; adversaires : tools/couronnement/tournois.py.

@ ------------------------------------------------------------------ Menu Start « MONDIAL »
Couronnement_EventScript_Statut::
	lockall
	special Couronnement_BufferStatut
	msgbox gCouronnementTexteEquipe, MSGBOX_DEFAULT
	msgbox Couronnement_Text_TournoisGagnes, MSGBOX_DEFAULT
	releaseall
	end

@ ------------------------------------------------------------------ Pass remis par le rival (labo du Prof. Seko)
Couronnement_EventScript_RivalDonnePass::
	msgbox Couronnement_Text_RivalPass, MSGBOX_DEFAULT
	playfanfare MUS_OBTAIN_ITEM
	message Couronnement_Text_PassRecu
	waitfanfare
	special Couronnement_DonnerPass
	msgbox Couronnement_Text_RivalPassExplique, MSGBOX_DEFAULT
	return

@ ------------------------------------------------------------------ Hôtesse des tournois (Centres Pokémon)
@ Entrée : VAR_0x8004 = numéro du tournoi (COURONNEMENT_TOURNOI_*).
Couronnement_EventScript_Hotesse::
	lock
	faceplayer
	special Couronnement_EtatTournoi
	goto_if_eq VAR_RESULT, TOURNOI_PAS_DE_PASS, Couronnement_EventScript_PasDePass
	goto_if_eq VAR_RESULT, TOURNOI_BADGE_MANQUANT, Couronnement_EventScript_BadgeManquant
	goto_if_eq VAR_RESULT, TOURNOI_DEJA_GAGNE, Couronnement_EventScript_DejaGagne
	msgbox Couronnement_Text_Presentation, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, Couronnement_EventScript_Refus
	special Couronnement_CompterPokemonAptes
	goto_if_lt VAR_RESULT, COURONNEMENT_TAILLE_EQUIPE, Couronnement_EventScript_PasAssez
	msgbox Couronnement_Text_ChoisisQuatre, MSGBOX_DEFAULT
	closemessage
	special Couronnement_DebutTournoi
	special SavePlayerParty
	fadescreen FADE_TO_BLACK
	special ChooseCouronnementParty
	waitstate
	setvar VAR_FRONTIER_FACILITY, 0
	goto_if_eq VAR_RESULT, 0, Couronnement_EventScript_Annule
	special ReducePlayerPartyToSelectedMons
	setflag FLAG_COURONNEMENT_EN_COURS
	msgbox Couronnement_Text_Cest_Parti, MSGBOX_DEFAULT
Couronnement_EventScript_Manche::
	special HealPlayerParty
	special Couronnement_PreparerManche
	msgbox Couronnement_Text_Manche, MSGBOX_DEFAULT
	msgbox gCouronnementTexteEquipe, MSGBOX_DEFAULT
	closemessage
	trainerbattle_no_intro TRAINER_COURONNEMENT, Couronnement_Text_AdversaireBattu
	cleartrainerflag TRAINER_COURONNEMENT
	special Couronnement_EnregistrerResultat
	goto_if_eq VAR_RESULT, FALSE, Couronnement_EventScript_Elimine
	msgbox Couronnement_Text_MancheGagnee, MSGBOX_DEFAULT
	special Couronnement_MancheSuivante
	goto_if_eq VAR_RESULT, TRUE, Couronnement_EventScript_Manche
	call Couronnement_EventScript_Fin
	special Couronnement_TournoiGagne
	msgbox Couronnement_Text_TournoiGagne, MSGBOX_DEFAULT
	giveitem VAR_0x8006
	release
	end

Couronnement_EventScript_Elimine::
	msgbox Couronnement_Text_Elimine, MSGBOX_DEFAULT
	call Couronnement_EventScript_Fin
	msgbox Couronnement_Text_Revenir, MSGBOX_DEFAULT
	release
	end

Couronnement_EventScript_Fin::
	clearflag FLAG_COURONNEMENT_EN_COURS
	special LoadPlayerParty
	special HealPlayerParty
	return

Couronnement_EventScript_Annule::
	special LoadPlayerParty
	msgbox Couronnement_Text_Revenir, MSGBOX_DEFAULT
	release
	end

Couronnement_EventScript_PasDePass::
	msgbox Couronnement_Text_PasDePass, MSGBOX_DEFAULT
	release
	end

Couronnement_EventScript_BadgeManquant::
	msgbox Couronnement_Text_BadgeManquant, MSGBOX_DEFAULT
	release
	end

Couronnement_EventScript_DejaGagne::
	msgbox Couronnement_Text_DejaGagne, MSGBOX_DEFAULT
	release
	end

Couronnement_EventScript_Refus::
	msgbox Couronnement_Text_Revenir, MSGBOX_DEFAULT
	release
	end

Couronnement_EventScript_PasAssez::
	msgbox Couronnement_Text_PasAssez, MSGBOX_DEFAULT
	release
	end

@ Un appel par Centre Pokémon de ville d'Arène.
Couronnement_EventScript_HotesseMerouville::
	setvar VAR_0x8004, COURONNEMENT_TOURNOI_MEROUVILLE
	goto Couronnement_EventScript_Hotesse
Couronnement_EventScript_HotesseMyokara::
	setvar VAR_0x8004, COURONNEMENT_TOURNOI_MYOKARA
	goto Couronnement_EventScript_Hotesse
Couronnement_EventScript_HotesseLavandia::
	setvar VAR_0x8004, COURONNEMENT_TOURNOI_LAVANDIA
	goto Couronnement_EventScript_Hotesse
Couronnement_EventScript_HotesseVermilava::
	setvar VAR_0x8004, COURONNEMENT_TOURNOI_VERMILAVA
	goto Couronnement_EventScript_Hotesse
Couronnement_EventScript_HotesseClementi::
	setvar VAR_0x8004, COURONNEMENT_TOURNOI_CLEMENTI
	goto Couronnement_EventScript_Hotesse
Couronnement_EventScript_HotesseCimetronelle::
	setvar VAR_0x8004, COURONNEMENT_TOURNOI_CIMETRONELLE
	goto Couronnement_EventScript_Hotesse
Couronnement_EventScript_HotesseAlgatia::
	setvar VAR_0x8004, COURONNEMENT_TOURNOI_ALGATIA
	goto Couronnement_EventScript_Hotesse
Couronnement_EventScript_HotesseAtalanopolis::
	setvar VAR_0x8004, COURONNEMENT_TOURNOI_ATALANOPOLIS
	goto Couronnement_EventScript_Hotesse

@ ------------------------------------------------------------------ Textes
Couronnement_Text_TournoisGagnes:
	.string "Tournois remportés : {STR_VAR_1} / 12.$"

Couronnement_Text_RivalPass:
	.string "{RIVAL} : Ah, attends ! J'ai failli\n"
	.string "oublier le plus important.\p"
	.string "Papa m'a donné deux pass pour le\n"
	.string "Couronnement Mondial. Tiens, le tien.$"

Couronnement_Text_PassRecu:
	.string "{PLAYER} reçoit le Pass Couronnement !$"

Couronnement_Text_RivalPassExplique:
	.string "C'est le classement mondial des\n"
	.string "Dresseurs. Tout le monde part à\l"
	.string "1500 d'Elo, rang 10 000.\p"
	.string "Les Centres Pokémon des villes\n"
	.string "d'Arène organisent des tournois :\l"
	.string "trois combats en double, quatre\l"
	.string "Pokémon choisis, zéro droit à l'erreur.\p"
	.string "Les 32 meilleurs deviennent\n"
	.string "Champions et disputent la finale\l"
	.string "à la Tour de Combat.\p"
	.string "Ton classement est dans le menu,\n"
	.string "ligne MONDIAL. On se retrouve au\l"
	.string "sommet. Enfin… moi, en tout cas.$"

Couronnement_Text_Presentation:
	.string "Bienvenue au tournoi du\n"
	.string "Couronnement Mondial !\p"
	.string "Trois combats en double à la suite.\n"
	.string "Tu choisis 4 Pokémon, un seul\l"
	.string "légendaire. On soigne entre chaque\l"
	.string "combat. Une défaite et c'est fini.\p"
	.string "Chaque combat fait bouger ton Elo.\n"
	.string "Tu t'inscris ?$"

Couronnement_Text_ChoisisQuatre:
	.string "Choisis tes 4 Pokémon.$"

Couronnement_Text_Cest_Parti:
	.string "Inscription validée. Que le meilleur\n"
	.string "gagne… et que l'autre pleure.$"

Couronnement_Text_Manche:
	.string "Combat {STR_VAR_2} sur 3 :\n"
	.string "{STR_VAR_1}, Elo {STR_VAR_3}.$"

Couronnement_Text_AdversaireBattu:
	.string "Bien joué. Tu mérites ta place.$"

Couronnement_Text_MancheGagnee:
	.string "Victoire ! Elo {STR_VAR_1}.\n"
	.string "Nouvel Elo : {STR_VAR_2}.$"

Couronnement_Text_Elimine:
	.string "Élimination… Elo {STR_VAR_1}.\n"
	.string "Nouvel Elo : {STR_VAR_2}.\p"
	.string "Le classement ne pardonne pas.$"

Couronnement_Text_TournoiGagne:
	.string "Tu remportes le tournoi !\n"
	.string "Voici ta récompense.$"

Couronnement_Text_Revenir:
	.string "Reviens quand tu veux tenter ta\n"
	.string "chance.$"

Couronnement_Text_PasDePass:
	.string "Ce tournoi est réservé aux\n"
	.string "détenteurs du Pass Couronnement.$"

Couronnement_Text_BadgeManquant:
	.string "Le tournoi de cette ville s'ouvre\n"
	.string "quand tu as battu le Champion\l"
	.string "de l'Arène locale.$"

Couronnement_Text_DejaGagne:
	.string "Tu as déjà remporté ce tournoi.\n"
	.string "Ton nom est gravé au tableau !$"

Couronnement_Text_PasAssez:
	.string "Il te faut au moins 4 Pokémon\n"
	.string "pour t'inscrire.$"
