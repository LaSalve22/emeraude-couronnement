#!/usr/bin/env python3
"""Émeraude Couronnement : place les Méga-Gemmes (et les Dark Balls) dans des Poké Balls existantes (objets ordinaires remplacés).

Relançable : chaque entrée (carte, objet d'origine) remplace la première Poké Ball restante de cet objet. Si l'objet a déjà été
remplacé, l'entrée est ignorée. Les autres Méga-Gemmes sont vendues après la Ligue (data/scripts/mega_gemmes.inc).
"""
import json, pathlib

ROOT = pathlib.Path(__file__).resolve().parents[2]

PLACEMENTS = [
    # (carte, objet d'origine, Méga-Gemme)
    ('Route118', 'HYPER_POTION', 'MANECTITE'),
    ('Route119', 'SUPER_REPEL', 'HERACRONITE'),
    ('Route119', 'HYPER_POTION', 'VENUSAURITE'),
    ('Route119', 'HYPER_POTION', 'BEEDRILLITE'),
    ('Route119', 'ELIXIR', 'SCIZORITE'),
    ('Route120', 'FULL_HEAL', 'ABSOLITE'),
    ('Route120', 'HYPER_POTION', 'LUCARIONITE'),
    ('Route120', 'NEST_BALL', 'AMPHAROSITE'),
    ('Route121', 'REVIVE', 'STEELIXITE'),
    ('Route123', 'ULTRA_BALL', 'GARDEVOIRITE'),
    ('Route123', 'ELIXIR', 'GALLADITE'),
    ('MtPyre_Exterior', 'MAX_POTION', 'MEDICHAMITE'),
    ('MtPyre_2F', 'ULTRA_BALL', 'GENGARITE'),
    ('MtPyre_3F', 'SUPER_REPEL', 'BANETTITE'),
    ('LilycoveCity', 'MAX_REPEL', 'SLOWBRONITE'),
    ('MossdeepCity', 'NET_BALL', 'ALAKAZITE'),
    ('AquaHideout_B1F', 'NUGGET', 'SWAMPERTITE'),
    ('AquaHideout_B2F', 'NEST_BALL', 'SHARPEDONITE'),
    ('Route125', 'BIG_PEARL', 'GYARADOSITE'),
    ('ShoalCave_LowTideStairsRoom', 'ICE_HEAL', 'GLALITITE'),
    ('ShoalCave_LowTideEntranceRoom', 'BIG_PEARL', 'ABOMASITE'),
    ('MagmaHideout_2F_2R', 'MAX_ELIXIR', 'CAMERUPTITE'),
    ('MagmaHideout_2F_2R', 'FULL_RESTORE', 'BLAZIKENITE'),
    ('MagmaHideout_3F_1R', 'NUGGET', 'CHARIZARDITE_X'),
    ('MagmaHideout_3F_3R', 'ESCAPE_ROPE', 'HOUNDOOMINITE'),
    ('MagmaHideout_4F', 'MAX_REVIVE', 'CHARIZARDITE_Y'),
    ('Route127', 'CARBOS', 'PIDGEOTITE'),
    ('Route127', 'ZINC', 'LOPUNNITE'),
    ('Route132', 'PROTEIN', 'AERODACTYLITE'),
    ('Route133', 'BIG_PEARL', 'ALTARIANITE'),
    ('SafariZone_North', 'CALCIUM', 'PINSIRITE'),
    ('SafariZone_Northeast', 'NUGGET', 'KANGASKHANITE'),
    ('SafariZone_Southwest', 'MAX_REVIVE', 'AUDINITE'),
    ('AbandonedShip_Rooms_1F', 'HARBOR_MAIL', 'SABLENITE'),
    ('AbandonedShip_Rooms_B1F', 'ESCAPE_ROPE', 'MAWILITE'),
    ('MeteorFalls_1F_1R', 'FULL_HEAL', 'SALAMENCITE'),
    ('VictoryRoad_1F', 'MAX_ELIXIR', 'TYRANITARITE'),
    ('VictoryRoad_B1F', 'FULL_RESTORE', 'GARCHOMPITE'),
    ('VictoryRoad_B2F', 'FULL_HEAL', 'AGGRONITE'),
    ('ArtisanCave_B1F', 'HP_UP', 'METAGROSSITE'),
    # Dark Balls (ex-Étrange Ball) : quelques-unes dès le chapitre 3, les autres vendues après la Ligue
    ('JaggedPass', 'BURN_HEAL', 'STRANGE_BALL'),
    ('Route113', 'SUPER_REPEL', 'STRANGE_BALL'),
    ('Route114', 'ENERGY_POWDER', 'STRANGE_BALL'),
    ('AquaHideout_B1F', 'MAX_ELIXIR', 'STRANGE_BALL'),
]


def main():
    for carte, orig, mega in PLACEMENTS:
        p = ROOT / 'data' / 'maps' / carte / 'map.json'
        j = json.loads(p.read_text(encoding='utf-8'))
        balls = [o for o in j['object_events'] if o.get('script') == 'Common_EventScript_FindItem']
        if any(o['trainer_sight_or_berry_tree_id'] == 'ITEM_' + mega for o in balls):
            continue
        cands = [o for o in balls if o['trainer_sight_or_berry_tree_id'] == 'ITEM_' + orig]
        if not cands:
            raise SystemExit(f'{carte} : plus de {orig}')
        cands[0]['trainer_sight_or_berry_tree_id'] = 'ITEM_' + mega
        p.write_text(json.dumps(j, indent=2, ensure_ascii=False) + '\n', encoding='utf-8')
    print(f'{len(PLACEMENTS)} Méga-Gemmes placées.')


if __name__ == '__main__':
    main()
