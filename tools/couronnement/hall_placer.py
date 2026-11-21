#!/usr/bin/env python3
"""Hall du Couronnement Mondial (v0.56, édition Carrière) : un bâtiment neuf dans chacune des 8 villes d'Arène.

- Cherche dans chaque ville un rectangle 4 x 3 de cases libres (praticables, sans comportement, sans événement) dont la case
  sous la porte est praticable, sans couper aucun passage (les cases atteignables restent atteignables), au plus près du Centre
  Pokémon ;
- ajoute à la ville un passage vers MAP_COURONNEMENT_HALL sur la porte (en édition classique la case reste un sol ordinaire :
  le passage ne s'y déclenche pas) ;
- écrit src/data/couronnement_halls.h (ville, coin, tournoi) : la façade est dessinée au chargement de la carte en Carrière.
Relançable. Usage : python3 tools/couronnement/hall_placer.py
"""
import json
from collections import deque
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
VILLES = [  # carte, constante, tournoi du Couronnement
    ('RustboroCity', 'MAP_RUSTBORO_CITY', 'COURONNEMENT_TOURNOI_MEROUVILLE'),
    ('DewfordTown', 'MAP_DEWFORD_TOWN', 'COURONNEMENT_TOURNOI_MYOKARA'),
    ('MauvilleCity', 'MAP_MAUVILLE_CITY', 'COURONNEMENT_TOURNOI_LAVANDIA'),
    ('LavaridgeTown', 'MAP_LAVARIDGE_TOWN', 'COURONNEMENT_TOURNOI_VERMILAVA'),
    ('PetalburgCity', 'MAP_PETALBURG_CITY', 'COURONNEMENT_TOURNOI_CLEMENTI'),
    ('FortreeCity', 'MAP_FORTREE_CITY', 'COURONNEMENT_TOURNOI_CIMETRONELLE'),
    ('MossdeepCity', 'MAP_MOSSDEEP_CITY', 'COURONNEMENT_TOURNOI_ALGATIA'),
    ('SootopolisCity', 'MAP_SOOTOPOLIS_CITY', 'COURONNEMENT_TOURNOI_ATALANOPOLIS'),
]
HALL = 'MAP_COURONNEMENT_HALL'
LARGEUR, HAUTEUR, PORTE = 4, 3, 1
SOLS = (0, 33)  # MB_NORMAL, MB_SAND  # porte : colonne 1 de la dernière rangée

import sys
sys.path.insert(0, str(Path(__file__).resolve().parent))
from clubs_placer import charger_layout, charger_comportements  # noqa: E402


def placer(nom):
    chemin = ROOT / 'data/maps' / nom / 'map.json'
    m = json.load(open(chemin, encoding='utf-8'))
    m['warp_events'] = [w for w in m['warp_events'] if w['dest_map'] != HALL]
    w, h, cases = charger_layout(m['layout'])
    comp = charger_comportements(m['layout'])
    occupe = set()
    for o in m['object_events'] + m['warp_events'] + m.get('coord_events', []) + m.get('bg_events', []):
        occupe.add((o['x'], o['y']))
    for wp in m['warp_events']:
        occupe.add((wp['x'], wp['y'] + 1))  # devant les portes

    def libre(x, y):  # sol ordinaire ou sable, sans événement
        return (0 <= x < w and 0 <= y < h and (cases[y][x] >> 10) & 3 == 0
                and comp.get(cases[y][x] & 0x3FF, 0) in SOLS and (x, y) not in occupe)

    def praticable(x, y, bloque=()):
        return 0 <= x < w and 0 <= y < h and (cases[y][x] >> 10) & 3 == 0 and (x, y) not in bloque

    pc = next(wp for wp in m['warp_events'] if 'POKEMON_CENTER' in wp['dest_map'])
    ref = (pc['x'], pc['y'] + 1)

    def atteignables(bloque):
        vus, f = {ref}, deque([ref])
        while f:
            x, y = f.popleft()
            for dx, dy in ((1, 0), (-1, 0), (0, 1), (0, -1)):
                n = (x + dx, y + dy)
                if n not in vus and praticable(*n, bloque):
                    vus.add(n)
                    f.append(n)
        return vus

    avant = atteignables(())
    candidats = []
    for y in range(1, h - HAUTEUR - 1):
        for x in range(1, w - LARGEUR - 1):
            zone = {(x + i, y + j) for i in range(LARGEUR) for j in range(HAUTEUR)}
            devant = (x + PORTE, y + HAUTEUR)
            # une rangée libre autour du bâtiment, pour ne rien coller
            autour = {(x + i, y + j) for i in range(-1, LARGEUR + 1) for j in range(-1, HAUTEUR + 1)}
            if not all(libre(*c) for c in zone) or not libre(*devant) or devant not in avant:
                continue
            marge = sum(1 for c in autour - zone if not libre(*c))  # cases collées au bâtiment : moins c'est mieux
            elev = {(cases[cy][cx] >> 12) for cx, cy in zone}
            if len(elev) != 1:
                continue
            if atteignables(zone) != avant - zone:
                continue
            candidats.append((marge * 8 + abs(x - ref[0]) + abs(y - ref[1]), x, y))
    if not candidats:
        raise SystemExit(f'{nom} : aucune place libre')
    _, x, y = min(candidats)
    m['warp_events'].append({'x': x + PORTE, 'y': y + HAUTEUR - 1, 'elevation': 0, 'dest_map': HALL, 'dest_warp_id': '0'})
    with open(chemin, 'w', encoding='utf-8') as f:
        json.dump(m, f, indent=2, ensure_ascii=False)
        f.write('\n')
    return x, y


def main():
    lignes = ['// Généré par tools/couronnement/hall_placer.py : halls du Couronnement Mondial (édition Carrière).',
              '// Ville, coin haut-gauche du bâtiment 4 x 3, tournoi.']
    for nom, const, tournoi in VILLES:
        x, y = placer(nom)
        print(f'{nom} : bâtiment en ({x}, {y}), porte en ({x + PORTE}, {y + HAUTEUR - 1})')
        lignes.append(f'    {{MAP_GROUP({const}), MAP_NUM({const}), {x}, {y}, {tournoi}}},')
    (ROOT / 'src/data/couronnement_halls.h').write_text('\n'.join(lignes) + '\n', encoding='utf-8')


if __name__ == '__main__':
    main()
