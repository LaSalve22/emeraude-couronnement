# Audit de l'économie (lot 0 du cahier v0.42) — état v0.41.1

Simulateur : `python3 tools/sim_economy.py [--jours 100] [--graine 0x...] [--md fichier]`.
Il reprend les formules exactes du jeu, avec le même hasard déterministe (Melanger / Alea, graine = n° de Dresseur) et
l'arithmétique entière du C. Les chiffres ci-dessous viennent de la graine 0x2A5F17C3 ; trois autres graines donnent les mêmes conclusions.
Constat fait sur la v0.41.1 ; les failles 1 et 2 sont corrigées en v0.42 (le simulateur applique désormais les nouvelles règles,
`--v041` rejoue les anciennes).

## 1. Formules (sources)

| Système | Fichier | Formules clés |
|---|---|---|
| Bourse | `src/bourse.c` | 9 entreprises : tendance + aléa ±volatilité ‰/jour ; krach (-40 à -80 %) et envolée (+30 % à +envoléeMax) en ‰/jour ; krach général 4/1000 (-30 à -60 %) ; fermée le week-end ; frais 3 % ; 9 999 parts max ; cours 2 à 60 000. Grands Magasins : dividende 2 % du cours par part chaque vendredi. Crédit : 1 %/jour, plafond 10 000 + moitié du portefeuille (5 M max), huissier si dette > portefeuille + 10 000 (vente -10 %, puis liquide). Chocs du scénario une seule fois (`sChocs`). |
| Or | `src/bourse.c` | +0,1 ‰/jour (+0,4 ‰ pendant la crise), ±1 %, chute 2/1000 (-5 à -10 %), envolée 3/1000 (+5 à +15 %), +4 à +10 % les jours de krach général ; frais 1 %. |
| Immobilier | `src/immobilier.c` | Prix = base × indice/1000 × (1 ± 10 % local, change chaque semaine) × 0,75 pour le bien soldé de la semaine. Indice +0,1 %/jour ±0,8 %, krach 2/1000 (-15 à -30 %), -0,3 %/jour pendant la crise. Achat +8 %, revente -5 %. Chaque jour et par bien : squat 1,5 % (Aqua active), impayé 5 %, travaux 3 % (3 % du prix de base), sinon loyer = loyer de base × indice. |
| Société | `src/societe.c` | Valeur = maisons du joueur + commerces + arènes + trésorerie. Cours = valeur × humeur / 10 000 actions ; humeur ±1,5 %/jour (+0,3 % si trésorerie > 0), bornée 300 à 3 000 ‰. Introduction à 100 000 ¥ (25 %), paquets de 500, plancher 51 %, frais 3 %. |
| Commerces | `src/commerces.c` | Boutique : passage × stock % × niveau % × prix % / 10⁶ par jour, ±aléa du stock, samedi +30 %, dimanche -50 %, concurrent -40 % (0,5 à 2 %/jour d'arrivée). Centre : 250 ¥ et 30 points par niveau et par jour. Valeur = prix payés. |
| Banques | `src/banques.c` | Devon 0,3 %/jour (plafond 30 000 + 40 000/badge, +1 M après la Ligue), Foncier 0,5 % (60 % maisons + commerces), Casino 2 % (200 000 + 50 000/badge). Échéance du lundi 5 % (500 min) sur le liquide seul ; défaut : Devon +10 %, Foncier saisit la maison la moins chère (-20 %), Casino prend le liquide puis +20 %. |
| Arènes | `src/arenes.c` | Sponsors 150 ¥ × réputation (0-100) par jour et par arène ; victoire contre un challenger : +3 réputation, 2 000 ¥ × badges ; défaite : -8, 5 000 ¥ × badges payés. Valeur = 500 000 ¥ par arène. |
| Couronnement | `src/couronnement.c` | Paliers 30 000 / 100 000 / 300 000 / 1 000 000 ¥ (une fois par saison), tournois (saison 2+) 20 000 + 10 000 × n°, finale 500 000 (+3 000 000 si gagnée) ; × 1 + 0,5 par saison, × 3 max. |
| Calendrier | `src/calendrier.c` | Jour 1 = samedi ; tout se rattrape au plus 30 jours en arrière. |

## 2. Profils simulés (100 jours)

| Profil | J+10 | J+30 | J+60 | J+100 | Dettes | Maisons |
|---|---|---|---|---|---|---|
| Prudent (4 badges, 150 000 ¥, sans crédit) | 147 494 | 150 372 | 170 382 | 185 480 | 0 | 2 |
| Agressif (60 000 ¥, crédits au maximum, tout en Bourse) | -360 113 | -1 408 710 | -6 328 924 | -36 162 155 | 36 162 155 | 0 |
| Levier raisonné (150 000 ¥, Banque Devon, garde pour les lundis) | 146 806 | 151 742 | 157 896 | 195 671 | 125 061 | 4 |
| Post-Ligue (2 000 000 ¥, société, commerces, arènes) | 2 146 855 | 2 661 413 | 4 454 436 | 8 904 152 | 0 | 10 |

Lecture :
- **Début de partie lent** : un joueur prudent gagne ~35 000 ¥ en 100 jours (+24 %). Avant 4 badges, la finance ne sert presque à rien.
- **Le levier tue… sur le papier** : le joueur agressif accumule 36 M ¥ de dette, mais (voir faille 2) **rien ne se passe en jeu** :
  il garde le liquide emprunté. Le levier raisonné (Devon) rapporte un peu plus que le prudent, sans risque réel.
- **Après la Ligue, les arènes écrasent tout** : 8,4 M ¥ sur 100 jours (sponsors 150 ¥ × réputation, jusqu'à 15 000 ¥/jour par arène),
  contre 0,5 M de loyers et 0,3 M de boutique. Les autres placements deviennent inutiles.

## 3. Failles, de la plus grave à la moins grave

| # | Faille | Effet mesuré | Correctif proposé |
|---|---|---|---|
| 1 | ✅ v0.42 — **Horloge remise en arrière** : si `aujourdhui < jour`, chaque module recale son jour puis **rejoue** les mêmes jours quand l'horloge revient (loyers, recettes, dividendes, sponsors, variations de cours). | 30 jours rejoués 5 fois : 3,2 M -> 5,7 M ¥ (jusqu'à 10,8 M selon la graine). Illimité. | Garder le jour le plus avancé ; si l'horloge recule, **geler** (rien ne se passe tant qu'on n'a pas dépassé ce jour). Un seul helper `Calendrier_JourValide()` pour les 6 modules. |
| 2 | ✅ v0.42 — **Défaut bancaire sans saisie** : l'échéance ne se prend que sur le liquide du lundi ; Devon ne fait qu'ajouter 10 %, le Casino ne prend que le liquide. En gardant tout en Bourse / trésorerie le dimanche, on ne rembourse jamais. | 1,95 M ¥ empruntés et gardés ; la dette monte à 62 M sans aucune conséquence. | Échéance prise aussi sur la trésorerie puis le portefeuille ; au 2e défaut : **interdit bancaire** (plus de crédit nulle part), saisie des actions / or (-10 %) puis des maisons ; au 4e : huissier général. |
| 3 | **Double comptage maisons / société** : les maisons appartiennent au joueur mais gonflent la valeur de **sa** société. On vend 49 % des actions puis on revend les maisons. | +555 000 ¥ immédiats sur 1,9 M de maisons, la société cotée vaut 0. | Les maisons comptent dans la société seulement si elles y sont **apportées** (apport en capital, lot 2.3) ; un bien apporté ne se revend que par la société. |
| 4 | **Humeur de la société** sans lien avec les résultats (300 à 3 000 ‰, marche au hasard) : on vend des actions jusqu'à 3× la valeur réelle. | +36 % en 1 an sans rien produire (480 000 -> 652 175 ¥). | Humeur bornée 700-1 500 ‰ et rappelée vers 1 000 ; cours qui baisse quand le joueur vend (impact de 1 % par paquet) ; humeur liée aux bénéfices (recettes - charges). |
| 5 | **Bien soldé de la semaine** : -25 % à l'achat, puis revente la semaine où l'écart local est à +10 %. | 22 allers-retours rentables en 30 semaines, +450 000 ¥ (en plus des loyers). | Revente au prix d'indice **sans** écart local ni remise, ou impôt sur la plus-value (lot 2.4) de 30 % si détenu moins de 30 jours. |
| 6 | ✅ v0.42.1 — **Dividende des Grands Magasins** : 2 % du cours **par semaine** (~104 % par an). | Position de 5 M ¥ : 100 000 ¥ par semaine sans risque. | 0,2 % par semaine (~10 %/an), ou dividende sur le bénéfice. |
| 7 | ✅ v0.42.1 — **Crédit Foncier** : plafond sur les maisons, mais les maisons restent vendables une fois l'argent emprunté. | Prêt sans garantie (puis défaut = +10 % seulement s'il n'y a plus de maison). | Maison hypothéquée = invendable tant que la dette Foncier n'est pas remboursée. |
| 8 | ✅ v0.42.1 — **Sponsors des arènes** trop forts (voir profil post-Ligue). | 15 000 ¥/jour par arène à réputation 100, ~5,4 M ¥/an par arène. | 50 ¥ par point (5 000 ¥/jour max), plus les charges d'entretien du lot 2.4. |
| 9 | **Voyage dans le futur** : le hasard ne dépend que du jour et du n° de Dresseur. Avancer l'horloge, regarder les cours, relancer sans sauvegarder, acheter. | Information parfaite sur les cours de demain. | Inévitable avec l'horloge de la cartouche ; à accepter (c'est l'esprit du « délit d'initié ») ou à freiner avec un délai d'un jour ouvré avant de revendre. |
| 10 | **Délit d'initié du scénario** (prévu) : Aqua +80 % à la Grotte Fonds Marins, Stern +100 % à Atalanopolis, Magma +60 % au Repaire. | 9 999 parts d'Aqua (~8 M ¥) : +6,4 M ¥ en un jour. | Garder (c'est voulu), mais c'est la bonne place pour la vente à découvert du lot 2.1 (Aqua / Magma avant leur faillite). |

## 4. Goulots et équilibrage

- **Avant 4 badges**, aucune source ne rapporte vraiment : la maison la moins chère (Myokara, ~38 000 ¥ frais compris) rapporte ~200 ¥/jour,
  remboursée en ~250 jours de jeu. Piste : des primes de dresseur plus généreuses en Hard / Kaizo, ou le Lot 1 (Bonbons XP achetables) comme vrai puits d'argent.
- **Les loyers** (~0,42 %/jour net après imprévus, ~150 %/an) sont très au-dessus d'un vrai marché, mais restent lents en valeur absolue : à garder,
  avec les charges du lot 2.4 (entretien ~0,05 %/jour du prix).
- **Le Couronnement** paie 4,9 M ¥ la 1re saison et 17 M ¥ à partir de la 5e : cohérent comme récompense de fin de jeu, à condition de corriger les failles 1, 2 et 8,
  sinon l'argent ne sert plus à rien bien avant.
- **Plafonds** : tous les calculs internes sont déjà bornés à 2 000 000 000 en 64 bits ; aucun débordement trouvé.

## 5. Corrigé en v0.42

- **Horloge** : `Calendrier_Aujourdhui()` ne revient jamais avant le dernier jour traité (Bourse, société, immobilier, banques, arènes) ;
  chaque `*_MettreAJourJusqua` gèle au lieu de reculer. Les prix de l'immobilier gardent aussi la semaine la plus avancée.
  Simulation : 5 retours en arrière de 30 jours ne rapportent plus rien (3 214 546 ¥ dans les deux cas).
- **Banques** : l'échéance du lundi est prise sur le liquide, puis la trésorerie de la société, puis par vente forcée des placements
  (or d'abord). Défaut seulement si tout cela ne suffit pas. 2e défaut chez une banque : interdit bancaire (plus aucun crédit, Bourse
  comprise) et une maison saisie ; 4e : maisons saisies jusqu'à solder la dette. Rembourser une dette efface ses défauts.
  Simulation : 1,95 M ¥ empruntés et placés en Bourse ; il en reste 0,9 M au bout de 100 jours (2,3 M avant le correctif).

## 6. Corrigé en v0.42.1 (réglages)

- **Dividende** des Grands Magasins : 0,2 % du cours par part et par semaine (`BOURSE_DIVIDENDE_POURMILLE` = 2), soit ~10 %/an.
- **Hypothèque** : une maison dont la vente ferait passer la garantie (60 % des biens restants + commerces) sous la dette du Crédit Foncier
  est invendable (`Immo_Hypothequee`) ; message « Cette maison garantit ton prêt du Crédit Foncier. Rembourse d'abord. ».
- **Sponsors** des arènes : 50 ¥ par point de réputation et par jour (`ARENE_SPONSORS_PAR_POINT`), 5 000 ¥/jour par arène au maximum.
  Profil post-Ligue sur 100 jours : 8,9 M -> 5,8 M ¥ ; ce qui reste vient surtout des combats contre les challengers (à jouer soi-même).

## 7. Ordre de correction proposé (suite)

1. ~~Horloge et défauts bancaires~~ (fait en v0.42).
2. ~~Dividende, sponsors, Foncier~~ (fait en v0.42.1).
3. Société et immobilier (failles 3, 4, 5) : à faire avec les comptes séparés et la fiscalité du lot 2 (apport en capital, plus-values).
