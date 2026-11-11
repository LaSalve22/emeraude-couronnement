SlateportCity_SternsShipyard_1F_MapScripts::
	.byte 0

SlateportCity_SternsShipyard_1F_EventScript_Dock::
	lockall
	goto_if_set FLAG_SYS_GAME_CLEAR, SlateportCity_SternsShipyard_1F_EventScript_FerryReady
	goto_if_set FLAG_BADGE07_GET, SlateportCity_SternsShipyard_1F_EventScript_BrineyJoined
	goto_if_set FLAG_DELIVERED_DEVON_GOODS, SlateportCity_SternsShipyard_1F_EventScript_NeedVeteran
	goto_if_set FLAG_DOCK_REJECTED_DEVON_GOODS, SlateportCity_SternsShipyard_1F_EventScript_GoFindStern
	msgbox SlateportCity_SternsShipyard_1F_Text_CantMakeHeadsOrTails, MSGBOX_DEFAULT
	applymovement LOCALID_DOCK, Common_Movement_FacePlayer
	waitmovement 0
	msgbox SlateportCity_SternsShipyard_1F_Text_MeetDockDeliverToStern, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_DOCK, Common_Movement_FaceOriginalDirection
	waitmovement 0
	setflag FLAG_DOCK_REJECTED_DEVON_GOODS
	setflag FLAG_HIDE_SLATEPORT_CITY_TEAM_AQUA
	releaseall
	end

SlateportCity_SternsShipyard_1F_EventScript_FerryReady::
	applymovement LOCALID_DOCK, Common_Movement_FacePlayer
	waitmovement 0
	msgbox SlateportCity_SternsShipyard_1F_Text_FerryIsReady, MSGBOX_DEFAULT
	releaseall
	end

SlateportCity_SternsShipyard_1F_EventScript_BrineyJoined::
	applymovement LOCALID_DOCK, Common_Movement_FacePlayer
	waitmovement 0
	msgbox SlateportCity_SternsShipyard_1F_Text_BrineyJoinedUs, MSGBOX_DEFAULT
	releaseall
	end

SlateportCity_SternsShipyard_1F_EventScript_GoFindStern::
	applymovement LOCALID_DOCK, Common_Movement_FacePlayer
	waitmovement 0
	msgbox SlateportCity_SternsShipyard_1F_Text_CouldYouFindStern, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_DOCK, Common_Movement_FaceOriginalDirection
	waitmovement 0
	releaseall
	end

SlateportCity_SternsShipyard_1F_EventScript_NeedVeteran::
	applymovement LOCALID_DOCK, Common_Movement_FacePlayer
	waitmovement 0
	msgbox SlateportCity_SternsShipyard_1F_Text_CouldUseAdviceFromVeteran, MSGBOX_DEFAULT
	releaseall
	end

SlateportCity_SternsShipyard_1F_EventScript_Scientist1::
	msgbox SlateportCity_SternsShipyard_1F_Text_SeaIsLikeLivingThing, MSGBOX_NPC
	end

SlateportCity_SternsShipyard_1F_EventScript_Scientist2::
	msgbox SlateportCity_SternsShipyard_1F_Text_GetSeasickEasily, MSGBOX_NPC
	end

SlateportCity_SternsShipyard_1F_EventScript_Briney::
	msgbox SlateportCity_SternsShipyard_1F_Text_DecidedToHelpDock, MSGBOX_NPC
	end

SlateportCity_SternsShipyard_1F_Text_CantMakeHeadsOrTails:
	.string "Euh… Si ça va là, et que ça va là-bas…\p"
	.string "Alors ce truc-là, il va où ? Et ce bidule\n"
	.string "?\p"
	.string "Aaargh ! J'y comprends rien !$"

SlateportCity_SternsShipyard_1F_Text_MeetDockDeliverToStern:
	.string "Hm ? Salut, moi c'est Dock.\p"
	.string "Le Capitaine Stern m'a commandé un\n"
	.string "ferry.\p"
	.string "Oh ! Ce sont les Pièces Devon ?\p"
	.string "Mais, hmm… Ça ne va pas…\p"
	.string "Le Capitaine est parti quelque part. Il\n"
	.string "avait du travail, il a dit.\p"
	.string "Tu pourrais le retrouver et lui remettre\n"
	.string "ce colis ?$"

SlateportCity_SternsShipyard_1F_Text_CouldYouFindStern:
	.string "Dock : Où est passé le Capitaine Stern ?\p"
	.string "Tu pourrais le retrouver et lui remettre\n"
	.string "ce colis ?$"

SlateportCity_SternsShipyard_1F_Text_CouldUseAdviceFromVeteran:
	.string "Dock : Construire un navire, c'est un\n"
	.string "art.\p"
	.string "Beaucoup de choses ne se calculent\n"
	.string "pas.\p"
	.string "J'aurais bien besoin des conseils d'un\n"
	.string "vieux marin qui connaît la mer…$"

SlateportCity_SternsShipyard_1F_Text_BrineyJoinedUs:
	.string "Dock : Salut ! M. Marco nous a rejoints\n"
	.string "pour nous aider.\p"
	.string "Grâce à ce vieux loup de mer, le ferry\n"
	.string "prend forme.$"

SlateportCity_SternsShipyard_1F_Text_FerryIsReady:
	.string "Dock : Le ferry est enfin prêt !\p"
	.string "Le nouveau M.S. Marée est une merveille\n"
	.string "de technologie !\p"
	.string "Mais j'ai l'impression qu'on peut faire\n"
	.string "encore mieux.\p"
	.string "La technologie n'arrête jamais\n"
	.string "d'avancer. Moi non plus.$"

SlateportCity_SternsShipyard_1F_Text_DecidedToHelpDock:
	.string "M. Marco : Ah, {PLAYER} ! Ça faisait\n"
	.string "longtemps !\p"
	.string "Depuis notre rencontre, ce vieux loup\n"
	.string "de mer a retrouvé la forme !\p"
	.string "Alors j'aide Dock à construire un ferry.\p"
	.string "Un ferry, ça transporte plein de gens.\n"
	.string "En sécurité. Pas comme mon vieux rafiot.\p"
	.string "Ce Dock, c'est un sacré gamin.\p"
	.string "Avec son génie et mon expérience, on va\n"
	.string "construire un navire que la mer ne\l"
	.string "pourra pas avaler !$"

SlateportCity_SternsShipyard_1F_Text_SeaIsLikeLivingThing:
	.string "Les saisons, la météo, la place de la\n"
	.string "lune dans le ciel…\p"
	.string "Tout ça change l'humeur de la mer.\p"
	.string "Oui ! La mer est vivante. Et elle a de la\n"
	.string "rancune.$"

SlateportCity_SternsShipyard_1F_Text_GetSeasickEasily:
	.string "J'ai le mal de mer au moindre clapotis.\n"
	.string "Alors j'aide ici, sur la terre ferme.$"

