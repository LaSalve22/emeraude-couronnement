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

# Tournoi t : accessible après le badge t+1. Niveaux = cap du badge suivant - 3, +1 par manche.
NIVEAUX = [24, 33, 42, 51, 60, 68, 75, 82]

# (nom, classe, sprite, genre, [4 Pokémon])
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
            assert len(mons) == 4, (t, r)
            defines.append(f'#define COURONNEMENT_ADV_{idx} {idx}')
            out += [f'=== COURONNEMENT_ADV_{idx} ===', f'Name: {name}', f'Class: {cls}', f'Pic: {pic}',
                    f'Gender: {gender}', f'Music: {gender}', 'Double Battle: Yes',
                    'AI: Basic Trainer / Smart Switching', '']
            out += [mon_block(m, NIVEAUX[t] + r) + '\n' for m in mons]
            idx += 1
    defines += ['', f'#define COURONNEMENT_ADV_COUNT {idx}', '', '#endif // GUARD_CONSTANTS_COURONNEMENT_ADVERSAIRES_H', '']
    (ROOT / 'src/data/couronnement.party').write_text('\n'.join(out) + '\n', encoding='utf-8')
    (ROOT / 'include/constants/couronnement_adversaires.h').write_text('\n'.join(defines), encoding='utf-8')
    print(f'{idx} adversaires écrits.')


if __name__ == '__main__':
    main()
