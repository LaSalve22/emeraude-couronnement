PetalburgCity_PokemonCenter_1F_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, PetalburgCity_PokemonCenter_1F_OnTransition
	map_script MAP_SCRIPT_ON_RESUME, CableClub_OnResume
	.byte 0

PetalburgCity_PokemonCenter_1F_OnTransition:
	setrespawn HEAL_LOCATION_PETALBURG_CITY
	call Common_EventScript_UpdateBrineyLocation
	end

PetalburgCity_PokemonCenter_1F_EventScript_Nurse::
	setvar VAR_0x800B, LOCALID_PETALBURG_NURSE
	call Common_EventScript_PkmnCenterNurse
	waitmessage
	waitbuttonpress
	release
	end

PetalburgCity_PokemonCenter_1F_EventScript_FatMan::
	msgbox PetalburgCity_PokemonCenter_1F_Text_PCStorageSystem, MSGBOX_NPC
	end

PetalburgCity_PokemonCenter_1F_EventScript_Youngster::
	msgbox PetalburgCity_PokemonCenter_1F_Text_OranBerryRegainedHP, MSGBOX_NPC
	end

PetalburgCity_PokemonCenter_1F_EventScript_Woman::
	lock
	faceplayer
	msgbox PetalburgCity_PokemonCenter_1F_Text_ManyTypesOfPokemon, MSGBOX_DEFAULT
	specialvar VAR_RESULT, IsStarterInParty
	goto_if_eq VAR_RESULT, TRUE, PetalburgCity_PokemonCenter_1F_EventScript_SayStarterTypeInfo
	release
	end

PetalburgCity_PokemonCenter_1F_EventScript_SayStarterTypeInfo::
	call_if_eq VAR_STARTER_MON, 0, PetalburgCity_PokemonCenter_1F_EventScript_SayTreeckoType
	call_if_eq VAR_STARTER_MON, 1, PetalburgCity_PokemonCenter_1F_EventScript_SayTorchicType
	call_if_eq VAR_STARTER_MON, 2, PetalburgCity_PokemonCenter_1F_EventScript_SayMudkipType
	release
	end

PetalburgCity_PokemonCenter_1F_EventScript_SayTreeckoType::
	msgbox PetalburgCity_PokemonCenter_1F_Text_TreeckoIsGrassType, MSGBOX_DEFAULT
	return

PetalburgCity_PokemonCenter_1F_EventScript_SayTorchicType::
	msgbox PetalburgCity_PokemonCenter_1F_Text_TorchicIsFireType, MSGBOX_DEFAULT
	return

PetalburgCity_PokemonCenter_1F_EventScript_SayMudkipType::
	msgbox PetalburgCity_PokemonCenter_1F_Text_MudkipIsWaterType, MSGBOX_DEFAULT
	return

PetalburgCity_PokemonCenter_1F_Text_PCStorageSystem:
	.string "Le système de stockage de Pokémon sur\n"
	.string "PC…\p"
	.string "Celui qui a inventé ça est un génie. Ou\n"
	.string "un psychopathe. Je n'ai pas encore\l"
	.string "tranché.$"

PetalburgCity_PokemonCenter_1F_Text_OranBerryRegainedHP:
	.string "Mon Pokémon a mangé une Baie Oran et il\n"
	.string "a récupéré des PV ! Moi, j'en ai mangé\l"
	.string "une et j'ai eu la diarrhée.$"

PetalburgCity_PokemonCenter_1F_Text_ManyTypesOfPokemon:
	.string "Il existe plein de types de Pokémon.\p"
	.string "Chacun a ses forces et ses faiblesses\n"
	.string "face aux autres.\p"
	.string "Selon les types, un combat peut être\n"
	.string "une promenade… ou une exécution.$"

PetalburgCity_PokemonCenter_1F_Text_TreeckoIsGrassType:
	.string "Par exemple, ton Arcko est de type\n"
	.string "Plante.\p"
	.string "Il écrase les types Eau et Sol.\p"
	.string "Mais face au Feu, il fait barbecue.$"

PetalburgCity_PokemonCenter_1F_Text_TorchicIsFireType:
	.string "Par exemple, ton Héricendre est de type\n"
	.string "Feu.\p"
	.string "Il crame les types Plante et Insecte.\p"
	.string "Mais un seau d'Eau et pfft. Plus rien.$"

PetalburgCity_PokemonCenter_1F_Text_MudkipIsWaterType:
	.string "Par exemple, ton Carapuce est de type\n"
	.string "Eau.\p"
	.string "Il noie le type Feu.\p"
	.string "Mais il craint les Plantes et l'Électrik.\n"
	.string "Surtout dans le bain.$"

