#!/usr/bin/env python3
"""Façade du hall du Couronnement Mondial (v0.56, édition Carrière) : un Centre Pokémon au toit doré, sans l'enseigne « P.C ».

Écrit 11 métatuiles neuves dans des emplacements inutilisés du tileset commun (gTileset_General) : copies des métatuiles du
Centre Pokémon avec la palette 1 (toit rouge) remplacée par la palette 5 (toit doré), enseigne remplacée par du mur.
La porte reste la métatuile d'origine (0x61) pour garder son animation. Relançable.
Usage : python3 tools/couronnement/hall_gfx.py [apercu.png]
"""
import struct
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
TS = ROOT / 'data/tilesets/primary/general'
# (source, emplacement libre) ; None = garder la métatuile d'origine
PLAN = [[(0x50, 0x2A), (0x51, 0x5C), (0x52, 0x135), (0x53, 0x13F)],
        [(0x58, 0x1A2), (0x59, 0x1B6), (0x5A, 0x1E3), (0x5B, 0xDB)],
        [(0x60, 0xDC), (0x61, None), (0x62, 0xDD), (0x63, 0xDE)]]
MUR = 353  # tuile de mur blanc (remplace l'enseigne 327 / 328)


def main():
    mt = bytearray((TS / 'metatiles.bin').read_bytes())
    at = bytearray((TS / 'metatile_attributes.bin').read_bytes())
    for ligne in PLAN:
        for src, dst in ligne:
            if dst is None:
                continue
            for k in range(8):
                t = struct.unpack_from('<H', mt, src * 16 + 2 * k)[0]
                if t >> 12 == 1:
                    t = (t & 0x0FFF) | (5 << 12)
                if t & 0x3FF in (327, 328):
                    t = (t & ~0x3FF) | MUR
                struct.pack_into('<H', mt, dst * 16 + 2 * k, t)
            at[2 * dst:2 * dst + 2] = at[2 * src:2 * src + 2]
    (TS / 'metatiles.bin').write_bytes(bytes(mt))
    (TS / 'metatile_attributes.bin').write_bytes(bytes(at))
    ids = [[(dst if dst is not None else src) for src, dst in ligne] for ligne in PLAN]
    print('métatuiles du hall :', [[hex(i) for i in l] for l in ids])
    if len(sys.argv) > 1:
        apercu(ids, sys.argv[1])


def apercu(ids, chemin):
    from PIL import Image
    tiles = Image.open(TS / 'tiles.png')
    tw = tiles.width // 8
    px = tiles.load()
    pals = []
    for i in range(16):
        f = TS / 'palettes' / ('%02d.pal' % i)
        pals.append([tuple(map(int, l.split())) for l in f.read_text().split('\n')[3:19]] if f.exists() else None)
    mt = (TS / 'metatiles.bin').read_bytes()
    img = Image.new('RGB', (64, 48), (90, 160, 90))
    for r, ligne in enumerate(ids):
        for c, m in enumerate(ligne):
            e = [struct.unpack_from('<H', mt, m * 16 + 2 * k)[0] for k in range(8)]
            for couche in range(2):
                for k in range(4):
                    t = e[couche * 4 + k]
                    idx, hf, vf, p = t & 0x3FF, (t >> 10) & 1, (t >> 11) & 1, pals[t >> 12]
                    if idx >= 512 or p is None:
                        continue
                    for y in range(8):
                        for x in range(8):
                            v = px[(idx % tw) * 8 + (7 - x if hf else x), (idx // tw) * 8 + (7 - y if vf else y)]
                            v = v[0] if isinstance(v, tuple) else v
                            if couche == 1 and v == 0:
                                continue
                            img.putpixel((c * 16 + (k % 2) * 8 + x, r * 16 + (k // 2) * 8 + y), p[v])
    img.resize((192, 144), Image.NEAREST).save(chemin)


if __name__ == '__main__':
    main()
