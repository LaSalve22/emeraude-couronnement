Route116_TunnelersRestHouse_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, Route116_TunnelersRestHouse_OnTransition
	.byte 0

Route116_TunnelersRestHouse_OnTransition:
	setflag FLAG_LANDMARK_TUNNELERS_REST_HOUSE
	end

Route116_TunnelersRestHouse_EventScript_Tunneler1::
	msgbox Route116_TunnelersRestHouse_Text_WeHadToStopBoring, MSGBOX_NPC
	end

Route116_TunnelersRestHouse_EventScript_Tunneler2::
	msgbox Route116_TunnelersRestHouse_Text_ManDiggingHisWayToVerdanturf, MSGBOX_NPC
	end

Route116_TunnelersRestHouse_EventScript_Tunneler3::
	lock
	faceplayer
	goto_if_set FLAG_RUSTURF_TUNNEL_OPENED, Route116_TunnelersRestHouse_EventScript_TunnelOpened
	msgbox Route116_TunnelersRestHouse_Text_GetToVerdanturfWithoutTunnel, MSGBOX_DEFAULT
	release
	end

Route116_TunnelersRestHouse_EventScript_TunnelOpened::
	msgbox Route116_TunnelersRestHouse_Text_TunnelHasGoneThrough, MSGBOX_DEFAULT
	release
	end

Route116_TunnelersRestHouse_Text_WeHadToStopBoring:
	.string "Le Tunnel Mérazon, là…\p"
	.string "Au début, on avait une grosse équipe et\n"
	.string "des machines dernier cri pour percer la\l"
	.string "roche. Mais on a dû arrêter.\p"
	.string "Les travaux faisaient du mal aux\n"
	.string "Pokémon sauvages du coin.\p"
	.string "Alors maintenant, on glande ici à ne rien\n"
	.string "faire. Payés quand même.$"

Route116_TunnelersRestHouse_Text_ManDiggingHisWayToVerdanturf:
	.string "Il y a un homme qui creuse jusqu'à\n"
	.string "Vergazon, tout seul. Il est prêt à tout\l"
	.string "pour passer.\p"
	.string "Il dit qu'en creusant petit à petit,\n"
	.string "sans machine, il ne dérangera pas les\l"
	.string "Pokémon ni la nature.\p"
	.string "Je me demande s'il est arrivé au bout. Ou\n"
	.string "s'il est mort là-dessous.$"

Route116_TunnelersRestHouse_Text_GetToVerdanturfWithoutTunnel:
	.string "Pour aller à Vergazon sans ce Tunnel,\n"
	.string "faut traverser la mer jusqu'à Myokara,\l"
	.string "naviguer jusqu'à Poivressel, puis\l"
	.string "passer par Lavandia.$"

Route116_TunnelersRestHouse_Text_TunnelHasGoneThrough:
	.string "T'as entendu ? Le Tunnel jusqu'à\n"
	.string "Vergazon est percé !\p"
	.string "Parfois, quand on y croit assez fort,\n"
	.string "les rêves se réalisent.$"

