PetalburgCity_House2_MapScripts::
	.byte 0

PetalburgCity_House2_EventScript_Woman::
	msgbox PetalburgCity_House2_Text_NormanBecameGymLeader, MSGBOX_NPC
	end

PetalburgCity_House2_EventScript_SchoolKid::
	msgbox PetalburgCity_House2_Text_BattledNormanOnce, MSGBOX_NPC
	end

PetalburgCity_House2_Text_NormanBecameGymLeader:
	.string "Norman est devenu le Champion de notre\n"
	.string "ville.\p"
	.string "Il a fait venir sa famille de loin,\n"
	.string "paraît-il. On ne le voit jamais avec eux.\l"
	.string "On le voit juste s'entraîner à 5 h du\n"
	.string "matin en hurlant.$"

PetalburgCity_House2_Text_BattledNormanOnce:
	.string "J'ai affronté Norman, une fois. Il m'a\n"
	.string "démoli.\p"
	.string "Comment dire ?\p"
	.string "Ce type ne vit que pour la force. Il n'y a\n"
	.string "rien d'autre dans ses yeux. C'est ça qui\l"
	.string "fait flipper.$"

