#!/usr/bin/env python3
"""Applique les traductions des chaînes du code C (messages de combat, menus).

Format des fichiers translations/fr/c/*.txt :

    @fichier src/battle_message.c
    PKMNGAINEDEXP = {B_BUFF1} gagne{B_BUFF2} {B_BUFF3} points d'Exp. !\\p
    sText_WildPkmnPrefix = " sauvage"

- « @fichier » choisit le fichier source pour les lignes suivantes.
- La clé est soit un identifiant de chaîne (const u8 sText_X[] = _("…")),
  soit le suffixe d'un STRINGID_ du tableau des messages de combat (STRINGID_PKMNGAINEDEXP -> PKMNGAINEDEXP).
- Le texte est recopié tel quel (codes {…}, \\n, \\p, \\l conservés). Des guillemets autour sont optionnels.
- « // » en début de ligne = commentaire.

Usage : python3 tools/fr/cstrings_fr.py translations/fr/c/*.txt
"""
import re
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]


def load(paths):
    by_file = {}
    for p in paths:
        current = None
        for n, raw in enumerate(Path(p).read_text(encoding='utf-8').splitlines(), 1):
            line = raw.strip()
            if not line or line.startswith('//'):
                continue
            if line.startswith('@fichier'):
                current = line.split(None, 1)[1].strip()
                by_file.setdefault(current, {})
                continue
            if ' = ' not in line or current is None:
                print(f'{p}:{n}: ligne ignorée : {raw}')
                continue
            key, val = line.split(' = ', 1)
            val = val.strip()
            if len(val) >= 2 and val[0] == '"' and val[-1] == '"':
                val = val[1:-1]
            val = val.replace('«', '“').replace('»', '”').replace('—', '-').replace(' ', ' ').replace(' ', ' ')
            val = re.sub(r'(?<!\\)"', '\\"', val)
            by_file[current][key.strip()] = val
    return by_file


def apply(by_file):
    total, missing = 0, []
    for rel, entries in by_file.items():
        path = ROOT / rel
        text = path.read_text(encoding='utf-8')
        for key, val in entries.items():
            if key.isupper() or key[0].isdigit():
                rx = re.compile(r'(\[STRINGID_' + re.escape(key) + r'\]\s*=\s*COMPOUND_STRING\(")((?:[^"\\]|\\.)*)("\))')
            else:
                rx = re.compile(r'(\b' + re.escape(key) + r'\[\]\s*=\s*_\(")((?:[^"\\]|\\.)*)("\))')
            text, n = rx.subn(lambda m: m.group(1) + val + m.group(3), text, count=1)
            if n:
                total += 1
            else:
                missing.append(f'{rel}:{key}')
        path.write_text(text, encoding='utf-8')
    print(f'{total} chaînes traduites.')
    if missing:
        print('Clés introuvables :', ', '.join(missing))
        return 1
    return 0


if __name__ == '__main__':
    if len(sys.argv) < 2:
        print(__doc__)
        sys.exit(2)
    sys.exit(apply(load(sys.argv[1:])))
