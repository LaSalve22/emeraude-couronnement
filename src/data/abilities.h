const struct AbilityInfo gAbilitiesInfo[ABILITIES_COUNT] =
{
    [ABILITY_NONE] =
    {
        .name = _("-------"),
        .description = COMPOUND_STRING("Aucun talent."),
        .aiRating = 0,
        .cantBeSwapped = TRUE,
        .cantBeTraced = TRUE,
    },

    [ABILITY_STENCH] =
    {
        .name = _("Puanteur"),
        .description = COMPOUND_STRING("Repousse POKéMON sauvage."),
        .aiRating = 1,
    },

    [ABILITY_DRIZZLE] =
    {
        .name = _("Crachin"),
        .description = COMPOUND_STRING("Fait pleuvoir en entrant."),
        .aiRating = 9,
    },

    [ABILITY_SPEED_BOOST] =
    {
        .name = _("Turbo"),
        .description = COMPOUND_STRING("Améliore la VITESSE."),
        .aiRating = 9,
    },

    [ABILITY_BATTLE_ARMOR] =
    {
        .name = _("Armurbaston"),
        .description = COMPOUND_STRING("Bloque les coups critiques."),
        .aiRating = 2,
        .breakable = TRUE,
    },

    [ABILITY_STURDY] =
    {
        .name = _("Fermeté"),
        .description = COMPOUND_STRING("Résiste aux K.O. en un coup."),
        .aiRating = 6,
        .breakable = TRUE,
    },

    [ABILITY_DAMP] =
    {
        .name = _("Moiteur"),
        .description = COMPOUND_STRING("Empêche auto-destruction."),
        .aiRating = 2,
        .breakable = TRUE,
    },

    [ABILITY_LIMBER] =
    {
        .name = _("Échauffement"),
        .description = COMPOUND_STRING("Empêche la paralysie."),
        .aiRating = 3,
        .breakable = TRUE,
    },

    [ABILITY_SAND_VEIL] =
    {
        .name = _("Voile Sable"),
        .description = COMPOUND_STRING("Evasion si tempête sable."),
        .aiRating = 3,
        .breakable = TRUE,
    },

    [ABILITY_STATIC] =
    {
        .name = _("Statik"),
        .description = COMPOUND_STRING("Paralyse au toucher."),
        .aiRating = 4,
    },

    [ABILITY_VOLT_ABSORB] =
    {
        .name = _("Absorbe-Volt"),
        .description = COMPOUND_STRING("Change l'électricité en PV."),
        .aiRating = 7,
        .breakable = TRUE,
    },

    [ABILITY_WATER_ABSORB] =
    {
        .name = _("Absorbe-Eau"),
        .description = COMPOUND_STRING("Change l'eau en PV."),
        .aiRating = 7,
        .breakable = TRUE,
    },

    [ABILITY_OBLIVIOUS] =
    {
        .name = _("Benêt"),
        .description = COMPOUND_STRING("Empêche l'attirance."),
        .aiRating = 2,
        .breakable = TRUE,
    },

    [ABILITY_CLOUD_NINE] =
    {
        .name = _("Ciel Gris"),
        .description = COMPOUND_STRING("Annule les effets du climat."),
        .aiRating = 5,
    },

    [ABILITY_COMPOUND_EYES] =
    {
        .name = _("Œil Composé"),
        .description = COMPOUND_STRING("Augmente la précision."),
        .aiRating = 7,
    },

    [ABILITY_INSOMNIA] =
    {
        .name = _("Insomnia"),
        .description = COMPOUND_STRING("Empêche de s'endormir."),
        .aiRating = 4,
        .breakable = TRUE,
    },

    [ABILITY_COLOR_CHANGE] =
    {
        .name = _("Homochromie"),
        .description = COMPOUND_STRING("Prend le type de l'attaque reçue."),
        .aiRating = 2,
    },

    [ABILITY_IMMUNITY] =
    {
        .name = _("Vaccin"),
        .description = COMPOUND_STRING("Empêche l'empoisonnement."),
        .aiRating = 4,
        .breakable = TRUE,
    },

    [ABILITY_FLASH_FIRE] =
    {
        .name = _("Torche"),
        .description = COMPOUND_STRING("Puissance monte si brûlé."),
        .aiRating = 6,
        .breakable = TRUE,
    },

    [ABILITY_SHIELD_DUST] =
    {
        .name = _("Écran Poudre"),
        .description = COMPOUND_STRING("Empêche le cumul d'effets."),
        .aiRating = 5,
        .breakable = TRUE,
    },

    [ABILITY_OWN_TEMPO] =
    {
        .name = _("Tempo Perso"),
        .description = COMPOUND_STRING("Empêche la confusion."),
        .aiRating = 3,
        .breakable = TRUE,
    },

    [ABILITY_SUCTION_CUPS] =
    {
        .name = _("Ventouse"),
        .description = COMPOUND_STRING("Fixe solidement le corps."),
        .aiRating = 2,
        .breakable = TRUE,
    },

    [ABILITY_INTIMIDATE] =
    {
        .name = _("Intimidation"),
        .description = COMPOUND_STRING("Baisse l'Attaque ennemie."),
        .aiRating = 7,
    },

    [ABILITY_SHADOW_TAG] =
    {
        .name = _("Marque Ombre"),
        .description = COMPOUND_STRING("Empêche la fuite ennemie."),
        .aiRating = 10,
    },

    [ABILITY_ROUGH_SKIN] =
    {
        .name = _("Peau Dure"),
        .description = COMPOUND_STRING("Blesse quand touché."),
        .aiRating = 6,
    },

    [ABILITY_WONDER_GUARD] =
    {
        .name = _("Garde Mystik"),
        .description = COMPOUND_STRING("Reçoit seul. sup. eff.."),
        .aiRating = 10,
        .cantBeCopied = TRUE,
        .cantBeSwapped = TRUE,
        .breakable = TRUE,
    },

    [ABILITY_LEVITATE] =
    {
        .name = _("Lévitation"),
        .description = COMPOUND_STRING("Ignore les attaques SOL."),
        .aiRating = 7,
        .breakable = TRUE,
    },

    [ABILITY_EFFECT_SPORE] =
    {
        .name = _("Pose Spore"),
        .description = COMPOUND_STRING("Laisse spores si touché."),
        .aiRating = 4,
    },

    [ABILITY_SYNCHRONIZE] =
    {
        .name = _("Synchro"),
        .description = COMPOUND_STRING("Transfère statuts."),
        .aiRating = 4,
    },

    [ABILITY_CLEAR_BODY] =
    {
        .name = _("Corps Sain"),
        .description = COMPOUND_STRING("Empêche réduction de cap."),
        .aiRating = 4,
        .breakable = TRUE,
    },

    [ABILITY_NATURAL_CURE] =
    {
        .name = _("Médic Nature"),
        .description = COMPOUND_STRING("Soigne POKéMON changeant."),
        .aiRating = 7,
    },

    [ABILITY_LIGHTNING_ROD] =
    {
        .name = _("Paratonnerre"),
        .description = COMPOUND_STRING("Attire l'Électrik, Atq. Spé. +1."),
        .aiRating = 7,
        .breakable = TRUE,
    },

    [ABILITY_SERENE_GRACE] =
    {
        .name = _("Sérénité"),
        .description = COMPOUND_STRING("Favorise le cumul d'effet."),
        .aiRating = 8,
    },

    [ABILITY_SWIFT_SWIM] =
    {
        .name = _("Glissade"),
        .description = COMPOUND_STRING("Monte VITESSE s'il pleut."),
        .aiRating = 6,
    },

    [ABILITY_CHLOROPHYLL] =
    {
        .name = _("Chlorophylle"),
        .description = COMPOUND_STRING("Augmente vitesse au soleil."),
        .aiRating = 6,
    },

    [ABILITY_ILLUMINATE] =
    {
        .name = _("Lumiattirance"),
        .description = 
        #if B_ILLUMINATE_EFFECT >= GEN_9
            COMPOUND_STRING("Accuracy can't be lowered."),
        #else
            COMPOUND_STRING("Encounter rate increases."),
        #endif
        .aiRating = 0,
        .breakable = TRUE,
    },

    [ABILITY_TRACE] =
    {
        .name = _("Calque"),
        .description = COMPOUND_STRING("Imite le talent de l'ennemi."),
        .aiRating = 6,
        .cantBeCopied = TRUE,
        .cantBeTraced = TRUE, //B_UPDATED_ABILITY_DATA >= GEN_4
    },

    [ABILITY_HUGE_POWER] =
    {
        .name = _("Coloforce"),
        .description = COMPOUND_STRING("Augmente ATTAQUE."),
        .aiRating = 10,
    },

    [ABILITY_POISON_POINT] =
    {
        .name = _("Point Poison"),
        .description = COMPOUND_STRING("Empoisonne quand touché."),
        .aiRating = 4,
    },

    [ABILITY_INNER_FOCUS] =
    {
        .name = _("Attention"),
        .description = COMPOUND_STRING("Empêche la peur."),
        .aiRating = 2,
        .breakable = TRUE,
    },

    [ABILITY_MAGMA_ARMOR] =
    {
        .name = _("Armumagma"),
        .description = COMPOUND_STRING("Empêche le gel."),
        .aiRating = 1,
        .breakable = TRUE,
    },

    [ABILITY_WATER_VEIL] =
    {
        .name = _("Ignifu-Voile"),
        .description = COMPOUND_STRING("Empêche les brûlures."),
        .aiRating = 4,
        .breakable = TRUE,
    },

    [ABILITY_MAGNET_PULL] =
    {
        .name = _("Magnépiège"),
        .description = COMPOUND_STRING("Piège les POKéMON ACIER."),
        .aiRating = 9,
    },

    [ABILITY_SOUNDPROOF] =
    {
        .name = _("Anti-Bruit"),
        .description = COMPOUND_STRING("Évite les attaques sonores."),
        .aiRating = 4,
        .breakable = TRUE,
    },

    [ABILITY_RAIN_DISH] =
    {
        .name = _("Cuvette"),
        .description = COMPOUND_STRING("Gagne quelques PV si pluie."),
        .aiRating = 3,
    },

    [ABILITY_SAND_STREAM] =
    {
        .name = _("Sable Volant"),
        .description = COMPOUND_STRING("Lance une tempête de sable."),
        .aiRating = 9,
    },

    [ABILITY_PRESSURE] =
    {
        .name = _("Pression"),
        .description = COMPOUND_STRING("Force l'ennemi à dépenser plus de PP."),
        .aiRating = 5,
    },

    [ABILITY_THICK_FAT] =
    {
        .name = _("Isograisse"),
        .description = COMPOUND_STRING("Contre chaleur et froid."),
        .aiRating = 7,
        .breakable = TRUE,
    },

    [ABILITY_EARLY_BIRD] =
    {
        .name = _("Matinal"),
        .description = COMPOUND_STRING("Se réveille rapidement."),
        .aiRating = 4,
    },

    [ABILITY_FLAME_BODY] =
    {
        .name = _("Corps Ardent"),
        .description = COMPOUND_STRING("Brûle l'ennemi quand touché."),
        .aiRating = 4,
    },

    [ABILITY_RUN_AWAY] =
    {
        .name = _("Fuite"),
        .description = COMPOUND_STRING("Améliore chances de fuite."),
        .aiRating = 0,
    },

    [ABILITY_KEEN_EYE] =
    {
        .name = _("Regard Vif"),
        .description = COMPOUND_STRING("Empêche perte de précision."),
        .aiRating = 1,
        .breakable = TRUE,
    },

    [ABILITY_HYPER_CUTTER] =
    {
        .name = _("Hyper Cutter"),
        .description = COMPOUND_STRING("Empêche réduction ATTAQUE."),
        .aiRating = 3,
        .breakable = TRUE,
    },

    [ABILITY_PICKUP] =
    {
        .name = _("Ramassage"),
        .description = COMPOUND_STRING("Ramasse parfois des objets."),
        .aiRating = 1,
    },

    [ABILITY_TRUANT] =
    {
        .name = _("Absentéisme"),
        .description = COMPOUND_STRING("Frappe tous les 2 tours."),
        .aiRating = -2,
        .cantBeOverwritten = TRUE,
    },

    [ABILITY_HUSTLE] =
    {
        .name = _("Agitation"),
        .description = COMPOUND_STRING("Substitue préc. par puiss."),
        .aiRating = 7,
    },

    [ABILITY_CUTE_CHARM] =
    {
        .name = _("Joli Sourire"),
        .description = COMPOUND_STRING("Séduit quand touché."),
        .aiRating = 2,
    },

    [ABILITY_PLUS] =
    {
        .name = _("Plus"),
        .description =
        #if B_PLUS_MINUS_INTERACTION >= GEN_5
            COMPOUND_STRING("Ups Sp. Atk with Minus."),
        #else
            COMPOUND_STRING("Plus or Minus ups Sp. Atk."),
        #endif
        .aiRating = 0,
    },

    [ABILITY_MINUS] =
    {
        .name = _("Moins"),
        .description =
        #if B_PLUS_MINUS_INTERACTION >= GEN_5
            COMPOUND_STRING("Ups Sp. Atk with Plus."),
        #else
            COMPOUND_STRING("Plus or Minus ups Sp. Atk."),
        #endif
        .aiRating = 0,
    },

    [ABILITY_FORECAST] =
    {
        .name = _("Météo"),
        .description = COMPOUND_STRING("Change selon le climat."),
        .aiRating = 6,
        .cantBeCopied = TRUE,
        .cantBeTraced = B_UPDATED_ABILITY_DATA >= GEN_4,
        .failsOnImposter = B_UPDATED_ABILITY_DATA >= GEN_5,
    },

    [ABILITY_STICKY_HOLD] =
    {
        .name = _("Glu"),
        .description = COMPOUND_STRING("Empêche le vol d'objets."),
        .aiRating = 3,
        .breakable = TRUE,
    },

    [ABILITY_SHED_SKIN] =
    {
        .name = _("Mue"),
        .description = COMPOUND_STRING("Mue pour se soigner."),
        .aiRating = 7,
    },

    [ABILITY_GUTS] =
    {
        .name = _("Cran"),
        .description = COMPOUND_STRING("Augmente ATT. quand blessé."),
        .aiRating = 6,
    },

    [ABILITY_MARVEL_SCALE] =
    {
        .name = _("Écaille Spéciale"),
        .description = COMPOUND_STRING("Augmente DEF. quand blessé."),
        .aiRating = 5,
        .breakable = TRUE,
    },

    [ABILITY_LIQUID_OOZE] =
    {
        .name = _("Suintement"),
        .description = COMPOUND_STRING("Blesse quand drainé."),
        .aiRating = 3,
    },

    [ABILITY_OVERGROW] =
    {
        .name = _("Engrais"),
        .description = COMPOUND_STRING("Augmente attaques PLANTE."),
        .aiRating = 5,
    },

    [ABILITY_BLAZE] =
    {
        .name = _("Brasier"),
        .description = COMPOUND_STRING("Augmente attaques feu."),
        .aiRating = 5,
    },

    [ABILITY_TORRENT] =
    {
        .name = _("Torrent"),
        .description = COMPOUND_STRING("Augmente attaques EAU."),
        .aiRating = 5,
    },

    [ABILITY_SWARM] =
    {
        .name = _("Essaim"),
        .description = COMPOUND_STRING("Augmente attaques INSECT."),
        .aiRating = 5,
    },

    [ABILITY_ROCK_HEAD] =
    {
        .name = _("Tête de Roc"),
        .description = COMPOUND_STRING("Empêche dégâts du recul."),
        .aiRating = 5,
    },

    [ABILITY_DROUGHT] =
    {
        .name = _("Sécheresse"),
        .description = COMPOUND_STRING("Invoque le soleil."),
        .aiRating = 9,
    },

    [ABILITY_ARENA_TRAP] =
    {
        .name = _("Piège Sable"),
        .description = COMPOUND_STRING("Empêche la fuite."),
        .aiRating = 9,
    },

    [ABILITY_VITAL_SPIRIT] =
    {
        .name = _("Esprit Vital"),
        .description = COMPOUND_STRING("Empêche le sommeil."),
        .aiRating = 4,
        .breakable = TRUE,
    },

    [ABILITY_WHITE_SMOKE] =
    {
        .name = _("Écran Fumée"),
        .description = COMPOUND_STRING("Empêche réduction des cap."),
        .aiRating = 4,
        .breakable = TRUE,
    },

    [ABILITY_PURE_POWER] =
    {
        .name = _("Force Pure"),
        .description = COMPOUND_STRING("Augmente l'ATTAQUE."),
        .aiRating = 10,
    },

    [ABILITY_SHELL_ARMOR] =
    {
        .name = _("Coque Armure"),
        .description = COMPOUND_STRING("Bloque les coups critiques."),
        .aiRating = 2,
        .breakable = TRUE,
    },

    [ABILITY_AIR_LOCK] =
    {
        .name = _("Air Lock"),
        .description = COMPOUND_STRING("Annule les effets du climat."),
        .aiRating = 5,
    },

    [ABILITY_TANGLED_FEET] =
    {
        .name = _("Pieds Confus"),
        .description = COMPOUND_STRING("Esquive accrue si confus."),
        .aiRating = 2,
        .breakable = TRUE,
    },

    [ABILITY_MOTOR_DRIVE] =
    {
        .name = _("Motorisé"),
        .description = COMPOUND_STRING("L'Électrik augmente la Vitesse."),
        .aiRating = 6,
        .breakable = TRUE,
    },

    [ABILITY_RIVALRY] =
    {
        .name = _("Rivalité"),
        .description = COMPOUND_STRING("Fort contre le même sexe."),
        .aiRating = 1,
    },

    [ABILITY_STEADFAST] =
    {
        .name = _("Impassible"),
        .description = COMPOUND_STRING("Avoir peur augmente la Vitesse."),
        .aiRating = 2,
    },

    [ABILITY_SNOW_CLOAK] =
    {
        .name = _("Rideau Neige"),
        .description = COMPOUND_STRING("Esquive accrue sous la neige."),
        .aiRating = 3,
        .breakable = TRUE,
    },

    [ABILITY_GLUTTONY] =
    {
        .name = _("Gloutonnerie"),
        .description = COMPOUND_STRING("Encourage l'utilisation d'une Baie tenue."),
        .aiRating = 3,
    },

    [ABILITY_ANGER_POINT] =
    {
        .name = _("Colérique"),
        .description = COMPOUND_STRING("Attaque au max si critique reçu."),
        .aiRating = 4,
    },

    [ABILITY_UNBURDEN] =
    {
        .name = _("Délestage"),
        .description = COMPOUND_STRING("Perdre son objet double la Vitesse."),
        .aiRating = 7,
    },

    [ABILITY_HEATPROOF] =
    {
        .name = _("Ignifugé"),
        .description = COMPOUND_STRING("Divise par 2 les dégâts Feu."),
        .aiRating = 5,
        .breakable = TRUE,
    },

    [ABILITY_SIMPLE] =
    {
        .name = _("Simple"),
        .description = COMPOUND_STRING("Double les changements de stats."),
        .aiRating = 8,
        .breakable = TRUE,
    },

    [ABILITY_DRY_SKIN] =
    {
        .name = _("Peau Sèche"),
        .description = COMPOUND_STRING("Eau soigne, Feu et soleil blessent."),
        .aiRating = 6,
        .breakable = TRUE,
    },

    [ABILITY_DOWNLOAD] =
    {
        .name = _("Télécharge"),
        .description = COMPOUND_STRING("Ajuste la puissance selon l'ennemi."),
        .aiRating = 7,
    },

    [ABILITY_IRON_FIST] =
    {
        .name = _("Poing de Fer"),
        .description = COMPOUND_STRING("Renforce les coups de poing."),
        .aiRating = 6,
    },

    [ABILITY_POISON_HEAL] =
    {
        .name = _("Soin Poison"),
        .description = COMPOUND_STRING("Le poison soigne au lieu de blesser."),
        .aiRating = 8,
    },

    [ABILITY_ADAPTABILITY] =
    {
        .name = _("Adaptabilité"),
        .description = COMPOUND_STRING("Renforce les capacités du même type."),
        .aiRating = 8,
    },

    [ABILITY_SKILL_LINK] =
    {
        .name = _("Multi-Coups"),
        .description = COMPOUND_STRING("Les attaques multiples frappent 5 fois."),
        .aiRating = 7,
    },

    [ABILITY_HYDRATION] =
    {
        .name = _("Hydratation"),
        .description = COMPOUND_STRING("Soigne le statut sous la pluie."),
        .aiRating = 4,
    },

    [ABILITY_SOLAR_POWER] =
    {
        .name = _("Force Soleil"),
        .description = COMPOUND_STRING("Soleil : Atq. Spé. +50 %, mais blesse."),
        .aiRating = 3,
    },

    [ABILITY_QUICK_FEET] =
    {
        .name = _("Pied Véloce"),
        .description = COMPOUND_STRING("Vitesse accrue en cas de statut."),
        .aiRating = 5,
    },

    [ABILITY_NORMALIZE] =
    {
        .name = _("Normalise"),
        .description = COMPOUND_STRING("Toutes les capacités deviennent Normal."),
        .aiRating = -1,
    },

    [ABILITY_SNIPER] =
    {
        .name = _("Sniper"),
        .description = COMPOUND_STRING("Renforce les coups critiques."),
        .aiRating = 3,
    },

    [ABILITY_MAGIC_GUARD] =
    {
        .name = _("Garde Magik"),
        .description = COMPOUND_STRING("Blessé seulement par les attaques."),
        .aiRating = 9,
    },

    [ABILITY_NO_GUARD] =
    {
        .name = _("Annule Garde"),
        .description = COMPOUND_STRING("Toutes les attaques touchent."),
        .aiRating = 8,
    },

    [ABILITY_STALL] =
    {
        .name = _("Frein"),
        .description = COMPOUND_STRING("Agit toujours en dernier."),
        .aiRating = -1,
    },

    [ABILITY_TECHNICIAN] =
    {
        .name = _("Technicien"),
        .description = COMPOUND_STRING("Renforce les capacités faibles."),
        .aiRating = 8,
    },

    [ABILITY_LEAF_GUARD] =
    {
        .name = _("Feuille Garde"),
        .description = COMPOUND_STRING("Pas de statut au soleil."),
        .aiRating = 2,
        .breakable = TRUE,
    },

    [ABILITY_KLUTZ] =
    {
        .name = _("Maladresse"),
        .description = COMPOUND_STRING("Ne peut pas utiliser son objet."),
        .aiRating = -1,
    },

    [ABILITY_MOLD_BREAKER] =
    {
        .name = _("Brise Moule"),
        .description = COMPOUND_STRING("Les capacités ignorent les talents."),
        .aiRating = 7,
    },

    [ABILITY_SUPER_LUCK] =
    {
        .name = _("Chanceux"),
        .description = COMPOUND_STRING("Taux de critique augmenté."),
        .aiRating = 3,
    },

    [ABILITY_AFTERMATH] =
    {
        .name = _("Boom Final"),
        .description = COMPOUND_STRING("Blesse qui le met K.O. au contact."),
        .aiRating = 5,
    },

    [ABILITY_ANTICIPATION] =
    {
        .name = _("Anticipation"),
        .description = COMPOUND_STRING("Sent les capacités dangereuses."),
        .aiRating = 2,
    },

    [ABILITY_FOREWARN] =
    {
        .name = _("Prédiction"),
        .description = COMPOUND_STRING("Devine une capacité de l'ennemi."),
        .aiRating = 2,
    },

    [ABILITY_UNAWARE] =
    {
        .name = _("Inconscient"),
        .description = COMPOUND_STRING("Ignore les stats boostées ennemies."),
        .aiRating = 6,
        .breakable = TRUE,
    },

    [ABILITY_TINTED_LENS] =
    {
        .name = _("Lentiteintée"),
        .description = COMPOUND_STRING("Renforce le  pas très efficace ."),
        .aiRating = 7,
    },

    [ABILITY_FILTER] =
    {
        .name = _("Filtre"),
        .description = COMPOUND_STRING("Réduit le  super efficace ."),
        .aiRating = 6,
        .breakable = TRUE,
    },

    [ABILITY_SLOW_START] =
    {
        .name = _("Début Calme"),
        .description = COMPOUND_STRING("Met 5 tours à démarrer."),
        .aiRating = -2,
    },

    [ABILITY_SCRAPPY] =
    {
        .name = _("Querelleur"),
        .description = COMPOUND_STRING("Normal et Combat touchent Spectre."),
        .aiRating = 6,
    },

    [ABILITY_STORM_DRAIN] =
    {
        .name = _("Lavabo"),
        .description = COMPOUND_STRING("Attire l'eau et augmente l'Atq. Spé."),
        .aiRating = 7,
        .breakable = TRUE,
    },

    [ABILITY_ICE_BODY] =
    {
        .name = _("Corps Gel"),
        .description = COMPOUND_STRING("Récupère des PV sous la neige."),
        .aiRating = 3,
    },

    [ABILITY_SOLID_ROCK] =
    {
        .name = _("Solide Roc"),
        .description = COMPOUND_STRING("Réduit le  super efficace ."),
        .aiRating = 6,
        .breakable = TRUE,
    },

    [ABILITY_SNOW_WARNING] =
    {
        .name = _("Alerte Neige"),
    #if B_SNOW_WARNING >= GEN_9
        .description = COMPOUND_STRING("Fait neiger en entrant."),
    #else
        .description = COMPOUND_STRING("Summons hail in battle."),
    #endif
        .aiRating = 8,
    },

    [ABILITY_HONEY_GATHER] =
    {
        .name = _("Cherche Miel"),
        .description = COMPOUND_STRING("Ramasse parfois du Miel."),
        .aiRating = 0,
    },

    [ABILITY_FRISK] =
    {
        .name = _("Fouille"),
        .description = COMPOUND_STRING("Voit l'objet tenu par l'ennemi."),
        .aiRating = 3,
    },

    [ABILITY_RECKLESS] =
    {
        .name = _("Téméraire"),
        .description = COMPOUND_STRING("Renforce les capacités à recul."),
        .aiRating = 6,
    },

    [ABILITY_MULTITYPE] =
    {
        .name = _("Multi-Type"),
        .description = COMPOUND_STRING("Type selon la Plaque tenue."),
        .aiRating = 8,
        .cantBeCopied = TRUE,
        .cantBeSwapped = TRUE,
        .cantBeTraced = TRUE,
        .cantBeSuppressed = TRUE,
        .cantBeOverwritten = TRUE,
        .failsOnImposter = B_UPDATED_ABILITY_DATA >= GEN_5,
    },

    [ABILITY_FLOWER_GIFT] =
    {
        .name = _("Don Floral"),
        .description = COMPOUND_STRING("Soleil : Attaque et Déf. Spé. alliées +."),
        .aiRating = 4,
        .cantBeCopied = TRUE,
        .cantBeTraced = B_UPDATED_ABILITY_DATA >= GEN_5,
        .breakable = TRUE,
    },

    [ABILITY_BAD_DREAMS] =
    {
        .name = _("Mauvais Rêve"),
        .description = COMPOUND_STRING("Réduit les PV d'un ennemi endormi."),
        .aiRating = 4,
    },

    [ABILITY_PICKPOCKET] =
    {
        .name = _("Pickpocket"),
        .description = COMPOUND_STRING("Vole l'objet de qui le touche."),
        .aiRating = 3,
    },

    [ABILITY_SHEER_FORCE] =
    {
        .name = _("Sans Limite"),
        .description = COMPOUND_STRING("Sacrifie les effets pour la puissance."),
        .aiRating = 8,
    },

    [ABILITY_CONTRARY] =
    {
        .name = _("Contestation"),
        .description = COMPOUND_STRING("Inverse les variations de stats."),
        .aiRating = 8,
        .breakable = TRUE,
    },

    [ABILITY_UNNERVE] =
    {
        .name = _("Tension"),
        .description = COMPOUND_STRING("Les ennemis ne mangent plus de Baies."),
        .aiRating = 3,
    },

    [ABILITY_DEFIANT] =
    {
        .name = _("Acharné"),
        .description = COMPOUND_STRING("Une stat baissée : Attaque +2."),
        .aiRating = 5,
    },

    [ABILITY_DEFEATIST] =
    {
        .name = _("Défaitiste"),
        .description = COMPOUND_STRING("Attaques divisées par 2 sous 50 % PV."),
        .aiRating = -1,
    },

    [ABILITY_CURSED_BODY] =
    {
        .name = _("Corps Maudit"),
        .description = COMPOUND_STRING("Peut bloquer la capacité qui le touche."),
        .aiRating = 4,
    },

    [ABILITY_HEALER] =
    {
        .name = _("Cœur Soin"),
        .description = COMPOUND_STRING("Peut soigner le statut d'un allié."),
        .aiRating = 0,
    },

    [ABILITY_FRIEND_GUARD] =
    {
        .name = _("Garde-Ami"),
        .description = COMPOUND_STRING("Diminue les dégâts subis par les alliés."),
        .aiRating = 0,
        .breakable = TRUE,
    },

    [ABILITY_WEAK_ARMOR] =
    {
        .name = _("Armurouillée"),
        .description = COMPOUND_STRING("Coup physique : Défense -, Vitesse +."),
        .aiRating = 2,
    },

    [ABILITY_HEAVY_METAL] =
    {
        .name = _("Heavy Metal"),
        .description = COMPOUND_STRING("Double le poids du Pokémon."),
        .aiRating = -1,
        .breakable = TRUE,
    },

    [ABILITY_LIGHT_METAL] =
    {
        .name = _("Light Metal"),
        .description = COMPOUND_STRING("Divise par deux le poids du Pokémon."),
        .aiRating = 2,
        .breakable = TRUE,
    },

    [ABILITY_MULTISCALE] =
    {
        .name = _("Multiécaille"),
        .description = COMPOUND_STRING("Moins de dégâts à PV pleins."),
        .aiRating = 8,
        .breakable = TRUE,
    },

    [ABILITY_TOXIC_BOOST] =
    {
        .name = _("Rage Poison"),
        .description = COMPOUND_STRING("Attaque accrue s'il est empoisonné."),
        .aiRating = 6,
    },

    [ABILITY_FLARE_BOOST] =
    {
        .name = _("Rage Brûlure"),
        .description = COMPOUND_STRING("Atq. Spé. accrue s'il est brûlé."),
        .aiRating = 5,
    },

    [ABILITY_HARVEST] =
    {
        .name = _("Récolte"),
        .description = COMPOUND_STRING("Les Baies utilisées peuvent repousser."),
        .aiRating = 5,
    },

    [ABILITY_TELEPATHY] =
    {
        .name = _("Télépathe"),
        .description = COMPOUND_STRING("Évite les attaques de ses alliés."),
        .aiRating = 0,
        .breakable = TRUE,
    },

    [ABILITY_MOODY] =
    {
        .name = _("Lunatique"),
        .description = COMPOUND_STRING("Une stat monte, une autre baisse."),
        .aiRating = 10,
    },

    [ABILITY_OVERCOAT] =
    {
        .name = _("Envelocape"),
        .description = COMPOUND_STRING("Neutralise les dégâts dus à la météo."),
        .aiRating = 5,
        .breakable = TRUE,
    },

    [ABILITY_POISON_TOUCH] =
    {
        .name = _("Toxitouche"),
        .description = COMPOUND_STRING("Le contact peut empoisonner."),
        .aiRating = 4,
    },

    [ABILITY_REGENERATOR] =
    {
        .name = _("Régé-Force"),
        .description = COMPOUND_STRING("Récupère des PV en sortant."),
        .aiRating = 8,
    },

    [ABILITY_BIG_PECKS] =
    {
        .name = _("Cœur de Coq"),
        .description = COMPOUND_STRING("Sa Défense ne peut pas baisser."),
        .aiRating = 1,
        .breakable = TRUE,
    },

    [ABILITY_SAND_RUSH] =
    {
        .name = _("Baigne Sable"),
        .description = COMPOUND_STRING("Vitesse doublée dans le sable."),
        .aiRating = 6,
    },

    [ABILITY_WONDER_SKIN] =
    {
        .name = _("Peau Miracle"),
        .description = COMPOUND_STRING("Résiste mieux aux attaques de statut."),
        .aiRating = 4,
        .breakable = TRUE,
    },

    [ABILITY_ANALYTIC] =
    {
        .name = _("Analyste"),
        .description = COMPOUND_STRING("Plus fort s'il agit en dernier."),
        .aiRating = 5,
    },

    [ABILITY_ILLUSION] =
    {
        .name = _("Illusion"),
        .description = COMPOUND_STRING("Se déguise en dernier allié."),
        .aiRating = 8,
        .cantBeCopied = TRUE,
        .cantBeSwapped = TRUE,
        .cantBeTraced = TRUE,
    },

    [ABILITY_IMPOSTER] =
    {
        .name = _("Imposteur"),
        .description = COMPOUND_STRING("Donne l'apparence du Pokémon adverse."),
        .aiRating = 9,
        .cantBeCopied = TRUE,
        .cantBeTraced = TRUE,
    },

    [ABILITY_INFILTRATOR] =
    {
        .name = _("Infiltration"),
        .description = COMPOUND_STRING("Passe à travers murs et clones."),
        .aiRating = 6,
    },

    [ABILITY_MUMMY] =
    {
        .name = _("Momie"),
        .description = COMPOUND_STRING("Transmet son talent au contact."),
        .aiRating = 5,
    },

    [ABILITY_MOXIE] =
    {
        .name = _("Impudence"),
        .description = COMPOUND_STRING("Un K.O. augmente l'Attaque."),
        .aiRating = 7,
    },

    [ABILITY_JUSTIFIED] =
    {
        .name = _("Cœur Noble"),
        .description = COMPOUND_STRING("Touché par Ténèbres : Attaque +1."),
        .aiRating = 4,
    },

    [ABILITY_RATTLED] =
    {
        .name = _("Phobique"),
        .description = COMPOUND_STRING("La peur augmente la Vitesse."),
        .aiRating = 3,
    },

    [ABILITY_MAGIC_BOUNCE] =
    {
        .name = _("Miroir Magik"),
        .description = COMPOUND_STRING("Renvoie les attaques de statut."),
        .aiRating = 9,
        .breakable = TRUE,
    },

    [ABILITY_SAP_SIPPER] =
    {
        .name = _("Herbivore"),
        .description = COMPOUND_STRING("Absorbe la Plante : Attaque +1."),
        .aiRating = 7,
        .breakable = TRUE,
    },

    [ABILITY_PRANKSTER] =
    {
        .name = _("Farceur"),
        .description = COMPOUND_STRING("Capacités de statut prioritaires."),
        .aiRating = 8,
    },

    [ABILITY_SAND_FORCE] =
    {
        .name = _("Force Sable"),
        .description = COMPOUND_STRING("Plus fort dans une tempête de sable."),
        .aiRating = 4,
    },

    [ABILITY_IRON_BARBS] =
    {
        .name = _("Épine de Fer"),
        .description = COMPOUND_STRING("Blesse l'attaquant au moindre contact."),
        .aiRating = 6,
    },

    [ABILITY_ZEN_MODE] =
    {
        .name = _("Mode Transe"),
        .description = COMPOUND_STRING("Change de forme sous 50 % PV."),
        .aiRating = -1,
        .cantBeCopied = TRUE,
        .cantBeSwapped = B_UPDATED_ABILITY_DATA >= GEN_7,
        .cantBeTraced = TRUE,
        .cantBeSuppressed = B_UPDATED_ABILITY_DATA >= GEN_7,
        .cantBeOverwritten = B_UPDATED_ABILITY_DATA >= GEN_7,
        .failsOnImposter = TRUE,
    },

    [ABILITY_VICTORY_STAR] =
    {
        .name = _("Victorieux"),
        .description = COMPOUND_STRING("Précision de l'équipe augmentée."),
        .aiRating = 6,
    },

    [ABILITY_TURBOBLAZE] =
    {
        .name = _("Turbo Brasier"),
        .description = COMPOUND_STRING("Les capacités ignorent les talents."),
        .aiRating = 7,
    },

    [ABILITY_TERAVOLT] =
    {
        .name = _("Téra-Voltage"),
        .description = COMPOUND_STRING("Les capacités ignorent les talents."),
        .aiRating = 7,
    },

    [ABILITY_AROMA_VEIL] =
    {
        .name = _("Aroma-Voile"),
        .description = COMPOUND_STRING("Protège l'équipe de Provoc et autres."),
        .aiRating = 3,
        .breakable = TRUE,
    },

    [ABILITY_FLOWER_VEIL] =
    {
        .name = _("Flora-Voile"),
        .description = COMPOUND_STRING("Protège les alliés Plante."),
        .aiRating = 0,
        .breakable = TRUE,
    },

    [ABILITY_CHEEK_POUCH] =
    {
        .name = _("Bajoues"),
        .description = COMPOUND_STRING("Manger une Baie soigne aussi."),
        .aiRating = 4,
    },

    [ABILITY_PROTEAN] =
    {
        .name = _("Protéen"),
        .description = COMPOUND_STRING("Prend le type de la capacité lancée."),
        .aiRating = 8,
    },

    [ABILITY_FUR_COAT] =
    {
        .name = _("Toison Épaisse"),
        .description = COMPOUND_STRING("Divise par 2 les dégâts physiques."),
        .aiRating = 7,
        .breakable = TRUE,
    },

    [ABILITY_MAGICIAN] =
    {
        .name = _("Magicien"),
        .description = COMPOUND_STRING("Vole l'objet de l'ennemi qu'il frappe."),
        .aiRating = 3,
    },

    [ABILITY_BULLETPROOF] =
    {
        .name = _("Pare-Balles"),
        .description = COMPOUND_STRING("Immunisé aux balles et bombes."),
        .breakable = TRUE,
        .aiRating = 7,
    },

    [ABILITY_COMPETITIVE] =
    {
        .name = _("Battant"),
        .description = COMPOUND_STRING("Une stat baissée : Atq. Spé. +2."),
        .aiRating = 5,
    },

    [ABILITY_STRONG_JAW] =
    {
        .name = _("Prognathe"),
        .description = COMPOUND_STRING("Renforce les morsures."),
        .aiRating = 6,
    },

    [ABILITY_REFRIGERATE] =
    {
        .name = _("Peau Gelée"),
        .description = COMPOUND_STRING("Normal devient Glace et plus fort."),
        .aiRating = 8,
    },

    [ABILITY_SWEET_VEIL] =
    {
        .name = _("Gluco-Voile"),
        .description = COMPOUND_STRING("L'équipe ne peut pas s'endormir."),
        .aiRating = 4,
        .breakable = TRUE,
    },

    [ABILITY_STANCE_CHANGE] =
    {
        .name = _("Déclic Tactique"),
        .description = COMPOUND_STRING("Change de forme selon l'action."),
        .aiRating = 10,
        .cantBeCopied = TRUE,
        .cantBeSwapped = TRUE,
        .cantBeTraced = TRUE,
        .cantBeSuppressed = TRUE,
        .cantBeOverwritten = TRUE,
        .failsOnImposter = TRUE,
    },

    [ABILITY_GALE_WINGS] =
    {
        .name = _("Ailes Bourrasque"),
        .description = COMPOUND_STRING("Capacités Vol prioritaires à PV pleins."),
        .aiRating = 6,
    },

    [ABILITY_MEGA_LAUNCHER] =
    {
        .name = _("Méga Blaster"),
        .description = COMPOUND_STRING("Renforce les capacités  Vibra ."),
        .aiRating = 7,
    },

    [ABILITY_GRASS_PELT] =
    {
        .name = _("Toison Herbue"),
        .description = COMPOUND_STRING("Défense accrue sur Champ Herbu."),
        .aiRating = 2,
        .breakable = TRUE,
    },

    [ABILITY_SYMBIOSIS] =
    {
        .name = _("Symbiose"),
        .description = COMPOUND_STRING("Donne son objet à un allié."),
        .aiRating = 0,
    },

    [ABILITY_TOUGH_CLAWS] =
    {
        .name = _("Griffe Dure"),
        .description = COMPOUND_STRING("Renforce les capacités de contact."),
        .aiRating = 7,
    },

    [ABILITY_PIXILATE] =
    {
        .name = _("Peau Féérique"),
        .description = COMPOUND_STRING("Normal devient Fée et plus fort."),
        .aiRating = 8,
    },

    [ABILITY_GOOEY] =
    {
        .name = _("Poisseux"),
        .description = COMPOUND_STRING("Le contact baisse la Vitesse ennemie."),
        .aiRating = 5,
    },

    [ABILITY_AERILATE] =
    {
        .name = _("Peau Céleste"),
        .description = COMPOUND_STRING("Normal devient Vol et plus fort."),
        .aiRating = 8,
    },

    [ABILITY_PARENTAL_BOND] =
    {
        .name = _("Amour Filial"),
        .description = COMPOUND_STRING("Attaque en famille."),
        .aiRating = 10,
    },

    [ABILITY_DARK_AURA] =
    {
        .name = _("Aura Ténébreuse"),
        .description = COMPOUND_STRING("Renforce les capacités Ténèbres."),
        .aiRating = 6,
        .breakable = B_UPDATED_ABILITY_DATA < GEN_8,
    },

    [ABILITY_FAIRY_AURA] =
    {
        .name = _("Aura Féérique"),
        .description = COMPOUND_STRING("Renforce toutes les capacités Fée."),
        .aiRating = 6,
        .breakable = B_UPDATED_ABILITY_DATA < GEN_8,
    },

    [ABILITY_AURA_BREAK] =
    {
        .name = _("Aura Inversée"),
        .description = COMPOUND_STRING("L'effet des auras est inversé."),
        .aiRating = 3,
        .breakable = TRUE,
    },

    [ABILITY_PRIMORDIAL_SEA] =
    {
        .name = _("Mer Primaire"),
        .description = COMPOUND_STRING("Déclenche une pluie battante."),
        .aiRating = 10,
    },

    [ABILITY_DESOLATE_LAND] =
    {
        .name = _("Terre Finale"),
        .description = COMPOUND_STRING("Déclenche un soleil écrasant."),
        .aiRating = 10,
    },

    [ABILITY_DELTA_STREAM] =
    {
        .name = _("Souffle Delta"),
        .description = COMPOUND_STRING("Déclenche des vents violents."),
        .aiRating = 10,
    },

    [ABILITY_STAMINA] =
    {
        .name = _("Endurance"),
        .description = COMPOUND_STRING("Touché : Défense +1."),
        .aiRating = 6,
    },

    [ABILITY_WIMP_OUT] =
    {
        .name = _("Escampette"),
        .description = COMPOUND_STRING("Fuit sous 50 % PV."),
        .aiRating = 3,
    },

    [ABILITY_EMERGENCY_EXIT] =
    {
        .name = _("Repli Tactique"),
        .description = COMPOUND_STRING("Fuit sous 50 % PV."),
        .aiRating = 3,
    },

    [ABILITY_WATER_COMPACTION] =
    {
        .name = _("Sable Humide"),
        .description = COMPOUND_STRING("Touché par Eau : Défense +2."),
        .aiRating = 4,
    },

    [ABILITY_MERCILESS] =
    {
        .name = _("Cruauté"),
        .description = COMPOUND_STRING("Critique sur les ennemis empoisonnés."),
        .aiRating = 4,
    },

    [ABILITY_SHIELDS_DOWN] =
    {
        .name = _("Bouclier-Carcan"),
        .description = COMPOUND_STRING("La carapace casse sous 50 % PV."),
        .aiRating = 6,
        .cantBeCopied = TRUE,
        .cantBeSwapped = TRUE,
        .cantBeTraced = TRUE,
        .cantBeSuppressed = TRUE,
        .cantBeOverwritten = TRUE,
        .failsOnImposter = TRUE,
    },

    [ABILITY_STAKEOUT] =
    {
        .name = _("Filature"),
        .description = COMPOUND_STRING("Double dégâts sur qui vient d'entrer."),
        .aiRating = 6,
    },

    [ABILITY_WATER_BUBBLE] =
    {
        .name = _("Aquabulle"),
        .description = COMPOUND_STRING("Protège du Feu et des brûlures."),
        .aiRating = 8,
        .breakable = TRUE,
    },

    [ABILITY_STEELWORKER] =
    {
        .name = _("Expert Acier"),
        .description = COMPOUND_STRING("Renforce les capacités Acier."),
        .aiRating = 6,
    },

    [ABILITY_BERSERK] =
    {
        .name = _("Folle Furie"),
        .description = COMPOUND_STRING("Sous 50 % PV : Atq. Spé. +1."),
        .aiRating = 5,
    },

    [ABILITY_SLUSH_RUSH] =
    {
        .name = _("Chasse-Neige"),
        .description = COMPOUND_STRING("Vitesse doublée sous la neige."),
        .aiRating = 5,
    },

    [ABILITY_LONG_REACH] =
    {
        .name = _("Longue Portée"),
        .description = COMPOUND_STRING("Ses capacités ne font pas contact."),
        .aiRating = 3,
    },

    [ABILITY_LIQUID_VOICE] =
    {
        .name = _("Hydrata-Son"),
        .description = COMPOUND_STRING("Les sons deviennent Eau."),
        .aiRating = 5,
    },

    [ABILITY_TRIAGE] =
    {
        .name = _("Prioguérison"),
        .description = COMPOUND_STRING("Rend les capacités de soin prioritaires."),
        .aiRating = 7,
    },

    [ABILITY_GALVANIZE] =
    {
        .name = _("Peau Électrique"),
        .description = COMPOUND_STRING("Normal devient Électrik et plus fort."),
        .aiRating = 8,
    },

    [ABILITY_SURGE_SURFER] =
    {
        .name = _("Surf Caudal"),
        .description = COMPOUND_STRING("Vitesse doublée sur Champ Électrifié."),
        .aiRating = 4,
    },

    [ABILITY_SCHOOLING] =
    {
        .name = _("Banc"),
        .description = COMPOUND_STRING("Forme un banc à PV hauts."),
        .aiRating = 6,
        .cantBeCopied = TRUE,
        .cantBeSwapped = TRUE,
        .cantBeTraced = TRUE,
        .cantBeSuppressed = TRUE,
        .cantBeOverwritten = TRUE,
        .failsOnImposter = TRUE,
    },

    [ABILITY_DISGUISE] =
    {
        .name = _("Fantômasque"),
        .description = COMPOUND_STRING("Un déguisement encaisse un coup."),
        .aiRating = 8,
        .breakable = TRUE,
        .cantBeCopied = TRUE,
        .cantBeSwapped = TRUE,
        .cantBeTraced = TRUE,
        .cantBeSuppressed = TRUE,
        .cantBeOverwritten = TRUE,
        .failsOnImposter = TRUE,
    },

    [ABILITY_BATTLE_BOND] =
    {
        .name = _("Synergie"),
        .description = COMPOUND_STRING("Change de forme après un K.O."),
        .aiRating = 6,
        .cantBeCopied = TRUE,
        .cantBeSwapped = TRUE,
        .cantBeTraced = TRUE,
        .cantBeSuppressed = TRUE,
        .cantBeOverwritten = TRUE,
        .failsOnImposter = TRUE,
    },

    [ABILITY_POWER_CONSTRUCT] =
    {
        .name = _("Rassemblement"),
        .description = COMPOUND_STRING("Change de forme sous 50 % PV."),
        .aiRating = 10,
        .cantBeCopied = TRUE,
        .cantBeSwapped = TRUE,
        .cantBeTraced = TRUE,
        .cantBeSuppressed = TRUE,
        .cantBeOverwritten = TRUE,
        .failsOnImposter = TRUE,
    },

    [ABILITY_CORROSION] =
    {
        .name = _("Corrosion"),
        .description = COMPOUND_STRING("Peut empoisonner Acier et Poison."),
        .aiRating = 5,
    },

    [ABILITY_COMATOSE] =
    {
        .name = _("Hypersommeil"),
        .description = COMPOUND_STRING("Toujours somnolent, sans statut."),
        .aiRating = 6,
        .cantBeCopied = TRUE,
        .cantBeSwapped = TRUE,
        .cantBeTraced = TRUE,
        .cantBeSuppressed = TRUE,
        .cantBeOverwritten = TRUE,
    },

    [ABILITY_QUEENLY_MAJESTY] =
    {
        .name = _("Prestance Royale"),
        .description = COMPOUND_STRING("Bloque les capacités prioritaires."),
        .aiRating = 6,
        .breakable = TRUE,
    },

    [ABILITY_INNARDS_OUT] =
    {
        .name = _("Expuls'Organes"),
        .description = COMPOUND_STRING("K.O. : inflige ses derniers PV."),
        .aiRating = 5,
    },

    [ABILITY_DANCER] =
    {
        .name = _("Danseuse"),
        .description = COMPOUND_STRING("Copie les danses des autres."),
        .aiRating = 5,
    },

    [ABILITY_BATTERY] =
    {
        .name = _("Batterie"),
        .description = COMPOUND_STRING("Renforce l'Atq. Spé. alliée."),
        .aiRating = 0,
    },

    [ABILITY_FLUFFY] =
    {
        .name = _("Boule de Poils"),
        .description = COMPOUND_STRING("Contact divisé par 2, Feu doublé."),
        .aiRating = 5,
        .breakable = TRUE,
    },

    [ABILITY_DAZZLING] =
    {
        .name = _("Corps Coloré"),
        .description = COMPOUND_STRING("Bloque les capacités prioritaires."),
        .aiRating = 5,
        .breakable = TRUE,
    },

    [ABILITY_SOUL_HEART] =
    {
        .name = _("Animacœur"),
        .description = COMPOUND_STRING("Chaque K.O. : Atq. Spé. +1."),
        .aiRating = 7,
    },

    [ABILITY_TANGLING_HAIR] =
    {
        .name = _("Mèche Rebelle"),
        .description = COMPOUND_STRING("Le contact baisse la Vitesse ennemie."),
        .aiRating = 5,
    },

    [ABILITY_RECEIVER] =
    {
        .name = _("Receveur"),
        .description = COMPOUND_STRING("Copie le talent d'un allié K.O."),
        .aiRating = 0,
        .cantBeCopied = TRUE,
        .cantBeTraced = TRUE,
    },

    [ABILITY_POWER_OF_ALCHEMY] =
    {
        .name = _("Osmose"),
        .description = COMPOUND_STRING("Copie le talent d'un allié K.O."),
        .aiRating = 0,
        .cantBeCopied = TRUE,
        .cantBeTraced = TRUE,
    },

    [ABILITY_BEAST_BOOST] =
    {
        .name = _("Boost Chimère"),
        .description = COMPOUND_STRING("Un K.O. augmente sa meilleure stat."),
        .aiRating = 7,
    },

    [ABILITY_RKS_SYSTEM] =
    {
        .name = _("Système Alpha"),
        .description = COMPOUND_STRING("Type selon la ROM tenue."),
        .aiRating = 8,
        .cantBeCopied = TRUE,
        .cantBeSwapped = TRUE,
        .cantBeTraced = TRUE,
        .cantBeSuppressed = TRUE,
        .cantBeOverwritten = TRUE,
        .failsOnImposter = TRUE,
    },

    [ABILITY_ELECTRIC_SURGE] =
    {
        .name = _("Créa-Élec"),
        .description = COMPOUND_STRING("Crée un Champ Électrifié."),
        .aiRating = 8,
    },

    [ABILITY_PSYCHIC_SURGE] =
    {
        .name = _("Créa-Psy"),
        .description = COMPOUND_STRING("Crée un Champ Psychique."),
        .aiRating = 8,
    },

    [ABILITY_MISTY_SURGE] =
    {
        .name = _("Créa-Brume"),
        .description = COMPOUND_STRING("Crée un Champ Brumeux."),
        .aiRating = 8,
    },

    [ABILITY_GRASSY_SURGE] =
    {
        .name = _("Créa-Herbe"),
        .description = COMPOUND_STRING("Crée un Champ Herbu."),
        .aiRating = 8,
    },

    [ABILITY_FULL_METAL_BODY] =
    {
        .name = _("Métallo-Garde"),
        .description = COMPOUND_STRING("Ses stats ne peuvent pas baisser."),
        .aiRating = 4,
    },

    [ABILITY_SHADOW_SHIELD] =
    {
        .name = _("Spectro-Bouclier"),
        .description = COMPOUND_STRING("Moins de dégâts à PV pleins."),
        .aiRating = 8,
    },

    [ABILITY_PRISM_ARMOR] =
    {
        .name = _("Prisme-Armure"),
        .description = COMPOUND_STRING("Réduit le  super efficace ."),
        .aiRating = 6,
    },

    [ABILITY_NEUROFORCE] =
    {
        .name = _("Cérébro-Force"),
        .description = COMPOUND_STRING("Renforce le  super efficace ."),
        .aiRating = 6,
    },

    [ABILITY_INTREPID_SWORD] =
    {
        .name = _("Lame Indomptable"),
        .description = COMPOUND_STRING("Attaque +1 en entrant (une fois)."),
        .aiRating = 3,
    },

    [ABILITY_DAUNTLESS_SHIELD] =
    {
        .name = _("Égide Inflexible"),
        .description = COMPOUND_STRING("Défense +1 en entrant (une fois)."),
        .aiRating = 3,
    },

    [ABILITY_LIBERO] =
    {
        .name = _("Libéro"),
        .description = COMPOUND_STRING("Prend le type de la capacité lancée."),
    },

    [ABILITY_BALL_FETCH] =
    {
        .name = _("Ramasse Ball"),
        .description = COMPOUND_STRING("Rapporte la 1re Ball ratée."),
        .aiRating = 0,
    },

    [ABILITY_COTTON_DOWN] =
    {
        .name = _("Effilochage"),
        .description = COMPOUND_STRING("Touché : la Vitesse des autres baisse."),
        .aiRating = 3,
    },

    [ABILITY_PROPELLER_TAIL] =
    {
        .name = _("Propulseur"),
        .description = COMPOUND_STRING("Ignore les attire-coups ennemis."),
        .aiRating = 2,
    },

    [ABILITY_MIRROR_ARMOR] =
    {
        .name = _("Armure Miroir"),
        .description = COMPOUND_STRING("Renvoie les baisses de stats."),
        .aiRating = 6,
        .breakable = TRUE,
    },

    [ABILITY_GULP_MISSILE] =
    {
        .name = _("Dégobage"),
        .description = COMPOUND_STRING("Crache une proie quand il est touché."),
        .aiRating = 3,
        .cantBeSwapped = B_UPDATED_ABILITY_DATA < GEN_9,
        .cantBeCopied = B_UPDATED_ABILITY_DATA < GEN_9,
        .cantBeTraced = B_UPDATED_ABILITY_DATA < GEN_9,
        .cantBeSuppressed = TRUE,
        .cantBeOverwritten = TRUE,
        .failsOnImposter = TRUE,
    },

    [ABILITY_STALWART] =
    {
        .name = _("Nerfs d'Acier"),
        .description = COMPOUND_STRING("Ignore les attire-coups ennemis."),
        .aiRating = 2,
    },

    [ABILITY_STEAM_ENGINE] =
    {
        .name = _("Turbine"),
        .description = COMPOUND_STRING("Touché par Feu ou Eau : Vitesse +6."),
        .aiRating = 3,
    },

    [ABILITY_PUNK_ROCK] =
    {
        .name = _("Punk Rock"),
        .description = COMPOUND_STRING("Sons renforcés, sons reçus réduits."),
        .aiRating = 2,
        .breakable = TRUE,
    },

    [ABILITY_SAND_SPIT] =
    {
        .name = _("Expul'Sable"),
        .description = COMPOUND_STRING("Touché : tempête de sable."),
        .aiRating = 5,
    },

    [ABILITY_ICE_SCALES] =
    {
        .name = _("Écailles Glacées"),
        .description = COMPOUND_STRING("Divise par 2 les dégâts spéciaux."),
        .aiRating = 7,
        .breakable = TRUE,
    },

    [ABILITY_RIPEN] =
    {
        .name = _("Mûrissement"),
        .description = COMPOUND_STRING("Double l'effet des Baies."),
        .aiRating = 4,
    },

    [ABILITY_ICE_FACE] =
    {
        .name = _("Tête de Gel"),
        .description = COMPOUND_STRING("Bloque un coup physique."),
        .aiRating = 4,
        .cantBeCopied = TRUE,
        .cantBeSwapped = TRUE,
        .cantBeTraced = TRUE,
        .cantBeSuppressed = TRUE,
        .cantBeOverwritten = TRUE,
        .breakable = TRUE,
        .failsOnImposter = TRUE,
    },

    [ABILITY_POWER_SPOT] =
    {
        .name = _("Cercle d'Énergie"),
        .description = COMPOUND_STRING("Renforce les capacités alliées."),
        .aiRating = 2,
    },

    [ABILITY_MIMICRY] =
    {
        .name = _("Mimétisme"),
        .description = COMPOUND_STRING("Change de type selon le champ."),
        .aiRating = 2,
    },

    [ABILITY_SCREEN_CLEANER] =
    {
        .name = _("Brise-Barrière"),
        .description = COMPOUND_STRING("Retire les murs en entrant."),
        .aiRating = 3,
    },

    [ABILITY_STEELY_SPIRIT] =
    {
        .name = _("Boost Acier"),
        .description = COMPOUND_STRING("Renforce l'Acier de l'équipe."),
        .aiRating = 2,
    },

    [ABILITY_PERISH_BODY] =
    {
        .name = _("Corps Condamné"),
        .description = COMPOUND_STRING("Contact : les deux K.O. dans 3 tours."),
        .aiRating = -1,
    },

    [ABILITY_WANDERING_SPIRIT] =
    {
        .name = _("Âme Vagabonde"),
        .description = COMPOUND_STRING("Échange les talents au contact."),
        .aiRating = 2,
    },

    [ABILITY_GORILLA_TACTICS] =
    {
        .name = _("Entêtement"),
        .description = COMPOUND_STRING("Attaque +50 %, une seule capacité."),
        .aiRating = 4,
    },

    [ABILITY_NEUTRALIZING_GAS] =
    {
        .name = _("Gaz Inhibiteur"),
        .description = COMPOUND_STRING("Annule les talents des autres."),
        .aiRating = 5,
        .cantBeCopied = TRUE,
        .cantBeSwapped = TRUE,
        .cantBeTraced = TRUE,
        .failsOnImposter = TRUE,
    },

    [ABILITY_PASTEL_VEIL] =
    {
        .name = _("Voile Pastel"),
        .description = COMPOUND_STRING("L'équipe ne peut pas être empoisonnée."),
        .aiRating = 4,
        .breakable = TRUE,
    },

    [ABILITY_HUNGER_SWITCH] =
    {
        .name = _("Déclic Fringale"),
        .description = COMPOUND_STRING("Change de forme à chaque tour."),
        .aiRating = 2,
        .cantBeCopied = TRUE,
        .cantBeSwapped = TRUE,
        .cantBeTraced = TRUE,
        .failsOnImposter = TRUE,
    },

    [ABILITY_QUICK_DRAW] =
    {
        .name = _("Tir Vif"),
        .description = COMPOUND_STRING("Agit parfois en premier."),
        .aiRating = 4,
    },

    [ABILITY_UNSEEN_FIST] =
    {
        .name = _("Poing Invisible"),
        .description = COMPOUND_STRING("Le contact passe les protections."),
        .aiRating = 6,
    },

    [ABILITY_CURIOUS_MEDICINE] =
    {
        .name = _("Breuvage Suspect"),
        .description = COMPOUND_STRING("Annule les stats changées des alliés."),
        .aiRating = 3,
    },

    [ABILITY_TRANSISTOR] =
    {
        .name = _("Transistor"),
        .description = COMPOUND_STRING("Renforce les capacités Électrik."),
        .aiRating = 6,
    },

    [ABILITY_DRAGONS_MAW] =
    {
        .name = _("Dent de Dragon"),
        .description = COMPOUND_STRING("Renforce les capacités Dragon."),
        .aiRating = 6,
    },

    [ABILITY_CHILLING_NEIGH] =
    {
        .name = _("Blanche Ruade"),
        .description = COMPOUND_STRING("Un K.O. augmente l'Attaque."),
        .aiRating = 7,
    },

    [ABILITY_GRIM_NEIGH] =
    {
        .name = _("Sombre Ruade"),
        .description = COMPOUND_STRING("Un K.O. augmente l'Atq. Spé."),
        .aiRating = 7,
    },

    [ABILITY_AS_ONE_ICE_RIDER] =
    {
        .name = _("Osmose Équine"),
        .description = COMPOUND_STRING("Tension et Blanche Ruade."),
        .aiRating = 10,
        .cantBeCopied = TRUE,
        .cantBeSwapped = TRUE,
        .cantBeTraced = TRUE,
        .cantBeSuppressed = TRUE,
        .cantBeOverwritten = TRUE,
    },

    [ABILITY_AS_ONE_SHADOW_RIDER] =
    {
        .name = _("Osmose Équine"),
        .description = COMPOUND_STRING("Tension et Sombre Ruade."),
        .aiRating = 10,
        .cantBeCopied = TRUE,
        .cantBeSwapped = TRUE,
        .cantBeTraced = TRUE,
        .cantBeSuppressed = TRUE,
        .cantBeOverwritten = TRUE,
    },

    [ABILITY_LINGERING_AROMA] =
    {
        .name = _("Odeur Tenace"),
        .description = COMPOUND_STRING("Transmet son talent au contact."),
        .aiRating = 5,
    },

    [ABILITY_SEED_SOWER] =
    {
        .name = _("Semencier"),
        .description = COMPOUND_STRING("Touché : crée un Champ Herbu."),
        .aiRating = 5,
    },

    [ABILITY_THERMAL_EXCHANGE] =
    {
        .name = _("Thermodynamique"),
        .description = COMPOUND_STRING("Touché par Feu : Attaque +1."),
        .aiRating = 4,
        .breakable = TRUE,
    },

    [ABILITY_ANGER_SHELL] =
    {
        .name = _("Courroupace"),
        .description = COMPOUND_STRING("Sous 50 % PV : attaques +, défenses -."),
        .aiRating = 3,
    },

    [ABILITY_PURIFYING_SALT] =
    {
        .name = _("Sel Purificateur"),
        .description = COMPOUND_STRING("Pas de statut, Spectre réduit."),
        .aiRating = 6,
        .breakable = TRUE,
    },

    [ABILITY_WELL_BAKED_BODY] =
    {
        .name = _("Bien Cuit"),
        .description = COMPOUND_STRING("Touché par Feu : Défense +2."),
        .aiRating = 5,
        .breakable = TRUE,
    },

    [ABILITY_WIND_RIDER] =
    {
        .name = _("Aéroporté"),
        .description = COMPOUND_STRING("Le vent augmente l'Attaque."),
        .aiRating = 4,
        .breakable = TRUE,
    },

    [ABILITY_GUARD_DOG] =
    {
        .name = _("Chien de Garde"),
        .description = COMPOUND_STRING("Intimidé : Attaque +1."),
        .aiRating = 5,
        .breakable = TRUE,
    },

    [ABILITY_ROCKY_PAYLOAD] =
    {
        .name = _("Porte-Roche"),
        .description = COMPOUND_STRING("Renforce les capacités Roche."),
        .aiRating = 6,
    },

    [ABILITY_WIND_POWER] =
    {
        .name = _("Turbine Éolienne"),
        .description = COMPOUND_STRING("Le vent le charge d'électricité."),
        .aiRating = 4,
    },

    [ABILITY_ZERO_TO_HERO] =
    {
        .name = _("Supermutation"),
        .description = COMPOUND_STRING("Change de forme en sortant."),
        .aiRating = 10,
        .cantBeCopied = TRUE,
        .cantBeSwapped = TRUE,
        .cantBeTraced = TRUE,
        .cantBeSuppressed = TRUE,
        .cantBeOverwritten = TRUE,
        .failsOnImposter = TRUE,
    },

    [ABILITY_COMMANDER] =
    {
        .name = _("Commandant"),
        .description = COMPOUND_STRING("Commande depuis la bouche d'Oyacata."),
        .aiRating = 10,
        .cantBeCopied = TRUE,
        .cantBeSwapped = TRUE,
        .cantBeTraced = TRUE,
    },

    [ABILITY_ELECTROMORPHOSIS] =
    {
        .name = _("Grecharge"),
        .description = COMPOUND_STRING("Touché : se charge d'électricité."),
        .aiRating = 5,
    },

    [ABILITY_PROTOSYNTHESIS] =
    {
        .name = _("Paléosynthèse"),
        .description = COMPOUND_STRING("Le soleil augmente sa meilleure stat."),
        .aiRating = 7,
        .cantBeCopied = TRUE,
        .cantBeSwapped = TRUE,
        .cantBeTraced = TRUE,
        .failsOnImposter = TRUE,
    },

    [ABILITY_QUARK_DRIVE] =
    {
        .name = _("Charge Quantique"),
        .description = COMPOUND_STRING("Champ Électrifié : meilleure stat +."),
        .aiRating = 7,
        .cantBeCopied = TRUE,
        .cantBeSwapped = TRUE,
        .cantBeTraced = TRUE,
        .failsOnImposter = TRUE,
    },

    [ABILITY_GOOD_AS_GOLD] =
    {
        .name = _("Corps en Or"),
        .description = COMPOUND_STRING("Immunisé aux capacités de statut."),
        .aiRating = 8,
        .breakable = TRUE,
    },

    [ABILITY_VESSEL_OF_RUIN] =
    {
        .name = _("Urne du Fléau"),
        .description = COMPOUND_STRING("Baisse l'Atq. Spé. des autres."),
        .aiRating = 5,
        .breakable = TRUE,
    },

    [ABILITY_SWORD_OF_RUIN] =
    {
        .name = _("Épée du Fléau"),
        .description = COMPOUND_STRING("Baisse la Défense des autres."),
        .aiRating = 5,
        .breakable = TRUE,
    },

    [ABILITY_TABLETS_OF_RUIN] =
    {
        .name = _("Bois du Fléau"),
        .description = COMPOUND_STRING("Baisse l'Attaque des autres."),
        .aiRating = 5,
        .breakable = TRUE,
    },

    [ABILITY_BEADS_OF_RUIN] =
    {
        .name = _("Perles du Fléau"),
        .description = COMPOUND_STRING("Baisse la Déf. Spé. des autres."),
        .aiRating = 5,
        .breakable = TRUE,
    },

    [ABILITY_ORICHALCUM_PULSE] =
    {
        .name = _("Pouls Orichalque"),
        .description = COMPOUND_STRING("Grand soleil et Attaque accrue."),
        .aiRating = 8,
    },

    [ABILITY_HADRON_ENGINE] =
    {
        .name = _("Moteur à Hadrons"),
        .description = COMPOUND_STRING("Champ Électrifié et Atq. Spé. accrue."),
        .aiRating = 8,
    },

    [ABILITY_OPPORTUNIST] =
    {
        .name = _("Opportuniste"),
        .description = COMPOUND_STRING("Copie les boosts de stats ennemis."),
        .aiRating = 5,
    },

    [ABILITY_CUD_CHEW] =
    {
        .name = _("Ruminant"),
        .description = COMPOUND_STRING("Remange sa Baie au tour suivant."),
        .aiRating = 4,
    },

    [ABILITY_SHARPNESS] =
    {
        .name = _("Incisif"),
        .description = COMPOUND_STRING("Renforce les capacités tranchantes."),
        .aiRating = 7,
    },

    [ABILITY_SUPREME_OVERLORD] =
    {
        .name = _("Général Suprême"),
        .description = COMPOUND_STRING("Plus fort pour chaque allié K.O."),
        .aiRating = 6,
    },

    [ABILITY_COSTAR] =
    {
        .name = _("Collab"),
        .description = COMPOUND_STRING("Copie les stats de son allié."),
        .aiRating = 5,
    },

    [ABILITY_TOXIC_DEBRIS] =
    {
        .name = _("Dépôt Toxique"),
        .description = COMPOUND_STRING("Touché : pose des Pics Toxik."),
        .aiRating = 4,
    },

    [ABILITY_ARMOR_TAIL] =
    {
        .name = _("Armure Caudale"),
        .description = COMPOUND_STRING("Bloque les capacités prioritaires."),
        .aiRating = 5,
        .breakable = TRUE,
    },

    [ABILITY_EARTH_EATER] =
    {
        .name = _("Absorbe-Terre"),
        .description = COMPOUND_STRING("Le Sol le soigne au lieu de blesser."),
        .aiRating = 7,
        .breakable = TRUE,
    },

    [ABILITY_MYCELIUM_MIGHT] =
    {
        .name = _("Force Fongique"),
        .description = COMPOUND_STRING("Statut en dernier, ignore les talents."),
        .aiRating = 2,
    },

    [ABILITY_HOSPITALITY] =
    {
        .name = _("Aux Petits Soins"),
        .description = COMPOUND_STRING("Soigne un allié en entrant."),
        .aiRating = 5,
    },

    [ABILITY_MINDS_EYE] =
    {
        .name = _("Œil Révélateur"),
        .description = COMPOUND_STRING("Ignore l'Esquive, touche Spectre."),
        .aiRating = 8,
        .breakable = TRUE,
    },

    [ABILITY_EMBODY_ASPECT_TEAL_MASK] =
    {
        .name = _("Force Mémorielle"),
        .description = COMPOUND_STRING("Téracristal : Vitesse +1."),
        .aiRating = 6,
        .cantBeCopied = TRUE,
        .cantBeSwapped = TRUE,
        .cantBeTraced = TRUE,
        .failsOnImposter = TRUE,
    },

    [ABILITY_EMBODY_ASPECT_HEARTHFLAME_MASK] =
    {
        .name = _("Force Mémorielle"),
        .description = COMPOUND_STRING("Téracristal : Attaque +1."),
        .aiRating = 6,
        .cantBeCopied = TRUE,
        .cantBeSwapped = TRUE,
        .cantBeTraced = TRUE,
        .failsOnImposter = TRUE,
    },

    [ABILITY_EMBODY_ASPECT_WELLSPRING_MASK] =
    {
        .name = _("Force Mémorielle"),
        .description = COMPOUND_STRING("Téracristal : Déf. Spé. +1."),
        .aiRating = 6,
        .cantBeCopied = TRUE,
        .cantBeSwapped = TRUE,
        .cantBeTraced = TRUE,
        .failsOnImposter = TRUE,
    },

    [ABILITY_EMBODY_ASPECT_CORNERSTONE_MASK] =
    {
        .name = _("Force Mémorielle"),
        .description = COMPOUND_STRING("Téracristal : Défense +1."),
        .aiRating = 6,
        .cantBeCopied = TRUE,
        .cantBeSwapped = TRUE,
        .cantBeTraced = TRUE,
        .failsOnImposter = TRUE,
    },

    [ABILITY_TOXIC_CHAIN] =
    {
        .name = _("Chaîne Toxique"),
        .description = COMPOUND_STRING("Peut gravement empoisonner."),
        .aiRating = 8,
    },

    [ABILITY_SUPERSWEET_SYRUP] =
    {
        .name = _("Nectar Mielleux"),
        .description = COMPOUND_STRING("Baisse l'Esquive ennemie en entrant."),
        .aiRating = 5,
    },

    [ABILITY_TERA_SHIFT] =
    {
        .name = _("Téramorphose"),
        .description = COMPOUND_STRING("Change de forme en entrant."),
        .aiRating = 10,
        .cantBeCopied = TRUE,
        .cantBeSwapped = TRUE,
        .cantBeTraced = TRUE,
        .cantBeSuppressed = TRUE,
        .cantBeOverwritten = TRUE,
        .failsOnImposter = TRUE,
    },

    [ABILITY_TERA_SHELL] =
    {
        .name = _("Téra-Carapace"),
        .description = COMPOUND_STRING("PV pleins : résiste à tout."),
        .aiRating = 10,
        .cantBeCopied = TRUE,
        .cantBeSwapped = TRUE,
        .cantBeTraced = TRUE,
        .breakable = TRUE,
    },

    [ABILITY_TERAFORM_ZERO] =
    {
        .name = _("Téraformation 0"),
        .description = COMPOUND_STRING("Annule météo et champ."),
        .aiRating = 10,
        .cantBeCopied = TRUE,
        .cantBeSwapped = TRUE,
        .cantBeTraced = TRUE,
    },

    [ABILITY_POISON_PUPPETEER] =
    {
        .name = _("Emprise Toxique"),
        .description = COMPOUND_STRING("Le poison rend aussi confus."),
        .aiRating = 8,
        .cantBeCopied = TRUE,
        .cantBeSwapped = TRUE,
        .cantBeTraced = TRUE,
    },

    [ABILITY_PIERCING_DRILL] =
    {
        .name = _("Transperceuse"),
        .description = COMPOUND_STRING("Le contact passe les protections."),
    },

    [ABILITY_DRAGONIZE] =
    {
        .name = _("Peau Draconique"),
        .description = COMPOUND_STRING("Normal devient Dragon et plus fort."),
    },

    [ABILITY_EELEVATE] =
    {
        .name = _("Eelevate"),
        .description = COMPOUND_STRING("Lévitation et Boost Chimère."),
    },

    [ABILITY_314] =
    {
        .name = _("-------"),
        .description = COMPOUND_STRING("Aucun talent spécial."),
    },

    [ABILITY_MEGA_SOL] =
    {
        .name = _("Méga-Soleil"),
        .description = COMPOUND_STRING("Agit comme sous le soleil."),
    },

    [ABILITY_FIRE_MANE] =
    {
        .name = _("Fire Mane"),
        .description = COMPOUND_STRING("Renforce les capacités Feu."),
    },

    [ABILITY_317] =
    {
        .name = _("-------"),
        .description = COMPOUND_STRING("Aucun talent spécial."),
    },

    [ABILITY_SPICY_SPRAY] =
    {
        .name = _("Habanéruption"),
        .description = COMPOUND_STRING("Brûle qui lui inflige des dégâts."),
    },

    [ABILITY_AURA_GUARD] =
    {
        .name = _("Aura Guard"),
        .description = COMPOUND_STRING("Pas encore disponible."),
    },
};
