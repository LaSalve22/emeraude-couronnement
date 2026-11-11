PetalburgCity_House2_MapScripts::
	.byte 0

PetalburgCity_House2_EventScript_Woman::
	msgbox PetalburgCity_House2_Text_NormanBecameGymLeader, MSGBOX_NPC
	end

PetalburgCity_House2_EventScript_SchoolKid::
	msgbox PetalburgCity_House2_Text_BattledNormanOnce, MSGBOX_NPC
	end

PetalburgCity_House2_Text_NormanBecameGymLeader:
	.string "Norman est devenu le nouveau Champion\n"
	.string "de notre ville.\p"
	.string "Il a fait venir sa famille de loin,\n"
	.string "paraît-il. Mais on ne le voit jamais avec\l"
	.string "eux.$"

PetalburgCity_House2_Text_BattledNormanOnce:
	.string "J'ai affronté Norman, une fois. Il m'a\n"
	.string "écrasé.\p"
	.string "Comment dire ?\p"
	.string "Ce type ne vit que pour les Pokémon. Il\n"
	.string "n'y a rien d'autre en lui. C'est ça qui\l"
	.string "fait peur.$"

