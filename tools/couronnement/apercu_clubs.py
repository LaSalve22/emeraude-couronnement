#!/usr/bin/env python3
"""Aperçu du tableau de bord de la Ligue des Clubs (v0.42) à partir du journal de `make check TESTS="Clubs"`.

Usage : python3 tools/couronnement/apercu_clubs.py <journal> <sortie_prefixe>
Écrit <prefixe>_classement.png, _match.png, _match2.png, _effectif.png (240 x 104, agrandis x3).
"""
import re
import sys
import zlib
import struct

LARG_TUILES, HAUT_TUILES = 30, 13
# Palette de src/club_dashboard.c (indice 4 = couleur du club, ici FC Poivressel).
PAL = [(0, 0, 0), (2, 3, 6), (31, 31, 31), (8, 9, 12), (4, 12, 28), (31, 25, 6), (7, 8, 12), (17, 18, 21),
       (8, 27, 10), (30, 9, 7), (5, 6, 11)] + [(31, 0, 31)] * 5
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
    donnees = {i: "" for i in range(6)}
    for ligne in open(journal, errors="replace"):
        m = re.search(r"CLUB (\d):([0-9a-f]+)", ligne)
        if m:
            donnees[int(m.group(1))] += m.group(2)
    for mode, nom in ((0, "classement"), (1, "match"), (2, "match2"), (3, "effectif"), (4, "mercato"), (5, "finances")):
        if not donnees[mode]:
            continue
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
