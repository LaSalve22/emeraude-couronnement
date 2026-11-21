#!/usr/bin/env python3
"""Rendu PNG d'une zone de carte (métatuiles des tilesets primaire + secondaire), pour vérifier un placement.
Usage : python3 tools/couronnement/apercu_carte.py <Ville> <x> <y> <largeur> <hauteur> <sortie.png> [carriere]"""
import json
import struct
import sys
from pathlib import Path
from PIL import Image

ROOT = Path(__file__).resolve().parents[2]


def dossier_tileset(nom):
    n = nom.replace('gTileset_', '').lower()
    for base in ('primary', 'secondary'):
        for d in (ROOT / 'data/tilesets' / base).iterdir():
            if d.name.replace('_', '').lower() == n:
                return d


def charger(d):
    t = Image.open(d / 'tiles.png')
    pals = {}
    for i in range(16):
        f = d / 'palettes' / ('%02d.pal' % i)
        if f.exists():
            pals[i] = [tuple(map(int, l.split())) for l in f.read_text().split('\n')[3:19]]
    return t, pals, (d / 'metatiles.bin').read_bytes()


def main():
    ville, x0, y0, lw, lh, sortie = sys.argv[1], *map(int, sys.argv[2:6]), sys.argv[6]
    carriere = len(sys.argv) > 7
    m = json.load(open(ROOT / 'data/maps' / ville / 'map.json'))
    lay = next(l for l in json.load(open(ROOT / 'data/layouts/layouts.json'))['layouts'] if l.get('id') == m['layout'])
    w = lay['width']
    f = ROOT / lay['blockdata_filepath']
    if carriere:
        f = f.with_name('map_carriere.bin')
    b = f.read_bytes()
    pri, sec = charger(dossier_tileset(lay['primary_tileset'])), charger(dossier_tileset(lay['secondary_tileset']))
    pals = dict(pri[1])
    pals.update({k: v for k, v in sec[1].items() if k >= 6})
    img = Image.new('RGB', (lw * 16, lh * 16))
    for my in range(lh):
        for mx in range(lw):
            bloc = struct.unpack_from('<H', b, 2 * ((y0 + my) * w + x0 + mx))[0] & 0x3FF
            src = pri[2] if bloc < 512 else sec[2]
            e = [struct.unpack_from('<H', src, (bloc % 512) * 16 + 2 * k)[0] for k in range(8)]
            for couche in range(2):
                for k in range(4):
                    t = e[couche * 4 + k]
                    idx, hf, vf, p = t & 0x3FF, (t >> 10) & 1, (t >> 11) & 1, pals.get(t >> 12)
                    tiles = pri[0] if idx < 512 else sec[0]
                    idx %= 512
                    tw = tiles.width // 8
                    if p is None or idx // tw >= tiles.height // 8:
                        continue
                    px = tiles.load()
                    for yy in range(8):
                        for xx in range(8):
                            v = px[(idx % tw) * 8 + (7 - xx if hf else xx), (idx // tw) * 8 + (7 - yy if vf else yy)]
                            v = v[0] if isinstance(v, tuple) else v
                            if couche == 1 and v == 0:
                                continue
                            img.putpixel((mx * 16 + (k % 2) * 8 + xx, my * 16 + (k // 2) * 8 + yy), p[v])
    img.save(sortie)


if __name__ == '__main__':
    main()
