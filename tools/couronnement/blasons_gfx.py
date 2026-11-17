#!/usr/bin/env python3
"""Blasons des clubs (v0.45) tirés de la planche d'Adrien (2 rangées de 5, ordre des clubs 0 à 9).

Usage : python3 tools/couronnement/blasons_gfx.py <planche.png|webp>
Écrit graphics/couronnement/blasons/blason_N.png (dessin de 16 px de haut) et blason_petit_N.png (12 px, pour les lignes du
classement) : toile 16 x 16, indexé, 16 couleurs (0 = transparent), une palette par image.
"""
import sys
from pathlib import Path
from PIL import Image

ROOT = Path(__file__).resolve().parents[2]
TAILLE = 16  # toile du sprite


def zones(px, w, h, fond):
    def vide(c):
        return sum(abs(a - b) for a, b in zip(c, fond)) < 40

    def plages(v):
        out, s = [], None
        for i, b in enumerate(v + [False]):
            if b and s is None:
                s = i
            if not b and s is not None:
                if i - s > 20:
                    out.append((s, i))
                s = None
        return out
    lignes = plages([any(not vide(px[x, y]) for x in range(0, w, 2)) for y in range(h)])
    for y0, y1 in lignes:
        for x0, x1 in plages([any(not vide(px[x, y]) for y in range(y0, y1)) for x in range(w)]):
            ys = [y for y in range(y0, y1) if any(not vide(px[x, y]) for x in range(x0, x1, 2))]
            yield x0, ys[0], x1, ys[-1] + 1


def main():
    im = Image.open(sys.argv[1]).convert('RGB')
    px = im.load()
    fond = px[5, 5]
    for n, (x0, y0, x1, y1) in enumerate(zones(px, *im.size, fond)):
        bout = im.crop((x0, y0, x1, y1))
        masque = Image.new('L', bout.size, 0)
        mp, bp = masque.load(), bout.load()
        for y in range(bout.size[1]):
            for x in range(bout.size[0]):
                c = bp[x, y]
                mp[x, y] = 0 if sum(abs(a - b) for a, b in zip(c, fond)) < 40 else 255
        for haut, nom in ((16, f'blason_{n}'), (12, f'blason_petit_{n}')):
            reduire(bout, masque, haut, nom)


def reduire(bout, masque, haut, nom):
    if True:
        cote = max(bout.size)
        k = haut / cote
        tw, th = max(1, round(bout.size[0] * k)), max(1, round(bout.size[1] * k))
        petit = bout.resize((tw, th), Image.LANCZOS)
        pmask = masque.resize((tw, th), Image.BOX)
        q = petit.quantize(15, method=Image.MEDIANCUT, dither=Image.NONE)
        pal = q.getpalette()[:45]
        sortie = Image.new('P', (TAILLE, TAILLE), 0)
        sortie.putpalette([255, 0, 255] + pal + [0] * (768 - 48))
        ox, oy = (TAILLE - tw) // 2, (TAILLE - th) // 2
        qp, mp2, sp = q.load(), pmask.load(), sortie.load()
        for y in range(th):
            for x in range(tw):
                if mp2[x, y] >= 128:
                    sp[ox + x, oy + y] = qp[x, y] + 1
        sortie.save(ROOT / f'graphics/couronnement/blasons/{nom}.png', transparency=0)
        print(f'{nom} : {bout.size} -> {tw}x{th}')


if __name__ == '__main__':
    main()
