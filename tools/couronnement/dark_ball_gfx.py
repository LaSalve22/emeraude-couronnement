#!/usr/bin/env python3
"""Émeraude Couronnement : graphismes de la Dark Ball (ex-Étrange Ball) à partir du dessin d'Adrien.

- graphics/items/icons/strange_ball.png + icon_palettes/strange_ball.pal : icône 24x24 (sac, boutiques)
- graphics/balls/strange.png : Ball lancée en combat (3 images 16x16, forme ronde de l'Hyper Ball, violet clair)
Sources : graphics/couronnement/dark_ball_source.jpg et strange_ball_original.png (fond en damier « transparent » incrusté : on découpe le disque).
"""
from pathlib import Path
from PIL import Image, ImageDraw, ImageEnhance

ROOT = Path(__file__).resolve().parents[2]
SRC = ROOT / 'graphics/couronnement/dark_ball_source.jpg'


def disque(taille):
    src = Image.open(SRC).convert('RGB')
    w, h = src.size
    masque = Image.new('L', src.size, 0)
    m = 14  # marge : enlève le damier au bord du disque
    ImageDraw.Draw(masque).ellipse((m, m, w - m, h - m), fill=255)
    src = ImageEnhance.Contrast(src).enhance(1.5)
    src = ImageEnhance.Color(src).enhance(1.6)
    src = ImageEnhance.Brightness(src).enhance(1.25)
    rgba = src.convert('RGBA')
    rgba.putalpha(masque)
    return rgba.resize((taille, taille), Image.LANCZOS)


def contour(rgba, couleur=(24, 16, 32, 255)):
    """Contour d'1 pixel foncé autour du disque, comme les autres Balls."""
    out = rgba.copy()
    a = rgba.split()[3].load()
    w, h = rgba.size
    for y in range(h):
        for x in range(w):
            if a[x, y] >= 128 and any(not (0 <= x+dx < w and 0 <= y+dy < h) or a[x+dx, y+dy] < 128
                                      for dx, dy in ((1, 0), (-1, 0), (0, 1), (0, -1))):
                out.putpixel((x, y), couleur)
    return out


def quantifier(rgba, couleurs):
    """Image P : index 0 transparent, couleurs 1..n."""
    opaque = Image.new('RGB', rgba.size, (0, 0, 0))
    opaque.paste(rgba, mask=rgba.split()[3])
    q = opaque.quantize(couleurs, method=Image.MEDIANCUT)
    pal = q.getpalette()[:couleurs * 3]
    out = Image.new('P', rgba.size, 0)
    out.putpalette([0, 0, 0] + pal + [0, 0, 0] * (15 - couleurs))
    a = rgba.split()[3].load()
    qp, op = q.load(), out.load()
    for y in range(rgba.size[1]):
        for x in range(rgba.size[0]):
            if a[x, y] >= 128:
                op[x, y] = qp[x, y] + 1
    return out


def ecrire_jasc(path, palette):
    lignes = ['JASC-PAL', '0100', '16'] + [f'{palette[i*3]} {palette[i*3+1]} {palette[i*3+2]}' for i in range(16)]
    path.write_text('\n'.join(lignes) + '\n')


def icone():
    ic = Image.new('RGBA', (24, 24), (0, 0, 0, 0))
    ic.paste(disque(22), (1, 1))
    p = quantifier(contour(ic), 15)
    pal = p.getpalette()[:48]
    pal[0:3] = [180, 180, 180]  # couleur transparente (convention des icônes)
    p.putpalette(pal)
    p.save(ROOT / 'graphics/items/icons/strange_ball.png', transparency=0)
    ecrire_jasc(ROOT / 'graphics/items/icon_palettes/strange_ball.pal', pal)


def balle():
    """Ball de combat : silhouette ronde de l'Hyper Ball, violet clair, bandes grises en X et bouton argent
    (comme le dessin) ; les images d'ouverture sont celles de l'Hyper Ball recolorées en violet."""
    import math
    ultra = Image.open(ROOT / 'graphics/balls/ultra.png')
    u = ultra.load()
    pal = [(255, 255, 255), (88, 56, 136), (124, 88, 184), (160, 128, 216), (208, 184, 248), (255, 255, 255),
           (176, 172, 192), (56, 48, 72), (104, 100, 120)] + [(255, 255, 255)] * 7
    out = Image.new('P', (16, 48), 0)
    o = out.load()
    for y in range(16, 48):
        for x in range(16):
            o[x, y] = u[x, y]
    masque = {(x, y) for y in range(16) for x in range(16) if u[x, y]}

    def bord(x, y):
        return any((x + dx, y + dy) not in masque for dx, dy in ((1, 0), (-1, 0), (0, 1), (0, -1)))

    for (x, y) in masque:
        if bord(x, y):
            o[x, y] = 7
            continue
        d = math.hypot(x - 5.5, y - 5)  # lumière en haut à gauche
        c = 4 if d < 1.6 else 3 if d < 3.6 else 2 if d < 5.6 else 1
        dx, dy = x - 7.5, y - 9.5
        if y >= 5 and (abs(dx - dy * 0.8) < 0.7 or abs(dx + dy * 0.8) < 0.7):
            c = 6 if d < 2.5 else 8
        o[x, y] = c
    for p, c in (((10, 4), 5), ((11, 5), 6), ((10, 5), 6), ((9, 4), 6)):
        if p in masque and not bord(*p):
            o[p] = c
    out.putpalette([v for t in pal for v in t])
    out.save(ROOT / 'graphics/balls/strange.png', transparency=0)


if __name__ == '__main__':
    icone()
    balle()
    print('Dark Ball : icône et Ball de combat générées.')
