#!/usr/bin/env python3
"""Descriptions françaises officielles des attaques, talents et objets (Émeraude Couronnement).

Source : tables CSV publiques de PokéAPI (data/v2/csv) : moves / abilities / items (identifiants),
move_names / ability_names / item_names, et *_flavor_text (textes des jeux, langue 5 = français).

Correspondance : constante du code (MOVE_POUND -> pound) ; sinon nom français déjà dans le code.
Parmi les textes français de toutes les versions, on garde celui qui tient dans la fenêtre du jeu
avec la police la plus large possible (normale, puis étroite, puis plus étroite, comme GetFontIdToFit).
Les entrées sans texte ou trop longues sont listées dans translations/fr/descriptions_rapport.txt ;
corrections manuelles : translations/fr/descriptions/*.txt (CLÉ|texte, mis en forme automatiquement)
ou translations/fr/descriptions_overrides.json (clé = constante, '\\n' = texte déjà mis en forme).

Usage : python3 tools/fr/descriptions_fr.py <dossier_csv_pokeapi>
"""
import csv
import json
import re
import sys
import unicodedata
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
FR = '5'
OVERRIDES = ROOT / 'translations/fr/descriptions_overrides.json'
REPORT = ROOT / 'translations/fr/descriptions_rapport.txt'

# (catégorie, préfixe, fichier, table, colonne id, lignes max, largeur px)
CATEGORIES = [
    ('Attaques', 'MOVE_', ROOT / 'src/data/moves_info.h', 'move', 'move_id', 2, 154),
    ('Talents', 'ABILITY_', ROOT / 'src/data/abilities.h', 'ability', 'ability_id', 1, 144),
    ('Objets', 'ITEM_', ROOT / 'src/data/items.h', 'item', 'item_id', 3, 108),
]
FONTS = ['gFontNormalLatinGlyphWidths', 'gFontNarrowLatinGlyphWidths', 'gFontNarrowerLatinGlyphWidths']
REMPLACEMENTS = {'«': '"', '»': '"', ' ': ' ', '\xa0': ' ', '\xad': '', '–': '-', '—': '-', '‘': "'",
                 'œ': 'œ', '’': "'", 'ʼ': "'"}


def load_charmap():
    charmap = {}
    for line in (ROOT / 'charmap.txt').read_text(encoding='utf-8').splitlines():
        m = re.match(r"^'(.)'\s*=\s*([0-9A-Fa-f]{2})\s*$", line)
        if m and m.group(1) not in charmap:
            charmap[m.group(1)] = int(m.group(2), 16)
        elif line.startswith("'\\''"):
            charmap["'"] = int(line.split('=')[1].strip(), 16)
    return charmap


CHARMAP = load_charmap()
SRC_FONTS = (ROOT / 'src/fonts.c').read_text(encoding='utf-8')


def table(name):
    block = SRC_FONTS.split(name + '[] = {', 1)[1].split('};', 1)[0]
    t = [int(x) for x in re.findall(r'\d+', block)]
    return {c: t[code] for c, code in CHARMAP.items() if code < len(t)}


WIDTHS = [table(f) for f in FONTS]


def width(text, w):
    return sum(w.get(c, 6) for c in text)


def wrap(text, maxw, w):
    lines, cur = [], ''
    for word in text.split(' '):
        cand = word if not cur else cur + ' ' + word
        if width(cand, w) <= maxw:
            cur = cand
        else:
            if cur:
                lines.append(cur)
            cur = word
            if width(word, w) > maxw:
                return None
    if cur:
        lines.append(cur)
    return lines


def normalise(text):
    text = unicodedata.normalize('NFC', text)
    for a, b in REMPLACEMENTS.items():
        text = text.replace(a, b)
    text = re.sub(r'-\s*\n\s*', '-', text)  # césure de fin de ligne des jeux DS
    text = re.sub(r'\s+', ' ', text).strip()
    text = ''.join(c for c in text if c in CHARMAP or c == ' ')
    return text


def fit(text, nlines, maxw):
    """(rang de police, lignes) ou None."""
    for rank, w in enumerate(WIDTHS):
        lines = wrap(text, maxw, w)
        if lines and len(lines) <= nlines:
            return rank, lines
    return None


def read_csv(d, name):
    with open(Path(d) / f'{name}.csv', encoding='utf-8') as f:
        return list(csv.DictReader(f))


def c_string(lines):
    esc = [l.replace('\\', '\\\\').replace('"', '\\"') for l in lines]
    if len(esc) == 1:
        return f'"{esc[0]}"'
    parts = [f'"{l}\\n"' for l in esc[:-1]] + [f'"{esc[-1]}"']
    return '\n            '.join(parts)


def main(csvdir):
    overrides = json.loads(OVERRIDES.read_text(encoding='utf-8')) if OVERRIDES.exists() else {}
    # Traductions écrites à la main : translations/fr/descriptions/<attaques|talents|objets>_N.txt, lignes CLÉ|texte
    prefixes = {'attaques': 'MOVE_', 'talents': 'ABILITY_', 'objets': 'ITEM_'}
    for f in sorted((ROOT / 'translations/fr/descriptions').glob('*.txt')):
        pre = prefixes[f.stem.split('_')[0]]
        for line in f.read_text(encoding='utf-8').splitlines():
            if '|' in line and not line.startswith('#'):
                k, v = line.split('|', 1)
                overrides[pre + k.strip()] = v.strip()
    report = []
    for cat, prefix, path, tbl, idcol, nlines, maxw in CATEGORIES:
        ident = {r['identifier']: r['id'] for r in read_csv(csvdir, tbl + 's' if tbl != 'ability' else 'abilities')}
        noms = {}
        for r in read_csv(csvdir, tbl + '_names'):
            if r['local_language_id'] == FR:
                noms.setdefault(r['name'].lower(), r[idcol])
        textes = {}
        for r in read_csv(csvdir, tbl + '_flavor_text'):
            if r['language_id'] == FR:
                textes.setdefault(r[idcol], []).append((int(r['version_group_id']), normalise(r['flavor_text'])))
        src = path.read_text(encoding='utf-8')
        entry_re = re.compile(r'(    \[(' + prefix + r'\w+)\] =\n    \{\n)(.*?)(\n    \},)', re.S)
        desc_re = re.compile(r'(\.description = COMPOUND_STRING\()(.*?")(\),)', re.S)  # "), : un « ), » peut être dans le texte
        nb_ok = 0

        def repl(m):
            nonlocal nb_ok
            const, body = m.group(2), m.group(3)
            dm = desc_re.search(body)
            if not dm:
                return m.group(0)
            fin = dm.end()
            if '#if' in dm.group(2):  # description avec variantes #if / #else / #endif : on remplace tout le bloc
                e = body.find('#endif', dm.end())
                fin = body.index('\n', e) if '\n' in body[e:] else len(body)
            if const in overrides:
                if '\n' in overrides[const]:
                    lines = overrides[const].split('\n')
                else:  # texte libre : mis en forme automatiquement
                    f = fit(normalise(overrides[const]), nlines, maxw)
                    if not f:
                        report.append(f'{cat}\t{const}\ttraduction trop longue')
                        return m.group(0)
                    lines = f[1]
            else:
                key = const[len(prefix):].lower().replace('_', '-')
                pid = ident.get(key)
                if pid is None:
                    nm = re.search(r'\.name = (?:COMPOUND_STRING|ITEM_NAME|_)\("([^"]*)"\)', body)
                    pid = noms.get(nm.group(1).lower()) if nm else None
                cands = []
                for vg, t in textes.get(pid, []):
                    f = fit(t, nlines, maxw)
                    if f:
                        cands.append((f[0], -vg, f[1]))
                if not cands:
                    report.append(f'{cat}\t{const}\t' + ('pas de texte' if pid not in textes else 'trop long'))
                    return m.group(0)
                lines = sorted(cands)[0][2]
            nb_ok += 1
            new = dm.group(1) + '\n            ' + c_string(lines) + dm.group(3) if nlines > 1 \
                else dm.group(1) + c_string(lines) + dm.group(3)
            return m.group(1) + body[:dm.start()] + new + body[fin:] + m.group(4)

        src = entry_re.sub(repl, src)
        path.write_text(src, encoding='utf-8')
        print(f'{cat} : {nb_ok} descriptions en français.')
    REPORT.write_text('Descriptions restées en anglais (à compléter dans descriptions_overrides.json)\n'
                      + '\n'.join(report) + '\n', encoding='utf-8')
    print(f'{len(report)} restées en anglais : {REPORT.relative_to(ROOT)}')


if __name__ == '__main__':
    if len(sys.argv) != 2:
        raise SystemExit(__doc__)
    main(sys.argv[1])
