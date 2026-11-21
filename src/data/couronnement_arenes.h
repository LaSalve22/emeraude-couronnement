// Généré par tools/couronnement/hall_placer.py : bâtiments des arènes du joueur (édition Carrière, v0.57.1).
// Ville, layout, coin haut-gauche du bâtiment 4 x 4, n° d'arène (src/arenes.c), carte avec le bâtiment.
static const u16 sAreneCarte0[] = INCBIN_U16("data/layouts/LittlerootTown/map_carriere.bin");
static const u16 sAreneCarte1[] = INCBIN_U16("data/layouts/OldaleTown/map_carriere.bin");
static const u16 sAreneCarte2[] = INCBIN_U16("data/layouts/SlateportCity/map_carriere.bin");
static const u16 sAreneCarte3[] = INCBIN_U16("data/layouts/FallarborTown/map_carriere.bin");
static const u16 sAreneCarte4[] = INCBIN_U16("data/layouts/VerdanturfTown/map_carriere.bin");
static const u16 sAreneCarte5[] = INCBIN_U16("data/layouts/LilycoveCity/map_carriere.bin");

static const struct TycoonHall sArenesBatiments[] =
{
    {MAP_GROUP(MAP_LITTLEROOT_TOWN), MAP_NUM(MAP_LITTLEROOT_TOWN), LAYOUT_LITTLEROOT_TOWN, 7, 3, 0, sAreneCarte0, 0, 0},
    {MAP_GROUP(MAP_OLDALE_TOWN), MAP_NUM(MAP_OLDALE_TOWN), LAYOUT_OLDALE_TOWN, 8, 6, 1, sAreneCarte1, 0, 0},
    {MAP_GROUP(MAP_SLATEPORT_CITY), MAP_NUM(MAP_SLATEPORT_CITY), LAYOUT_SLATEPORT_CITY, 12, 16, 2, sAreneCarte2, 0, 0},
    {MAP_GROUP(MAP_FALLARBOR_TOWN), MAP_NUM(MAP_FALLARBOR_TOWN), LAYOUT_FALLARBOR_TOWN, 1, 10, 3, sAreneCarte3, 0, 0},
    {MAP_GROUP(MAP_VERDANTURF_TOWN), MAP_NUM(MAP_VERDANTURF_TOWN), LAYOUT_VERDANTURF_TOWN, 8, 5, 4, sAreneCarte4, 0, 0},
    {MAP_GROUP(MAP_LILYCOVE_CITY), MAP_NUM(MAP_LILYCOVE_CITY), LAYOUT_LILYCOVE_CITY, 42, 21, 5, sAreneCarte5, 0, 0},
};
