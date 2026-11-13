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
	.string "Écoute-moi bien ! Tu veux devenir\n"
	.string "Maître de la Ligue ?\p"
	.string "Je suis pas Dresseur. J'ai essayé, une\n"
	.string "fois. Mon Pokémon m'a mordu et il est\l"
	.string "parti avec ma copine.\p"
	.string "Mais des conseils, j'en ai à revendre !\p"
	.string "C'est réglé ! On vise le titre ensemble !\p"
	.string "Ton boulot, c'est de rafler les Badges.\n"
	.string "Mon boulot, c'est de t'empêcher de\l"
	.string "mourir bêtement en essayant.\p"
	.string "Roxanne, la Championne, utilise des\n"
	.string "Pokémon Roche.\p"
	.string "La Roche, c'est dur comme… bah, comme\n"
	.string "de la roche. Mais l'Eau et la Plante la\l"
	.string "font fondre.\p"
	.string "Reviens me voir si tu gagnes. Ou si tu\n"
	.string "perds, je te prêterai un mouchoir.\p"
	.string "Allez, fonce !$"

RustboroCity_Gym_Text_GymGuidePostVictory:
	.string "WAOUH ! Quelle victoire ! C'est mes\n"
	.string "encouragements, hein ? Dis oui.\p"
	.string "Regarde ta Carte Dresseur.\p"
	.string "Le Badge Roche doit y être gravé.\p"
	.string "Autrement dit…\p"
	.string "Tu viens de gravir la première marche\n"
	.string "vers le sommet !\p"
	.string "Plus que sept. Et une Ligue. Et toute ta\n"
	.string "vie. Mais c'est un bon début !$"

RustboroCity_Gym_Text_JoshIntro:
	.string "Ne prends pas les Dresseurs d'Arène à la\n"
	.string "légère ! On s'entraîne ici depuis qu'on\l"
	.string "sait marcher !$"

RustboroCity_Gym_Text_JoshDefeat:
	.string "T'étais trop balèze pour moi…$"

RustboroCity_Gym_Text_JoshPostBattle:
	.string "T'as encore rien vu de la vraie\n"
	.string "puissance du type Roche ! Roxanne, elle,\l"
	.string "elle est en granit.$"

RustboroCity_Gym_Text_TommyIntro:
	.string "Si tu me bats pas, Roxanne va te broyer\n"
	.string "comme du gravier !$"

RustboroCity_Gym_Text_TommyDefeat:
	.string "Waouh ! T'as un sacré potentiel !$"

RustboroCity_Gym_Text_TommyPostBattle:
	.string "Roxanne est dix fois plus forte que moi.\p"
	.string "Méfie-toi. Elle sourit jamais en combat.\n"
	.string "Elle remonte juste ses lunettes. Et là,\l"
	.string "t'es déjà mort.$"

RustboroCity_Gym_Text_MarcIntro:
	.string "On est en pleine ville, et alors ?\p"
	.string "Mes Pokémon Roche ont besoin de place\n"
	.string "pour tout péter !$"

RustboroCity_Gym_Text_MarcDefeat:
	.string "Oh là là ! Ce challenger a du mordant !$"

RustboroCity_Gym_Text_MarcPostBattle:
	.string "Faut reconnaître une chose à la\n"
	.string "Championne.\p"
	.string "Choisir la Roche à son âge, c'est malin.\p"
	.string "La Roche ne pleure pas. Ne doute pas. Ne\n"
	.string "trahit pas. Je crois que c'est pour ça\l"
	.string "qu'elle l'a choisie.$"

RustboroCity_Gym_Text_RoxanneIntro:
	.string "Bonjour. Je suis Roxanne, Championne de\n"
	.string "l'Arène de Mérouville.\p"
	.string "Major de promotion de l'École des\n"
	.string "Dresseurs. Moyenne générale : 19,8. Le\l"
	.string "0,2 manquant, c'était une erreur de\l"
	.string "correction.\p"
	.string "Je suis devenue Championne pour\n"
	.string "appliquer la théorie au combat. La\l"
	.string "théorie ne s'est jamais trompée.\p"
	.string "Montre-moi comment tu te bats. Et\n"
	.string "tâche de ne pas me faire perdre mon\l"
	.string "temps.$"

RustboroCity_Gym_Text_RoxanneDefeat:
	.string "…\p"
	.string "Donc… j'ai perdu.\p"
	.string "Ce n'était pas dans le manuel.\p"
	.string "…Pardon. Laisse-moi une seconde.\p"
	.string "… … …\p"
	.string "Bien. Je comprends.\p"
	.string "Le règlement de la Ligue Pokémon\n"
	.string "stipule qu'un Dresseur qui bat un\l"
	.string "Champion reçoit ceci.\p"
	.string "Accepte le Badge Roche officiel de la\n"
	.string "Ligue Pokémon. Tu l'as mérité. Ça me\l"
	.string "coûte de le dire.$"

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
	.string "Elle ensevelit l'adversaire sous les\n"
	.string "pierres et réduit sa Vitesse.\l"
	.string "Méthodique. Efficace. Comme moi,\l"
	.string "d'habitude.\p"
	.string "Une CT apprend l'attaque\n"
	.string "instantanément à un Pokémon.\p"
	.string "Et elle se réutilise à volonté.\n"
	.string "Profites-en.$"

RustboroCity_Gym_Text_RoxannePostBattle:
	.string "Va défier les autres Champions.\p"
	.string "En affrontant beaucoup de Dresseurs,\n"
	.string "tu apprendras beaucoup.\p"
	.string "Moi, je viens d'apprendre que les livres\n"
	.string "ne disent pas tout. C'est… terrifiant.\l"
	.string "Et un peu excitant.$"

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
	.string "échelons à une vitesse indécente.\p"
	.string "J'ai refait tous mes calculs. Je refuse\n"
	.string "de croire que je ne peux plus te battre.\p"
	.string "J'ai entièrement réorganisé mon Arène.\n"
	.string "Nouvelles stratégies. Nouveaux\l"
	.string "Pokémon. Moins de sommeil.\p"
	.string "Reviens quand tu veux. Cette fois, je ne\n"
	.string "perdrai pas. C'est noté dans mon\l"
	.string "agenda.\p"
	.string "… … … … … … … … … … … Clic !$"

RustboroCity_Gym_Text_RegisteredRoxanne:
	.string "La Championne Roxanne a été\n"
	.string "enregistrée dans le PokéNav.$"

RustboroCity_Gym_Text_RoxannePreRematch:
	.string "Roxanne : Te revoilà. Je suis Roxanne, la\n"
	.string "Championne de cette Arène.\p"
	.string "Depuis notre dernier combat, j'ai\n"
	.string "analysé ta stratégie quatre cent douze\l"
	.string "fois.\p"
	.string "Voyons laquelle de nous deux a le plus\n"
	.string "progressé. Pardon. Lequel. Laquelle. Peu\l"
	.string "importe. En garde !$"

RustboroCity_Gym_Text_RoxanneRematchDefeat:
	.string "Grr… Encore… Quatre cent douze\n"
	.string "analyses pour rien…$"

RustboroCity_Gym_Text_RoxannePostRematch:
	.string "Roxanne : J'ai encore tant à apprendre.\p"
	.string "Avant, ça me faisait peur. Maintenant,\n"
	.string "c'est la seule chose qui me fait me lever\l"
	.string "le matin.$"

RustboroCity_Gym_Text_RoxanneRematchNeedTwoMons:
	.string "Roxanne : Te revoilà. Je suis Roxanne, la\n"
	.string "Championne de cette Arène.\p"
	.string "Depuis notre dernier combat, j'ai\n"
	.string "analysé ta stratégie quatre cent douze\l"
	.string "fois.\p"
	.string "Voyons qui a le plus progressé…\p"
	.string "Oh. Tu n'as qu'un seul Pokémon ?\p"
	.string "Statistiquement, c'est du suicide.\n"
	.string "Reviens avec au moins un Pokémon de\l"
	.string "plus.$"
