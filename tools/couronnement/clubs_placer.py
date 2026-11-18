#!/usr/bin/env python3
"""Place les guichets de la Ligue des Clubs (v0.44) : un par Centre Pokémon des 10 villes de club,
plus celui de la Tour de Combat (finale après la Ligue) et ceux des sièges dans les arènes (v0.48).

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
          'SootopolisCity_PokemonCenter_1F', 'BattleFrontier_BattleTowerLobby',
          # v0.48 : sièges des clubs, à l'entrée des arènes des villes de club
          'PetalburgCity_Gym', 'RustboroCity_Gym', 'MauvilleCity_Gym', 'LavaridgeTown_Gym_1F', 'FortreeCity_Gym',
          'MossdeepCity_Gym', 'SootopolisCity_Gym_1F']
SPRITE = 'OBJ_EVENT_GFX_MAN_3'  # COOLTRAINER_M n'existe que dans Rouge Feu (invisible et plantage des boutiques)
SCRIPT = 'Club_EventScript_Guichet'


def charger_layout(nom):
    layouts = json.load(open(ROOT / 'data/layouts/layouts.json'))['layouts']
    lay = next(l for l in layouts if l.get('id') == nom)
    data = (ROOT / lay['blockdata_filepath']).read_bytes()
    w, h = lay['width'], lay['height']
    return w, h, [[int.from_bytes(data[(y * w + x) * 2:(y * w + x) * 2 + 2], 'little') for x in range(w)] for y in range(h)]


def charger_comportements(nom):
    """Comportement (MB_*) de chaque métatuile du layout, lu dans les metatile_attributes.bin des deux tilesets."""
    layouts = json.load(open(ROOT / 'data/layouts/layouts.json'))['layouts']
    lay = next(l for l in layouts if l.get('id') == nom)
    res = {}
    for i, cle in enumerate(('primary_tileset', 'secondary_tileset')):
        nomts = lay[cle].replace('gTileset_', '')
        for dossier in (ROOT / 'data/tilesets/primary', ROOT / 'data/tilesets/secondary'):
            for d in dossier.iterdir():
                if d.name.replace('_', '').lower() == nomts.lower():
                    data = (d / 'metatile_attributes.bin').read_bytes()
                    base = 0 if i == 0 else 512
                    for k in range(len(data) // 2):
                        res[base + k] = int.from_bytes(data[2 * k:2 * k + 2], 'little') & 0xFF
    return res


def main():
    for carte in CARTES:
        chemin = ROOT / 'data/maps' / carte / 'map.json'
        m = json.load(open(chemin, encoding='utf-8'))
        m['object_events'] = [o for o in m['object_events'] if o.get('script') != SCRIPT]
        w, h, cases = charger_layout(m['layout'])
        comportements = charger_comportements(m['layout'])
        persos = {(o['x'], o['y']) for o in m['object_events']}
        occupe = set(persos)
        for o in m.get('warp_events', []) + m.get('coord_events', []) + m.get('bg_events', []):
            occupe.add((o['x'], o['y']))
        porte = max(m['warp_events'], key=lambda w: w['y'])  # la porte d'entrée est la plus basse
        ref = (porte['x'], porte['y'] - 1)
        for wp in m.get('warp_events', []):
            occupe.add((wp['x'], wp['y'] - 1))

        def glissant(x, y):  # glace, trous, tapis... : comportement de case non nul = on évite
            if not (0 <= x < w and 0 <= y < h):
                return True
            return comportements.get(cases[y][x] & 0x3FF, 0) != 0

        def praticable(x, y):
            return 0 <= x < w and 0 <= y < h and (cases[y][x] >> 10) & 3 == 0 and (x, y) not in occupe

        def accessibles(bloque):
            vus2, f2 = {ref}, deque([ref])
            while f2:
                x, y = f2.popleft()
                for dx, dy in ((1, 0), (-1, 0), (0, 1), (0, -1)):
                    n = (x + dx, y + dy)
                    if n not in vus2 and n != bloque and 0 <= n[0] < w and 0 <= n[1] < h and (cases[n[1]][n[0]] >> 10) & 3 == 0:
                        vus2.add(n)
                        f2.append(n)
            return vus2

        tout = accessibles(None)
        choix = None
        # Candidats par distance croissante à la porte ; le guichet ne doit isoler aucune case (connexité conservée).
        for ecart_porte in (2, 1):
            candidats = sorted(tout, key=lambda c: abs(c[0] - ref[0]) + abs(c[1] - ref[1]))
            for x, y in candidats:
                if (abs(x - ref[0]) + abs(y - ref[1]) < 2 or abs(x - porte['x']) < ecart_porte or (x, y) in occupe
                        or (x, y + 1) in occupe or (x, y + 1) not in tout or glissant(x, y) or glissant(x, y + 1)
                        or any((x + dx, y + dy) in persos for dx in (-1, 0, 1) for dy in (-1, 0, 1))):
                    continue
                if len(accessibles((x, y))) == len(tout) - 1:
                    choix = (x, y)
                    break
            if choix:
                break
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
