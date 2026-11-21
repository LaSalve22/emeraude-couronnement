// Généré par tools/couronnement/hall_placer.py : halls du Couronnement Mondial (édition Carrière).
// Ville, layout, coin haut-gauche du bâtiment 4 x 4 (0xFF : hall dans l'Arène), tournoi, carte avec le bâtiment, Arène.
static const u16 sHallCarte0[] = INCBIN_U16("data/layouts/RustboroCity/map_carriere.bin");
static const u16 sHallCarte1[] = INCBIN_U16("data/layouts/DewfordTown/map_carriere.bin");
static const u16 sHallCarte2[] = INCBIN_U16("data/layouts/MauvilleCity/map_carriere.bin");
static const u16 sHallCarte3[] = INCBIN_U16("data/layouts/LavaridgeTown/map_carriere.bin");
static const u16 sHallCarte6[] = INCBIN_U16("data/layouts/MossdeepCity/map_carriere.bin");

static const struct TycoonHall sHalls[] =
{
    {MAP_GROUP(MAP_RUSTBORO_CITY), MAP_NUM(MAP_RUSTBORO_CITY), LAYOUT_RUSTBORO_CITY, 20, 49, COURONNEMENT_TOURNOI_MEROUVILLE, sHallCarte0, 0, 0},
    {MAP_GROUP(MAP_DEWFORD_TOWN), MAP_NUM(MAP_DEWFORD_TOWN), LAYOUT_DEWFORD_TOWN, 1, 12, COURONNEMENT_TOURNOI_MYOKARA, sHallCarte1, 0, 0},
    {MAP_GROUP(MAP_MAUVILLE_CITY), MAP_NUM(MAP_MAUVILLE_CITY), LAYOUT_MAUVILLE_CITY, 1, 5, COURONNEMENT_TOURNOI_LAVANDIA, sHallCarte2, 0, 0},
    {MAP_GROUP(MAP_LAVARIDGE_TOWN), MAP_NUM(MAP_LAVARIDGE_TOWN), LAYOUT_LAVARIDGE_TOWN, 8, 8, COURONNEMENT_TOURNOI_VERMILAVA, sHallCarte3, 0, 0},
    {MAP_GROUP(MAP_PETALBURG_CITY), MAP_NUM(MAP_PETALBURG_CITY), 0, 0xFF, 0xFF, COURONNEMENT_TOURNOI_CLEMENTI, NULL, MAP_GROUP(MAP_PETALBURG_CITY_GYM), MAP_NUM(MAP_PETALBURG_CITY_GYM)},
    {MAP_GROUP(MAP_FORTREE_CITY), MAP_NUM(MAP_FORTREE_CITY), 0, 0xFF, 0xFF, COURONNEMENT_TOURNOI_CIMETRONELLE, NULL, MAP_GROUP(MAP_FORTREE_CITY_GYM), MAP_NUM(MAP_FORTREE_CITY_GYM)},
    {MAP_GROUP(MAP_MOSSDEEP_CITY), MAP_NUM(MAP_MOSSDEEP_CITY), LAYOUT_MOSSDEEP_CITY, 46, 15, COURONNEMENT_TOURNOI_ALGATIA, sHallCarte6, 0, 0},
    {MAP_GROUP(MAP_SOOTOPOLIS_CITY), MAP_NUM(MAP_SOOTOPOLIS_CITY), 0, 0xFF, 0xFF, COURONNEMENT_TOURNOI_ATALANOPOLIS, NULL, MAP_GROUP(MAP_SOOTOPOLIS_CITY_GYM_1F), MAP_NUM(MAP_SOOTOPOLIS_CITY_GYM_1F)},
};
