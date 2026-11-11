#!/usr/bin/env python3
"""Redessine en français les titres de l'écran de résumé (graphics/summary_screen/tiles.png).

Police 5x7 du jeu (extraite de l'image d'origine) + ombre portée d'un pixel vers la droite et le bas.
Chaque titre occupe une bande de 8 px de haut : (y, x de début de bande, x de fin de bande).
Relançable (la bande est effacée puis réécrite). Pas d'accents : la police n'a pas la place au-dessus des capitales.
"""
from pathlib import Path
from PIL import Image

ROOT = Path(__file__).resolve().parents[2]
PNG = ROOT / 'graphics/summary_screen/tiles.png'

FONT = {
    '.': ('.....', '.....', '.....', '.....', '.....', '##...', '##...'),
    'A': ('.###.', '#...#', '#...#', '#...#', '#####', '#...#', '#...#'),
    'B': ('####.', '#...#', '#...#', '####.', '#...#', '#...#', '####.'),
    'C': ('.###.', '#...#', '#....', '#....', '#....', '#...#', '.###.'),
    'D': ('####.', '#...#', '#...#', '#...#', '#...#', '#...#', '####.'),
    'E': ('#####', '#....', '#....', '####.', '#....', '#....', '#####'),
    'F': ('#####', '#....', '#....', '####.', '#....', '#....', '#....'),
    'I': ('#####', '..#..', '..#..', '..#..', '..#..', '..#..', '#####'),
    'J': ('....#', '....#', '....#', '....#', '#...#', '#...#', '.###.'),
    'L': ('#....', '#....', '#....', '#....', '#....', '#....', '#####'),
    'M': ('#...#', '##.##', '#.#.#', '#...#', '#...#', '#...#', '#...#'),
    'N': ('#...#', '##..#', '#.#.#', '#..##', '#...#', '#...#', '#...#'),
    'O': ('.###.', '#...#', '#...#', '#...#', '#...#', '#...#', '.###.'),
    'P': ('####.', '#...#', '#...#', '#...#', '####.', '#....', '#....'),
    'Q': ('.###.', '#...#', '#...#', '#...#', '#.#.#', '#..#.', '.##.#'),
    'R': ('####.', '#...#', '#...#', '#...#', '####.', '#..#.', '#...#'),
    'S': ('.###.', '#...#', '#....', '.###.', '....#', '#...#', '.###.'),
    'T': ('#####', '..#..', '..#..', '..#..', '..#..', '..#..', '..#..'),
    'U': ('#...#', '#...#', '#...#', '#...#', '#...#', '#...#', '.###.'),
    'V': ('#...#', '#...#', '#...#', '#...#', '.#.#.', '.#.#.', '..#..'),
    'X': ('#...#', '.#.#.', '..#..', '..#..', '..#..', '.#.#.', '#...#'),
    'Y': ('#...#', '#...#', '.#.#.', '..#..', '..#..', '..#..', '..#..'),
}
BLANC, OMBRE = 2, 6  # couleurs de la banque 0

# (texte, y, début de bande, fin de bande)
TITRES = [
    ('PROFIL', 64, 0, 72), ('TALENT', 72, 0, 72), ('ETAT', 88, 0, 72), ('CAPACITES', 96, 0, 72),
    ('DESCRIPTION', 104, 0, 80), ('MEMO DRESSEUR', 112, 0, 80),
    ('OBJET', 64, 72, 128), ('LIEN', 72, 72, 128), ('STATS', 80, 72, 128), ('EXP.', 88, 72, 128),
    ('EFFET', 96, 80, 128),
]


def main():
    im = Image.open(PNG)
    px = im.load()
    for texte, y0, xa, xb in TITRES:
        # effacer : les lettres (BLANC / OMBRE, banque 0) prennent la couleur de fond la plus fréquente de la bande
        from collections import Counter
        fond = Counter(px[x, y] for y in range(y0, y0 + 8) for x in range(xa, xb)
                       if px[x, y] not in (BLANC, OMBRE)).most_common(1)[0][0]
        for y in range(y0, y0 + 8):
            for x in range(xa, xb):
                if px[x, y] in (BLANC, OMBRE):
                    px[x, y] = fond
        blanc = set()
        x = xa + 2
        for ch in texte:
            if ch == ' ':
                x += 3
                continue
            for dy, row in enumerate(FONT[ch]):
                for dx, c in enumerate(row):
                    if c == '#':
                        blanc.add((x + dx, y0 + dy))
            x += 6
        if x > xb:
            raise SystemExit(f'{texte} : trop long ({x} > {xb})')
        for (bx, by) in blanc:
            for ox, oy in ((1, 0), (0, 1), (1, 1)):
                p = (bx + ox, by + oy)
                if p not in blanc and p[1] < y0 + 8 and p[0] < xb:
                    px[p] = OMBRE
        for p in blanc:
            px[p] = BLANC
    im.save(PNG)
    print(f'{len(TITRES)} titres redessinés.')


if __name__ == '__main__':
    main()
