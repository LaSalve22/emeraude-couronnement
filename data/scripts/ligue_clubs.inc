@ Émeraude Couronnement : Ligue des Clubs (v0.42). Appli « Ligue des Clubs » du menu L (avec le pass), src/league_club.c,
@ tableau de bord src/club_dashboard.c. Le duel du joueur utilise l'emplacement TRAINER_COURONNEMENT.
@ v0.44 : le menu L ne sert plus qu'à consulter ; on joue, signe et gère son club aux guichets de ligue
@ (Centres Pokémon des 10 villes de club, Tour de Combat ; tools/couronnement/clubs_placer.py).
Club_EventScript_Appli::
	special Club_Ouvrir
	goto_if_eq VAR_RESULT, FALSE, Club_EventScript_Consultation
	playse SE_PC_LOGIN
	msgbox gStringVar4, MSGBOX_DEFAULT
Club_EventScript_Consultation::
	special Club_Resume
	message gStringVar4
	waitmessage
	dynmultichoice 0, 0, FALSE, 3, 0, DYN_MULTICHOICE_CB_NONE, Club_Text_Tableau, Club_Text_Regles, Club_Text_Fermer
	goto_if_eq VAR_RESULT, 0, Club_EventScript_TableauConsultation
	goto_if_eq VAR_RESULT, 1, Club_EventScript_ReglesConsultation
	goto Club_EventScript_Fin
Club_EventScript_TableauConsultation::
	closemessage
	special Club_TableauDeBord
	waitstate
	goto_if_ne VAR_RESULT, 1, Club_EventScript_Consultation
	msgbox Club_Text_AllerAuGuichet, MSGBOX_DEFAULT
	goto Club_EventScript_Consultation
Club_EventScript_ReglesConsultation::
	msgbox Club_Text_ReglesTexte, MSGBOX_DEFAULT
	goto Club_EventScript_Consultation

Club_EventScript_Guichet::
	lock
	faceplayer
	msgbox Club_Text_Bienvenue, MSGBOX_DEFAULT
	call Club_EventScript_AppliGuichet
	release
	end

Club_EventScript_AppliGuichet::
	special Club_Ouvrir
	goto_if_eq VAR_RESULT, FALSE, Club_EventScript_Accueil
	playse SE_PC_LOGIN
	msgbox gStringVar4, MSGBOX_DEFAULT
Club_EventScript_Accueil::
	special Club_Resume
	message gStringVar4
	waitmessage
	dynmultichoice 0, 0, FALSE, 6, 0, DYN_MULTICHOICE_CB_NONE, Club_Text_Tableau, Club_Text_Jouer, Club_Text_Licence, Club_Text_Propriete, Club_Text_Regles, Club_Text_Fermer
	switch VAR_RESULT
	case 0, Club_EventScript_Tableau
	case 1, Club_EventScript_Jouer
	case 2, Club_EventScript_Licence
	case 3, Club_EventScript_Propriete
	case 4, Club_EventScript_Regles
Club_EventScript_Fin::
	closemessage
	return

Club_EventScript_Tableau::
	closemessage
	special Club_TableauDeBord
	waitstate
	goto_if_eq VAR_RESULT, 1, Club_EventScript_Agrandir
	goto Club_EventScript_Accueil

Club_EventScript_Regles::
	msgbox Club_Text_ReglesTexte, MSGBOX_DEFAULT
	goto Club_EventScript_Accueil

Club_EventScript_Jouer::
	special Club_PreparerMatch
	goto_if_eq VAR_RESULT, FALSE, Club_EventScript_Info
	msgbox gStringVar4, MSGBOX_DEFAULT
	msgbox Club_Text_OnYVa, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, Club_EventScript_Accueil
	closemessage
	special Club_DebutMatch
	trainerbattle_no_intro TRAINER_COURONNEMENT, Club_Text_AdversaireBattu
	cleartrainerflag TRAINER_COURONNEMENT
	special Club_FinMatch
	goto_if_eq VAR_RESULT, FALSE, Club_EventScript_Defaite
	playfanfare MUS_OBTAIN_ITEM
	msgbox gStringVar4, MSGBOX_DEFAULT
	waitfanfare
	goto Club_EventScript_Accueil

Club_EventScript_Defaite::
	playse SE_BOO
	msgbox gStringVar4, MSGBOX_DEFAULT
	goto Club_EventScript_Accueil

Club_EventScript_Info::
	msgbox gStringVar4, MSGBOX_DEFAULT
	goto Club_EventScript_Accueil

Club_EventScript_Licence::
	message Club_Text_QuelClub
	waitmessage
	dynmultichoice 0, 0, FALSE, 6, 0, DYN_MULTICHOICE_CB_NONE, Club_Text_Club0, Club_Text_Club1, Club_Text_Club2, Club_Text_Club3, Club_Text_Club4, Club_Text_Club5, Club_Text_Club6, Club_Text_Club7, Club_Text_Club8, Club_Text_Club9, Club_Text_Retour
	goto_if_eq VAR_RESULT, MULTI_B_PRESSED, Club_EventScript_Accueil
	goto_if_eq VAR_RESULT, 10, Club_EventScript_Accueil
	copyvar VAR_0x8004, VAR_RESULT
	special Club_FicheLicence
	goto_if_eq VAR_RESULT, FALSE, Club_EventScript_Info
	msgbox gStringVar4, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, Club_EventScript_Licence
	special Club_ScriptSigner
	goto_if_eq VAR_RESULT, FALSE, Club_EventScript_Info
	playfanfare MUS_OBTAIN_ITEM
	msgbox gStringVar4, MSGBOX_DEFAULT
	waitfanfare
	goto Club_EventScript_Accueil

@ ---------------------------------------------------------------- phase 2 : propriétaire (src/league_club_finances.c)
Club_EventScript_Propriete::
	special Club_ScriptPropriete
	goto_if_eq VAR_RESULT, 2, Club_EventScript_Gestion
	goto_if_eq VAR_RESULT, 0, Club_EventScript_Info
	message Club_Text_QuelRachat
	waitmessage
	dynmultichoice 0, 0, FALSE, 6, 0, DYN_MULTICHOICE_CB_NONE, Club_Text_Club0, Club_Text_Club1, Club_Text_Club2, Club_Text_Club3, Club_Text_Club4, Club_Text_Club5, Club_Text_Club6, Club_Text_Club7, Club_Text_Club8, Club_Text_Club9, Club_Text_Retour
	goto_if_eq VAR_RESULT, MULTI_B_PRESSED, Club_EventScript_Accueil
	goto_if_eq VAR_RESULT, 10, Club_EventScript_Accueil
	copyvar VAR_0x8004, VAR_RESULT
	special Club_FicheRachat
	msgbox gStringVar4, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, Club_EventScript_Accueil
	special Club_ScriptRacheter
	goto_if_eq VAR_RESULT, FALSE, Club_EventScript_Info
	playfanfare MUS_OBTAIN_SYMBOL
	msgbox gStringVar4, MSGBOX_DEFAULT
	waitfanfare
Club_EventScript_Gestion::
	special Club_ResumeProprio
	message gStringVar4
	waitmessage
	dynmultichoice 0, 0, FALSE, 6, 0, DYN_MULTICHOICE_CB_NONE, Club_Text_Recruter, Club_Text_Coaching, Club_Text_Stade, Club_Text_Boutique, Club_Text_Revendre, Club_Text_Retour
	switch VAR_RESULT
	case 0, Club_EventScript_Recruter
	case 1, Club_EventScript_Coaching
	case 2, Club_EventScript_Agrandir
	case 3, Club_EventScript_Boutique
	case 4, Club_EventScript_Revendre
	goto Club_EventScript_Accueil

Club_EventScript_ChoisirPlace::
	message Club_Text_QuellePlace
	waitmessage
	dynmultichoice 0, 0, FALSE, 4, 0, DYN_MULTICHOICE_CB_NONE, Club_Text_Capitaine, Club_Text_Titulaire2, Club_Text_Titulaire3, Club_Text_Retour
	return

Club_EventScript_Recruter::
	call Club_EventScript_ChoisirPlace
	goto_if_eq VAR_RESULT, MULTI_B_PRESSED, Club_EventScript_Gestion
	goto_if_eq VAR_RESULT, 3, Club_EventScript_Gestion
	copyvar VAR_0x8005, VAR_RESULT
	special Club_PreparerVivier
	message Club_Text_QuelleRecrue
	waitmessage
	dynmultipush gClubLignes + 0, 0
	dynmultipush gClubLignes + 28, 1
	dynmultipush gClubLignes + 56, 2
	dynmultipush gClubLignes + 84, 3
	dynmultipush gClubLignes + 112, 4
	dynmultipush gClubLignes + 140, 5
	dynmultipush gClubLignes + 168, 6
	dynmultipush gClubLignes + 196, 7
	dynmultipush gClubLignes + 224, 8
	dynmultipush gClubLignes + 252, 9
	dynmultipush gClubLignes + 280, 10
	dynmultipush gClubLignes + 308, 11
	dynmultipush gClubLignes + 336, 12
	dynmultipush gClubLignes + 364, 13
	dynmultipush gClubLignes + 392, 14
	dynmultipush gClubLignes + 420, 15
	dynmultipush gClubLignes + 448, 16
	dynmultipush gClubLignes + 476, 17
	dynmultipush gClubLignes + 504, 18
	dynmultipush gClubLignes + 532, 19
	dynmultipush Club_Text_RendrePlace, 20
	dynmultipush Club_Text_Retour, 21
	dynmultistack 0, 0, FALSE, 6, FALSE, 0, DYN_MULTICHOICE_CB_NONE
	goto_if_eq VAR_RESULT, MULTI_B_PRESSED, Club_EventScript_Gestion
	goto_if_eq VAR_RESULT, 21, Club_EventScript_Gestion
	copyvar VAR_0x8006, VAR_RESULT
	special Club_ScriptRecruter
	goto Club_EventScript_ResultatGestion

Club_EventScript_Coaching::
	call Club_EventScript_ChoisirPlace
	goto_if_eq VAR_RESULT, MULTI_B_PRESSED, Club_EventScript_Gestion
	goto_if_eq VAR_RESULT, 3, Club_EventScript_Gestion
	copyvar VAR_0x8005, VAR_RESULT
	message Club_Text_QuelBonbon
	waitmessage
	dynmultichoice 0, 0, FALSE, 4, 0, DYN_MULTICHOICE_CB_NONE, Club_Text_BonbonL, Club_Text_BonbonXL, Club_Text_ObjetCombat, Club_Text_Retour
	goto_if_eq VAR_RESULT, MULTI_B_PRESSED, Club_EventScript_Gestion
	goto_if_eq VAR_RESULT, 3, Club_EventScript_Gestion
	goto_if_eq VAR_RESULT, 2, Club_EventScript_CoachingObjet
	copyvar VAR_0x8006, VAR_RESULT
	special Club_ScriptCoacher
	goto Club_EventScript_ResultatGestion

Club_EventScript_CoachingObjet::
	special Club_ScriptCoacherObjet
	goto Club_EventScript_ResultatGestion

Club_EventScript_Agrandir::
	special Club_ScriptPropriete
	goto_if_ne VAR_RESULT, 2, Club_EventScript_Accueil
	special Club_DevisStade
	goto_if_eq VAR_RESULT, FALSE, Club_EventScript_InfoGestion
	msgbox gStringVar4, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, Club_EventScript_Gestion
	special Club_ScriptAgrandir
	goto Club_EventScript_ResultatGestion

Club_EventScript_Boutique::
	msgbox Club_Text_BoutiqueDevis, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, Club_EventScript_Gestion
	special Club_ScriptBoutique
	goto Club_EventScript_ResultatGestion

Club_EventScript_Revendre::
	msgbox Club_Text_RevendreConfirme, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, Club_EventScript_Gestion
	special Club_ScriptRevendre
	playse SE_SHOP
	msgbox gStringVar4, MSGBOX_DEFAULT
	goto Club_EventScript_Accueil

Club_EventScript_ResultatGestion::
	goto_if_eq VAR_RESULT, FALSE, Club_EventScript_ResultatRate
	playse SE_SHOP
	msgbox gStringVar4, MSGBOX_DEFAULT
	goto Club_EventScript_Gestion
Club_EventScript_ResultatRate::
	playse SE_BOO
Club_EventScript_InfoGestion::
	msgbox gStringVar4, MSGBOX_DEFAULT
	goto Club_EventScript_Gestion

Club_Text_Propriete:
	.string "Propriété du club$"
Club_Text_Recruter:
	.string "Recruter$"
Club_Text_Coaching:
	.string "Coaching$"
Club_Text_Stade:
	.string "Agrandir le stade$"
Club_Text_Boutique:
	.string "Boutique officielle$"
Club_Text_Revendre:
	.string "Revendre le club$"
Club_Text_Capitaine:
	.string "Capitaine$"
Club_Text_Titulaire2:
	.string "Titulaire (2e)$"
Club_Text_Titulaire3:
	.string "Titulaire (3e)$"
Club_Text_RendrePlace:
	.string "Rendre au titulaire d'origine$"
Club_Text_BonbonL:
	.string "Bonbon Exp. L (+1)$"
Club_Text_BonbonXL:
	.string "Bonbon Exp. XL (+2)$"
Club_Text_ObjetCombat:
	.string "Objet de combat (+3)$"
Club_Text_QuelRachat:
	.string "Quel club veux-tu racheter ?$"
Club_Text_QuellePlace:
	.string "Quelle place ? Toi, tu restes la\n"
	.string "Recrue.$"
Club_Text_QuelleRecrue:
	.string "Qui recrutes-tu ? Prime : une\n"
	.string "saison de salaire.$"
Club_Text_QuelBonbon:
	.string "Quel bonbon lui donnes-tu ?$"
Club_Text_BoutiqueDevis:
	.string "Tenues officielles et Balls Devon :\n"
	.string "120 000 ¥. La boutique passe de\l"
	.string "10 à 25 % de la billetterie. OK ?$"
Club_Text_RevendreConfirme:
	.string "Revendre le club ? Ta société\n"
	.string "récupère 80 % de ton investissement.$"

Club_Text_Bienvenue:
.if EDITION_TYCOON_SIM == TRUE
	.string "Guichet de la NPHL !\n"
.else
	.string "Guichet de la Ligue des Clubs !\n"
.endif
	.string "Matchs, licences et propriété.$"

Club_Text_AllerAuGuichet:
	.string "Les travaux se commandent à un\n"
	.string "guichet de ligue (Centre Pokémon).$"

Club_Text_Tableau:
	.string "Tableau de bord$"
Club_Text_Jouer:
	.string "Jouer mon duel$"
Club_Text_Licence:
	.string "Licence et transferts$"
Club_Text_Regles:
	.string "Règlement$"
Club_Text_Fermer:
	.string "Fermer$"
Club_Text_Retour:
	.string "Retour$"
Club_Text_Club0:
	.string "AC Clémenti (C)$"
Club_Text_Club1:
	.string "AS Autéquia (C)$"
Club_Text_Club2:
	.string "SC Vermilava (B)$"
Club_Text_Club3:
	.string "Aigles Cimetronelle (B)$"
Club_Text_Club4:
	.string "Olympique Mérouville (B)$"
Club_Text_Club5:
	.string "Dynamo Lavandia (B)$"
Club_Text_Club6:
	.string "FC Poivressel (A)$"
Club_Text_Club7:
	.string "Cosmos Algatia (A)$"
Club_Text_Club8:
	.string "Stade Nénucrique (S)$"
Club_Text_Club9:
	.string "Royale Atalanopolis (S)$"

Club_Text_QuelClub:
	.string "Quel club ?$"

Club_Text_OnYVa:
	.string "Ton équipe sera soignée avant et\n"
	.string "après le duel. On y va ?$"

Club_Text_AdversaireBattu:
	.string "Bien joué. Le club retiendra\n"
	.string "ton nom.$"

Club_Text_ReglesTexte:
.if EDITION_TYCOON_SIM == TRUE
	.string "NPHL : 10 clubs dans une seule poule,\n"
	.string "saison de 9 mois. Mercato jours 1-18.\p"
	.string "18 journées en aller-retour, une\n"
	.string "tous les 13 jours (dès le jour 20).\p"
	.string "Les 4 premiers vont en play-offs :\n"
	.string "demi-finales jour 255 (1er-4e, 2e-3e,\l"
	.string "chez le mieux classé), finale jour 265.\p"
.else
	.string "10 clubs, 2 poules de 5 tirées au\n"
	.string "sort à chaque saison de 30 jours.\p"
	.string "Mercato les jours 1 et 2. Dix\n"
	.string "journées en aller-retour, finale\l"
	.string "le jour 30 entre les deux premiers.\p"
.endif
	.string "Chaque rencontre : 4 duels en 6\n"
	.string "contre 6, sans objets du sac.\p"
	.string "Victoire 4 pts, nul 2, défaite 0.\n"
	.string "Gagner 6-0 : +1. Perdre 5-6 : +1.\p"
	.string "Toi, tu joues le duel des Recrues.\n"
	.string "Tu as jusqu'à la journée suivante.\p"
	.string "On joue, signe et gère son club\n"
	.string "aux guichets des Centres Pokémon.\p"
	.string "Après la Ligue, la finale se joue\n"
	.string "à la Tour de Combat.\p"
	.string "Sinon : forfait, défaite 0-6. Les\n"
	.string "autres duels se jouent sans toi.\p"
	.string "Le stade qui reçoit lance son\n"
	.string "effet au 1er tour, pour 5 tours.\p"
	.string "Avec un titre et une société, tu\n"
	.string "peux racheter un club : billets,\l"
	.string "droits TV et boutique, moins les\l"
	.string "salaires, l'entretien et 20 %\l"
	.string "d'impôt, versés en fin de saison.$"
