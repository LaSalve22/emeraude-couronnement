#!/usr/bin/env python3
"""Place le responsable de l'arène (v0.40) dans les 6 villes sans arène.

Usage : python3 tools/couronnement/arenes_placer.py   (relançable : remplace les responsables déjà posés)

Case choisie : atteignable à pied depuis une case de référence (sous le panneau « À vendre » de l'immobilier,
ou devant la première porte), sol le plus courant de la ville et même hauteur, libre, ses 4 voisines praticables (on ne bouche pas un passage)
et la case du dessous libre (le joueur s'y place pour parler).
"""
import json
from collections import deque
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
VILLES = ['LittlerootTown', 'OldaleTown', 'SlateportCity', 'VerdanturfTown', 'FallarborTown', 'LilycoveCity']
SPRITE = 'OBJ_EVENT_GFX_COOLTRAINER_M'


def charger_layout(nom):
    layouts = json.load(open(ROOT / 'data/layouts/layouts.json'))['layouts']
    lay = next(l for l in layouts if l.get('id') == nom)
    data = (ROOT / lay['blockdata_filepath']).read_bytes()
    w, h = lay['width'], lay['height']
    cases = [[int.from_bytes(data[(y * w + x) * 2:(y * w + x) * 2 + 2], 'little') for x in range(w)] for y in range(h)]
    return w, h, cases


def main():
    for i, ville in enumerate(VILLES):
        chemin = ROOT / 'data/maps' / ville / 'map.json'
        m = json.load(open(chemin))
        script = f'Arene_EventScript_Guichet{i}'
        m['object_events'] = [o for o in m['object_events'] if o.get('script') != script]
        w, h, cases = charger_layout(m['layout'])
        occupe = set()
        for o in m['object_events'] + m.get('warp_events', []) + m.get('coord_events', []) + m.get('bg_events', []):
            occupe.add((o['x'], o['y']))
        for wp in m.get('warp_events', []):
            occupe.add((wp['x'], wp['y'] + 1))  # devant les portes
        panneau = next((o for o in m['object_events'] if str(o.get('script', '')).startswith('Immo_EventScript_Panneau')), None)
        ref = (panneau['x'], panneau['y'] + 1) if panneau else (m['warp_events'][0]['x'], m['warp_events'][0]['y'] + 1)

        def praticable(x, y):
            return 0 <= x < w and 0 <= y < h and (cases[y][x] >> 10) & 3 == 0

        hauteur = cases[ref[1]][ref[0]] >> 12
        # sol = la case praticable la plus courante à cette hauteur (herbe rase, pavés…)
        compte = {}
        for ligne in cases:
            for c in ligne:
                if (c >> 10) & 3 == 0 and c >> 12 == hauteur:
                    compte[c & 0x3FF] = compte.get(c & 0x3FF, 0) + 1
        sol = max(compte, key=compte.get)
        vus, file, choix = {ref}, deque([(ref, 0)]), None
        while file and choix is None:
            (x, y), d = file.popleft()
            if (d >= 3 and (x, y) not in occupe and (x, y + 1) not in occupe
                    and cases[y][x] & 0x3FF == sol and all(praticable(x + dx, y + dy) for dx, dy in ((1, 0), (-1, 0), (0, 1), (0, -1)))
                    and cases[y + 1][x] >> 12 == hauteur):
                choix = (x, y)
                break
            for dx, dy in ((1, 0), (-1, 0), (0, 1), (0, -1)):
                nx, ny = x + dx, y + dy
                if (nx, ny) not in vus and praticable(nx, ny) and cases[ny][nx] >> 12 == hauteur:
                    vus.add((nx, ny))
                    file.append(((nx, ny), d + 1))
        assert choix, ville
        m['object_events'].append({
            'graphics_id': SPRITE, 'x': choix[0], 'y': choix[1], 'elevation': hauteur,
            'movement_type': 'MOVEMENT_TYPE_FACE_DOWN', 'movement_range_x': 0, 'movement_range_y': 0,
            'trainer_type': 'TRAINER_TYPE_NONE', 'trainer_sight_or_berry_tree_id': '0', 'script': script, 'flag': '0',
        })
        json.dump(m, open(chemin, 'w'), indent=2, ensure_ascii=False)
        open(chemin, 'a').write('\n')
        print(f'{ville}: responsable en {choix} (réf. {ref}, {len(m["object_events"])} objets)')


if __name__ == '__main__':
    main()
