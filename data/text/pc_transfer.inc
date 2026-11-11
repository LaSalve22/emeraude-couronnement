gText_PkmnTransferredSomeonesPC::
	.string "{STR_VAR_2} est envoyé sur le PC de\n"
	.string "quelqu'un.\p"
	.string "Il est rangé dans la BOÎTE\n"
	.string "“{STR_VAR_1}”.$"

gText_PkmnTransferredLanettesPC::
	.string "{STR_VAR_2} was transferred to\n"
#if IS_FRLG
	.string "BILL'S PC.\p"
#else
	.string "LANETTE'S PC.\p"
#endif
	.string "It was placed in \n"
	.string "BOX “{STR_VAR_1}.”$"

gText_PkmnTransferredSomeonesPCBoxFull::
	.string "La BOÎTE “{STR_VAR_3}” du PC de\n"
	.string "quelqu'un est pleine.\p"
	.string "{STR_VAR_2} est envoyé dans la BOÎTE\n"
	.string "“{STR_VAR_1}”.$"

gText_PkmnTransferredLanettesPCBoxFull::
	.string "BOX “{STR_VAR_3}” on\n"
#if IS_FRLG
	.string "BILL'S PC was full.\p"
#else
	.string "LANETTE'S PC was full.\p"
#endif
	.string "{STR_VAR_2} was transferred to\n"
	.string "BOX “{STR_VAR_1}.”$"

gText_PkmnSentToPCAfterCatch::
	.string "{STR_VAR_2} est envoyé sur le PC {B_PC_CREATOR_NAME}.\p"
	.string "Il est rangé dans la BOÎTE\n"
	.string "“{STR_VAR_1}”.$"

gText_PlayerObtainedTheMon::
	.string "{PLAYER} obtient {STR_VAR_1} !$"

gText_NoMoreRoomForPokemon::
	.string "Plus de place pour les Pokémon !\p"
	.string "Les BOÎTES sont pleines et ne peuvent\n"
	.string "plus en accueillir !$"

gText_NicknameThisPokemon::
	.string "Donner un surnom à {STR_VAR_1} ?$"
