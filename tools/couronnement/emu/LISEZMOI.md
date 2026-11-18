# Tester la ROM sans écran (mGBA en bibliothèque)

Pré-requis (Linux) : `apt-get install mgba-sdl libmgba-dev`.

    gcc tools/couronnement/emu/jouer.c -o jouer -lmgba
    gcc tools/couronnement/emu/trace_plantage.c -o trace -lmgba

- ROM de test qui démarre directement devant l'herboriste de Clémenti (badge 5, pass) :
  `make EDITION=carriere DEPART=1` (code sous `COURONNEMENT_DEBUG_DEPART` dans `src/intro.c` et `WarpToTruck`, `src/new_game.c` ;
  changer la carte / la case dans `WarpToTruck`). Penser à recompiler sans `DEPART=1` avant de livrer.
- `./jouer rom.gba script.txt prefixe` : chaque ligne du script = `<images> <touches en hexa> [nom de capture]`
  (A=1, B=2, SELECT=4, START=8, droite=10, gauche=20, haut=40, bas=80, R=100, L=200). Les captures sortent en PPM.
- `./trace rom.gba script.txt` : rejoue le script, appuie sur A puis suit le processeur pas à pas ; si le jeu redémarre
  (saut au début de la ROM), affiche les 300 derniers PC, les objets du terrain et les sprites actifs
  (adresses de `gSprites` / `gObjectEvents` à mettre à jour avec `arm-none-eabi-nm pokeemerald.elf`).
