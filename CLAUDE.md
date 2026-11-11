# Émeraude — Couronnement Mondial

Romhack de Pokémon Émeraude d'Adrien, basé sur pokeemerald-expansion 1.17.2.
Réponds toujours en français. Le jeu est traduit en français au fur et à mesure (version plus adulte, voir `translations/fr/GUIDE.md`).

## Sources de vérité

- **Bible du projet (conception complète)** : https://claude.ai/artifact/FVafGjzmqKFXxQj5Xf5som
  Si tu ne peux pas l'ouvrir, le résumé ci-dessous suffit pour avancer.
- Ce fichier : état d'avancement et prochaine étape.

## Résumé de la conception

- Difficulté visée : Radical Red / Emerald Imperium. Trois modes au lancement : Normal, Hard, Kaizo.
  On peut descendre d'un mode en cours de partie, jamais remonter.
- Scénario d'Émeraude conservé. En Hard et Kaizo, les dresseurs ont d'autres Pokémon, mieux construits (EV/IV, attaques, objets).
- Level caps stricts par badge : 18, 27, 36, 45, 54, 63, 71, 78, Ligue 85. Finale du Couronnement entre 90 et 100.
- Objets de soin en combat : autorisés en Normal, interdits en Hard et Kaizo. Mode Set forcé en Hard et Kaizo.
- Starters : Carapuce, Arcko, Héricendre, avec talent caché et stats de base +20 %, bonus réservé aux starters du joueur (toute leur lignée).
- Personnages jouables : Red, Blue, Green, Brendan, May, Ash, Lucas, Dawn. Rival : May si personnage masculin, Brendan si féminin.
- Méga-évolution débloquée après le 5e badge, Téracristal après le 2e.
- Liens avec les Pokémon : bonus d'XP et d'EV selon l'amitié et l'ancienneté dans l'équipe ; EV jusqu'à 302 par stat avec un lien fort.
- Objets tenus de combat récupérés après chaque combat. 3 à 6 formes régionales, plus tard.
- **Couronnement Mondial** (façon Pokémon Champions) : pass donné par le rival au départ, rang 10 000, Elo (1500 = équilibré),
  paliers Poké < 1300 ≤ Super < 1600 ≤ Hyper < 1800 ≤ Master, Champions = Top 32 (~2000 d'Elo).
  Écran dans le menu Start (insigne, rang, Elo). Format double, 6 montrés / 4 choisis. Un seul légendaire par équipe.
  12 sous-tournois optionnels (8 villes d'arène + 4 salles de concours), 3 combats avec soin entre chaque, défaite = tournoi perdu.
  Finale à 32 à la Tour de Combat après la Ligue, 5 tours, un soin avant la demi-finale, Red et Blue en finale.

## Fait (v0.1)

- Level caps stricts par badge (`src/caps.c`), Super Bonbons bloqués au plafond.
- Multi Exp permanent via `FLAG_EXP_SHARE_ALWAYS`, activé quand Seko donne le Pokédex.
- Shiny 1/256 (`SHINY_ODDS` = 256), CT réutilisables, IV/EV en valeurs dans l'écran de statut, tous les réapprentissages.
- `VAR_DIFFICULTY` (0x404E) branchée sur `B_VAR_DIFFICULTY`, initialisée à Normal.

## Fait (v0.2)

- Starters Arcko / Héricendre / Carapuce (`src/starter_choose.c`, ordre plante / feu / eau conservé pour `VAR_STARTER_MON`).
- Starter du joueur marqué par le bit `isPlayerStarter` (ex-`unused_0B` de `PokemonSubstruct3`, accès `MON_DATA_PLAYER_STARTER`),
  posé par `MarkPlayerStarter()` à la remise du starter : talent caché + stats de base +20 % pour toute la lignée
  (réglages `P_PLAYER_STARTER_STAT_BONUS` et `P_PLAYER_STARTER_HIDDEN_ABILITY` dans `include/config/pokemon.h`).
  Les bébés obtenus par reproduction n'ont pas le bonus.
- Équipes du rival adaptées (Poussifeu → Héricendre, Gobou → Carapuce et leurs évolutions), texte de l'infirmière de Clémenti-Ville,
  poupée Héricendre au Casino de Lavandia. Tests : `test/player_starter.c` (`make check TESTS="Player starter"`).
- À revoir plus tard : le Prof. Seko propose encore Héricendre parmi les starters de Johto après la Ligue ; pas de poupée Carapuce
  (la poupée Gobou reste pour le choix eau).

## Fait (v0.3 et v0.4)

- Traduction française du chapitre 1 (intro → Mérouville, Tunnel Mérazon compris). v0.4 : ton réécrit après le test d'Adrien, humour manga + textes adultes avec du fond + trash si besoin :
  jurons, thèmes durs (mort, maladie, deuil), scénario inchangé. Guide de ton et lexique : `translations/fr/GUIDE.md`.
- Textes sources dans `translations/fr/*.txt`, appliqués par `python3 tools/fr/textfr.py apply translations/fr/*.txt`
  (mise en forme automatique au pixel près ; `check` signale les lignes trop larges).
- Rival renommé Flora / Brice (dialogues et combats), noms de lieux du chapitre 1 traduits dans `region_map_sections.json`.
- Limite connue : noms de Pokémon, attaques, objets, menus et messages de combat encore en anglais.

## Fait (v0.5)

- Choix de la difficulté au tout début (camion) : Normal / Hard / Kaizo (`DIFFICULTY_KAIZO` ajouté,
  `VAR_DIFFICULTY_CHOSEN` = 0x4083). Les règles propres à Hard/Kaizo ne sont pas encore codées (étape 2).
- Noms français officiels de tous les Pokémon, attaques, objets, talents, natures et types (source PokéAPI,
  `tools/fr/names_fr.py <csv>`, rapport `translations/fr/noms_rapport.txt`). Attaques Z/Gigamax et quelques objets récents restent en anglais.
- Messages de combat (821 chaînes) et menus / messages système (~500 chaînes de `src/strings.c`, Centre Pokémon, objets reçus,
  sauvegarde, PC) en français. Le qualificatif suit le nom : « Zigzaton sauvage », « Medhyèna ennemi ».
  Outil : `tools/fr/cstrings_fr.py translations/fr/c/*.txt`.
- Zones sauvages de Hoenn repeuplées (`tools/wild/repeupler.py <csv>`, rapport `docs/rencontres_sauvages.md`) :
  toutes générations, surtout Gen 1-4 avant le 5e badge, équilibré ensuite ; biome par zone ; forme selon le niveau ;
  les 27 starters de toutes les régions capturables (slots à 4 %) ; pseudo-légendaires rares en fin de jeu.
  Exclus : légendaires, fabuleux, Ultra-Chimères, Paradoxes, bébés, fossiles. Niveaux d'origine conservés.
- Attention : les tests de combat du moteur (`make check` complet) vérifient des messages en anglais et vont échouer ;
  seuls nos tests (`TESTS="Player starter"`) font foi.
- Reste en anglais : descriptions (objets, attaques, talents, Pokédex), écrans secondaires (concours, Zone de Combat, Easy Chat),
  dialogues des chapitres 2 et suivants.

## Fait (v0.5.1)

- Menu OPTIONS traduit + nouvelle ligne « MUSIQUE COMBAT » : Hoenn / Kanto / Aléa. (`VAR_BATTLE_MUSIC` = 0x408B,
  `include/constants/battle_music.h`, correspondances dans `GetBattleBGM` de `src/pokemon.c`). Kanto = thèmes Rouge/Bleu
  en version Rouge Feu / Vert Feuille, déjà présents. Aléa. = tirage stable par adversaire. Légendaires inchangés.
  Tests : `test/battle_music.c`.
- Johto (Or/Argent), Sinnoh (Diamant/Perle) et animé : pas de fichiers. Pour ajouter une région quand Adrien fournit des MIDI :
  .mid dans `sound/songs/midi/`, ligne dans `sound/songs/midi/midi.cfg` (voicegroup), entrée dans `sound/song_table.inc`
  et constante `MUS_…` dans `include/constants/songs.h`, puis nouvelle valeur `BATTLE_MUSIC_…`, libellé dans `src/option_menu.c`
  et correspondance dans `GetBattleBGM`. Ne pas télécharger de rips : uniquement des fichiers fournis par Adrien.

## Fait (v0.5.2)

- Choix ANIMÉ dans « MUSIQUE COMBAT » : thème de l'animé (MIDI fourni par Adrien, séquencé par Ash_Ketchum_Of_Pallet,
  non commercial), `MUS_ANIME_BATTLE` = 610, voicegroup `sound/voicegroups/anime_battle.inc`.
  Conversion : `python3 tools/music/prepare_midi.py <src.mid> sound/songs/midi/mus_xxx.mid --map 0:1,34:33,... --drums 0`
  (réassigne les instruments, branche la batterie, ajoute la boucle), puis ligne dans `midi.cfg`, `song_table.inc`, `songs.h`.
  L'option affiche le choix courant seul (4 choix ne tiennent pas sur la ligne).

## Fait (v0.6)

- Personnages jouables choisis dans l'intro du Prof. Seko : Brice (Brendan), Flora (May), Red, Green (Leaf).
  `VAR_PLAYER_CHARACTER` = 0x4091, `include/constants/player_character.h`, `src/player_character.c`.
  Le sexe découle du personnage (maison et rival inchangés : rival Flora si garçon, Brice si fille).
  Sprites de Red/Green (terrain, vélo, surf, pêche, face, dos) branchés dans `GetPlayerAvatarGraphicsIdByStateIdAndGender`,
  `PlayerGenderToFrontTrainerPicId` et `GetPlayerTrainerPic` ; leurs données de terrain, réservées à Rouge Feu, sont désormais
  compilées aussi pour Émeraude (garde `#if IS_FRLG` déplacée). Nom par défaut = nom du personnage. Tests : `test/player_character.c`.
- Manquants : Blue (seulement un sprite de PNJ, pas de course/vélo/surf/dos), Ash, Lucas, Dawn (aucun sprite).
  Écrans encore aux sprites de Brendan/May pour Red/Green : carte Dresseur, icône de la carte de la région, PokéNav, Panthéon.

## Fait (v0.7)

- Règles Hard / Kaizo : mode Set forcé (`src/battle_main.c`), objets de soin (PV, statut, rappel, PP) interdits en combat
  pour le joueur (`IsHealingItemBannedInBattle`, `src/item_use.c`). Poké Balls et objets X restent autorisés.
- OPTIONS : nouvelle ligne « DIFFICULTÉ » ; gauche pour baisser, impossible de remonter au-dessus de la valeur à l'ouverture.
  Le bandeau « OPTIONS » est retiré pour loger 9 lignes de 16 px ; tuiles du cadre déplacées en 0x220.
- Tests : `test/difficulty_rules.c`.
- Mis de côté (Adrien) : Blue / Lucas / Aurore, en attente de sprites.

## Fait (v0.8)

- Équipes Hard et Kaizo des 40 dresseurs du chapitre 1 (Routes 102, 104, 116, Bois Clémenti, Arène de Mérouville,
  rivaux Route 103 et Mérouville, voyous Aqua) : `tools/trainers/equipes_couronnement.py` (données lisibles, relançable),
  blocs ajoutés à la fin de `src/data/trainers.party` entre deux commentaires. Hard : IV 20, équipes variées, ~+3 niveaux.
  Kaizo : IV 31, EVs, natures, objets, ~+5 niveaux, jamais au-dessus du cap (Roxanne N.18 en Kaizo, 6 Pokémon).
  En Hard/Kaizo, les dresseurs n'ont plus d'objets de soin (bloc Items retiré), comme le joueur.
- Les tests ne peuvent pas vérifier ces équipes (les builds de test utilisent leurs propres dresseurs) :
  contrôle dans `src/data/trainers.h` généré.

## Fait (v0.9)

- Équipes Hard / Kaizo du chapitre 2 (61 dresseurs de plus, 101 au total). Chefs faits à la main : Bastien (cap 27),
  Voltère (cap 36), rival Route 110, Timmy à Lavandia, voyous Aqua du Musée. Dresseurs ordinaires générés par règles
  (`AUTO` + `POOLS` par classe dans `tools/trainers/equipes_couronnement.py`) : équipe d'origine +3 / +5 niveaux,
  plancher à 4 niveaux sous le meilleur, +1 / +2 Pokémon de la réserve de leur classe, capacités par niveau.
- Feuille de route lisible pour Adrien : `docs/FEUILLE_DE_ROUTE.md`.

## Fait (v0.10)

- Téracristal : Bastien remet l'Orbe Téracristal après le 2e badge (`DewfordTown_Gym_EventScript_GiveTeraOrb`),
  rechargé au Centre Pokémon (`B_FLAG_TERA_ORB_CHARGED` = `FLAG_TERA_ORB_CHARGED` 0x21).
- Méga-évolution : Norman remet le Méga-Anneau après le 5e badge, plus Jungkite (Arcko) ou Tortankite (Carapuce).
  Les autres Méga-Gemmes restent à placer dans le monde (à faire avec les objets de fin de jeu).

## Fait (v0.11)

- Liens avec les Pokémon (`src/pokemon_bond.c`, `include/pokemon_bond.h`) : lien (amitié >= 150) +5 % XP / +25 % EV gagnés ;
  lien fort (>= 220) +10 % XP / +50 % EV et EV effectifs jusqu'à 302 (252 compte comme 302, calcul dans CalculateMonStatsCont,
  le stockage reste sur un octet) ; ancienneté = `MON_DATA_BOND_BATTLES` (ex-`unused_02` de PokemonSubstruct0, 0-63,
  +1 par K.O. reçu en équipe), +1 % XP tous les 6, max +10 %. Tests : `test/pokemon_bond.c`.
- À faire plus tard : afficher le niveau de lien dans l'écran de résumé.

## Fait (v0.12 à v0.14)

- Chapitre 2 entièrement en français (`translations/fr/14` à `21`) : Myokara, Grotte Granite, Poivressel, Musée, chantier de Stern,
  Route 110, Lavandia, Arène de Voltère, Rydel Cycles, Routes 109 et 117, Maison de la Plage, Fan Club, Casino,
  et les répliques des dresseurs des Routes 109, 110, 116 et 117 (`data/text/trainers.inc`, fichier `20_dresseurs_chapitre2.txt`).
- Routes 105 à 108 (accessibles avec Surf) laissées pour le chapitre 3.

## Fait (v0.15)

- Équipes Hard / Kaizo du chapitre 3 (73 dresseurs de plus, 174 au total). Faits à la main : Adriane (cap 45, 6 Pokémon en Kaizo,
  Sécheresse), Max et Tabitha au Mont Chimnée, voyous Magma (Mont Chimnée, Chemin Escarpé).
- Dresseurs ordinaires : `AUTO[id] = (cap, plancher Hard, plancher Kaizo)` ; les niveaux d'origine étant bas, un plancher
  (30/34 sur les routes, 35/39 dans l'Arène, 40/44 dans le désert de la Route 111, cap 54) relève les équipes, et `EVOLVE`
  fait évoluer les espèces au bon niveau. Nouvelles réserves : sol, feu, roche, oiseau, poison, elite, beaute.
  Les chapitres 1-2 (entrées sans plancher) sont inchangés.

## Fait (v0.16)

- Équipes Hard / Kaizo du chapitre 4 (53 dresseurs de plus, 227 au total) : Norman fait à la main (cap 54 ; Kaizo : Kangourex,
  Linéon Cognobidon + Vitesse Extrême, Ursaring Orbe Flamme, Spinda Contestation, Vigoroth Évoluroc, Monaflèmit Orbe Vie),
  ses 7 Topdresseurs (plancher 46/50, réserve Normal via le 4e champ de `AUTO`), puis les dresseurs ouverts par Surf
  (Routes 105-109, 115, Épave Abandonnée ; cap 63, plancher 47/51).
- `EVOLVE` couvre aussi les évolutions par pierre / échange / amitié (niveau arbitraire 40).

## Fait (v0.17) - Couronnement Mondial, 1re partie

- Pass : le rival le donne au labo de Seko juste après le Pokédex (`Couronnement_EventScript_RivalDonnePass`), Elo 1500.
- Menu Start « MONDIAL » (si `FLAG_COURONNEMENT_PASS`) : palier, rang, Elo, tournois gagnés. Fenêtre du menu remontée
  quand il y a 9 entrées (`AddStartMenuWindow`, `src/menu.c`).
- Calculs (`src/couronnement.c`) : K = 32, attente Elo par table ; rang 10 000 à 1500, /1,12 tous les 10 points (2000 -> ~32),
  +20 places par point sous 1500 ; paliers Poké < 1300 <= Super < 1600 <= Hyper < 1800 <= Master, Champions = rang <= 32.
  Tests : `test/couronnement.c` (`make check TESTS="Couronnement"`).
- Tournois des 8 villes d'Arène : hôtesse dans chaque Centre Pokémon (case 12,3), ouverts après le badge de la ville.
  4 Pokémon choisis (`ChooseCouronnementParty`, `FACILITY_COURONNEMENT` dans `party_menu.c`, un seul légendaire),
  3 combats en double, soin entre chaque, défaite = éliminé sans blackout (`B_FLAG_NO_WHITEOUT` = `FLAG_COURONNEMENT_EN_COURS`),
  équipe d'origine rendue à la fin (Save/LoadPlayerParty). Équipe adverse affichée avant chaque combat.
  Récompenses : Bandeau Muscle, Lunettes Sages, Ceinture Pro, Ceinture Force, Orbe Vie, Mouchoir / Bandeau / Lunettes Choix.
- Adversaires : un seul emplacement `TRAINER_COURONNEMENT` (855) dont l'équipe vient de `gCouronnementTrainers[VAR_COURONNEMENT_ADVERSAIRE]`
  (hook dans `GetTrainerStructFromId`, `include/data.h`) ; 24 équipes dans `tools/couronnement/tournois.py`
  -> `src/data/couronnement.party` (trainerproc) + `include/constants/couronnement_adversaires.h`. Elo adverse 1350 + 50/tournoi + 40/manche.
  Il ne reste que 8 drapeaux de dresseurs libres : toujours passer par cet emplacement pour les nouveaux adversaires.
- Variables : `VAR_COURONNEMENT_ELO` 0x409B, `VAR_COURONNEMENT_TOURNOIS` 0x409D, `VAR_COURONNEMENT_ADVERSAIRE` 0x40A1 ;
  drapeaux `FLAG_COURONNEMENT_PASS` 0x22, `FLAG_COURONNEMENT_EN_COURS` 0x23.
- Pas encore : aperçu des 6 Pokémon adverses (ils en ont 4), tournois des 4 salles de Concours, finale à 32 (Tour de Combat, Red et Blue).

## Prochaines étapes, dans l'ordre

0. Priorité d'Adrien : finir le contenu du jeu (équipes, scénario, systèmes) ; la traduction (chapitres 3+ et descriptions) se fera à la fin.
1. Équipes Hard/Kaizo du chapitre 5 (Routes 118-120, Institut Météo, rival Route 119, Arène d'Alizée ; cap 63), même outil.
2. Placer les Méga-Gemmes dans le monde.
3. Couronnement Mondial, 2e partie : tournois des 4 salles de Concours, finale à 32 après la Ligue (Tour de Combat, 5 tours, soin avant la demi-finale, Red et Blue en finale).
4. Puis : niveau de lien dans le résumé, objets tenus récupérés après combat, Blue / Ash / Lucas / Dawn (sprites à fournir).

## Méthode de travail

- Compiler : `make -j$(sysctl -n hw.ncpu)` sur Mac. La ROM sort en `pokeemerald.gba`.
  Toolchain sur le Mac d'Adrien : ARM GNU Toolchain 14.3 dans `~/opt/arm-gnu-toolchain` (pas de devkitPro) ;
  si `arm-none-eabi-gcc` est introuvable : `export PATH="$HOME/opt/arm-gnu-toolchain/bin:$PATH"`.
- Après chaque étape qui compile : commit en français, push sur `main`, puis livrer une copie nommée `emeraude-couronnement-vX.Y.gba`.
- Adrien teste sur Eclipse (iPhone) et mGBA (Mac).
- Garder les réglages dans `include/config/` quand c'est possible plutôt que de modifier le moteur.
- Mettre à jour la section « Fait » et « Prochaines étapes » de ce fichier à chaque étape.
