#!/usr/bin/env python3
"""Émeraude Couronnement : jaquette originale (1200x1200) -> docs/jaquette/jaquette.png.
Pas de logo ni de personnage officiel : titre, émeraude, couronne et Dark Ball (dessin d'Adrien) dessinés ici."""
import math
from pathlib import Path
from PIL import Image, ImageDraw, ImageFilter, ImageFont

ROOT = Path(__file__).resolve().parents[2]
W = H = 1200
SUP = '/System/Library/Fonts/Supplemental/'


def font(name, size, index=0):
    return ImageFont.truetype(SUP + name, size, index=index)


def fond():
    img = Image.new('RGB', (W, H))
    px = img.load()
    cx, cy = W / 2, H * 0.56
    for y in range(H):
        for x in range(W):
            d = min(1, math.hypot(x - cx, y - cy) / 820)
            px[x, y] = (int(14 * (1 - d) + 4), int(92 * (1 - d) ** 1.4 + 8), int(62 * (1 - d) ** 1.4 + 14))
    # rayons de lumière
    rays = Image.new('L', (W, H), 0)
    dr = ImageDraw.Draw(rays)
    for i in range(24):
        a = i * math.pi / 12
        pts = [(cx, cy)] + [(cx + 1800 * math.cos(a + s), cy + 1800 * math.sin(a + s)) for s in (-0.05, 0.05)]
        dr.polygon(pts, fill=34)
    rays = rays.filter(ImageFilter.GaussianBlur(10))
    img = Image.composite(Image.new('RGB', (W, H), (120, 230, 170)), img, rays)
    return img


def emeraude(img, cx, cy, r):
    glow = Image.new('L', (W, H), 0)
    ImageDraw.Draw(glow).ellipse((cx - r * 1.5, cy - r * 1.5, cx + r * 1.5, cy + r * 1.5), fill=150)
    img.paste((90, 255, 170), mask=glow.filter(ImageFilter.GaussianBlur(70)))
    d = ImageDraw.Draw(img)
    # taille émeraude : octogone allongé, table centrale et facettes
    rx, ry = r * 0.78, r
    c = 0.32
    ext = [(cx - rx * (1 - c), cy - ry), (cx + rx * (1 - c), cy - ry), (cx + rx, cy - ry * (1 - c)), (cx + rx, cy + ry * (1 - c)),
           (cx + rx * (1 - c), cy + ry), (cx - rx * (1 - c), cy + ry), (cx - rx, cy + ry * (1 - c)), (cx - rx, cy - ry * (1 - c))]
    k = 0.55
    tab = [(cx + (x - cx) * k, cy + (y - cy) * k) for x, y in ext]
    teintes = [(120, 240, 170), (60, 200, 130), (20, 140, 90), (10, 100, 65), (15, 120, 80), (30, 160, 105), (70, 210, 140), (150, 250, 195)]
    for i in range(8):
        j = (i + 1) % 8
        d.polygon([ext[i], ext[j], tab[j], tab[i]], fill=teintes[i])
    d.polygon(tab, fill=(40, 185, 120))
    # reflets sur la table
    d.polygon([tab[0], tab[1], (cx + rx * 0.1, cy - ry * 0.05), (cx - rx * 0.3, cy - ry * 0.05)], fill=(70, 215, 145))
    d.polygon([(cx - rx * 0.32, cy - ry * 0.45), (cx - rx * 0.12, cy - ry * 0.45), (cx - rx * 0.3, cy + ry * 0.1), (cx - rx * 0.42, cy + ry * 0.1)],
              fill=(190, 255, 220))
    d.line(ext + [ext[0]], fill=(8, 50, 35), width=6)
    for a, b in zip(ext, tab):
        d.line([a, b], fill=(12, 70, 48), width=2)
    d.line(tab + [tab[0]], fill=(12, 70, 48), width=2)
    # étincelles
    for sx, sy, s in ((cx + rx * 0.55, cy - ry * 0.7, 34), (cx - rx * 1.25, cy + ry * 0.35, 22), (cx + rx * 1.2, cy + ry * 0.6, 18)):
        d.polygon([(sx, sy - s), (sx + s * 0.18, sy - s * 0.18), (sx + s, sy), (sx + s * 0.18, sy + s * 0.18),
                   (sx, sy + s), (sx - s * 0.18, sy + s * 0.18), (sx - s, sy), (sx - s * 0.18, sy - s * 0.18)], fill=(240, 255, 245))


def couronne(img, cx, base, w):
    d = ImageDraw.Draw(img)
    h = w * 0.55
    l, r = cx - w / 2, cx + w / 2
    pts = [(l, base), (l - w * 0.04, base - h * 0.75), (l + w * 0.22, base - h * 0.35), (l + w * 0.32, base - h),
           (cx, base - h * 0.42), (r - w * 0.32, base - h), (r - w * 0.22, base - h * 0.35), (r + w * 0.04, base - h * 0.75), (r, base)]
    pts = [(l, base), (l - w * 0.05, base - h * 0.8), (l + w * 0.2, base - h * 0.4), (l + w * 0.3, base - h * 0.95),
           (cx - w * 0.1, base - h * 0.42), (cx, base - h * 1.15), (cx + w * 0.1, base - h * 0.42), (r - w * 0.3, base - h * 0.95),
           (r - w * 0.2, base - h * 0.4), (r + w * 0.05, base - h * 0.8), (r, base)]
    d.polygon(pts, fill=(232, 182, 48), outline=(90, 55, 10), width=6)
    d.polygon([(l + w * 0.06, base - h * 0.12), (l + w * 0.03, base - h * 0.55), (l + w * 0.2, base - h * 0.28)], fill=(255, 226, 120))
    d.rounded_rectangle((l - 8, base - 6, r + 8, base + h * 0.24), 10, fill=(214, 160, 36), outline=(90, 55, 10), width=6)
    for i, x in enumerate((l + w * 0.18, cx, r - w * 0.18)):
        col = (220, 40, 60) if i != 1 else (40, 200, 120)
        y = base + h * 0.09
        d.ellipse((x - 16, y - 13, x + 16, y + 13), fill=col, outline=(90, 55, 10), width=3)
    for x, y in ((l - w * 0.05, base - h * 0.8), (l + w * 0.3, base - h * 0.95), (cx, base - h * 1.15),
                 (r - w * 0.3, base - h * 0.95), (r + w * 0.05, base - h * 0.8)):
        d.ellipse((x - 13, y - 13, x + 13, y + 13), fill=(255, 236, 150), outline=(90, 55, 10), width=4)


def texte_or(img, txt, f, cy, spacing=0):
    d = ImageDraw.Draw(img)
    bb = d.textbbox((0, 0), txt, font=f, stroke_width=14)
    x = (W - (bb[2] - bb[0])) / 2 - bb[0]
    y = cy - (bb[3] - bb[1]) / 2 - bb[1]
    ombre = Image.new('L', (W, H), 0)
    ImageDraw.Draw(ombre).text((x + 8, y + 12), txt, font=f, fill=200, stroke_width=14, stroke_fill=200)
    img.paste((0, 0, 0), mask=ombre.filter(ImageFilter.GaussianBlur(8)))
    d.text((x, y), txt, font=f, fill=(20, 60, 40), stroke_width=14, stroke_fill=(10, 40, 28))
    d.text((x, y), txt, font=f, fill=(250, 250, 240), stroke_width=7, stroke_fill=(250, 245, 225))
    masque = Image.new('L', (W, H), 0)
    ImageDraw.Draw(masque).text((x, y), txt, font=f, fill=255)
    grad = Image.new('RGB', (W, H))
    top, bot = y + bb[1], y + bb[3]
    gd = ImageDraw.Draw(grad)
    for yy in range(H):
        t = min(1, max(0, (yy - top) / max(1, bot - top)))
        c1, c2, c3 = (255, 240, 150), (240, 185, 40), (170, 100, 15)
        c = [int(a + (b - a) * (t * 2)) for a, b in zip(c1, c2)] if t < 0.5 else [int(a + (b - a) * (t * 2 - 1)) for a, b in zip(c2, c3)]
        gd.line([(0, yy), (W, yy)], fill=tuple(c))
    img.paste(grad, mask=masque)


def texte_centre(img, txt, f, cy, fill, stroke=0, stroke_fill=None, track=0):
    d = ImageDraw.Draw(img)
    largeurs = [d.textlength(ch, font=f) for ch in txt]
    total = sum(largeurs) + track * (len(txt) - 1)
    x = (W - total) / 2
    for ch, lw in zip(txt, largeurs):
        d.text((x, cy), ch, font=f, fill=fill, anchor='lm', stroke_width=stroke, stroke_fill=stroke_fill)
        x += lw + track


def dark_ball(img, cx, cy, r):
    src = Image.open(ROOT / 'graphics/couronnement/dark_ball_source.jpg').convert('RGB')
    m = Image.new('L', src.size, 0)
    ImageDraw.Draw(m).ellipse((14, 14, src.size[0] - 14, src.size[1] - 14), fill=255)
    src = src.resize((2 * r, 2 * r), Image.LANCZOS)
    m = m.resize((2 * r, 2 * r), Image.LANCZOS)
    aura = Image.new('L', (W, H), 0)
    ImageDraw.Draw(aura).ellipse((cx - r * 1.4, cy - r * 1.4, cx + r * 1.4, cy + r * 1.4), fill=190)
    img.paste((120, 40, 200), mask=aura.filter(ImageFilter.GaussianBlur(28)))
    img.paste(src, (cx - r, cy - r), m)


def main():
    img = fond()
    emeraude(img, W // 2, 760, 220)
    couronne(img, W // 2, 548, 270)
    dark_ball(img, 1010, 1010, 92)
    texte_or(img, 'ÉMERAUDE', font('Impact.ttf', 210), 160)
    texte_centre(img, 'COURONNEMENT MONDIAL', font('Copperplate.ttc', 62, 1), 300, (235, 255, 240), 4, (8, 40, 28), 6)
    d = ImageDraw.Draw(img)
    d.rectangle((0, H - 92, W, H), fill=(6, 22, 16))
    d.line((0, H - 92, W, H - 92), fill=(232, 182, 48), width=4)
    texte_centre(img, 'NORMAL  ·  HARD  ·  KAIZO', font('Futura.ttc', 40, 2), H - 46, (232, 182, 48), track=3)
    d.text((34, 34), 'ROMHACK', font=font('Futura.ttc', 28, 2), fill=(170, 230, 200))
    out = ROOT / 'docs/jaquette/jaquette.png'
    img.save(out)
    print(out)


if __name__ == '__main__':
    main()
