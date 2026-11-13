#!/usr/bin/env python3
"""Émeraude Couronnement : étiquettes de type en français (graphics/types/<type>.png, 32x16).

Les originaux anglais sont gardés dans graphics/couronnement/types_en/ (copiés au premier lancement).
On garde le cadre et le fond, on efface le texte et on écrit le nom français (police 3x7, ombre à droite et dessous).
"""
from collections import Counter
from pathlib import Path
import shutil
from PIL import Image

ROOT = Path(__file__).resolve().parents[2]
TYPES = ROOT / 'graphics/types'
ORIG = ROOT / 'graphics/couronnement/types_en'

NOMS = {
    'normal': 'NORMAL', 'fight': 'COMBAT', 'flying': 'VOL', 'poison': 'POISON', 'ground': 'SOL', 'rock': 'ROCHE',
    'bug': 'INSECTE', 'ghost': 'SPECTRE', 'steel': 'ACIER', 'fire': 'FEU', 'water': 'EAU', 'grass': 'PLANTE',
    'electric': 'ÉLECTRIK', 'psychic': 'PSY', 'ice': 'GLACE', 'dragon': 'DRAGON', 'dark': 'TÉNÈBRES', 'fairy': 'FÉE',
    'stellar': 'STELLAIRE',
}

# lettres 7 lignes ; '#' = texte. Les accents se mettent sur la ligne au-dessus (une seule ligne libre).
G = {
    'A': ['.#.', '#.#', '#.#', '###', '#.#', '#.#', '#.#'],
    'B': ['##.', '#.#', '#.#', '##.', '#.#', '#.#', '##.'],
    'C': ['.##', '#..', '#..', '#..', '#..', '#..', '.##'],
    'D': ['##.', '#.#', '#.#', '#.#', '#.#', '#.#', '##.'],
    'E': ['###', '#..', '#..', '##.', '#..', '#..', '###'],
    'F': ['###', '#..', '#..', '##.', '#..', '#..', '#..'],
    'G': ['.##', '#..', '#..', '#.#', '#.#', '#.#', '.##'],
    'I': ['#', '#', '#', '#', '#', '#', '#'],
    'K': ['#.#', '#.#', '##.', '#..', '##.', '#.#', '#.#'],
    'L': ['#.', '#.', '#.', '#.', '#.', '#.', '##'],
    'M': ['#...#', '##.##', '#.#.#', '#...#', '#...#', '#...#', '#...#'],
    'N': ['#..#', '##.#', '##.#', '#.##', '#.##', '#..#', '#..#'],
    'O': ['.#.', '#.#', '#.#', '#.#', '#.#', '#.#', '.#.'],
    'P': ['##.', '#.#', '#.#', '##.', '#..', '#..', '#..'],
    'R': ['##.', '#.#', '#.#', '##.', '#.#', '#.#', '#.#'],
    'S': ['.##', '#..', '#..', '.#.', '..#', '..#', '##.'],
    'T': ['###', '.#.', '.#.', '.#.', '.#.', '.#.', '.#.'],
    'U': ['#.#', '#.#', '#.#', '#.#', '#.#', '#.#', '.#.'],
    'V': ['#.#', '#.#', '#.#', '#.#', '#.#', '.#.', '.#.'],
    'Y': ['#.#', '#.#', '#.#', '.#.', '.#.', '.#.', '.#.'],
    'H': ['#.#', '#.#', '#.#', '###', '#.#', '#.#', '#.#'],
}
ACCENTS = {'É': ('E', '..#'), 'È': ('E', '#..')}
TEXTE, OMBRE = 15, 14


def glyphe(ch):
    if ch in ACCENTS:
        base, acc = ACCENTS[ch]
        return G[base], acc
    return G[ch], None


def largeur(nom):
    return sum(len(glyphe(c)[0][0]) + 1 for c in nom)


def dessiner(typ, nom):
    src = ORIG / f'{typ}.png'
    if not src.exists():
        shutil.copy(TYPES / f'{typ}.png', src)
    im = Image.open(src).copy()
    px = im.load()
    fond = px[16, 3]  # ligne vide sous le cadre (le texte peut être plus abondant que le fond)
    # couleurs du texte d'origine : tout ce qui n'est ni le fond ni le cadre dans la zone du texte ;
    # la plus claire = texte, l'autre = ombre
    pal = im.getpalette()
    cadre = {px[0, 3], px[31, 3]}
    autres = Counter(px[x, y] for y in range(4, 12) for x in range(2, 30) if px[x, y] != fond and px[x, y] not in cadre)
    idx = [i for i, _ in autres.most_common(2)]
    lum = lambda i: sum(pal[i * 3:i * 3 + 3])
    texte_c, ombre_c = (max(idx, key=lum), min(idx, key=lum)) if len(idx) == 2 else (TEXTE, OMBRE)
    for y in range(3, 13):
        for x in range(32):
            if px[x, y] in (texte_c, ombre_c) and px[x, y] != fond:
                px[x, y] = fond
    w = largeur(nom)
    if w - 1 > 32:  # la dernière ombre peut sortir
        raise SystemExit(f'{nom} : trop long ({w} px)')
    x = max(0, (32 - w + 1) // 2)
    texte = set()
    for ch in nom:
        g, acc = glyphe(ch)
        for dy, row in enumerate(g):
            for dx, c in enumerate(row):
                if c == '#':
                    texte.add((x + dx, 4 + dy))
        if acc:
            for dx, c in enumerate(acc):
                if c == '#':
                    texte.add((x + dx, 3))
        x += len(g[0]) + 1
    for (tx, ty) in texte:
        for ox, oy in ((1, 0), (0, 1), (1, 1)):
            p = (tx + ox, ty + oy)
            if p not in texte and p[0] < 32 and p[1] <= 11:
                px[p] = ombre_c
    for p in texte:
        px[p] = texte_c
    im.save(TYPES / f'{typ}.png')


if __name__ == '__main__':
    ORIG.mkdir(parents=True, exist_ok=True)
    for t, n in NOMS.items():
        dessiner(t, n)
    print(f'{len(NOMS)} étiquettes de type en français.')
