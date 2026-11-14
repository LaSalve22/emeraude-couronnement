@ Émeraude Couronnement : Couronnement Mondial (pass, statut, tournois des villes d'Arène).
@ Logique et données : src/couronnement.c ; adversaires : tools/couronnement/tournois.py.

@ ------------------------------------------------------------------ Menu Start « MONDIAL »
Couronnement_EventScript_Statut::
	lockall
	call Couronnement_EventScript_AfficherRang
	releaseall
	end

@ Rang, palier (avec l'icône de la Ball du palier), Elo, tournois gagnés. Utilisé par MONDIAL et le menu L.
Couronnement_EventScript_AfficherRang::
	special Couronnement_BufferStatut
	special Couronnement_AfficherBallePalier
	msgbox gCouronnementTexteEquipe, MSGBOX_DEFAULT
	msgbox Couronnement_Text_TournoisGagnes, MSGBOX_DEFAULT
	special Couronnement_EstChampionDuMonde
	call_if_eq VAR_RESULT, TRUE, Couronnement_EventScript_TitreChampion
	special Couronnement_CacherBallePalier
	closemessage
	return

Couronnement_EventScript_TitreChampion::
	msgbox Couronnement_Text_TitreChampion, MSGBOX_DEFAULT
	return

@ ------------------------------------------------------------------ Pass remis par le rival (labo du Prof. Seko)
Couronnement_EventScript_RivalDonnePass::
	msgbox Couronnement_Text_RivalPass, MSGBOX_DEFAULT
	playfanfare MUS_OBTAIN_ITEM
	message Couronnement_Text_PassRecu
	waitfanfare
	special Couronnement_DonnerPass
	msgbox Couronnement_Text_RivalPassExplique, MSGBOX_DEFAULT
	call Couronnement_EventScript_DonnerRepousseInfinie
	return

@ Repousse Infinie (objet clé) : donnée avec le pass ; l'herboriste de Clémenti la donne aux sauvegardes plus anciennes.
Couronnement_EventScript_DonnerRepousseInfinie::
	checkitem ITEM_REPOUSSE_INFINIE
	goto_if_eq VAR_RESULT, TRUE, Couronnement_EventScript_DonnerRepousseInfinieFin
	msgbox Couronnement_Text_RepousseInfinie, MSGBOX_DEFAULT
	giveitem ITEM_REPOUSSE_INFINIE
	msgbox Couronnement_Text_RepousseInfinieExplique, MSGBOX_DEFAULT
Couronnement_EventScript_DonnerRepousseInfinieFin::
	return

@ ------------------------------------------------------------------ Hôtesse des tournois (villes et salles de Concours)
@ Entrée : VAR_0x8004 = numéro du tournoi (COURONNEMENT_TOURNOI_*).
@ À chaque manche : l'adversaire montre 6 Pokémon, le joueur en choisit 4, l'adversaire garde ses 4 meilleurs.
Couronnement_EventScript_Hotesse::
	lock
	faceplayer
	special Couronnement_EtatTournoi
	goto_if_eq VAR_RESULT, TOURNOI_PAS_DE_PASS, Couronnement_EventScript_PasDePass
	goto_if_eq VAR_RESULT, TOURNOI_BADGE_MANQUANT, Couronnement_EventScript_BadgeManquant
	goto_if_eq VAR_RESULT, TOURNOI_DEJA_GAGNE, Couronnement_EventScript_DejaGagne
	msgbox Couronnement_Text_Presentation, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, Couronnement_EventScript_Refus
Couronnement_EventScript_Inscription::
	special Couronnement_CompterPokemonAptes
	goto_if_lt VAR_RESULT, COURONNEMENT_TAILLE_EQUIPE, Couronnement_EventScript_PasAssez
	special Couronnement_DebutTournoi
	special SavePlayerParty
	setflag FLAG_COURONNEMENT_EN_COURS
	msgbox Couronnement_Text_Cest_Parti, MSGBOX_DEFAULT
Couronnement_EventScript_Manche::
	special LoadPlayerParty
	special HealPlayerParty
	special Couronnement_PreparerManche
	msgbox Couronnement_Text_Manche, MSGBOX_DEFAULT
	msgbox gCouronnementTexteEquipe, MSGBOX_DEFAULT
	msgbox Couronnement_Text_ChoisisQuatre, MSGBOX_DEFAULT
	closemessage
	fadescreen FADE_TO_BLACK
	special ChooseCouronnementParty
	waitstate
	setvar VAR_FRONTIER_FACILITY, 0
	goto_if_eq VAR_RESULT, 0, Couronnement_EventScript_Abandon
	special ReducePlayerPartyToSelectedMons
	special Couronnement_ChoixAdverse
	trainerbattle_no_intro TRAINER_COURONNEMENT, Couronnement_Text_AdversaireBattu
	cleartrainerflag TRAINER_COURONNEMENT
	special Couronnement_EnregistrerResultat
	goto_if_eq VAR_RESULT, FALSE, Couronnement_EventScript_Elimine
	msgbox Couronnement_Text_MancheGagnee, MSGBOX_DEFAULT
	special Couronnement_MancheSuivante
	goto_if_eq VAR_RESULT, TRUE, Couronnement_EventScript_Manche
	call Couronnement_EventScript_Fin
	special Couronnement_TournoiGagne
	goto_if_eq VAR_0x8006, ITEM_NONE, Couronnement_EventScript_GagneSansRecompense
	msgbox Couronnement_Text_TournoiGagne, MSGBOX_DEFAULT
	giveitem VAR_0x8006
	release
	end

Couronnement_EventScript_GagneSansRecompense::
	msgbox Couronnement_Text_RegagneElo, MSGBOX_DEFAULT
	release
	end

Couronnement_EventScript_Abandon::
	call Couronnement_EventScript_Fin
	msgbox Couronnement_Text_Abandon, MSGBOX_DEFAULT
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
	special Couronnement_FinTournoi
	special LoadPlayerParty
	special HealPlayerParty
	return

Couronnement_EventScript_Annule::
	special Couronnement_FinTournoi
	special LoadPlayerParty
	msgbox Couronnement_Text_Revenir, MSGBOX_DEFAULT
	release
	end

@ ------------------------------------------------------------------ Finale à 32 (Tour de Combat, après la Ligue)
@ 4 Pokémon choisis une fois pour les 5 tours ; soin seulement avant la demi-finale (Blue), puis Red en finale.
Couronnement_EventScript_HotesseFinale::
	lock
	faceplayer
	setvar VAR_0x8004, COURONNEMENT_TOURNOI_FINALE
	special Couronnement_EtatTournoi
	goto_if_eq VAR_RESULT, TOURNOI_PAS_DE_PASS, Couronnement_EventScript_PasDePass
	goto_if_eq VAR_RESULT, TOURNOI_LIGUE_MANQUANTE, Couronnement_EventScript_LigueManquante
	goto_if_eq VAR_RESULT, TOURNOI_PAS_TOP32, Couronnement_EventScript_PasTop32
	goto_if_eq VAR_RESULT, TOURNOI_DEJA_GAGNE, Couronnement_EventScript_FinaleRejouer
	msgbox Couronnement_Text_FinalePresentation, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, Couronnement_EventScript_Refus
Couronnement_EventScript_FinaleInscription::
	special Couronnement_CompterPokemonAptes
	goto_if_lt VAR_RESULT, COURONNEMENT_TAILLE_EQUIPE, Couronnement_EventScript_PasAssez
	msgbox Couronnement_Text_FinaleChoisis, MSGBOX_DEFAULT
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
	special HealPlayerParty
	msgbox Couronnement_Text_FinaleCestParti, MSGBOX_DEFAULT
Couronnement_EventScript_FinaleManche::
	special Couronnement_PreparerManche
	copyvar VAR_0x8008, VAR_RESULT
	goto_if_ne VAR_0x8008, COURONNEMENT_DEMI_FINALE, Couronnement_EventScript_FinaleAnnonce
	msgbox Couronnement_Text_FinaleSoin, MSGBOX_DEFAULT
	special HealPlayerParty
	playfanfare MUS_HEAL
	waitfanfare
	msgbox Couronnement_Text_FinaleBlue, MSGBOX_DEFAULT
Couronnement_EventScript_FinaleAnnonce::
	goto_if_ne VAR_0x8008, 4, Couronnement_EventScript_FinaleTour
	msgbox Couronnement_Text_FinaleRed, MSGBOX_DEFAULT
Couronnement_EventScript_FinaleTour::
	msgbox Couronnement_Text_FinaleTour, MSGBOX_DEFAULT
	msgbox gCouronnementTexteEquipe, MSGBOX_DEFAULT
	closemessage
	special Couronnement_ChoixAdverse
	trainerbattle_no_intro TRAINER_COURONNEMENT, Couronnement_Text_FinaleBattu
	cleartrainerflag TRAINER_COURONNEMENT
	special Couronnement_EnregistrerResultat
	goto_if_eq VAR_RESULT, FALSE, Couronnement_EventScript_Elimine
	msgbox Couronnement_Text_MancheGagnee, MSGBOX_DEFAULT
	special Couronnement_MancheSuivante
	goto_if_eq VAR_RESULT, TRUE, Couronnement_EventScript_FinaleManche
	call Couronnement_EventScript_Fin
	special Couronnement_TournoiGagne
	playfanfare MUS_OBTAIN_SYMBOL
	msgbox Couronnement_Text_ChampionDuMonde, MSGBOX_DEFAULT
	waitfanfare
	goto_if_eq VAR_0x8006, ITEM_NONE, Couronnement_EventScript_GagneSansRecompense
	giveitem VAR_0x8006
	giveitem ITEM_MASTER_BALL
	release
	end

Couronnement_EventScript_FinaleRejouer::
	msgbox Couronnement_Text_FinaleRejouer, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, Couronnement_EventScript_Refus
	goto Couronnement_EventScript_FinaleInscription

Couronnement_EventScript_LigueManquante::
	msgbox Couronnement_Text_LigueManquante, MSGBOX_DEFAULT
	release
	end

Couronnement_EventScript_PasTop32::
	msgbox Couronnement_Text_PasTop32, MSGBOX_DEFAULT
	release
	end

Couronnement_EventScript_PasDePass::
	goto_if_set FLAG_RECEIVED_POKEDEX_FROM_BIRCH, Couronnement_EventScript_PassDeRattrapage
	msgbox Couronnement_Text_PasDePass, MSGBOX_DEFAULT
	release
	end

@ Partie commencée avant la v0.17 : le labo est passé, l'hôtesse remet le pass.
Couronnement_EventScript_PassDeRattrapage::
	msgbox Couronnement_Text_PassOublie, MSGBOX_DEFAULT
	playfanfare MUS_OBTAIN_ITEM
	message Couronnement_Text_PassRecu
	waitfanfare
	special Couronnement_DonnerPass
	msgbox Couronnement_Text_PassMenu, MSGBOX_DEFAULT
	release
	end

Couronnement_EventScript_BadgeManquant::
	msgbox Couronnement_Text_BadgeManquant, MSGBOX_DEFAULT
	release
	end

Couronnement_EventScript_DejaGagne::
	msgbox Couronnement_Text_DejaGagne, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, Couronnement_EventScript_Refus
	goto Couronnement_EventScript_Inscription

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
@ Salles de Concours (Tentes de Combat et salle de Nénucrique).
Couronnement_EventScript_HotessePoivressel::
	setvar VAR_0x8004, COURONNEMENT_TOURNOI_POIVRESSEL
	goto Couronnement_EventScript_Hotesse
Couronnement_EventScript_HotesseVergazon::
	setvar VAR_0x8004, COURONNEMENT_TOURNOI_VERGAZON
	goto Couronnement_EventScript_Hotesse
Couronnement_EventScript_HotesseAutequia::
	setvar VAR_0x8004, COURONNEMENT_TOURNOI_AUTEQUIA
	goto Couronnement_EventScript_Hotesse
Couronnement_EventScript_HotesseNenucrique::
	setvar VAR_0x8004, COURONNEMENT_TOURNOI_NENUCRIQUE
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
	.string "Ton adversaire montre 6 Pokémon,\l"
	.string "tu en choisis 4, un seul légendaire.\p"
	.string "On soigne entre chaque combat.\n"
	.string "Une défaite et c'est fini.\p"
	.string "Chaque combat fait bouger ton Elo.\n"
	.string "Tu t'inscris ?$"

Couronnement_Text_ChoisisQuatre:
	.string "Choisis tes 4 Pokémon pour ce\n"
	.string "combat.$"

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
	.string "Ton nom est gravé au tableau !\p"
	.string "Tu peux le rejouer pour ton Elo,\n"
	.string "sans récompense. On y va ?$"

Couronnement_Text_PassOublie:
	.string "Ton rival t'a inscrit au Couronnement\n"
	.string "Mondial, mais ton pass est resté ici.\l"
	.string "Tiens, le voilà.$"

Couronnement_Text_PassMenu:
	.string "Ton classement est dans le menu,\n"
	.string "ligne MONDIAL. Reviens me parler\l"
	.string "pour t'inscrire au tournoi.$"

Couronnement_Text_PasAssez:
	.string "Il te faut au moins 4 Pokémon\n"
	.string "pour t'inscrire.$"

Couronnement_Text_TitreChampion:
	.string "Titre : Champion du Monde.\n"
	.string "Rien que ça.$"

Couronnement_Text_RegagneElo:
	.string "Encore gagné ! Pas de récompense\n"
	.string "cette fois, mais ton Elo a monté.$"

Couronnement_Text_Abandon:
	.string "Tu déclares forfait. Ton Elo ne\n"
	.string "bouge pas… mais ta fierté, si.$"

Couronnement_Text_LigueManquante:
	.string "Ici se joue la finale du\n"
	.string "Couronnement Mondial.\p"
	.string "Reviens quand tu auras battu la\n"
	.string "Ligue Pokémon.$"

Couronnement_Text_PasTop32:
	.string "La finale réunit les 32 meilleurs\n"
	.string "Dresseurs du monde.\p"
	.string "Tu es au rang {STR_VAR_1}. Remonte\n"
	.string "dans le Top 32 : rejoue les\l"
	.string "tournois pour gagner de l'Elo.$"

Couronnement_Text_FinalePresentation:
	.string "Bienvenue à la finale du\n"
	.string "Couronnement Mondial !\p"
	.string "32 Dresseurs, cinq tours en double.\n"
	.string "Tu choisis 4 Pokémon pour toute\l"
	.string "la finale, un seul légendaire.\p"
	.string "Pas de soin entre les tours, sauf\n"
	.string "avant la demi-finale.\p"
	.string "On commence ?$"

Couronnement_Text_FinaleChoisis:
	.string "Choisis tes 4 Pokémon. Ils\n"
	.string "joueront les cinq tours.$"

Couronnement_Text_FinaleCestParti:
	.string "Le public est en délire. Ne\n"
	.string "te chie pas dessus en direct.$"

Couronnement_Text_FinaleTour:
	.string "Tour {STR_VAR_2} sur 5 :\n"
	.string "{STR_VAR_1}, Elo {STR_VAR_3}.$"

Couronnement_Text_FinaleSoin:
	.string "Demi-finale ! Tes Pokémon sont\n"
	.string "soignés une dernière fois.$"

Couronnement_Text_FinaleBlue:
	.string "Blue : Tiens, tiens. La recrue\n"
	.string "d'Hoenn.\p"
	.string "J'ai été Champion avant que tu\n"
	.string "saches marcher. Montre-moi si tu\l"
	.string "mérites la finale.$"

Couronnement_Text_FinaleRed:
	.string "…\p"
	.string "Red te fixe sans un mot.\n"
	.string "La finale commence.$"

Couronnement_Text_FinaleBattu:
	.string "… Bien joué.$"

Couronnement_Text_ChampionDuMonde:
	.string "{PLAYER} remporte le Couronnement\n"
	.string "Mondial !\p"
	.string "Rang 1. Champion du Monde.\n"
	.string "Le monde entier connaît ton nom.$"

Couronnement_Text_FinaleRejouer:
	.string "Le Champion du Monde revient\n"
	.string "défendre son titre ?\p"
	.string "Pas de récompense, mais ton Elo\n"
	.string "est en jeu. On y va ?$"

Couronnement_Text_RepousseInfinie:
	.string "Ah, et prends ça. Les Pokémon\n"
	.string "sauvages qui te sautent dessus tous\l"
	.string "les trois pas, ça va cinq minutes.$"

Couronnement_Text_RepousseInfinieExplique:
	.string "La Repousse Infinie coupe les\n"
	.string "rencontres, et les remet quand tu\l"
	.string "veux. Mets-la sur SELECT.$"

@ ------------------------------------------------------------------ Menu L (terrain), fonctions débloquées au fil du jeu
@ Repousse Infinie et Course : dès le début ; PC : 1er badge ; Rang mondial : pass ; Type Téra : Orbe Téracristal ;
@ Soin : 8e badge ; Coach EV / IV : après la Ligue ; Bourse de Hoenn, Immobilier, Ma société et Mes commerces : avec le pass. Soin, PC, EV et Téra sont masqués pendant un tournoi du Couronnement.
Couronnement_EventScript_MenuL::
	lockall
Couronnement_EventScript_MenuLBoucle::
	special Couronnement_BufferMenuL
	message Couronnement_Text_MenuL
	waitmessage
	goto_if_set FLAG_COURONNEMENT_EN_COURS, Couronnement_EventScript_MenuLRepousseCourse
	goto_if_unset FLAG_BADGE08_GET, Couronnement_EventScript_MenuLPC
	dynmultipush Couronnement_Text_MenuLSoin, 0
Couronnement_EventScript_MenuLPC::
	goto_if_unset FLAG_BADGE01_GET, Couronnement_EventScript_MenuLRepousseCourse
	dynmultipush Couronnement_Text_MenuLPC, 1
Couronnement_EventScript_MenuLRepousseCourse::
	dynmultipush gStringVar1, 2
	dynmultipush gStringVar2, 3
	goto_if_set FLAG_COURONNEMENT_EN_COURS, Couronnement_EventScript_MenuLRang
	goto_if_unset FLAG_SYS_GAME_CLEAR, Couronnement_EventScript_MenuLTera
	dynmultipush Couronnement_Text_MenuLEV, 4
Couronnement_EventScript_MenuLTera::
	checkitem ITEM_TERA_ORB
	goto_if_eq VAR_RESULT, FALSE, Couronnement_EventScript_MenuLRang
	dynmultipush Couronnement_Text_MenuLTera, 5
Couronnement_EventScript_MenuLRang::
	goto_if_unset FLAG_COURONNEMENT_PASS, Couronnement_EventScript_MenuLFermer
	dynmultipush Couronnement_Text_MenuLRang, 6
	dynmultipush Couronnement_Text_MenuLBourse, 8
	dynmultipush Couronnement_Text_MenuLImmo, 9
	dynmultipush Couronnement_Text_MenuLSociete, 10
	dynmultipush Couronnement_Text_MenuLCommerces, 11
Couronnement_EventScript_MenuLFermer::
	dynmultipush Couronnement_Text_MenuLFermer, 7
	dynmultistack 0, 0, FALSE, 8, FALSE, 0, DYN_MULTICHOICE_CB_NONE
	switch VAR_RESULT
	case 0, Couronnement_EventScript_MenuLSoin
	case 1, Couronnement_EventScript_MenuLOuvrirPC
	case 2, Couronnement_EventScript_MenuLRepousse
	case 3, Couronnement_EventScript_MenuLCourse
	case 4, Couronnement_EventScript_MenuLEV
	case 5, Couronnement_EventScript_MenuLChoisirTera
	case 6, Couronnement_EventScript_MenuLAfficherRang
	case 8, Couronnement_EventScript_MenuLBourse
	case 9, Couronnement_EventScript_MenuLImmo
	case 10, Couronnement_EventScript_MenuLSociete
	case 11, Couronnement_EventScript_MenuLCommerces
	closemessage
	releaseall
	end

Couronnement_EventScript_MenuLSoin::
	special HealPlayerParty
	playse SE_USE_ITEM
	msgbox Couronnement_Text_MenuLSoigne, MSGBOX_DEFAULT
	goto Couronnement_EventScript_MenuLBoucle

Couronnement_EventScript_MenuLOuvrirPC::
	playse SE_PC_LOGIN
	special ShowPokemonStorageSystemPC
	goto Couronnement_EventScript_MenuLBoucle

Couronnement_EventScript_MenuLRepousse::
	goto_if_set FLAG_REPOUSSE_INFINIE, Couronnement_EventScript_MenuLRepousseOff
	setflag FLAG_REPOUSSE_INFINIE
	playse SE_REPEL
	goto Couronnement_EventScript_MenuLBoucle
Couronnement_EventScript_MenuLRepousseOff::
	clearflag FLAG_REPOUSSE_INFINIE
	playse SE_PC_OFF
	goto Couronnement_EventScript_MenuLBoucle

Couronnement_EventScript_MenuLCourse::
	goto_if_set FLAG_COURSE_AUTO, Couronnement_EventScript_MenuLCourseOff
	setflag FLAG_COURSE_AUTO
	playse SE_SELECT
	goto Couronnement_EventScript_MenuLBoucle
Couronnement_EventScript_MenuLCourseOff::
	clearflag FLAG_COURSE_AUTO
	playse SE_SELECT
	goto Couronnement_EventScript_MenuLBoucle

Couronnement_EventScript_MenuLEV::
	call CoachEV_EventScript_Choix
	goto Couronnement_EventScript_MenuLBoucle

Couronnement_EventScript_MenuLCommerces::
	call Commerce_EventScript_Appli
	goto Couronnement_EventScript_MenuLBoucle

Couronnement_EventScript_MenuLSociete::
	call Societe_EventScript_Appli
	goto Couronnement_EventScript_MenuLBoucle

Couronnement_EventScript_MenuLImmo::
	call Immo_EventScript_Appli
	goto Couronnement_EventScript_MenuLBoucle

Couronnement_EventScript_MenuLBourse::
	call Bourse_EventScript_Appli
	goto Couronnement_EventScript_MenuLBoucle

Couronnement_EventScript_MenuLAfficherRang::
	call Couronnement_EventScript_AfficherRang
	goto Couronnement_EventScript_MenuLBoucle

Couronnement_EventScript_MenuLChoisirTera::
	msgbox Couronnement_Text_MenuLQuelPokemon, MSGBOX_DEFAULT
	special ChoosePartyMon
	waitstate
	goto_if_eq VAR_0x8004, PARTY_NOTHING_CHOSEN, Couronnement_EventScript_MenuLBoucle
	specialvar VAR_RESULT, ScriptGetPartyMonSpecies
	goto_if_eq VAR_RESULT, SPECIES_EGG, Couronnement_EventScript_MenuLBoucle
	message Couronnement_Text_MenuLQuelType
	waitmessage
	dynmultichoice 0, 0, FALSE, 7, 0, DYN_MULTICHOICE_CB_NONE, Couronnement_Text_TypeNormal, Couronnement_Text_TypeFeu, Couronnement_Text_TypeEau, Couronnement_Text_TypePlante, Couronnement_Text_TypeElectrik, Couronnement_Text_TypeGlace, Couronnement_Text_TypeCombat, Couronnement_Text_TypePoison, Couronnement_Text_TypeSol, Couronnement_Text_TypeVol, Couronnement_Text_TypePsy, Couronnement_Text_TypeInsecte, Couronnement_Text_TypeRoche, Couronnement_Text_TypeSpectre, Couronnement_Text_TypeDragon, Couronnement_Text_TypeTenebres, Couronnement_Text_TypeAcier, Couronnement_Text_TypeFee
	goto_if_eq VAR_RESULT, MULTI_B_PRESSED, Couronnement_EventScript_MenuLBoucle
	copyvar VAR_0x8005, VAR_RESULT
	special Couronnement_ChangerTypeTera
	playse SE_SUCCESS
	msgbox Couronnement_Text_MenuLTeraChange, MSGBOX_DEFAULT
	goto Couronnement_EventScript_MenuLBoucle

Couronnement_Text_MenuL:
	.string "Réglages rapides (L).$"
Couronnement_Text_MenuLSoin:
	.string "Soigner l'équipe$"
Couronnement_Text_MenuLPC:
	.string "PC de stockage$"
Couronnement_Text_MenuLCommerces:
	.string "Mes commerces$"
Couronnement_Text_MenuLSociete:
	.string "Ma société$"
Couronnement_Text_MenuLImmo:
	.string "Immobilier$"
Couronnement_Text_MenuLBourse:
	.string "Bourse de Hoenn$"
Couronnement_Text_MenuLEV:
	.string "Coach EV / IV$"
Couronnement_Text_MenuLTera:
	.string "Type Téra$"
Couronnement_Text_MenuLRang:
	.string "Rang mondial$"
Couronnement_Text_MenuLFermer:
	.string "Fermer$"
Couronnement_Text_MenuLSoigne:
	.string "Ton équipe est en pleine forme.$"
Couronnement_Text_MenuLQuelPokemon:
	.string "Quel Pokémon change de type Téra ?$"
Couronnement_Text_MenuLQuelType:
	.string "Quel type Téra ?$"
Couronnement_Text_MenuLTeraChange:
	.string "Type Téra changé.$"
Couronnement_Text_TypeNormal:
	.string "Normal$"
Couronnement_Text_TypeFeu:
	.string "Feu$"
Couronnement_Text_TypeEau:
	.string "Eau$"
Couronnement_Text_TypePlante:
	.string "Plante$"
Couronnement_Text_TypeElectrik:
	.string "Électrik$"
Couronnement_Text_TypeGlace:
	.string "Glace$"
Couronnement_Text_TypeCombat:
	.string "Combat$"
Couronnement_Text_TypePoison:
	.string "Poison$"
Couronnement_Text_TypeSol:
	.string "Sol$"
Couronnement_Text_TypeVol:
	.string "Vol$"
Couronnement_Text_TypePsy:
	.string "Psy$"
Couronnement_Text_TypeInsecte:
	.string "Insecte$"
Couronnement_Text_TypeRoche:
	.string "Roche$"
Couronnement_Text_TypeSpectre:
	.string "Spectre$"
Couronnement_Text_TypeDragon:
	.string "Dragon$"
Couronnement_Text_TypeTenebres:
	.string "Ténèbres$"
Couronnement_Text_TypeAcier:
	.string "Acier$"
Couronnement_Text_TypeFee:
	.string "Fée$"

