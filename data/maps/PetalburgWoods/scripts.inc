PetalburgWoods_MapScripts::
	.byte 0

PetalburgWoods_EventScript_DevonResearcherLeft::
	lockall
	call PetalburgWoods_EventScript_DevonResearcherIntro
	applymovement LOCALID_PETALBURG_WOODS_DEVON_EMPLOYEE, PetalburgWoods_Movement_DevonResearcherApproachPlayerLeft
	waitmovement 0
	msgbox PetalburgWoods_Text_HaveYouSeenShroomish, MSGBOX_DEFAULT
	closemessage
	playbgm MUS_ENCOUNTER_AQUA, FALSE
	applymovement LOCALID_PETALBURG_WOODS_GRUNT, PetalburgWoods_Movement_AquaEntrance
	waitmovement 0
	msgbox PetalburgWoods_Text_IWasGoingToAmbushYou, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_PETALBURG_WOODS_GRUNT, PetalburgWoods_Movement_AquaApproachResearcherLeft
	waitmovement 0
	applymovement LOCALID_PETALBURG_WOODS_DEVON_EMPLOYEE, Common_Movement_WalkInPlaceFasterUp
	waitmovement 0
	msgbox PetalburgWoods_Text_HandOverThosePapers, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_PETALBURG_WOODS_DEVON_EMPLOYEE, PetalburgWoods_Movement_DevonResearcherFleeToPlayerLeft
	waitmovement 0
	msgbox PetalburgWoods_Text_YouHaveToHelpMe, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_PETALBURG_WOODS_GRUNT, PetalburgWoods_Movement_AquaApproachPlayer
	waitmovement 0
	msgbox PetalburgWoods_Text_NoOneCrossesTeamAqua, MSGBOX_DEFAULT
	trainerbattle_no_intro TRAINER_GRUNT_PETALBURG_WOODS, PetalburgWoods_Text_YoureKiddingMe
	applymovement LOCALID_PETALBURG_WOODS_GRUNT, PetalburgWoods_Movement_AquaBackOff
	waitmovement 0
	call PetalburgWoods_EventScript_DevonResearcherPostBattle
	applymovement LOCALID_PLAYER, PetalburgWoods_Movement_WatchResearcherLeave
	applymovement LOCALID_PETALBURG_WOODS_DEVON_EMPLOYEE, PetalburgWoods_Movement_DevonResearcherExitLeft
	waitmovement 0
	goto PetalburgWoods_EventScript_RemoveDevonResearcher
	end

PetalburgWoods_EventScript_DevonResearcherRight::
	lockall
	call PetalburgWoods_EventScript_DevonResearcherIntro
	applymovement LOCALID_PETALBURG_WOODS_DEVON_EMPLOYEE, PetalburgWoods_Movement_DevonResearcherApproachPlayerRight
	waitmovement 0
	applymovement LOCALID_PLAYER, Common_Movement_WalkInPlaceFasterLeft
	waitmovement 0
	msgbox PetalburgWoods_Text_HaveYouSeenShroomish, MSGBOX_DEFAULT
	closemessage
	playbgm MUS_ENCOUNTER_AQUA, FALSE
	applymovement LOCALID_PETALBURG_WOODS_GRUNT, PetalburgWoods_Movement_AquaEntrance
	waitmovement 0
	msgbox PetalburgWoods_Text_IWasGoingToAmbushYou, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_PETALBURG_WOODS_GRUNT, PetalburgWoods_Movement_AquaApproachResearcherRight
	waitmovement 0
	applymovement LOCALID_PETALBURG_WOODS_DEVON_EMPLOYEE, Common_Movement_WalkInPlaceFasterUp
	waitmovement 0
	msgbox PetalburgWoods_Text_HandOverThosePapers, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_PETALBURG_WOODS_DEVON_EMPLOYEE, PetalburgWoods_Movement_DevonResearcherFleeToPlayerRight
	waitmovement 0
	msgbox PetalburgWoods_Text_YouHaveToHelpMe, MSGBOX_DEFAULT
	applymovement LOCALID_PLAYER, Common_Movement_WalkInPlaceFasterUp
	waitmovement 0
	msgbox PetalburgWoods_Text_NoOneCrossesTeamAqua, MSGBOX_DEFAULT
	trainerbattle_no_intro TRAINER_GRUNT_PETALBURG_WOODS, PetalburgWoods_Text_YoureKiddingMe
	applymovement LOCALID_PETALBURG_WOODS_GRUNT, PetalburgWoods_Movement_AquaBackOff
	waitmovement 0
	call PetalburgWoods_EventScript_DevonResearcherPostBattle
	applymovement LOCALID_PLAYER, PetalburgWoods_Movement_WatchResearcherLeave
	applymovement LOCALID_PETALBURG_WOODS_DEVON_EMPLOYEE, PetalburgWoods_Movement_DevonResearcherExitRight
	waitmovement 0
	goto PetalburgWoods_EventScript_RemoveDevonResearcher
	end

PetalburgWoods_EventScript_DevonResearcherIntro::
	applymovement LOCALID_PETALBURG_WOODS_DEVON_EMPLOYEE, PetalburgWoods_Movement_DevonResearcherLookAround
	waitmovement 0
	msgbox PetalburgWoods_Text_NotAOneToBeFound, MSGBOX_DEFAULT
	closemessage
	return

PetalburgWoods_EventScript_DevonResearcherPostBattle::
	msgbox PetalburgWoods_Text_YouveGotSomeNerve, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_PETALBURG_WOODS_GRUNT, PetalburgWoods_Movement_AquaRunAway
	waitmovement 0
	removeobject LOCALID_PETALBURG_WOODS_GRUNT
	applymovement LOCALID_PLAYER, Common_Movement_WalkInPlaceFasterDown
	waitmovement 0
	msgbox PetalburgWoods_Text_ThatWasAwfullyClose, MSGBOX_DEFAULT
	giveitem ITEM_GREAT_BALL
	goto_if_eq VAR_RESULT, FALSE, PetalburgWoods_EventScript_BagFull
	goto PetalburgWoods_EventScript_DevonResearcherFinish
	end

PetalburgWoods_EventScript_BagFull::
	msgbox PetalburgWoods_Text_YoureLoadedWithItems, MSGBOX_DEFAULT
	goto PetalburgWoods_EventScript_DevonResearcherFinish
	end

PetalburgWoods_EventScript_DevonResearcherFinish::
	msgbox PetalburgWoods_Text_TeamAquaAfterSomethingInRustboro, MSGBOX_DEFAULT
	applymovement LOCALID_PETALBURG_WOODS_DEVON_EMPLOYEE, PetalburgWoods_Movement_DevonResearcherStartExit
	waitmovement 0
	msgbox PetalburgWoods_Text_ICantBeWastingTime, MSGBOX_DEFAULT
	closemessage
	return

PetalburgWoods_EventScript_RemoveDevonResearcher::
	removeobject LOCALID_PETALBURG_WOODS_DEVON_EMPLOYEE
	setvar VAR_PETALBURG_WOODS_STATE, 1
	releaseall
	end

PetalburgWoods_Movement_DevonResearcherLookAround:
	face_up
	delay_16
	delay_4
	face_right
	delay_16
	delay_8
	face_left
	delay_16
	delay_8
	face_down
	delay_16
	face_right
	delay_16
	delay_8
	face_up
	delay_16
	step_end

PetalburgWoods_Movement_DevonResearcherExitLeft:
	walk_fast_right
	walk_fast_up
	walk_fast_up
	walk_fast_up
	walk_fast_up
	walk_fast_up
	walk_fast_up
	walk_fast_up
	step_end

PetalburgWoods_Movement_DevonResearcherApproachPlayerLeft:
	delay_16
	face_player
	walk_down
	walk_down
	step_end

PetalburgWoods_Movement_DevonResearcherApproachPlayerRight:
	delay_16
	face_player
	walk_down
	walk_down
	walk_down
	walk_in_place_faster_right
	step_end

PetalburgWoods_Movement_DevonResearcherExitRight:
	walk_fast_left
	walk_fast_up
	walk_fast_up
	walk_fast_up
	walk_fast_up
	walk_fast_up
	walk_fast_up
	walk_fast_up
	step_end

PetalburgWoods_Movement_WatchResearcherLeave:
	delay_16
	delay_16
	walk_in_place_faster_up
	step_end

PetalburgWoods_Movement_DevonResearcherFleeToPlayerLeft:
	walk_fast_right
	walk_fast_down
	walk_fast_down
	walk_fast_left
	walk_in_place_faster_up
	step_end

PetalburgWoods_Movement_DevonResearcherFleeToPlayerRight:
	walk_fast_down
	walk_fast_right
	walk_in_place_faster_up
	step_end

PetalburgWoods_Movement_DevonResearcherStartExit:
	walk_in_place_faster_down
	delay_16
	delay_16
	delay_16
	delay_16
	delay_16
	face_up
	step_end

PetalburgWoods_Movement_AquaApproachResearcherLeft:
	walk_fast_down
	walk_fast_down
	step_end

PetalburgWoods_Movement_AquaBackOff:
	lock_facing_direction
	walk_up
	unlock_facing_direction
	step_end

PetalburgWoods_Movement_AquaRunAway:
	walk_fast_up
	walk_fast_up
	walk_fast_up
	walk_fast_up
	walk_fast_up
	delay_16
	delay_16
	step_end

PetalburgWoods_Movement_AquaApproachResearcherRight:
	walk_fast_down
	walk_fast_down
	walk_fast_down
	step_end

PetalburgWoods_Movement_AquaEntrance:
	walk_down
	walk_down
	delay_16
	delay_16
	step_end

PetalburgWoods_Movement_AquaApproachPlayer:
	walk_down
	step_end

PetalburgWoods_EventScript_Boy1::
	msgbox PetalburgWoods_Text_StayOutOfTallGrass, MSGBOX_NPC
	end

PetalburgWoods_EventScript_Boy2::
	msgbox PetalburgWoods_Text_HiddenItemsExplanation, MSGBOX_NPC
	end

PetalburgWoods_EventScript_Girl::
	lock
	faceplayer
	goto_if_set FLAG_RECEIVED_MIRACLE_SEED, PetalburgWoods_EventScript_ExplainMiracleSeed
	msgbox PetalburgWoods_Text_TryUsingThisItem, MSGBOX_DEFAULT
	giveitem ITEM_MIRACLE_SEED
	goto_if_eq VAR_RESULT, FALSE, Common_EventScript_ShowBagIsFull
	setflag FLAG_RECEIVED_MIRACLE_SEED
	release
	end

PetalburgWoods_EventScript_ExplainMiracleSeed::
	msgbox PetalburgWoods_Text_MiracleSeedExplanation, MSGBOX_DEFAULT
	release
	end

PetalburgWoods_EventScript_Sign1::
	msgbox PetalburgWoods_Text_TrainerTipsExperience, MSGBOX_SIGN
	end

PetalburgWoods_EventScript_Sign2::
	msgbox PetalburgWoods_Text_TrainerTipsPP, MSGBOX_SIGN
	end

PetalburgWoods_EventScript_Lyle::
	trainerbattle_single TRAINER_LYLE, PetalburgWoods_Text_GoBugPokemonTeam, PetalburgWoods_Text_ICouldntWin
	msgbox PetalburgWoods_Text_ImOutOfPokeBalls, MSGBOX_AUTOCLOSE
	end

PetalburgWoods_EventScript_James::
	trainerbattle_single TRAINER_JAMES_1, PetalburgWoods_Text_InstantlyPopularWithBugPokemon, PetalburgWoods_Text_CantBePopularIfILose, PetalburgWoods_EventScript_TryRegisterJames
	specialvar VAR_RESULT, ShouldTryRematchBattle
	goto_if_eq VAR_RESULT, TRUE, PetalburgWoods_EventScript_JamesRematch
	setvar VAR_0x8004, TRAINER_JAMES_1
	specialvar VAR_RESULT, IsTrainerRegistered
	goto_if_eq VAR_RESULT, FALSE, PetalburgWoods_EventScript_TryRegisterJames2
	msgbox PetalburgWoods_Text_PeopleRespectYou, MSGBOX_DEFAULT
	release
	end

PetalburgWoods_EventScript_TryRegisterJames::
	special PlayerFaceTrainerAfterBattle
	waitmovement 0
	goto_if_set FLAG_HAS_MATCH_CALL, PetalburgWoods_EventScript_RegisterJames
	release
	end

PetalburgWoods_EventScript_RegisterJames::
	msgbox PetalburgWoods_Text_IWantRematch1, MSGBOX_DEFAULT
	register_matchcall TRAINER_JAMES_1
	release
	end

PetalburgWoods_EventScript_TryRegisterJames2::
	goto_if_set FLAG_HAS_MATCH_CALL, PetalburgWoods_EventScript_RegisterJames2
	msgbox PetalburgWoods_Text_PeopleRespectYou, MSGBOX_DEFAULT
	release
	end

PetalburgWoods_EventScript_RegisterJames2::
	msgbox PetalburgWoods_Text_IWantRematch2, MSGBOX_DEFAULT
	register_matchcall TRAINER_JAMES_1
	release
	end

PetalburgWoods_EventScript_JamesRematch::
	trainerbattle_rematch TRAINER_JAMES_1, PetalburgWoods_Text_MyPokemonHaveGrown, PetalburgWoods_Text_CantBePopularIfLose
	msgbox PetalburgWoods_Text_IveBeenTrainingHard, MSGBOX_AUTOCLOSE
	end

PetalburgWoods_Text_NotAOneToBeFound:
	.string "Hmmm… Pas un seul… Ils étaient là,\n"
	.string "pourtant. Quelque chose les a fait fuir.$"

PetalburgWoods_Text_HaveYouSeenShroomish:
	.string "Bonjour. Tu n'aurais pas vu des\n"
	.string "Balignon, par ici ?\p"
	.string "J'adore ce Pokémon. Et ses spores,\n"
	.string "quand on sait les doser…$"

PetalburgWoods_Text_IWasGoingToAmbushYou:
	.string "Je comptais te tendre une embuscade,\n"
	.string "mais tu traînes dans ce putain de bois\l"
	.string "depuis des heures !\p"
	.string "J'en ai eu marre d'attendre, alors me\n"
	.string "voilà !$"

PetalburgWoods_Text_HandOverThosePapers:
	.string "Toi ! Le chercheur de Devon !\p"
	.string "Donne-moi ces documents. Tout de\n"
	.string "suite. Ou je te fais bouffer la mousse.$"

PetalburgWoods_Text_YouHaveToHelpMe:
	.string "Aïeeeuh !\p"
	.string "Tu es Dresseur, hein ? Aide-moi, je t'en\n"
	.string "supplie ! Il va me tuer !$"

PetalburgWoods_Text_NoOneCrossesTeamAqua:
	.string "Hein ? Tu fais quoi, là ? Tu comptes le\n"
	.string "protéger ?\p"
	.string "Personne ne se met en travers de la\n"
	.string "Team Aqua. On n'a aucune pitié. Même\l"
	.string "pour les gosses !\p"
	.string "Viens te battre, qu'on en finisse !$"

PetalburgWoods_Text_YoureKiddingMe:
	.string "C'est pas vrai ! T'es une teigne !$"

PetalburgWoods_Text_YouveGotSomeNerve:
	.string "Grrr… T'as du culot de te mêler des\n"
	.string "affaires de la Team Aqua ! Reviens te\l"
	.string "battre !\p"
	.string "J'aimerais bien, mais j'ai plus de\n"
	.string "Pokémon en état…\p"
	.string "Et puis, la Team Aqua a aussi des\n"
	.string "affaires à régler à Mérouville.\p"
	.string "Je te laisse filer pour aujourd'hui. Mais\n"
	.string "la prochaine fois, on te retrouvera\l"
	.string "dans un fossé.$"

PetalburgWoods_Text_ThatWasAwfullyClose:
	.string "Pfiou… C'était moins une !\p"
	.string "Grâce à toi, il ne m'a pas volé ces\n"
	.string "documents. Ni la vie.\p"
	.string "Tiens, prends cette Super Ball. C'est le\n"
	.string "moins que je puisse faire !$"

PetalburgWoods_Text_TeamAquaAfterSomethingInRustboro:
	.string "Ce voyou de la Team Aqua n'a pas dit\n"
	.string "qu'ils avaient des affaires à\l"
	.string "Mérouville, eux aussi ?$"

PetalburgWoods_Text_ICantBeWastingTime:
	.string "Oh non ! C'est une urgence ! Je n'ai pas\n"
	.string "de temps à perdre !$"

PetalburgWoods_Text_YoureLoadedWithItems:
	.string "Ton Sac déborde. Je ne peux pas te\n"
	.string "donner cette Super Ball.$"

PetalburgWoods_Text_GoBugPokemonTeam:
	.string "J'ai attrapé une tonne de Pokémon !\p"
	.string "Allez, allez, allez ! Mon équipe\n"
	.string "d'Insectes, à l'attaque !$"

PetalburgWoods_Text_ICouldntWin:
	.string "J'ai tous ces Pokémon, et j'ai quand\n"
	.string "même perdu…$"

PetalburgWoods_Text_ImOutOfPokeBalls:
	.string "J'ai attrapé plein de Pokémon.\n"
	.string "Maintenant, j'ai plus de Poké Balls.$"

PetalburgWoods_Text_InstantlyPopularWithBugPokemon:
	.string "Si tu ramènes des Pokémon Insecte à\n"
	.string "l'école, tout le monde t'adore !$"

PetalburgWoods_Text_CantBePopularIfILose:
	.string "Si je perds, plus personne va m'aimer.$"

PetalburgWoods_Text_PeopleRespectYou:
	.string "Avec un gros Pokémon Insecte, les\n"
	.string "autres te respectent. Ou ils ont peur.\l"
	.string "C'est pareil.$"

PetalburgWoods_Text_IWantRematch1:
	.string "Je veux une revanche quand mes\n"
	.string "Insectes auront grandi !\p"
	.string "Je t'enregistre dans mon PokéNav !$"

PetalburgWoods_Text_IWantRematch2:
	.string "Je veux une revanche quand mes\n"
	.string "Insectes auront grandi !\p"
	.string "Je t'enregistre dans mon PokéNav !$"

PetalburgWoods_Text_MyPokemonHaveGrown:
	.string "Mes Insectes ont grandi. Je vais\n"
	.string "redevenir populaire.$"

PetalburgWoods_Text_CantBePopularIfLose:
	.string "Si je perds, plus personne va m'aimer.$"

PetalburgWoods_Text_IveBeenTrainingHard:
	.string "Avec des Pokémon forts, on devient\n"
	.string "populaire, non ? Alors je me suis\l"
	.string "entraîné comme un malade.$"

PetalburgWoods_Text_StayOutOfTallGrass:
	.string "Hé, toi ! Tes Pokémon tiennent le coup ?\p"
	.string "S'ils sont faibles, évite les hautes\n"
	.string "herbes. Le mois dernier, on a retrouvé\l"
	.string "un randonneur ici. Enfin, ce qu'il en\n"
	.string "restait.$"

PetalburgWoods_Text_HiddenItemsExplanation:
	.string "Parfois, il y a des choses par terre,\n"
	.string "même si on ne les voit pas.\p"
	.string "C'est pour ça que je regarde toujours\n"
	.string "où je marche. On trouve des objets… et\l"
	.string "parfois pire.$"

PetalburgWoods_Text_TryUsingThisItem:
	.string "Oh, cool ! C'est le Badge de l'Arène de\n"
	.string "Mérouville !\p"
	.string "Tu dois être Dresseur. Essaie donc cet\n"
	.string "objet.$"

PetalburgWoods_Text_MiracleSeedExplanation:
	.string "C'est une Graine Miracle. Si un Pokémon\n"
	.string "la tient, ses attaques Plante\l"
	.string "deviennent plus puissantes.\p"
	.string "Il existe d'autres objets utiles à faire\n"
	.string "tenir aux Pokémon, je crois.$"

PetalburgWoods_Text_TrainerTipsExperience:
	.string "Conseils aux Dresseurs\p"
	.string "Tout Pokémon qui participe à un combat,\n"
	.string "même un instant, reçoit des points\l"
	.string "d'expérience.\p"
	.string "Pour entraîner un Pokémon faible,\n"
	.string "place-le en tête d'équipe.\p"
	.string "Puis remplace-le dès le début du\n"
	.string "combat. Il gagnera de l'expérience sans\l"
	.string "risquer sa vie.$"

PetalburgWoods_Text_TrainerTipsPP:
	.string "Conseils aux Dresseurs\p"
	.string "En plus des PV, les Pokémon ont des PP,\n"
	.string "qui servent à lancer leurs attaques.\p"
	.string "Un Pokémon à court de PP doit être\n"
	.string "emmené dans un Centre Pokémon. Ou il se\l"
	.string "battra à mains nues.$"

