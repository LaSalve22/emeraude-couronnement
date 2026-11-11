#!/usr/bin/env python3
"""Équipes Hard et Kaizo des dresseurs d'Émeraude Couronnement.

Ajoute (ou remplace) à la fin de src/data/trainers.party un bloc « Difficulty: Hard » et un bloc
« Difficulty: Kaizo » pour chaque dresseur listé ici. L'en-tête (nom, classe, sprite, IA…) est recopié
du bloc Normal ; seul l'équipe change. Le moteur choisit le bloc selon VAR_DIFFICULTY.

Format d'un Pokémon (syntaxe Showdown, noms anglais) :
    "Geodude @ Oran Berry | Impish | Sturdy | 15 | 252 Def / 252 HP | Rock Tomb, Magnitude, Defense Curl, Rock Throw"
    champs : espèce [@ objet] | nature | talent | niveau | EVs ('' = aucun) | capacités

Règles de conception (chapitre 1, level cap 18 avant Roxanne, 27 avant Bastien) :
- Hard : équipes plus variées et mieux construites, IV 20, quelques EVs, niveaux ~+3.
- Kaizo : IV 31, EVs ciblés, natures, objets, niveaux ~+5, jamais au-dessus du cap.

Usage : python3 tools/trainers/equipes_couronnement.py
"""
import re
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
PARTY = ROOT / 'src/data/trainers.party'
BEGIN = '/* --- Émeraude Couronnement : équipes Hard / Kaizo (généré par tools/trainers/equipes_couronnement.py) --- */'
END = '/* --- fin des équipes Couronnement --- */'

IV = {'Hard': 20, 'Kaizo': 31}

# ------------------------------------------------------------------ chapitre 1
TEAMS = {
    # Route 102
    'CALVIN_1': {
        'Hard': ["Poochyena | Jolly | Quick Feet | 7 | | Tackle, Howl, Sand Attack, Bite"],
        'Kaizo': ["Poochyena @ Oran Berry | Jolly | Quick Feet | 8 | 252 Atk / 252 Spe | Bite, Howl, Sand Attack, Tackle",
                  "Zigzagoon | Jolly | Pickup | 7 | 252 Spe | Headbutt, Tail Whip, Sand Attack, Baby-Doll Eyes"],
    },
    'RICK': {
        'Hard': ["Wurmple | Adamant | Shield Dust | 6 | | Tackle, String Shot, Poison Sting, Bug Bite",
                 "Weedle | Jolly | Shield Dust | 6 | | Poison Sting, String Shot, Bug Bite"],
        'Kaizo': ["Weedle @ Oran Berry | Jolly | Shield Dust | 8 | 252 Atk / 252 Spe | Poison Sting, Bug Bite, String Shot",
                  "Wurmple | Adamant | Shield Dust | 8 | 252 Atk / 252 HP | Bug Bite, Poison Sting, String Shot, Tackle",
                  "Kricketot | Adamant | Shed Skin | 8 | 252 Atk | Bug Bite, Bide, Growl"],
    },
    'TIANA': {
        'Hard': ["Zigzagoon | Jolly | Pickup | 6 | | Headbutt, Tail Whip, Sand Attack",
                 "Shroomish | Bold | Effect Spore | 6 | | Absorb, Stun Spore, Tackle"],
        'Kaizo': ["Shroomish @ Oran Berry | Bold | Effect Spore | 8 | 252 HP / 252 Def | Stun Spore, Absorb, Leech Seed, Tackle",
                  "Hoppip | Timid | Chlorophyll | 8 | 252 Spe | Fairy Wind, Synthesis, Tail Whip, Absorb",
                  "Zigzagoon | Jolly | Pickup | 8 | 252 Atk / 252 Spe | Headbutt, Sand Attack, Tail Whip, Baby-Doll Eyes"],
    },
    'ALLEN': {
        'Hard': ["Taillow | Jolly | Guts | 6 | | Peck, Growl, Quick Attack",
                 "Zigzagoon | Jolly | Pickup | 6 | | Headbutt, Tail Whip, Sand Attack"],
        'Kaizo': ["Starly @ Oran Berry | Jolly | Keen Eye | 8 | 252 Atk / 252 Spe | Quick Attack, Wing Attack, Growl",
                  "Taillow | Jolly | Guts | 8 | 252 Atk / 252 Spe | Peck, Quick Attack, Growl, Focus Energy",
                  "Bidoof | Adamant | Simple | 8 | 252 Atk / 252 HP | Headbutt, Defense Curl, Rollout"],
    },
    # Bois Clémenti
    'LYLE': {
        'Hard': ["Wurmple | Adamant | Shield Dust | 6 | | Bug Bite, Poison Sting, String Shot",
                 "Silcoon | Bold | Shed Skin | 7 | | Harden, Tackle",
                 "Cascoon | Bold | Shed Skin | 7 | | Harden, Tackle",
                 "Kakuna | Bold | Shed Skin | 7 | | Harden, Poison Sting"],
        'Kaizo': ["Beautifly @ Oran Berry | Modest | Swarm | 10 | 252 SpA / 252 Spe | Gust, Absorb, Stun Spore",
                  "Dustox | Bold | Shield Dust | 10 | 252 HP / 252 Def | Confusion, Poison Sting, Gust",
                  "Kakuna | Bold | Shed Skin | 9 | 252 HP | Harden, Poison Sting",
                  "Paras | Adamant | Effect Spore | 9 | 252 Atk | Stun Spore, Scratch, Absorb"],
    },
    'JAMES_1': {
        'Hard': ["Nincada | Adamant | Compound Eyes | 8 | | Scratch, Sand Attack, Leech Life, Harden",
                 "Paras | Adamant | Effect Spore | 8 | | Stun Spore, Scratch, Absorb"],
        'Kaizo': ["Nincada @ Oran Berry | Adamant | Compound Eyes | 10 | 252 Atk / 252 HP | Leech Life, Sand Attack, Harden, Scratch",
                  "Paras | Adamant | Effect Spore | 10 | 252 Atk / 252 HP | Stun Spore, Absorb, Scratch, Fury Cutter",
                  "Venonat | Calm | Compound Eyes | 10 | 252 HP / 252 SpD | Disable, Supersonic, Confusion, Tackle"],
    },
    'GRUNT_PETALBURG_WOODS': {
        'Hard': ["Poochyena | Jolly | Quick Feet | 10 | | Bite, Howl, Sand Attack, Tackle",
                 "Carvanha | Adamant | Rough Skin | 9 | | Bite, Leer, Focus Energy"],
        'Kaizo': ["Poochyena @ Oran Berry | Jolly | Quick Feet | 12 | 252 Atk / 252 Spe | Bite, Howl, Sand Attack, Tackle",
                  "Carvanha | Adamant | Rough Skin | 12 | 252 Atk / 252 Spe | Bite, Aqua Jet, Leer, Focus Energy",
                  "Stunky | Jolly | Stench | 11 | 252 Atk / 252 Spe | Fury Swipes, Poison Gas, Feint"],
    },
    # Route 104
    'BILLY': {
        'Hard': ["Zigzagoon | Jolly | Pickup | 8 | | Headbutt, Tail Whip, Sand Attack",
                 "Seedot | Adamant | Chlorophyll | 9 | | Bullet Seed, Harden, Bide"],
        'Kaizo': ["Seedot @ Oran Berry | Adamant | Early Bird | 11 | 252 Atk / 252 HP | Bullet Seed, Harden, Astonish, Growth",
                  "Sandshrew | Adamant | Sand Veil | 11 | 252 Atk / 252 Def | Rollout, Defense Curl, Sand Attack, Poison Sting",
                  "Zigzagoon | Jolly | Pickup | 10 | 252 Atk / 252 Spe | Headbutt, Sand Attack, Tail Whip, Baby-Doll Eyes"],
    },
    'HALEY_1': {
        'Hard': ["Lotad | Calm | Rain Dish | 8 | | Absorb, Water Gun, Growl",
                 "Shroomish | Bold | Effect Spore | 8 | | Absorb, Stun Spore, Leech Seed"],
        'Kaizo': ["Lotad @ Oran Berry | Calm | Swift Swim | 11 | 252 HP / 252 SpD | Mega Drain, Water Gun, Growl, Mist",
                  "Shroomish | Bold | Effect Spore | 11 | 252 HP / 252 Def | Leech Seed, Stun Spore, Mega Drain, Headbutt",
                  "Budew | Timid | Natural Cure | 10 | 252 SpA / 252 Spe | Mega Drain, Stun Spore, Growth"],
    },
    'WINSTON_1': {
        'Hard': ["Zigzagoon @ Oran Berry | Jolly | Pickup | 9 | | Headbutt, Tail Whip, Sand Attack, Odor Sleuth",
                 "Skitty | Jolly | Normalize | 9 | | Fake Out, Tackle, Tail Whip"],
        'Kaizo': ["Meowth @ Silk Scarf | Jolly | Technician | 12 | 252 Atk / 252 Spe | Fake Out, Fury Swipes, Bite, Pay Day",
                  "Skitty | Jolly | Normalize | 11 | 252 Atk / 252 Spe | Fake Out, Double Kick, Tail Whip, Sing",
                  "Zigzagoon @ Oran Berry | Jolly | Pickup | 11 | 252 Atk / 252 Spe | Headbutt, Sand Attack, Baby-Doll Eyes, Odor Sleuth"],
    },
    'CINDY_1': {
        'Hard': ["Zigzagoon | Jolly | Pickup | 9 | | Headbutt, Tail Whip, Sand Attack",
                 "Azurill | Bold | Thick Fat | 9 | | Bubble, Charm, Tail Whip"],
        'Kaizo': ["Marill @ Oran Berry | Adamant | Huge Power | 11 | 252 Atk / 252 HP | Bubble, Defense Curl, Rollout, Tail Whip",
                  "Clefairy | Bold | Cute Charm | 11 | 252 HP / 252 Def | Disarming Voice, Defense Curl, Sing, Pound",
                  "Zigzagoon | Jolly | Pickup | 11 | 252 Atk / 252 Spe | Headbutt, Sand Attack, Tail Whip, Baby-Doll Eyes"],
    },
    'GINA_AND_MIA_1': {
        'Hard': ["Seedot | Adamant | Chlorophyll | 8 | | Bullet Seed, Harden, Bide",
                 "Lotad | Modest | Rain Dish | 8 | | Water Gun, Absorb, Growl"],
        'Kaizo': ["Lotad @ Oran Berry | Modest | Swift Swim | 11 | 252 SpA / 252 Spe | Water Gun, Mega Drain, Growl, Mist",
                  "Seedot @ Oran Berry | Adamant | Chlorophyll | 11 | 252 Atk / 252 HP | Bullet Seed, Astonish, Harden, Growth"],
    },
    'IVAN': {
        'Hard': ["Magikarp | Jolly | Swift Swim | 8 | | Splash, Tackle",
                 "Goldeen | Adamant | Swift Swim | 8 | | Peck, Tail Whip, Water Sport",
                 "Tentacool | Calm | Clear Body | 8 | | Poison Sting, Supersonic, Constrict"],
        'Kaizo': ["Tentacool @ Oran Berry | Calm | Clear Body | 11 | 252 HP / 252 SpD | Acid, Water Gun, Supersonic, Wrap",
                  "Goldeen | Adamant | Swift Swim | 11 | 252 Atk / 252 Spe | Peck, Horn Attack, Water Pulse, Supersonic",
                  "Barboach | Adamant | Oblivious | 11 | 252 Atk / 252 HP | Mud-Slap, Water Gun, Mud Sport"],
    },
    'DARIAN': {
        'Hard': ["Magikarp | Jolly | Swift Swim | 10 | | Splash, Tackle",
                 "Horsea | Modest | Swift Swim | 10 | | Bubble, Smokescreen, Leer"],
        'Kaizo': ["Horsea @ Oran Berry | Modest | Sniper | 12 | 252 SpA / 252 Spe | Water Gun, Smokescreen, Leer, Twister",
                  "Psyduck | Modest | Damp | 12 | 252 SpA / 252 Spe | Water Gun, Confusion, Tail Whip, Disable",
                  "Magikarp | Jolly | Swift Swim | 12 | | Splash, Tackle, Flail"],
    },
    # Arène de Mérouville
    'JOSH': {
        'Hard': ["Geodude | Impish | Sturdy | 12 | | Rock Throw, Defense Curl, Magnitude",
                 "Nosepass | Impish | Sturdy | 12 | | Rock Throw, Harden, Block"],
        'Kaizo': ["Geodude @ Oran Berry | Adamant | Sturdy | 14 | 252 Atk / 252 HP | Rock Tomb, Magnitude, Defense Curl, Rollout",
                  "Onix | Impish | Sturdy | 14 | 252 HP / 252 Def | Rock Tomb, Bind, Harden, Smack Down",
                  "Larvitar | Adamant | Guts | 13 | 252 Atk / 252 Spe | Rock Slide, Bite, Leer, Sandstorm"],
    },
    'TOMMY': {
        'Hard': ["Geodude | Impish | Sturdy | 11 | | Rock Throw, Defense Curl, Tackle",
                 "Onix | Impish | Sturdy | 11 | | Rock Throw, Bind, Harden"],
        'Kaizo': ["Onix @ Oran Berry | Impish | Sturdy | 14 | 252 HP / 252 Def | Rock Tomb, Bind, Harden, Smack Down",
                  "Roggenrola | Adamant | Sturdy | 14 | 252 Atk / 252 HP | Rock Blast, Headbutt, Sand Attack, Iron Defense",
                  "Geodude | Adamant | Sturdy | 13 | 252 Atk / 252 HP | Rock Tomb, Magnitude, Defense Curl, Rollout"],
    },
    'MARC': {
        'Hard': ["Geodude | Impish | Sturdy | 11 | | Rock Throw, Magnitude, Defense Curl",
                 "Sandshrew | Adamant | Sand Veil | 11 | | Rollout, Defense Curl, Sand Attack"],
        'Kaizo': ["Rhyhorn @ Oran Berry | Adamant | Rock Head | 15 | 252 Atk / 252 HP | Rock Blast, Horn Attack, Stomp, Bulldoze",
                  "Sandshrew | Adamant | Sand Veil | 14 | 252 Atk / 252 Def | Rollout, Defense Curl, Rapid Spin, Bulldoze",
                  "Geodude | Adamant | Sturdy | 14 | 252 Atk / 252 HP | Rock Tomb, Magnitude, Defense Curl, Rollout"],
    },
    'ROXANNE_1': {
        'Hard': ["Geodude @ Oran Berry | Adamant | Sturdy | 14 | 128 Atk / 128 HP | Rock Tomb, Magnitude, Defense Curl, Rollout",
                 "Onix | Impish | Sturdy | 14 | 128 HP / 128 Def | Rock Tomb, Bind, Harden, Smack Down",
                 "Rhyhorn | Adamant | Rock Head | 15 | 128 Atk / 128 HP | Rock Blast, Horn Attack, Bulldoze, Scary Face",
                 "Nosepass @ Sitrus Berry | Relaxed | Sturdy | 16 | 252 HP / 252 Def | Rock Tomb, Thunder Wave, Block, Shock Wave"],
        'Kaizo': ["Larvitar @ Oran Berry | Adamant | Guts | 16 | 252 Atk / 252 Spe | Rock Slide, Bite, Screech, Sandstorm",
                  "Aron @ Oran Berry | Adamant | Sturdy | 16 | 252 Atk / 252 HP | Headbutt, Rock Tomb, Metal Claw, Iron Defense",
                  "Kabuto @ Eviolite | Adamant | Swift Swim | 16 | 252 Atk / 252 Def | Aqua Jet, Rock Tomb, Mud Shot, Harden",
                  "Rhyhorn @ Eviolite | Adamant | Rock Head | 17 | 252 Atk / 252 HP | Rock Blast, Bulldoze, Horn Attack, Scary Face",
                  "Onix | Jolly | Sturdy | 16 | 252 Atk / 252 Spe | Stealth Rock, Rock Tomb, Dragon Breath, Bind",
                  "Nosepass @ Sitrus Berry | Relaxed | Sturdy | 18 | 252 HP / 252 Def | Rock Tomb, Thunder Wave, Shock Wave, Block"],
    },
    # Route 116
    'JOEY': {
        'Hard': ["Machop | Adamant | Guts | 11 | | Karate Chop, Low Kick, Leer"],
        'Kaizo': ["Machop @ Oran Berry | Adamant | Guts | 13 | 252 Atk / 252 HP | Karate Chop, Low Kick, Focus Energy, Leer",
                  "Makuhita | Adamant | Thick Fat | 13 | 252 Atk / 252 HP | Arm Thrust, Fake Out, Sand Attack, Focus Energy"],
    },
    'JOSE': {
        'Hard': ["Nincada | Adamant | Compound Eyes | 10 | | Leech Life, Sand Attack, Harden",
                 "Wurmple | Adamant | Shield Dust | 10 | | Bug Bite, Poison Sting, String Shot"],
        'Kaizo': ["Venonat @ Oran Berry | Calm | Compound Eyes | 13 | 252 HP / 252 SpD | Confusion, Poison Powder, Supersonic, Disable",
                  "Nincada | Adamant | Compound Eyes | 12 | 252 Atk / 252 HP | Leech Life, Sand Attack, Fury Swipes, Harden",
                  "Skorupi | Adamant | Battle Armor | 12 | 252 Atk / 252 HP | Bug Bite, Poison Fang, Leer, Knock Off"],
    },
    'CLARK': {
        'Hard': ["Geodude | Impish | Sturdy | 10 | | Rock Throw, Magnitude, Defense Curl",
                 "Machop | Adamant | Guts | 10 | | Karate Chop, Low Kick"],
        'Kaizo': ["Geodude @ Oran Berry | Adamant | Sturdy | 13 | 252 Atk / 252 HP | Rock Throw, Magnitude, Defense Curl, Rollout",
                  "Phanpy | Adamant | Pickup | 13 | 252 Atk / 252 HP | Rollout, Defense Curl, Flail, Bulldoze"],
    },
    'JANICE': {
        'Hard': ["Marill | Adamant | Huge Power | 10 | | Bubble, Tail Whip, Defense Curl"],
        'Kaizo': ["Marill @ Oran Berry | Adamant | Huge Power | 13 | 252 Atk / 252 HP | Bubble, Rollout, Defense Curl, Water Gun",
                  "Clefairy | Bold | Magic Guard | 13 | 252 HP / 252 Def | Disarming Voice, Sing, Defense Curl, Pound"],
    },
    'KAREN_1': {
        'Hard': ["Shroomish | Bold | Effect Spore | 10 | | Absorb, Stun Spore, Leech Seed",
                 "Oddish | Modest | Chlorophyll | 10 | | Absorb, Acid, Sweet Scent"],
        'Kaizo': ["Oddish @ Oran Berry | Modest | Chlorophyll | 13 | 252 SpA / 252 HP | Mega Drain, Acid, Stun Spore, Sweet Scent",
                  "Shroomish | Bold | Effect Spore | 13 | 252 HP / 252 Def | Leech Seed, Stun Spore, Mega Drain, Headbutt",
                  "Bellsprout | Adamant | Chlorophyll | 12 | 252 Atk / 252 Spe | Vine Whip, Wrap, Poison Powder, Growth"],
    },
    'JOHNSON': {
        'Hard': ["Shroomish | Bold | Effect Spore | 10 | | Absorb, Stun Spore",
                 "Lotad | Calm | Rain Dish | 10 | | Water Gun, Absorb"],
        'Kaizo': ["Lotad @ Oran Berry | Calm | Swift Swim | 13 | 252 HP / 252 SpD | Mega Drain, Water Gun, Mist, Growl",
                  "Shroomish | Bold | Effect Spore | 13 | 252 HP / 252 Def | Leech Seed, Stun Spore, Mega Drain, Headbutt",
                  "Wooper | Impish | Damp | 12 | 252 HP / 252 Def | Mud Shot, Water Gun, Tail Whip, Slam"],
    },
    'DAWSON': {
        'Hard': ["Zigzagoon | Jolly | Pickup | 10 | | Headbutt, Tail Whip, Sand Attack",
                 "Poochyena | Jolly | Quick Feet | 10 | | Bite, Howl, Sand Attack"],
        'Kaizo': ["Growlithe @ Oran Berry | Adamant | Intimidate | 13 | 252 Atk / 252 Spe | Ember, Bite, Leer, Howl",
                  "Poochyena | Jolly | Quick Feet | 13 | 252 Atk / 252 Spe | Bite, Howl, Sand Attack, Tackle",
                  "Meowth | Jolly | Technician | 12 | 252 Atk / 252 Spe | Fake Out, Fury Swipes, Bite"],
    },
    'SARAH': {
        'Hard': ["Lotad | Calm | Rain Dish | 10 | | Water Gun, Absorb",
                 "Zigzagoon | Jolly | Pickup | 10 | | Headbutt, Tail Whip"],
        'Kaizo': ["Lotad @ Oran Berry | Modest | Swift Swim | 13 | 252 SpA / 252 Spe | Water Gun, Mega Drain, Growl, Mist",
                  "Skitty | Jolly | Normalize | 13 | 252 Atk / 252 Spe | Fake Out, Double Kick, Sing, Tail Whip",
                  "Azurill | Adamant | Huge Power | 12 | 252 Atk / 252 HP | Bubble, Slam, Tail Whip, Charm"],
    },
    'JERRY_1': {
        'Hard': ["Ralts | Modest | Synchronize | 11 | | Confusion, Growl, Disarming Voice"],
        'Kaizo': ["Ralts @ Oran Berry | Modest | Trace | 13 | 252 SpA / 252 Spe | Confusion, Disarming Voice, Double Team, Teleport",
                  "Abra | Timid | Synchronize | 13 | 252 SpA / 252 Spe | Teleport, Confusion",
                  "Spoink | Modest | Thick Fat | 13 | 252 SpA / 252 HP | Psybeam, Splash, Odor Sleuth"],
    },
    'GRUNT_RUSTURF_TUNNEL': {
        'Hard': ["Poochyena | Jolly | Quick Feet | 15 | | Bite, Howl, Sand Attack, Roar",
                 "Carvanha | Adamant | Rough Skin | 15 | | Bite, Aqua Jet, Leer, Focus Energy",
                 "Zubat | Jolly | Inner Focus | 14 | | Bite, Supersonic, Astonish, Wing Attack"],
        'Kaizo': ["Mightyena @ Oran Berry | Jolly | Intimidate | 19 | 252 Atk / 252 Spe | Bite, Roar, Swagger, Sand Attack",
                  "Carvanha @ Oran Berry | Adamant | Speed Boost | 19 | 252 Atk / 252 Spe | Bite, Aqua Jet, Ice Fang, Focus Energy",
                  "Golbat | Jolly | Inner Focus | 19 | 252 Atk / 252 Spe | Bite, Wing Attack, Confuse Ray, Mean Look",
                  "Stunky | Jolly | Aftermath | 18 | 252 Atk / 252 Spe | Feint, Fury Swipes, Poison Gas, Screech"],
    },
}

# Rivaux : la lignée dépend du starter du joueur (suffixe de l'ID = starter du joueur).
RIVAL_LINES = {  # starter du joueur -> (base, milieu) de la lignée du rival
    'TREECKO': ('Cyndaquil', 'Quilava', 'Blaze', 'Flash Fire', 'Ember, Quick Attack, Tackle, Flame Charge', 'Flame Wheel, Quick Attack, Smokescreen, Swift'),
    'TORCHIC': ('Squirtle', 'Wartortle', 'Torrent', 'Rain Dish', 'Water Gun, Tackle, Bubble, Rapid Spin', 'Water Pulse, Bite, Rapid Spin, Protect'),
    'MUDKIP':  ('Treecko', 'Grovyle', 'Overgrow', 'Unburden', 'Absorb, Quick Attack, Pound, Bullet Seed', 'Mega Drain, Quick Attack, Pursuit, Fury Cutter'),
}
for rival in ('MAY', 'BRENDAN'):
    for starter, (base, mid, ab, ha, m1, m2) in RIVAL_LINES.items():
        TEAMS[f'{rival}_ROUTE_103_{starter}'] = {
            'Hard': [f"{base} | Hardy | {ab} | 6 | | {m1}"],
            'Kaizo': [f"{base} @ Oran Berry | Timid | {ha} | 7 | 252 Spe / 252 HP | {m1}",
                      "Zigzagoon | Jolly | Pickup | 6 | 252 Atk / 252 Spe | Headbutt, Tackle, Covet"],
        }
        TEAMS[f'{rival}_RUSTBORO_{starter}'] = {
            'Hard': ["Taillow | Jolly | Guts | 16 | | Wing Attack, Quick Attack, Double Team, Focus Energy",
                     "Lotad | Calm | Rain Dish | 16 | | Mega Drain, Water Gun, Mist, Growl",
                     f"{mid} @ Oran Berry | Hardy | {ab} | 18 | 128 HP / 128 Spe | {m2}"],
            'Kaizo': ["Swellow @ Oran Berry | Jolly | Guts | 19 | 252 Atk / 252 Spe | Wing Attack, Quick Attack, Double Team, Endeavor",
                      "Lombre | Modest | Swift Swim | 19 | 252 SpA / 252 Spe | Mega Drain, Water Pulse, Fake Out, Rain Dance",
                      "Riolu | Jolly | Inner Focus | 19 | 252 Atk / 252 Spe | Force Palm, Quick Attack, Counter, Feint",
                      f"{mid} @ Sitrus Berry | Timid | {ha} | 21 | 252 Spe / 252 HP | {m2}"],
        }


# ------------------------------------------------------------------ chapitre 2 : chefs (faits à la main)
# Caps : 27 avant Bastien (2e badge), 36 avant Voltère (3e badge).
TEAMS.update({
    'BRAWLY_1': {
        'Hard': ["Machop @ Oran Berry | Adamant | Guts | 21 | 128 Atk / 128 HP | Karate Chop, Bulk Up, Rock Tomb, Knock Off",
                 "Meditite | Jolly | Pure Power | 21 | 128 Atk / 128 Spe | Force Palm, Confusion, Detect, Bulk Up",
                 "Mankey | Jolly | Vital Spirit | 21 | 128 Atk / 128 Spe | Low Kick, Fury Swipes, Focus Energy, Seismic Toss",
                 "Croagunk | Adamant | Dry Skin | 22 | 128 Atk / 128 HP | Revenge, Poison Jab, Sucker Punch, Swagger",
                 "Makuhita @ Sitrus Berry | Adamant | Thick Fat | 24 | 252 Atk / 252 HP | Arm Thrust, Fake Out, Bulk Up, Reversal"],
        'Kaizo': ["Riolu @ Oran Berry | Jolly | Inner Focus | 24 | 252 Atk / 252 Spe | Force Palm, Quick Attack, Copycat, Feint",
                  "Meditite @ Oran Berry | Jolly | Pure Power | 24 | 252 Atk / 252 Spe | Hi Jump Kick, Zen Headbutt, Detect, Bulk Up",
                  "Timburr @ Eviolite | Adamant | Guts | 24 | 252 Atk / 252 HP | Low Kick, Rock Throw, Bulk Up, Wake-Up Slap",
                  "Croagunk @ Black Sludge | Adamant | Dry Skin | 25 | 252 Atk / 252 Spe | Revenge, Poison Jab, Sucker Punch, Nasty Plot",
                  "Machoke @ Eviolite | Adamant | Guts | 26 | 252 Atk / 252 HP | Dual Chop, Bulk Up, Rock Tomb, Knock Off",
                  "Makuhita @ Sitrus Berry | Adamant | Thick Fat | 27 | 252 Atk / 252 HP | Arm Thrust, Fake Out, Bulk Up, Reversal"],
    },
    'WATTSON_1': {
        'Hard': ["Voltorb | Timid | Static | 28 | 128 SpA / 128 Spe | Spark, Rollout, Light Screen, Self-Destruct",
                 "Electrike | Timid | Lightning Rod | 28 | 128 SpA / 128 Spe | Shock Wave, Bite, Howl, Quick Attack",
                 "Magneton | Modest | Sturdy | 30 | 128 SpA / 128 HP | Spark, Mirror Shot, Thunder Wave, Sonic Boom",
                 "Shinx | Adamant | Intimidate | 29 | 128 Atk / 128 Spe | Spark, Bite, Charge, Roar",
                 "Manectric @ Sitrus Berry | Timid | Static | 32 | 252 SpA / 252 Spe | Shock Wave, Bite, Thunder Wave, Quick Attack"],
        'Kaizo': ["Jolteon @ Magnet | Timid | Volt Absorb | 32 | 252 SpA / 252 Spe | Thunderbolt, Double Kick, Thunder Wave, Pin Missile",
                  "Magneton @ Eviolite | Modest | Sturdy | 33 | 252 SpA / 252 HP | Thunderbolt, Flash Cannon, Thunder Wave, Tri Attack",
                  "Ampharos @ Sitrus Berry | Modest | Static | 33 | 252 SpA / 252 HP | Thunderbolt, Power Gem, Thunder Wave, Light Screen",
                  "Luxio @ Eviolite | Adamant | Intimidate | 33 | 252 Atk / 252 Spe | Spark, Crunch, Ice Fang, Swagger",
                  "Electrode @ Light Clay | Timid | Soundproof | 33 | 252 SpA / 252 Spe | Light Screen, Reflect, Thunderbolt, Explosion",
                  "Manectric @ Sitrus Berry | Timid | Lightning Rod | 35 | 252 SpA / 252 Spe | Thunderbolt, Flamethrower, Thunder Wave, Quick Attack"],
    },
    'WALLY_MAUVILLE': {
        'Hard': ["Ralts | Modest | Synchronize | 19 | | Confusion, Disarming Voice, Double Team, Teleport"],
        'Kaizo': ["Kirlia @ Oran Berry | Timid | Trace | 22 | 252 SpA / 252 Spe | Confusion, Draining Kiss, Double Team, Calm Mind"],
    },
    'GRUNT_MUSEUM_1': {
        'Hard': ["Carvanha | Adamant | Rough Skin | 18 | | Bite, Aqua Jet, Leer, Focus Energy",
                 "Poochyena | Jolly | Quick Feet | 18 | | Bite, Howl, Sand Attack, Roar"],
        'Kaizo': ["Sharpedo @ Oran Berry | Adamant | Rough Skin | 22 | 252 Atk / 252 Spe | Bite, Aqua Jet, Ice Fang, Focus Energy",
                  "Mightyena | Jolly | Intimidate | 22 | 252 Atk / 252 Spe | Bite, Swagger, Sand Attack, Roar",
                  "Golbat | Jolly | Inner Focus | 21 | 252 Atk / 252 Spe | Bite, Wing Attack, Confuse Ray, Mean Look"],
    },
    'GRUNT_MUSEUM_2': {
        'Hard': ["Zubat | Jolly | Inner Focus | 17 | | Bite, Supersonic, Astonish, Wing Attack",
                 "Carvanha | Adamant | Rough Skin | 18 | | Bite, Aqua Jet, Leer, Focus Energy",
                 "Stunky | Jolly | Stench | 17 | | Fury Swipes, Poison Gas, Feint, Screech"],
        'Kaizo': ["Golbat @ Oran Berry | Jolly | Inner Focus | 22 | 252 Atk / 252 Spe | Bite, Wing Attack, Confuse Ray, Mean Look",
                  "Carvanha @ Oran Berry | Adamant | Speed Boost | 22 | 252 Atk / 252 Spe | Bite, Aqua Jet, Ice Fang, Focus Energy",
                  "Skuntank | Jolly | Aftermath | 22 | 252 Atk / 252 Spe | Night Slash, Poison Fang, Screech, Feint"],
    },
})

# Rival sur la Route 110 (cap 36) : la lignée suit le starter du joueur.
RIVAL_110 = {  # starter du joueur -> (milieu de lignée, capacités Hard, capacités Kaizo)
    'TREECKO': ('Quilava', 'Blaze', 'Flash Fire', 'Flame Wheel, Quick Attack, Smokescreen, Swift', 'Flamethrower, Extrasensory, Quick Attack, Smokescreen'),
    'TORCHIC': ('Wartortle', 'Torrent', 'Rain Dish', 'Water Pulse, Bite, Rapid Spin, Protect', 'Aqua Tail, Bite, Rapid Spin, Ice Beam'),
    'MUDKIP':  ('Grovyle', 'Overgrow', 'Unburden', 'Leaf Blade, Quick Attack, Pursuit, Fury Cutter', 'Leaf Blade, X-Scissor, Quick Attack, Agility'),
}
for rival in ('MAY', 'BRENDAN'):
    for starter, (mid, ab, ha, mh, mk) in RIVAL_110.items():
        TEAMS[f'{rival}_ROUTE_110_{starter}'] = {
            'Hard': ["Swellow | Jolly | Guts | 22 | 128 Atk / 128 Spe | Wing Attack, Quick Attack, Double Team, Endeavor",
                     "Lombre | Modest | Swift Swim | 22 | 128 SpA / 128 Spe | Mega Drain, Water Pulse, Fake Out, Nature Power",
                     "Numel | Modest | Oblivious | 22 | 128 SpA / 128 HP | Ember, Magnitude, Focus Energy, Take Down",
                     f"{mid} @ Oran Berry | Hardy | {ab} | 24 | 128 HP / 128 Spe | {mh}"],
            'Kaizo': ["Swellow @ Oran Berry | Jolly | Guts | 26 | 252 Atk / 252 Spe | Aerial Ace, Quick Attack, Double Team, Endeavor",
                      "Lombre @ Oran Berry | Modest | Swift Swim | 26 | 252 SpA / 252 Spe | Giga Drain, Water Pulse, Fake Out, Rain Dance",
                      "Lucario @ Oran Berry | Jolly | Inner Focus | 26 | 252 Atk / 252 Spe | Force Palm, Metal Claw, Quick Attack, Swords Dance",
                      "Camerupt | Modest | Solid Rock | 27 | 252 SpA / 252 HP | Flamethrower, Earth Power, Rock Slide, Amnesia",
                      f"{mid} @ Sitrus Berry | Timid | {ha} | 28 | 252 Spe / 252 HP | {mk}"],
        }

# ------------------------------------------------------------------ chapitre 3 : chefs (faits à la main)
# Cap 45 avant Adriane (4e badge). Mont Chimnée et Chemin Escarpé juste avant l'Arène.
TEAMS.update({
    'GRUNT_MT_CHIMNEY_1': {
        'Hard': ["Mightyena | Jolly | Intimidate | 32 | 128 Atk / 128 Spe | Crunch, Fire Fang, Swagger, Sand Attack",
                 "Camerupt | Modest | Solid Rock | 33 | 128 SpA / 128 HP | Lava Plume, Earth Power, Rock Slide, Growth"],
        'Kaizo': ["Golbat @ Oran Berry | Jolly | Inner Focus | 36 | 252 Atk / 252 Spe | Poison Fang, Wing Attack, Confuse Ray, Mean Look",
                  "Mightyena @ Oran Berry | Jolly | Intimidate | 36 | 252 Atk / 252 Spe | Crunch, Fire Fang, Sucker Punch, Swagger",
                  "Camerupt @ Sitrus Berry | Modest | Solid Rock | 37 | 252 SpA / 252 HP | Lava Plume, Earth Power, Rock Slide, Yawn"],
    },
    'GRUNT_MT_CHIMNEY_2': {
        'Hard': ["Golbat | Jolly | Inner Focus | 33 | 128 Atk / 128 Spe | Poison Fang, Wing Attack, Bite, Confuse Ray",
                 "Weezing | Bold | Levitate | 33 | 128 HP / 128 Def | Sludge, Will-O-Wisp, Smokescreen, Self-Destruct"],
        'Kaizo': ["Golbat @ Oran Berry | Jolly | Inner Focus | 37 | 252 Atk / 252 Spe | Poison Fang, Wing Attack, Confuse Ray, Mean Look",
                  "Weezing @ Black Sludge | Bold | Levitate | 36 | 252 HP / 252 Def | Sludge Bomb, Will-O-Wisp, Fire Blast, Pain Split",
                  "Houndoom @ Oran Berry | Timid | Flash Fire | 36 | 252 SpA / 252 Spe | Flamethrower, Dark Pulse, Roar, Nasty Plot"],
    },
    'TABITHA_MT_CHIMNEY': {
        'Hard': ["Mightyena | Jolly | Intimidate | 34 | 128 Atk / 128 Spe | Crunch, Fire Fang, Swagger, Roar",
                 "Golbat | Jolly | Inner Focus | 34 | 128 Atk / 128 Spe | Poison Fang, Wing Attack, Confuse Ray, Mean Look",
                 "Camerupt | Modest | Solid Rock | 35 | 128 SpA / 128 HP | Lava Plume, Earth Power, Rock Slide, Yawn",
                 "Houndoom @ Oran Berry | Timid | Flash Fire | 36 | 252 SpA / 252 Spe | Flamethrower, Crunch, Roar, Nasty Plot"],
        'Kaizo': ["Mightyena @ Sitrus Berry | Jolly | Intimidate | 38 | 252 Atk / 252 Spe | Crunch, Fire Fang, Sucker Punch, Swagger",
                  "Crobat @ Oran Berry | Jolly | Inner Focus | 38 | 252 Atk / 252 Spe | Cross Poison, Acrobatics, Super Fang, Taunt",
                  "Weezing @ Black Sludge | Bold | Levitate | 38 | 252 HP / 252 Def | Sludge Bomb, Will-O-Wisp, Fire Blast, Pain Split",
                  "Camerupt @ Sitrus Berry | Modest | Solid Rock | 39 | 252 SpA / 252 HP | Lava Plume, Earth Power, Rock Slide, Yawn",
                  "Houndoom @ Focus Sash | Timid | Flash Fire | 40 | 252 SpA / 252 Spe | Flamethrower, Dark Pulse, Sludge Bomb, Nasty Plot"],
    },
    'MAXIE_MT_CHIMNEY': {
        'Hard': ["Mightyena | Jolly | Intimidate | 36 | 128 Atk / 128 Spe | Crunch, Fire Fang, Sucker Punch, Swagger",
                 "Golbat | Jolly | Inner Focus | 36 | 128 Atk / 128 Spe | Poison Fang, Wing Attack, Confuse Ray, Mean Look",
                 "Weezing | Bold | Levitate | 36 | 128 HP / 128 Def | Sludge Bomb, Will-O-Wisp, Smokescreen, Pain Split",
                 "Camerupt @ Sitrus Berry | Modest | Solid Rock | 38 | 252 SpA / 252 HP | Lava Plume, Earth Power, Rock Slide, Yawn"],
        'Kaizo': ["Mightyena @ Sitrus Berry | Jolly | Moxie | 40 | 252 Atk / 252 Spe | Crunch, Fire Fang, Sucker Punch, Play Rough",
                  "Crobat @ Oran Berry | Jolly | Inner Focus | 40 | 252 Atk / 252 Spe | Cross Poison, Acrobatics, Super Fang, Taunt",
                  "Weezing @ Black Sludge | Bold | Levitate | 40 | 252 HP / 252 Def | Sludge Bomb, Will-O-Wisp, Fire Blast, Pain Split",
                  "Torkoal @ Heat Rock | Relaxed | Drought | 40 | 252 HP / 252 Def | Lava Plume, Stealth Rock, Yawn, Body Press",
                  "Houndoom @ Focus Sash | Timid | Flash Fire | 41 | 252 SpA / 252 Spe | Fire Blast, Dark Pulse, Solar Beam, Nasty Plot",
                  "Camerupt @ Sitrus Berry | Modest | Solid Rock | 42 | 252 SpA / 252 HP | Flamethrower, Earth Power, Rock Slide, Yawn"],
    },
    'GRUNT_JAGGED_PASS': {
        'Hard': ["Mightyena | Jolly | Intimidate | 34 | 128 Atk / 128 Spe | Crunch, Fire Fang, Swagger, Sand Attack",
                 "Camerupt | Modest | Solid Rock | 34 | 128 SpA / 128 HP | Lava Plume, Earth Power, Rock Slide, Growth"],
        'Kaizo': ["Mightyena @ Oran Berry | Jolly | Intimidate | 38 | 252 Atk / 252 Spe | Crunch, Fire Fang, Sucker Punch, Swagger",
                  "Torkoal @ Sitrus Berry | Relaxed | Drought | 37 | 252 HP / 252 Def | Lava Plume, Yawn, Rock Slide, Protect",
                  "Camerupt @ Oran Berry | Modest | Solid Rock | 38 | 252 SpA / 252 HP | Lava Plume, Earth Power, Rock Slide, Yawn"],
    },
    'FLANNERY_1': {
        'Hard': ["Magcargo | Bold | Flame Body | 40 | 128 HP / 128 Def | Lava Plume, Rock Slide, Yawn, Amnesia",
                 "Ninetales | Timid | Flash Fire | 40 | 128 SpA / 128 Spe | Flamethrower, Will-O-Wisp, Confuse Ray, Extrasensory",
                 "Houndoom | Timid | Flash Fire | 40 | 128 SpA / 128 Spe | Flamethrower, Crunch, Nasty Plot, Sludge Bomb",
                 "Camerupt | Modest | Solid Rock | 41 | 128 SpA / 128 HP | Lava Plume, Earth Power, Rock Slide, Yawn",
                 "Torkoal @ Sitrus Berry | Quiet | Drought | 42 | 252 HP / 252 SpA | Overheat, Rock Slide, Yawn, Protect"],
        'Kaizo': ["Torkoal @ Heat Rock | Relaxed | Drought | 44 | 252 HP / 252 Def | Lava Plume, Stealth Rock, Yawn, Rapid Spin",
                  "Ninetales @ Life Orb | Timid | Flash Fire | 44 | 252 SpA / 252 Spe | Fire Blast, Solar Beam, Nasty Plot, Extrasensory",
                  "Houndoom @ Focus Sash | Timid | Flash Fire | 44 | 252 SpA / 252 Spe | Flamethrower, Dark Pulse, Sludge Bomb, Nasty Plot",
                  "Arcanine @ Sitrus Berry | Adamant | Intimidate | 44 | 252 Atk / 252 Spe | Flare Blitz, Extreme Speed, Wild Charge, Crunch",
                  "Magcargo @ White Herb | Modest | Flame Body | 44 | 252 SpA / 252 HP | Shell Smash, Lava Plume, Earth Power, Ancient Power",
                  "Camerupt @ Sitrus Berry | Quiet | Solid Rock | 45 | 252 SpA / 252 HP | Eruption, Earth Power, Rock Slide, Yawn"],
    },
})


# ------------------------------------------------------------------ chapitre 4 : Norman (cap 54)
TEAMS.update({
    'NORMAN_1': {
        'Hard': ["Spinda | Jolly | Own Tempo | 48 | 128 Atk / 128 Spe | Teeter Dance, Sucker Punch, Zen Headbutt, Double-Edge",
                 "Vigoroth | Jolly | Vital Spirit | 48 | 128 Atk / 128 Spe | Slash, Bulk Up, Shadow Claw, Encore",
                 "Linoone | Jolly | Gluttony | 48 | 128 Atk / 128 Spe | Body Slam, Shadow Claw, Seed Bomb, Rest",
                 "Ursaring | Adamant | Guts | 49 | 128 Atk / 128 HP | Hammer Arm, Crunch, Slash, Scary Face",
                 "Slaking @ Sitrus Berry | Adamant | Truant | 50 | 252 Atk / 252 HP | Giga Impact, Shadow Claw, Earthquake, Yawn"],
        'Kaizo': ["Kangaskhan @ Silk Scarf | Adamant | Scrappy | 52 | 252 Atk / 252 Spe | Fake Out, Double-Edge, Sucker Punch, Earthquake",
                  "Linoone @ Sitrus Berry | Jolly | Gluttony | 52 | 252 Atk / 252 Spe | Belly Drum, Extreme Speed, Shadow Claw, Seed Bomb",
                  "Ursaring @ Flame Orb | Jolly | Guts | 52 | 252 Atk / 252 Spe | Facade, Close Combat, Crunch, Swords Dance",
                  "Spinda @ Focus Sash | Jolly | Contrary | 52 | 252 Atk / 252 Spe | Superpower, Sucker Punch, Teeter Dance, Fake Out",
                  "Vigoroth @ Eviolite | Jolly | Vital Spirit | 53 | 252 HP / 252 Def | Bulk Up, Slack Off, Knock Off, Body Slam",
                  "Slaking @ Life Orb | Adamant | Truant | 54 | 252 Atk / 252 Spe | Double-Edge, Earthquake, Knock Off, Hammer Arm"],
    },
})


# ------------------------------------------------------------------ chapitre 5 : Institut Météo, rival Route 119, Alizée (cap 63)
TEAMS.update({
    'GRUNT_WEATHER_INST_1': {
        'Hard': ["Golbat | Jolly | Inner Focus | 52 | 128 Atk / 128 Spe | Poison Fang, Wing Attack, Bite, Confuse Ray",
                 "Sharpedo | Adamant | Rough Skin | 52 | 128 Atk / 128 Spe | Crunch, Aqua Jet, Poison Fang, Ice Fang"],
        'Kaizo': ["Crobat @ Oran Berry | Jolly | Inner Focus | 56 | 252 Atk / 252 Spe | Cross Poison, Acrobatics, Super Fang, Taunt",
                  "Sharpedo @ Focus Sash | Adamant | Speed Boost | 56 | 252 Atk / 252 Spe | Crunch, Waterfall, Ice Fang, Protect",
                  "Mightyena @ Sitrus Berry | Jolly | Intimidate | 56 | 252 Atk / 252 Spe | Crunch, Play Rough, Sucker Punch, Fire Fang"],
    },
    'GRUNT_WEATHER_INST_2': {
        'Hard': ["Mightyena | Jolly | Intimidate | 52 | 128 Atk / 128 Spe | Crunch, Fire Fang, Sucker Punch, Swagger",
                 "Pelipper | Modest | Drizzle | 52 | 128 SpA / 128 HP | Scald, Hurricane, Roost, Protect"],
        'Kaizo': ["Pelipper @ Damp Rock | Modest | Drizzle | 56 | 252 SpA / 252 HP | Scald, Hurricane, U-turn, Roost",
                  "Mightyena @ Sitrus Berry | Jolly | Intimidate | 56 | 252 Atk / 252 Spe | Crunch, Play Rough, Sucker Punch, Fire Fang",
                  "Ludicolo @ Life Orb | Modest | Swift Swim | 57 | 252 SpA / 252 Spe | Hydro Pump, Giga Drain, Ice Beam, Fake Out"],
    },
    'GRUNT_WEATHER_INST_3': {
        'Hard': ["Mightyena | Jolly | Intimidate | 51 | 128 Atk / 128 Spe | Crunch, Ice Fang, Sucker Punch, Swagger",
                 "Golbat | Jolly | Inner Focus | 52 | 128 Atk / 128 Spe | Poison Fang, Wing Attack, Bite, Confuse Ray"],
        'Kaizo': ["Mightyena @ Oran Berry | Jolly | Moxie | 56 | 252 Atk / 252 Spe | Crunch, Play Rough, Sucker Punch, Ice Fang",
                  "Crobat @ Sitrus Berry | Jolly | Inner Focus | 56 | 252 Atk / 252 Spe | Cross Poison, Acrobatics, Super Fang, U-turn",
                  "Whiscash @ Sitrus Berry | Adamant | Oblivious | 56 | 252 Atk / 252 HP | Earthquake, Waterfall, Stone Edge, Dragon Dance"],
    },
    'GRUNT_WEATHER_INST_4': {
        'Hard': ["Sharpedo | Adamant | Rough Skin | 53 | 128 Atk / 128 Spe | Crunch, Aqua Jet, Poison Fang, Ice Fang",
                 "Mightyena | Jolly | Intimidate | 52 | 128 Atk / 128 Spe | Crunch, Fire Fang, Sucker Punch, Swagger"],
        'Kaizo': ["Sharpedo @ Life Orb | Adamant | Speed Boost | 57 | 252 Atk / 252 Spe | Crunch, Waterfall, Close Combat, Protect",
                  "Tentacruel @ Black Sludge | Calm | Clear Body | 56 | 252 HP / 252 SpD | Scald, Sludge Bomb, Toxic Spikes, Rapid Spin",
                  "Crobat @ Oran Berry | Jolly | Inner Focus | 56 | 252 Atk / 252 Spe | Cross Poison, Acrobatics, Super Fang, Taunt"],
    },
    'GRUNT_WEATHER_INST_5': {
        'Hard': ["Golbat | Jolly | Inner Focus | 52 | 128 Atk / 128 Spe | Poison Fang, Wing Attack, Bite, Confuse Ray",
                 "Tentacruel | Calm | Clear Body | 52 | 128 HP / 128 SpD | Surf, Sludge Bomb, Toxic Spikes, Hex"],
        'Kaizo': ["Crobat @ Oran Berry | Jolly | Inner Focus | 56 | 252 Atk / 252 Spe | Cross Poison, Acrobatics, Super Fang, Taunt",
                  "Tentacruel @ Black Sludge | Timid | Clear Body | 56 | 252 SpA / 252 Spe | Surf, Sludge Bomb, Ice Beam, Toxic Spikes",
                  "Sharpedo @ Focus Sash | Adamant | Speed Boost | 56 | 252 Atk / 252 Spe | Crunch, Waterfall, Ice Fang, Protect"],
    },
    'SHELLY_WEATHER_INSTITUTE': {
        'Hard': ["Mightyena | Jolly | Intimidate | 54 | 128 Atk / 128 Spe | Crunch, Ice Fang, Sucker Punch, Play Rough",
                 "Pelipper @ Damp Rock | Modest | Drizzle | 54 | 128 SpA / 128 HP | Scald, Hurricane, Roost, Protect",
                 "Crobat | Jolly | Inner Focus | 55 | 128 Atk / 128 Spe | Cross Poison, Acrobatics, Super Fang, Confuse Ray",
                 "Sharpedo @ Sitrus Berry | Adamant | Rough Skin | 56 | 252 Atk / 252 Spe | Crunch, Waterfall, Ice Fang, Poison Fang"],
        'Kaizo': ["Pelipper @ Damp Rock | Modest | Drizzle | 59 | 252 SpA / 252 HP | Scald, Hurricane, U-turn, Roost",
                  "Kingdra @ Life Orb | Modest | Swift Swim | 59 | 252 SpA / 252 Spe | Hydro Pump, Draco Meteor, Ice Beam, Flash Cannon",
                  "Ludicolo @ Sitrus Berry | Modest | Swift Swim | 59 | 252 SpA / 252 Spe | Hydro Pump, Giga Drain, Ice Beam, Fake Out",
                  "Crobat @ Sitrus Berry | Jolly | Inner Focus | 59 | 252 Atk / 252 Spe | Cross Poison, Brave Bird, Super Fang, U-turn",
                  "Mightyena @ Oran Berry | Jolly | Intimidate | 59 | 252 Atk / 252 Spe | Crunch, Play Rough, Sucker Punch, Ice Fang",
                  "Sharpedo @ Focus Sash | Adamant | Speed Boost | 60 | 252 Atk / 252 Spe | Crunch, Waterfall, Close Combat, Protect"],
    },
    'WINONA_1': {
        'Hard': ["Swellow | Jolly | Guts | 59 | 128 Atk / 128 Spe | Brave Bird, Facade, Quick Attack, U-turn",
                 "Tropius | Modest | Chlorophyll | 59 | 128 SpA / 128 HP | Air Slash, Giga Drain, Synthesis, Sunny Day",
                 "Pelipper | Modest | Drizzle | 60 | 128 SpA / 128 HP | Scald, Hurricane, Roost, Protect",
                 "Skarmory | Impish | Sturdy | 60 | 128 HP / 128 Def | Steel Wing, Drill Peck, Spikes, Roost",
                 "Altaria @ Sitrus Berry | Adamant | Natural Cure | 62 | 252 Atk / 252 Spe | Dragon Dance, Dragon Claw, Earthquake, Aerial Ace"],
        'Kaizo': ["Skarmory @ Rocky Helmet | Impish | Sturdy | 62 | 252 HP / 252 Def | Brave Bird, Body Press, Spikes, Roost",
                  "Pelipper @ Damp Rock | Modest | Drizzle | 62 | 252 SpA / 252 HP | Scald, Hurricane, U-turn, Roost",
                  "Swellow @ Flame Orb | Jolly | Guts | 62 | 252 Atk / 252 Spe | Facade, Brave Bird, Quick Attack, U-turn",
                  "Noivern @ Choice Specs | Timid | Infiltrator | 62 | 252 SpA / 252 Spe | Hurricane, Draco Meteor, Flamethrower, Boomburst",
                  "Gyarados @ Sitrus Berry | Jolly | Intimidate | 62 | 252 Atk / 252 Spe | Dragon Dance, Waterfall, Earthquake, Bounce",
                  "Altaria @ Lum Berry | Adamant | Natural Cure | 63 | 252 Atk / 252 Spe | Dragon Dance, Dragon Claw, Earthquake, Fire Blast"],
    },
})

RIVAL_FINAL = {  # starter du joueur -> évolution finale du rival
    'TREECKO': ('Typhlosion', 'Blaze', 'Flash Fire', 'Flamethrower, Focus Blast, Shadow Ball, Extrasensory', 'Eruption, Focus Blast, Shadow Ball, Fire Blast', 'Timid'),
    'TORCHIC': ('Blastoise', 'Torrent', 'Rain Dish', 'Surf, Ice Beam, Dark Pulse, Rapid Spin', 'Hydro Pump, Ice Beam, Dark Pulse, Shell Smash', 'Modest'),
    'MUDKIP':  ('Sceptile', 'Overgrow', 'Unburden', 'Leaf Blade, Dragon Claw, Earthquake, Quick Attack', 'Leaf Blade, Earthquake, Rock Slide, Swords Dance', 'Jolly'),
}
for rival in ('MAY', 'BRENDAN'):
    for starter, (fin, ab, ha, mh, mk, nat) in RIVAL_FINAL.items():
        TEAMS[f'{rival}_ROUTE_119_{starter}'] = {
            'Hard': ["Swellow | Jolly | Guts | 56 | 128 Atk / 128 Spe | Brave Bird, Facade, Quick Attack, Steel Wing",
                     "Ludicolo | Modest | Swift Swim | 56 | 128 SpA / 128 Spe | Surf, Giga Drain, Ice Beam, Fake Out",
                     "Lucario | Jolly | Inner Focus | 57 | 128 Atk / 128 Spe | Close Combat, Extreme Speed, Crunch, Bullet Punch",
                     f"{fin} @ Sitrus Berry | {nat} | {ab} | 58 | 252 Spe / 252 HP | {mh}"],
            'Kaizo': ["Swellow @ Flame Orb | Jolly | Guts | 60 | 252 Atk / 252 Spe | Facade, Brave Bird, Quick Attack, U-turn",
                      "Ludicolo @ Life Orb | Modest | Swift Swim | 60 | 252 SpA / 252 Spe | Hydro Pump, Giga Drain, Ice Beam, Fake Out",
                      "Lucario @ Focus Sash | Jolly | Inner Focus | 60 | 252 Atk / 252 Spe | Close Combat, Extreme Speed, Meteor Mash, Swords Dance",
                      "Magcargo @ White Herb | Modest | Flame Body | 60 | 252 SpA / 252 HP | Shell Smash, Lava Plume, Earth Power, Ancient Power",
                      "Gardevoir @ Choice Scarf | Timid | Trace | 60 | 252 SpA / 252 Spe | Moonblast, Psychic, Shadow Ball, Thunderbolt",
                      f"{fin} @ Sitrus Berry | {nat} | {ha} | 61 | 252 Spe / 252 HP | {mk}"],
        }

# ------------------------------------------------------------------ chapitre 6 : rival à Nénucrique, Repaire Aqua (Max), Lévy & Tatia (cap 72)
TEAMS.update({
    'MATT': {
        'Hard': ["Mightyena | Jolly | Intimidate | 63 | 128 Atk / 128 Spe | Crunch, Play Rough, Sucker Punch, Ice Fang",
                 "Crobat | Jolly | Inner Focus | 63 | 128 Atk / 128 Spe | Cross Poison, Brave Bird, Super Fang, Confuse Ray",
                 "Sharpedo | Adamant | Rough Skin | 64 | 128 Atk / 128 Spe | Crunch, Waterfall, Ice Fang, Poison Fang",
                 "Wailord @ Sitrus Berry | Modest | Water Veil | 65 | 252 HP / 252 SpA | Water Spout, Ice Beam, Hydro Pump, Rest"],
        'Kaizo': ["Pelipper @ Damp Rock | Modest | Drizzle | 66 | 252 SpA / 252 HP | Scald, Hurricane, U-turn, Roost",
                  "Sharpedo @ Life Orb | Adamant | Speed Boost | 66 | 252 Atk / 252 Spe | Crunch, Waterfall, Close Combat, Protect",
                  "Crawdaunt @ Focus Sash | Adamant | Adaptability | 66 | 252 Atk / 252 Spe | Crabhammer, Knock Off, Aqua Jet, Swords Dance",
                  "Crobat @ Sitrus Berry | Jolly | Inner Focus | 66 | 252 Atk / 252 Spe | Cross Poison, Brave Bird, Super Fang, U-turn",
                  "Mightyena @ Sitrus Berry | Jolly | Intimidate | 66 | 252 Atk / 252 Spe | Crunch, Play Rough, Sucker Punch, Ice Fang",
                  "Wailord @ Choice Specs | Modest | Water Veil | 67 | 252 HP / 252 SpA | Water Spout, Hydro Pump, Ice Beam, Hyper Voice"],
    },
    'TATE_AND_LIZA_1': {
        'Hard': ["Claydol | Bold | Levitate | 68 | 128 HP / 128 Def | Earthquake, Psychic, Ancient Power, Light Screen",
                 "Xatu | Timid | Magic Bounce | 68 | 128 SpA / 128 Spe | Psychic, Air Slash, Heat Wave, Tailwind",
                 "Lunatone @ Sitrus Berry | Modest | Levitate | 69 | 252 SpA / 252 HP | Psychic, Moonblast, Ice Beam, Calm Mind",
                 "Solrock @ Sitrus Berry | Adamant | Levitate | 70 | 252 Atk / 252 HP | Rock Slide, Zen Headbutt, Flare Blitz, Trick Room"],
        'Kaizo': ["Solrock @ Sitrus Berry | Brave | Levitate | 71 | 252 HP / 252 Atk | Rock Slide, Zen Headbutt, Flare Blitz, Trick Room",
                  "Lunatone @ Life Orb | Quiet | Levitate | 71 | 252 HP / 252 SpA | Moonblast, Psychic, Ice Beam, Earth Power",
                  "Claydol @ Light Clay | Bold | Levitate | 71 | 252 HP / 252 Def | Earth Power, Psychic, Reflect, Light Screen",
                  "Xatu @ Focus Sash | Timid | Magic Bounce | 71 | 252 SpA / 252 Spe | Psychic, Heat Wave, Tailwind, Protect",
                  "Metagross @ Weakness Policy | Adamant | Clear Body | 72 | 252 Atk / 252 HP | Meteor Mash, Zen Headbutt, Earthquake, Bullet Punch",
                  "Gardevoir @ Choice Specs | Modest | Trace | 72 | 252 SpA / 252 Spe | Moonblast, Psychic, Shadow Ball, Mystical Fire"],
    },
})
for rival in ('MAY', 'BRENDAN'):
    for starter, (fin, ab, ha, mh, mk, nat) in RIVAL_FINAL.items():
        TEAMS[f'{rival}_LILYCOVE_{starter}'] = {
            'Hard': ["Tropius | Modest | Harvest | 62 | 128 SpA / 128 HP | Air Slash, Giga Drain, Synthesis, Sunny Day",
                     "Magcargo | Modest | Flame Body | 62 | 128 SpA / 128 HP | Lava Plume, Earth Power, Ancient Power, Shell Smash",
                     "Pelipper | Modest | Drizzle | 62 | 128 SpA / 128 HP | Scald, Hurricane, Roost, Protect",
                     "Lucario | Jolly | Inner Focus | 63 | 128 Atk / 128 Spe | Close Combat, Extreme Speed, Crunch, Bullet Punch",
                     f"{fin} @ Sitrus Berry | {nat} | {ab} | 64 | 252 Spe / 252 HP | {mh}"],
            'Kaizo': ["Swellow @ Flame Orb | Jolly | Guts | 65 | 252 Atk / 252 Spe | Facade, Brave Bird, Quick Attack, U-turn",
                      "Ludicolo @ Life Orb | Modest | Swift Swim | 65 | 252 SpA / 252 Spe | Hydro Pump, Giga Drain, Ice Beam, Fake Out",
                      "Lucario @ Focus Sash | Jolly | Inner Focus | 65 | 252 Atk / 252 Spe | Close Combat, Extreme Speed, Meteor Mash, Swords Dance",
                      "Magcargo @ White Herb | Modest | Flame Body | 65 | 252 SpA / 252 HP | Shell Smash, Lava Plume, Earth Power, Ancient Power",
                      "Gardevoir @ Choice Scarf | Timid | Trace | 65 | 252 SpA / 252 Spe | Moonblast, Psychic, Shadow Ball, Thunderbolt",
                      f"{fin} @ Sitrus Berry | {nat} | {ha} | 66 | 252 Spe / 252 HP | {mk}"],
        }

# ------------------------------------------------------------------ chapitre 7 : Centre Spatial, Grotte Fonds Marins, Juan (cap 81)
# Max et Tabitha se battent ensemble contre le joueur et Pierre (combat multi) : 3 Pokémon chacun au maximum.
TEAMS.update({
    'TABITHA_MOSSDEEP': {
        'Hard': ["Camerupt | Modest | Solid Rock | 71 | 128 SpA / 128 HP | Lava Plume, Earth Power, Rock Slide, Yawn",
                 "Mightyena | Jolly | Intimidate | 71 | 128 Atk / 128 Spe | Crunch, Play Rough, Sucker Punch, Fire Fang",
                 "Crobat @ Sitrus Berry | Jolly | Inner Focus | 72 | 252 Atk / 252 Spe | Cross Poison, Brave Bird, Super Fang, Taunt"],
        'Kaizo': ["Torkoal @ Heat Rock | Quiet | Drought | 74 | 252 HP / 252 SpA | Eruption, Earth Power, Solar Beam, Protect",
                  "Houndoom @ Life Orb | Timid | Flash Fire | 74 | 252 SpA / 252 Spe | Heat Wave, Dark Pulse, Sludge Bomb, Nasty Plot",
                  "Crobat @ Sitrus Berry | Jolly | Inner Focus | 75 | 252 Atk / 252 Spe | Cross Poison, Brave Bird, Super Fang, Tailwind"],
    },
    'MAXIE_MOSSDEEP': {
        'Hard': ["Mightyena | Jolly | Intimidate | 73 | 128 Atk / 128 Spe | Crunch, Play Rough, Sucker Punch, Fire Fang",
                 "Weezing | Bold | Levitate | 73 | 128 HP / 128 Def | Sludge Bomb, Fire Blast, Will-O-Wisp, Pain Split",
                 "Camerupt @ Sitrus Berry | Modest | Solid Rock | 74 | 252 SpA / 252 HP | Eruption, Earth Power, Rock Slide, Protect"],
        'Kaizo': ["Mightyena @ Sitrus Berry | Jolly | Intimidate | 76 | 252 Atk / 252 Spe | Crunch, Play Rough, Sucker Punch, Fire Fang",
                  "Weezing-Galar @ Black Sludge | Bold | Neutralizing Gas | 76 | 252 HP / 252 Def | Strange Steam, Sludge Bomb, Will-O-Wisp, Protect",
                  "Camerupt @ Choice Specs | Quiet | Solid Rock | 77 | 252 HP / 252 SpA | Eruption, Earth Power, Heat Wave, Ancient Power"],
    },
    'SHELLY_SEAFLOOR_CAVERN': {
        'Hard': ["Mightyena | Jolly | Intimidate | 72 | 128 Atk / 128 Spe | Crunch, Play Rough, Sucker Punch, Ice Fang",
                 "Pelipper @ Damp Rock | Modest | Drizzle | 72 | 128 SpA / 128 HP | Scald, Hurricane, Roost, Protect",
                 "Sharpedo @ Sitrus Berry | Adamant | Rough Skin | 73 | 252 Atk / 252 Spe | Crunch, Waterfall, Ice Fang, Poison Fang"],
        'Kaizo': ["Pelipper @ Damp Rock | Modest | Drizzle | 75 | 252 SpA / 252 HP | Scald, Hurricane, U-turn, Roost",
                  "Kingdra @ Life Orb | Modest | Swift Swim | 75 | 252 SpA / 252 Spe | Hydro Pump, Draco Meteor, Ice Beam, Flash Cannon",
                  "Ludicolo @ Sitrus Berry | Modest | Swift Swim | 75 | 252 SpA / 252 Spe | Hydro Pump, Giga Drain, Ice Beam, Fake Out",
                  "Mightyena @ Oran Berry | Jolly | Intimidate | 75 | 252 Atk / 252 Spe | Crunch, Play Rough, Sucker Punch, Ice Fang",
                  "Sharpedo @ Focus Sash | Adamant | Speed Boost | 76 | 252 Atk / 252 Spe | Crunch, Waterfall, Close Combat, Protect"],
    },
    'ARCHIE': {
        'Hard': ["Mightyena | Jolly | Intimidate | 75 | 128 Atk / 128 Spe | Crunch, Play Rough, Sucker Punch, Ice Fang",
                 "Crobat | Jolly | Inner Focus | 75 | 128 Atk / 128 Spe | Cross Poison, Brave Bird, Super Fang, Confuse Ray",
                 "Muk | Adamant | Poison Touch | 76 | 128 Atk / 128 HP | Gunk Shot, Knock Off, Ice Punch, Shadow Sneak",
                 "Sharpedo @ Sitrus Berry | Adamant | Rough Skin | 77 | 252 Atk / 252 Spe | Crunch, Waterfall, Ice Fang, Close Combat"],
        'Kaizo': ["Pelipper @ Damp Rock | Modest | Drizzle | 78 | 252 SpA / 252 HP | Scald, Hurricane, U-turn, Roost",
                  "Crobat @ Sitrus Berry | Jolly | Inner Focus | 78 | 252 Atk / 252 Spe | Cross Poison, Brave Bird, Super Fang, U-turn",
                  "Muk-Alola @ Assault Vest | Adamant | Poison Touch | 78 | 252 HP / 252 Atk | Knock Off, Poison Jab, Ice Punch, Shadow Sneak",
                  "Mightyena @ Sitrus Berry | Jolly | Intimidate | 78 | 252 Atk / 252 Spe | Crunch, Play Rough, Sucker Punch, Ice Fang",
                  "Swampert @ Leftovers | Adamant | Damp | 78 | 252 HP / 252 Atk | Waterfall, Earthquake, Ice Punch, Stealth Rock",
                  "Sharpedo @ Life Orb | Adamant | Speed Boost | 79 | 252 Atk / 252 Spe | Crunch, Waterfall, Close Combat, Protect"],
    },
    'JUAN_1': {
        'Hard': ["Whiscash | Adamant | Oblivious | 78 | 128 Atk / 128 HP | Earthquake, Waterfall, Stone Edge, Dragon Dance",
                 "Walrein | Modest | Thick Fat | 78 | 128 SpA / 128 HP | Surf, Ice Beam, Body Slam, Rest",
                 "Crawdaunt | Adamant | Adaptability | 79 | 128 Atk / 128 Spe | Crabhammer, Knock Off, Aqua Jet, Swords Dance",
                 "Milotic | Bold | Marvel Scale | 79 | 128 HP / 128 Def | Scald, Ice Beam, Recover, Dragon Tail",
                 "Kingdra @ Sitrus Berry | Modest | Swift Swim | 80 | 252 SpA / 252 Spe | Hydro Pump, Draco Meteor, Ice Beam, Rain Dance"],
        'Kaizo': ["Politoed @ Damp Rock | Calm | Drizzle | 80 | 252 HP / 252 SpD | Scald, Ice Beam, Encore, Perish Song",
                  "Kingdra @ Life Orb | Modest | Swift Swim | 80 | 252 SpA / 252 Spe | Hydro Pump, Draco Meteor, Ice Beam, Flash Cannon",
                  "Crawdaunt @ Focus Sash | Adamant | Adaptability | 80 | 252 Atk / 252 Spe | Crabhammer, Knock Off, Aqua Jet, Swords Dance",
                  "Whiscash @ Sitrus Berry | Adamant | Oblivious | 80 | 252 Atk / 252 HP | Earthquake, Waterfall, Stone Edge, Dragon Dance",
                  "Walrein @ Leftovers | Modest | Thick Fat | 80 | 252 HP / 252 SpA | Blizzard, Surf, Protect, Substitute",
                  "Milotic @ Flame Orb | Bold | Marvel Scale | 81 | 252 HP / 252 Def | Scald, Ice Beam, Recover, Haze"],
    },
})

# Chapitre 8 : Route Victoire (Timmy), Conseil 4 et Marc (cap 85 de la Ligue). Méga-évolution des as en Kaizo.
TEAMS.update({
    'WALLY_VR_1': {
        'Hard': ["Altaria | Adamant | Natural Cure | 80 | 128 Atk / 128 Spe | Dragon Dance, Dragon Claw, Earthquake, Roost",
                 "Delcatty | Jolly | Wonder Skin | 80 | 128 Atk / 128 Spe | Fake Out, Double-Edge, Sucker Punch, Wild Charge",
                 "Roserade | Timid | Natural Cure | 81 | 128 SpA / 128 Spe | Giga Drain, Sludge Bomb, Shadow Ball, Sleep Powder",
                 "Magnezone | Modest | Magnet Pull | 81 | 128 SpA / 128 HP | Thunderbolt, Flash Cannon, Volt Switch, Protect",
                 "Gardevoir @ Sitrus Berry | Modest | Trace | 82 | 252 SpA / 252 Spe | Moonblast, Psychic, Mystical Fire, Calm Mind"],
        'Kaizo': ["Altaria @ Lum Berry | Adamant | Natural Cure | 83 | 252 Atk / 252 Spe | Dragon Dance, Dragon Claw, Earthquake, Roost",
                  "Gallade @ Focus Sash | Jolly | Sharpness | 83 | 252 Atk / 252 Spe | Sacred Sword, Psycho Cut, Leaf Blade, Swords Dance",
                  "Roserade @ Life Orb | Timid | Technician | 83 | 252 SpA / 252 Spe | Giga Drain, Sludge Bomb, Shadow Ball, Spikes",
                  "Magnezone @ Choice Specs | Modest | Magnet Pull | 83 | 252 SpA / 252 HP | Thunderbolt, Flash Cannon, Volt Switch, Body Press",
                  "Delcatty @ Silk Scarf | Jolly | Normalize | 83 | 252 Atk / 252 Spe | Fake Out, Double-Edge, Sucker Punch, Wild Charge",
                  "Gardevoir @ Gardevoirite | Timid | Trace | 84 | 252 SpA / 252 Spe | Hyper Voice, Psyshock, Mystical Fire, Calm Mind"],
    },
    'WALLY_VR_2': {
        'Hard': ["Altaria | Adamant | Natural Cure | 86 | 128 Atk / 128 Spe | Dragon Dance, Dragon Claw, Earthquake, Roost",
                 "Delcatty | Jolly | Wonder Skin | 86 | 128 Atk / 128 Spe | Fake Out, Double-Edge, Sucker Punch, Wild Charge",
                 "Roserade | Timid | Natural Cure | 87 | 128 SpA / 128 Spe | Giga Drain, Sludge Bomb, Shadow Ball, Sleep Powder",
                 "Magnezone | Modest | Magnet Pull | 87 | 128 SpA / 128 HP | Thunderbolt, Flash Cannon, Volt Switch, Protect",
                 "Gardevoir @ Sitrus Berry | Modest | Trace | 88 | 252 SpA / 252 Spe | Moonblast, Psychic, Mystical Fire, Calm Mind"],
        'Kaizo': ["Altaria @ Lum Berry | Adamant | Natural Cure | 89 | 252 Atk / 252 Spe | Dragon Dance, Dragon Claw, Earthquake, Roost",
                  "Gallade @ Focus Sash | Jolly | Sharpness | 89 | 252 Atk / 252 Spe | Sacred Sword, Psycho Cut, Leaf Blade, Swords Dance",
                  "Roserade @ Life Orb | Timid | Technician | 89 | 252 SpA / 252 Spe | Giga Drain, Sludge Bomb, Shadow Ball, Spikes",
                  "Magnezone @ Choice Specs | Modest | Magnet Pull | 89 | 252 SpA / 252 HP | Thunderbolt, Flash Cannon, Volt Switch, Body Press",
                  "Delcatty @ Silk Scarf | Jolly | Normalize | 89 | 252 Atk / 252 Spe | Fake Out, Double-Edge, Sucker Punch, Wild Charge",
                  "Gardevoir @ Gardevoirite | Timid | Trace | 90 | 252 SpA / 252 Spe | Hyper Voice, Psyshock, Mystical Fire, Calm Mind"],
    },
    'SIDNEY': {
        'Hard': ["Mightyena | Jolly | Intimidate | 81 | 128 Atk / 128 Spe | Crunch, Play Rough, Sucker Punch, Ice Fang",
                 "Shiftry | Adamant | Chlorophyll | 81 | 128 Atk / 128 Spe | Leaf Blade, Knock Off, Sucker Punch, Swords Dance",
                 "Cacturne | Adamant | Water Absorb | 82 | 128 Atk / 128 HP | Seed Bomb, Knock Off, Drain Punch, Spikes",
                 "Crawdaunt | Adamant | Adaptability | 82 | 128 Atk / 128 Spe | Crabhammer, Knock Off, Aqua Jet, Swords Dance",
                 "Absol @ Sitrus Berry | Jolly | Super Luck | 83 | 252 Atk / 252 Spe | Night Slash, Psycho Cut, Sucker Punch, Swords Dance"],
        'Kaizo': ["Mightyena @ Sitrus Berry | Jolly | Intimidate | 83 | 252 Atk / 252 Spe | Crunch, Play Rough, Sucker Punch, Ice Fang",
                  "Shiftry @ Focus Sash | Adamant | Wind Rider | 83 | 252 Atk / 252 Spe | Leaf Blade, Knock Off, Sucker Punch, Tailwind",
                  "Hydreigon @ Choice Specs | Timid | Levitate | 83 | 252 SpA / 252 Spe | Dark Pulse, Draco Meteor, Flash Cannon, Fire Blast",
                  "Crawdaunt @ Life Orb | Adamant | Adaptability | 83 | 252 Atk / 252 Spe | Crabhammer, Knock Off, Aqua Jet, Swords Dance",
                  "Tyranitar @ Leftovers | Adamant | Sand Stream | 83 | 252 HP / 252 Atk | Stone Edge, Crunch, Earthquake, Stealth Rock",
                  "Absol @ Absolite | Jolly | Justified | 84 | 252 Atk / 252 Spe | Knock Off, Psycho Cut, Sucker Punch, Swords Dance"],
    },
    'PHOEBE': {
        'Hard': ["Dusclops | Relaxed | Pressure | 82 | 128 HP / 128 Def | Shadow Sneak, Will-O-Wisp, Pain Split, Trick Room",
                 "Banette | Adamant | Frisk | 82 | 128 Atk / 128 HP | Poltergeist, Shadow Sneak, Knock Off, Gunk Shot",
                 "Sableye | Bold | Prankster | 83 | 128 HP / 128 Def | Foul Play, Will-O-Wisp, Recover, Knock Off",
                 "Mismagius | Timid | Levitate | 83 | 128 SpA / 128 Spe | Shadow Ball, Dazzling Gleam, Thunderbolt, Nasty Plot",
                 "Dusknoir @ Sitrus Berry | Adamant | Pressure | 84 | 252 HP / 252 Atk | Poltergeist, Earthquake, Ice Punch, Shadow Sneak"],
        'Kaizo': ["Sableye @ Light Clay | Bold | Prankster | 84 | 252 HP / 252 Def | Foul Play, Will-O-Wisp, Reflect, Light Screen",
                  "Gengar @ Focus Sash | Timid | Cursed Body | 84 | 252 SpA / 252 Spe | Shadow Ball, Sludge Wave, Focus Blast, Destiny Bond",
                  "Chandelure @ Choice Scarf | Timid | Flash Fire | 84 | 252 SpA / 252 Spe | Shadow Ball, Fire Blast, Energy Ball, Trick",
                  "Dusknoir @ Leftovers | Adamant | Frisk | 84 | 252 HP / 252 Atk | Poltergeist, Earthquake, Ice Punch, Trick Room",
                  "Mismagius @ Life Orb | Timid | Levitate | 84 | 252 SpA / 252 Spe | Shadow Ball, Dazzling Gleam, Thunderbolt, Nasty Plot",
                  "Banette @ Banettite | Adamant | Frisk | 85 | 252 Atk / 252 HP | Poltergeist, Shadow Sneak, Knock Off, Gunk Shot"],
    },
    'GLACIA': {
        'Hard': ["Glalie | Jolly | Inner Focus | 83 | 128 Atk / 128 Spe | Icicle Crash, Earthquake, Ice Shard, Crunch",
                 "Froslass | Timid | Snow Cloak | 83 | 128 SpA / 128 Spe | Ice Beam, Shadow Ball, Thunderbolt, Spikes",
                 "Abomasnow | Quiet | Snow Warning | 83 | 128 SpA / 128 HP | Blizzard, Giga Drain, Earth Power, Ice Shard",
                 "Cloyster | Adamant | Skill Link | 84 | 128 Atk / 128 Spe | Icicle Spear, Rock Blast, Ice Shard, Shell Smash",
                 "Walrein @ Sitrus Berry | Modest | Thick Fat | 84 | 252 HP / 252 SpA | Blizzard, Surf, Earth Power, Protect"],
        'Kaizo': ["Ninetales-Alola @ Light Clay | Timid | Snow Warning | 84 | 252 SpA / 252 Spe | Blizzard, Moonblast, Aurora Veil, Freeze-Dry",
                  "Cloyster @ Focus Sash | Adamant | Skill Link | 84 | 252 Atk / 252 Spe | Icicle Spear, Rock Blast, Ice Shard, Shell Smash",
                  "Mamoswine @ Life Orb | Adamant | Thick Fat | 85 | 252 Atk / 252 Spe | Icicle Crash, Earthquake, Ice Shard, Knock Off",
                  "Weavile @ Choice Band | Jolly | Pressure | 85 | 252 Atk / 252 Spe | Triple Axel, Knock Off, Ice Shard, Low Kick",
                  "Walrein @ Leftovers | Modest | Thick Fat | 85 | 252 HP / 252 SpA | Blizzard, Surf, Earth Power, Protect",
                  "Glalie @ Glalitite | Jolly | Inner Focus | 85 | 252 Atk / 252 Spe | Double-Edge, Earthquake, Ice Shard, Explosion"],
    },
    'DRAKE': {
        'Hard': ["Altaria | Adamant | Natural Cure | 84 | 128 Atk / 128 Spe | Dragon Dance, Dragon Claw, Earthquake, Roost",
                 "Kingdra | Modest | Swift Swim | 84 | 128 SpA / 128 Spe | Hydro Pump, Draco Meteor, Ice Beam, Flash Cannon",
                 "Flygon | Jolly | Levitate | 84 | 128 Atk / 128 Spe | Earthquake, Dragon Claw, U-turn, Fire Punch",
                 "Haxorus | Jolly | Mold Breaker | 84 | 128 Atk / 128 Spe | Dragon Claw, Earthquake, Poison Jab, Dragon Dance",
                 "Salamence @ Sitrus Berry | Adamant | Intimidate | 85 | 252 Atk / 252 Spe | Dragon Claw, Earthquake, Fire Fang, Dragon Dance"],
        'Kaizo': ["Dragonite @ Heavy-Duty Boots | Adamant | Multiscale | 85 | 252 Atk / 252 Spe | Extreme Speed, Dragon Claw, Earthquake, Dragon Dance",
                  "Garchomp @ Rocky Helmet | Jolly | Rough Skin | 85 | 252 Atk / 252 Spe | Earthquake, Scale Shot, Stone Edge, Stealth Rock",
                  "Kingdra @ Choice Specs | Modest | Sniper | 85 | 252 SpA / 252 Spe | Hydro Pump, Draco Meteor, Ice Beam, Flash Cannon",
                  "Flygon @ Choice Scarf | Jolly | Levitate | 85 | 252 Atk / 252 Spe | Earthquake, Outrage, U-turn, Fire Punch",
                  "Haxorus @ Focus Sash | Jolly | Mold Breaker | 85 | 252 Atk / 252 Spe | Outrage, Earthquake, Poison Jab, Dragon Dance",
                  "Salamence @ Salamencite | Adamant | Intimidate | 85 | 252 Atk / 252 Spe | Double-Edge, Earthquake, Dual Wingbeat, Dragon Dance"],
    },
    'WALLACE': {
        'Hard': ["Wailord | Modest | Pressure | 84 | 128 HP / 128 SpA | Water Spout, Ice Beam, Hydro Pump, Rain Dance",
                 "Tentacruel | Timid | Clear Body | 84 | 128 SpA / 128 Spe | Sludge Bomb, Scald, Knock Off, Toxic Spikes",
                 "Ludicolo | Modest | Swift Swim | 84 | 128 SpA / 128 Spe | Hydro Pump, Giga Drain, Ice Beam, Fake Out",
                 "Whiscash | Adamant | Oblivious | 85 | 128 Atk / 128 HP | Earthquake, Waterfall, Stone Edge, Dragon Dance",
                 "Gyarados | Adamant | Intimidate | 85 | 128 Atk / 128 Spe | Waterfall, Earthquake, Crunch, Dragon Dance",
                 "Milotic @ Sitrus Berry | Modest | Competitive | 85 | 252 HP / 252 SpA | Scald, Ice Beam, Dragon Pulse, Recover"],
        'Kaizo': ["Pelipper @ Damp Rock | Modest | Drizzle | 85 | 252 HP / 252 SpA | Scald, Hurricane, U-turn, Roost",
                  "Ludicolo @ Life Orb | Modest | Swift Swim | 85 | 252 SpA / 252 Spe | Hydro Pump, Giga Drain, Ice Beam, Fake Out",
                  "Tentacruel @ Black Sludge | Timid | Clear Body | 85 | 252 SpA / 252 Spe | Sludge Bomb, Hydro Pump, Knock Off, Toxic Spikes",
                  "Swampert @ Leftovers | Adamant | Damp | 85 | 252 HP / 252 Atk | Waterfall, Earthquake, Ice Punch, Stealth Rock",
                  "Gyarados @ Gyaradosite | Adamant | Intimidate | 85 | 252 Atk / 252 Spe | Waterfall, Crunch, Earthquake, Dragon Dance",
                  "Milotic @ Leftovers | Bold | Competitive | 85 | 252 HP / 252 Def | Scald, Ice Beam, Recover, Haze"],
    },
})

# Attaques qui ne font que baisser les stats adverses (ou presque rien) : retirées des équipes Hard / Kaizo
# pour que l'IA attaque au lieu de spammer Rugissement / Jet de Sable (retour d'Adrien, v0.17).
WEAK_MOVES = {'Leer', 'Tail Whip', 'Growl', 'Sand Attack', 'Smokescreen', 'String Shot', 'Sweet Scent',
              'Scary Face', 'Charm', 'Screech', 'Mist', 'Harden', 'Withdraw', 'Defense Curl', 'Poison Gas', 'Kinesis'}


def filter_moves(moves):
    kept = [m.strip() for m in moves.split(',') if m.strip() not in WEAK_MOVES]
    if 'Rollout' in moves and 'Defense Curl' in moves:
        kept.insert(0, 'Defense Curl')
    return ', '.join(kept) if len(kept) >= 2 else moves


def mon_block(spec, iv):
    parts = [p.strip() for p in spec.split('|')] + [''] * 6
    species, nature, ability, level, evs, moves = parts[:6]
    lines = [species]
    if nature:
        lines.append(f'{nature} Nature')
    lines.append(f'Level: {level}')
    if ability:
        lines.append(f'Ability: {ability}')
    lines.append(f'IVs: {iv} HP / {iv} Atk / {iv} Def / {iv} SpA / {iv} SpD / {iv} Spe')
    if evs:
        lines.append(f'EVs: {evs}')
    if moves:  # sans capacités : le jeu prend les capacités apprises par niveau
        lines += [f'- {m.strip()}' for m in filter_moves(moves).split(',')]
    return '\n'.join(lines)


# ------------------------------------------------------------------ dresseurs ordinaires (règles)
# Réserves par classe de dresseur (Gen 1-4 majoritaires en début de jeu).
POOLS = {
    'combat':   ['Machop', 'Meditite', 'Makuhita', 'Mankey', 'Riolu', 'Croagunk', 'Timburr', 'Tyrogue'],
    'eau':      ['Tentacool', 'Wingull', 'Marill', 'Goldeen', 'Horsea', 'Psyduck', 'Krabby', 'Shellder', 'Poliwag', 'Buizel', 'Chinchou', 'Remoraid'],
    'psy':      ['Abra', 'Ralts', 'Natu', 'Spoink', 'Drowzee', 'Slowpoke', 'Meditite', 'Munna'],
    'electrik': ['Magnemite', 'Voltorb', 'Electrike', 'Plusle', 'Minun', 'Shinx', 'Pikachu', 'Mareep', 'Blitzle'],
    'course':   ['Doduo', 'Ponyta', 'Taillow', 'Starly', 'Pidgey', 'Electrike'],
    'insecte':  ['Dustox', 'Beautifly', 'Volbeat', 'Illumise', 'Nincada', 'Paras', 'Venonat', 'Combee', 'Kricketune'],
    'plante':   ['Shroomish', 'Roselia', 'Oddish', 'Bellsprout', 'Seedot', 'Budew', 'Hoppip'],
    'sol':      ['Sandshrew', 'Baltoy', 'Trapinch', 'Cacnea', 'Hippopotas', 'Gligar', 'Diglett', 'Phanpy'],
    'feu':      ['Numel', 'Slugma', 'Growlithe', 'Vulpix', 'Houndour', 'Ponyta', 'Magby', 'Litleo'],
    'roche':    ['Geodude', 'Onix', 'Nosepass', 'Aron', 'Rhyhorn', 'Roggenrola', 'Larvitar'],
    'oiseau':   ['Swablu', 'Taillow', 'Natu', 'Hoothoot', 'Starly', 'Murkrow', 'Skarmory', 'Doduo'],
    'poison':   ['Koffing', 'Grimer', 'Ekans', 'Gulpin', 'Seviper', 'Skorupi', 'Stunky', 'Ninjask'],
    'elite':    ['Absol', 'Kirlia', 'Lairon', 'Vibrava', 'Growlithe', 'Eevee', 'Sneasel', 'Murkrow'],
    'beaute':   ['Marill', 'Roselia', 'Skitty', 'Vulpix', 'Swablu', 'Chinchou', 'Snubbull', 'Milotic'],
    'normal':   ['Ursaring', 'Zangoose', 'Kecleon', 'Spinda', 'Bibarel', 'Lopunny', 'Furret', 'Tauros', 'Miltank', 'Porygon', 'Linoone', 'Girafarig'],
    'spectre':  ['Shuppet', 'Duskull', 'Misdreavus', 'Gastly', 'Sableye', 'Litwick', 'Drifloon', 'Phantump'],
    'aqua':     ['Carvanha', 'Zubat', 'Poochyena', 'Wailmer', 'Corphish', 'Tentacool', 'Spheal', 'Barboach'],
    'magma':    ['Numel', 'Poochyena', 'Zubat', 'Slugma', 'Baltoy', 'Koffing', 'Houndour', 'Torkoal'],
    'mixte':    ['Zigzagoon', 'Skitty', 'Whismur', 'Aron', 'Poochyena', 'Azurill', 'Eevee', 'Teddiursa', 'Snubbull', 'Buneary'],
}
CLASS_POOL = {
    'Black Belt': 'combat', 'Battle Girl': 'combat', 'Sailor': 'combat', 'Tuber M': 'eau', 'Tuber F': 'eau',
    'Fisherman': 'eau', 'Swimmer M': 'eau', 'Swimmer F': 'eau', 'Psychic': 'psy', 'Triathlete': 'electrik',
    'Guitarist': 'electrik', 'Pokefan': 'electrik', 'Bug Maniac': 'insecte', 'Aroma Lady': 'plante',
    'Collector': 'plante', 'Pkmn Breeder': 'mixte', 'Youngster': 'mixte', 'Twins': 'electrik', 'Sr And Jr': 'mixte',
    'Camper': 'sol', 'Hex Maniac': 'spectre', 'Team Aqua': 'aqua', 'Team Magma': 'magma', 'Cooltrainer 2': 'elite', 'Pkmn Ranger': 'plante', 'Bug Catcher': 'insecte', 'Ruin Maniac': 'sol', 'Picnicker': 'plante', 'Kindler': 'feu', 'Hiker': 'roche',
    'Bird Keeper': 'oiseau', 'Ninja Boy': 'poison', 'Cooltrainer': 'elite', 'Expert': 'combat',
    'Parasol Lady': 'beaute', 'Sis And Bro': 'eau', 'Young Couple': 'insecte', 'Gentleman': 'mixte', 'Lass': 'mixte', 'Beauty': 'beaute', 'Pokemaniac': 'roche', 'Winstrate': 'mixte',
}
EVS_KAIZO = '84 HP / 84 Atk / 84 Def / 84 SpA / 84 SpD / 84 Spe'

# Dresseurs ordinaires générés : ID -> level cap de la zone.
AUTO = {}
for tid in ['TAKAO', 'JOCELYN', 'LAURA', 'BRENDEN', 'CRISTIAN', 'LILITH', 'ELLIOT_1', 'NED',
            'HUEY', 'EDMOND', 'RICKY_1', 'LOLA_1', 'CHANDLER', 'HAILEY']:
    AUTO[tid] = 27
for tid in ['EDWARD', 'JACLYN', 'EDWIN_1', 'DALE', 'JACOB', 'ANTHONY', 'BENJAMIN_1', 'JASMINE', 'ABIGAIL_1',
            'ISABEL_1', 'TIMMY', 'KALEB', 'JOSEPH', 'ALYSSA', 'DAISY', 'AMY_AND_LIV_1', 'ANDREW', 'MIGUEL_1',
            'MARCOS', 'RHETT', 'PETE', 'ISABELLE', 'ISAAC_1', 'LYDIA_1', 'DYLAN_1', 'MARIA_1', 'DEREK',
            'ANNA_AND_MEG_1', 'MELINA', 'BRANDI', 'AISHA', 'KIRK', 'SHAWN', 'BEN', 'VIVIAN', 'ANGELO']:
    AUTO[tid] = 36


# Chapitre 3 (cap 45 avant Adriane ; 54 pour le désert de la Route 111, ouvert après le 4e badge).
# Les niveaux d'origine sont bas : plancher par difficulté pour garder la pression.
for tid in ['VICTOR', 'VICTORIA', 'VIVI', 'VICKY', 'IRENE', 'TRAVIS', 'TYRON', 'DAISUKE', 'WILTON_1', 'BROOKE_1',
            'CELINA', 'BIANCA', 'HAYDEN', 'BRICE', 'BRYANT', 'CAROL', 'LARRY', 'SHAYLA', 'TRENT_1',
            'COBY', 'DILLON', 'JAYLEN', 'LAO_1', 'LAWRENCE', 'LUNG', 'MADELINE_1', 'SOPHIE', 'TORI_AND_TIA', 'WYATT',
            'ANGELINA', 'BERNIE_1', 'CHARLOTTE', 'CLAUDE', 'KAI', 'LENNY', 'LUCAS_1', 'NANCY', 'NOLAN', 'SHANE',
            'STEVE_1', 'TYRA_AND_IVY', 'MELISSA', 'SAWYER_1', 'SHEILA', 'SHELBY_1', 'SHIRLEY',
            'AUTUMN', 'DIANA_1', 'ERIC', 'ETHAN_1', 'JULIO']:
    AUTO[tid] = (45, 30, 34)
for tid in ['AXLE', 'COLE', 'DANIELLE', 'ELI', 'GERALD', 'JACE', 'JEFF', 'KEEGAN']:  # Arène d'Adriane
    AUTO[tid] = (45, 35, 39)
for tid in ['BEAU', 'BECKY', 'BRANDEN', 'BRYAN', 'CELIA', 'DREW', 'DUSTY_1', 'HEIDI']:  # désert
    AUTO[tid] = (54, 40, 44)

# Chapitre 4 : Arène de Norman (cap 54, réserve Normal), puis eaux ouvertes par Surf (Routes 105-109, 115,
# Épave Abandonnée), parcourues avant Alizée (cap 63).
for tid in ['ALEXIA', 'BERKE', 'GEORGE', 'JODY', 'MARY', 'PARKER', 'RANDALL']:
    AUTO[tid] = (54, 46, 50, 'normal')
for tid in ['ANDRES_1', 'BEVERLY', 'DOMINIK', 'FOSTER', 'IMANI', 'JOSUE', 'LUIS', 'DOUGLAS', 'KYLA',
            'BETH', 'CAMRON', 'DARRIN', 'DENISE', 'LISA_AND_RAY', 'TONY_1', 'CAROLINA', 'CORY_1', 'JEROME',
            'MATTHEW', 'MISSY', 'TARA', 'ALICE', 'AUSTINA', 'CARTER', 'DAVID', 'ELIJAH', 'GWEN', 'MEL_AND_PAUL',
            'ALIX', 'CYNDY_1', 'HECTOR', 'HELENE', 'JAIDEN', 'KOICHI', 'KYRA', 'MARLENE', 'NOB_1', 'TIMOTHY_1',
            'CHARLIE', 'DEMETRIUS', 'DUNCAN', 'GARRISON', 'JANI', 'KIRA_AND_DAN_1', 'THALIA_1']:
    AUTO[tid] = (63, 47, 51)

# Chapitre 5 : Routes 118-120 (cap 63 avant Alizée), puis l'Arène d'Alizée (réserve Vol).
for tid in ['BARNY', 'CHESTER', 'DALTON_1', 'DEANDRE', 'PERRY', 'ROSE_1', 'WADE',
            'BRENT', 'CATHERINE_1', 'CHRIS', 'DAYTON', 'DONALD', 'DOUG', 'FABIAN', 'GREG', 'HIDEO', 'HUGH',
            'JACKSON_1', 'KENT', 'PHIL', 'RACHEL', 'TAKASHI', 'TAYLOR', 'YASU',
            'ANGELICA', 'CALLIE', 'CHIP', 'CLARISSA', 'COLIN', 'JEFFREY_1', 'JENNA', 'JENNIFER', 'KEIGO',
            'LEONEL', 'LORENZO', 'RILEY', 'ROBERT_1']:
    AUTO[tid] = (63, 52, 56)
for tid in ['ASHLEY', 'DARIUS', 'EDWARDO', 'FLINT', 'HUMBERTO', 'JARED']:  # Arène d'Alizée
    AUTO[tid] = (63, 56, 60, 'oiseau')

# Chapitre 6 : Routes 121 et 123, Mont Mémoria (sbires Aqua au sommet), Repaire Aqua, Routes 124-125 puis Arène
# de Lévy & Tatia (cap 72, réserve Psy).
for tid in ['CALE', 'CRISTIN_1', 'JESSICA_1', 'KATE_AND_JOY', 'MARCEL', 'MYLES', 'PAT', 'TAMMY', 'VANESSA', 'WALTER_1',
            'ALBERTO', 'BRAXTON', 'CAMERON_1', 'DAVIS', 'ED', 'FERNANDO_1', 'FREDRICK', 'JACKI_1', 'JAZMYN', 'JONAS',
            'KAYLEY', 'KINDRA', 'MIU_AND_YUKI', 'VIOLET', 'WENDY',
            'DEZ_AND_LUKE', 'LEAH', 'MARK', 'ZANDER', 'GABRIELLE_1', 'KAYLA', 'WILLIAM', 'ATSUSHI', 'TASHA', 'CEDRIC',
            'VALERIE_1', 'GRUNT_MT_PYRE_1', 'GRUNT_MT_PYRE_2', 'GRUNT_MT_PYRE_3', 'GRUNT_MT_PYRE_4']:
    AUTO[tid] = (72, 58, 62)
for tid in ['GRUNT_AQUA_HIDEOUT_1', 'GRUNT_AQUA_HIDEOUT_2', 'GRUNT_AQUA_HIDEOUT_3', 'GRUNT_AQUA_HIDEOUT_4',
            'GRUNT_AQUA_HIDEOUT_5', 'GRUNT_AQUA_HIDEOUT_6', 'GRUNT_AQUA_HIDEOUT_7', 'GRUNT_AQUA_HIDEOUT_8',
            'CHAD', 'DECLAN', 'GRACE', 'ISABELLA', 'JENNY_1', 'LILA_AND_ROY_1', 'ROLAND', 'SPENCER',
            'AURON', 'ERNEST_1', 'KIM_AND_IRIS', 'NOLEN', 'PRESLEY', 'SHARON', 'STAN', 'TANYA']:
    AUTO[tid] = (72, 60, 64)
for tid in ['BLAKE', 'CLIFFORD', 'HANNAH', 'KATHLEEN', 'MACEY', 'MAURA', 'NATE', 'NICHOLAS', 'PRESTON',
            'SAMANTHA', 'SYLVIA', 'VIRGIL']:  # Arène de Lévy & Tatia
    AUTO[tid] = (72, 64, 68, 'psy')

# Chapitre 7 : Centre Spatial (Magma), Routes 126-128 (mer), Grotte Fonds Marins (Aqua), Arène de Juan (cap 81).
for tid in ['GRUNT_SPACE_CENTER_1', 'GRUNT_SPACE_CENTER_2', 'GRUNT_SPACE_CENTER_3', 'GRUNT_SPACE_CENTER_4',
            'GRUNT_SPACE_CENTER_5', 'GRUNT_SPACE_CENTER_6', 'GRUNT_SPACE_CENTER_7',
            'GRUNT_SEAFLOOR_CAVERN_1', 'GRUNT_SEAFLOOR_CAVERN_2', 'GRUNT_SEAFLOOR_CAVERN_3', 'GRUNT_SEAFLOOR_CAVERN_4',
            'GRUNT_SEAFLOOR_CAVERN_5']:
    AUTO[tid] = (81, 68, 72)
for tid in ['BARRY', 'BRENDA', 'DEAN', 'ISOBEL', 'LEONARDO', 'NIKKI', 'PABLO_1', 'SIENNA',
            'AIDAN', 'ATHENA', 'CAMDEN', 'DONNY', 'HENRY', 'JONAH', 'KOJI_1', 'ROGER',
            'ALEXA', 'CARLEE', 'HARRISON', 'ISAIAH_1', 'KATELYN_1', 'RUBEN', 'WAYNE']:
    AUTO[tid] = (81, 66, 70)
for tid in ['ANDREA', 'ANNIKA', 'BETHANY', 'BRIANNA', 'BRIDGET', 'CONNIE', 'CRISSY', 'DAPHNE', 'OLIVIA', 'TIFFANY']:
    AUTO[tid] = (81, 72, 76, 'eau')  # Arène de Juan

# Chapitre 8 : mers du sud (Routes 129-134, ouvertes avec Plongée avant Juan), puis Route Victoire (cap 85 de la Ligue).
for tid in ['ALLISON', 'CHASE', 'CLARENCE', 'REED', 'TISHA', 'KATIE', 'RODNEY', 'SANTIAGO',
            'HERMAN', 'KARA', 'KEVIN', 'RELI_AND_IAN', 'RICHARD', 'SUSIE', 'TALIA',
            'DANA', 'DARCY', 'GILBERT', 'JONATHAN', 'KIYO', 'MAKAYLA', 'PAXTON', 'RONALD',
            'BECK', 'CONOR', 'DEBRA', 'FRANKLIN', 'LINDA', 'MOLLIE', 'WARREN',
            'AARON', 'ALEX', 'HITOSHI', 'HUDSON', 'JACK', 'KELVIN', 'LAUREL', 'MARLEY', 'REYNA']:
    AUTO[tid] = (81, 68, 72)
for tid in ['HALLE', 'MICHELLE', 'MITCHELL', 'SAMUEL', 'SHANNON', 'ALBERT', 'EDGAR', 'HOPE', 'KATELYNN', 'QUINCY',
            'CAROLINE', 'DIANNE', 'FELIX', 'JULIE', 'OWEN', 'VITO']:
    AUTO[tid] = (85, 76, 80, 'elite')  # Route Victoire

# Évolutions par niveau (espèce -> (niveau, évolution)), utilisées quand un plancher fait monter le niveau.
EVOLVE = {
    'Numel': (33, 'Camerupt'), 'Geodude': (25, 'Graveler'), 'Sandshrew': (22, 'Sandslash'), 'Baltoy': (36, 'Claydol'),
    'Shroomish': (23, 'Breloom'), 'Marill': (18, 'Azumarill'), 'Taillow': (22, 'Swellow'), 'Slugma': (38, 'Magcargo'),
    'Aron': (32, 'Lairon'), 'Lairon': (42, 'Aggron'), 'Trapinch': (35, 'Vibrava'), 'Vibrava': (45, 'Flygon'),
    'Koffing': (35, 'Weezing'), 'Wingull': (25, 'Pelipper'), 'Barboach': (30, 'Whiscash'), 'Magikarp': (20, 'Gyarados'),
    'Goldeen': (33, 'Seaking'), 'Electrike': (26, 'Manectric'), 'Makuhita': (24, 'Hariyama'), 'Meditite': (37, 'Medicham'),
    'Zubat': (22, 'Golbat'), 'Poochyena': (18, 'Mightyena'), 'Swablu': (35, 'Altaria'), 'Oddish': (21, 'Gloom'),
    'Magnemite': (30, 'Magneton'), 'Zigzagoon': (20, 'Linoone'), 'Machop': (28, 'Machoke'), 'Cacnea': (32, 'Cacturne'),
    'Hippopotas': (34, 'Hippowdon'), 'Diglett': (26, 'Dugtrio'), 'Phanpy': (25, 'Donphan'), 'Houndour': (24, 'Houndoom'),
    'Ponyta': (40, 'Rapidash'), 'Magby': (30, 'Magmar'), 'Litleo': (35, 'Pyroar'), 'Nosepass': (40, 'Probopass'),
    'Rhyhorn': (42, 'Rhydon'), 'Roggenrola': (25, 'Boldore'), 'Larvitar': (30, 'Pupitar'), 'Natu': (25, 'Xatu'),
    'Hoothoot': (20, 'Noctowl'), 'Starly': (14, 'Staravia'), 'Staravia': (34, 'Staraptor'), 'Doduo': (31, 'Dodrio'),
    'Grimer': (38, 'Muk'), 'Ekans': (22, 'Arbok'), 'Gulpin': (26, 'Swalot'), 'Skorupi': (40, 'Drapion'),
    'Stunky': (34, 'Skuntank'), 'Kirlia': (30, 'Gardevoir'), 'Chinchou': (27, 'Lanturn'), 'Snubbull': (23, 'Granbull'),
    'Nincada': (20, 'Ninjask'), 'Spoink': (32, 'Grumpig'), 'Wailmer': (40, 'Wailord'), 'Lotad': (14, 'Lombre'),
    'Seedot': (14, 'Nuzleaf'), 'Ralts': (20, 'Kirlia'), 'Skitty': (40, 'Delcatty'),
    'Azurill': (18, 'Marill'), 'Whismur': (20, 'Loudred'), 'Loudred': (40, 'Exploud'), 'Teddiursa': (30, 'Ursaring'),
    'Budew': (18, 'Roselia'), 'Bellsprout': (21, 'Weepinbell'), 'Paras': (24, 'Parasect'), 'Venonat': (31, 'Venomoth'),
    'Abra': (16, 'Kadabra'), 'Drowzee': (26, 'Hypno'), 'Slowpoke': (37, 'Slowbro'), 'Voltorb': (30, 'Electrode'),
    'Shinx': (15, 'Luxio'), 'Mareep': (15, 'Flaaffy'), 'Flaaffy': (30, 'Ampharos'), 'Blitzle': (27, 'Zebstrika'),
    'Pidgey': (18, 'Pidgeotto'), 'Tentacool': (30, 'Tentacruel'), 'Horsea': (32, 'Seadra'), 'Psyduck': (33, 'Golduck'),
    'Krabby': (28, 'Kingler'), 'Poliwag': (25, 'Poliwhirl'), 'Buizel': (26, 'Floatzel'), 'Remoraid': (25, 'Octillery'),
    'Mankey': (28, 'Primeape'), 'Croagunk': (37, 'Toxicroak'), 'Timburr': (25, 'Gurdurr'), 'Riolu': (20, 'Lucario'),
    'Tyrogue': (20, 'Hitmontop'), 'Ninjask': (99, 'Ninjask'), 'Machoke': (40, 'Machamp'),
    # évolutions par pierre / échange / amitié : niveau arbitraire (40) pour les dresseurs
    'Carvanha': (30, 'Sharpedo'), 'Staryu': (35, 'Starmie'), 'Kadabra': (40, 'Alakazam'), 'Graveler': (40, 'Golem'),
    'Slakoth': (18, 'Vigoroth'), 'Vigoroth': (36, 'Slaking'), 'Sentret': (15, 'Furret'), 'Bidoof': (15, 'Bibarel'),
    'Buneary': (30, 'Lopunny'), 'Porygon': (40, 'Porygon2'), 'Gloom': (40, 'Vileplume'), 'Weepinbell': (40, 'Victreebel'),
    'Lombre': (40, 'Ludicolo'), 'Nuzleaf': (40, 'Shiftry'), 'Poliwhirl': (40, 'Poliwrath'), 'Pupitar': (55, 'Tyranitar'),
    'Golbat': (40, 'Crobat'), 'Shellder': (35, 'Cloyster'), 'Seadra': (45, 'Kingdra'), 'Boldore': (40, 'Gigalith'),
    'Murkrow': (40, 'Honchkrow'), 'Sneasel': (40, 'Weavile'), 'Eevee': (40, 'Espeon'), 'Gligar': (40, 'Gliscor'),
    'Roselia': (40, 'Roserade'), 'Onix': (40, 'Steelix'), 'Rhydon': (55, 'Rhyperior'), 'Pikachu': (40, 'Raichu'),
    'Vulpix': (40, 'Ninetales'), 'Growlithe': (40, 'Arcanine'), 'Snorunt': (42, 'Glalie'), 'Combee': (21, 'Vespiquen'),
    'Wurmple': (7, 'Silcoon'), 'Silcoon': (10, 'Beautifly'), 'Surskit': (22, 'Masquerain'),
    'Shuppet': (37, 'Banette'), 'Duskull': (37, 'Dusclops'), 'Dusclops': (55, 'Dusknoir'), 'Misdreavus': (40, 'Mismagius'),
    'Gastly': (25, 'Haunter'), 'Haunter': (40, 'Gengar'), 'Litwick': (41, 'Lampent'), 'Lampent': (45, 'Chandelure'),
    'Drifloon': (28, 'Drifblim'), 'Phantump': (40, 'Trevenant'), 'Corphish': (30, 'Crawdaunt'), 'Spheal': (32, 'Sealeo'),
    'Sealeo': (44, 'Walrein'), 'Munna': (40, 'Musharna'),
    'Hoppip': (18, 'Skiploom'), 'Skiploom': (27, 'Jumpluff'),
}


def evolve(species, level):
    while species in EVOLVE and level >= EVOLVE[species][0] and EVOLVE[species][1] != species:
        species = EVOLVE[species][1]
    return species


def auto_teams(tid, block, rule):
    rule = rule if isinstance(rule, tuple) else (rule, 0, 0)
    cap, floor_h, floor_k = rule[:3]
    pool_name = rule[3] if len(rule) > 3 else None
    floors = {'Hard': floor_h, 'Kaizo': floor_k}
    return _auto_teams(tid, block, cap, floors, pool_name)


def _auto_teams(tid, block, cap, floors, pool_name=None):
    """Équipe Hard / Kaizo d'un dresseur ordinaire à partir de son équipe Normal."""
    import random
    cls = re.search(r'(?m)^Class: (.*)$', block).group(1)
    pool = POOLS[pool_name or CLASS_POOL.get(cls, 'mixte')]
    mons = re.findall(r'(?m)^([A-Z][A-Za-z\.\' -]+?)(?: @ .*)?\nLevel: (\d+)', block)
    rng = random.Random('couronnement:' + tid)
    top = max(int(l) for _, l in mons)
    teams = {}
    for diff, bonus, extra in (('Hard', 3, 1), ('Kaizo', 5, 2)):
        floor = floors[diff]
        team = [(sp, min(max(int(l) + bonus, top + bonus - 4, floor), cap)) for sp, l in mons]  # plancher : 4 niveaux sous le meilleur
        names = {sp for sp, _ in team}
        choices = [p for p in pool if p not in names]
        rng.shuffle(choices)
        for sp in choices[:extra]:
            team.append((sp, min(max(top + bonus - 1, floor), cap)))
        team = team[:6]
        if floor:  # chapitre 3 et suivants : faire évoluer ce qui a pris des niveaux
            team = [(evolve(sp, lvl), lvl) for sp, lvl in team]
        specs = []
        for i, (sp, lvl) in enumerate(team):
            item = ' @ Oran Berry' if diff == 'Kaizo' and i == len(team) - 1 else ''
            evs = EVS_KAIZO if diff == 'Kaizo' else ''
            specs.append(f'{sp}{item} | {"Serious" if diff == "Kaizo" else ""} | | {lvl} | {evs} |')
        teams[diff] = specs
    return teams


def main():
    text = PARTY.read_text(encoding='utf-8')
    if BEGIN in text:
        text = text[:text.index(BEGIN)].rstrip('\n') + '\n'
    out = [BEGIN, '']
    all_teams = dict(TEAMS)
    for tid, cap in AUTO.items():
        m = re.search(r'(?ms)^=== TRAINER_' + tid + r' ===\n(.*?)(?=^=== |\Z)', text)
        if not m:
            raise SystemExit(f'Dresseur introuvable : TRAINER_{tid}')
        all_teams[tid] = auto_teams(tid, m.group(1), cap)
    for tid, diffs in all_teams.items():
        m = re.search(r'(?ms)^=== TRAINER_' + tid + r' ===\n(.*?)\n\n', text)
        if not m:
            raise SystemExit(f'Dresseur introuvable : TRAINER_{tid}')
        header = [l for l in m.group(1).split('\n') if not l.startswith(('Difficulty:', 'Items:'))]
        for diff, mons in diffs.items():
            out.append(f'=== TRAINER_{tid} ===')
            ai = {'Hard': 'AI: Basic Trainer', 'Kaizo': 'AI: Smart Trainer'}[diff]  # IA : au moins « Basic » en Hard, « Smart » en Kaizo
            hdr = [(ai if l.startswith('AI:') else l) for l in header]
            out += hdr + [f'Difficulty: {diff}', '']
            out += [mon_block(s, IV[diff]) + '\n' for s in mons]
    out.append(END)
    PARTY.write_text(text.rstrip('\n') + '\n\n' + '\n'.join(out) + '\n', encoding='utf-8')
    print(f'{len(all_teams)} dresseurs : blocs Hard et Kaizo écrits.')


if __name__ == '__main__':
    main()
