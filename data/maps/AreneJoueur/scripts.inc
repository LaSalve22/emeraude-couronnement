@ Émeraude Couronnement (v0.57.1), édition Carrière : intérieur des arènes du joueur (même carte pour les 6 villes ;
@ la sortie ramène à la ville d'où l'on vient, l'arène est celle de cette ville : Tycoon_AreneEntree / Arene_IndexIci).
AreneJoueur_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, AreneJoueur_OnTransition
	.byte 0

AreneJoueur_OnTransition:
	special Tycoon_AreneEntree
	end

Arene_EventScript_GuichetInterieur::
	lock
	faceplayer
	special Arene_IndexIci
	goto Arene_EventScript_GuichetSuite

Arene_EventScript_Entraineur::
	msgbox Arene_Text_Entraineur, MSGBOX_NPC
	end

Arene_EventScript_Statue::
	special Arene_IndexIci
	special Arene_TextePanneau
	msgbox gStringVar4, MSGBOX_SIGN
	end

Arene_Text_Entraineur:
	.string "Les membres du club s'entraînent ici\n"
	.string "avec les Pokémon de l'arène.\p"
	.string "Plus tu combats avec eux, plus votre\n"
	.string "lien grandit. Parle au responsable\l"
	.string "à l'entrée.$"
