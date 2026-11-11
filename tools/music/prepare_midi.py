#!/usr/bin/env python3
"""Prépare un MIDI General MIDI pour le moteur son GBA (mid2agb / m4a).

- Réassigne les programmes (instruments) vers ceux du voicegroup choisi.
- Donne un programme « batterie » à la piste du canal 10 (canal 9 en base 0).
- Ajoute les marqueurs de boucle « [ » au début et « ] » à la fin (le morceau tourne en continu).
- Supprime les pistes vides (textes seuls) sauf la piste 0 (tempo, crédits).

Usage :
    python3 tools/music/prepare_midi.py <source.mid> <sortie.mid> --map 0:1,34:33 --drums 0 [--loop-end TICKS]
"""
import argparse
import struct
import sys


def vlq_read(b, i):
    v = 0
    while True:
        c = b[i]
        i += 1
        v = (v << 7) | (c & 0x7F)
        if c < 0x80:
            return v, i


def vlq_write(v):
    out = [v & 0x7F]
    v >>= 7
    while v:
        out.append(0x80 | (v & 0x7F))
        v >>= 7
    return bytes(reversed(out))


def parse_track(b):
    """Renvoie une liste d'événements (tick absolu, octets bruts sans delta, statut)."""
    events, i, tick, run = [], 0, 0, None
    while i < len(b):
        dt, i = vlq_read(b, i)
        tick += dt
        s = b[i]
        if s == 0xFF:
            typ = b[i + 1]
            ln, j = vlq_read(b, i + 2)
            events.append((tick, b[i:j + ln]))
            i = j + ln
            if typ == 0x2F:
                break
            continue
        if s in (0xF0, 0xF7):
            ln, j = vlq_read(b, i + 1)
            events.append((tick, b[i:j + ln]))
            i = j + ln
            continue
        if s & 0x80:
            run = s
            i += 1
        typ = run >> 4
        n = 2 if typ in (8, 9, 0xA, 0xB, 0xE) else 1
        events.append((tick, bytes([run]) + b[i:i + n]))
        i += n
    return events


def meta(typ, data):
    return bytes([0xFF, typ]) + vlq_write(len(data)) + data


def build_track(events):
    events = sorted(events, key=lambda e: (e[0], 0 if e[1][0] == 0xFF and e[1][1] != 0x2F else 1))
    out, last = bytearray(), 0
    for tick, raw in events:
        out += vlq_write(tick - last) + raw
        last = tick
    return b'MTrk' + struct.pack('>I', len(out)) + bytes(out)


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('src')
    ap.add_argument('dst')
    ap.add_argument('--map', default='', help='programmes source:destination, ex. 0:1,34:33')
    ap.add_argument('--drums', type=int, default=0, help='programme du kit de batterie dans le voicegroup')
    ap.add_argument('--loop-end', type=int, default=None)
    args = ap.parse_args()
    prog_map = {int(a): int(b) for a, b in (x.split(':') for x in args.map.split(',') if x)}

    d = open(args.src, 'rb').read()
    if d[:4] != b'MThd':
        sys.exit('Pas un fichier MIDI.')
    fmt, ntrk, div = struct.unpack('>HHH', d[8:14])
    pos, tracks = 8 + struct.unpack('>I', d[4:8])[0], []
    for _ in range(ntrk):
        ln = struct.unpack('>I', d[pos + 4:pos + 8])[0]
        tracks.append(parse_track(d[pos + 8:pos + 8 + ln]))
        pos += 8 + ln

    end_tick = max((e[0] for t in tracks for e in t), default=0)
    loop_end = args.loop_end or ((end_tick + div * 4 - 1) // (div * 4)) * div * 4  # fin de mesure (4/4)

    out_tracks = []
    for n, events in enumerate(tracks):
        has_notes = any(raw[0] & 0xF0 == 0x90 for _, raw in events)
        if n != 0 and not has_notes:
            continue
        new = []
        drum_channels = set()
        for tick, raw in events:
            if raw[0] == 0xFF and raw[1] == 0x2F:
                continue  # fin de piste réécrite plus bas
            st = raw[0]
            if st & 0xF0 == 0xC0:
                ch = st & 0x0F
                if ch == 9:
                    continue
                raw = bytes([st, prog_map.get(raw[1], raw[1])])
            if st & 0xF0 in (0x80, 0x90) and st & 0x0F == 9:
                drum_channels.add(9)
            new.append((tick, raw))
        if drum_channels:
            new.insert(0, (0, bytes([0xC9, args.drums])))
        if n == 0:
            new.append((0, meta(0x06, b'[')))
            new.append((loop_end, meta(0x06, b']')))
        new.append((loop_end if n == 0 else max([t for t, _ in new] + [0]), meta(0x2F, b'')))
        out_tracks.append(build_track(new))

    header = b'MThd' + struct.pack('>IHHH', 6, 1, len(out_tracks), div)
    open(args.dst, 'wb').write(header + b''.join(out_tracks))
    print(f'{args.dst} : {len(out_tracks)} pistes, boucle 0 -> {loop_end} ticks ({div} par noire).')


if __name__ == '__main__':
    main()
