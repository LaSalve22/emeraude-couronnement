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
	.string "Celui qui l'a inventé est un génie. Ou un\n"
	.string "monstre. Je n'ai pas encore décidé.$"

PetalburgCity_PokemonCenter_1F_Text_OranBerryRegainedHP:
	.string "Mon Pokémon a mangé une Baie Oran et il\n"
	.string "a récupéré des PV !$"

PetalburgCity_PokemonCenter_1F_Text_ManyTypesOfPokemon:
	.string "Il existe beaucoup de types de\n"
	.string "Pokémon.\p"
	.string "Chaque type a ses forces et ses\n"
	.string "faiblesses face aux autres.\p"
	.string "Selon les types, un combat peut être\n"
	.string "une formalité… ou un massacre.$"

PetalburgCity_PokemonCenter_1F_Text_TreeckoIsGrassType:
	.string "Par exemple, ton Arcko est de type\n"
	.string "Plante.\p"
	.string "Il est fort contre les types Eau et Sol.\p"
	.string "Mais le Feu le réduit en cendres.$"

PetalburgCity_PokemonCenter_1F_Text_TorchicIsFireType:
	.string "Par exemple, ton Héricendre est de type\n"
	.string "Feu.\p"
	.string "Il est fort contre les types Plante et\n"
	.string "Insecte.\p"
	.string "Mais l'Eau l'éteint en un rien de temps.$"

PetalburgCity_PokemonCenter_1F_Text_MudkipIsWaterType:
	.string "Par exemple, ton Carapuce est de type\n"
	.string "Eau.\p"
	.string "Il est fort contre le type Feu.\p"
	.string "Mais il craint les types Plante et\n"
	.string "Électrik.$"

