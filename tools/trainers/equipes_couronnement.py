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
    'TREECKO': ('Cyndaquil', 'Quilava', 'Blaze', 'Flash Fire', 'Ember, Quick Attack, Smokescreen, Leer', 'Flame Wheel, Quick Attack, Smokescreen, Swift'),
    'TORCHIC': ('Squirtle', 'Wartortle', 'Torrent', 'Rain Dish', 'Water Gun, Tackle, Withdraw, Tail Whip', 'Water Pulse, Bite, Rapid Spin, Protect'),
    'MUDKIP':  ('Treecko', 'Grovyle', 'Overgrow', 'Unburden', 'Absorb, Quick Attack, Leer, Pound', 'Mega Drain, Quick Attack, Pursuit, Fury Cutter'),
}
for rival in ('MAY', 'BRENDAN'):
    for starter, (base, mid, ab, ha, m1, m2) in RIVAL_LINES.items():
        TEAMS[f'{rival}_ROUTE_103_{starter}'] = {
            'Hard': [f"{base} | Hardy | {ab} | 6 | | {m1}"],
            'Kaizo': [f"{base} @ Oran Berry | Timid | {ha} | 7 | 252 Spe / 252 HP | {m1}",
                      "Zigzagoon | Jolly | Pickup | 6 | 252 Atk / 252 Spe | Headbutt, Sand Attack, Tail Whip"],
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


def mon_block(spec, iv):
    parts = [p.strip() for p in spec.split('|')]
    species, nature, ability, level, evs, moves = parts
    lines = [species, f'{nature} Nature', f'Level: {level}', f'Ability: {ability}',
             f'IVs: {iv} HP / {iv} Atk / {iv} Def / {iv} SpA / {iv} SpD / {iv} Spe']
    if evs:
        lines.append(f'EVs: {evs}')
    lines += [f'- {m.strip()}' for m in moves.split(',')]
    return '\n'.join(lines)


def main():
    text = PARTY.read_text(encoding='utf-8')
    if BEGIN in text:
        text = text[:text.index(BEGIN)].rstrip('\n') + '\n'
    out = [BEGIN, '']
    for tid, diffs in TEAMS.items():
        m = re.search(r'(?ms)^=== TRAINER_' + tid + r' ===\n(.*?)\n\n', text)
        if not m:
            raise SystemExit(f'Dresseur introuvable : TRAINER_{tid}')
        header = [l for l in m.group(1).split('\n') if not l.startswith(('Difficulty:', 'Items:'))]
        for diff, mons in diffs.items():
            out.append(f'=== TRAINER_{tid} ===')
            out += header + [f'Difficulty: {diff}', '']
            out += [mon_block(s, IV[diff]) + '\n' for s in mons]
    out.append(END)
    PARTY.write_text(text.rstrip('\n') + '\n\n' + '\n'.join(out) + '\n', encoding='utf-8')
    print(f'{len(TEAMS)} dresseurs : blocs Hard et Kaizo écrits.')


if __name__ == '__main__':
    main()
