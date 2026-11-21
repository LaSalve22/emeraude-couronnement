@ Émeraude Couronnement (v0.56), édition Carrière : hall du Couronnement Mondial, un dans chacune des 8 villes d'Arène
@ (même carte partout ; la sortie ramène à la ville d'où l'on vient, le tournoi est celui de cette ville).
CouronnementHall_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, CouronnementHall_OnTransition
	.byte 0

CouronnementHall_OnTransition:
	special Tycoon_HallEntree
	end

Couronnement_EventScript_HotesseHall::
	special Tycoon_HallTournoi
	goto Couronnement_EventScript_Hotesse

Couronnement_EventScript_HallSupporter::
	msgbox Couronnement_Text_HallSupporter, MSGBOX_NPC
	end

Couronnement_EventScript_HallFan::
	msgbox Couronnement_Text_HallFan, MSGBOX_NPC
	end

Couronnement_EventScript_HallPanneau::
	msgbox Couronnement_Text_HallPanneau, MSGBOX_SIGN
	end

Couronnement_Text_HallSupporter:
	.string "Le Couronnement Mondial, c'est du\n"
	.string "double : 6 montrés, 4 choisis.\p"
	.string "Ici, on ne pardonne pas. Une défaite\n"
	.string "et ton tournoi est fini.$"

Couronnement_Text_HallFan:
	.string "J'ai fait tous les halls de Hoenn.\n"
	.string "Chacun a son tournoi et sa prime.\p"
	.string "Les meilleurs finissent au Top 32,\n"
	.string "à la Tour de Combat.$"

Couronnement_Text_HallPanneau:
	.string "HALL DU COURONNEMENT MONDIAL\n"
	.string "3 combats en double, soin entre\l"
	.string "chaque, un seul légendaire.$"
