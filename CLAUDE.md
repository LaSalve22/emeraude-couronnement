# Émeraude — Couronnement Mondial

Romhack de Pokémon Émeraude d'Adrien, basé sur pokeemerald-expansion 1.17.2.
Réponds toujours en français. Le jeu lui-même reste en anglais.

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

## Prochaines étapes, dans l'ordre

1. Starters Carapuce / Arcko / Héricendre à la place du trio de Hoenn, talent caché, +20 % de stats (version spéciale réservée au joueur).
2. Choix du mode au lancement (Normal / Hard / Kaizo) : ajouter `DIFFICULTY_KAIZO`, écran ou dialogue de choix, descente possible plus tard.
3. Blocage des objets de soin en combat et mode Set forcé en Hard et Kaizo.
4. Téracristal (après badge 2) et Méga-évolution (après badge 5).
5. Puis : équipes Hard/Kaizo du chapitre 1, Couronnement Mondial, liens avec les Pokémon, personnages jouables.

## Méthode de travail

- Compiler : `make -j$(sysctl -n hw.ncpu)` sur Mac. La ROM sort en `pokeemerald.gba`.
- Après chaque étape qui compile : commit en français, push sur `main`, puis livrer une copie nommée `emeraude-couronnement-vX.Y.gba`.
- Adrien teste sur Eclipse (iPhone) et mGBA (Mac).
- Garder les réglages dans `include/config/` quand c'est possible plutôt que de modifier le moteur.
- Mettre à jour la section « Fait » et « Prochaines étapes » de ce fichier à chaque étape.
