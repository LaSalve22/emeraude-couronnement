RustboroCity_Gym_MapScripts::
	.byte 0

RustboroCity_Gym_EventScript_Roxanne::
	trainerbattle_single TRAINER_ROXANNE_1, RustboroCity_Gym_Text_RoxanneIntro, RustboroCity_Gym_Text_RoxanneDefeat, RustboroCity_Gym_EventScript_RoxanneDefeated, NO_MUSIC
	specialvar VAR_RESULT, ShouldTryRematchBattle
	goto_if_eq VAR_RESULT, TRUE, RustboroCity_Gym_EventScript_RoxanneRematch
	goto_if_unset FLAG_RECEIVED_TM_ROCK_TOMB, RustboroCity_Gym_EventScript_GiveRockTomb
	msgbox RustboroCity_Gym_Text_RoxannePostBattle, MSGBOX_DEFAULT
	release
	end

RustboroCity_Gym_EventScript_RoxanneDefeated::
	message RustboroCity_Gym_Text_ReceivedStoneBadge
	waitmessage
	call Common_EventScript_PlayGymBadgeFanfare
	msgbox RustboroCity_Gym_Text_StoneBadgeInfoTakeThis, MSGBOX_DEFAULT
	setflag FLAG_DEFEATED_RUSTBORO_GYM
	setflag FLAG_BADGE01_GET
	setvar VAR_RUSTBORO_CITY_STATE, 1
	addvar VAR_PETALBURG_GYM_STATE, 1
	setvar VAR_0x8008, 1
	call Common_EventScript_SetGymTrainers
	call_if_eq VAR_PETALBURG_GYM_STATE, 6, Common_EventScript_ReadyPetalburgGymForBattle
	goto RustboroCity_Gym_EventScript_GiveRockTomb
	end

RustboroCity_Gym_EventScript_GiveRockTomb::
	giveitem ITEM_TM_ROCK_TOMB
	goto_if_eq VAR_RESULT, FALSE, Common_EventScript_ShowBagIsFull
	setflag FLAG_RECEIVED_TM_ROCK_TOMB
	msgbox RustboroCity_Gym_Text_ExplainRockTomb, MSGBOX_DEFAULT
	release
	end

RustboroCity_Gym_EventScript_RoxanneRematch::
	trainerbattle_rematch_double TRAINER_ROXANNE_1, RustboroCity_Gym_Text_RoxannePreRematch, RustboroCity_Gym_Text_RoxanneRematchDefeat, RustboroCity_Gym_Text_RoxanneRematchNeedTwoMons
	msgbox RustboroCity_Gym_Text_RoxannePostRematch, MSGBOX_AUTOCLOSE
	end

RustboroCity_Gym_EventScript_Josh::
	trainerbattle_single TRAINER_JOSH, RustboroCity_Gym_Text_JoshIntro, RustboroCity_Gym_Text_JoshDefeat
	msgbox RustboroCity_Gym_Text_JoshPostBattle, MSGBOX_AUTOCLOSE
	end

RustboroCity_Gym_EventScript_Tommy::
	trainerbattle_single TRAINER_TOMMY, RustboroCity_Gym_Text_TommyIntro, RustboroCity_Gym_Text_TommyDefeat
	msgbox RustboroCity_Gym_Text_TommyPostBattle, MSGBOX_AUTOCLOSE
	end

RustboroCity_Gym_EventScript_Marc::
	trainerbattle_single TRAINER_MARC, RustboroCity_Gym_Text_MarcIntro, RustboroCity_Gym_Text_MarcDefeat
	msgbox RustboroCity_Gym_Text_MarcPostBattle, MSGBOX_AUTOCLOSE
	end

RustboroCity_Gym_EventScript_GymGuide::
	lock
	faceplayer
	goto_if_set FLAG_DEFEATED_RUSTBORO_GYM, RustboroCity_Gym_EventScript_GymGuidePostVictory
	msgbox RustboroCity_Gym_Text_GymGuideAdvice, MSGBOX_DEFAULT
	release
	end

RustboroCity_Gym_EventScript_GymGuidePostVictory::
	msgbox RustboroCity_Gym_Text_GymGuidePostVictory, MSGBOX_DEFAULT
	release
	end

RustboroCity_Gym_EventScript_LeftGymStatue::
	lockall
	goto_if_set FLAG_BADGE01_GET, RustboroCity_Gym_EventScript_GymStatueCertified
	goto RustboroCity_Gym_EventScript_GymStatue
	end

RustboroCity_Gym_EventScript_RightGymStatue::
	lockall
	goto_if_set FLAG_BADGE01_GET, RustboroCity_Gym_EventScript_GymStatueCertified
	goto RustboroCity_Gym_EventScript_GymStatue
	end

RustboroCity_Gym_EventScript_GymStatueCertified::
	msgbox RustboroCity_Gym_Text_GymStatueCertified, MSGBOX_DEFAULT
	releaseall
	end

RustboroCity_Gym_EventScript_GymStatue::
	msgbox RustboroCity_Gym_Text_GymStatue, MSGBOX_DEFAULT
	releaseall
	end

RustboroCity_Gym_EventScript_RegisterRoxanne::
	lockall
	pokenavcall RustboroCity_Gym_Text_RoxanneRegisterCall
	waitmessage
	delay 30
	playfanfare MUS_REGISTER_MATCH_CALL
	msgbox RustboroCity_Gym_Text_RegisteredRoxanne, MSGBOX_DEFAULT
	waitfanfare
	closemessage
	delay 30
	setflag FLAG_ENABLE_ROXANNE_MATCH_CALL
	clearflag FLAG_ENABLE_ROXANNE_FIRST_CALL
	releaseall
	end

RustboroCity_Gym_Text_GymGuideAdvice:
	.string "Yo, ça roule ?\p"
	.string "Écoute bien ! Tu veux devenir Maître de\n"
	.string "la Ligue ?\p"
	.string "Je ne suis pas Dresseur, mais des\n"
	.string "conseils pour gagner, j'en ai à revendre.\p"
	.string "C'est réglé, alors ! On vise le titre\n"
	.string "ensemble !\p"
	.string "Ton boulot de Dresseur, c'est de rafler\n"
	.string "les Badges, pas vrai ?\p"
	.string "Mais les Champions d'Arène ne sont pas\n"
	.string "des tendres. Certains challengers\l"
	.string "repartent sur une civière.\p"
	.string "Et c'est là que j'interviens ! Conseils\n"
	.string "d'expert garantis !\p"
	.string "Roxanne, la Championne, utilise des\n"
	.string "Pokémon de type Roche.\p"
	.string "La Roche est très résistante, mais elle\n"
	.string "ne supporte pas les attaques Eau et\l"
	.string "Plante.\p"
	.string "Reviens me voir si tu bats la\n"
	.string "Championne.\p"
	.string "Allez, fonce !$"

RustboroCity_Gym_Text_GymGuidePostVictory:
	.string "Waouh ! Quelle victoire ! Mes\n"
	.string "encouragements ont dû aider !\p"
	.string "Parfait ! Regarde ta Carte Dresseur.\p"
	.string "Le Badge Roche que tu as gagné doit y\n"
	.string "être inscrit.\p"
	.string "Autrement dit…\p"
	.string "Tu viens de gravir la première marche\n"
	.string "vers le titre de Maître !\p"
	.string "Ça doit faire un bien fou !$"

RustboroCity_Gym_Text_JoshIntro:
	.string "Ne prends pas les Dresseurs d'Arène à la\n"
	.string "légère ! Je vais te montrer pourquoi on\l"
	.string "est meilleurs !$"

RustboroCity_Gym_Text_JoshDefeat:
	.string "T'étais trop balèze pour moi…$"

RustboroCity_Gym_Text_JoshPostBattle:
	.string "Tu n'as encore rien vu de la puissance\n"
	.string "terrifiante du type Roche !$"

RustboroCity_Gym_Text_TommyIntro:
	.string "Si tu ne me bats pas, Roxanne va te\n"
	.string "broyer !$"

RustboroCity_Gym_Text_TommyDefeat:
	.string "Waouh ! T'as du potentiel !$"

RustboroCity_Gym_Text_TommyPostBattle:
	.string "Roxanne est plusieurs fois plus forte\n"
	.string "que moi.\p"
	.string "Méfie-toi. Elle ne sourit jamais pendant\n"
	.string "un combat.$"

RustboroCity_Gym_Text_MarcIntro:
	.string "On est en pleine ville, et alors ?\p"
	.string "Mes Pokémon Roche ont besoin de place\n"
	.string "pour tout casser !$"

RustboroCity_Gym_Text_MarcDefeat:
	.string "Oh là là ! Ce challenger a du mordant !$"

RustboroCity_Gym_Text_MarcPostBattle:
	.string "Faut reconnaître une chose à notre\n"
	.string "Championne.\p"
	.string "Choisir la Roche si jeune, c'est malin. La\n"
	.string "Roche ne pleure pas, ne doute pas, ne\l"
	.string "meurt pas.$"

RustboroCity_Gym_Text_RoxanneIntro:
	.string "Bonjour. Je suis Roxanne, Championne de\n"
	.string "l'Arène de Mérouville.\p"
	.string "Je suis devenue Championne pour\n"
	.string "appliquer au combat ce que j'ai appris à\l"
	.string "l'École des Dresseurs. Major de ma\n"
	.string "promotion, bien sûr.\p"
	.string "Montre-moi comment tu te bats, et avec\n"
	.string "quels Pokémon. Essaie de ne pas me\l"
	.string "faire perdre mon temps.$"

RustboroCity_Gym_Text_RoxanneDefeat:
	.string "Donc… j'ai perdu…\p"
	.string "Il semble que j'aie encore beaucoup à\n"
	.string "apprendre… C'est… humiliant.\p"
	.string "Je comprends.\p"
	.string "Le règlement de la Ligue Pokémon\n"
	.string "stipule qu'un Dresseur qui bat un\l"
	.string "Champion reçoit ceci.\p"
	.string "Accepte le Badge Roche officiel de la\n"
	.string "Ligue Pokémon.$"

RustboroCity_Gym_Text_ReceivedStoneBadge:
	.string "{PLAYER} reçoit le Badge Roche de la\n"
	.string "part de Roxanne.$"

RustboroCity_Gym_Text_StoneBadgeInfoTakeThis:
	.string "Le Badge Roche augmente l'Attaque de\n"
	.string "tes Pokémon.\p"
	.string "Il leur permet aussi d'utiliser la CS\n"
	.string "Coupe hors combat.\p"
	.string "Prends également ceci.$"

RustboroCity_Gym_Text_ExplainRockTomb:
	.string "Cette Capsule Technique, la CT39,\n"
	.string "contient Tomberoche.\p"
	.string "Elle écrase l'adversaire sous les\n"
	.string "pierres et réduit sa Vitesse. Ensevelir\l"
	.string "avant d'achever.\p"
	.string "Une CT apprend instantanément\n"
	.string "l'attaque à un Pokémon.\p"
	.string "Et elle se réutilise à volonté.\n"
	.string "Profites-en.$"

RustboroCity_Gym_Text_RoxannePostBattle:
	.string "Puisque tu as tant de talent, va défier\n"
	.string "les autres Champions.\p"
	.string "En affrontant beaucoup de Dresseurs,\n"
	.string "tu apprendras beaucoup. Moi aussi,\l"
	.string "visiblement.$"

RustboroCity_Gym_Text_GymStatue:
	.string "Arène Pokémon de Mérouville$"

RustboroCity_Gym_Text_GymStatueCertified:
	.string "Arène Pokémon de Mérouville\p"
	.string "Dresseurs reconnus par Roxanne :\n"
	.string "{PLAYER}$"

RustboroCity_Gym_Text_RoxanneRegisterCall:
	.string "… … … … … … … … … … … Bip !\p"
	.string "Roxanne : Allô, {PLAYER} ? Ici Roxanne,\n"
	.string "de Mérouville.\p"
	.string "Bastien m'a dit que tu grimpais les\n"
	.string "échelons des Dresseurs à une vitesse\l"
	.string "folle.\p"
	.string "Je refuse de croire que je ne peux plus\n"
	.string "te battre.\p"
	.string "Je te promets de remettre mon Arène à\n"
	.string "niveau pour que tu puisses nous défier\l"
	.string "à nouveau.\p"
	.string "J'espère que tu attends ce moment\n"
	.string "autant que moi. Cette fois, je ne\l"
	.string "perdrai pas.\p"
	.string "… … … … … … … … … … … Clic !$"

RustboroCity_Gym_Text_RegisteredRoxanne:
	.string "La Championne Roxanne a été\n"
	.string "enregistrée dans le PokéNav.$"

RustboroCity_Gym_Text_RoxannePreRematch:
	.string "Roxanne : Ravie de te revoir. Je suis\n"
	.string "Roxanne, la Championne de cette Arène.\p"
	.string "Nous avons beaucoup combattu, toi et\n"
	.string "moi, depuis notre dernière rencontre.\p"
	.string "Voyons qui a le plus progressé. En garde\n"
	.string "!$"

RustboroCity_Gym_Text_RoxanneRematchDefeat:
	.string "Grr… J'ai encore perdu…$"

RustboroCity_Gym_Text_RoxannePostRematch:
	.string "Roxanne : J'ai encore tant à apprendre\n"
	.string "sur les combats Pokémon.\p"
	.string "Et c'est justement ce qui me les fait\n"
	.string "aimer davantage. Même quand ils me\l"
	.string "brisent.$"

RustboroCity_Gym_Text_RoxanneRematchNeedTwoMons:
	.string "Roxanne : Ravie de te revoir. Je suis\n"
	.string "Roxanne, la Championne de cette Arène.\p"
	.string "Nous avons beaucoup combattu, toi et\n"
	.string "moi, depuis notre dernière rencontre.\p"
	.string "Voyons qui a le plus progressé…\p"
	.string "Oh, non… Tu n'as qu'un seul Pokémon ?\p"
	.string "Reviens avec au moins un Pokémon de\n"
	.string "plus.$"
