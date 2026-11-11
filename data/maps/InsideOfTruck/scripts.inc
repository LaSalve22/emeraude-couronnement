InsideOfTruck_MapScripts::
	map_script MAP_SCRIPT_ON_LOAD, InsideOfTruck_OnLoad
	map_script MAP_SCRIPT_ON_RESUME, InsideOfTruck_OnResume
	map_script MAP_SCRIPT_ON_FRAME_TABLE, InsideOfTruck_OnFrame
	.byte 0

InsideOfTruck_OnFrame:
	map_script_2 VAR_DIFFICULTY_CHOSEN, 0, InsideOfTruck_EventScript_ChooseDifficulty
	.2byte 0

@ Émeraude Couronnement : choix du mode de difficulté au tout début de la partie.
InsideOfTruck_EventScript_ChooseDifficulty::
	lockall
	msgbox InsideOfTruck_Text_ChooseDifficultyIntro, MSGBOX_DEFAULT
InsideOfTruck_EventScript_DifficultyMenu::
	message InsideOfTruck_Text_WhichDifficulty
	waitmessage
	dynmultichoice 0, 0, TRUE, 3, 0, DYN_MULTICHOICE_CB_NONE, InsideOfTruck_Text_DifficultyNormal, InsideOfTruck_Text_DifficultyHard, InsideOfTruck_Text_DifficultyKaizo
	switch VAR_RESULT
	case 0, InsideOfTruck_EventScript_PickNormal
	case 1, InsideOfTruck_EventScript_PickHard
	case 2, InsideOfTruck_EventScript_PickKaizo
	goto InsideOfTruck_EventScript_DifficultyMenu

InsideOfTruck_EventScript_PickNormal::
	msgbox InsideOfTruck_Text_ExplainNormal, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, InsideOfTruck_EventScript_DifficultyMenu
	setdifficulty DIFFICULTY_NORMAL
	goto InsideOfTruck_EventScript_DifficultyChosen

InsideOfTruck_EventScript_PickHard::
	msgbox InsideOfTruck_Text_ExplainHard, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, InsideOfTruck_EventScript_DifficultyMenu
	setdifficulty DIFFICULTY_HARD
	goto InsideOfTruck_EventScript_DifficultyChosen

InsideOfTruck_EventScript_PickKaizo::
	msgbox InsideOfTruck_Text_ExplainKaizo, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, InsideOfTruck_EventScript_DifficultyMenu
	setdifficulty DIFFICULTY_KAIZO
	goto InsideOfTruck_EventScript_DifficultyChosen

InsideOfTruck_EventScript_DifficultyChosen::
	setvar VAR_DIFFICULTY_CHOSEN, 1
	msgbox InsideOfTruck_Text_DifficultyChosen, MSGBOX_DEFAULT
	releaseall
	end

InsideOfTruck_Text_DifficultyNormal:
	.string "Normal$"

InsideOfTruck_Text_DifficultyHard:
	.string "Hard$"

InsideOfTruck_Text_DifficultyKaizo:
	.string "Kaizo$"

InsideOfTruck_Text_ChooseDifficultyIntro:
	.string "Le camion roule. Les cartons tremblent.\p"
	.string "Avant que l'aventure commence, une\n"
	.string "question. Et elle compte.$"

InsideOfTruck_Text_WhichDifficulty:
	.string "Quelle difficulté veux-tu ?$"

InsideOfTruck_Text_ExplainNormal:
	.string "Normal\p"
	.string "Le défi d'Émeraude Couronnement, avec\n"
	.string "des règles souples.\p"
	.string "Objets de soin autorisés en combat. Tu\n"
	.string "peux changer de Pokémon après un K.O.\l"
	.string "adverse.\p"
	.string "Choisir Normal ?$"

InsideOfTruck_Text_ExplainHard:
	.string "Hard\p"
	.string "Les dresseurs ont d'autres Pokémon,\n"
	.string "bien mieux construits : EV, IV, attaques\l"
	.string "et objets pensés pour te faire mal.\p"
	.string "Aucun objet de soin en combat. Mode Set\n"
	.string "obligatoire.\p"
	.string "Choisir Hard ?$"

InsideOfTruck_Text_ExplainKaizo:
	.string "Kaizo\p"
	.string "Comme Hard, en pire. Les équipes\n"
	.string "adverses sont taillées pour te broyer.\l"
	.string "Chaque erreur se paie.\p"
	.string "Aucun objet de soin en combat. Mode Set\n"
	.string "obligatoire.\p"
	.string "On t'aura prévenu. Choisir Kaizo ?$"

InsideOfTruck_Text_DifficultyChosen:
	.string "C'est noté.\p"
	.string "Tu pourras baisser la difficulté plus\n"
	.string "tard. Jamais la remonter.\p"
	.string "Bonne route. Et bonne chance.$"

InsideOfTruck_OnLoad:
	setmetatile 4, 1, METATILE_InsideOfTruck_ExitLight_Top, FALSE
	setmetatile 4, 2, METATILE_InsideOfTruck_ExitLight_Mid, FALSE
	setmetatile 4, 3, METATILE_InsideOfTruck_ExitLight_Bottom, FALSE
	end

InsideOfTruck_OnResume:
	setstepcallback STEP_CB_TRUCK
	end

InsideOfTruck_EventScript_SetIntroFlags::
	lockall
	setflag FLAG_HIDE_MAP_NAME_POPUP
	checkplayergender
	goto_if_eq VAR_RESULT, MALE, InsideOfTruck_EventScript_SetIntroFlagsMale
	goto_if_eq VAR_RESULT, FEMALE, InsideOfTruck_EventScript_SetIntroFlagsFemale
	end

InsideOfTruck_EventScript_SetIntroFlagsMale::
	setrespawn HEAL_LOCATION_LITTLEROOT_TOWN_BRENDANS_HOUSE_2F
	setvar VAR_LITTLEROOT_INTRO_STATE, 1
	setflag FLAG_HIDE_LITTLEROOT_TOWN_MAYS_HOUSE_MOM
	setflag FLAG_HIDE_LITTLEROOT_TOWN_MAYS_HOUSE_TRUCK
	setflag FLAG_HIDE_LITTLEROOT_TOWN_BRENDANS_HOUSE_RIVAL_MOM
	setflag FLAG_HIDE_LITTLEROOT_TOWN_BRENDANS_HOUSE_RIVAL_SIBLING
	setflag FLAG_HIDE_LITTLEROOT_TOWN_BRENDANS_HOUSE_2F_POKE_BALL
	setvar VAR_LITTLEROOT_HOUSES_STATE_BRENDAN, 1
	setdynamicwarp MAP_LITTLEROOT_TOWN, 3, 10
	releaseall
	end

InsideOfTruck_EventScript_SetIntroFlagsFemale::
	setrespawn HEAL_LOCATION_LITTLEROOT_TOWN_MAYS_HOUSE_2F
	setvar VAR_LITTLEROOT_INTRO_STATE, 2
	setflag FLAG_HIDE_LITTLEROOT_TOWN_BRENDANS_HOUSE_MOM
	setflag FLAG_HIDE_LITTLEROOT_TOWN_BRENDANS_HOUSE_TRUCK
	setflag FLAG_HIDE_LITTLEROOT_TOWN_MAYS_HOUSE_RIVAL_MOM
	setflag FLAG_HIDE_LITTLEROOT_TOWN_MAYS_HOUSE_RIVAL_SIBLING
	setflag FLAG_HIDE_LITTLEROOT_TOWN_MAYS_HOUSE_2F_POKE_BALL
	setvar VAR_LITTLEROOT_HOUSES_STATE_MAY, 1
	setdynamicwarp MAP_LITTLEROOT_TOWN, 12, 10
	releaseall
	end

InsideOfTruck_EventScript_MovingBox::
	msgbox InsideOfTruck_Text_BoxPrintedWithMonLogo, MSGBOX_SIGN
	end

InsideOfTruck_Text_BoxPrintedWithMonLogo:
	.string "Un carton marqué d'un logo Pokémon.\p"
	.string "“Déménagements Pokémon - Nos Machoc\n"
	.string "portent tout. Même vos regrets.”$"

