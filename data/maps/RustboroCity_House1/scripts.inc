RustboroCity_House1_MapScripts::
	.byte 0

RustboroCity_House1_EventScript_Trader::
	lock
	faceplayer
	goto_if_set FLAG_RUSTBORO_NPC_TRADE_COMPLETED, RustboroCity_House1_EventScript_TradeCompleted
	ingame_trade INGAME_TRADE_SEEDOT, RustboroCity_House1_Text_IllTradeIfYouWant, RustboroCity_House1_EventScript_DeclineTrade, RustboroCity_House1_EventScript_NotRequestedMon, RustboroCity_House1_Text_PleaseBeGoodToMyPokemon
	setflag FLAG_RUSTBORO_NPC_TRADE_COMPLETED
	release
	end

RustboroCity_House1_EventScript_DeclineTrade::
	msgbox RustboroCity_House1_Text_YouDontWantToThatsOkay, MSGBOX_DEFAULT
	release
	end

RustboroCity_House1_EventScript_NotRequestedMon::
	bufferspeciesname STR_VAR_1, VAR_0x8009
	msgbox RustboroCity_House1_Text_DoesntLookLikeMonToMe, MSGBOX_DEFAULT
	release
	end

RustboroCity_House1_EventScript_TradeCompleted::
	msgbox RustboroCity_House1_Text_AnyPokemonCanBeCute, MSGBOX_DEFAULT
	release
	end

RustboroCity_House1_EventScript_Hiker::
	msgbox RustboroCity_House1_Text_AllSortsOfPlaces, MSGBOX_NPC
	end

RustboroCity_House1_Text_IllTradeIfYouWant:
	.string "Hein ? Mon Pokémon est mignon ? Je sais,\n"
	.string "oui.\p"
	.string "Mais si tu y tiens vraiment, je veux bien\n"
	.string "l'échanger.\p"
	.string "Mon {STR_VAR_2} contre un {STR_VAR_1},\n"
	.string "si tu veux.$"

RustboroCity_House1_Text_PleaseBeGoodToMyPokemon:
	.string "Hé hé… Prends soin de mon Pokémon.\n"
	.string "Promis ?$"

RustboroCity_House1_Text_DoesntLookLikeMonToMe:
	.string "Hein ? Ça ne ressemble pas du tout à un\n"
	.string "{STR_VAR_1}, ça.$"

RustboroCity_House1_Text_YouDontWantToThatsOkay:
	.string "Bon, si tu ne veux pas, tant pis. Mais il\n"
	.string "est mignon, mon Pokémon…$"

RustboroCity_House1_Text_AnyPokemonCanBeCute:
	.string "N'importe quel Pokémon devient mignon\n"
	.string "si on l'élève avec soin et douceur.$"

RustboroCity_House1_Text_AllSortsOfPlaces:
	.string "Partout, il y a toutes sortes de\n"
	.string "Pokémon et toutes sortes de gens.\p"
	.string "Ça me fascine, alors je voyage partout.$"

