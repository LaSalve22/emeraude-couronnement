#!/usr/bin/env python3
"""Aperçu du graphique de la Bourse (v0.38) à partir du journal de `make check TESTS="Graphique"`.

Usage : python3 tools/couronnement/apercu_graphique.py <journal> <sortie_prefixe>
Écrit <prefixe>_bougies.png et <prefixe>_courbe.png (240 x 104, agrandis x3).
"""
import re
import sys
import zlib
import struct

LARG_TUILES, HAUT_TUILES = 30, 13
PAL = [(0, 0, 0), (2, 3, 6), (31, 31, 31), (10, 11, 14), (6, 26, 10), (30, 8, 6), (7, 8, 12), (31, 26, 6)] + [(31, 0, 31)] * 8
ECHELLE = 3


def png(chemin, larg, haut, rgb):
    brut = b"".join(b"\x00" + bytes(rgb[y * larg * 3:(y + 1) * larg * 3]) for y in range(haut))
    def bloc(t, d):
        return struct.pack(">I", len(d)) + t + d + struct.pack(">I", zlib.crc32(t + d) & 0xFFFFFFFF)
    with open(chemin, "wb") as f:
        f.write(b"\x89PNG\r\n\x1a\n" + bloc(b"IHDR", struct.pack(">IIBBBBB", larg, haut, 8, 2, 0, 0, 0))
                + bloc(b"IDAT", zlib.compress(brut, 9)) + bloc(b"IEND", b""))


def main():
    journal, prefixe = sys.argv[1], sys.argv[2]
    donnees = {0: "", 1: ""}
    for ligne in open(journal, errors="replace"):
        m = re.search(r"GRAPH (\d):([0-9a-f]+)", ligne)
        if m:
            donnees[int(m.group(1))] += m.group(2)
    for mode, nom in ((0, "bougies"), (1, "courbe")):
        octets = bytes.fromhex(donnees[mode])
        larg, haut = LARG_TUILES * 8, HAUT_TUILES * 8
        pixels = [[0] * larg for _ in range(haut)]
        for t in range(LARG_TUILES * HAUT_TUILES):
            tx, ty = t % LARG_TUILES, t // LARG_TUILES
            for i in range(32):
                b = octets[t * 32 + i]
                y, x = i // 4, (i % 4) * 2
                pixels[ty * 8 + y][tx * 8 + x] = b & 15
                pixels[ty * 8 + y][tx * 8 + x + 1] = b >> 4
        rgb = []
        for y in range(haut * ECHELLE):
            for x in range(larg * ECHELLE):
                r, g, bl = PAL[pixels[y // ECHELLE][x // ECHELLE]]
                rgb += [r * 255 // 31, g * 255 // 31, bl * 255 // 31]
        png(f"{prefixe}_{nom}.png", larg * ECHELLE, haut * ECHELLE, rgb)


if __name__ == "__main__":
    main()
