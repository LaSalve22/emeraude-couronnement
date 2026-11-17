#!/usr/bin/env python3
"""Émeraude Couronnement : simulation autonome de l'économie (lot 0 du cahier v0.42).

Reprend les formules exactes de la v0.41.1 (src/bourse.c, immobilier.c, societe.c, commerces.c, banques.c, arenes.c,
couronnement.c, calendrier.c), y compris le hasard déterministe (Melanger / Alea, graine = n° de Dresseur) et l'arithmétique
entière du C. Simule 100 jours pour 3 profils de joueur, puis rejoue les failles repérées.

Correctifs v0.42 (horloge gelée, échéances prises partout, sanctions graduelles) appliqués ; --v041 rejoue les anciennes règles.
Usage : python3 tools/sim_economy.py [--jours 100] [--graine 0x12345678] [--md rapport.md] [--v041]
Aucune dépendance (Python 3.8+).
"""
import argparse

V041 = False  # anciennes règles (avant les correctifs du lot 0)

M32 = 0xFFFFFFFF
MAX_MONEY = 999_999_999
CALC_MAX = 2_000_000_000


def tdiv(a, b):
    """Division entière du C (tronque vers zéro)."""
    q = abs(a) // abs(b)
    return q if (a >= 0) == (b >= 0) else -q


def melanger(x):
    x &= M32
    x ^= x >> 16
    x = (x * 0x7FEB352D) & M32
    x ^= x >> 15
    x = (x * 0x846CA68B) & M32
    x ^= x >> 16
    return x


def borner(v, lo, hi):
    return lo if v < lo else hi if v > hi else v


# ---------------------------------------------------------------- calendrier (jour 1 = samedi 1er janvier 2000)
SUN, MON, TUE, WED, THU, FRI, SAT = range(7)


def jour_semaine(j):
    return (SAT + j + 7 - 1) % 7


def weekend(j):
    return jour_semaine(j) in (SAT, SUN)


# ---------------------------------------------------------------- état du monde (drapeaux du scénario)
class Monde:
    def __init__(self, badges, ligue, crise=False, aqua_faillite=False, magma_faillite=False, elo=1500):
        self.badges, self.ligue, self.crise = badges, ligue, crise
        self.aqua_faillite, self.magma_faillite = aqua_faillite, magma_faillite
        self.elo = elo
        self.aqua_active = not aqua_faillite


# ---------------------------------------------------------------- Bourse (src/bourse.c)
ENTREPRISES = [  # nom, départ, tendance, volatilité, krach, envolée, envoléeMax
    ('Devon SA', 600, 4, 25, 3, 3, 600), ('Chantier Stern', 300, 3, 35, 4, 4, 800),
    ('Casino Lavandia', 150, 0, 120, 15, 15, 2000), ('Télé Hoenn', 400, 2, 40, 4, 4, 800),
    ('Grands Magasins', 500, 3, 15, 2, 2, 400), ('Aqua Corp', 800, 10, 50, 5, 8, 1000),
    ('Magma Corp', 800, 10, 50, 5, 8, 1000), ('Pension', 100, 2, 10, 1, 1, 300), ('Parc Safari', 120, 2, 12, 1, 2, 400),
]
DEVON, STERN, CASINO, TELE, MAGASINS, AQUA, MAGMA, PENSION, SAFARI = range(9)
OR = 9


class Bourse:
    def __init__(self, graine, monde):
        self.g, self.m = graine, monde
        self.cours = [e[1] for e in ENTREPRISES]
        self.parts = [0] * 10
        self.or_cours, self.dette, self.huissier = 20000, 0, 0

    def alea(self, jour, ent, tirage):
        return melanger(((jour * 0x9E3779B1) & M32) ^ self.g ^ (ent << 8) ^ (tirage << 20))

    def variation(self, e, jour):
        _, _, tendance, vol, krach, envolee, envmax = ENTREPRISES[e]
        m = self.m
        if (e == AQUA and m.aqua_faillite) or (e == MAGMA and m.magma_faillite):
            tendance, vol, envolee = -2, 15, 0
        elif m.crise:
            if e == STERN:
                tendance -= 20
            elif e in (PENSION, SAFARI):
                tendance -= 5
            elif e in (TELE, AQUA, MAGMA):
                tendance += 15
        if e == TELE and m.elo:
            tendance += tdiv(m.elo - 1500, 100)
        de = self.alea(jour, e, 0) % 1000
        if de < krach:
            v = -(400 + self.alea(jour, e, 2) % 401)
        elif de >= 1000 - envolee:
            v = 300 + self.alea(jour, e, 3) % (envmax - 300 + 1)
        else:
            v = tendance + self.alea(jour, e, 1) % (2 * vol + 1) - vol
        if self.alea(jour, 15, 0) % 1000 < 4:
            v -= 300 + self.alea(jour, 15, 1) % 301
        return max(v, -900)

    def variation_or(self, jour):
        tendance = 1 + (3 if self.m.crise else 0)
        de = self.alea(jour, 20, 0) % 1000
        if de < 2:
            v = -(50 + self.alea(jour, 20, 1) % 51)
        elif de >= 997:
            v = 50 + self.alea(jour, 20, 2) % 101
        else:
            v = tendance + self.alea(jour, 20, 3) % 21 - 10
        if self.alea(jour, 15, 0) % 1000 < 4:
            v += 40 + self.alea(jour, 20, 4) % 61
        return v

    def cours_actif(self, a):
        return self.or_cours if a == OR else self.cours[a]

    def frais(self, a):
        return 1 if a == OR else 3

    def passer_jour(self, jour, joueur):
        if not weekend(jour):
            for e in range(9):
                self.cours[e] = borner(self.cours[e] * (1000 + max(self.variation(e, jour), -900)) // 1000, 2, 60000)
            self.or_cours = borner(self.or_cours * (1000 + self.variation_or(jour)) // 1000, 2000, 2_000_000)
        if self.dette:
            i = self.dette * 10 // 1000
            self.dette += i if i else 1
        if jour_semaine(jour) == FRI:
            joueur.ajouter(self.parts[MAGASINS] * self.cours[MAGASINS] * 20 // 1000, 'dividendes')
        self.verifier_huissier(joueur)

    def valeur(self):
        t = sum(self.parts[e] * self.cours[e] for e in range(9)) + self.parts[OR] * self.or_cours
        return borner(t, 0, CALC_MAX)

    def verifier_huissier(self, joueur):
        v = self.valeur()
        if self.dette == 0 or self.dette <= v + 10000:
            return
        rec = v * 90 // 100
        self.parts = [0] * 10
        if rec >= self.dette:
            joueur.argent = min(MAX_MONEY, joueur.argent + rec - self.dette)
            rec, self.dette = self.dette, 0
        else:
            self.dette -= rec
            pris = min(joueur.argent, self.dette)
            joueur.argent -= pris
            self.dette -= pris
            rec += pris
        self.huissier += rec

    def acheter(self, joueur, a, q):
        c = self.cours_actif(a)
        possible = joueur.argent * 100 // (c * (100 + self.frais(a)))
        q = min(q, possible, 9999 - self.parts[a])
        if q <= 0:
            return 0
        brut = q * c
        joueur.argent -= brut + brut * self.frais(a) // 100
        self.parts[a] += q
        return q

    def vendre(self, joueur, a, q):
        q = min(q, self.parts[a])
        c = self.cours_actif(a)
        q = min(q, (MAX_MONEY - joueur.argent) * 100 // (c * (100 - self.frais(a))))
        if q <= 0:
            return 0
        joueur.argent += q * c * (100 - self.frais(a)) // 100
        self.parts[a] -= q
        return q

    def plafond_credit(self):
        return borner(10000 + self.valeur() // 2, 0, 5_000_000)

    def emprunter(self, joueur, montant):
        p = self.plafond_credit()
        montant = min(montant, max(0, p - self.dette), MAX_MONEY - joueur.argent)
        self.dette += montant
        joueur.argent += montant
        return montant


# ---------------------------------------------------------------- immobilier (src/immobilier.c)
BIENS = [('Rosyères', 40000, 220), ('Clémenti', 60000, 330), ('Mérouville', 90000, 500), ('Myokara', 35000, 200),
         ('Poivressel', 120000, 650), ('Lavandia', 110000, 620), ('Vergazon', 80000, 430), ('Vermilava', 130000, 720),
         ('Cimetronelle', 100000, 540), ('Nénucrique', 300000, 1600), ('Algatia', 180000, 980),
         ('Atalanopolis', 250000, 1350)]


class Immo:
    def __init__(self, graine, monde):
        self.g, self.m = graine, monde
        self.indice, self.possede, self.squat = 1000, set(), set()
        self.loyers = self.travaux = self.impayes = self.squats = 0

    def alea(self, jour, bien, tirage):
        return melanger(((jour * 0x85EBCA6B) & M32) ^ self.g ^ (bien << 10) ^ (tirage << 22) ^ 0x1440)

    def bonne_affaire(self, jour):
        return self.alea(jour // 7, 99, 0) % 12

    def prix(self, b, jour):
        local = self.alea(jour // 7, b, 1) % 201 - 100
        p = BIENS[b][1] * self.indice // 1000
        p = p * (1000 + local) // 1000
        if self.bonne_affaire(jour) == b:
            p = p * 75 // 100
        return p

    def loyer(self, b):
        return BIENS[b][2] * self.indice // 1000

    def valeur(self, jour):
        return sum(self.prix(b, jour) for b in self.possede)

    def passer_jour(self, jour, joueur):
        v = 1 + self.alea(jour, 50, 0) % 17 - 8
        if self.m.crise:
            v -= 3
        if self.alea(jour, 50, 1) % 1000 < 2:
            v -= 150 + self.alea(jour, 50, 2) % 151
        self.indice = borner(tdiv(self.indice * (1000 + v), 1000), 400, 3000)
        for b in sorted(self.possede):
            if b in self.squat:
                continue
            de = self.alea(jour, b, 2) % 1000
            if self.m.aqua_active and de < 15:
                self.squat.add(b)
                self.squats += 1
            elif de < 65:
                self.impayes += 1
            elif de < 95:
                cout = BIENS[b][1] * 3 // 100
                joueur.payer(cout, 'travaux')
                self.travaux += cout
            else:
                l = self.loyer(b)
                joueur.encaisser_societe(l, 'loyers')
                self.loyers += l

    def acheter(self, joueur, b, jour):
        if b in self.possede:
            return False
        cout = self.prix(b, jour) * 108 // 100
        if joueur.argent < cout:
            return False
        joueur.argent -= cout
        self.possede.add(b)
        self.squat.discard(b)
        return True

    def vendre(self, joueur, b, jour):
        if b not in self.possede:
            return False
        joueur.argent = min(MAX_MONEY, joueur.argent + self.prix(b, jour) * 95 // 100)
        self.possede.discard(b)
        self.squat.discard(b)
        return True


# ---------------------------------------------------------------- société (src/societe.c)
class Societe:
    def __init__(self, graine):
        self.g, self.etat, self.tresorerie, self.actions, self.sentiment = graine, 0, 0, 10000, 1000

    def alea(self, jour, tirage):
        return melanger(((jour * 0xC2B2AE35) & M32) ^ self.g ^ (tirage << 16) ^ 0x50C1E7E)

    def passer_jour(self, jour):
        if self.etat == 0:
            return
        v = self.alea(jour, 0) % 31 - 15 + (3 if self.tresorerie > 0 else 0)
        self.sentiment = borner(tdiv(self.sentiment * (1000 + v), 1000), 300, 3000)

    def valeur(self, sim, jour):
        if self.etat == 0:
            return 0
        return borner(sim.immo.valeur(jour) + sim.commerce.valeur() + sim.arenes.valeur() + self.tresorerie, 0, CALC_MAX)

    def cours(self, sim, jour):
        return borner(self.valeur(sim, jour) * self.sentiment // 1000 // 10000, 1, 9_999_999)

    def vendre_public(self, sim, n, jour):
        net = n * self.cours(sim, jour) * 97 // 100
        if self.actions < 5100 + n or sim.joueur.argent + net > MAX_MONEY:
            return 0
        self.actions -= n
        sim.joueur.argent += net
        return net

    def racheter(self, sim, jour):
        cout = 500 * self.cours(sim, jour) * 103 // 100
        if self.etat != 2 or self.actions + 500 > 10000 or sim.joueur.argent < cout:
            return 0
        sim.joueur.argent -= cout
        self.actions += 500
        return cout

    def retirer(self, joueur):
        part = self.tresorerie * self.actions // 10000
        part = min(part, MAX_MONEY - joueur.argent)
        self.tresorerie = 0
        joueur.argent += part
        return part


# ---------------------------------------------------------------- commerces (src/commerces.c)
PASSAGE = [200, 300, 500, 150, 600, 650, 250, 400, 350, 900, 500, 450]
STOCK_PC, STOCK_ALEA = [80, 100, 120, 150], [20, 30, 40, 70]
PRIX_PC, CONC = [90, 100, 115], [5, 10, 20]
NIVEAU_PC = [100, 160, 220]
AGR_BOUTIQUE, AGR_CENTRE = [40000, 100000], [60000, 150000]


class Commerce:
    def __init__(self, graine):
        self.g = graine
        self.boutique = None  # dict(ville, stock, prix, niveau, concurrent, guerre)
        self.centre = None    # niveau
        self.points = 0

    def alea(self, jour, tirage):
        return melanger(((jour * 0x27D4EB2F) & M32) ^ self.g ^ (tirage << 18) ^ 0xC0FFEE)

    def recette_moyenne(self):
        b = self.boutique
        if not b:
            return 0
        r = PASSAGE[b['ville']] * STOCK_PC[b['stock']] * NIVEAU_PC[b['niveau']] * PRIX_PC[b['prix']] // 1_000_000
        return r * 60 // 100 if b['concurrent'] else r

    def passer_jour(self, jour, joueur):
        b = self.boutique
        if b:
            a = STOCK_ALEA[b['stock']]
            ecart = self.alea(jour, 1) % (2 * a + 1) - a
            r = self.recette_moyenne() * (100 + ecart) // 100
            if jour_semaine(jour) == SAT:
                r = r * 130 // 100
            elif jour_semaine(jour) == SUN:
                r //= 2
            joueur.encaisser_societe(r, 'boutique')
            if b['concurrent']:
                if b['prix'] == 0:
                    b['guerre'] += 1
                    if b['guerre'] >= 5:
                        b['concurrent'], b['guerre'] = False, 0
                else:
                    b['guerre'] = 0
            elif self.alea(jour, 2) % 1000 < CONC[b['prix']]:
                b['concurrent'], b['guerre'] = True, 0
        if self.centre is not None:
            n = self.centre + 1
            self.points = min(9999, self.points + 30 * n * (2 if jour_semaine(jour) == SUN else 1))
            joueur.encaisser_societe(250 * n, 'centre')

    def valeur(self):
        v = 0
        if self.boutique:
            v += PASSAGE[self.boutique['ville']] * 150 + sum(AGR_BOUTIQUE[:self.boutique['niveau']])
        if self.centre is not None:
            v += 80000 + sum(AGR_CENTRE[:self.centre])
        return v


# ---------------------------------------------------------------- banques (src/banques.c)
TAUX = [30, 50, 200]  # pour dix mille par jour : Devon, Foncier, Casino
B_DEVON, B_FONCIER, B_CASINO = range(3)


class Banques:
    def __init__(self, monde):
        self.m, self.dette, self.defauts, self.paye = monde, [0, 0, 0], [0, 0, 0], 0

    def plafond(self, b, sim, jour):
        if b == B_DEVON:
            return 30000 + 40000 * self.m.badges + (1_000_000 if self.m.ligue else 0)
        if b == B_FONCIER:
            return borner((sim.immo.valeur(jour) + sim.commerce.valeur()) * 60 // 100, 0, CALC_MAX)
        return 200000 + 50000 * self.m.badges

    def emprunter(self, sim, b, montant, jour):
        if self.interdit():
            return 0
        p = self.plafond(b, sim, jour)
        montant = min(montant, max(0, p - self.dette[b]), MAX_MONEY - sim.joueur.argent)
        self.dette[b] += montant
        sim.joueur.argent += montant
        return montant

    def passer_jour(self, jour, sim):
        j = sim.joueur
        for b in range(3):
            if not self.dette[b]:
                continue
            self.dette[b] = borner(self.dette[b] + (self.dette[b] * TAUX[b] + 9999) // 10000, 0, CALC_MAX)
            if jour_semaine(jour) == MON:
                ech = self.dette[b] * 5 // 100
                if ech < 500:
                    ech = min(self.dette[b], 500)
                if self.prelever(sim, ech):
                    self.dette[b] -= ech
                    self.paye += ech
                    if self.dette[b] == 0:
                        self.defauts[b] = 0
                else:
                    self.defaut(b, jour, sim)

    def prelever(self, sim, montant):
        j, soc, bo = sim.joueur, sim.societe, sim.bourse
        if V041:
            if j.argent >= montant:
                j.argent -= montant
                return True
            return False
        dispo = j.argent + (soc.tresorerie if soc.etat else 0) + bo.valeur() * 97 // 100
        if dispo < montant:
            return False
        pris = min(montant, j.argent)
        j.argent -= pris
        montant -= pris
        if soc.etat:
            pris = min(montant, soc.tresorerie)
            soc.tresorerie -= pris
            montant -= pris
        for a in range(9, -1, -1):
            if montant <= 0:
                break
            c = bo.cours_actif(a)
            if not bo.parts[a]:
                continue
            bo.vendre(j, a, (montant * 100 + c * 97 - 1) // (c * 97))
            pris = min(montant, j.argent)
            j.argent -= pris
            montant -= pris
        return montant == 0

    def interdit(self):
        return not V041 and any(self.dette[b] and self.defauts[b] >= 2 for b in range(3))

    def saisir_maison(self, b, jour, sim):
        if not sim.immo.possede:
            return False
        moins = min(sim.immo.possede, key=lambda x: sim.immo.prix(x, jour))
        produit = sim.immo.prix(moins, jour) * 80 // 100
        sim.immo.possede.discard(moins)
        if produit >= self.dette[b]:
            sim.joueur.argent += produit - self.dette[b]
            self.dette[b] = 0
        else:
            self.dette[b] -= produit
        return True

    def defaut(self, b, jour, sim):
        self.defauts[b] += 1
        j = sim.joueur
        if b == B_DEVON:
            self.dette[b] = borner(self.dette[b] * 110 // 100, 0, CALC_MAX)
        elif b == B_FONCIER:
            if not sim.immo.possede:
                self.dette[b] = borner(self.dette[b] * 110 // 100, 0, CALC_MAX)
                return
            moins = min(sim.immo.possede, key=lambda x: sim.immo.prix(x, jour))
            produit = sim.immo.prix(moins, jour) * 80 // 100
            sim.immo.possede.discard(moins)
            if produit >= self.dette[b]:
                j.argent += produit - self.dette[b]
                self.dette[b] = 0
            else:
                self.dette[b] -= produit
        else:
            pris = min(j.argent, self.dette[b])
            j.argent -= pris
            self.dette[b] -= pris
            self.paye += pris
            self.dette[b] = borner(self.dette[b] * 120 // 100, 0, CALC_MAX)
        if V041:
            return
        n = self.defauts[b]
        if n >= 4:
            while self.dette[b] and self.saisir_maison(b, jour, sim):
                pass
        elif n >= 2 and b != B_FONCIER:
            self.saisir_maison(b, jour, sim)
        if self.dette[b] == 0:
            self.defauts[b] = 0


# ---------------------------------------------------------------- arènes (src/arenes.c), joueur supposé gagner chaque défi
class Arenes:
    def __init__(self, graine):
        self.g, self.arenes, self.gains_defis = graine, [], 0

    def valeur(self):
        return 500000 * len(self.arenes)

    def passer_jour(self, jour, joueur, defis_par_jour=1):
        for a in self.arenes:
            joueur.encaisser_societe(150 * a['rep'], 'arènes')
            for _ in range(min(defis_par_jour, 1 + a['rep'] // 40)):
                badges = 1 + melanger(jour * 31 + a['id']) % 8
                joueur.encaisser_societe(2000 * badges, 'arènes')
                self.gains_defis += 2000 * badges
                a['rep'] = min(100, a['rep'] + 3)


# ---------------------------------------------------------------- joueur
class Joueur:
    def __init__(self, argent, sim):
        self.argent, self.sim, self.flux = argent, sim, {}

    def noter(self, poste, v):
        self.flux[poste] = self.flux.get(poste, 0) + v

    def ajouter(self, v, poste):
        if v:
            self.argent = min(MAX_MONEY, self.argent + v)
            self.noter(poste, v)

    def encaisser_societe(self, v, poste):
        s = self.sim.societe
        if s.etat:
            s.tresorerie = min(CALC_MAX, s.tresorerie + v)
        else:
            self.argent = min(MAX_MONEY, self.argent + v)
        self.noter(poste, v)

    def payer(self, v, poste):
        s = self.sim.societe
        if s.etat:
            pris = min(v, s.tresorerie)
            s.tresorerie -= pris
            v2 = v - pris
        else:
            v2 = v
        self.argent -= min(v2, self.argent)
        self.noter(poste, -v)


# ---------------------------------------------------------------- simulation
class Sim:
    def __init__(self, graine, monde, argent, jour0=1000):
        self.monde, self.jour, self.jour_max = monde, jour0, jour0
        self.joueur = Joueur(argent, self)
        self.bourse, self.immo = Bourse(graine, monde), Immo(graine, monde)
        self.societe, self.commerce = Societe(graine), Commerce(graine)
        self.banques, self.arenes = Banques(monde), Arenes(graine)

    def passer_jour(self):
        self.jour += 1
        j = self.jour
        if not V041:  # v0.42 : un jour déjà traité n'est jamais rejoué (horloge reculée)
            if j <= self.jour_max:
                return
            self.jour_max = j
        self.bourse.passer_jour(j, self.joueur)
        self.immo.passer_jour(j, self.joueur)
        self.commerce.passer_jour(j, self.joueur)
        self.banques.passer_jour(j, self)
        self.societe.passer_jour(j)
        self.arenes.passer_jour(j, self.joueur)
        # squatteurs expulsés au bout de 3 jours (combat contre un sbire)
        if self.immo.squat and j % 3 == 0:
            self.immo.squat.clear()

    def dettes(self):
        return self.bourse.dette + sum(self.banques.dette)

    def patrimoine(self):
        s = self.societe
        part_soc = 0
        if s.etat:
            # les biens comptent pour le joueur ; la trésorerie et les autres actifs de la société au prorata
            part_soc = (s.tresorerie + self.commerce.valeur() + self.arenes.valeur()) * s.actions // 10000
        else:
            part_soc = self.commerce.valeur() + self.arenes.valeur()
        return (self.joueur.argent + self.bourse.valeur() + self.immo.valeur(self.jour) + part_soc - self.dettes())


def acheter_maisons(sim, garde=10000):
    for b in sorted(range(12), key=lambda x: sim.immo.prix(x, sim.jour)):
        if b not in sim.immo.possede and sim.joueur.argent - sim.immo.prix(b, sim.jour) * 108 // 100 >= garde:
            sim.immo.acheter(sim.joueur, b, sim.jour)


def profil_prudent(graine, jours):
    monde = Monde(badges=4, ligue=False)
    sim = Sim(graine, monde, 150000)
    sim.bourse.acheter(sim.joueur, MAGASINS, 40)
    journal = []
    for d in range(jours):
        sim.passer_jour()
        acheter_maisons(sim, garde=20000)
        if sim.commerce.boutique is None and sim.joueur.argent > 80000:
            sim.joueur.argent -= PASSAGE[1] * 150
            sim.commerce.boutique = dict(ville=1, stock=1, prix=1, niveau=0, concurrent=False, guerre=0)
        if sim.commerce.boutique and sim.commerce.boutique['concurrent']:
            sim.commerce.boutique['prix'] = 0
        elif sim.commerce.boutique:
            sim.commerce.boutique['prix'] = 1
        journal.append(sim.patrimoine())
    return sim, journal


def profil_agressif(graine, jours):
    monde = Monde(badges=4, ligue=False)
    sim = Sim(graine, monde, 60000)
    sim.banques.emprunter(sim, B_CASINO, 10**9, sim.jour)
    sim.banques.emprunter(sim, B_DEVON, 10**9, sim.jour)
    acheter_maisons(sim, garde=30000)
    sim.banques.emprunter(sim, B_FONCIER, 10**9, sim.jour)
    sim.bourse.acheter(sim.joueur, CASINO, 9999)
    sim.bourse.emprunter(sim.joueur, 10**9)
    sim.bourse.acheter(sim.joueur, CASINO, 9999)
    journal = []
    for d in range(jours):
        sim.passer_jour()
        acheter_maisons(sim, garde=60000)
        journal.append(sim.patrimoine())
    return sim, journal


def profil_levier_raisonne(graine, jours):
    """Emprunte à la Banque Devon (0,3 %/jour) pour acheter des maisons, garde de quoi payer les échéances du lundi."""
    monde = Monde(badges=4, ligue=False)
    sim = Sim(graine, monde, 150000)
    sim.banques.emprunter(sim, B_DEVON, 10**9, sim.jour)
    acheter_maisons(sim, garde=40000)
    journal = []
    for d in range(jours):
        sim.passer_jour()
        acheter_maisons(sim, garde=40000)
        if sim.joueur.argent > 120000 and sim.banques.dette[B_DEVON]:
            rembourse = min(sim.joueur.argent - 80000, sim.banques.dette[B_DEVON])
            sim.joueur.argent -= rembourse
            sim.banques.dette[B_DEVON] -= rembourse
        journal.append(sim.patrimoine())
    return sim, journal


def profil_post_ligue(graine, jours):
    monde = Monde(badges=8, ligue=True, aqua_faillite=True, magma_faillite=True, elo=1800)
    sim = Sim(graine, monde, 2_000_000)
    sim.societe.etat = 1
    sim.joueur.argent -= 20000
    sim.commerce.boutique = dict(ville=9, stock=3, prix=1, niveau=2, concurrent=False, guerre=0)
    sim.joueur.argent -= PASSAGE[9] * 150 + sum(AGR_BOUTIQUE)
    sim.commerce.centre = 2
    sim.joueur.argent -= 80000 + sum(AGR_CENTRE)
    acheter_maisons(sim, garde=1_000_000)
    sim.arenes.arenes.append(dict(id=0, rep=20))
    sim.joueur.argent -= 500000
    journal = []
    for d in range(jours):
        sim.passer_jour()
        if sim.societe.tresorerie > 600000 and len(sim.arenes.arenes) < 6:
            sim.societe.tresorerie -= 500000
            sim.arenes.arenes.append(dict(id=len(sim.arenes.arenes), rep=20))
        if sim.societe.etat == 1 and sim.societe.valeur(sim, sim.jour) >= 100000:
            sim.societe.vendre_public(sim, 2500, sim.jour)
            sim.societe.etat = 2
        acheter_maisons(sim, garde=200000)
        journal.append(sim.patrimoine())
    return sim, journal


# ---------------------------------------------------------------- failles
def faille_societe(graine):
    """Investir dans les commerces, attendre l'humeur haute, vendre 49 %, racheter quand elle est basse."""
    monde = Monde(badges=8, ligue=True, aqua_faillite=True, magma_faillite=True)
    sim = Sim(graine, monde, 500_000)
    sim.societe.etat = 1
    sim.joueur.argent -= 20000
    sim.commerce.boutique = dict(ville=9, stock=3, prix=1, niveau=2, concurrent=False, guerre=0)
    sim.joueur.argent -= PASSAGE[9] * 150 + sum(AGR_BOUTIQUE)
    depart = sim.joueur.argent + sim.commerce.valeur()
    sim.societe.vendre_public(sim, 2500, sim.jour)
    sim.societe.etat = 2
    haut = bas = None
    ventes = rachats = 0
    for d in range(365):
        sim.passer_jour()
        sim.societe.tresorerie = 0  # le joueur ne laisse rien dans la trésorerie : l'humeur ne monte pas d'elle-même
        s = sim.societe.sentiment
        if s >= 2000 and sim.societe.actions > 5100:
            while sim.societe.actions >= 5600:
                ventes += sim.societe.vendre_public(sim, 500, sim.jour)
        elif s <= 700 and sim.societe.actions < 10000:
            while sim.societe.actions + 500 <= 10000 and sim.joueur.argent > 500 * sim.societe.cours(sim, sim.jour) * 2:
                rachats += sim.societe.racheter(sim, sim.jour)
        haut = max(haut or s, s)
        bas = min(bas or s, s)
    return dict(depart=depart, fin=sim.joueur.argent + sim.commerce.valeur(), ventes=ventes, rachats=rachats,
                humeur=(bas, haut))


def faille_double_comptage(graine):
    """Les maisons du joueur gonflent la valeur de SA société : vendre 49 % des actions, puis vendre les maisons."""
    monde = Monde(badges=8, ligue=True, aqua_faillite=True, magma_faillite=True)
    sim = Sim(graine, monde, 2_000_000)
    for b in range(12):
        sim.immo.acheter(sim.joueur, b, sim.jour)
    apres_achat = sim.joueur.argent + sim.immo.valeur(sim.jour)
    sim.societe.etat = 1
    sim.joueur.argent -= 20000
    sim.societe.vendre_public(sim, 2500, sim.jour)
    sim.societe.etat = 2
    while sim.societe.actions >= 5600:
        sim.societe.vendre_public(sim, 500, sim.jour)
    for b in range(12):
        sim.immo.vendre(sim.joueur, b, sim.jour)
    return dict(avant=apres_achat, apres=sim.joueur.argent, actions=sim.societe.actions,
                valeur_soc=sim.societe.valeur(sim, sim.jour))


def faille_bonne_affaire(graine):
    """Acheter le bien soldé de la semaine (-25 %) et le revendre la semaine où son écart local est le plus haut."""
    monde = Monde(badges=8, ligue=True, aqua_faillite=True, magma_faillite=True)
    sim = Sim(graine, monde, 3_000_000)
    gains, operations = 0, 0
    for semaine in range(30):
        j = sim.jour
        b = sim.immo.bonne_affaire(j)
        if b not in sim.immo.possede:
            cout = sim.immo.prix(b, j) * 108 // 100
            if sim.immo.acheter(sim.joueur, b, j):
                for _ in range(7):
                    sim.passer_jour()
                # revendre si la plus-value nette est positive (sinon garder : il rapporte un loyer)
                net = sim.immo.prix(b, sim.jour) * 95 // 100
                if net > cout:
                    sim.immo.vendre(sim.joueur, b, sim.jour)
                    gains += net - cout
                    operations += 1
                continue
        for _ in range(7):
            sim.passer_jour()
    return dict(gains=gains, operations=operations)


def faille_horloge(graine):
    """Remettre l'horloge 30 jours en arrière puis la ramener : les 30 jours sont rejoués (loyers, cours, dividendes)."""
    monde = Monde(badges=8, ligue=True, aqua_faillite=True, magma_faillite=True)
    sim = Sim(graine, monde, 3_000_000)
    for b in range(12):
        sim.immo.acheter(sim.joueur, b, sim.jour)
    sim.bourse.acheter(sim.joueur, MAGASINS, 2000)
    debut = sim.jour
    for _ in range(30):
        sim.passer_jour()
    normal = sim.patrimoine()
    tours = 5
    for _ in range(tours):
        sim.jour = debut  # aujourdhui < X.jour : chaque module recale son jour et rejoue la fenêtre
        for _ in range(30):
            sim.passer_jour()
    return dict(normal=normal, triche=sim.patrimoine(), tours=tours)


def faille_defaut_bancaire(graine):
    """Emprunter le maximum puis ne jamais avoir de liquide le lundi : seules des pénalités chiffrées, rien n'est saisi."""
    monde = Monde(badges=8, ligue=True, aqua_faillite=True, magma_faillite=True)
    sim = Sim(graine, monde, 0)
    recu = sim.banques.emprunter(sim, B_DEVON, 10**9, sim.jour) + sim.banques.emprunter(sim, B_CASINO, 10**9, sim.jour)
    for a in (MAGASINS, DEVON, PENSION, SAFARI, TELE):  # tout est placé en Bourse : plus de liquide le lundi
        sim.bourse.acheter(sim.joueur, a, 9999)
    for _ in range(100):
        sim.passer_jour()
    garde = sim.joueur.argent + sim.bourse.valeur()
    return dict(recu=recu, garde=garde, dettes=sum(sim.banques.dette), defauts=list(sim.banques.defauts),
                interdit=sim.banques.interdit())


def primes_couronnement():
    lignes = []
    for saison in range(1, 6):
        m = min(300, 100 + 50 * (saison - 1))
        paliers = (30000 + 100000 + 300000 + 1_000_000) * m // 100
        tournois = 0 if saison == 1 else sum((20000 + 10000 * t) * m // 100 for t in range(11))
        finale = (500000 + 3_000_000) * m // 100
        lignes.append((saison, m, paliers, tournois, finale, paliers + tournois + finale))
    return lignes


# ---------------------------------------------------------------- rapport
def fmt(n):
    s = f'{abs(int(n)):,}'.replace(',', ' ')
    return ('-' if n < 0 else '') + s


def rapport(graine, jours):
    regles = 'v0.41.1' if V041 else 'v0.42 (correctifs horloge et banques)'
    out = [f'# Audit économique, règles {regles} (simulation, graine 0x{graine:08X}, {jours} jours)', '']
    profils = [('Prudent (4 badges, 150 000 ¥, sans crédit)', profil_prudent),
               ('Agressif (4 badges, 60 000 ¥, crédits au maximum, tout en Bourse)', profil_agressif),
               ('Levier raisonné (4 badges, 150 000 ¥, Banque Devon)', profil_levier_raisonne),
               ('Post-Ligue (2 000 000 ¥, société, commerces, arènes)', profil_post_ligue)]
    out += ['## Profils', '', '| Profil | J+10 | J+30 | J+60 | J+100 | Dettes à la fin | Maisons |', '|---|---|---|---|---|---|---|']
    details = []
    for nom, f in profils:
        sim, j = f(graine, jours)
        pts = [j[min(i, len(j)) - 1] for i in (10, 30, 60, jours)]
        out.append(f'| {nom} | ' + ' | '.join(fmt(p) for p in pts) + f' | {fmt(sim.dettes())} | {len(sim.immo.possede)} |')
        flux = ', '.join(f'{k} {fmt(v)}' for k, v in sorted(sim.joueur.flux.items(), key=lambda kv: -abs(kv[1])))
        details.append(f'- **{nom.split(" (")[0]}** : flux sur {jours} jours : {flux}. Huissier : {fmt(sim.bourse.huissier)} ; '
                       f'défauts bancaires Devon / Foncier / Casino : {sim.banques.defauts}.')
    out += ['', 'Patrimoine net (liquide + Bourse + maisons + part de société - dettes).', ''] + details + ['']

    out += ['## Failles rejouées', '']
    f = faille_societe(graine)
    out.append(f'- **Humeur de la société** : humeur entre {f["humeur"][0]} et {f["humeur"][1]} pour mille. '
               f'Ventes d\'actions {fmt(f["ventes"])} ¥, rachats {fmt(f["rachats"])} ¥ ; '
               f'liquide + commerces : {fmt(f["depart"])} -> {fmt(f["fin"])} ¥ en 1 an, sans rien produire.')
    f = faille_double_comptage(graine)
    out.append(f'- **Double comptage maisons / société** : 12 maisons achetées ({fmt(f["avant"])} ¥ de patrimoine), '
               f'49 % des actions vendues puis les maisons revendues : {fmt(f["apres"])} ¥ de liquide ; '
               f'la société cotée ne vaut plus que {fmt(f["valeur_soc"])} ¥.')
    f = faille_bonne_affaire(graine)
    out.append(f'- **Bien soldé de la semaine** : {f["operations"]} allers-retours rentables en 30 semaines, '
               f'+{fmt(f["gains"])} ¥ de plus-value nette (en plus des loyers).')
    f = faille_horloge(graine)
    out.append(f'- **Horloge remise en arrière** : patrimoine {fmt(f["normal"])} ¥ après 30 jours normaux, '
               f'{fmt(f["triche"])} ¥ après {f["tours"]} retours en arrière de 30 jours.')
    f = faille_defaut_bancaire(graine)
    out.append(f'- **Crédit jamais remboursé** : {fmt(f["recu"])} ¥ empruntés (Devon + Casino), tout placé en Bourse ; '
               f'après 100 jours : liquide + placements {fmt(f["garde"])} ¥, dettes {fmt(f["dettes"])} ¥, '
               f'défauts en cours {f["defauts"]}, interdit bancaire : {"oui" if f["interdit"] else "non"}.')
    out += ['', '## Primes du Couronnement par saison', '', '| Saison | Multiplicateur | Paliers | Tournois | Finale gagnée | Total |',
            '|---|---|---|---|---|---|']
    for s, m, p, t, fi, tot in primes_couronnement():
        out.append(f'| {s} | x{m / 100:g} | {fmt(p)} | {fmt(t)} | {fmt(fi)} | {fmt(tot)} |')
    return '\n'.join(out) + '\n'


if __name__ == '__main__':
    ap = argparse.ArgumentParser(description=__doc__.split('\n')[0])
    ap.add_argument('--jours', type=int, default=100)
    ap.add_argument('--graine', type=lambda s: int(s, 0), default=0x2A5F17C3)
    ap.add_argument('--md')
    ap.add_argument('--v041', action='store_true', help='anciennes règles (avant les correctifs)')
    a = ap.parse_args()
    V041 = a.v041
    texte = rapport(a.graine, a.jours)
    if a.md:
        open(a.md, 'w', encoding='utf-8').write(texte)
    print(texte)
