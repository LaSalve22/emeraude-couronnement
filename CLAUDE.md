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

## Prochaines étapes, dans l'ordre

0. Traduction : faire valider le ton v0.4 par Adrien, puis chapitres 2 et suivants et les descriptions.
1. Descente de difficulté en cours de partie (menu ou PNJ), jamais de remontée.
2. Blocage des objets de soin en combat et mode Set forcé en Hard et Kaizo.
3. Téracristal (après badge 2) et Méga-évolution (après badge 5).
4. Puis : équipes Hard/Kaizo du chapitre 1, Couronnement Mondial, liens avec les Pokémon, personnages jouables.

## Méthode de travail

- Compiler : `make -j$(sysctl -n hw.ncpu)` sur Mac. La ROM sort en `pokeemerald.gba`.
  Toolchain sur le Mac d'Adrien : ARM GNU Toolchain 14.3 dans `~/opt/arm-gnu-toolchain` (pas de devkitPro) ;
  si `arm-none-eabi-gcc` est introuvable : `export PATH="$HOME/opt/arm-gnu-toolchain/bin:$PATH"`.
- Après chaque étape qui compile : commit en français, push sur `main`, puis livrer une copie nommée `emeraude-couronnement-vX.Y.gba`.
- Adrien teste sur Eclipse (iPhone) et mGBA (Mac).
- Garder les réglages dans `include/config/` quand c'est possible plutôt que de modifier le moteur.
- Mettre à jour la section « Fait » et « Prochaines étapes » de ce fichier à chaque étape.
