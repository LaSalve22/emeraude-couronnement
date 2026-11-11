SlateportCity_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, SlateportCity_OnTransition
	map_script MAP_SCRIPT_ON_FRAME_TABLE, SlateportCity_OnFrame
	.byte 0

SlateportCity_OnTransition:
	setvar VAR_SLATEPORT_MUSEUM_1F_STATE, 0
	call SlateportCity_EventScript_EnterSlateport
	call_if_eq VAR_SLATEPORT_CITY_STATE, 1, SlateportCity_EventScript_MovePeopleForSternInterview
	call_if_eq VAR_SLATEPORT_OUTSIDE_MUSEUM_STATE, 1, SlateportCity_EventScript_SetReadyForScottScene
	end

SlateportCity_EventScript_EnterSlateport::
	setflag FLAG_VISITED_SLATEPORT_CITY
	setvar VAR_CONTEST_HALL_STATE, 0
	setflag FLAG_HIDE_SLATEPORT_CITY_CONTEST_REPORTER
	return

SlateportCity_EventScript_MovePeopleForSternInterview::
	setobjectxyperm LOCALID_SLATEPORT_CAPT_STERN, 28, 13
	setobjectxyperm LOCALID_SLATEPORT_OLD_WOMAN, 25, 13
	setobjectxyperm LOCALID_SLATEPORT_RICH_BOY, 25, 14
	setobjectxyperm LOCALID_SLATEPORT_COOK, 27, 16
	setobjectxyperm LOCALID_SLATEPORT_GIRL, 28, 16
	setobjectxyperm LOCALID_SLATEPORT_FAT_MAN, 29, 16
	setobjectxyperm LOCALID_SLATEPORT_MAN_1, 31, 14
	setobjectmovementtype LOCALID_SLATEPORT_CAPT_STERN, MOVEMENT_TYPE_FACE_DOWN
	setobjectmovementtype LOCALID_SLATEPORT_OLD_WOMAN, MOVEMENT_TYPE_FACE_DOWN_AND_RIGHT
	setobjectmovementtype LOCALID_SLATEPORT_RICH_BOY, MOVEMENT_TYPE_FACE_UP_AND_RIGHT
	setobjectmovementtype LOCALID_SLATEPORT_COOK, MOVEMENT_TYPE_FACE_UP
	setobjectmovementtype LOCALID_SLATEPORT_GIRL, MOVEMENT_TYPE_FACE_UP
	setobjectmovementtype LOCALID_SLATEPORT_FAT_MAN, MOVEMENT_TYPE_FACE_UP
	setobjectmovementtype LOCALID_SLATEPORT_MAN_1, MOVEMENT_TYPE_FACE_LEFT
	return

SlateportCity_EventScript_SetReadyForScottScene::
	setflag FLAG_HIDE_MAP_NAME_POPUP
	getplayerxy VAR_0x8004, VAR_0x8005
	goto_if_eq VAR_0x8004, 30, SlateportCity_EventScript_MoveScottLeft
	setobjectxyperm LOCALID_SLATEPORT_SCOTT, 23, 27
	setobjectmovementtype LOCALID_SLATEPORT_SCOTT, MOVEMENT_TYPE_FACE_RIGHT
	return

SlateportCity_EventScript_MoveScottLeft::
	setobjectxyperm LOCALID_SLATEPORT_SCOTT, 22, 27
	setobjectmovementtype LOCALID_SLATEPORT_SCOTT, MOVEMENT_TYPE_FACE_RIGHT
	return

SlateportCity_OnFrame:
	map_script_2 VAR_SLATEPORT_OUTSIDE_MUSEUM_STATE, 1, SlateportCity_EventScript_ScottScene
	.2byte 0

SlateportCity_EventScript_ScottScene::
	lockall
	addobject LOCALID_SLATEPORT_SCOTT
	applymovement LOCALID_PLAYER, SlateportCity_Movement_PlayerFaceScott
	applymovement LOCALID_SLATEPORT_SCOTT, SlateportCity_Movement_ScottApproachPlayer
	waitmovement 0
	msgbox SlateportCity_Text_YouDroveTeamAquaAway, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_SLATEPORT_SCOTT, Common_Movement_WalkInPlaceFasterLeft
	waitmovement 0
	delay 60
	msgbox SlateportCity_Text_MaybeThisTrainer, MSGBOX_DEFAULT
	applymovement LOCALID_SLATEPORT_SCOTT, Common_Movement_WalkInPlaceFasterRight
	waitmovement 0
	msgbox SlateportCity_Text_LetsRegisterEachOther, MSGBOX_DEFAULT
	closemessage
	delay 30
	playfanfare MUS_REGISTER_MATCH_CALL
	msgbox SlateportCity_Text_RegisteredScott, MSGBOX_DEFAULT
	waitfanfare
	closemessage
	delay 30
	setflag FLAG_ENABLE_SCOTT_MATCH_CALL
	msgbox SlateportCity_Text_KeepEyeOnTrainersBeSeeingYou, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_SLATEPORT_SCOTT, SlateportCity_Movement_ScottExit
	waitmovement 0
	removeobject LOCALID_SLATEPORT_SCOTT
	setobjectxyperm LOCALID_SLATEPORT_SCOTT, 10, 12
	setobjectmovementtype LOCALID_SLATEPORT_SCOTT, MOVEMENT_TYPE_FACE_DOWN
	clearflag FLAG_HIDE_MAP_NAME_POPUP
	setvar VAR_SLATEPORT_OUTSIDE_MUSEUM_STATE, 2
	addvar VAR_SCOTT_STATE, 1
	releaseall
	end

SlateportCity_Movement_PlayerFaceScott:
	delay_16
	delay_8
	walk_in_place_faster_left
	step_end

SlateportCity_Movement_ScottApproachPlayer:
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	step_end

SlateportCity_Movement_ScottExit:
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	step_end

SlateportCity_EventScript_EnergyGuru::
	lock
	faceplayer
	message SlateportCity_Text_EnergyGuruSellWhatYouNeed
	waitmessage
	pokemart SlateportCity_Pokemart_EnergyGuru
	msgbox gText_PleaseComeAgain, MSGBOX_DEFAULT
	release
	end

	.align 2
SlateportCity_Pokemart_EnergyGuru:
	.2byte ITEM_PROTEIN
	.2byte ITEM_IRON
	.2byte ITEM_CARBOS
	.2byte ITEM_ZINC
	.2byte ITEM_CALCIUM
	.2byte ITEM_HP_UP
	pokemartlistend

SlateportCity_EventScript_EffortRibbonWoman::
	lock
	faceplayer
	bufferleadmonspeciesname STR_VAR_1
	msgbox SlateportCity_Text_OhYourPokemon, MSGBOX_DEFAULT
	specialvar VAR_RESULT, LeadMonHasEffortRibbon
	call_if_eq VAR_RESULT, TRUE, SlateportCity_EventScript_MonHasEffortRibbon
	specialvar VAR_RESULT, Special_AreLeadMonEVsMaxedOut
	call_if_eq VAR_RESULT, FALSE, SlateportCity_EventScript_MonEVsNotMaxed
	msgbox SlateportCity_Text_PleaseGiveItThisEffortRibbon, MSGBOX_DEFAULT
	playfanfare MUS_OBTAIN_ITEM
	message SlateportCity_Text_ReceivedEffortRibbon
	waitfanfare
	msgbox SlateportCity_Text_PutEffortRibbonOnMon, MSGBOX_DEFAULT
	special GiveLeadMonEffortRibbon
	release
	end

SlateportCity_EventScript_MonEVsNotMaxed::
	msgbox SlateportCity_Text_GoForItLittleHarder, MSGBOX_DEFAULT
	release
	end

SlateportCity_EventScript_MonHasEffortRibbon::
	msgbox SlateportCity_Text_EffortRibbonLooksGoodOnIt, MSGBOX_DEFAULT
	release
	end

SlateportCity_EventScript_Cook::
	lock
	faceplayer
	call_if_eq VAR_SLATEPORT_CITY_STATE, 1, SlateportCity_EventScript_CookSternInterview
	msgbox SlateportCity_Text_SeaweedFullOfLife, MSGBOX_DEFAULT
	release
	end

SlateportCity_EventScript_CookSternInterview::
	msgbox SlateportCity_Text_CaptainComeBackWithBigFish, MSGBOX_DEFAULT
	release
	end

SlateportCity_EventScript_OldWoman::
	lock
	faceplayer
	call_if_eq VAR_SLATEPORT_CITY_STATE, 1, SlateportCity_EventScript_OldWomanSternInterview
	msgbox SlateportCity_Text_HowTownIsBornAndGrows, MSGBOX_DEFAULT
	release
	end

SlateportCity_EventScript_OldWomanSternInterview::
	msgbox SlateportCity_Text_CaptSternBeingInterviewed, MSGBOX_DEFAULT
	release
	end

SlateportCity_EventScript_Girl::
	lock
	faceplayer
	call_if_eq VAR_SLATEPORT_CITY_STATE, 1, SlateportCity_EventScript_GirlSternInterview
	goto_if_set FLAG_RECEIVED_SECRET_POWER, SlateportCity_EventScript_GirlSecretBase
	msgbox SlateportCity_Text_SlateportWonderfulPlace, MSGBOX_DEFAULT
	release
	end

SlateportCity_EventScript_GirlSternInterview::
	msgbox SlateportCity_Text_InterviewerSoCool, MSGBOX_DEFAULT
	release
	end

SlateportCity_EventScript_GirlSecretBase::
	msgbox SlateportCity_Text_BuyBricksSoDecorWontGetDirty, MSGBOX_DEFAULT
	release
	end

SlateportCity_EventScript_RichBoy::
	lock
	faceplayer
	call_if_eq VAR_SLATEPORT_CITY_STATE, 1, SlateportCity_EventScript_RichBoySternInterview
	msgbox SlateportCity_Text_GoingToCompeteInBattleTent, MSGBOX_DEFAULT
	release
	end

SlateportCity_EventScript_RichBoySternInterview::
	msgbox SlateportCity_Text_SternSaysDiscoveredSomething, MSGBOX_DEFAULT
	release
	end

SlateportCity_EventScript_FatMan::
	goto_if_eq VAR_SLATEPORT_CITY_STATE, 1, SlateportCity_EventScript_FatManSternInterview
	msgbox SlateportCity_Text_BushedHikingFromMauville, MSGBOX_NPC
	end

SlateportCity_EventScript_FatManSternInterview::
	msgbox SlateportCity_Text_AmIOnTV, MSGBOX_SIGN
	end

SlateportCity_EventScript_Man1::
	lock
	faceplayer
	call_if_eq VAR_SLATEPORT_CITY_STATE, 1, SlateportCity_EventScript_Man1SternInterview
	msgbox SlateportCity_Text_EveryoneCallsHimCaptStern, MSGBOX_DEFAULT
	release
	end

SlateportCity_EventScript_Man1SternInterview::
	msgbox SlateportCity_Text_CaptainsACelebrity, MSGBOX_DEFAULT
	release
	end

SlateportCity_EventScript_Woman1::
	lock
	faceplayer
	goto_if_set FLAG_DOCK_REJECTED_DEVON_GOODS, SlateportCity_EventScript_Woman1AquaGone
	msgbox SlateportCity_Text_WhatsLongLineOverThere, MSGBOX_DEFAULT
	release
	end

SlateportCity_EventScript_Woman1AquaGone::
	msgbox SlateportCity_Text_VisitedMuseumOften, MSGBOX_DEFAULT
	release
	end

SlateportCity_EventScript_BattleTentSign::
	msgbox SlateportCity_Text_BattleTentSign, MSGBOX_SIGN
	end

SlateportCity_EventScript_SternsShipyardSign::
	lockall
	goto_if_set FLAG_SYS_GAME_CLEAR, SlateportCity_EventScript_SternsShipyardFerryComplete
	goto_if_set FLAG_BADGE07_GET, SlateportCity_EventScript_SternsShipyardNearsCompletion
	msgbox SlateportCity_Text_SternsShipyardWantedSign, MSGBOX_DEFAULT
	releaseall
	end

SlateportCity_EventScript_SternsShipyardNearsCompletion::
	msgbox SlateportCity_Text_SternsShipyardNearsCompletion, MSGBOX_DEFAULT
	releaseall
	end

SlateportCity_EventScript_SternsShipyardFerryComplete::
	msgbox SlateportCity_Text_SternsShipyardFerryComplete, MSGBOX_DEFAULT
	releaseall
	end

SlateportCity_EventScript_PokemonFanClubSign::
	msgbox SlateportCity_Text_PokemonFanClubSign, MSGBOX_SIGN
	end

SlateportCity_EventScript_OceanicMuseumSign::
	msgbox SlateportCity_Text_OceanicMuseumSign, MSGBOX_SIGN
	end

SlateportCity_EventScript_CitySign::
	msgbox SlateportCity_Text_CitySign, MSGBOX_SIGN
	end

SlateportCity_EventScript_MarketSign::
	msgbox SlateportCity_Text_MarketSign, MSGBOX_SIGN
	end

SlateportCity_EventScript_HarborSign::
	lockall
	goto_if_set FLAG_SYS_GAME_CLEAR, SlateportCity_EventScript_HarborSignFerryComplete
	msgbox SlateportCity_Text_HarborFerryUnderConstruction, MSGBOX_DEFAULT
	releaseall
	end

SlateportCity_EventScript_HarborSignFerryComplete::
	msgbox SlateportCity_Text_HarborSign, MSGBOX_DEFAULT
	releaseall
	end

SlateportCity_EventScript_NameRatersHouseSign::
	msgbox SlateportCity_Text_NameRatersHouseSign, MSGBOX_SIGN
	end

SlateportCity_EventScript_Maniac::
	lock
	faceplayer
	call_if_eq VAR_SLATEPORT_CITY_STATE, 1, SlateportCity_EventScript_ManiacSternInterview
	msgbox SlateportCity_Text_GetNameRaterToHelpYou, MSGBOX_DEFAULT
	release
	end

@ Unclear if the text here was meant to be different, but its not
SlateportCity_EventScript_ManiacSternInterview::
	msgbox SlateportCity_Text_GetNameRaterToHelpYou, MSGBOX_DEFAULT
	release
	end

SlateportCity_EventScript_Woman2::
	msgbox SlateportCity_Text_CantChangeTradeMonName, MSGBOX_NPC
	end

SlateportCity_EventScript_Sailor1::
	msgbox SlateportCity_Text_SeaIsSoWet, MSGBOX_NPC
	end

SlateportCity_EventScript_Sailor2::
	msgbox SlateportCity_Text_SinkOldBoats, MSGBOX_NPC
	end

SlateportCity_EventScript_PokefanF::
	msgbox SlateportCity_Text_BuyTooMuch, MSGBOX_NPC
	end

SlateportCity_EventScript_Man2::
	msgbox SlateportCity_Text_BattleTentBuiltRecently, MSGBOX_NPC
	end

SlateportCity_EventScript_AquaGrunt1::
	lock
	faceplayer
	msgbox SlateportCity_Text_QuitPushing, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_SLATEPORT_GRUNT_1, Common_Movement_FaceOriginalDirection
	waitmovement 0
	release
	end

SlateportCity_EventScript_AquaGrunt2::
	lock
	faceplayer
	msgbox SlateportCity_Text_AquaHasPolicy, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_SLATEPORT_GRUNT_2, Common_Movement_FaceOriginalDirection
	waitmovement 0
	release
	end

SlateportCity_EventScript_AquaGrunt3::
	lock
	faceplayer
	msgbox SlateportCity_Text_BossIsBrilliant, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_SLATEPORT_GRUNT_3, Common_Movement_FaceOriginalDirection
	waitmovement 0
	release
	end

SlateportCity_EventScript_AquaGrunt4::
	lock
	faceplayer
	msgbox SlateportCity_Text_WhatsNewSchemeIWonder, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_SLATEPORT_GRUNT_4, Common_Movement_FaceOriginalDirection
	waitmovement 0
	release
	end

SlateportCity_EventScript_AquaGrunt5::
	lock
	faceplayer
	msgbox SlateportCity_Text_ShouldTakeItAll, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_SLATEPORT_GRUNT_5, Common_Movement_FaceOriginalDirection
	waitmovement 0
	release
	end

SlateportCity_EventScript_AquaGrunt6::
	lock
	faceplayer
	msgbox SlateportCity_Text_DontButtIn, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_SLATEPORT_GRUNT_6, Common_Movement_FaceOriginalDirection
	waitmovement 0
	release
	end

SlateportCity_EventScript_AquaGrunt7::
	lock
	faceplayer
	msgbox SlateportCity_Text_RemindsMeOfLongLineForGames, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_SLATEPORT_GRUNT_7, Common_Movement_FaceOriginalDirection
	waitmovement 0
	release
	end

SlateportCity_EventScript_AquaGrunt8::
	lock
	faceplayer
	msgbox SlateportCity_Text_WhyAreWeLiningUp, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_SLATEPORT_GRUNT_8, Common_Movement_FaceOriginalDirection
	waitmovement 0
	release
	end

SlateportCity_EventScript_AquaGrunt9::
	lock
	faceplayer
	playse SE_PIN
	applymovement LOCALID_SLATEPORT_GRUNT_9, Common_Movement_ExclamationMark
	waitmovement 0
	applymovement LOCALID_SLATEPORT_GRUNT_9, Common_Movement_Delay48
	waitmovement 0
	msgbox SlateportCity_Text_WhatDoYouWant, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_SLATEPORT_GRUNT_9, Common_Movement_FaceOriginalDirection
	waitmovement 0
	applymovement LOCALID_SLATEPORT_GRUNT_9, SlateportCity_Movement_DelayAquaGrunt
	waitmovement 0
	applymovement LOCALID_SLATEPORT_GRUNT_9, Common_Movement_FacePlayer
	waitmovement 0
	msgbox SlateportCity_Text_IllReadSignForYou, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_SLATEPORT_GRUNT_9, Common_Movement_FaceOriginalDirection
	waitmovement 0
	applymovement LOCALID_SLATEPORT_GRUNT_9, SlateportCity_Movement_DelayAquaGrunt
	waitmovement 0
	msgbox SlateportCity_Text_SaysSomethingLikeSeaIsEndless, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_SLATEPORT_GRUNT_9, Common_Movement_FacePlayer
	waitmovement 0
	applymovement LOCALID_SLATEPORT_GRUNT_9, SlateportCity_Movement_DelayAquaGrunt
	waitmovement 0
	applymovement LOCALID_SLATEPORT_GRUNT_9, Common_Movement_FaceOriginalDirection
	waitmovement 0
	release
	end

SlateportCity_Movement_DelayAquaGrunt:
	delay_16
	delay_16
	step_end

SlateportCity_EventScript_AquaGrunt10::
	msgbox SlateportCity_Text_ShouldveBroughtMyGameBoy, MSGBOX_SIGN
	end

SlateportCity_EventScript_AquaGrunt11::
	lock
	faceplayer
	msgbox SlateportCity_Text_HotSpringsAfterOperation, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_SLATEPORT_GRUNT_11, Common_Movement_FaceOriginalDirection
	waitmovement 0
	release
	end

SlateportCity_EventScript_DollClerk::
	lock
	faceplayer
	message gText_HowMayIServeYou
	waitmessage
	pokemartdecoration SlateportCity_PokemartDecor_Dolls
	msgbox gText_PleaseComeAgain, MSGBOX_DEFAULT
	release
	end

	.align 2
SlateportCity_PokemartDecor_Dolls:
	.2byte DECOR_AZURILL_DOLL
	.2byte DECOR_MARILL_DOLL
	.2byte DECOR_SKITTY_DOLL
	pokemartlistend

SlateportCity_EventScript_ComeBackWithSecretPower::
	msgbox gText_ComeBackWithSecretPower, MSGBOX_DEFAULT
	release
	end

SlateportCity_EventScript_DecorClerk::
	lock
	faceplayer
	goto_if_unset FLAG_RECEIVED_SECRET_POWER, SlateportCity_EventScript_ComeBackWithSecretPower
	message gText_HowMayIServeYou
	waitmessage
	pokemartdecoration SlateportCity_PokemartDecor
	msgbox gText_PleaseComeAgain, MSGBOX_DEFAULT
	release
	end

	.align 2
SlateportCity_PokemartDecor:
	.2byte DECOR_RED_BRICK
	.2byte DECOR_BLUE_BRICK
	.2byte DECOR_YELLOW_BRICK
	.2byte DECOR_RED_BALLOON
	.2byte DECOR_BLUE_BALLOON
	.2byte DECOR_YELLOW_BALLOON
	.2byte DECOR_C_LOW_NOTE_MAT
	.2byte DECOR_D_NOTE_MAT
	.2byte DECOR_E_NOTE_MAT
	.2byte DECOR_F_NOTE_MAT
	.2byte DECOR_G_NOTE_MAT
	.2byte DECOR_A_NOTE_MAT
	.2byte DECOR_B_NOTE_MAT
	.2byte DECOR_C_HIGH_NOTE_MAT
	pokemartlistend

SlateportCity_EventScript_PowerTMClerk::
	lock
	faceplayer
	message gText_HowMayIServeYou
	waitmessage
	pokemart SlateportCity_Pokemart_PowerTMs
	msgbox gText_PleaseComeAgain, MSGBOX_DEFAULT
	release
	end

	.align 2
SlateportCity_Pokemart_PowerTMs:
	.2byte ITEM_TM_HIDDEN_POWER
	.2byte ITEM_TM_SECRET_POWER
	pokemartlistend

@ Scene with Capt Sterns interview and Team Aqua announcing plans to steal Submarine
SlateportCity_EventScript_CaptStern::
	lockall
	msgbox SlateportCity_Text_SternMoveAheadWithExploration, MSGBOX_DEFAULT
	msgbox SlateportCity_Text_GabbyWonderfulThanksForInterview, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_SLATEPORT_GABBY, Common_Movement_WalkInPlaceFasterRight
	waitmovement 0
	delay 10
	applymovement LOCALID_SLATEPORT_TY, Common_Movement_WalkInPlaceFasterDown
	waitmovement 0
	delay 25
	applymovement LOCALID_SLATEPORT_CAPT_STERN, SlateportCity_Movement_SternWatchGabbyAndTyExit
	applymovement LOCALID_PLAYER, SlateportCity_Movement_PlayerFaceStern
	applymovement LOCALID_SLATEPORT_GABBY, SlateportCity_Movement_GabbyExit
	applymovement LOCALID_SLATEPORT_TY, SlateportCity_Movement_TyExit
	waitmovement 0
	removeobject LOCALID_SLATEPORT_GABBY
	removeobject LOCALID_SLATEPORT_TY
	msgbox SlateportCity_Text_SternWhewFirstInterview, MSGBOX_DEFAULT
	applymovement LOCALID_SLATEPORT_CAPT_STERN, Common_Movement_WalkInPlaceFasterUp
	waitmovement 0
	msgbox SlateportCity_Text_OhPlayerWeMadeDiscovery, MSGBOX_DEFAULT
	playbgm MUS_ENCOUNTER_AQUA, FALSE
	msgbox SlateportCity_Text_AquaWillAssumeControlOfSubmarine, MSGBOX_DEFAULT
	applymovement LOCALID_SLATEPORT_COOK, Common_Movement_WalkInPlaceFasterLeft
	applymovement LOCALID_SLATEPORT_FAT_MAN, Common_Movement_WalkInPlaceFasterLeft
	applymovement LOCALID_SLATEPORT_OLD_WOMAN, SlateportCity_Movement_OldWomanConcern
	applymovement LOCALID_SLATEPORT_RICH_BOY, Common_Movement_QuestionMark
	applymovement LOCALID_SLATEPORT_MAN_1, SlateportCity_Movement_ManConcern
	waitmovement 0
	applymovement LOCALID_SLATEPORT_CAPT_STERN, Common_Movement_WalkInPlaceFasterDown
	waitmovement 0
	msgbox SlateportCity_Text_SternWhatWasAllThat, MSGBOX_DEFAULT
	playse SE_PIN
	applymovement LOCALID_SLATEPORT_CAPT_STERN, Common_Movement_ExclamationMark
	waitmovement 0
	applymovement LOCALID_SLATEPORT_CAPT_STERN, Common_Movement_Delay48
	waitmovement 0
	applymovement LOCALID_SLATEPORT_CAPT_STERN, Common_Movement_WalkInPlaceFasterUp
	waitmovement 0
	msgbox SlateportCity_Text_FromHarborTryingToTakeSub, MSGBOX_DEFAULT
	msgbox SlateportCity_Text_PleaseComeWithMe, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_SLATEPORT_CAPT_STERN, SlateportCity_Movement_SternEnterHarbor
	applymovement LOCALID_PLAYER, SlateportCity_Movement_PlayerEnterHarbor
	waitmovement 0
	removeobject LOCALID_SLATEPORT_CAPT_STERN
	clearflag FLAG_HIDE_SLATEPORT_CITY_HARBOR_CAPTAIN_STERN
	clearflag FLAG_HIDE_SLATEPORT_CITY_HARBOR_SUBMARINE_SHADOW
	clearflag FLAG_HIDE_SLATEPORT_CITY_HARBOR_AQUA_GRUNT
	clearflag FLAG_HIDE_SLATEPORT_CITY_HARBOR_ARCHIE
	setvar VAR_SLATEPORT_CITY_STATE, 2
	warp MAP_SLATEPORT_CITY_HARBOR, 11, 14
	waitstate
	releaseall
	end

SlateportCity_Movement_OldWomanConcern:
	delay_16
	delay_16
	emote_question_mark
	walk_in_place_faster_right
	delay_16
	delay_16
	walk_in_place_faster_left
	step_end

SlateportCity_Movement_ManConcern:
	emote_question_mark
	walk_in_place_faster_up
	delay_16
	walk_in_place_faster_down
	delay_16
	delay_16
	walk_in_place_faster_left
	step_end

SlateportCity_Movement_GabbyExit:
	delay_16
	walk_left
	walk_left
	walk_down
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	step_end

SlateportCity_Movement_TyExit:
	walk_down
	walk_left
	walk_left
	walk_left
	walk_down
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	step_end

SlateportCity_Movement_Unused:
	walk_down
	walk_in_place_faster_up
	step_end

SlateportCity_Movement_SternEnterHarbor:
	walk_right
	walk_up
	walk_up
	set_invisible
	step_end

SlateportCity_Movement_SternWatchGabbyAndTyExit:
	delay_16
	delay_16
	delay_16
	delay_16
	delay_16
	walk_down
	walk_left
	step_end

SlateportCity_Movement_PlayerEnterHarbor:
	walk_in_place_faster_right
	delay_16
	delay_16
	walk_right
	walk_up
	step_end

SlateportCity_Movement_PlayerFaceStern:
	delay_16
	delay_16
	delay_16
	delay_16
	delay_16
	delay_16
	delay_16
	walk_in_place_faster_down
	step_end

SlateportCity_EventScript_Ty::
	msgbox SlateportCity_Text_BigSmileForCamera, MSGBOX_SIGN
	end

SlateportCity_EventScript_Gabby::
	msgbox SlateportCity_Text_MostInvaluableExperience, MSGBOX_SIGN
	end

SlateportCity_EventScript_Man3::
	msgbox SlateportCity_Text_WonderIfLighthouseStartlesPokemon, MSGBOX_NPC
	end

SlateportCity_EventScript_BerryPowderClerk::
	lock
	faceplayer
	goto_if_set FLAG_RECEIVED_POWDER_JAR, SlateportCity_EventScript_ReceivedPowderJar
	msgbox SlateportCity_Text_ExplainBerries, MSGBOX_DEFAULT
	giveitem ITEM_POWDER_JAR
	setflag FLAG_RECEIVED_POWDER_JAR
	msgbox SlateportCity_Text_ExplainBerryPowder, MSGBOX_DEFAULT
	release
	end

SlateportCity_EventScript_ReceivedPowderJar::
	setvar VAR_0x8004, 1
	specialvar VAR_RESULT, HasEnoughBerryPowder
	goto_if_eq VAR_RESULT, FALSE, SlateportCity_EventScript_ExplainBerryPowder
	msgbox SlateportCity_Text_BroughtMeSomeBerryPowder, MSGBOX_DEFAULT
	special DisplayBerryPowderVendorMenu
	goto SlateportCity_EventScript_ChooseBerryPowderItem
	end

SlateportCity_EventScript_ExplainBerryPowder::
	msgbox SlateportCity_Text_ExplainBerryPowder, MSGBOX_DEFAULT
	release
	end

SlateportCity_EventScript_ChooseBerryPowderItem::
	message SlateportCity_Text_ExchangeWhatWithIt
	waitmessage
	setvar VAR_0x8004, SCROLL_MULTI_BERRY_POWDER_VENDOR
	special ShowScrollableMultichoice
	switch VAR_RESULT
	case 0, SlateportCity_EventScript_EnergyPowder
	case 1, SlateportCity_EventScript_EnergyRoot
	case 2, SlateportCity_EventScript_HealPowder
	case 3, SlateportCity_EventScript_RevivalHerb
	case 4, SlateportCity_EventScript_Protein
	case 5, SlateportCity_EventScript_Iron
	case 6, SlateportCity_EventScript_Carbos
	case 7, SlateportCity_EventScript_Calcium
	case 8, SlateportCity_EventScript_Zinc
	case 9, SlateportCity_EventScript_HPUp
	case 10, SlateportCity_EventScript_PPUp
	case 11, SlateportCity_EventScript_CancelPowderItemSelect
	case MULTI_B_PRESSED, SlateportCity_EventScript_CancelPowderItemSelect
	end

SlateportCity_EventScript_EnergyPowder::
	bufferitemname STR_VAR_1, ITEM_ENERGY_POWDER
	setvar VAR_0x8008, ITEM_ENERGY_POWDER
	setvar VAR_0x8009, 50
	goto SlateportCity_EventScript_TryBuyBerryPowderItem
	end

SlateportCity_EventScript_EnergyRoot::
	bufferitemname STR_VAR_1, ITEM_ENERGY_ROOT
	setvar VAR_0x8008, ITEM_ENERGY_ROOT
	setvar VAR_0x8009, 80
	goto SlateportCity_EventScript_TryBuyBerryPowderItem
	end

SlateportCity_EventScript_HealPowder::
	bufferitemname STR_VAR_1, ITEM_HEAL_POWDER
	setvar VAR_0x8008, ITEM_HEAL_POWDER
	setvar VAR_0x8009, 50
	goto SlateportCity_EventScript_TryBuyBerryPowderItem
	end

SlateportCity_EventScript_RevivalHerb::
	bufferitemname STR_VAR_1, ITEM_REVIVAL_HERB
	setvar VAR_0x8008, ITEM_REVIVAL_HERB
	setvar VAR_0x8009, 300
	goto SlateportCity_EventScript_TryBuyBerryPowderItem
	end

SlateportCity_EventScript_Protein::
	bufferitemname STR_VAR_1, ITEM_PROTEIN
	setvar VAR_0x8008, ITEM_PROTEIN
	setvar VAR_0x8009, 1000
	goto SlateportCity_EventScript_TryBuyBerryPowderItem
	end

SlateportCity_EventScript_Iron::
	bufferitemname STR_VAR_1, ITEM_IRON
	setvar VAR_0x8008, ITEM_IRON
	setvar VAR_0x8009, 1000
	goto SlateportCity_EventScript_TryBuyBerryPowderItem
	end

SlateportCity_EventScript_Carbos::
	bufferitemname STR_VAR_1, ITEM_CARBOS
	setvar VAR_0x8008, ITEM_CARBOS
	setvar VAR_0x8009, 1000
	goto SlateportCity_EventScript_TryBuyBerryPowderItem
	end

SlateportCity_EventScript_Calcium::
	bufferitemname STR_VAR_1, ITEM_CALCIUM
	setvar VAR_0x8008, ITEM_CALCIUM
	setvar VAR_0x8009, 1000
	goto SlateportCity_EventScript_TryBuyBerryPowderItem
	end

SlateportCity_EventScript_Zinc::
	bufferitemname STR_VAR_1, ITEM_ZINC
	setvar VAR_0x8008, ITEM_ZINC
	setvar VAR_0x8009, 1000
	goto SlateportCity_EventScript_TryBuyBerryPowderItem
	end

SlateportCity_EventScript_HPUp::
	bufferitemname STR_VAR_1, ITEM_HP_UP
	setvar VAR_0x8008, ITEM_HP_UP
	setvar VAR_0x8009, 1000
	goto SlateportCity_EventScript_TryBuyBerryPowderItem
	end

SlateportCity_EventScript_PPUp::
	bufferitemname STR_VAR_1, ITEM_PP_UP
	setvar VAR_0x8008, ITEM_PP_UP
	setvar VAR_0x8009, 3000
	goto SlateportCity_EventScript_TryBuyBerryPowderItem
	end

SlateportCity_EventScript_CancelPowderItemSelect::
	msgbox SlateportCity_Text_ComeBackToTradeBerryPowder, MSGBOX_DEFAULT
	special RemoveBerryPowderVendorMenu
	release
	end

SlateportCity_EventScript_TryBuyBerryPowderItem::
	msgbox SlateportCity_Text_ExchangeBerryPowderForItem, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, SlateportCity_EventScript_ChooseBerryPowderItem
	copyvar VAR_0x8004, VAR_0x8009
	specialvar VAR_RESULT, HasEnoughBerryPowder
	goto_if_eq VAR_RESULT, FALSE, SlateportCity_EventScript_NotEnoughBerryPowder
	giveitem VAR_0x8008
	goto_if_eq VAR_RESULT, FALSE, SlateportCity_EventScript_NoRoomForBerryPowderItem
	copyvar VAR_0x8004, VAR_0x8009
	special TakeBerryPowder
	special PrintPlayerBerryPowderAmount
	msgbox SlateportCity_Text_FineBerryPowderTradeSomethingElse, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, YES, SlateportCity_EventScript_ChooseBerryPowderItem
	msgbox SlateportCity_Text_WhenYouGetMoreBringItToMe, MSGBOX_DEFAULT
	special RemoveBerryPowderVendorMenu
	release
	end

SlateportCity_EventScript_NoRoomForBerryPowderItem::
	msgbox gText_TheBagIsFull, MSGBOX_DEFAULT
	special RemoveBerryPowderVendorMenu
	release
	end

SlateportCity_EventScript_NotEnoughBerryPowder::
	msgbox SlateportCity_Text_DontHaveEnoughBerryPowder, MSGBOX_DEFAULT
	goto SlateportCity_EventScript_ChooseBerryPowderItem
	end

SlateportCity_EventScript_ScottBattleTentScene::
	lockall
	applymovement LOCALID_PLAYER, Common_Movement_FaceUp
	waitmovement 0
	opendoor 10, 12
	waitdooranim
	addobject LOCALID_SLATEPORT_SCOTT
	applymovement LOCALID_PLAYER, SlateportCity_Movement_PushPlayerDown
	applymovement LOCALID_SLATEPORT_SCOTT, SlateportCity_Movement_ScottExitBattleTent
	waitmovement 0
	closedoor 10, 12
	waitdooranim
	msgbox SlateportCity_Text_TakingBattleTentChallenge, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_PLAYER, SlateportCity_Movement_PlayerWatchScottExit
	applymovement LOCALID_SLATEPORT_SCOTT, SlateportCity_Movement_ScottExitAfterBattleTent
	waitmovement 0
	removeobject LOCALID_SLATEPORT_SCOTT
	addvar VAR_SCOTT_STATE, 1
	setvar VAR_SLATEPORT_OUTSIDE_MUSEUM_STATE, 3
	releaseall
	end

SlateportCity_Movement_PushPlayerDown:
	lock_facing_direction
	walk_down
	unlock_facing_direction
	step_end

SlateportCity_Movement_PlayerWatchScottExit:
	delay_16
	walk_in_place_faster_right
	step_end

SlateportCity_Movement_ScottExitBattleTent:
	delay_8
	walk_down
	step_end

SlateportCity_Movement_ScottExitAfterBattleTent:
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	step_end

SlateportCity_EventScript_BerryCrushRankingsSign::
	lockall
	special ShowBerryCrushRankings
	releaseall
	end

SlateportCity_Text_EnergyGuruSellWhatYouNeed:
	.string "Je suis le GOUROU DE L'ÉNERGIE ! Je vais\n"
	.string "tout donner pour te vendre ce qu'il te\l"
	.string "faut !$"

SlateportCity_Text_OhYourPokemon:
	.string "Oh ? Ton {STR_VAR_1}…$"

SlateportCity_Text_PleaseGiveItThisEffortRibbon:
	.string "Il s'est entraîné comme une bête !\p"
	.string "En récompense, donne-lui ce Ruban\n"
	.string "Effort.$"

SlateportCity_Text_ReceivedEffortRibbon:
	.string "{PLAYER} reçoit le Ruban Effort.$"

SlateportCity_Text_PutEffortRibbonOnMon:
	.string "{PLAYER} accroche le Ruban Effort à\n"
	.string "{STR_VAR_1}.$"

SlateportCity_Text_GoForItLittleHarder:
	.string "Il doit encore se donner un peu plus.\p"
	.string "Après, je lui offrirai un joli cadeau.$"

SlateportCity_Text_EffortRibbonLooksGoodOnIt:
	.string "Oh ! Ton {STR_VAR_1}, ce Ruban Effort\n"
	.string "lui va trop bien !$"

SlateportCity_Text_WonderIfLighthouseStartlesPokemon:
	.string "La lumière du phare porte à des dizaines\n"
	.string "de kilomètres.\p"
	.string "Je me demande si elle ne fait pas peur\n"
	.string "aux Pokémon de la mer. Ou si elle les\l"
	.string "attire.$"

SlateportCity_Text_SeaweedFullOfLife:
	.string "Oh, regarde ça !\p"
	.string "Les algues d'ici sont fraîches et\n"
	.string "pleines de vie.\p"
	.string "On dirait qu'elles pourraient se dresser\n"
	.string "et t'attaquer !$"

SlateportCity_Text_HowTownIsBornAndGrows:
	.string "Là où l'eau est pure, on récolte des\n"
	.string "fruits en abondance.\p"
	.string "Là où les gens se rassemblent, un\n"
	.string "marché naît.\p"
	.string "C'est comme ça qu'une ville naît et\n"
	.string "grandit. Et qu'elle finit par oublier d'où\l"
	.string "elle vient.$"

SlateportCity_Text_SlateportWonderfulPlace:
	.string "Faire ses courses en respirant l'air\n"
	.string "marin…\p"
	.string "Poivressel, c'est un endroit merveilleux\n"
	.string "!$"

SlateportCity_Text_BuyBricksSoDecorWontGetDirty:
	.string "Si tu poses tes Poupées ou tes\n"
	.string "Coussins par terre, ils se salissent.\p"
	.string "Je vais acheter des Briques pour que\n"
	.string "mes Poupées restent propres. On a ses\l"
	.string "priorités.$"

SlateportCity_Text_GoingToCompeteInBattleTent:
	.string "Wroooar ! Moi aussi, je vais participer à\n"
	.string "la Tente de Combat !\p"
	.string "Mais d'abord, faut que je capture des\n"
	.string "Pokémon !$"

SlateportCity_Text_BushedHikingFromMauville:
	.string "Pfiou… Je suis lessivé…\p"
	.string "Je suis venu à pied de Lavandia. Mais\n"
	.string "cette ville est immense.\p"
	.string "Si j'avais su, j'aurais pris mon Vélo.$"

SlateportCity_Text_EveryoneCallsHimCaptStern:
	.string "Stern, le type qui a construit le Musée,\n"
	.string "dirige aussi une équipe d'exploration\l"
	.string "sous-marine.\p"
	.string "Alors tout le monde l'appelle Capitaine\n"
	.string "Stern.$"

SlateportCity_Text_WhatsLongLineOverThere:
	.string "C'est quoi, là-bas ? Cette longue file…$"

SlateportCity_Text_VisitedMuseumOften:
	.string "Petit, j'allais souvent au Musée.\p"
	.string "Je rêvais des mystères de la mer en\n"
	.string "regardant les expositions.$"

SlateportCity_Text_QuitPushing:
	.string "Hé ! Arrête de pousser ! C'est la file,\n"
	.string "t'es aveugle ?$"

SlateportCity_Text_AquaHasPolicy:
	.string "La Team Aqua a une règle : on se\n"
	.string "rassemble et on se disperse sur le lieu\l"
	.string "de l'opération.$"

SlateportCity_Text_BossIsBrilliant:
	.string "Notre Boss est un génie.\p"
	.string "Mais qu'est-ce qu'il veut faire d'un\n"
	.string "Musée ? Brûler les tableaux ?$"

SlateportCity_Text_WhatsNewSchemeIWonder:
	.string "C'est quoi, le nouveau plan ?\p"
	.string "Le Boss fait peur quand il est en\n"
	.string "colère. Le dernier qui l'a déçu, on l'a\l"
	.string "plus revu…$"

SlateportCity_Text_ShouldTakeItAll:
	.string "S'il y a un truc qui nous intéresse au\n"
	.string "Musée, on prend tout !$"

SlateportCity_Text_DontButtIn:
	.string "Hé, toi ! Me double pas !$"

SlateportCity_Text_RemindsMeOfLongLineForGames:
	.string "Une longue file, hein ?\p"
	.string "Ça me rappelle les nuits à camper\n"
	.string "devant les magasins pour les sorties de\l"
	.string "jeux…$"

SlateportCity_Text_WhyAreWeLiningUp:
	.string "Pourquoi on fait la queue et on paie ?\n"
	.string "On devrait juste défoncer la porte !$"

SlateportCity_Text_WhatDoYouWant:
	.string "Quoi ? Tu veux quoi ?$"

SlateportCity_Text_IllReadSignForYou:
	.string "Tu veux lire ce panneau ? Je vais te le\n"
	.string "lire !$"

SlateportCity_Text_SaysSomethingLikeSeaIsEndless:
	.string "Voyons…\p"
	.string "Euh… Je crois que c'est écrit “la vie de\n"
	.string "la mer est infinie”.\p"
	.string "Ouais, j'en suis presque sûr. J'ai pas\n"
	.string "mes lunettes.$"

SlateportCity_Text_ShouldveBroughtMyGameBoy:
	.string "Grmbl…\p"
	.string "J'aurais dû prendre ma Game Boy Advance\n"
	.string "pour m'occuper dans la queue…\p"
	.string "Grmbl…$"

SlateportCity_Text_HotSpringsAfterOperation:
	.string "Après l'opération, je t'emmène aux\n"
	.string "sources chaudes !\p"
	.string "C'est ce que notre chef a promis. J'ai\n"
	.string "trop hâte ! On le mérite, après tout ce\l"
	.string "sang.$"

SlateportCity_Text_SeaIsSoWet:
	.string "La mer est tellement immense…\p"
	.string "Et si la mer était faite des larmes des\n"
	.string "Pokémon ?$"

SlateportCity_Text_SinkOldBoats:
	.string "Tu sais ce qu'on fait des vieux bateaux\n"
	.string "qui ne naviguent plus ?\p"
	.string "On les coule, pour qu'ils deviennent des\n"
	.string "refuges pour les Pokémon. Une seconde\l"
	.string "vie, au fond.$"

SlateportCity_Text_BuyTooMuch:
	.string "Chaque fois que je viens ici, je craque\n"
	.string "et j'achète n'importe quoi.$"

SlateportCity_Text_GetNameRaterToHelpYou:
	.string "Pour changer le surnom d'un Pokémon, il\n"
	.string "faut aller voir l'Expert Surnom.$"

SlateportCity_Text_CantChangeTradeMonName:
	.string "Un Pokémon obtenu par échange, tu ne\n"
	.string "peux pas changer son surnom.\p"
	.string "L'amour de son premier Dresseur est\n"
	.string "dans ce nom.$"

SlateportCity_Text_BattleTentBuiltRecently:
	.string "Une Tente de Combat vient d'ouvrir à\n"
	.string "Poivressel.\p"
	.string "Les Arènes, c'est sympa, mais la Tente\n"
	.string "de Combat, c'est autre chose.\p"
	.string "Va trouver des Pokémon costauds pour\n"
	.string "la Tente !$"

SlateportCity_Text_CaptSternBeingInterviewed:
	.string "J'espérais une star pour avoir un\n"
	.string "autographe.\p"
	.string "Mais c'est qui, qu'on interviewe ? C'est\n"
	.string "pas le Capitaine Stern ?$"

SlateportCity_Text_InterviewerSoCool:
	.string "La journaliste est trop classe.\p"
	.string "Quand je serai grande, je serai reporter\n"
	.string "internationale !$"

SlateportCity_Text_SternSaysDiscoveredSomething:
	.string "Le Capitaine Stern dit qu'ils ont\n"
	.string "découvert un truc au fond de la mer.\p"
	.string "C'est quoi ? C'est quoi ?$"

SlateportCity_Text_CaptainComeBackWithBigFish:
	.string "Qu'est-ce qui se passe ?\p"
	.string "Le Capitaine est revenu avec un\n"
	.string "monstre des abysses ?$"

SlateportCity_Text_AmIOnTV:
	.string "Hé ! Vous regardez ? Je passe à la télé ?$"

SlateportCity_Text_CaptainsACelebrity:
	.string "Une interview télé ! Ici ! Le Capitaine\n"
	.string "est une star !$"

SlateportCity_Text_BigSmileForCamera:
	.string "Ty : Bon, Capitaine Stern, un grand\n"
	.string "sourire pour la caméra !$"

SlateportCity_Text_MostInvaluableExperience:
	.string "Gabby : Je vois, je vois. Une expérience\n"
	.string "inestimable…$"

SlateportCity_Text_SternMoveAheadWithExploration:
	.string "Capitaine Stern : Oui. Nous comptons\n"
	.string "poursuivre l'exploration.$"

SlateportCity_Text_GabbyWonderfulThanksForInterview:
	.string "Gabby : Formidable, Capitaine Stern !\n"
	.string "Merci d'avoir pris le temps de nous\l"
	.string "parler.\p"
	.string "Nous espérons vous revoir bientôt avec\n"
	.string "de nouvelles découvertes !$"

SlateportCity_Text_SternWhewFirstInterview:
	.string "Capitaine Stern : Pfiou…\p"
	.string "C'était ma première fois à la télé. J'ai\n"
	.string "transpiré comme dans un sous-marin en\l"
	.string "panne.$"

SlateportCity_Text_OhPlayerWeMadeDiscovery:
	.string "Oh ! {PLAYER} ! Tu as l'air en forme !\p"
	.string "On a fait une découverte énorme lors de\n"
	.string "notre dernière plongée.\p"
	.string "Une caverne sous-marine, sur la Route\n"
	.string "128.\p"
	.string "On pense que c'est l'habitat d'un\n"
	.string "Pokémon qu'on croyait disparu depuis\l"
	.string "des millénaires.$"

SlateportCity_Text_AquaWillAssumeControlOfSubmarine:
	.string "Hu hu hu… Le Capitaine Stern, je\n"
	.string "présume.\p"
	.string "La Team Aqua réquisitionne votre\n"
	.string "sous-marin !\p"
	.string "Vos objections ne valent rien.\n"
	.string "Coopérez, ou on vous coule avec.\p"
	.string "Hu hu hu… Regardez bien ce que la Team\n"
	.string "Aqua a prévu !$"

SlateportCity_Text_SternWhatWasAllThat:
	.string "Capitaine Stern : C'était quoi, ça ?\p"
	.string "On aurait dit quelqu'un avec un\n"
	.string "mégaphone…\p"
	.string "Ça venait d'où ?$"

SlateportCity_Text_FromHarborTryingToTakeSub:
	.string "Du port !\p"
	.string "Le sous-marin ! Ils essaient de le voler\n"
	.string "!$"

SlateportCity_Text_PleaseComeWithMe:
	.string "{PLAYER} ! Viens avec moi, vite !$"

SlateportCity_Text_BattleTentSign:
	.string "Tente de Combat de Poivressel\n"
	.string "“Trouve-le ! L'ultime Pokémon !”$"

SlateportCity_Text_SternsShipyardWantedSign:
	.string "Chantier naval de Stern\p"
	.string "“Recherche : marin capable de naviguer\n"
	.string "dans tous les courants.”$"

SlateportCity_Text_SternsShipyardNearsCompletion:
	.string "Chantier naval de Stern\p"
	.string "“Le ferry M.S. Marée, qui reliera\n"
	.string "Poivressel et Nénucrique, est presque\l"
	.string "terminé.”$"

SlateportCity_Text_SternsShipyardFerryComplete:
	.string "Chantier naval de Stern\p"
	.string "“L'embarquement du ferry M.S. Marée\n"
	.string "(Poivressel - Nénucrique) se fait au\l"
	.string "port.”$"

SlateportCity_Text_PokemonFanClubSign:
	.string "Fan Club Pokémon\n"
	.string "“Appel à tous les fans de Pokémon !”$"

SlateportCity_Text_OceanicMuseumSign:
	.string "“La mer infinie nourrit toute vie.”\p"
	.string "Musée Océanographique$"

SlateportCity_Text_CitySign:
	.string "Poivressel\p"
	.string "“Le port où se croisent les gens et les\n"
	.string "Pokémon.”$"

SlateportCity_Text_MarketSign:
	.string "Marché de Poivressel\n"
	.string "“Des objets introuvables ailleurs !”$"

SlateportCity_Text_HarborFerryUnderConstruction:
	.string "Port de Poivressel\p"
	.string "“Le ferry M.S. Marée est en\n"
	.string "construction au chantier naval.\p"
	.string "“Mise en service prochainement.”$"

SlateportCity_Text_HarborSign:
	.string "Port de Poivressel\p"
	.string "“Profitez d'une croisière à bord du\n"
	.string "ferry M.S. Marée.”$"

SlateportCity_Text_NameRatersHouseSign:
	.string "Maison de l'Expert Surnom\n"
	.string "“Vos surnoms de Pokémon évalués.”$"

SlateportCity_Text_ExplainBerries:
	.string "Quand un Pokémon sauvage est blessé, il\n"
	.string "mâchonne des Baies pour se soigner.\p"
	.string "Tu le savais ?\p"
	.string "Quelqu'un l'a vu faire et a eu l'idée d'en\n"
	.string "tirer des médicaments.\p"
	.string "Tu le savais ?\p"
	.string "Pour transformer des Baies en\n"
	.string "médicament, il faut les réduire en\l"
	.string "poudre.\p"
	.string "Tu le savais ?\p"
	.string "Les Baies ont l'air de t'intéresser !\p"
	.string "J'ai un petit truc pour quelqu'un comme\n"
	.string "toi.$"

SlateportCity_Text_ExplainBerryPowder:
	.string "J'ai fait installer des machines pour\n"
	.string "écraser les Baies dans les Centres\l"
	.string "Pokémon.\p"
	.string "Les machines Écrase-Baies sont au Coin\n"
	.string "Direct, à l'étage.\p"
	.string "Tu le savais ?\p"
	.string "Tu pourrais me faire de la Poudre de\n"
	.string "Baie avec ?\p"
	.string "Si tu m'en apportes beaucoup, je te\n"
	.string "fabrique toutes sortes de médicaments.$"

SlateportCity_Text_BroughtMeSomeBerryPowder:
	.string "Tu m'as apporté de la Poudre de Baie ?$"

SlateportCity_Text_ExchangeWhatWithIt:
	.string "Contre quoi veux-tu l'échanger ?$"

SlateportCity_Text_ExchangeBerryPowderForItem:
	.string "D'accord, ta Poudre de Baie contre\n"
	.string "{STR_VAR_1} ?$"

SlateportCity_Text_DontHaveEnoughBerryPowder:
	.string "Oh là là. Tu n'as pas assez de Poudre de\n"
	.string "Baie.$"

SlateportCity_Text_FineBerryPowderTradeSomethingElse:
	.string "Belle Poudre de Baie. Ça fera\n"
	.string "d'excellents médicaments.\p"
	.string "Tu veux échanger encore un peu de\n"
	.string "Poudre ?$"

SlateportCity_Text_WhenYouGetMoreBringItToMe:
	.string "D'accord ! Quand tu en auras plus,\n"
	.string "rapporte-la-moi !$"

SlateportCity_Text_ComeBackToTradeBerryPowder:
	.string "Reviens échanger ta Poudre de Baie\n"
	.string "contre des médicaments.\p"
	.string "Mon bazar est toujours ouvert.\p"
	.string "Tu le savais ?$"

SlateportCity_Text_YouDroveTeamAquaAway:
	.string "Scott : Hein ? Je suis sûr qu'on s'est\n"
	.string "déjà croisés.\p"
	.string "Je me suis présenté ? Sinon, moi, c'est\n"
	.string "Scott.\p"
	.string "Je viens de voir la Team Aqua détaler\n"
	.string "d'ici comme si elle avait le feu aux\l"
	.string "fesses.\p"
	.string "Laisse-moi deviner… c'est toi qui les as\n"
	.string "fait fuir ?$"

SlateportCity_Text_MaybeThisTrainer:
	.string "Scott : Hmm… Peut-être que ce\n"
	.string "Dresseur…$"

SlateportCity_Text_LetsRegisterEachOther:
	.string "Scott : Parfait ! Je sens qu'on va bien\n"
	.string "s'entendre.\p"
	.string "Enregistrons-nous dans nos PokéNav.\p"
	.string "… … … … … …$"

SlateportCity_Text_RegisteredScott:
	.string "Ce Scott a été enregistré dans le\n"
	.string "PokéNav.$"

SlateportCity_Text_KeepEyeOnTrainersBeSeeingYou:
	.string "Scott : J'aimerais bien te suivre, mais\n"
	.string "je dois aussi garder un œil sur les\l"
	.string "autres talents.\p"
	.string "Je vais faire le tour des autres villes.\p"
	.string "À plus, {PLAYER} !$"

SlateportCity_Text_TakingBattleTentChallenge:
	.string "Scott : Tiens ! Mais c'est {PLAYER} !\p"
	.string "Laisse-moi deviner, tu vas tenter la\n"
	.string "Tente de Combat ?\p"
	.string "Ouais ! Excellente idée ! Vraiment\n"
	.string "excellente !\p"
	.string "Un Dresseur coriace, c'est exactement\n"
	.string "ce qu'il faut pour la Tente de Combat !\p"
	.string "Donne tout !$"
