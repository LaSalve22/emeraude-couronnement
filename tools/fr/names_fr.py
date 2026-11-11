#!/usr/bin/env python3
"""Traduit les noms du jeu (Pokémon, attaques, objets, talents, natures, types) en français officiel.

Source : tables CSV publiques de PokéAPI (https://github.com/PokeAPI/pokeapi, data/v2/csv).
Le correspondance se fait par le nom anglais : le nom anglais présent dans le code est cherché
dans la table anglaise de PokéAPI, puis remplacé par le nom français de la même entrée.

Les noms trop longs pour le jeu et les corrections manuelles vont dans translations/fr/noms_overrides.json
(clé = nom anglais tel qu'écrit dans le code, valeur = nom français).

Usage :
    python3 tools/fr/names_fr.py <dossier_csv_pokeapi> [--dry-run]
"""
import csv
import json
import re
import sys
import unicodedata
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
LANG_EN, LANG_FR = '9', '5'
OVERRIDES = ROOT / 'translations/fr/noms_overrides.json'
REPORT = ROOT / 'translations/fr/noms_rapport.txt'

CATEGORIES = [
    # (nom, table CSV, colonne id, fichiers, motif du nom dans le code, longueur max)
    ('Pokémon', 'pokemon_species_names', 'pokemon_species_id',
     sorted((ROOT / 'src/data/pokemon/species_info').glob('*.h')),
     r'(\.speciesName\s*=\s*_\(")([^"]*)("\))', 12),
    ('Attaques', 'move_names', 'move_id', [ROOT / 'src/data/moves_info.h'],
     r'(\.name\s*=\s*COMPOUND_STRING\(")([^"]*)("\))', 16),
    ('Objets', 'item_names', 'item_id', [ROOT / 'src/data/items.h'],
     r'(\.name\s*=\s*ITEM_NAME\(")([^"]*)("\))', 19),
    ('Talents', 'ability_names', 'ability_id', [ROOT / 'src/data/abilities.h'],
     r'(\.name\s*=\s*_\(")([^"]*)("\))', 16),
    ('Natures', 'nature_names', 'nature_id', [ROOT / 'src/pokemon.c'],
     r'(\[NATURE_\w+\]\s*=\s*\{\s*\.name\s*=\s*COMPOUND_STRING\(")([^"]*)("\))', 12),
]


def norm(s):
    s = unicodedata.normalize('NFKD', s.replace('♀', 'f').replace('♂', 'm'))
    s = ''.join(c for c in s if not unicodedata.combining(c))
    return re.sub(r'[^a-z0-9]', '', s.lower())


def load_table(csv_dir, table, id_col):
    en, fr = {}, {}
    with open(Path(csv_dir) / f'{table}.csv', encoding='utf-8') as f:
        for row in csv.DictReader(f):
            name = row.get('name') or row.get('genus')
            if row['local_language_id'] == LANG_EN:
                en[row[id_col]] = name
            elif row['local_language_id'] == LANG_FR:
                fr[row[id_col]] = name
    mapping = {}
    for i, e in en.items():
        if i in fr and norm(e) not in mapping:
            mapping[norm(e)] = fr[i]
    return mapping


def load_charset():
    chars = set()
    for line in (ROOT / 'charmap.txt').read_text(encoding='utf-8').splitlines():
        m = re.match(r"^'(.)'\s*=", line)
        if m:
            chars.add(m.group(1))
    chars.add("'")
    return chars


def main():
    if len(sys.argv) < 2:
        print(__doc__)
        return 2
    csv_dir, dry = sys.argv[1], '--dry-run' in sys.argv
    overrides = json.loads(OVERRIDES.read_text(encoding='utf-8')) if OVERRIDES.exists() else {}
    charset = load_charset()
    report = []
    for cat, table, id_col, files, pattern, maxlen in CATEGORIES:
        mapping = load_table(csv_dir, table, id_col)
        done, missing, toolong, badchar = 0, [], [], []
        for path in files:
            text = path.read_text(encoding='utf-8')

            def repl(m):
                nonlocal done
                en = m.group(2)
                if en in overrides:
                    fr = overrides[en]
                else:
                    fr = mapping.get(norm(en))
                    if fr is None:
                        if en.strip('-? ') and not re.fullmatch(r'[A-Z0-9 ]{1,3}', en):
                            missing.append(en)
                        return m.group(0)
                    fr = fr.replace('’', "'")
                if len(fr) > maxlen:
                    toolong.append(f'{en} -> {fr} ({len(fr)})')
                    return m.group(0)
                if any(c not in charset for c in fr):
                    badchar.append(f'{en} -> {fr}')
                    return m.group(0)
                done += 1
                return m.group(1) + fr.replace('"', '\\"') + m.group(3)

            new = re.sub(pattern, repl, text)
            if new != text and not dry:
                path.write_text(new, encoding='utf-8')
        report.append(f'== {cat} : {done} traduits, {len(missing)} introuvables, '
                      f'{len(toolong)} trop longs, {len(badchar)} caractères absents')
        for title, items in (('Introuvables', missing), ('Trop longs', toolong), ('Caractères', badchar)):
            if items:
                report.append(f'  {title} : ' + ' | '.join(sorted(set(items))))
    REPORT.write_text('\n'.join(report) + '\n', encoding='utf-8')
    print('\n'.join(line[:400] for line in report))
    return 0


if __name__ == '__main__':
    sys.exit(main())
