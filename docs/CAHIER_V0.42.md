# Cahier des charges v0.42+ — Émeraude : Couronnement Mondial

Synthèse de toutes les demandes d'Adrien du 14 au 16/11/2026, à réaliser par lots.
Lire d'abord `CLAUDE.md` (état v0.41.1, méthode de travail). Langue du jeu et des réponses : français.

## Règles techniques (à respecter partout)

1. **Charmap** : aucun caractère typographique dans les chaînes C ou `.inc` (`—`, `«`, `»`, `…`, apostrophes courbes).
   Accents français OK. Boîtes de message : 208 px max par ligne (`python3 tools/fr/textfr.py check`),
   et après un `\l`, toutes les coupures suivantes du paragraphe en `\l` (un `\n` après un `\l` bloque la boîte).
2. **Sauvegarde** : place restante mesurée en v0.41.1 : SaveBlock3 ~840 octets, fin de SaveBlock1 304, fin de SaveBlock2 84,
   fin du PC 1 568 ; zones libres d'origine toutes utilisées ; variables libres : 0x40BB et 0x40E5 seulement.
   Chaque nouveau bloc a un nombre magique (compatibilité des sauvegardes v0.41.1, zéro = initialisation).
   Mesurer la taille avant chaque ajout ; pour les gros systèmes (comptes séparés, décoration de plusieurs intérieurs, villes,
   tenues, clubs), proposer un plan de stockage (bits, compression, boîte du PC réservée, réduction de BOX_COUNT…) AVANT de coder.
3. **Débordements** : calculs intermédiaires en `u64` / `s64`, puis bornés (argent max 999 999 999 ¥).
4. Après chaque lot qui compile : tests (`make check TESTS="..."`), section « Fait » de `CLAUDE.md`, commit en français,
   push sur `main`, ROM livrée sous le nom `emeraude-couronnement-vX.Y.gba`.

## Lot 0 — Audit et simulation (avant de coder)

- Localiser les sources : `src/bourse.c`, `src/bourse_graphique.c`, `src/immobilier.c`, `src/societe.c`, `src/commerces.c`,
  `src/banques.c`, `src/calendrier.c`, `src/arenes.c`, `src/couronnement.c`, scripts `data/scripts/*.inc`, menu L dans `couronnement.inc`.
- Extraire les formules exactes (volatilité et krachs de la Bourse, or, loyers et imprévus, valeur et cours de la société,
  recettes des commerces, intérêts et sanctions des banques, revenus des arènes, primes du Couronnement) et écrire
  `tools/sim_economy.py`, autonome, qui simule 100 jours sur 3 profils : prudent, agressif avec levier (banques), post-Ligue.
- Rendre un rapport : exploits (argent infini, boucles achat/revente, crédit sans risque), goulots, réglages proposés.

## Lot 1 — Gameplay et confort

1. **Câble Link** : objet qui fait évoluer tout Pokémon qui évolue par échange (avec ou sans objet tenu selon l'espèce).
2. **CS sans capacité** : avec la CS dans le sac ET le badge correspondant, Coupe, Vol, Surf, Force, Éclate-Roc, Cascade,
   Plongée, Flash utilisables sur le terrain sans qu'un Pokémon connaisse l'attaque.
3. **Shiny** : plus fréquents, +10 % sur toutes les stats (indiqué dans le résumé, cumulable proprement avec la Dark Ball),
   les rivaux et dresseurs majeurs peuvent avoir des shiny.
4. **Bonbons XP** XS, S, M, L, XL + nouveau **XXL** : très abordables en boutique ; XS / S / M dès le 1er badge, L, XL, XXL
   débloqués avec les badges suivants. Respect strict du level cap (refus ou arrêt de l'XP au plafond).
5. **Champions d'arène** : après chaque victoire, grosse quantité de Bonbons XP adaptée au palier + un objet de combat
   signature réellement tenu par l'as ou un Pokémon clé de leur équipe.

## Lot 2 — Finance

1. **Vente à découvert** : vendre une action sans la détenir (ex. Aqua Corp / Magma Corp avant leur chute).
   Dépôt de garantie bloqué, frais d'emprunt par jour, rachat manuel pour encaisser, gain maximal automatique en cas de faillite,
   appel de marge / liquidation forcée si le cours monte trop. Positions courtes visibles dans la fiche et le portefeuille.
2. **« Mes investissements »** (PokéNav si possible, sinon menu L) : patrimoine net, soldes de chaque compte, lignes de
   Bourse (long / short), or, immobilier, sociétés, plus / moins-values latentes et réalisées, charges et impôts payés, revenus par jour.
3. **Comptes séparés** : compte personnel, un compte par boutique, un compte par société ; virements explicites et tracés
   (apport en capital, dividendes vers le joueur, transferts).
4. **Charges et fiscalité** : entretien périodique de chaque bien, boutique et société ; impôt sur les sociétés sur les bénéfices ;
   impôt sur les plus-values (actions, or, immobilier).
5. **Banques physiques** : bâtiments de banque dans les grandes villes, guichets et conseillers (en plus du menu L).

## Lot 3 — Immobilier et construction

1. **À vendre visible sur la map** : les panneaux existent déjà (`Immo_EventScript_Panneau0..11`) ; afficher prix, loyer estimé,
   charges, et achat direct sur place.
2. **Résidence principale** au choix : pas de loyer, PC privé, soin gratuit, améliorations.
3. **Intérieurs** : porte fonctionnelle pour chaque bien acheté ; décoration en réutilisant le moteur des Bases Secrètes
   (`src/secret_base.c`, `src/decoration.c`), terminal / PC intérieur. **Standing** calculé (rénovations + meubles) qui augmente
   loyer et valeur.
4. **Société de Travaux** (compte séparé, entretien, IS, cotable) : dépôt de Pokémon Combat / Roche / Acier (gros œuvre :
   coûts réduits, plus d'emplacements de déco), Électrique / Eau (réseaux : bonus de standing), Psy (restructuration, surface
   agrandie). Chiffre d'affaires de chantiers externes selon niveau / stats ; répare à prix réduit les imprévus des biens du joueur.
5. **Vague 2** : une fois les 12 maisons achetées, nouveaux biens (villas, immeubles de rapport, résidences secondaires en ville
   et sur les routes clés), plus chers, plus grands intérieurs.
6. **Terrains constructibles** (après les 12 maisons) : parcelles en ville, zones sauvages près des villes, maisons isolées à
   réhabiliter (Routes 110, 111, 117, 119, 123). Construction via la Société de Travaux (matériaux seulement, plus rapide avec ses
   Pokémon, valorise la société) ou via un prestataire externe (bien plus cher, sans gain pour la société).
7. **Fonder une ville** : assembler parcelles et infrastructures (logements, commerces, Centre Pokémon, arène locale, gare / port),
   nom choisi au clavier (stocké dans la sauvegarde, affiché sur la carte et les panneaux), évolution visuelle par phases
   (`MapGridSetMetatileIdAt`, warps / PNJ conditionnels).
8. **Élections municipales** : candidats PNJ (Écologiste, Pro-Business, Populiste…) ou le joueur ; campagne (budget, meetings,
   Télé Hoenn, promesses, réputation légale ; indice de recherche Rocket pénalisant). Maire d'une seule ville à la fois
   (démission pour changer) ; lobbying / financement ailleurs.
9. **Conseils municipaux** dans la mairie : pôle Commercial & Touristique, Industriel & Technologique, Éco-Cité & Réserve
   (réputation, subventions, Pokémon rares autour de la ville), Compétitif & Entraînement ; taux d'imposition local.

## Lot 4 — Sociétés (toutes cotables en Bourse au-delà d'un seuil de valeur)

1. **Taxi Volant** : dépôt de Pokémon Vol depuis le PC ; revenus selon nombre, niveau et stats, moins l'entretien de la flotte.
2. **Taxi Surfant** : même mécanique avec des Pokémon Eau.
3. **Aqua Éco / Magma Éco** : rachat après leur chute dans le scénario, reconversion écologique : recrutement d'anciens
   dresseurs, dépôt de Pokémon, quêtes secondaires écologiques (revenus, réputation, cours).
4. **Labo R&D Poké Ball de Devon** : débloqué par un seuil d'actions Devon SA ; Balls personnalisées (emplacements dédiés) :
   gabarit visuel, couleurs (coque haute / basse, bande, aura), effets modulaires (multiplicateur, bonus de type, premier tour,
   statut, effet à la capture : soin, amitié). Coût de production selon la puissance ; utilisables ou vendues en boutique.
- Pistes déjà en place : Pokémon déposés marqués comme pour les arènes (`MON_DATA_ARENE`, bits libres limités : prévoir un
  autre marquage), société du joueur dans `src/societe.c`.

## Lot 5 — Voie sombre et tenues

1. **Réputation légale** et **indice de recherche** (menu joueur). Au-dessus d'un seuil : Agentes Jenny en ville, amendes,
   accès aux arènes et à la Ligue bloqué un temps. Les missions Aqua / Magma Éco font remonter la réputation et baisser l'indice.
2. **Team Rocket** : QG clandestin, société écran cotable, sbires recrutés et envoyés en mission ; braconnage (missions ou raids
   au Parc Safari, Bois Clémenti) ; **Ball Rocket** pour voler un Pokémon à un dresseur ordinaire (jamais en Ligue ni en tournoi
   du Couronnement), revente, forte hausse de l'indice ; capture de Pokémon puissants en Dark Ball et location / vente à prix d'or
   via un marketing agressif (« gagne la Ligue et le Couronnement »), au détriment de la réputation.
3. **Magasin de vêtements** (Nénucrique ou Lavandia) + cabine d'essayage + garde-robe dans la résidence principale.
   Pour Red, Green, Brice, Flora : 3 gabarits (Standard, Costume PDG, Uniforme Team Rocket ; terrain marche / course / vélo /
   surf + combat face / dos ; placeholders propres si les planches manquent) + coloris par échange de palette (Noir, Bleu marine,
   Blanc, Rouge, Or…). Effets : uniforme Rocket requis pour les opérations Rocket et bloque arènes / Ligue ; costume PDG réduit
   les commissions bancaires / boursières et augmente les sponsors d'arène.

## Lot 6 — Couronnement Mondial

1. **Bâtiment dédié** dans chaque ville à tournoi (style Tente de Combat) : hall avec guichet d'inscription et affichage
   classement / Elo, salle intérieure avec ring où se jouent les matchs (remplace l'hôtesse du Centre Pokémon).
2. **Niveaux** : décision finale d'Adrien (cahier du 16/11) : PAS d'égalisation automatique des niveaux ; le joueur se prépare
   avec les Bonbons XP et l'entraînement.
3. **Ligue des clubs** (proposition d'Adrien du 16/11, à concevoir avec lui avant de coder) : 10 clubs de villes en 2 poules de 5,
   4 dresseurs par club, duels en 6 contre 6 ; le joueur peut être licencié (1 place) ou président (4 places). Esquisse fournie :

```c
#define NUM_CLUBS 10
#define CLUBS_PER_POOL 5
#define TRAINERS_PER_CLUB 4
#define PARTY_SIZE_LEAGUE 6

enum { OWNER_NPC = 0, OWNER_PLAYER_LICENSEE, OWNER_PLAYER_PRESIDENT };

struct ClubTrainer {
    u16 trainerTemplateId; // équipe 6v6
    u8 rating;             // puissance 1-100 pour la simulation IA
    u8 seasonPoints;
    s16 koDifferential;
};

struct CityClub {
    u8 cityId;
    u8 poolId;             // 0 = poule A, 1 = poule B
    u8 ownershipState;
    u8 stadiumFieldEffect; // météo ou terrain à domicile
    u16 seasonPoints;
    s16 koDifferential;
    u8 offensiveBonuses;   // victoires 6-0
    u8 defensiveBonuses;
    struct ClubTrainer roster[TRAINERS_PER_CLUB];
};

// Points « modèle 1 » d'un duel 6v6
void CalculateDuelPoints(u8 homeSurvivors, u8 awaySurvivors, u8 *outHomePts, u8 *outAwayPts)
{
    if (homeSurvivors == 0 && awaySurvivors == 0) { *outHomePts = 2; *outAwayPts = 2; return; }
    if (awaySurvivors == 0) {
        *outHomePts = 4 + (homeSurvivors == PARTY_SIZE_LEAGUE ? 1 : 0); // bonus offensif 6-0
        *outAwayPts = (homeSurvivors == 1 ? 1 : 0);                     // bonus défensif
    } else {
        *outAwayPts = 4 + (awaySurvivors == PARTY_SIZE_LEAGUE ? 1 : 0);
        *outHomePts = (awaySurvivors == 1 ? 1 : 0);
    }
}
```
   Attention : ~32 octets par club, 320 pour 10 : ne tient pas dans la place restante sans plan de stockage.
   Seuls 8 drapeaux de dresseurs sont libres : réutiliser l'emplacement `TRAINER_COURONNEMENT` comme pour les tournois.

## Lot 7 — Traduction

- Traduire en français les chapitres 3 et suivants (dialogues, `data/text/trainers.inc`, scripts de cartes) avec
  `tools/fr/textfr.py` et le ton de `translations/fr/GUIDE.md`, ainsi que tous les nouveaux menus, bâtiments et quêtes.
- Restent aussi : descriptions du Pokédex et des Concours.

## Ordre proposé

Lot 0 → Lot 1 → Lot 2 (vente à découvert en premier) → Lot 6.1 → Lot 4 → Lot 3 → Lot 5 → Lot 6.3 → Lot 7 en continu.
Avant chaque gros lot : poser à Adrien les questions de conception (avec des choix) et présenter le budget de sauvegarde.
