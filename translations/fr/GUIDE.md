# Traduction française — guide de ton et lexique

## Ton

- **Plus adulte, façon manga** : plus mûr, plus sombre, avec de l'humour et des répliques qui piquent.
- **Jurons autorisés** quand le personnage s'y prête : « merde », « putain », « bordel », « connard », « fais chier ».
  Jamais dans la bouche des enfants, ni dans les panneaux et les messages système.
- **Thèmes durs** autorisés : la mort, le deuil, la maladie, la violence des Pokémon sauvages, la peur, l'ambition qui écrase les autres.
  Pas de contenu sexuel, pas de gore gratuit.
- **Le scénario d'Émeraude ne change pas** : mêmes événements, mêmes objets, mêmes combats. Seuls le ton et les dialogues changent.
- Messages système (objets reçus, panneaux, PC, menus) : sobres et clairs, sans jurons.

## Personnages (voix)

| Personnage | Nom FR | Voix |
|---|---|---|
| Professeur Birch | Prof. Seko | Chercheur de terrain brillant mais imprudent, un peu cynique. Sait que la nature tue. |
| Mom | Maman | Fatiguée, aimante, inquiète. Elle connaît le prix de la vie de dresseur (son mari n'est jamais là). |
| Norman | Norman | Champion de Clémenti-Ville. Dur, distant, fier. Père absent. |
| May / Brendan (rival) | Flora / Brice | Compétitif, sarcastique, piquant, mais loyal au fond. Jure un peu. |
| Wally | Timmy | Enfant malade, fragile, lucide sur sa maladie. Touchant. Ne jure pas. |
| Team Aqua / Magma | Team Aqua / Team Magma | Voyous et fanatiques. Menaces réelles. Jurent. |
| Roxanne | Roxanne | Prodige froide, méthodique, condescendante. |
| Mr. Stone | M. Rochard | PDG de Devon SA. Charmeur, ambigu. |
| Mr. Briney | M. Marco | Vieux marin, a perdu son équipage en mer. Bourru. |
| Steven | Pierre Rochard | Fils de M. Rochard, ne lui parle plus. |
| Scott | Scott | Recruteur mystérieux. |

Le joueur peut être une fille ou un garçon : **éviter les accords genrés** quand on s'adresse au joueur
(« tu as l'air solide » plutôt que « tu as l'air fort », « merci de m'avoir accompagné » plutôt que « merci d'être venu »).

## Lieux

| Anglais | Français |
|---|---|
| Littleroot Town | Bourg-en-Vol |
| Oldale Town | Rosyères |
| Petalburg City | Clémenti-Ville |
| Petalburg Woods | Bois Clémenti |
| Rustboro City | Mérouville |
| Rusturf Tunnel | Tunnel Mérazon |
| Verdanturf Town | Vergazon |
| Lilycove City | Nénucrique |
| Ever Grande City | Éternara |
| Dewford Town | Village Myokara |
| Slateport City | Poivressel |
| Mauville City | Lavandia |
| Route 101… | Route 101… |
| Devon Corporation | Devon SA |
| Pokémon Center | Centre Pokémon |
| Poké Mart | Boutique Pokémon |
| Pokémon Trainer's School | École des Dresseurs |
| Hoenn | Hoenn |

## Pokémon, objets, termes

- Pokémon : noms français officiels (Zigzaton, Medhyèna, Chenipotte, Arcko, Héricendre, Carapuce, Tarsal, Nirondelle,
  Goélise, Racaillou, Tarinor, Balignon, Grainipiot, Parecool, Vigoroth, Monaflèmit…).
- Graphie : « Pokémon » (casse normale), « Poké Ball », « Pokédex ». Noms propres en casse normale (Flora, Prof. Seko).
- Objets : Potion, Poké Ball, Chaussures de sport, Pièces Devon, Lettre, CS01 Coupe, CT39 Tomberoche, Badge Roche.

## Limites connues (chapitre 1)

- Les noms de Pokémon, d'attaques, d'objets et les menus du moteur restent en anglais tant que la traduction globale
  n'est pas faite : en combat on verra encore « SQUIRTLE ». Étape à prévoir après validation du ton.
- Les dresseurs et le rival sont renommés dans `src/data/trainers.party` au fur et à mesure.

## Méthode

1. `python3 tools/fr/textfr.py dump data/maps/<Carte>/scripts.inc` pour voir le texte anglais.
2. Écrire `translations/fr/<zone>.txt` (format décrit en tête de `tools/fr/textfr.py`).
3. `python3 tools/fr/textfr.py apply translations/fr/*.txt` remet en forme (largeur en pixels) et remplace les textes.
4. `python3 tools/fr/textfr.py check data/maps/*/scripts.inc` puis `make`.
