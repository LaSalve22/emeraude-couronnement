#!/usr/bin/env python3
"""Repeuple les zones sauvages de Hoenn avec des Pokémon de toutes les générations.

Règles (Émeraude Couronnement) :
- Début de jeu (avant le 5e badge) : surtout des Gen 1 à 4, quelques Gen 5 à 9.
- Milieu et fin de jeu : toutes les générations à poids égal.
- Chaque zone a un biome (herbes, forêt, grotte, volcan, désert, mer, fantôme, ciel…) qui choisit les types.
- La forme proposée dépend du niveau du slot (forme évoluée si le niveau dépasse le niveau d'évolution).
- Les 27 starters de toutes les régions sont placés dans des zones précises (slots à 4 %).
- Exclus : légendaires, fabuleux, Ultra-Chimères, Paradoxes, bébés, fossiles.
- Les niveaux et les taux de rencontre d'origine sont conservés (level caps inchangés).

Données : tables CSV de PokéAPI (https://github.com/PokeAPI/pokeapi, data/v2/csv).

Usage : python3 tools/wild/repeupler.py <dossier_csv_pokeapi>
Écrit src/data/wild_encounters.json et docs/rencontres_sauvages.md (rapport par zone).
"""
import csv
import json
import random
import re
import sys
from collections import defaultdict
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
JSON_PATH = ROOT / 'src/data/wild_encounters.json'
REPORT_PATH = ROOT / 'docs/rencontres_sauvages.md'
SEED = 'emeraude-couronnement'

# ---------------------------------------------------------------- données

STARTERS = {  # lignée de base -> zone où elle est capturable
    'bulbasaur': 'MAP_PETALBURG_WOODS', 'charmander': 'MAP_FIERY_PATH', 'squirtle': 'MAP_ROUTE104',
    'chikorita': 'MAP_ROUTE116', 'cyndaquil': 'MAP_ROUTE113', 'totodile': 'MAP_ROUTE110',
    'treecko': 'MAP_ROUTE117', 'torchic': 'MAP_ROUTE102', 'mudkip': 'MAP_ROUTE103',
    'turtwig': 'MAP_ROUTE119', 'chimchar': 'MAP_JAGGED_PASS', 'piplup': 'MAP_ROUTE115',
    'snivy': 'MAP_ROUTE120', 'tepig': 'MAP_ROUTE112', 'oshawott': 'MAP_ROUTE118',
    'chespin': 'MAP_ROUTE121', 'fennekin': 'MAP_MT_PYRE_EXTERIOR', 'froakie': 'MAP_ROUTE123',
    'rowlet': 'MAP_SAFARI_ZONE_SOUTH', 'litten': 'MAP_MAGMA_HIDEOUT_1F', 'popplio': 'MAP_SAFARI_ZONE_SOUTHWEST',
    'grookey': 'MAP_SAFARI_ZONE_NORTHWEST', 'scorbunny': 'MAP_ROUTE111', 'sobble': 'MAP_ROUTE114',
    'sprigatito': 'MAP_SAFARI_ZONE_SOUTHEAST', 'fuecoco': 'MAP_MT_PYRE_SUMMIT', 'quaxly': 'MAP_ROUTE130',
}
PSEUDOS = {'dratini', 'larvitar', 'bagon', 'beldum', 'gible', 'deino', 'goomy', 'jangmo-o', 'dreepy', 'frigibax'}
FOSSILS = {'omanyte', 'kabuto', 'aerodactyl', 'lileep', 'anorith', 'cranidos', 'shieldon', 'tirtouga', 'archen',
           'tyrunt', 'amaura', 'dracozolt', 'arctozolt', 'dracovish', 'arctovish'}
ULTRA_BEASTS = set(range(793, 800)) | set(range(803, 807))
PARADOX = set(range(984, 996)) | set(range(1005, 1011)) | set(range(1020, 1024))
BANNED = {'unown', 'smeargle', 'ditto', 'wishiwashi', 'shedinja', 'castform', 'kecleon', 'spinda',
          'magearna', 'meltan', 'melmetal'}

# Biomes : types préférés (poids)
BIOMES = {
    'herbes':   {'normal': 3, 'bug': 2, 'flying': 2, 'grass': 2, 'electric': 1, 'fairy': 1, 'poison': 1, 'fighting': 1, 'ground': 1},
    'foret':    {'bug': 3, 'grass': 3, 'poison': 2, 'fairy': 1, 'normal': 1, 'dark': 1},
    'plage':    {'ground': 3, 'flying': 3, 'normal': 1, 'bug': 1, 'rock': 1},
    'pluie':    {'water': 3, 'grass': 2, 'bug': 2, 'electric': 1, 'poison': 1},
    'grotte':   {'rock': 3, 'ground': 3, 'steel': 2, 'dark': 1, 'poison': 1, 'fighting': 1},
    'grotte_eau': {'water': 2, 'rock': 2, 'ground': 1, 'dark': 1, 'ice': 1},
    'volcan':   {'fire': 4, 'rock': 2, 'ground': 2},
    'cendres':  {'fire': 2, 'ground': 2, 'rock': 1, 'poison': 1, 'steel': 1},
    'desert':   {'ground': 4, 'rock': 2, 'bug': 1, 'fire': 1},
    'fantome':  {'ghost': 5, 'dark': 2, 'psychic': 2},
    'sommet':   {'ghost': 2, 'psychic': 3, 'fairy': 2, 'dark': 1},
    'electrique': {'electric': 5, 'steel': 3},
    'glace':    {'ice': 5, 'water': 2},
    'ciel':     {'dragon': 3, 'flying': 3, 'psychic': 2, 'ghost': 1},
    'safari':   {'normal': 2, 'grass': 2, 'bug': 1, 'ground': 1, 'psychic': 1, 'fighting': 1, 'flying': 1, 'poison': 1},
    'route_dure': {'fighting': 2, 'dragon': 1, 'steel': 1, 'dark': 2, 'psychic': 1, 'normal': 1, 'flying': 1},
    'mer':      {'water': 6, 'flying': 1},
    'rochers':  {'rock': 4, 'ground': 3, 'steel': 1},
}

TIER_OF = [  # (motif de carte, palier de progression)
    (r'ROUTE10[1-4]$|PETALBURG_WOODS|ROUTE116|RUSTURF|GRANITE_CAVE|ROUTE106$|DEWFORD|PETALBURG_CITY|ALTERING', 0),
    (r'ROUTE10[9]$|ROUTE11[0-4]$|ROUTE117|FIERY_PATH|JAGGED|METEOR_FALLS_1F_1R|NEW_MAUVILLE|SLATEPORT|MIRAGE', 1),
    (r'ROUTE10[578]$|ROUTE11[5-9]$|ROUTE12[0-3]$|MT_PYRE|SAFARI_ZONE_(SOUTH|SOUTHWEST|NORTH|NORTHWEST)$|ABANDONED|LILYCOVE|MAGMA_HIDEOUT', 2),
    (r'.*', 3),
]

BIOME_OF = [  # (motif de carte, biome terrestre)
    (r'PETALBURG_WOODS|ROUTE119|ROUTE120|SAFARI_ZONE_(NORTHEAST|SOUTHEAST)', 'foret'),
    (r'ROUTE123|ROUTE121', 'route_dure'),
    (r'ROUTE104|ROUTE109|ROUTE130', 'plage'),
    (r'ROUTE117|ROUTE118|ROUTE115', 'pluie'),
    (r'GRANITE_CAVE|RUSTURF|VICTORY_ROAD|ARTISAN|ALTERING|DESERT_UNDERPASS', 'grotte'),
    (r'METEOR_FALLS|SEAFLOOR|CAVE_OF_ORIGIN', 'grotte_eau'),
    (r'FIERY_PATH|MAGMA_HIDEOUT', 'volcan'),
    (r'ROUTE113|JAGGED', 'cendres'),
    (r'ROUTE111|MIRAGE_TOWER', 'desert'),
    (r'MT_PYRE_[1-6]F', 'fantome'),
    (r'MT_PYRE_(EXTERIOR|SUMMIT)', 'sommet'),
    (r'NEW_MAUVILLE', 'electrique'),
    (r'SHOAL_CAVE', 'glace'),
    (r'SKY_PILLAR', 'ciel'),
    (r'SAFARI_ZONE', 'safari'),
    (r'ROUTE112|ROUTE114', 'rochers'),
    (r'.*', 'herbes'),
]

GEN_WEIGHT = {0: {g: (1.0 if g <= 4 else 0.12) for g in range(1, 10)},
              1: {g: (1.0 if g <= 4 else 0.2) for g in range(1, 10)},
              2: {g: 1.0 for g in range(1, 10)},
              3: {g: 1.0 for g in range(1, 10)}}
BST_CAP = {0: 330, 1: 370, 2: 430, 3: 600}


def load(csv_dir):
    d = Path(csv_dir)
    rows = lambda name: list(csv.DictReader(open(d / f'{name}.csv', encoding='utf-8')))
    species = {int(r['id']): r for r in rows('pokemon_species')}
    default_mon = {int(r['species_id']): int(r['id']) for r in rows('pokemon') if r['is_default'] == '1'}
    type_name = {int(r['id']): r['identifier'] for r in rows('types')}
    types = defaultdict(list)
    for r in rows('pokemon_types'):
        types[int(r['pokemon_id'])].append(type_name[int(r['type_id'])])
    bst = defaultdict(int)
    for r in rows('pokemon_stats'):
        bst[int(r['pokemon_id'])] += int(r['base_stat'])
    evo_level = {}
    for r in rows('pokemon_evolution'):
        sid = int(r['evolved_species_id'])
        lvl = int(r['minimum_level']) if r['minimum_level'] else None
        if sid not in evo_level or (lvl and (evo_level[sid] is None or lvl < evo_level[sid])):
            evo_level[sid] = lvl
    children = defaultdict(list)
    for sid, r in species.items():
        if r['evolves_from_species_id']:
            children[int(r['evolves_from_species_id'])].append(sid)
    return species, default_mon, types, bst, evo_level, children


def species_const(identifier):
    return 'SPECIES_' + re.sub(r'[^A-Z0-9]', '_', identifier.upper()).replace('__', '_').strip('_')


def main():
    if len(sys.argv) < 2:
        print(__doc__)
        return 2
    species, default_mon, types, bst, evo_level, children = load(sys.argv[1])
    known = set(re.findall(r'\b(SPECIES_\w+)\s*[=,]', (ROOT / 'include/constants/species.h').read_text()))
    name = {sid: r['identifier'] for sid, r in species.items()}
    const = {sid: species_const(n) for sid, n in name.items()}

    def info(sid):
        mon = default_mon.get(sid)
        return types.get(mon, []), bst.get(mon, 0)

    def usable(sid):
        r = species[sid]
        return (const[sid] in known and r['is_legendary'] == '0' and r['is_mythical'] == '0'
                and sid not in ULTRA_BEASTS and sid not in PARADOX and name[sid] not in BANNED)

    # Lignées : forme de base (hors bébé)
    def base_of(sid):
        while species[sid]['evolves_from_species_id']:
            parent = int(species[sid]['evolves_from_species_id'])
            if species[parent]['is_baby'] == '1':
                break
            sid = parent
        return sid

    bases = sorted({base_of(s) for s in species if usable(s) and species[s]['is_baby'] == '0'})
    bases = [b for b in bases if usable(b) and name[b] not in FOSSILS]
    starters = {b for b in bases if name[b] in STARTERS}
    pseudos = {b for b in bases if name[b] in PSEUDOS}
    generic = [b for b in bases if b not in starters and b not in pseudos]

    def form_for_level(base, level, rng):
        cur = base
        while True:
            opts = []
            for c in children.get(cur, []):
                if not usable(c):
                    continue
                req = evo_level.get(c)
                if req is None:
                    req = 30 if cur == base else 42
                if req <= level:
                    opts.append(c)
            if not opts:
                return cur
            cur = rng.choice(sorted(opts))

    usage = defaultdict(int)

    def pick(rng, tier, prefs, exclude, cap=None, water_only=False, no_water=False):
        cap = cap or BST_CAP[tier]
        weights = []
        for b in generic:
            if b in exclude:
                continue
            t, s = info(b)
            if s > cap or s == 0:
                continue
            if water_only and 'water' not in t:
                continue
            if no_water and 'water' in t:
                continue
            fit = sum(prefs.get(x, 0) for x in t)
            if fit <= 0:
                continue
            gen = int(species[b]['generation_id'])
            w = fit * GEN_WEIGHT[tier][gen] / (1 + 0.8 * usage[b])
            weights.append((b, w))
        if not weights:
            return None
        total = sum(w for _, w in weights)
        x = rng.random() * total
        for b, w in weights:
            x -= w
            if x <= 0:
                return b
        return weights[-1][0]

    def tier_of(m):
        return next(t for pat, t in TIER_OF if re.search(pat, m))

    def biome_of(m):
        return next(b for pat, b in BIOME_OF if re.search(pat, m))

    data = json.loads(JSON_PATH.read_text(encoding='utf-8'))
    group = data['wild_encounter_groups'][0]
    report = ['# Rencontres sauvages - Émeraude Couronnement', '',
              'Généré par `tools/wild/repeupler.py`. Taux et niveaux d\'origine conservés.', '']
    placed_starters = set()

    for enc in group['encounters']:
        if not enc.get('base_label', '').startswith('g'):
            continue  # cartes de Kanto (FRLG) : inchangées
        m = enc['map'].replace('MAP_', '')
        rng = random.Random(f"{SEED}:{enc['base_label']}")
        tier = tier_of(m)
        lines_report = [f"## {m} (palier {tier})", '']

        def fill(kind, slots_layout, prefs, cap=None, water_only=False, starter=None, rare_pseudo=False, no_water=False):
            mons = enc[kind]['mons']
            n = max(slots_layout) + 1
            chosen, exclude = [], set()
            for _ in range(n):
                b = pick(rng, tier, prefs, exclude, cap, water_only, no_water)
                if b is None:
                    b = chosen[-1] if chosen else generic[0]
                chosen.append(b)
                exclude.add(b)
            for b in chosen:
                usage[b] += 1
            if starter is not None and len(slots_layout) >= 10:
                chosen[slots_layout[8]] = starter
            if rare_pseudo and tier == 3 and len(slots_layout) >= 12:
                pool = sorted(p for p in pseudos if any(prefs.get(t, 0) for t in info(p)[0])) or sorted(pseudos)
                chosen[slots_layout[11]] = rng.choice(pool)
            out = []
            for i, slot in enumerate(mons):
                base = chosen[slots_layout[i]]
                # La forme dépend du niveau MINIMUM du slot : jamais de forme évoluée sous son niveau d'évolution.
                sid = form_for_level(base, slot['min_level'], rng)
                slot['species'] = const[sid]
                out.append(f"{const[sid][8:].title()} N.{slot['min_level']}-{slot['max_level']}")
            lines_report.append(f"- **{kind}** : " + ', '.join(out))

        st = None
        for sname, smap in STARTERS.items():
            if smap == enc['map'] and sname not in placed_starters:
                st = next(b for b in starters if name[b] == sname)
                placed_starters.add(sname)
                break
        if 'land_mons' in enc:
            biome = biome_of(m)
            fill('land_mons', [0, 1, 2, 3, 0, 1, 4, 5, 6, 7, 8, 9], BIOMES[biome], starter=st, rare_pseudo=True,
                 no_water=biome not in ('pluie', 'grotte_eau', 'glace'))
            st = None
        if 'water_mons' in enc:
            fill('water_mons', [0, 1, 2, 3, 4], BIOMES['mer'], water_only=True, cap=BST_CAP[max(tier, 2)])
        if 'rock_smash_mons' in enc:
            fill('rock_smash_mons', [0, 1, 2, 3, 4], BIOMES['rochers'])
        if 'fishing_mons' in enc:
            mons = enc['fishing_mons']['mons']
            for idxs, cap in (([0, 1], 330), ([2, 3, 4], 420), ([5, 6, 7, 8, 9], 600)):
                exclude = set()
                for i in idxs:
                    b = pick(rng, max(tier, 2), BIOMES['mer'], exclude, cap, water_only=True) or generic[0]
                    exclude.add(b)
                    usage[b] += 1
                    mons[i]['species'] = const[form_for_level(b, mons[i]['min_level'], rng)]
            lines_report.append('- **fishing_mons** : ' + ', '.join(
                f"{x['species'][8:].title()} N.{x['min_level']}-{x['max_level']}" for x in mons))
        if st is not None:
            placed_starters.discard(name[st])
        report += lines_report + ['']

    missing = sorted(set(STARTERS) - placed_starters)
    JSON_PATH.write_text(json.dumps(data, indent=2, ensure_ascii=False) + '\n', encoding='utf-8')
    gens = defaultdict(int)
    for b, n in usage.items():
        gens[int(species[b]['generation_id'])] += n
    report.insert(3, 'Répartition par génération (lignées tirées) : ' +
                  ', '.join(f'Gen {g} : {gens[g]}' for g in sorted(gens)) + '\n')
    report.insert(4, 'Starters placés : ' + ', '.join(sorted(placed_starters)) + '\n')
    REPORT_PATH.parent.mkdir(exist_ok=True)
    REPORT_PATH.write_text('\n'.join(report) + '\n', encoding='utf-8')
    print(f'{len(placed_starters)} starters placés. Manquants : {missing or "aucun"}')
    print('Lignées distinctes utilisées :', len(usage))
    print('Par génération :', dict(sorted(gens.items())))
    return 1 if missing else 0


if __name__ == '__main__':
    sys.exit(main())
