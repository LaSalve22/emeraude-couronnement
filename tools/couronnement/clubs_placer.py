#!/usr/bin/env python3
"""Place les guichets de la Ligue des Clubs (v0.44) : un par Centre Pokémon des 10 villes de club,
plus celui de la Tour de Combat (finale après la Ligue).

Usage : python3 tools/couronnement/clubs_placer.py   (relançable : remplace les guichets déjà posés)

Case choisie : atteignable à pied depuis la porte, libre, ses 8 voisines sans personnage (on ne colle personne),
ses 4 voisines praticables (on ne bouche pas un passage) et la case du dessous libre (le joueur s'y place pour parler).
"""
import json
from collections import deque
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
CARTES = ['PetalburgCity_PokemonCenter_1F', 'FallarborTown_PokemonCenter_1F', 'LavaridgeTown_PokemonCenter_1F',
          'FortreeCity_PokemonCenter_1F', 'RustboroCity_PokemonCenter_1F', 'MauvilleCity_PokemonCenter_1F',
          'SlateportCity_PokemonCenter_1F', 'MossdeepCity_PokemonCenter_1F', 'LilycoveCity_PokemonCenter_1F',
          'SootopolisCity_PokemonCenter_1F', 'BattleFrontier_BattleTowerLobby']
SPRITE = 'OBJ_EVENT_GFX_COOLTRAINER_M'
SCRIPT = 'Club_EventScript_Guichet'


def charger_layout(nom):
    layouts = json.load(open(ROOT / 'data/layouts/layouts.json'))['layouts']
    lay = next(l for l in layouts if l.get('id') == nom)
    data = (ROOT / lay['blockdata_filepath']).read_bytes()
    w, h = lay['width'], lay['height']
    return w, h, [[int.from_bytes(data[(y * w + x) * 2:(y * w + x) * 2 + 2], 'little') for x in range(w)] for y in range(h)]


def main():
    for carte in CARTES:
        chemin = ROOT / 'data/maps' / carte / 'map.json'
        m = json.load(open(chemin, encoding='utf-8'))
        m['object_events'] = [o for o in m['object_events'] if o.get('script') != SCRIPT]
        w, h, cases = charger_layout(m['layout'])
        persos = {(o['x'], o['y']) for o in m['object_events']}
        occupe = set(persos)
        for o in m.get('warp_events', []) + m.get('coord_events', []) + m.get('bg_events', []):
            occupe.add((o['x'], o['y']))
        porte = m['warp_events'][0]
        ref = (porte['x'], porte['y'] - 1)
        for wp in m.get('warp_events', []):
            occupe.add((wp['x'], wp['y'] - 1))

        def praticable(x, y):
            return 0 <= x < w and 0 <= y < h and (cases[y][x] >> 10) & 3 == 0 and (x, y) not in occupe

        vus, file, choix = {ref}, deque([(ref, 0)]), None
        while file:
            (x, y), d = file.popleft()
            if (d >= 3 and praticable(x, y) and praticable(x, y + 1)
                    and all(praticable(x + dx, y + dy) for dx, dy in ((1, 0), (-1, 0), (0, -1)))
                    and not any((x + dx, y + dy) in persos for dx in (-1, 0, 1) for dy in (-1, 0, 1))):
                choix = (x, y)
                break
            for dx, dy in ((1, 0), (-1, 0), (0, 1), (0, -1)):
                nx, ny = x + dx, y + dy
                if (nx, ny) not in vus and 0 <= nx < w and 0 <= ny < h and (cases[ny][nx] >> 10) & 3 == 0:
                    vus.add((nx, ny))
                    file.append(((nx, ny), d + 1))
        assert choix, carte
        m['object_events'].append({
            'graphics_id': SPRITE, 'x': choix[0], 'y': choix[1], 'elevation': 3,
            'movement_type': 'MOVEMENT_TYPE_FACE_DOWN', 'movement_range_x': 0, 'movement_range_y': 0,
            'trainer_type': 'TRAINER_TYPE_NONE', 'trainer_sight_or_berry_tree_id': '0', 'script': SCRIPT, 'flag': '0',
        })
        with open(chemin, 'w', encoding='utf-8') as f:
            json.dump(m, f, indent=2, ensure_ascii=False)
            f.write('\n')
        print(f'{carte}: guichet en {choix}')


if __name__ == '__main__':
    main()
