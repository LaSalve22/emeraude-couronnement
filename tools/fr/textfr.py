#!/usr/bin/env python3
"""Outil de traduction française d'Émeraude Couronnement.

Les traductions sont écrites en texte libre dans translations/fr/*.txt :

    @@ NomDuLabel
    Premier paragraphe (une boîte de dialogue ou plus).

    Paragraphe suivant : nouvelle boîte.

    @@ AutreLabel
    ...

- Une ligne vide = nouvelle boîte (\\p).
- « // » en début de ligne = commentaire ignoré.
- Une ligne qui commence par « > » est collée telle quelle (retour à la ligne forcé \\n à l'intérieur du paragraphe).
- Les codes {PLAYER}, {STR_VAR_1}, {COLOR ...}, etc. sont conservés.

L'outil remet les paragraphes en forme (2 lignes par boîte, largeur mesurée en pixels avec la
police FONT_NORMAL) et remplace le texte du label dans le fichier .inc où il est défini.

Usage :
    python3 tools/fr/textfr.py apply translations/fr/*.txt   # applique les traductions
    python3 tools/fr/textfr.py check  data/maps/*/scripts.inc  # signale les lignes trop larges et les \\n après \\l
    python3 tools/fr/textfr.py fixl   data/text/*.inc           # remplace les \\n après \\l par \\l
"""
import re
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
MAX_WIDTH = 208          # largeur utile d'une boîte de dialogue standard (pixels)
LINES_PER_BOX = 2

# Largeur supposée des codes variables (pire cas raisonnable).
PLACEHOLDER_WIDTH = {
    'PLAYER': 7 * 6,
    'RIVAL': 7 * 6,
    'STR_VAR_1': 10 * 6,
    'STR_VAR_2': 10 * 6,
    'STR_VAR_3': 10 * 6,
    'KUN': 0,
}


def load_widths():
    charmap = {}
    for line in (ROOT / 'charmap.txt').read_text(encoding='utf-8').splitlines():
        m = re.match(r"^'(.)'\s*=\s*([0-9A-Fa-f]{2})\s*$", line)
        if m and m.group(1) not in charmap:
            charmap[m.group(1)] = int(m.group(2), 16)
        elif line.startswith("'\\''"):
            charmap["'"] = int(line.split('=')[1].strip(), 16)
    src = (ROOT / 'src/fonts.c').read_text(encoding='utf-8')
    block = src.split('gFontNormalLatinGlyphWidths[] = {', 1)[1].split('};', 1)[0]
    table = [int(x) for x in re.findall(r'\d+', block)]
    return {c: table[code] for c, code in charmap.items() if code < len(table)}


WIDTHS = load_widths()
TOKEN_RE = re.compile(r'\{[^}]*\}|.', re.S)


def measure(text):
    width = 0
    for tok in TOKEN_RE.findall(text):
        if tok.startswith('{'):
            name = tok[1:-1].split(' ')[0]
            width += PLACEHOLDER_WIDTH.get(name, 0)
        else:
            width += WIDTHS.get(tok, 6)
    return width


def wrap_paragraph(par):
    """Découpe un paragraphe en lignes de MAX_WIDTH pixels au plus."""
    lines = []
    for forced in par:
        words = forced.split(' ')
        cur = ''
        for w in words:
            if not w:
                continue
            cand = w if not cur else cur + ' ' + w
            if measure(cand) <= MAX_WIDTH:
                cur = cand
            else:
                if cur:
                    lines.append(cur)
                if measure(w) > MAX_WIDTH:
                    raise ValueError(f'mot trop long pour une ligne : {w!r}')
                cur = w
        if cur:
            lines.append(cur)
    return lines


def format_text(paragraphs):
    """Renvoie la liste des .string pour un texte (liste de paragraphes, chacun liste de lignes forcées)."""
    out = []
    for pi, par in enumerate(paragraphs):
        lines = wrap_paragraph(par)
        for li, line in enumerate(lines):
            last_line = li == len(lines) - 1
            if last_line:
                end = '$' if pi == len(paragraphs) - 1 else '\\p'
            elif li == 0:  # seule la 1re coupure en \n ; après un \l, toutes les suivantes en \l (sinon la boîte bloque)
                end = '\\n'
            else:
                end = '\\l'
            out.append(line.replace('"', '\\"') + end)
    return out


def parse_translation_file(path):
    entries = {}
    label = None
    buf = []

    def flush():
        if label is None:
            return
        paragraphs, cur = [], []
        for raw in buf + ['']:
            if raw.strip() == '':
                if cur:
                    paragraphs.append(cur)
                    cur = []
            elif raw.startswith('>'):
                cur.append(raw[1:].strip())
            else:
                if cur:
                    cur[-1] = cur[-1] + ' ' + raw.strip()
                else:
                    cur.append(raw.strip())
        entries[label] = paragraphs

    text = Path(path).read_text(encoding='utf-8')
    # Typographie française -> caractères disponibles dans la police du jeu.
    for a, b in (('« ', '“'), (' »', '”'), ('«', '“'), ('»', '”'), ('—', '-'), ('–', '-'), ('\u202f', ' '), ('\u00a0', ' ')):
        text = text.replace(a, b)
    for raw in text.splitlines():
        if raw.startswith('//'):
            continue
        m = re.match(r'^@@\s*(\S+)\s*$', raw)
        if m:
            flush()
            label, buf = m.group(1), []
        else:
            buf.append(raw)
    flush()
    return entries


LABEL_BLOCK_RE = r'(?m)^({label}:{{1,2}}[^\n]*\n)((?:[ \t]*\.string[^\n]*\n)+)'


def find_label_files():
    files = list((ROOT / 'data').rglob('*.inc')) + list((ROOT / 'data').rglob('*.s'))
    return files


def apply(paths):
    entries = {}
    for p in paths:
        entries.update(parse_translation_file(p))
    unknown = {}
    for label, paragraphs in entries.items():
        for par in paragraphs:
            for line in par:
                for c in re.sub(r'\{[^}]*\}', '', line):
                    if c not in WIDTHS:
                        unknown.setdefault(c, label)
    if unknown:
        for c, label in unknown.items():
            print(f'Caractère absent de la police : {c!r} (dans {label})')
        return 1
    files = {f: f.read_text(encoding='utf-8') for f in find_label_files()}
    missing = []
    changed = set()
    for label, paragraphs in entries.items():
        rx = re.compile(LABEL_BLOCK_RE.format(label=re.escape(label)))
        done = False
        for f, text in files.items():
            m = rx.search(text)
            if not m:
                continue
            strings = format_text(paragraphs)
            # Conserve la fin d'origine (ex. \p final des textes enchaînés du discours d'intro).
            joined = ''.join(re.findall(r'\.string\s+"(.*)"', m.group(2)))
            tail = re.search(r'((?:\\[pnl])*)\$$', joined).group(1)
            if tail and strings[-1].endswith('$'):
                strings[-1] = strings[-1][:-1] + tail + '$'
            block = ''.join(f'\t.string "{s}"\n' for s in strings)
            files[f] = text[:m.start(2)] + block + text[m.end(2):]
            changed.add(f)
            done = True
            break
        if not done:
            missing.append(label)
    for f in changed:
        f.write_text(files[f], encoding='utf-8')
    print(f'{len(entries) - len(missing)} textes appliqués dans {len(changed)} fichiers.')
    if missing:
        print('Labels introuvables :', ', '.join(missing))
        return 1
    return 0


def dump(paths):
    """Affiche chaque texte d'un fichier .inc sous forme compacte (pour traduire)."""
    rx = re.compile(r'(?m)^(\w+):{1,2}[^\n]*\n((?:[ \t]*\.string[^\n]*\n)+)')
    for p in paths:
        text = Path(p).read_text(encoding='utf-8')
        for m in rx.finditer(text):
            body = ''.join(re.findall(r'\.string\s+"(.*)"', m.group(2)))
            body = body.replace('\\n', ' ').replace('\\l', ' ').replace('\\p', ' ¶ ').replace('$', '')
            print(f'@@ {m.group(1)}\n{body}')


def scan_n_after_l(path, fix=False):
    """Repère (et corrige si fix) les \\n qui suivent un \\l dans le même paragraphe : la boîte de texte se bloque."""
    lines = Path(path).read_text(encoding='utf-8').splitlines(keepends=True)
    found, prev, changed = [], None, False
    for n, line in enumerate(lines):
        m = re.match(r'(\s*\.string\s+")(.*)("\s*)$', line)
        if not m:
            if line.strip() and not line.strip().startswith(('@', '.if', '.else', '.endif')):
                prev = None  # nouveau label ou instruction : nouveau texte
            continue
        body = m.group(2)
        out, i = [], 0
        while i < len(body):
            if body[i] == '\\' and i + 1 < len(body) and body[i + 1] in 'nlp':
                code = body[i + 1]
                if code == 'n' and prev == 'l':
                    found.append((n + 1, line.strip()))
                    if fix:
                        code = 'l'
                        changed = True
                out.append('\\' + code)
                prev = None if code == 'p' else code
                i += 2
                continue
            if body[i] == '$':
                prev = None
            out.append(body[i])
            i += 1
        if fix:
            lines[n] = m.group(1) + ''.join(out) + m.group(3)
    if fix and changed:
        Path(path).write_text(''.join(lines), encoding='utf-8')
    return found


def fixl(paths):
    total = 0
    for p in paths:
        f = scan_n_after_l(p, fix=True)
        total += len(f)
        for n, l in f:
            print(f'{p}:{n}: corrigé : {l}')
    print(f'{total} \\n après \\l corrigés')
    return 0


def check(paths):
    worst = 0
    bad = 0
    nl = 0
    for p in paths:
        for n, l in scan_n_after_l(p):
            nl += 1
            print(f'{p}:{n}: \\n après \\l (la boîte bloque) : {l}')
        for n, line in enumerate(Path(p).read_text(encoding='utf-8').splitlines(), 1):
            m = re.match(r'\s*\.string\s+"(.*)"\s*$', line)
            if not m:
                continue
            for part in re.split(r'\\[nlp]', m.group(1).replace('$', '')):
                w = measure(part.replace('\\"', '"'))
                worst = max(worst, w)
                if w > MAX_WIDTH:
                    bad += 1
                    print(f'{p}:{n}: {w}px > {MAX_WIDTH}px : {part}')
    print(f'Ligne la plus large : {worst}px ; lignes trop larges : {bad} ; \\n après \\l : {nl}')
    return 1 if bad or nl else 0


if __name__ == '__main__':
    commands = {'apply': apply, 'check': check, 'dump': dump, 'fixl': fixl}
    if len(sys.argv) < 3 or sys.argv[1] not in commands:
        print(__doc__)
        sys.exit(2)
    sys.exit(commands[sys.argv[1]](sys.argv[2:]) or 0)
