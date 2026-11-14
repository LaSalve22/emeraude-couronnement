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

## Fait (v0.17.1) - retours d'Adrien

- IA : les blocs Hard ont au moins `AI: Basic Trainer`, Kaizo `AI: Smart Trainer`. Les attaques qui ne font que baisser les stats
  (Rugissement, Mimi-Queue, Jet de Sable, Brouillard, Groz'Yeux…, liste `WEAK_MOVES`) sont retirées des équipes faites à la main
  (le rival Kaizo ne faisait que ça). Rival Route 103 : 4 attaques offensives.
- Textes oubliés du chapitre 1 traduits : première rencontre du rival (`RivalsHouse_2F_Text_*WhoAreYou`), Route 116 et Maison des Foreurs.
- Red / Green : table d'animation `sAnimTable_RedGreenNormal` (course façon Rouge Feu) ; avec celle d'Émeraude, Red tournait sur lui-même.

## Fait (v0.18) - équipes Hard / Kaizo du chapitre 5

- Faits à la main : 5 sbires Aqua et Arlette à l'Institut Météo (52-56 Hard, 56-60 Kaizo), rival Route 119 (lignée finale :
  Typhlosion / Tortank / Jungko selon le starter ; 4 Pokémon en Hard jusqu'au 58, 6 en Kaizo jusqu'au 61), Alizée (5 en Hard
  jusqu'au 62, 6 en Kaizo au 62-63, dont Noctunoir et Léviator).
- Générés : 37 dresseurs des Routes 118-120 (plancher 52 Hard / 56 Kaizo, cap 63), 6 dresseurs de l'Arène d'Alizée (réserve Vol,
  plancher 56 / 60). Classes ajoutées : Pkmn Ranger (plante), Bug Catcher (insecte).

## Fait (v0.19) - Couronnement Mondial, 2e partie

- Chaque adversaire montre 6 Pokémon (`tools/couronnement/tournois.py` : 4 historiques + `EXTRA`). À chaque manche : LoadPlayerParty,
  aperçu, choix de 4 (`ChooseCouronnementParty`), puis `Couronnement_ChoixAdverse` garde les 4 adversaires qui s'en sortent le mieux
  (efficacité des attaques dans les deux sens + petit aléa). Copie modifiable `sAdversaire` en EWRAM, renvoyée par GetCouronnementTrainer.
- Salles de Concours d'Émeraude = Tentes de Combat de Poivressel (badge 2), Vergazon (3), Autéquia (4) et salle de Nénucrique (6) :
  tournois 8-11, hôtesse à (11,5) dans les tentes, (20,4) à Nénucrique. Récompenses : Casque Brut, Veste de Combat, Vulné-Assurance, Restes.
- Finale (tournoi 12) : hôtesse à la Tour de Combat (21,5), après la Ligue (FLAG_SYS_GAME_CLEAR) et au rang <= 32. 5 tours, 4 Pokémon
  choisis une fois, soin seulement avant la demi-finale (Blue, sprite Champion Rival Rouge Feu), Red en finale (Green si le joueur joue Red).
  Récompense : Patch Talent + Master Ball, titre « Champion du Monde » dans le menu MONDIAL.
- Elo adverse par tournoi (`sEloBase`, +50 par manche) : un parcours parfait mène vers ~1920 ; les tournois gagnés sont rejouables
  pour l'Elo (sans récompense) pour atteindre le Top 32 (~2000). Abandon en cours de tournoi : Elo inchangé.

## Fait (v0.20) - Aromates (changement de nature)

- Herboriste au Centre Pokémon de Clémenti-Ville (3,6), `data/scripts/aromates.inc` : vend les 21 Aromates (menthes du moteur,
  changent la nature effective) une fois le badge de Norman (FLAG_BADGE05_GET) obtenu. Prix : `COURONNEMENT_PRIX_AROMATE`
  (5000) dans `include/config/item.h`.

## Fait (v0.21) - équipes Hard / Kaizo du chapitre 6

- Faits à la main : rival à Nénucrique (5 en Hard jusqu'au 64, 6 en Kaizo jusqu'au 66), Max au Repaire Aqua (4 / 6, jusqu'au 65 / 67),
  Lévy & Tatia en double (4 en Hard 68-70, 6 en Kaizo 71-72 avec Métalosse et Gardevoir).
- Générés : 40 dresseurs des Routes 121 / 123 et du Mont Mémoria (plancher 58 / 62), 24 du Repaire Aqua et des Routes 124-125
  (60 / 64), 12 de l'Arène de Lévy & Tatia (réserve Psy, 64 / 68) ; cap 72. Réserves ajoutées : spectre (Hex Maniac), aqua (Team Aqua).

## Fait (v0.22) - équipes Hard / Kaizo du chapitre 7

- Faits à la main : Max et Tabitha au Centre Spatial (combat multi : 3 Pokémon chacun, 71-77), Arlette à la Grotte Fonds Marins
  (3 / 5), Arthur (4 / 6, jusqu'au 77 / 79), Juan (5 en Hard 78-80, 6 en Kaizo 80-81).
- Pierre (partenaire du Centre Spatial) a des blocs Hard / Kaizo dans `src/data/battle_partners.party` (Métalosse, Airmure,
  Galeking, 72-76).
- Générés : 12 sbires Magma / Aqua (68 / 72), 23 dresseurs des Routes 126-128 (66 / 70), 10 de l'Arène de Juan (réserve Eau,
  72 / 76) ; cap 81. Réserve ajoutée : magma (Team Magma).

## Fait (v0.23) - équipes Hard / Kaizo du chapitre 8

- Faits à la main (cap 85 de la Ligue) : Timmy à la Route Victoire (5 en Hard 80-82, 6 en Kaizo 83-84 avec Gallade et Méga-Gardevoir ;
  `WALLY_VR_2`, revanche d'après-Ligue, +6 niveaux), Damien, Spectra, Glacia, Aragon (Hard 5 Pokémon 81-85, Kaizo 6 au 83-85),
  Marc (6 / 6, 84-85). En Kaizo, l'as de chacun méga-évolue : Absol, Branette, Oniglali, Drattak, Léviator, Gardevoir.
- Générés : 39 dresseurs des Routes 129-134 (68 / 72, cap 81 comme les Routes 126-128), 16 de la Route Victoire
  (réserve elite, 76 / 80, cap 85).

## Fait (v0.23.1) - combats remis au level cap (choix d'Adrien)

- Les caps des règles AUTO suivent `src/caps.c` : 71 avant le 7e badge (chapitre 6), 78 avant le 8e (chapitre 7, Routes 129-134).
- `CAP_COMBAT` dans `tools/trainers/equipes_couronnement.py` décale vers le bas les équipes faites à la main qui dépassaient
  (Lévy & Tatia 71, Arthur 78, Juan 78 ; écarts conservés). Règle : l'as d'un combat ne dépasse jamais le cap du badge en jeu.

## Fait (v0.24) - Coach EV (changement d'EV après la Ligue)

- Coach EV (Black Belt) au Centre Pokémon de Clémenti-Ville (10,4), à côté de l'herboriste, après la Ligue (FLAG_SYS_GAME_CLEAR).
  Gratuit : choix du Pokémon, puis stat par stat 252 / 128 / 4 / 0, ou tout à 0 ; limite 252 par stat et 510 au total
  (valeur réduite au maximum possible s'il manque de la place). Stats recalculées aussitôt.
- Code : `src/coach_ev.c`, `include/coach_ev.h`, script `data/scripts/coach_ev.inc` ; tests `test/coach_ev.c` (`TESTS="Coach EV"`).

## Fait (v0.25) - Méga-Gemmes dans le monde

- 40 Méga-Gemmes classiques remplacent des objets ordinaires dans des Poké Balls existantes (aucun objet ajouté, pas de passage
  bloqué), à partir de la Route 118 : liste et emplacements dans `tools/megas/placer.py` (relançable).
- Les 52 autres (Jungkite / Tortankite, légendaires, Pokémon d'autres régions, Légendes Z-A) : Méga-Collectionneur au Centre
  Pokémon de Clémenti-Ville (1,4) après la Ligue, `data/scripts/mega_gemmes.inc`. Prix : `COURONNEMENT_PRIX_MEGA_GEMME` (10000)
  dans `include/config/item.h`, appliqué à toutes les Méga-Gemmes de `src/data/items.h`.

## Fait (v0.26) - confort et traduction des menus

- OPTIONS : la ligne SON (Mono / Stéréo) devient COURSE : BOUTON B / AUTO (`FLAG_COURSE_AUTO` 0x24 ; en AUTO on court sans
  tenir B, B pour marcher, `src/field_player_avatar.c`). L'écran est plein (9 lignes) : toute nouvelle option devra en remplacer une.
- Repousse Infinie (`ITEM_REPOUSSE_INFINIE` 874, objet clé assignable à SELECT) : bascule `FLAG_REPOUSSE_INFINIE` (0x25), branché sur
  `WE_FLAG_NO_ENCOUNTER` (plus de rencontres dans l'herbe / l'eau ; la pêche marche toujours). Donnée par le rival avec le pass ;
  l'herboriste de Clémenti la donne aux sauvegardes plus anciennes (`Couronnement_EventScript_DonnerRepousseInfinie`).
- Résumé du Pokémon : la ligne RUBAN devient LIEN (aucun / lien / fort + bonus d'XP total). Titres de l'écran redessinés en français
  dans `graphics/summary_screen/tiles.png` par `tools/fr/resume_titres.py` (PROFIL, TALENT, CAPACITES, OBJET, LIEN, EFFET, MEMO DRESSEUR…).
- Descriptions en français : 933 attaques, 317 talents, 857 objets (`tools/fr/descriptions_fr.py <csv PokéAPI>`) : texte officiel
  quand il tient dans la fenêtre (police normale, étroite ou plus étroite comme `GetFontIdToFit`), sinon traduction courte écrite à la
  main dans `translations/fr/descriptions/*.txt` (CLÉ|texte, mis en forme automatiquement). Sac et boutiques choisissent une police
  plus étroite si besoin. Restent : descriptions des Concours (`gContestEffects`), Pokédex.

## Fait (v0.27) - retours d'Adrien

- Menu Start MONDIAL : écran noir corrigé (`HandleStartMenuInput` lançait un fondu au noir pour toute entrée hors liste ;
  `StartMenuCouronnementCallback` ajouté aux exceptions comme Safari / Sauvegarde). Toute nouvelle entrée qui lance un script
  sur le terrain doit y être ajoutée.
- Réapprentissage : les capacités Œuf sont dans la liste « par niveau » (`GetRelearnerLevelUpMoves`, `src/move_relearner.c`),
  toujours proposées tant que le Pokémon ne les connaît pas ; onglet Œuf séparé désactivé ; `MAX_RELEARNER_MOVES` = 100.
- Kaizo : Seko donne 5 Poké Balls avant le combat contre le rival (Route 103) et la boutique de Rosyères en vend dès le début.
- Menu L sur le terrain (`Couronnement_EventScript_MenuL`) : Repousse Infinie OUI / NON et Course AUTO / BOUTON B (sauf si
  BOUTONS = L=A). On court dès le début, sans les Chaussures de Course (condition `FLAG_SYS_B_DASH` retirée).

## Fait (v0.28) - menu L complet, rang avec Ball, Dark Ball

- Menu L (`Couronnement_EventScript_MenuL`, `dynmultipush`) : Soigner l'équipe (8e badge), PC de stockage (1er badge), Repousse
  Infinie et Course (dès le début), Modifier les EV (après la Ligue, `CoachEV_EventScript_Session`), Type Téra (avec l'Orbe
  Téracristal, `Couronnement_ChangerTypeTera`), Rang mondial (avec le pass). Soin / PC / EV / Téra masqués pendant un tournoi.
- Rang mondial (MONDIAL et menu L) : palier affiché en Ball (Poké / Super / Hyper / Master, Champions = Master Ball) avec l'icône
  de la Ball dans la boîte de message (`Couronnement_AfficherBallePalier` / `CacherBallePalier`).
- Carte Dresseur : Red / Green ont leur sprite (`CreateTrainerCardTrainerPic`, `src/trainer_card.c`).
- Dark Ball = ex-Étrange Ball (`ITEM_STRANGE_BALL`, `BALL_STRANGE`) : le Pokémon capturé a `MON_DATA_IS_SHADOW` (bit isShadow, inutilisé
  par le moteur), amitié bloquée à 0 (SetBoxMonData), stats hors PV +20 % (`CalculateMonStats`), perd 10 % de ses PV max après
  chaque attaque qui touche (`MOVEEND_DARK_AURA`, `BattleScript_DarkAuraRecoil`), aura violet sombre en combat
  (`BattleLoadMonSpriteGfx`). Réglages : `COURONNEMENT_DARK_BALL_*` dans `include/config/item.h`. Graphismes tirés du dessin
  d'Adrien : `tools/couronnement/dark_ball_gfx.py`. 4 cachées (Chemin Escarpé, Routes 113-114, Repaire Aqua ; `tools/megas/placer.py`),
  vendues 5000 par le Méga-Collectionneur après la Ligue ; Seko en donne une avant le combat du rival (v0.28.1, tous modes). Tests : `test/dark_ball.c` (`TESTS="Dark Ball"`).

## Fait (v0.29) - Dark Ball retouchée

- Ball de combat redessinée (silhouette ronde de l'Hyper Ball, violet clair, bandes en X) : `balle()` de `tools/couronnement/dark_ball_gfx.py`.
- Capture x2,6 (Hyper Ball +30 %, `COURONNEMENT_DARK_BALL_TAUX` 260), stats hors PV +30 % (`COURONNEMENT_DARK_BALL_BONUS`).
- Sortie du Pokémon : particules de la Master Ball avec un éclat violet sombre (`sBallParticles[BALL_STRANGE]`, `src/battle_anim_throw.c`).
- Méga-Collectionneur : Dark Ball seule à 5000 dès le 6e badge (`MegaGemmes_BoutiqueDarkBall`), tout le stock après la Ligue.
- Résumé : la ligne LIEN affiche « SOMBRE +30 % » pour un Pokémon capturé en Dark Ball.

## Fait (v0.30) - retours d'Adrien sur la Dark Ball

- Résumé : sprite d'un Pokémon sombre teinté violet avec une aura qui pulse (`LancerAuraSombre` / `Task_AuraSombre`,
  `src/pokemon_summary_screen.c`) ; stats affichées « vraie valeur -> valeur avec bonus » en police étroite (`PrintDarkStats`,
  calcul sur une copie sans `MON_DATA_IS_SHADOW`). Colonne de droite des stats élargie d'une tuile (étiquettes sur 32 px).
- Ball de combat plus sombre (palette de `balle()` dans `tools/couronnement/dark_ball_gfx.py`).
- Vendeurs des Boutiques et arbres à Baies en français (`translations/fr/23_boutiques_baies.txt`, stades `.if` traduits à la main).
  Restent en anglais (désactivés par la config) : mutations de Baies, engrais, Noigrumes.

## Fait (v0.30.3) - corrections après test sur mGBA

- Boîtes de texte bloquées : `format_text` de `tools/fr/textfr.py` ne met plus qu'un `\n` par paragraphe, puis des `\l`
  (un `\n` après un `\l` bloque la boîte). `textfr.py check` signale aussi ces cas, `textfr.py fixl` les corrige ; 49 textes régénérés.
- Page stats : case blanche de gauche réduite à 5 tuiles, étiquettes de droite décalées d'une tuile (`page_skills.bin`), valeurs alignées à droite.
- Menus de l'équipe et du PC en français (`src/data/party_menu.h`, `src/pokemon_storage_system.c`). Étiquettes de type en français
  (`tools/fr/types_fr.py`, originaux dans `graphics/couronnement/types_en/`). Titre « MEMO DRESSEUR » remplacé par « CARNET ».
- Pokédex : n° national pour les espèces hors Pokédex de Hoenn, mètres / kilos (`UNITS_METRIC`), « Données ajoutées au POKéDEX. ».
  Reste en anglais : descriptions du Pokédex, étiquettes de Concours.

## Fait (v0.31) - starters retouchés

- Feurisson (N.36) -> Typhlosion le jour (désormais Feu / Sol), Typhlosion de Hisui la nuit (Feu / Spectre) :
  `IF_TIME` / `IF_NOT_TIME` `TIME_NIGHT` dans `src/data/pokemon/species_info/gen_2_families.h` (horloge de la cartouche).
- Arcko, Massko et Jungko sont Plante / Dragon (`gen_3_families.h`).

## Fait (v0.32) - modification des IV

- Capsule d'Argent (`ITEM_BOTTLE_CAP`) : une stat au choix à IV 31 ; Capsule d'Or (`ITEM_GOLD_BOTTLE_CAP`) : les six.
  Utilisables depuis le sac (`ItemUseOutOfBattle_Capsule`, `src/item_use.c`) : script `Capsule_EventScript_Utiliser`
  (`data/scripts/coach_ev.inc`), consommée seulement si un IV change. Pas en combat.
  Vendues par le Méga-Collectionneur de Clémenti dès le 6e badge (avec la Dark Ball) : 30 000 / 150 000
  (`COURONNEMENT_PRIX_CAPSULE_*`, `include/config/item.h`).
- Coach EV / IV (après la Ligue, Centre Pokémon de Clémenti et menu L « Coach EV / IV ») : choix EV ou IV
  (`CoachEV_EventScript_Choix`) ; IV de 0 à 31 stat par stat ou « Tout à 31 », gratuit (`CoachIV_*`, `src/coach_ev.c`).
- On écrit l'IV réel (visible dans le résumé, compte pour Puissance Cachée) et on retire l'Entraînement Ultime éventuel.
  Tests : `test/coach_iv.c` (`TESTS="Coach IV"` et `TESTS="Capsules"`).

## Fait (v0.33) - Bourse de Hoenn (marché)

- Appli « Bourse de Hoenn » dans le menu L dès que le rival a remis le pass (`data/scripts/bourse.inc`, `src/bourse.c`, `include/bourse.h`).
  Accueil (liquide, placements, dette) -> Le marché (9 cours du jour avec flèche) -> fiche (cours, variation d'hier, parts, valeur)
  -> Acheter / Vendre 1, 10, 100 ou le maximum ; Tuyau du jour ; Crédit (emprunter 5 000 / 20 000 / 50 000, rembourser).
- Entreprises : Devon SA, Chantier Stern, Casino Lavandia, Télé Hoenn, Grands Magasins, Aqua Corp, Magma Corp, Pension, Parc Safari
  (cours de départ, tendance, volatilité, krach, envolée dans `sEntreprises`). Frais 3 %, 9999 parts max par entreprise.
- Cours : une fois par jour de jeu (`RtcGetLocalDayCount`, 30 jours rattrapés au plus), hasard déterministe (jour + n° de Dresseur :
  recharger ne change rien). Krachs -40 à -80 %, envolées jusqu'à x3 (Casino), krach général ~1 jour sur 250.
  Chocs du scénario appliqués une fois (`sChocs`, un bit par choc) : pièces Devon, Mont Chimnée, Repaire Magma, Grotte Fonds Marins,
  Centre Spatial, fin de crise à Atalanopolis, badges et Ligue (Télé Hoenn). Crise des légendaires : Stern chute, Aqua / Magma montent ;
  Aqua / Magma en faillite après leur défaite. Télé Hoenn : tendance + (Elo - 1500) / 100 pour mille.
- Grands Magasins : dividende de 2 % du cours par part tous les 7 jours. Crédit : plafond 10 000 + moitié du portefeuille
  (max 500 000), 1 % d'intérêts par jour ; si dette > portefeuille + 10 000, l'huissier vend tout (-10 %) et saisit le liquide,
  annoncé à l'ouverture suivante. Tuyau du jour : rumeur sur le cours de demain d'une entreprise, vraie 7 fois sur 10.
- Sauvegarde : `struct BourseSave` (`include/global.h`) dans l'ancienne zone libre `pokedex.filler` de SaveBlock2 (0x68 octets) ;
  zéros = initialisation à la première ouverture (les chocs déjà passés ne sont pas rejoués). Remise à zéro dans `NewGameInitData`.
  Réglages : `BOURSE_*` dans `include/bourse.h`. Tests : `test/bourse.c` (`TESTS="Bourse"`).
- Reste prévu : courtier louche et quêtes de sabotage (v0.34), rumeurs à la télé / PNJ, immobilier (demande d'Adrien : maisons,
  location, société cotée, boutiques, centre d'entraînement).

## Prochaines étapes, dans l'ordre

0. Priorité d'Adrien : finir le contenu du jeu (équipes, scénario, systèmes) ; la traduction (chapitres 3+ et descriptions) se fera à la fin.
1. Puis : objets tenus récupérés après combat, descriptions des Concours et du Pokédex en français, Blue / Ash / Lucas / Dawn (sprites à fournir).

## Méthode de travail

- Compiler : `make -j$(sysctl -n hw.ncpu)` sur Mac. La ROM sort en `pokeemerald.gba`.
  Toolchain sur le Mac d'Adrien : ARM GNU Toolchain 14.3 dans `~/opt/arm-gnu-toolchain` (pas de devkitPro) ;
  si `arm-none-eabi-gcc` est introuvable : `export PATH="$HOME/opt/arm-gnu-toolchain/bin:$PATH"`.
- Après chaque étape qui compile : commit en français, push sur `main`, puis livrer une copie nommée `emeraude-couronnement-vX.Y.gba`.
- Adrien teste sur Eclipse (iPhone) et mGBA (Mac).
- Garder les réglages dans `include/config/` quand c'est possible plutôt que de modifier le moteur.
- Mettre à jour la section « Fait » et « Prochaines étapes » de ce fichier à chaque étape.
