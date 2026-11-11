Route102_MapScripts::
	.byte 0

Route102_EventScript_LittleBoy::
	msgbox Route102_Text_ImNotVeryTall, MSGBOX_NPC
	end

Route102_EventScript_RouteSignOldale::
	msgbox Route102_Text_RouteSignOldale, MSGBOX_SIGN
	end

Route102_EventScript_RouteSignPetalburg::
	msgbox Route102_Text_RouteSignPetalburg, MSGBOX_SIGN
	end

Route102_EventScript_Boy::
	msgbox Route102_Text_CatchWholeBunchOfPokemon, MSGBOX_NPC
	end

Route102_EventScript_Calvin::
	trainerbattle_single TRAINER_CALVIN_1, Route102_Text_CalvinIntro, Route102_Text_CalvinDefeated, Route102_EventScript_CalvinRegisterMatchCallAfterBattle
	specialvar VAR_RESULT, ShouldTryRematchBattle
	goto_if_eq VAR_RESULT, TRUE, Route102_EventScript_CalvinRematch
	setvar VAR_0x8004, TRAINER_CALVIN_1
	specialvar VAR_RESULT, IsTrainerRegistered
	goto_if_eq VAR_RESULT, FALSE, Route102_EventScript_CalvinTryRegister
	msgbox Route102_Text_CalvinPostBattle, MSGBOX_DEFAULT
	release
	end

Route102_EventScript_CalvinRegisterMatchCallAfterBattle::
	special PlayerFaceTrainerAfterBattle
	waitmovement 0
	goto_if_set FLAG_HAS_MATCH_CALL, Route102_EventScript_CalvinRegisterMatchCall
	release
	end

Route102_EventScript_CalvinRegisterMatchCall::
	msgbox Route102_Text_CalvinRegisterShort, MSGBOX_DEFAULT
	register_matchcall TRAINER_CALVIN_1
	release
	end

Route102_EventScript_CalvinTryRegister::
	goto_if_set FLAG_HAS_MATCH_CALL, Route102_EventScript_CalvinRegister
	msgbox Route102_Text_CalvinPostBattle, MSGBOX_DEFAULT
	release
	end

Route102_EventScript_CalvinRegister::
	msgbox Route102_Text_CalvinRegister, MSGBOX_DEFAULT
	register_matchcall TRAINER_CALVIN_1
	release
	end

Route102_EventScript_CalvinRematch::
	trainerbattle_rematch TRAINER_CALVIN_1, Route102_Text_CalvinRematchIntro, Route102_Text_CalvinRematchDefeated
	msgbox Route102_Text_CalvinRematchPostBattle, MSGBOX_AUTOCLOSE
	end

Route102_EventScript_Rick::
	trainerbattle_single TRAINER_RICK, Route102_Text_RickIntro, Route102_Text_RickDefeated
	msgbox Route102_Text_RickPostBattle, MSGBOX_AUTOCLOSE
	end

Route102_EventScript_Tiana::
	trainerbattle_single TRAINER_TIANA, Route102_Text_TianaIntro, Route102_Text_TianaDefeated
	msgbox Route102_Text_TianaPostBattle, MSGBOX_AUTOCLOSE
	end

Route102_EventScript_Allen::
	trainerbattle_single TRAINER_ALLEN, Route102_Text_AllenIntro, Route102_Text_AllenDefeated
	msgbox Route102_Text_AllenPostBattle, MSGBOX_AUTOCLOSE
	end

Route102_Text_WatchMeCatchPokemon:
	.string "Timmy : {PLAYER}… Les Pokémon se\n"
	.string "cachent dans les hautes herbes comme\l"
	.string "ça, hein ?\p"
	.string "Regarde-moi, s'il te plaît. Je veux\n"
	.string "savoir si j'en suis capable.\p"
	.string "Juste une fois. Faire un truc tout seul.\n"
	.string "Avant que…\p"
	.string "…Waah !$"

Route102_Text_WallyIDidIt:
	.string "Timmy : J'ai réussi… C'est mon… mon\n"
	.string "Pokémon à moi…\p"
	.string "J'ai jamais rien eu à moi. À part des\n"
	.string "médicaments.$"

Route102_Text_LetsGoBack:
	.string "{PLAYER}… merci. Je m'en souviendrai\n"
	.string "toute ma vie. Même si elle est courte. On\l"
	.string "retourne à l'Arène ?$"

Route102_Text_ImNotVeryTall:
	.string "Je suis… pas très grand, alors je\n"
	.string "disparais dans les hautes herbes.\p"
	.string "L'herbe me rentre dans le nez et…\n"
	.string "Fwafwafwafwa…\p"
	.string "ATCHAAA !\p"
	.string "…J'ai éternué sur un Chenipotte. Il me\n"
	.string "regarde. Il est pas content.$"

Route102_Text_CatchWholeBunchOfPokemon:
	.string "Je vais attraper plein de Pokémon ! Et\n"
	.string "les appeler tous Gérard !$"

Route102_Text_RouteSignOldale:
	.string "Route 102\n"
	.string "{RIGHT_ARROW} Rosyères$"

Route102_Text_RouteSignPetalburg:
	.string "Route 102\n"
	.string "{LEFT_ARROW} Clémenti-Ville$"
