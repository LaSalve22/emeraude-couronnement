RustboroCity_Flat1_2F_MapScripts::
	.byte 0

RustboroCity_Flat1_2F_EventScript_WaldasDad::
	lock
	faceplayer
	specialvar VAR_RESULT, TryBufferWaldaPhrase
	goto_if_eq VAR_RESULT, FALSE, RustboroCity_Flat1_2F_EventScript_WaldasDadFirstPhrase
	goto_if_eq VAR_RESULT, TRUE, RustboroCity_Flat1_2F_EventScript_WaldasDadNewPhrase

RustboroCity_Flat1_2F_EventScript_GivePhrase::
	special DoWaldaNamingScreen
	goto_if_eq VAR_0x8004, 1, RustboroCity_Flat1_2F_EventScript_CancelGivePhrase
	goto_if_eq VAR_0x8004, 2, RustboroCity_Flat1_2F_EventScript_CancelGiveFirstPhrase
	specialvar VAR_RESULT, TryGetWallpaperWithWaldaPhrase
	goto_if_eq VAR_RESULT, TRUE, RustboroCity_Flat1_2F_EventScript_WaldaLikesPhrase
	goto_if_eq VAR_RESULT, FALSE, RustboroCity_Flat1_2F_EventScript_WaldaDoesntLikePhrase
	end

RustboroCity_Flat1_2F_EventScript_WaldasDadFirstPhrase::
	msgbox RustboroCity_Flat1_2F_Text_HelloDoYouKnowFunnyPhrase, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, RustboroCity_Flat1_2F_EventScript_DeclineGivePhrase
	msgbox RustboroCity_Flat1_2F_Text_WonderfulLetsHearSuggestion, MSGBOX_DEFAULT
	goto RustboroCity_Flat1_2F_EventScript_GivePhrase

RustboroCity_Flat1_2F_EventScript_WaldasDadNewPhrase::
	msgbox RustboroCity_Flat1_2F_Text_BeenSayingXDoYouKnowBetterPhrase, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, RustboroCity_Flat1_2F_EventScript_DeclineGivePhrase
	msgbox RustboroCity_Flat1_2F_Text_WonderfulLetsHearSuggestion, MSGBOX_DEFAULT
	goto RustboroCity_Flat1_2F_EventScript_GivePhrase

RustboroCity_Flat1_2F_EventScript_DeclineGivePhrase::
	msgbox RustboroCity_Flat1_2F_Text_OhIsThatRight, MSGBOX_DEFAULT
	release
	end

RustboroCity_Flat1_2F_EventScript_CancelGivePhrase::
	msgbox RustboroCity_Flat1_2F_Text_OhYouDontKnowAny, MSGBOX_DEFAULT
	release
	end

RustboroCity_Flat1_2F_EventScript_CancelGiveFirstPhrase::
	msgbox RustboroCity_Flat1_2F_Text_ThinkOfMyOwnPhrase, MSGBOX_DEFAULT
	call RustboroCity_Flat1_2F_EventScript_WaldasDadFaceWalda
	msgbox RustboroCity_Flat1_2F_Text_ShesNotSmilingAtAll2, MSGBOX_DEFAULT
	release
	end

RustboroCity_Flat1_2F_EventScript_WaldaLikesPhrase::
	msgbox RustboroCity_Flat1_2F_Text_LetsGiveItATry2, MSGBOX_DEFAULT
	call RustboroCity_Flat1_2F_EventScript_WaldasDadFaceWalda
	msgbox RustboroCity_Flat1_2F_Text_OhShesLaughing, MSGBOX_DEFAULT
	applymovement LOCALID_WALDAS_DAD, Common_Movement_FacePlayer
	waitmovement 0
	msgbox RustboroCity_Flat1_2F_Text_ThankYouIllGiveYouWallpaper, MSGBOX_DEFAULT
	release
	end

RustboroCity_Flat1_2F_EventScript_WaldaDoesntLikePhrase::
	msgbox RustboroCity_Flat1_2F_Text_LetsGiveItATry, MSGBOX_DEFAULT
	call RustboroCity_Flat1_2F_EventScript_WaldasDadFaceWalda
	msgbox RustboroCity_Flat1_2F_Text_ShesNotSmilingAtAll, MSGBOX_DEFAULT
	release
	end

RustboroCity_Flat1_2F_EventScript_WaldasDadFaceWalda::
	turnobject LOCALID_WALDAS_DAD, DIR_EAST
	return

RustboroCity_Flat1_2F_EventScript_WaldasMom::
	msgbox RustboroCity_Flat1_2F_Text_ComingUpWithMealsIsHard, MSGBOX_NPC
	end

RustboroCity_Flat1_2F_EventScript_PokeDoll::
	msgbox RustboroCity_Flat1_2F_Text_ItsAPokemonPlushDoll, MSGBOX_SIGN
	end

RustboroCity_Flat1_2F_Text_ComingUpWithMealsIsHard:
	.string "Oh, c'est tellement dur, tous les jours…\p"
	.string "Quoi donc ? Tu as besoin de demander ?\p"
	.string "Trouver quoi faire à manger. Tous les\n"
	.string "jours.\p"
	.string "Franchement, c'est pire que n'importe\n"
	.string "quel combat de Pokémon.$"

RustboroCity_Flat1_2F_Text_HelloDoYouKnowFunnyPhrase:
	.string "Oh, bonjour ! Bienvenue chez les Pepper.\p"
	.string "J'ai une question. Tu as déjà gardé des\n"
	.string "enfants ?\p"
	.string "Je suis jeune papa, alors tout ça est\n"
	.string "nouveau pour moi.\p"
	.string "Et j'ai un problème. Ma fille, Walda, ne\n"
	.string "rit pas assez. Les médecins disent que\l"
	.string "tout va bien, mais…\p"
	.string "Je pense qu'elle rirait si je lui disais\n"
	.string "un truc drôle.\p"
	.string "Tu connais un mot ou une phrase\n"
	.string "rigolote ?$"

RustboroCity_Flat1_2F_Text_BeenSayingXDoYouKnowBetterPhrase:
	.string "Ces temps-ci, je lui dis “{STR_VAR_1}”\n"
	.string "pour l'amuser.\p"
	.string "Tu connais un mot ou une phrase qui\n"
	.string "marcherait mieux ?$"

RustboroCity_Flat1_2F_Text_WonderfulLetsHearSuggestion:
	.string "Oh, formidable. Alors, vas-y, je\n"
	.string "t'écoute.$"

RustboroCity_Flat1_2F_Text_OhIsThatRight:
	.string "Ah bon ?\p"
	.string "Si une bonne idée te vient, je suis tout\n"
	.string "ouïe.$"

RustboroCity_Flat1_2F_Text_LetsGiveItATry2:
	.string "Ah, je vois. Bon, essayons, d'accord ?$"

RustboroCity_Flat1_2F_Text_OhShesLaughing:
	.string "{STR_VAR_1}. {STR_VAR_1}.\p"
	.string "Oh, oui ! Elle rit ! Oh, je suis aussi\n"
	.string "heureux qu'elle !$"

RustboroCity_Flat1_2F_Text_LetsGiveItATry:
	.string "Ah, je vois. Bon, essayons, d'accord ?$"

RustboroCity_Flat1_2F_Text_ShesNotSmilingAtAll:
	.string "{STR_VAR_1}. {STR_VAR_1}.\p"
	.string "Hmmm… Pas l'ombre d'un sourire. Walda\n"
	.string "est peut-être une enfant très\l"
	.string "sérieuse…$"

RustboroCity_Flat1_2F_Text_ThinkOfMyOwnPhrase:
	.string "Oh, tu n'as pas d'idée. Je vais devoir\n"
	.string "trouver tout seul, alors.\p"
	.string "Hmm… Et pourquoi pas “{STR_VAR_1}” ?\n"
	.string "Voyons si ça marche.$"

RustboroCity_Flat1_2F_Text_ShesNotSmilingAtAll2:
	.string "{STR_VAR_1}. {STR_VAR_1}.\p"
	.string "Hmmm… Pas l'ombre d'un sourire. Walda\n"
	.string "est peut-être une enfant très\l"
	.string "sérieuse…$"

RustboroCity_Flat1_2F_Text_OhYouDontKnowAny:
	.string "Oh, tu n'as pas d'idée. Je vais continuer\n"
	.string "avec ma vieille formule, alors.\p"
	.string "En tout cas, si une bonne idée te vient,\n"
	.string "n'hésite pas, d'accord ?$"

RustboroCity_Flat1_2F_Text_ThankYouIllGiveYouWallpaper:
	.string "Merci !\p"
	.string "Grâce à toi, ma petite Walda a ri ! C'est\n"
	.string "la première fois depuis des semaines.\p"
	.string "Je n'en ai pas l'air, mais je suis un des\n"
	.string "meilleurs chercheurs de Devon SA.\p"
	.string "Laisse-moi te rendre la pareille.\p"
	.string "Je vais ajouter de nouveaux fonds\n"
	.string "d'écran pour les Boîtes du système de\l"
	.string "stockage Pokémon.\p"
	.string "Dans le menu des fonds d'écran, choisis\n"
	.string "“AMIS”.\p"
	.string "Tu y trouveras les nouveaux motifs.$"

RustboroCity_Flat1_2F_Text_ItsAPokemonPlushDoll:
	.string "Une peluche Pokémon ! Un œil est\n"
	.string "décousu.$"

