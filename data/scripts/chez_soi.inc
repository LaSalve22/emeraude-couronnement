@ Émeraude Couronnement (v0.59) : monde ouvert, édition classique. On se réveille seul chez soi (COURONNEMENT_DEPART_CHEZ_SOI) :
@ pas de camion, pas de maman ; la suite du scénario ne change pas (voisins, rival, Prof. Seko sur la Route 101).

Couronnement_EventScript_Reveil::
.if EDITION_TYCOON_SIM == TRUE
	goto Tycoon_EventScript_Reveil
.else
	lockall
	setflag FLAG_HIDE_MAP_NAME_POPUP
	checkplayergender
	call_if_eq VAR_RESULT, MALE, Couronnement_EventScript_ReveilGarcon
	call_if_eq VAR_RESULT, FEMALE, Couronnement_EventScript_ReveilFille
	setflag FLAG_HIDE_LITTLEROOT_TOWN_BRENDANS_HOUSE_MOM
	setflag FLAG_HIDE_LITTLEROOT_TOWN_MAYS_HOUSE_MOM
	setflag FLAG_HIDE_LITTLEROOT_TOWN_PLAYERS_BEDROOM_MOM
	setflag FLAG_HIDE_LITTLEROOT_TOWN_MOM_OUTSIDE
	setflag FLAG_HIDE_LITTLEROOT_TOWN_BRENDANS_HOUSE_TRUCK
	setflag FLAG_HIDE_LITTLEROOT_TOWN_MAYS_HOUSE_TRUCK
	setflag FLAG_HIDE_LITTLEROOT_TOWN_PLAYERS_HOUSE_VIGOROTH_1
	setflag FLAG_HIDE_LITTLEROOT_TOWN_PLAYERS_HOUSE_VIGOROTH_2
	clearflag FLAG_HIDE_LITTLEROOT_TOWN_FAT_MAN
	clearflag FLAG_HIDE_MAP_NAME_POPUP
	msgbox Couronnement_Text_Reveil, MSGBOX_DEFAULT
	call InsideOfTruck_EventScript_DifficultyChoice
	msgbox Couronnement_Text_ReveilHorloge, MSGBOX_DEFAULT
	call PlayersHouse_2F_EventScript_SetWallClock
	delay 30
	setflag FLAG_SET_WALL_CLOCK
	msgbox Couronnement_Text_ReveilDehors, MSGBOX_DEFAULT
	setvar VAR_LITTLEROOT_INTRO_STATE, 7
	releaseall
	end
.endif

@ Mêmes réglages que le camion d'origine (InsideOfTruck_EventScript_SetIntroFlags*), sans le passage vers le camion.
Couronnement_EventScript_ReveilGarcon::
	setrespawn HEAL_LOCATION_LITTLEROOT_TOWN_BRENDANS_HOUSE_2F
	setflag FLAG_HIDE_LITTLEROOT_TOWN_BRENDANS_HOUSE_RIVAL_MOM
	setflag FLAG_HIDE_LITTLEROOT_TOWN_BRENDANS_HOUSE_RIVAL_SIBLING
	setflag FLAG_HIDE_LITTLEROOT_TOWN_BRENDANS_HOUSE_2F_POKE_BALL
	setvar VAR_LITTLEROOT_HOUSES_STATE_BRENDAN, 1
	return

Couronnement_EventScript_ReveilFille::
	setrespawn HEAL_LOCATION_LITTLEROOT_TOWN_MAYS_HOUSE_2F
	setflag FLAG_HIDE_LITTLEROOT_TOWN_MAYS_HOUSE_RIVAL_MOM
	setflag FLAG_HIDE_LITTLEROOT_TOWN_MAYS_HOUSE_RIVAL_SIBLING
	setflag FLAG_HIDE_LITTLEROOT_TOWN_MAYS_HOUSE_2F_POKE_BALL
	setvar VAR_LITTLEROOT_HOUSES_STATE_MAY, 1
	return

@ Après la Ligue, maman passe à la maison pour la remise du billet du S.S. Marée et le flash sur Latios / Latias.
Couronnement_EventScript_VisiteMamanBrendans::
	clearflag FLAG_HIDE_LITTLEROOT_TOWN_BRENDANS_HOUSE_MOM
	return

Couronnement_EventScript_VisiteMamanMays::
	clearflag FLAG_HIDE_LITTLEROOT_TOWN_MAYS_HOUSE_MOM
	return

Couronnement_Text_Reveil:
	.string "Le réveil sonne. Ta chambre, ton\n"
	.string "appart, tes cartons pas encore vidés.\p"
	.string "Pas de maman pour te secouer ce\n"
	.string "matin. Tu vis en solo, maintenant.\l"
	.string "Bourg-en-Vol, c'est chez toi.$"

Couronnement_Text_ReveilHorloge:
	.string "L'horloge murale est arrêtée depuis\n"
	.string "le déménagement. Il faut la régler.$"

Couronnement_Text_ReveilDehors:
	.string "Bon. Il est temps d'aller dire\n"
	.string "bonjour aux voisins.$"
