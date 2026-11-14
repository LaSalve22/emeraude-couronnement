#!/usr/bin/env python3
"""Adversaires des tournois du Couronnement Mondial.

Génère :
- src/data/couronnement.party : équipes (syntaxe Showdown, compilée par trainerproc comme trainers.party) ;
- include/constants/couronnement_adversaires.h : indices COURONNEMENT_ADV_<n>.

Le jeu n'utilise qu'un seul emplacement de dresseur (TRAINER_COURONNEMENT) : son équipe est prise dans
gCouronnementTrainers[VAR_COURONNEMENT_ADVERSAIRE] (voir src/couronnement.c). Indice = tournoi * 3 + manche.

Format d'un Pokémon : "Espèce @ Objet | Nature | Talent | EVs | Capacités" (talent vide = talent par défaut).
Le niveau vient du tournoi : NIVEAUX[t] + manche. Tous les Pokémon ont des IV à 31.

Usage : python3 tools/couronnement/tournois.py
"""
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]

# Tournois 0-7 (villes) : accessibles après le badge t+1. Niveaux = cap du badge suivant - 3, +1 par manche.
NIVEAUX = [24, 33, 42, 51, 60, 68, 75, 82]

# (nom, classe, sprite, genre, [6 Pokémon]) ; les 4 premiers sont l'équipe historique, 2 ajoutés par EXTRA.
TOURNOIS = [
    # 0 — Mérouville (Roche)
    [
        ('Hugo', 'Camper', 'Camper', 'Male', [
            "Graveler @ Sitrus Berry | Adamant | Sturdy | 252 Atk / 252 HP | Rock Slide, Bulldoze, Protect, Rock Polish",
            "Nuzleaf | Jolly | | 252 Atk / 252 Spe | Fake Out, Razor Leaf, Feint Attack, Protect",
            "Pelipper | Modest | Drizzle | 252 SpA / 252 HP | Water Pulse, Wing Attack, Protect, Mist",
            "Linoone | Jolly | Pickup | 252 Atk / 252 Spe | Headbutt, Covet, Sand Attack, Protect"]),
        ('Inès', 'Picnicker', 'Picnicker', 'Female', [
            "Kirlia @ Oran Berry | Modest | Trace | 252 SpA / 252 Spe | Draining Kiss, Confusion, Helping Hand, Protect",
            "Breloom | Adamant | Poison Heal | 252 Atk / 252 Spe | Mach Punch, Bullet Seed, Headbutt, Protect",
            "Swellow | Jolly | Guts | 252 Atk / 252 Spe | Wing Attack, Quick Attack, Endeavor, Protect",
            "Aron | Adamant | Sturdy | 252 Atk / 252 HP | Metal Claw, Headbutt, Rock Tomb, Protect"]),
        ('Malik', 'Cooltrainer', 'Cooltrainer M', 'Male', [
            "Lucario @ Oran Berry | Jolly | Inner Focus | 252 Atk / 252 Spe | Force Palm, Metal Claw, Quick Attack, Protect",
            "Lombre | Modest | Swift Swim | 252 SpA / 252 Spe | Fake Out, Water Pulse, Mega Drain, Protect",
            "Vigoroth | Jolly | Vital Spirit | 252 Atk / 252 Spe | Slash, Encore, Bulk Up, Protect",
            "Graveler @ Sitrus Berry | Adamant | Sturdy | 252 Atk / 252 HP | Rock Slide, Bulldoze, Rock Polish, Protect"]),
    ],
    # 1 — Village Myokara (Combat)
    [
        ('Noa', 'Battle Girl', 'Battle Girl', 'Female', [
            "Machoke @ Sitrus Berry | Adamant | Guts | 252 Atk / 252 HP | Brick Break, Rock Slide, Bulk Up, Protect",
            "Meditite | Jolly | Pure Power | 252 Atk / 252 Spe | Fake Out, Hi Jump Kick, Zen Headbutt, Detect",
            "Hariyama | Adamant | Thick Fat | 252 Atk / 252 HP | Fake Out, Force Palm, Knock Off, Protect",
            "Pelipper | Modest | Drizzle | 252 SpA / 252 HP | Water Pulse, Wing Attack, Tailwind, Protect"]),
        ('Sacha', 'Black Belt', 'Black Belt', 'Male', [
            "Hitmontop @ Sitrus Berry | Adamant | Intimidate | 252 Atk / 252 HP | Fake Out, Brick Break, Rock Slide, Protect",
            "Breloom | Jolly | Technician | 252 Atk / 252 Spe | Mach Punch, Bullet Seed, Rock Tomb, Protect",
            "Primeape | Jolly | Defiant | 252 Atk / 252 Spe | Karate Chop, Rock Slide, Taunt, Protect",
            "Golbat | Jolly | Inner Focus | 252 Atk / 252 Spe | Air Cutter, Bite, Confuse Ray, Protect"]),
        ('Kenji', 'Expert', 'Expert M', 'Male', [
            "Lucario @ Life Orb | Timid | Inner Focus | 252 SpA / 252 Spe | Aura Sphere, Flash Cannon, Quick Attack, Protect",
            "Medicham | Jolly | Pure Power | 252 Atk / 252 Spe | Fake Out, Hi Jump Kick, Zen Headbutt, Detect",
            "Machoke | Adamant | Guts | 252 Atk / 252 HP | Brick Break, Rock Slide, Bulk Up, Protect",
            "Croagunk @ Sitrus Berry | Adamant | Dry Skin | 252 Atk / 252 Spe | Poison Jab, Revenge, Sucker Punch, Protect"]),
    ],
    # 2 — Lavandia (Électrik)
    [
        ('Bastian', 'Guitarist', 'Guitarist', 'Male', [
            "Manectric @ Sitrus Berry | Timid | Lightning Rod | 252 SpA / 252 Spe | Discharge, Bite, Thunder Wave, Protect",
            "Magneton | Modest | Sturdy | 252 SpA / 252 HP | Discharge, Flash Cannon, Thunder Wave, Protect",
            "Gyarados | Adamant | Intimidate | 252 Atk / 252 Spe | Waterfall, Bite, Dragon Dance, Protect",
            "Lanturn | Modest | Volt Absorb | 252 SpA / 252 HP | Discharge, Surf, Thunder Wave, Protect"]),
        ('Clara', 'Cooltrainer', 'Cooltrainer F', 'Female', [
            "Raichu @ Focus Sash | Timid | Static | 252 SpA / 252 Spe | Fake Out, Thunderbolt, Volt Switch, Protect",
            "Ampharos | Modest | Static | 252 SpA / 252 HP | Discharge, Power Gem, Thunder Wave, Protect",
            "Breloom | Jolly | Technician | 252 Atk / 252 Spe | Mach Punch, Bullet Seed, Rock Tomb, Protect",
            "Golduck @ Sitrus Berry | Modest | Cloud Nine | 252 SpA / 252 Spe | Surf, Ice Beam, Psych Up, Protect"]),
        ('Damien', 'Expert', 'Expert M', 'Male', [
            "Electrode @ Focus Sash | Timid | Soundproof | 252 SpA / 252 Spe | Thunderbolt, Taunt, Explosion, Protect",
            "Luxray | Adamant | Intimidate | 252 Atk / 252 Spe | Wild Charge, Crunch, Fire Fang, Protect",
            "Jolteon @ Life Orb | Timid | Volt Absorb | 252 SpA / 252 Spe | Thunderbolt, Shadow Ball, Helping Hand, Protect",
            "Gardevoir @ Sitrus Berry | Modest | Trace | 252 SpA / 252 Spe | Psychic, Dazzling Gleam, Helping Hand, Protect"]),
    ],
    # 3 — Vermilava (Feu)
    [
        ('Pyra', 'Kindler', 'Kindler', 'Male', [
            "Torkoal @ Charcoal | Quiet | Drought | 252 SpA / 252 HP | Heat Wave, Earth Power, Yawn, Protect",
            "Vileplume | Modest | Chlorophyll | 252 SpA / 252 Spe | Sleep Powder, Giga Drain, Sludge Bomb, Protect",
            "Camerupt @ Sitrus Berry | Modest | Solid Rock | 252 SpA / 252 HP | Heat Wave, Earth Power, Rock Slide, Protect",
            "Ninetales | Timid | Flash Fire | 252 SpA / 252 Spe | Heat Wave, Solar Beam, Will-O-Wisp, Protect"]),
        ('Soline', 'Aroma Lady', 'Aroma Lady', 'Female', [
            "Shiftry @ Focus Sash | Jolly | Chlorophyll | 252 Atk / 252 Spe | Fake Out, Leaf Blade, Sucker Punch, Protect",
            "Arcanine @ Sitrus Berry | Adamant | Intimidate | 252 Atk / 252 Spe | Flare Blitz, Extreme Speed, Snarl, Protect",
            "Houndoom @ Life Orb | Timid | Flash Fire | 252 SpA / 252 Spe | Heat Wave, Dark Pulse, Nasty Plot, Protect",
            "Claydol | Bold | Levitate | 252 HP / 252 Def | Earth Power, Psychic, Trick Room, Protect"]),
        ('Magnus', 'Kindler', 'Kindler', 'Male', [
            "Magmar @ Life Orb | Modest | Flame Body | 252 SpA / 252 Spe | Heat Wave, Thunder Punch, Clear Smog, Protect",
            "Torkoal @ Sitrus Berry | Quiet | Drought | 252 SpA / 252 HP | Heat Wave, Earth Power, Yawn, Protect",
            "Rhydon | Adamant | Lightning Rod | 252 Atk / 252 HP | Rock Slide, Drill Run, Hammer Arm, Protect",
            "Altaria | Modest | Natural Cure | 252 SpA / 252 Spe | Dragon Pulse, Dazzling Gleam, Roost, Protect"]),
    ],
    # 4 — Clémenti-Ville (Normal)
    [
        ('Cyril', 'Gentleman', 'Gentleman', 'Male', [
            "Kangaskhan @ Silk Scarf | Adamant | Scrappy | 252 Atk / 252 Spe | Fake Out, Double-Edge, Sucker Punch, Protect",
            "Ursaring @ Flame Orb | Adamant | Guts | 252 Atk / 252 HP | Facade, Close Combat, Crunch, Protect",
            "Porygon2 @ Eviolite | Quiet | Download | 252 HP / 252 SpA | Trick Room, Tri Attack, Ice Beam, Recover",
            "Exploud | Modest | Scrappy | 252 SpA / 252 HP | Hyper Voice, Fire Blast, Surf, Protect"]),
        ('Lily', 'Lady', 'Lady', 'Female', [
            "Lopunny @ Sitrus Berry | Jolly | Limber | 252 Atk / 252 Spe | Fake Out, Return, Ice Punch, Protect",
            "Persian | Jolly | Technician | 252 Atk / 252 Spe | Fake Out, U-turn, Feint, Hypnosis",
            "Zangoose @ Life Orb | Jolly | Toxic Boost | 252 Atk / 252 Spe | Close Combat, Quick Attack, Crush Claw, Protect",
            "Snorlax @ Leftovers | Adamant | Thick Fat | 252 HP / 252 Atk | Body Slam, Curse, Rest, Protect"]),
        ('Orson', 'Expert', 'Expert M', 'Male', [
            "Slaking @ Choice Band | Adamant | Truant | 252 Atk / 252 Spe | Double-Edge, Earthquake, Shadow Claw, Protect",
            "Staraptor @ Focus Sash | Jolly | Intimidate | 252 Atk / 252 Spe | Brave Bird, Close Combat, Quick Attack, Protect",
            "Tauros | Jolly | Intimidate | 252 Atk / 252 Spe | Double-Edge, Rock Slide, Zen Headbutt, Protect",
            "Chansey @ Eviolite | Bold | Natural Cure | 252 HP / 252 Def | Soft-Boiled, Seismic Toss, Helping Hand, Protect"]),
    ],
    # 5 — Cimetronelle (Vol)
    [
        ('Faustin', 'Bird Keeper', 'Bird Keeper', 'Male', [
            "Pelipper @ Damp Rock | Modest | Drizzle | 252 SpA / 252 HP | Hurricane, Surf, Tailwind, Protect",
            "Swellow @ Flame Orb | Jolly | Guts | 252 Atk / 252 Spe | Brave Bird, Facade, Quick Attack, Protect",
            "Skarmory | Impish | Sturdy | 252 HP / 252 Def | Iron Head, Tailwind, Roost, Protect",
            "Ludicolo @ Life Orb | Modest | Swift Swim | 252 SpA / 252 Spe | Fake Out, Hydro Pump, Giga Drain, Protect"]),
        ('Aria', 'Cooltrainer', 'Cooltrainer F', 'Female', [
            "Togekiss @ Sitrus Berry | Timid | Serene Grace | 252 SpA / 252 Spe | Follow Me, Air Slash, Dazzling Gleam, Protect",
            "Gyarados | Adamant | Intimidate | 252 Atk / 252 Spe | Waterfall, Ice Fang, Dragon Dance, Protect",
            "Staraptor @ Choice Band | Jolly | Intimidate | 252 Atk / 252 Spe | Brave Bird, Close Combat, Quick Attack, U-turn",
            "Gardevoir @ Life Orb | Modest | Trace | 252 SpA / 252 Spe | Psychic, Dazzling Gleam, Trick Room, Protect"]),
        ('Céleste', 'Expert', 'Expert F', 'Female', [
            "Noivern @ Life Orb | Timid | Infiltrator | 252 SpA / 252 Spe | Boomburst, Hurricane, Tailwind, Protect",
            "Talonflame @ Sitrus Berry | Adamant | Gale Wings | 252 Atk / 252 Spe | Brave Bird, Flare Blitz, Tailwind, Protect",
            "Crobat | Jolly | Inner Focus | 252 Atk / 252 Spe | Cross Poison, Super Fang, Taunt, Protect",
            "Corviknight @ Leftovers | Impish | Mirror Armor | 252 HP / 252 Def | Brave Bird, Body Press, Tailwind, Roost"]),
    ],
    # 6 — Algatia (Psy)
    [
        ('Iris', 'Psychic', 'Psychic F', 'Female', [
            "Gardevoir @ Sitrus Berry | Quiet | Trace | 252 HP / 252 SpA | Psychic, Dazzling Gleam, Trick Room, Protect",
            "Bronzong @ Leftovers | Relaxed | Levitate | 252 HP / 252 Def | Gyro Ball, Trick Room, Body Press, Protect",
            "Hariyama @ Assault Vest | Brave | Guts | 252 HP / 252 Atk | Fake Out, Close Combat, Knock Off, Heavy Slam",
            "Camerupt @ Choice Specs | Quiet | Solid Rock | 252 HP / 252 SpA | Eruption, Earth Power, Heat Wave, Ancient Power"]),
        ('Orion', 'Psychic', 'Psychic M', 'Male', [
            "Alakazam @ Focus Sash | Timid | Inner Focus | 252 SpA / 252 Spe | Psychic, Shadow Ball, Focus Blast, Protect",
            "Xatu | Timid | Magic Bounce | 252 SpA / 252 Spe | Tailwind, Psychic, Heat Wave, Protect",
            "Solrock @ Sitrus Berry | Brave | Levitate | 252 HP / 252 Atk | Rock Slide, Flare Blitz, Trick Room, Protect",
            "Lunatone @ Life Orb | Quiet | Levitate | 252 HP / 252 SpA | Moonblast, Psychic, Ice Beam, Protect"]),
        ('Shiro', 'Expert', 'Expert M', 'Male', [
            "Medicham @ Focus Sash | Jolly | Pure Power | 252 Atk / 252 Spe | Fake Out, Hi Jump Kick, Zen Headbutt, Detect",
            "Reuniclus @ Life Orb | Quiet | Magic Guard | 252 HP / 252 SpA | Trick Room, Psychic, Focus Blast, Protect",
            "Grumpig | Modest | Thick Fat | 252 SpA / 252 HP | Psychic, Shadow Ball, Power Gem, Protect",
            "Gallade @ Sitrus Berry | Adamant | Justified | 252 Atk / 252 Spe | Close Combat, Psycho Cut, Leaf Blade, Protect"]),
    ],
    # 7 — Atalanopolis (Eau)
    [
        ('Narcisse', 'Swimmer M', 'Swimmer M', 'Male', [
            "Pelipper @ Damp Rock | Modest | Drizzle | 252 SpA / 252 HP | Hurricane, Scald, Tailwind, Protect",
            "Kingdra @ Life Orb | Modest | Swift Swim | 252 SpA / 252 Spe | Muddy Water, Draco Meteor, Ice Beam, Protect",
            "Milotic @ Leftovers | Bold | Competitive | 252 HP / 252 Def | Scald, Ice Beam, Recover, Protect",
            "Ludicolo @ Sitrus Berry | Modest | Swift Swim | 252 SpA / 252 Spe | Fake Out, Hydro Pump, Giga Drain, Protect"]),
        ('Maëlle', 'Cooltrainer', 'Cooltrainer F', 'Female', [
            "Gyarados @ Sitrus Berry | Jolly | Intimidate | 252 Atk / 252 Spe | Waterfall, Ice Fang, Dragon Dance, Protect",
            "Lapras @ Leftovers | Modest | Water Absorb | 252 HP / 252 SpA | Freeze-Dry, Hydro Pump, Ice Shard, Protect",
            "Whiscash | Adamant | Oblivious | 252 Atk / 252 HP | Earthquake, Waterfall, Rock Slide, Protect",
            "Lanturn @ Assault Vest | Modest | Volt Absorb | 252 HP / 252 SpA | Discharge, Scald, Ice Beam, Thunder Wave"]),
        ('Néréis', 'Expert', 'Expert F', 'Female', [
            "Politoed @ Damp Rock | Modest | Drizzle | 252 HP / 252 SpA | Hydro Pump, Ice Beam, Helping Hand, Protect",
            "Kingdra @ Life Orb | Modest | Swift Swim | 252 SpA / 252 Spe | Muddy Water, Draco Meteor, Ice Beam, Protect",
            "Swampert @ Sitrus Berry | Adamant | Damp | 252 Atk / 252 HP | Earthquake, Waterfall, Ice Punch, Protect",
            "Sharpedo @ Focus Sash | Adamant | Speed Boost | 252 Atk / 252 Spe | Liquidation, Crunch, Ice Fang, Protect"]),
    ],
]


# Deux Pokémon de plus par adversaire des villes : 6 montrés, l'IA en garde 4 (src/couronnement.c).
EXTRA = [
    [["Mightyena | Jolly | Intimidate | 252 Atk / 252 Spe | Bite, Headbutt, Swagger, Protect",
      "Lombre | Modest | Swift Swim | 252 SpA / 252 Spe | Fake Out, Water Pulse, Mega Drain, Protect"],
     ["Marill @ Oran Berry | Adamant | Huge Power | 252 Atk / 252 HP | Aqua Jet, Rollout, Bubble Beam, Protect",
      "Electrike | Timid | Lightning Rod | 252 SpA / 252 Spe | Shock Wave, Quick Attack, Bite, Protect"],
     ["Nosepass | Bold | Sturdy | 252 HP / 252 Def | Rock Tomb, Thunder Wave, Block, Protect",
      "Wingull | Timid | Keen Eye | 252 SpA / 252 Spe | Wing Attack, Water Pulse, Quick Attack, Protect"]],
    [["Makuhita | Adamant | Thick Fat | 252 Atk / 252 HP | Fake Out, Force Palm, Knock Off, Protect",
      "Sableye | Careful | Prankster | 252 HP / 252 SpD | Fake Out, Shadow Sneak, Knock Off, Protect"],
     ["Hitmonlee | Jolly | Limber | 252 Atk / 252 Spe | High Jump Kick, Rock Slide, Fake Out, Detect",
      "Kirlia | Modest | Trace | 252 SpA / 252 Spe | Psybeam, Draining Kiss, Helping Hand, Protect"],
     ["Pelipper | Modest | Drizzle | 252 SpA / 252 HP | Water Pulse, Air Cutter, Tailwind, Protect",
      "Mawile @ Oran Berry | Adamant | Intimidate | 252 Atk / 252 HP | Iron Head, Play Rough, Sucker Punch, Protect"]],
    [["Magnemite | Modest | Sturdy | 252 SpA / 252 Spe | Spark, Flash Cannon, Thunder Wave, Protect",
      "Marill | Adamant | Huge Power | 252 Atk / 252 HP | Aqua Jet, Play Rough, Rollout, Protect"],
     ["Minun | Timid | Minus | 252 SpA / 252 Spe | Discharge, Helping Hand, Encore, Protect",
      "Plusle | Timid | Plus | 252 SpA / 252 Spe | Discharge, Helping Hand, Nasty Plot, Protect"],
     ["Swellow | Jolly | Guts | 252 Atk / 252 Spe | Aerial Ace, Quick Attack, Facade, Protect",
      "Graveler | Adamant | Sturdy | 252 Atk / 252 HP | Rock Slide, Bulldoze, Rock Polish, Protect"]],
    [["Slugma | Bold | Flame Body | 252 HP / 252 Def | Flamethrower, Rock Slide, Yawn, Protect",
      "Grumpig | Modest | Thick Fat | 252 SpA / 252 HP | Psychic, Power Gem, Trick Room, Protect"],
     ["Whiscash | Adamant | Oblivious | 252 Atk / 252 HP | Earthquake, Waterfall, Rock Slide, Protect",
      "Breloom | Jolly | Technician | 252 Atk / 252 Spe | Mach Punch, Bullet Seed, Rock Tomb, Protect"],
     ["Flygon @ Sitrus Berry | Jolly | Levitate | 252 Atk / 252 Spe | Earthquake, Dragon Claw, Rock Slide, Protect",
      "Pelipper | Modest | Drizzle | 252 SpA / 252 HP | Scald, Hurricane, Tailwind, Protect"]],
    [["Blissey | Bold | Natural Cure | 252 HP / 252 Def | Soft-Boiled, Seismic Toss, Helping Hand, Protect",
      "Clefable | Calm | Magic Guard | 252 HP / 252 SpD | Follow Me, Moonblast, Helping Hand, Protect"],
     ["Ambipom | Jolly | Technician | 252 Atk / 252 Spe | Fake Out, Double Hit, U-turn, Protect",
      "Gardevoir | Modest | Trace | 252 SpA / 252 Spe | Psychic, Moonblast, Trick Room, Protect"],
     ["Bewear @ Sitrus Berry | Adamant | Fluffy | 252 Atk / 252 HP | Close Combat, Double-Edge, Drain Punch, Protect",
      "Indeedee-F | Quiet | Psychic Surge | 252 HP / 252 SpA | Follow Me, Expanding Force, Helping Hand, Protect"]],
    [["Altaria | Adamant | Natural Cure | 252 Atk / 252 Spe | Dragon Claw, Earthquake, Dragon Dance, Protect",
      "Tropius | Modest | Chlorophyll | 252 SpA / 252 HP | Air Slash, Giga Drain, Tailwind, Protect"],
     ["Salamence @ Life Orb | Naive | Intimidate | 252 Atk / 252 Spe | Draco Meteor, Dragon Claw, Fire Blast, Protect",
      "Rotom-Wash @ Sitrus Berry | Modest | Levitate | 252 HP / 252 SpA | Hydro Pump, Thunderbolt, Will-O-Wisp, Protect"],
     ["Whimsicott @ Focus Sash | Timid | Prankster | 252 HP / 252 Spe | Tailwind, Moonblast, Encore, Protect",
      "Gyarados @ Sitrus Berry | Jolly | Intimidate | 252 Atk / 252 Spe | Waterfall, Ice Fang, Dragon Dance, Protect"]],
    [["Ursaring | Adamant | Guts | 252 Atk / 252 HP | Close Combat, Facade, Crunch, Protect",
      "Torkoal | Quiet | Drought | 252 HP / 252 SpA | Eruption, Earth Power, Heat Wave, Protect"],
     ["Metagross @ Sitrus Berry | Adamant | Clear Body | 252 Atk / 252 HP | Iron Head, Zen Headbutt, Bullet Punch, Protect",
      "Gastrodon | Calm | Storm Drain | 252 HP / 252 SpD | Earth Power, Scald, Recover, Protect"],
     ["Hatterene @ Life Orb | Quiet | Magic Bounce | 252 HP / 252 SpA | Dazzling Gleam, Psychic, Trick Room, Protect",
      "Conkeldurr @ Flame Orb | Brave | Guts | 252 HP / 252 Atk | Mach Punch, Drain Punch, Knock Off, Protect"]],
    [["Pelipper @ Sitrus Berry | Modest | Drizzle | 252 HP / 252 SpA | Hurricane, Scald, Wide Guard, Protect",
      "Starmie @ Life Orb | Timid | Analytic | 252 SpA / 252 Spe | Hydro Pump, Psychic, Thunderbolt, Protect"],
     ["Walrein @ Leftovers | Modest | Thick Fat | 252 HP / 252 SpA | Blizzard, Surf, Protect, Icy Wind",
      "Rotom-Wash | Modest | Levitate | 252 HP / 252 SpA | Hydro Pump, Thunderbolt, Will-O-Wisp, Protect"],
     ["Pelipper | Modest | Drizzle | 252 HP / 252 SpA | Hurricane, Scald, Tailwind, Protect",
      "Toxapex @ Black Sludge | Bold | Regenerator | 252 HP / 252 Def | Scald, Toxic, Baneful Bunker, Recover"]],
]
for t, extras in enumerate(EXTRA):
    for r, mons in enumerate(extras):
        TOURNOIS[t][r][4].extend(mons)

# Salles de Concours d'Émeraude (Tentes de Combat de Poivressel, Vergazon, Autéquia ; salle de Nénucrique).
NIVEAUX += [31, 40, 49, 66]
TOURNOIS += [
    # 8 — Poivressel (Sang-froid)
    [
        ('Marin', 'Sailor', 'Sailor', 'Male', [
            "Machoke @ Sitrus Berry | Adamant | Guts | 252 Atk / 252 HP | Brick Break, Rock Slide, Bulk Up, Protect",
            "Wingull | Timid | Keen Eye | 252 SpA / 252 Spe | Water Pulse, Air Cutter, Tailwind, Protect",
            "Carvanha | Adamant | Rough Skin | 252 Atk / 252 Spe | Bite, Aqua Jet, Ice Fang, Protect",
            "Makuhita | Adamant | Thick Fat | 252 Atk / 252 HP | Fake Out, Force Palm, Knock Off, Protect",
            "Electrike | Timid | Lightning Rod | 252 SpA / 252 Spe | Shock Wave, Bite, Thunder Wave, Protect",
            "Zigzagoon | Jolly | Pickup | 252 Atk / 252 Spe | Headbutt, Covet, Follow Me, Protect"]),
        ('Yuna', 'Cooltrainer', 'Cooltrainer F', 'Female', [
            "Absol @ Oran Berry | Jolly | Super Luck | 252 Atk / 252 Spe | Night Slash, Psycho Cut, Quick Attack, Protect",
            "Kirlia | Modest | Trace | 252 SpA / 252 Spe | Psybeam, Draining Kiss, Helping Hand, Protect",
            "Combusken | Adamant | Speed Boost | 252 Atk / 252 Spe | Double Kick, Flame Charge, Quick Attack, Protect",
            "Pelipper | Modest | Drizzle | 252 SpA / 252 HP | Water Pulse, Air Cutter, Tailwind, Protect",
            "Lairon | Adamant | Sturdy | 252 Atk / 252 HP | Iron Head, Rock Tomb, Headbutt, Protect",
            "Grovyle | Jolly | Unburden | 252 Atk / 252 Spe | Leaf Blade, Quick Attack, Pursuit, Protect"]),
        ('Raphaël', 'Expert', 'Expert M', 'Male', [
            "Medicham @ Sitrus Berry | Jolly | Pure Power | 252 Atk / 252 Spe | Fake Out, Brick Break, Zen Headbutt, Detect",
            "Swellow | Jolly | Guts | 252 Atk / 252 Spe | Aerial Ace, Quick Attack, Facade, Protect",
            "Manectric | Timid | Lightning Rod | 252 SpA / 252 Spe | Discharge, Bite, Thunder Wave, Protect",
            "Marshtomp | Adamant | Damp | 252 Atk / 252 HP | Mud Shot, Rock Slide, Water Pulse, Protect",
            "Mawile @ Oran Berry | Adamant | Intimidate | 252 Atk / 252 HP | Iron Head, Play Rough, Sucker Punch, Protect",
            "Breloom | Jolly | Technician | 252 Atk / 252 Spe | Mach Punch, Bullet Seed, Rock Tomb, Protect"]),
    ],
    # 9 — Vergazon (Grâce)
    [
        ('Lou', 'Lass', 'Lass', 'Female', [
            "Azumarill @ Sitrus Berry | Adamant | Huge Power | 252 Atk / 252 HP | Aqua Jet, Play Rough, Rollout, Protect",
            "Skitty | Jolly | Normalize | 252 Atk / 252 Spe | Fake Out, Double Slap, Charm, Protect",
            "Plusle | Timid | Plus | 252 SpA / 252 Spe | Discharge, Helping Hand, Nasty Plot, Protect",
            "Minun | Timid | Minus | 252 SpA / 252 Spe | Discharge, Helping Hand, Encore, Protect",
            "Jigglypuff | Calm | Friend Guard | 252 HP / 252 SpD | Follow Me, Disarming Voice, Sing, Protect",
            "Roselia | Timid | Natural Cure | 252 SpA / 252 Spe | Giga Drain, Sludge Bomb, Stun Spore, Protect"]),
        ('Prune', 'Pkmn Breeder', 'Pokemon Breeder F', 'Female', [
            "Clefairy @ Eviolite | Bold | Friend Guard | 252 HP / 252 Def | Follow Me, Moonblast, Helping Hand, Protect",
            "Vulpix | Timid | Drought | 252 SpA / 252 Spe | Flamethrower, Extrasensory, Will-O-Wisp, Protect",
            "Gloom | Modest | Chlorophyll | 252 SpA / 252 Spe | Giga Drain, Sludge Bomb, Sleep Powder, Protect",
            "Pikachu @ Light Ball | Timid | Lightning Rod | 252 SpA / 252 Spe | Fake Out, Thunderbolt, Grass Knot, Protect",
            "Sylveon | Modest | Pixilate | 252 SpA / 252 HP | Hyper Voice, Quick Attack, Yawn, Protect",
            "Togetic | Bold | Serene Grace | 252 HP / 252 Def | Follow Me, Air Slash, Dazzling Gleam, Protect"]),
        ('Amandine', 'Beauty', 'Beauty', 'Female', [
            "Delcatty @ Sitrus Berry | Jolly | Wonder Skin | 252 Atk / 252 Spe | Fake Out, Double-Edge, Sucker Punch, Protect",
            "Gardevoir | Modest | Trace | 252 SpA / 252 Spe | Psychic, Dazzling Gleam, Helping Hand, Protect",
            "Ninetales | Timid | Drought | 252 SpA / 252 Spe | Heat Wave, Solar Beam, Will-O-Wisp, Protect",
            "Vileplume | Modest | Chlorophyll | 252 SpA / 252 HP | Sleep Powder, Giga Drain, Sludge Bomb, Protect",
            "Azumarill | Adamant | Huge Power | 252 Atk / 252 HP | Aqua Jet, Play Rough, Superpower, Protect",
            "Raichu @ Focus Sash | Timid | Lightning Rod | 252 SpA / 252 Spe | Fake Out, Thunderbolt, Nasty Plot, Protect"]),
    ],
    # 10 — Autéquia (Intelligence)
    [
        ('Ernest', 'Ruin Maniac', 'Ruin Maniac', 'Male', [
            "Claydol @ Sitrus Berry | Bold | Levitate | 252 HP / 252 Def | Earth Power, Psychic, Trick Room, Protect",
            "Golem | Brave | Sturdy | 252 HP / 252 Atk | Rock Slide, Earthquake, Explosion, Protect",
            "Cacturne | Adamant | Water Absorb | 252 Atk / 252 Spe | Sucker Punch, Seed Bomb, Drain Punch, Protect",
            "Sandslash | Adamant | Sand Rush | 252 Atk / 252 Spe | Earthquake, Rock Slide, Knock Off, Protect",
            "Hippowdon @ Smooth Rock | Impish | Sand Stream | 252 HP / 252 Def | Earthquake, Rock Slide, Yawn, Slack Off",
            "Lunatone | Modest | Levitate | 252 SpA / 252 HP | Psychic, Moonblast, Trick Room, Protect"]),
        ('Victoire', 'Psychic', 'Psychic F', 'Female', [
            "Alakazam @ Focus Sash | Timid | Inner Focus | 252 SpA / 252 Spe | Psychic, Shadow Ball, Dazzling Gleam, Protect",
            "Slowbro @ Sitrus Berry | Relaxed | Oblivious | 252 HP / 252 Def | Scald, Psychic, Trick Room, Protect",
            "Magneton | Modest | Analytic | 252 SpA / 252 HP | Thunderbolt, Flash Cannon, Volt Switch, Protect",
            "Porygon2 @ Eviolite | Quiet | Download | 252 HP / 252 SpA | Tri Attack, Ice Beam, Trick Room, Recover",
            "Xatu | Timid | Magic Bounce | 252 SpA / 252 Spe | Psychic, Heat Wave, Tailwind, Protect",
            "Hariyama | Brave | Guts | 252 HP / 252 Atk | Fake Out, Close Combat, Knock Off, Protect"]),
        ('Augustin', 'Gentleman', 'Gentleman', 'Male', [
            "Metang @ Eviolite | Adamant | Clear Body | 252 HP / 252 Atk | Iron Head, Zen Headbutt, Bullet Punch, Protect",
            "Gardevoir @ Life Orb | Modest | Trace | 252 SpA / 252 Spe | Psychic, Moonblast, Thunderbolt, Protect",
            "Camerupt @ Sitrus Berry | Quiet | Solid Rock | 252 HP / 252 SpA | Eruption, Earth Power, Heat Wave, Protect",
            "Bronzong | Relaxed | Levitate | 252 HP / 252 Def | Gyro Ball, Trick Room, Body Press, Protect",
            "Gastrodon | Calm | Storm Drain | 252 HP / 252 SpD | Earth Power, Scald, Recover, Protect",
            "Whimsicott | Timid | Prankster | 252 HP / 252 Spe | Tailwind, Moonblast, Encore, Protect"]),
    ],
    # 11 — Nénucrique (Grand Concours : Beauté et Robustesse)
    [
        ('Ondine', 'Parasol Lady', 'Parasol Lady', 'Female', [
            "Milotic @ Leftovers | Bold | Competitive | 252 HP / 252 Def | Scald, Ice Beam, Recover, Protect",
            "Pelipper @ Damp Rock | Modest | Drizzle | 252 HP / 252 SpA | Hurricane, Scald, Tailwind, Protect",
            "Ludicolo @ Life Orb | Modest | Swift Swim | 252 SpA / 252 Spe | Fake Out, Hydro Pump, Giga Drain, Protect",
            "Kingdra | Modest | Swift Swim | 252 SpA / 252 Spe | Muddy Water, Draco Meteor, Ice Beam, Protect",
            "Glaceon @ Sitrus Berry | Modest | Ice Body | 252 SpA / 252 HP | Blizzard, Shadow Ball, Freeze-Dry, Protect",
            "Florges | Calm | Flower Veil | 252 HP / 252 SpD | Moonblast, Psychic, Helping Hand, Protect"]),
        ('Boris', 'Black Belt', 'Black Belt', 'Male', [
            "Aggron @ Sitrus Berry | Adamant | Rock Head | 252 Atk / 252 HP | Head Smash, Heavy Slam, Earthquake, Protect",
            "Hariyama @ Assault Vest | Brave | Guts | 252 HP / 252 Atk | Fake Out, Close Combat, Knock Off, Heavy Slam",
            "Tyranitar @ Chople Berry | Adamant | Sand Stream | 252 Atk / 252 HP | Rock Slide, Crunch, Low Kick, Protect",
            "Excadrill @ Life Orb | Jolly | Sand Rush | 252 Atk / 252 Spe | Earthquake, Iron Head, Rock Slide, Protect",
            "Rhyperior | Adamant | Solid Rock | 252 HP / 252 Atk | Rock Slide, High Horsepower, Hammer Arm, Protect",
            "Machamp @ Focus Sash | Adamant | No Guard | 252 Atk / 252 HP | Dynamic Punch, Stone Edge, Knock Off, Protect"]),
        ('Lisia', 'Expert', 'Expert F', 'Female', [
            "Altaria @ Sitrus Berry | Adamant | Natural Cure | 252 Atk / 252 Spe | Dragon Dance, Dragon Claw, Earthquake, Protect",
            "Gardevoir @ Life Orb | Modest | Trace | 252 SpA / 252 Spe | Psychic, Moonblast, Trick Room, Protect",
            "Salamence @ Life Orb | Naive | Intimidate | 252 Atk / 252 Spe | Draco Meteor, Dragon Claw, Fire Blast, Protect",
            "Metagross @ Sitrus Berry | Adamant | Clear Body | 252 Atk / 252 HP | Iron Head, Zen Headbutt, Bullet Punch, Protect",
            "Milotic | Bold | Competitive | 252 HP / 252 Def | Scald, Ice Beam, Recover, Protect",
            "Rotom-Wash | Modest | Levitate | 252 HP / 252 SpA | Hydro Pump, Thunderbolt, Will-O-Wisp, Protect"]),
    ],
]

# 12 — Finale du Couronnement (Tour de Combat) : 32 joueurs, 5 tours ; Blue en demi-finale, Red en finale.
FINALE_NIVEAUX = [86, 88, 90, 93, 96]
FINALE = [
    ('Sakura', 'Cooltrainer', 'Cooltrainer F', 'Female', [
        "Incineroar @ Sitrus Berry | Careful | Intimidate | 252 HP / 252 SpD | Fake Out, Flare Blitz, Knock Off, Parting Shot",
        "Amoonguss @ Rocky Helmet | Sassy | Regenerator | 252 HP / 252 Def | Spore, Rage Powder, Pollen Puff, Protect",
        "Rillaboom @ Choice Band | Adamant | Grassy Surge | 252 Atk / 252 HP | Grassy Glide, Wood Hammer, Knock Off, U-turn",
        "Gholdengo @ Life Orb | Modest | Good as Gold | 252 SpA / 252 Spe | Make It Rain, Shadow Ball, Nasty Plot, Protect",
        "Urshifu-Rapid-Strike @ Focus Sash | Jolly | Unseen Fist | 252 Atk / 252 Spe | Surging Strikes, Close Combat, Aqua Jet, Protect",
        "Tornadus @ Covert Cloak | Timid | Prankster | 252 SpA / 252 Spe | Tailwind, Bleakwind Storm, Taunt, Protect"]),
    ('Dante', 'Expert', 'Expert M', 'Male', [
        "Kingambit @ Black Glasses | Adamant | Defiant | 252 Atk / 252 HP | Kowtow Cleave, Sucker Punch, Iron Head, Protect",
        "Pelipper @ Damp Rock | Modest | Drizzle | 252 HP / 252 SpA | Hurricane, Weather Ball, Wide Guard, Protect",
        "Archaludon @ Assault Vest | Modest | Stamina | 252 HP / 252 SpA | Electro Shot, Draco Meteor, Flash Cannon, Body Press",
        "Basculegion @ Choice Scarf | Adamant | Swift Swim | 252 Atk / 252 Spe | Wave Crash, Last Respects, Aqua Jet, Flip Turn",
        "Farigiraf @ Sitrus Berry | Quiet | Armor Tail | 252 HP / 252 SpA | Trick Room, Psychic, Hyper Voice, Protect",
        "Ursaluna @ Flame Orb | Brave | Guts | 252 HP / 252 Atk | Headlong Rush, Facade, Earthquake, Protect"]),
    ('Lyra', 'Dragon Tamer', 'Dragon Tamer', 'Female', [
        "Dragonite @ Choice Band | Adamant | Inner Focus | 252 Atk / 252 Spe | Extreme Speed, Outrage, Earthquake, Fire Punch",
        "Flutter Mane @ Booster Energy | Timid | Protosynthesis | 252 SpA / 252 Spe | Moonblast, Shadow Ball, Icy Wind, Protect",
        "Torkoal @ Charcoal | Quiet | Drought | 252 HP / 252 SpA | Eruption, Heat Wave, Earth Power, Protect",
        "Lilligant-Hisui @ Focus Sash | Jolly | Chlorophyll | 252 Atk / 252 Spe | Close Combat, Leaf Blade, After You, Protect",
        "Garchomp @ Life Orb | Jolly | Rough Skin | 252 Atk / 252 Spe | Earthquake, Dragon Claw, Rock Slide, Protect",
        "Grimmsnarl @ Light Clay | Careful | Prankster | 252 HP / 252 SpD | Reflect, Light Screen, Spirit Break, Thunder Wave"]),
    ('Blue', 'Champion', 'Champion Rival Frlg', 'Male', [
        "Pidgeot @ Sitrus Berry | Timid | No Guard | 252 SpA / 252 Spe | Hurricane, Heat Wave, Tailwind, Protect",
        "Alakazam @ Focus Sash | Timid | Inner Focus | 252 SpA / 252 Spe | Psychic, Dazzling Gleam, Focus Blast, Protect",
        "Tyranitar @ Assault Vest | Adamant | Sand Stream | 252 HP / 252 Atk | Rock Slide, Knock Off, Low Kick, High Horsepower",
        "Exeggutor @ Sitrus Berry | Quiet | Harvest | 252 HP / 252 SpA | Trick Room, Giga Drain, Psychic, Protect",
        "Arcanine @ Safety Goggles | Adamant | Intimidate | 252 HP / 252 Atk | Flare Blitz, Extreme Speed, Snarl, Protect",
        "Gyarados @ Choice Band | Adamant | Intimidate | 252 Atk / 252 Spe | Waterfall, Stone Edge, Temper Flare, Ice Fang"]),
    ('Red', 'Champion', 'Red', 'Male', [
        "Pikachu @ Light Ball | Timid | Lightning Rod | 252 SpA / 252 Spe | Fake Out, Thunderbolt, Grass Knot, Protect",
        "Charizard @ Choice Specs | Timid | Solar Power | 252 SpA / 252 Spe | Heat Wave, Overheat, Air Slash, Solar Beam",
        "Venusaur @ Life Orb | Modest | Chlorophyll | 252 SpA / 252 Spe | Sleep Powder, Giga Drain, Sludge Bomb, Protect",
        "Blastoise @ Sitrus Berry | Modest | Rain Dish | 252 HP / 252 SpA | Water Spout, Ice Beam, Fake Out, Protect",
        "Snorlax @ Leftovers | Brave | Thick Fat | 252 HP / 252 Atk | Body Slam, Heat Crash, Belly Drum, Protect",
        "Lapras @ Assault Vest | Modest | Water Absorb | 252 HP / 252 SpA | Freeze-Dry, Sparkling Aria, Ice Shard, Perish Song"]),
]

# v0.39 : finalistes des saisons suivantes (niveau 100). Ordre de rotation dans src/couronnement.c :
# Blue (39), Red (40), Green (41), Peter (42), Pierre (43), Marc (44).
FINALISTES_SAISON = [
    ('Green', 'Champion', 'Leaf', 'Female', [
        "Venusaur @ Life Orb | Modest | Chlorophyll | 252 SpA / 252 Spe | Leaf Storm, Sludge Bomb, Earth Power, Protect",
        "Ninetales-Alola @ Light Clay | Timid | Snow Warning | 252 SpA / 252 Spe | Aurora Veil, Blizzard, Moonblast, Protect",
        "Clefable @ Sitrus Berry | Bold | Magic Guard | 252 HP / 252 Def | Follow Me, Moonblast, Helping Hand, Protect",
        "Gengar @ Focus Sash | Timid | Cursed Body | 252 SpA / 252 Spe | Shadow Ball, Sludge Bomb, Icy Wind, Protect",
        "Nidoqueen @ Assault Vest | Modest | Sheer Force | 252 HP / 252 SpA | Earth Power, Sludge Wave, Ice Beam, Flamethrower",
        "Jolteon @ Choice Specs | Timid | Volt Absorb | 252 SpA / 252 Spe | Thunderbolt, Volt Switch, Shadow Ball, Hyper Voice"]),
    ('Peter', 'Champion', 'Elite Four Lance Frlg', 'Male', [
        "Dragonite @ Lum Berry | Adamant | Multiscale | 252 Atk / 252 Spe | Dragon Dance, Extreme Speed, Dragon Claw, Protect",
        "Kingdra @ Choice Specs | Modest | Swift Swim | 252 SpA / 252 Spe | Draco Meteor, Muddy Water, Ice Beam, Hurricane",
        "Gyarados @ Sitrus Berry | Jolly | Intimidate | 252 Atk / 252 Spe | Waterfall, Bounce, Thunder Wave, Protect",
        "Aerodactyl @ Focus Sash | Jolly | Unnerve | 252 Atk / 252 Spe | Rock Slide, Tailwind, Wide Guard, Protect",
        "Charizard @ Charcoal | Timid | Blaze | 252 SpA / 252 Spe | Heat Wave, Air Slash, Dragon Pulse, Protect",
        "Salamence @ Life Orb | Naive | Intimidate | 252 Atk / 252 Spe | Draco Meteor, Dragon Claw, Fire Blast, Protect"]),
    ('Pierre', 'Champion', 'Steven', 'Male', [
        "Metagross @ Weakness Policy | Adamant | Clear Body | 252 Atk / 252 HP | Iron Head, Zen Headbutt, Bullet Punch, Protect",
        "Aggron @ Sitrus Berry | Adamant | Rock Head | 252 Atk / 252 HP | Head Smash, Heavy Slam, Earthquake, Protect",
        "Cradily @ Leftovers | Calm | Storm Drain | 252 HP / 252 SpD | Giga Drain, Rock Slide, Recover, Protect",
        "Armaldo @ Life Orb | Adamant | Battle Armor | 252 Atk / 252 Spe | X-Scissor, Rock Slide, Earthquake, Protect",
        "Skarmory @ Rocky Helmet | Impish | Sturdy | 252 HP / 252 Def | Brave Bird, Tailwind, Iron Defense, Protect",
        "Claydol @ Mental Herb | Sassy | Levitate | 252 HP / 252 SpD | Trick Room, Earth Power, Psychic, Ally Switch"]),
    ('Marc', 'Champion', 'Champion Wallace', 'Male', [
        "Milotic @ Leftovers | Bold | Competitive | 252 HP / 252 Def | Scald, Ice Beam, Recover, Protect",
        "Pelipper @ Damp Rock | Modest | Drizzle | 252 HP / 252 SpA | Hurricane, Scald, Tailwind, Protect",
        "Ludicolo @ Life Orb | Modest | Swift Swim | 252 SpA / 252 Spe | Fake Out, Hydro Pump, Giga Drain, Protect",
        "Swampert @ Sitrus Berry | Adamant | Damp | 252 Atk / 252 HP | Waterfall, High Horsepower, Ice Punch, Protect",
        "Kingdra @ Life Orb | Modest | Swift Swim | 252 SpA / 252 Spe | Muddy Water, Draco Meteor, Ice Beam, Protect",
        "Wailord @ Sitrus Berry | Quiet | Pressure | 252 HP / 252 SpA | Water Spout, Ice Beam, Hydro Pump, Protect"]),
]


def mon_block(spec, level):
    parts = [p.strip() for p in spec.split('|')]
    species, nature, ability, evs, moves = parts
    lines = [species, f'{nature} Nature', f'Level: {level}']
    if ability:
        lines.append(f'Ability: {ability}')
    lines.append('IVs: 31 HP / 31 Atk / 31 Def / 31 SpA / 31 SpD / 31 Spe')
    if evs:
        lines.append(f'EVs: {evs}')
    lines += [f'- {m.strip()}' for m in moves.split(',')]
    return '\n'.join(lines)


def main():
    out = ['/* Généré par tools/couronnement/tournois.py — ne pas modifier à la main. */', '']
    defines = ['// Généré par tools/couronnement/tournois.py — ne pas modifier à la main.',
               '#ifndef GUARD_CONSTANTS_COURONNEMENT_ADVERSAIRES_H',
               '#define GUARD_CONSTANTS_COURONNEMENT_ADVERSAIRES_H', '']
    idx = 0
    for t, adversaires in enumerate(TOURNOIS):
        assert len(adversaires) == 3, t
        for r, (name, cls, pic, gender, mons) in enumerate(adversaires):
            assert len(mons) == 6, (t, r)
            defines.append(f'#define COURONNEMENT_ADV_{idx} {idx}')
            out += [f'=== COURONNEMENT_ADV_{idx} ===', f'Name: {name}', f'Class: {cls}', f'Pic: {pic}',
                    f'Gender: {gender}', f'Music: {gender}', 'Double Battle: Yes',
                    'AI: Basic Trainer / Smart Switching', '']
            out += [mon_block(m, NIVEAUX[t] + r) + '\n' for m in mons]
            idx += 1
    for r, (name, cls, pic, gender, mons) in enumerate(FINALE):
        assert len(mons) == 6, ('finale', r)
        defines.append(f'#define COURONNEMENT_ADV_{idx} {idx}')
        out += [f'=== COURONNEMENT_ADV_{idx} ===', f'Name: {name}', f'Class: {cls}', f'Pic: {pic}',
                f'Gender: {gender}', f'Music: {gender}', 'Double Battle: Yes',
                'AI: Smart Trainer / Smart Switching', '']
        out += [mon_block(m, FINALE_NIVEAUX[r]) + '\n' for m in mons]
        idx += 1
    defines.append(f'#define COURONNEMENT_ADV_FINALISTES_SAISON {idx}')
    for name, cls, pic, gender, mons in FINALISTES_SAISON:
        assert len(mons) == 6, name
        defines.append(f'#define COURONNEMENT_ADV_{idx} {idx}')
        out += [f'=== COURONNEMENT_ADV_{idx} ===', f'Name: {name}', f'Class: {cls}', f'Pic: {pic}',
                f'Gender: {gender}', f'Music: {gender}', 'Double Battle: Yes',
                'AI: Smart Trainer / Smart Switching', '']
        out += [mon_block(m, 100) + '\n' for m in mons]
        idx += 1
    defines += ['', f'#define COURONNEMENT_ADV_COUNT {idx}', '', '#endif // GUARD_CONSTANTS_COURONNEMENT_ADVERSAIRES_H', '']
    (ROOT / 'src/data/couronnement.party').write_text('\n'.join(out) + '\n', encoding='utf-8')
    (ROOT / 'include/constants/couronnement_adversaires.h').write_text('\n'.join(defines), encoding='utf-8')
    print(f'{idx} adversaires écrits.')


if __name__ == '__main__':
    main()
