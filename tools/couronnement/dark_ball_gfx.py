#!/usr/bin/env python3
"""Émeraude Couronnement : graphismes de la Dark Ball (ex-Étrange Ball) à partir du dessin d'Adrien.

- graphics/items/icons/strange_ball.png + icon_palettes/strange_ball.pal : icône 24x24 (sac, boutiques)
- graphics/balls/strange.png : Ball lancée en combat (3 images 16x16 : fermée = dessin réduit, ouverture recolorée)
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
    path = ROOT / 'graphics/balls/strange.png'
    old = Image.open(ROOT / 'graphics/couronnement/strange_ball_original.png')  # sprite d'origine, jamais modifié
    pal = (old.getpalette() + [0] * 48)[:48]
    # recolore la palette d'origine (vert / turquoise) en violet sombre / gris
    neuve = list(pal)
    for i in range(1, 16):
        r, g, b = pal[i*3:i*3+3]
        lum = (r + g + b) / 3
        if abs(r - g) < 12 and abs(g - b) < 12:  # gris / blanc / contour : garder
            continue
        if b > r + 20 and g > r + 20:  # turquoise -> gris des bandes
            v = int(lum * 0.6)
            neuve[i*3:i*3+3] = [v, v, v + 8]
        else:  # vert -> violet
            neuve[i*3:i*3+3] = [min(255, int(lum * 0.55)), int(lum * 0.3), min(255, int(lum * 0.95))]
    rgb_pal = [tuple(neuve[i*3:i*3+3]) for i in range(16)]
    # image fermée : dessin réduit à 12x12, couleurs ramenées à la palette commune
    d = contour(disque(12), (65, 65, 65, 255))
    img = old.copy()
    img.putpalette(neuve)
    px = img.load()
    for y in range(16):
        for x in range(16):
            px[x, y] = 0
    dp = d.load()
    for y in range(12):
        for x in range(12):
            r, g, b, a = dp[x, y]
            if a < 128:
                continue
            best = min(range(1, 16), key=lambda i: (rgb_pal[i][0]-r)**2 + (rgb_pal[i][1]-g)**2 + (rgb_pal[i][2]-b)**2)
            px[x + 2, y + 2] = best
    img.save(path, transparency=0)


if __name__ == '__main__':
    icone()
    balle()
    print('Dark Ball : icône et Ball de combat générées.')
