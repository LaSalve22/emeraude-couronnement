#!/usr/bin/env python3
"""Hall du Couronnement Mondial (v0.56, édition Carrière) : un bâtiment neuf dans chacune des 8 villes d'Arène.

- Cherche dans chaque ville un rectangle 4 x 3 de cases libres (praticables, sans comportement, sans événement) dont la case
  sous la porte est praticable, sans couper aucun passage (les cases atteignables restent atteignables), au plus près du Centre
  Pokémon ;
- ajoute à la ville un passage vers MAP_COURONNEMENT_HALL sur la porte (en édition classique la case reste un sol ordinaire :
  le passage ne s'y déclenche pas) ;
- écrit data/layouts/<Ville>/map_carriere.bin (la carte avec le bâtiment, utilisée en Carrière à la place de map.bin, y compris
  quand la ville est vue depuis une route) et src/data/couronnement_halls.h (ville, layout, coin, tournoi, données de la carte).
Préférence : un sol uniforme (même métatuile que le sol le plus courant de la ville, marge comprise), jamais sur un chemin.
Relançable. Usage : python3 tools/couronnement/hall_placer.py
"""
import json
import struct
import sys
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
# Métatuiles de forêt que le bâtiment peut recouvrir (tuiles d'arbre d'une case, comme la Boutique de Cimetronelle)
REMPLACABLES = {'FortreeCity': {0x0C6, 0x0C7}}
# Arbres de 2 x 2 cases (haut-gauche, haut-droite, bas-gauche, bas-droite) : recouverts entiers, jamais coupés
ARBRES = {'PetalburgCity': [(0x1D4, 0x1D5, 0x1DC, 0x1DD)], 'MauvilleCity': [], 'LavaridgeTown': []}
FORCE = {}  # ville -> coin (x, y) choisi à la main après rendu (tools/couronnement/apercu_carte.py)
# Villes sans place propre pour un bâtiment 4 x 4 (vérifié au rendu) : leur Arène fermée sert de hall
HALL_DANS_ARENE = {'PetalburgCity': 'MAP_PETALBURG_CITY_GYM', 'FortreeCity': 'MAP_FORTREE_CITY_GYM',
                   'SootopolisCity': 'MAP_SOOTOPOLIS_CITY_GYM_1F'}
DEBUG = '--debug' in sys.argv
LARGEUR, HAUTEUR, PORTE = 4, 4, 1
SOLS = (0, 2, 33)  # MB_NORMAL, MB_TALL_GRASS, MB_SAND
METATUILES = [[0x2E, 0x2F, 0x35, 0x46], [0x2A, 0x5C, 0x135, 0x13F], [0x1A2, 0x1B6, 0x1E3, 0xDB], [0xDC, 0x61, 0xDD, 0xDE]]  # hall_gfx.py  # porte : colonne 1 de la dernière rangée

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

    def arbres_entiers(zone):
        for q in ARBRES.get(nom, []):
            for (cx, cy) in zone:
                t = cases[cy][cx] & 0x3FF
                if t in q:
                    k = q.index(t)
                    ox, oy = cx - (k % 2), cy - (k // 2)
                    for kk in range(4):
                        if (ox + kk % 2, oy + kk // 2) not in zone:
                            return False
        return True

    avant = atteignables(())
    from collections import Counter
    sol = Counter(cases[cy][cx] & 0x3FF for cx, cy in avant if libre(cx, cy)).most_common(1)[0][0]
    candidats = []
    for y in range(1, h - HAUTEUR - 1):
        for x in range(1, w - LARGEUR - 1):
            zone = {(x + i, y + j) for i in range(LARGEUR) for j in range(HAUTEUR)}
            devant = (x + PORTE, y + HAUTEUR)
            # une rangée libre autour du bâtiment, pour ne rien coller
            autour = {(x + i, y + j) for i in range(-1, LARGEUR + 1) for j in range(-1, HAUTEUR + 1)}
            foret = set(REMPLACABLES.get(nom, set()))
            for q in ARBRES.get(nom, []):
                foret |= set(q)
            if not arbres_entiers(zone):
                continue
            if not all(libre(*c) or (cases[c[1]][c[0]] & 0x3FF) in foret for c in zone if 0 <= c[0] < w and 0 <= c[1] < h) \
               or not libre(*devant) or devant not in avant:
                continue
            marge = sum(1 for c in autour - zone if not libre(*c))  # cases collées au bâtiment : moins c'est mieux
            # cases de chemin (sol différent du sol principal ou des hautes herbes) sous le bâtiment : le moins possible
            nchemin = sum(1 for cx, cy in zone | {devant} if cases[cy][cx] & 0x3FF != sol and comp.get(cases[cy][cx] & 0x3FF, 0) != 2
                          and cases[cy][cx] & 0x3FF not in foret)
            elev = {(cases[cy][cx] >> 12) for cx, cy in zone if (cases[cy][cx] & 0x3FF) not in foret}
            if len(elev) != 1:
                continue
            if atteignables(zone) != avant - zone:
                continue
            candidats.append((nchemin * 100 + marge * 8 + abs(x - ref[0]) + abs(y - ref[1]), x, y))
    if not candidats:
        print(f'  {nom} : aucune place libre')
        return None
    candidats.sort()
    if nom in FORCE:
        x, y = FORCE[nom]
        score = next((c[0] for c in candidats if (c[1], c[2]) == (x, y)), -1)
    else:
        score, x, y = candidats[0]
    if DEBUG:
        print('   meilleurs :', candidats[:8])
    print(f'  {nom} : sol 0x{sol:03X}, score {score}')
    m['warp_events'].append({'x': x + PORTE, 'y': y + HAUTEUR - 1, 'elevation': 0, 'dest_map': HALL, 'dest_warp_id': '0'})
    with open(chemin, 'w', encoding='utf-8') as f:
        json.dump(m, f, indent=2, ensure_ascii=False)
        f.write('\n')
    # carte Carrière : la ville avec le bâtiment (collision sur les 16 cases, élévation d'origine)
    layouts = json.load(open(ROOT / 'data/layouts/layouts.json'))['layouts']
    lay = next(l for l in layouts if l.get('id') == m['layout'])
    src = ROOT / lay['blockdata_filepath']
    data = bytearray(src.read_bytes())
    for j in range(HAUTEUR):
        for i in range(LARGEUR):
            k = ((y + j) * w + (x + i)) * 2
            orig = int.from_bytes(data[k:k + 2], 'little')
            data[k:k + 2] = (METATUILES[j][i] | 0x0C00 | (cases[y + HAUTEUR][x + PORTE] & 0xF000)).to_bytes(2, 'little')
    sous_sol(nom, lay, x, y, cases, data, w)
    dst = src.with_name('map_carriere.bin')
    dst.write_bytes(bytes(data))
    return x, y, m['layout'], dst.relative_to(ROOT)


def dossier(nom_ts):
    n = nom_ts.replace('gTileset_', '').lower()
    for base in ('primary', 'secondary'):
        for d in (ROOT / 'data/tilesets' / base).iterdir():
            if d.name.replace('_', '').lower() == n:
                return d


REGISTRE = ROOT / 'tools/couronnement/hall_tuiles.json'  # nombre de métatuiles d'origine de chaque tileset secondaire


def sous_sol(nom, lay, x, y, cases, data, w):
    """Remplace l'herbe intégrée aux métatuiles du bâtiment par le sol d'origine de chaque case : métatuiles propres à la
    ville, ajoutées à la fin de son tileset secondaire (relançable grâce au registre du nombre de métatuiles d'origine)."""
    prim = dossier(lay['primary_tileset'])
    sec = dossier(lay['secondary_tileset'])
    pmt = (prim / 'metatiles.bin').read_bytes()
    pat = (prim / 'metatile_attributes.bin').read_bytes()
    registre = json.loads(REGISTRE.read_text()) if REGISTRE.exists() else {}
    cle = sec.name
    smt = bytearray((sec / 'metatiles.bin').read_bytes())
    sat = bytearray((sec / 'metatile_attributes.bin').read_bytes())
    if cle not in registre:
        registre[cle] = len(smt) // 16
    base = registre[cle]
    del smt[base * 16:]
    del sat[base * 2:]

    def entrees(mid):
        src, i = (pmt, mid) if mid < 512 else (smt, mid - 512)
        return [struct.unpack_from('<H', src, i * 16 + 2 * k)[0] for k in range(8)]

    ajoutees = 0
    for j in range(HAUTEUR):
        for i in range(LARGEUR):
            m = METATUILES[j][i]
            e = entrees(m)
            herbe = [k for k in range(4) if e[k] >> 12 == 2 and e[k] & 0x3FF in (2, 3)]
            orig = cases[y + j][x + i] & 0x3FF
            if not herbe or orig == 0x001:
                continue
            eo = entrees(orig)
            if all(eo[k] >> 12 == 2 and eo[k] & 0x3FF in (2, 3) for k in herbe):
                continue  # la case d'origine est déjà de l'herbe
            if base + ajoutees >= 512:
                print(f'  {nom} : plus de place dans {cle}, herbe gardée en ({x + i}, {y + j})')
                continue
            for k in herbe:
                e[k] = eo[k]
            smt += b''.join(struct.pack('<H', t) for t in e)
            sat += pat[2 * m:2 * m + 2]
            nouveau = 512 + base + ajoutees
            ajoutees += 1
            kk = ((y + j) * w + (x + i)) * 2
            ancien = int.from_bytes(data[kk:kk + 2], 'little')
            data[kk:kk + 2] = ((ancien & ~0x3FF) | nouveau).to_bytes(2, 'little')
    (sec / 'metatiles.bin').write_bytes(bytes(smt))
    (sec / 'metatile_attributes.bin').write_bytes(bytes(sat))
    REGISTRE.write_text(json.dumps(registre, indent=2) + '\n')
    print(f'  {nom} : {ajoutees} métatuiles sans herbe dans {cle}')


def main():
    blocs, lignes = [], []
    for k, (nom, const, tournoi) in enumerate(VILLES):
        if nom in HALL_DANS_ARENE:
            m = json.load(open(ROOT / 'data/maps' / nom / 'map.json', encoding='utf-8'))
            n = len(m['warp_events'])
            m['warp_events'] = [w for w in m['warp_events'] if w['dest_map'] != HALL]
            if len(m['warp_events']) != n:
                with open(ROOT / 'data/maps' / nom / 'map.json', 'w', encoding='utf-8') as f:
                    json.dump(m, f, indent=2, ensure_ascii=False)
                    f.write('\n')
            print(f'{nom} : hall dans l\'Arène ({HALL_DANS_ARENE[nom]})')
            lignes.append(f'    {{MAP_GROUP({const}), MAP_NUM({const}), 0, 0xFF, 0xFF, {tournoi}, NULL, '
                          f'MAP_GROUP({HALL_DANS_ARENE[nom]}), MAP_NUM({HALL_DANS_ARENE[nom]})}},')
            continue
        r = placer(nom)
        if r is None:
            continue
        x, y, layout, binaire = r
        print(f'{nom} : bâtiment en ({x}, {y}), porte en ({x + PORTE}, {y + HAUTEUR - 1})')
        blocs.append(f'static const u16 sHallCarte{k}[] = INCBIN_U16("{binaire.as_posix()}");')
        lignes.append(f'    {{MAP_GROUP({const}), MAP_NUM({const}), {layout}, {x}, {y}, {tournoi}, sHallCarte{k}, 0, 0}},')
    texte = ['// Généré par tools/couronnement/hall_placer.py : halls du Couronnement Mondial (édition Carrière).',
             '// Ville, layout, coin haut-gauche du bâtiment 4 x 4 (0xFF : hall dans l\'Arène), tournoi, carte avec le bâtiment, Arène.', *blocs, '',
             'static const struct TycoonHall sHalls[] =', '{', *lignes, '};']
    (ROOT / 'src/data/couronnement_halls.h').write_text('\n'.join(texte) + '\n', encoding='utf-8')


if __name__ == '__main__':
    main()
