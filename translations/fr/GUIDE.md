# Traduction française — guide de ton et lexique

## Ton (v0.4, après le premier test d'Adrien)

Le mélange recherché : **humour manga + textes adultes qui ont du fond + trash quand ça sert la scène.**

- **Humour manga** : réactions exagérées, gags récurrents, persos qui cassent le quatrième mur ou commentent la situation,
  ruptures de ton (une scène tragique coupée net par un gag, puis une phrase qui fait mal), répliques punchlines.
  Exemples de ressorts : le Prof. Seko qui frôle la mort à chaque apparition, Maman qui pleure en mode cascade puis lâche
  une phrase glaciale, le rival qui se la raconte et se fait remettre à sa place.
- **Profondeur et vrai sens** : chaque personnage important porte un thème (l'absence du père, la peur de mourir jeune,
  le deuil, le prix de la force, ce qu'on doit aux Pokémon qu'on envoie se battre). Une ou deux phrases qui restent en tête
  par scène, pas plus : le fond se glisse entre deux vannes.
- **Trash si besoin** : jurons francs, insultes, humour cru ou dégueu, menaces violentes chez les voyous, cynisme assumé.
  Ça doit servir le perso ou la scène, pas remplir. Pas de contenu sexuel explicite, pas de gore gratuit.
- **Enfants** : ils peuvent être drôles et dire des choses dures avec innocence, mais ils ne jurent pas.
- **Panneaux et messages système** (objet reçu, PC, menus) : clairs. Un panneau peut avoir une blague tagguée dessus.
- **Le scénario d'Émeraude ne change pas** : mêmes événements, objets et combats.
- **Le joueur** peut être une fille ou un garçon : pas d'accord genré quand on s'adresse à lui.

## Personnages (voix)

| Personnage | Nom FR | Voix |
|---|---|---|
| Professeur Birch | Prof. Seko | Génie de terrain suicidaire. Gag : il manque de mourir à chaque scène et tient le compte. Fond : il sait que la nature ne doit rien à personne, et il y va quand même. |
| Mom | Maman | Mère poule en mode pleurs-cascade, puis phrase glaciale. Fond : elle a épousé un homme qui ne rentre jamais et voit son enfant prendre le même chemin. |
| Norman | Norman | Shōnen de 45 ans, obsédé par la force. Parle peu. Fond : il a choisi l'Arène plutôt que sa famille et il le sait. |
| May (rival si joueur garçon) | Flora | Pile électrique, grande gueule, jure quand elle est vexée. Gag : elle veut être amie avec les Pokémon qui ont essayé de la bouffer. Fond : elle a peur de finir comme son père. |
| Brendan (rival si joueuse fille) | Brice | Rival shōnen qui se la raconte, se fait remettre à sa place. Fond : il vit dans l'ombre de son père et n'en peut plus. |
| Wally | Timmy | Gamin malade, petit corps, volonté monstre. Fond : il sait qu'il peut mourir et veut vivre une vraie aventure avant. Ne jure pas. |
| Team Aqua / Magma | Team Aqua / Team Magma | Voyous, trash-talk, menaces sérieuses. Bêtes mais dangereux. |
| Roxanne | Roxanne | Major de promo à lunettes, froide, condescendante. Gag : elle cite ses notes. Fond : elle n'a jamais perdu et ne sait pas quoi faire de l'échec. |
| Mr. Stone | M. Rochard | PDG charmeur, trop poli pour être honnête. Fond : il ne parle plus à son fils. |
| Mr. Briney | M. Marco | Vieux loup de mer bruyant qui sent le rhum. Fond : la mer a pris son équipage, Peeko est tout ce qui lui reste. |
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
