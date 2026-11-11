const struct AbilityInfo gAbilitiesInfo[ABILITIES_COUNT] =
{
    [ABILITY_NONE] =
    {
        .name = _("-------"),
        .description = COMPOUND_STRING("No Ability."),
        .aiRating = 0,
        .cantBeSwapped = TRUE,
        .cantBeTraced = TRUE,
    },

    [ABILITY_STENCH] =
    {
        .name = _("Puanteur"),
        .description = COMPOUND_STRING("Moves may cause flinching."),
        .aiRating = 1,
    },

    [ABILITY_DRIZZLE] =
    {
        .name = _("Crachin"),
        .description = COMPOUND_STRING("Summons rain in battle."),
        .aiRating = 9,
    },

    [ABILITY_SPEED_BOOST] =
    {
        .name = _("Turbo"),
        .description = COMPOUND_STRING("Speed raises every turn."),
        .aiRating = 9,
    },

    [ABILITY_BATTLE_ARMOR] =
    {
        .name = _("Armurbaston"),
        .description = COMPOUND_STRING("Blocks critical hits."),
        .aiRating = 2,
        .breakable = TRUE,
    },

    [ABILITY_STURDY] =
    {
        .name = _("Fermeté"),
        .description = COMPOUND_STRING("Negates 1-hit KO attacks."),
        .aiRating = 6,
        .breakable = TRUE,
    },

    [ABILITY_DAMP] =
    {
        .name = _("Moiteur"),
        .description = COMPOUND_STRING("Prevents self-destruction."),
        .aiRating = 2,
        .breakable = TRUE,
    },

    [ABILITY_LIMBER] =
    {
        .name = _("Échauffement"),
        .description = COMPOUND_STRING("Prevents paralysis."),
        .aiRating = 3,
        .breakable = TRUE,
    },

    [ABILITY_SAND_VEIL] =
    {
        .name = _("Voile Sable"),
        .description = COMPOUND_STRING("Ups evasion in a sandstorm."),
        .aiRating = 3,
        .breakable = TRUE,
    },

    [ABILITY_STATIC] =
    {
        .name = _("Statik"),
        .description = COMPOUND_STRING("May paralyze on contact."),
        .aiRating = 4,
    },

    [ABILITY_VOLT_ABSORB] =
    {
        .name = _("Absorbe-Volt"),
        .description = COMPOUND_STRING("Restores if hit by Electric."),
        .aiRating = 7,
        .breakable = TRUE,
    },

    [ABILITY_WATER_ABSORB] =
    {
        .name = _("Absorbe-Eau"),
        .description = COMPOUND_STRING("Restores HP if hit by Water."),
        .aiRating = 7,
        .breakable = TRUE,
    },

    [ABILITY_OBLIVIOUS] =
    {
        .name = _("Benêt"),
        .description = COMPOUND_STRING(
        #if B_OBLIVIOUS_TAUNT >= GEN_6
            "Blocks Attract and Taunt."),
        #else
            "Prevents attraction."),
        #endif
        .aiRating = 2,
        .breakable = TRUE,
    },

    [ABILITY_CLOUD_NINE] =
    {
        .name = _("Ciel Gris"),
        .description = COMPOUND_STRING("Negates weather effects."),
        .aiRating = 5,
    },

    [ABILITY_COMPOUND_EYES] =
    {
        .name = _("Œil Composé"),
        .description = COMPOUND_STRING("Boosts accuracy."),
        .aiRating = 7,
    },

    [ABILITY_INSOMNIA] =
    {
        .name = _("Insomnia"),
        .description = COMPOUND_STRING("Prevents sleep."),
        .aiRating = 4,
        .breakable = TRUE,
    },

    [ABILITY_COLOR_CHANGE] =
    {
        .name = _("Homochromie"),
        .description = COMPOUND_STRING("Changes type to foe's move."),
        .aiRating = 2,
    },

    [ABILITY_IMMUNITY] =
    {
        .name = _("Vaccin"),
        .description = COMPOUND_STRING("Prevents poisoning."),
        .aiRating = 4,
        .breakable = TRUE,
    },

    [ABILITY_FLASH_FIRE] =
    {
        .name = _("Torche"),
        .description = COMPOUND_STRING("Powers up Fire if hit by it."),
        .aiRating = 6,
        .breakable = TRUE,
    },

    [ABILITY_SHIELD_DUST] =
    {
        .name = _("Écran Poudre"),
        .description = COMPOUND_STRING("Blocks foes' added effects."),
        .aiRating = 5,
        .breakable = TRUE,
    },

    [ABILITY_OWN_TEMPO] =
    {
        .name = _("Tempo Perso"),
        .description = COMPOUND_STRING("Prevents confusion."),
        .aiRating = 3,
        .breakable = TRUE,
    },

    [ABILITY_SUCTION_CUPS] =
    {
        .name = _("Ventouse"),
        .description = COMPOUND_STRING("Can't be forced to switch."),
        .aiRating = 2,
        .breakable = TRUE,
    },

    [ABILITY_INTIMIDATE] =
    {
        .name = _("Intimidation"),
        .description = COMPOUND_STRING("Lowers foes' Atk on entry."),
        .aiRating = 7,
    },

    [ABILITY_SHADOW_TAG] =
    {
        .name = _("Marque Ombre"),
        .description = COMPOUND_STRING("Prevents the foes' escape."),
        .aiRating = 10,
    },

    [ABILITY_ROUGH_SKIN] =
    {
        .name = _("Peau Dure"),
        .description = COMPOUND_STRING("Damages foe on contact."),
        .aiRating = 6,
    },

    [ABILITY_WONDER_GUARD] =
    {
        .name = _("Garde Mystik"),
        .description = COMPOUND_STRING("Only “supereffective” hits."),
        .aiRating = 10,
        .cantBeCopied = TRUE,
        .cantBeSwapped = TRUE,
        .breakable = TRUE,
    },

    [ABILITY_LEVITATE] =
    {
        .name = _("Lévitation"),
        .description = COMPOUND_STRING("Not hit by Ground attacks."),
        .aiRating = 7,
        .breakable = TRUE,
    },

    [ABILITY_EFFECT_SPORE] =
    {
        .name = _("Pose Spore"),
        .description = COMPOUND_STRING("Contact may psn/slp/par."),
        .aiRating = 4,
    },

    [ABILITY_SYNCHRONIZE] =
    {
        .name = _("Synchro"),
        .description = COMPOUND_STRING("Passes on brn/par/psn."),
        .aiRating = 4,
    },

    [ABILITY_CLEAR_BODY] =
    {
        .name = _("Corps Sain"),
        .description = COMPOUND_STRING("Stats can't be lowered."),
        .aiRating = 4,
        .breakable = TRUE,
    },

    [ABILITY_NATURAL_CURE] =
    {
        .name = _("Médic Nature"),
        .description = COMPOUND_STRING("Heals status upon switch."),
        .aiRating = 7,
    },

    [ABILITY_LIGHTNING_ROD] =
    {
        .name = _("Paratonnerre"),
        .description = COMPOUND_STRING(
        #if B_REDIRECT_ABILITY_IMMUNITY >= GEN_4
            "Draws Electr. to up Sp. Atk."),
        #else
            "Draws Electric moves."),
        #endif
        .aiRating = 7,
        .breakable = TRUE,
    },

    [ABILITY_SERENE_GRACE] =
    {
        .name = _("Sérénité"),
        .description = COMPOUND_STRING("Promotes added effects."),
        .aiRating = 8,
    },

    [ABILITY_SWIFT_SWIM] =
    {
        .name = _("Glissade"),
        .description = COMPOUND_STRING("Boosts Speed in rain."),
        .aiRating = 6,
    },

    [ABILITY_CHLOROPHYLL] =
    {
        .name = _("Chlorophylle"),
        .description = COMPOUND_STRING("Boosts Speed in sunlight."),
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
        .description = COMPOUND_STRING("Copies a foe's Ability."),
        .aiRating = 6,
        .cantBeCopied = TRUE,
        .cantBeTraced = TRUE, //B_UPDATED_ABILITY_DATA >= GEN_4
    },

    [ABILITY_HUGE_POWER] =
    {
        .name = _("Coloforce"),
        .description = COMPOUND_STRING("Doubles Attack."),
        .aiRating = 10,
    },

    [ABILITY_POISON_POINT] =
    {
        .name = _("Point Poison"),
        .description = COMPOUND_STRING("May poison foe on contact."),
        .aiRating = 4,
    },

    [ABILITY_INNER_FOCUS] =
    {
        .name = _("Attention"),
        .description = COMPOUND_STRING("Prevents flinching."),
        .aiRating = 2,
        .breakable = TRUE,
    },

    [ABILITY_MAGMA_ARMOR] =
    {
        .name = _("Armumagma"),
        .description = COMPOUND_STRING(
        #if B_USE_FROSTBITE
            "Prevents frostbite."),
        #else
            "Prevents freezing."),
        #endif
        .aiRating = 1,
        .breakable = TRUE,
    },

    [ABILITY_WATER_VEIL] =
    {
        .name = _("Ignifu-Voile"),
        .description = COMPOUND_STRING("Prevents burns."),
        .aiRating = 4,
        .breakable = TRUE,
    },

    [ABILITY_MAGNET_PULL] =
    {
        .name = _("Magnépiège"),
        .description = COMPOUND_STRING("Traps Steel-type foes."),
        .aiRating = 9,
    },

    [ABILITY_SOUNDPROOF] =
    {
        .name = _("Anti-Bruit"),
        .description = COMPOUND_STRING("Avoids sound-based moves."),
        .aiRating = 4,
        .breakable = TRUE,
    },

    [ABILITY_RAIN_DISH] =
    {
        .name = _("Cuvette"),
        .description = COMPOUND_STRING("Slight HP recovery in rain."),
        .aiRating = 3,
    },

    [ABILITY_SAND_STREAM] =
    {
        .name = _("Sable Volant"),
        .description = COMPOUND_STRING("Summons a sandstorm."),
        .aiRating = 9,
    },

    [ABILITY_PRESSURE] =
    {
        .name = _("Pression"),
        .description = COMPOUND_STRING("Raises foe's PP usage."),
        .aiRating = 5,
    },

    [ABILITY_THICK_FAT] =
    {
        .name = _("Isograisse"),
        .description = COMPOUND_STRING("Halves Fire and Ice damage."),
        .aiRating = 7,
        .breakable = TRUE,
    },

    [ABILITY_EARLY_BIRD] =
    {
        .name = _("Matinal"),
        .description = COMPOUND_STRING("Awakens twice as fast."),
        .aiRating = 4,
    },

    [ABILITY_FLAME_BODY] =
    {
        .name = _("Corps Ardent"),
        .description = COMPOUND_STRING("May burn foe on contact."),
        .aiRating = 4,
    },

    [ABILITY_RUN_AWAY] =
    {
        .name = _("Fuite"),
        .description = COMPOUND_STRING("Enables a sure getaway."),
        .aiRating = 0,
    },

    [ABILITY_KEEN_EYE] =
    {
        .name = _("Regard Vif"),
        .description = COMPOUND_STRING("Accuracy can't be lowered."),
        .aiRating = 1,
        .breakable = TRUE,
    },

    [ABILITY_HYPER_CUTTER] =
    {
        .name = _("Hyper Cutter"),
        .description = COMPOUND_STRING("Attack can't be lowered."),
        .aiRating = 3,
        .breakable = TRUE,
    },

    [ABILITY_PICKUP] =
    {
        .name = _("Ramassage"),
        .description = COMPOUND_STRING("May pick up items."),
        .aiRating = 1,
    },

    [ABILITY_TRUANT] =
    {
        .name = _("Absentéisme"),
        .description = COMPOUND_STRING("Moves only every two turns."),
        .aiRating = -2,
        .cantBeOverwritten = TRUE,
    },

    [ABILITY_HUSTLE] =
    {
        .name = _("Agitation"),
        .description = COMPOUND_STRING("Trades accuracy for power."),
        .aiRating = 7,
    },

    [ABILITY_CUTE_CHARM] =
    {
        .name = _("Joli Sourire"),
        .description = COMPOUND_STRING("May infatuate on contact."),
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
        .description = COMPOUND_STRING("Changes with the weather."),
        .aiRating = 6,
        .cantBeCopied = TRUE,
        .cantBeTraced = B_UPDATED_ABILITY_DATA >= GEN_4,
        .failsOnImposter = B_UPDATED_ABILITY_DATA >= GEN_5,
    },

    [ABILITY_STICKY_HOLD] =
    {
        .name = _("Glu"),
        .description = COMPOUND_STRING("Held item can't be removed."),
        .aiRating = 3,
        .breakable = TRUE,
    },

    [ABILITY_SHED_SKIN] =
    {
        .name = _("Mue"),
        .description = COMPOUND_STRING("May heal status problems."),
        .aiRating = 7,
    },

    [ABILITY_GUTS] =
    {
        .name = _("Cran"),
        .description = COMPOUND_STRING("Boosts Attack if statused."),
        .aiRating = 6,
    },

    [ABILITY_MARVEL_SCALE] =
    {
        .name = _("Écaille Spéciale"),
        .description = COMPOUND_STRING("Ups Defense if statused."),
        .aiRating = 5,
        .breakable = TRUE,
    },

    [ABILITY_LIQUID_OOZE] =
    {
        .name = _("Suintement"),
        .description = COMPOUND_STRING("Damages HP-draining foes."),
        .aiRating = 3,
    },

    [ABILITY_OVERGROW] =
    {
        .name = _("Engrais"),
        .description = COMPOUND_STRING("Ups Grass moves at low HP."),
        .aiRating = 5,
    },

    [ABILITY_BLAZE] =
    {
        .name = _("Brasier"),
        .description = COMPOUND_STRING("Ups Fire moves at low HP."),
        .aiRating = 5,
    },

    [ABILITY_TORRENT] =
    {
        .name = _("Torrent"),
        .description = COMPOUND_STRING("Ups Water moves at low HP."),
        .aiRating = 5,
    },

    [ABILITY_SWARM] =
    {
        .name = _("Essaim"),
        .description = COMPOUND_STRING("Ups Bug moves at low HP."),
        .aiRating = 5,
    },

    [ABILITY_ROCK_HEAD] =
    {
        .name = _("Tête de Roc"),
        .description = COMPOUND_STRING("Prevents recoil damage."),
        .aiRating = 5,
    },

    [ABILITY_DROUGHT] =
    {
        .name = _("Sécheresse"),
        .description = COMPOUND_STRING("Summons sunlight in battle."),
        .aiRating = 9,
    },

    [ABILITY_ARENA_TRAP] =
    {
        .name = _("Piège Sable"),
        .description = COMPOUND_STRING("Prevents foes from fleeing."),
        .aiRating = 9,
    },

    [ABILITY_VITAL_SPIRIT] =
    {
        .name = _("Esprit Vital"),
        .description = COMPOUND_STRING("Prevents sleep."),
        .aiRating = 4,
        .breakable = TRUE,
    },

    [ABILITY_WHITE_SMOKE] =
    {
        .name = _("Écran Fumée"),
        .description = COMPOUND_STRING("Stats can't be lowered."),
        .aiRating = 4,
        .breakable = TRUE,
    },

    [ABILITY_PURE_POWER] =
    {
        .name = _("Force Pure"),
        .description = COMPOUND_STRING("Doubles Attack."),
        .aiRating = 10,
    },

    [ABILITY_SHELL_ARMOR] =
    {
        .name = _("Coque Armure"),
        .description = COMPOUND_STRING("Blocks critical hits."),
        .aiRating = 2,
        .breakable = TRUE,
    },

    [ABILITY_AIR_LOCK] =
    {
        .name = _("Air Lock"),
        .description = COMPOUND_STRING("Negates weather effects."),
        .aiRating = 5,
    },

    [ABILITY_TANGLED_FEET] =
    {
        .name = _("Pieds Confus"),
        .description = COMPOUND_STRING("Boosts evasion if confused."),
        .aiRating = 2,
        .breakable = TRUE,
    },

    [ABILITY_MOTOR_DRIVE] =
    {
        .name = _("Motorisé"),
        .description = COMPOUND_STRING("Ups Speed if hit by Electr."),
        .aiRating = 6,
        .breakable = TRUE,
    },

    [ABILITY_RIVALRY] =
    {
        .name = _("Rivalité"),
        .description = COMPOUND_STRING("Strong vs. opposite gender."),
        .aiRating = 1,
    },

    [ABILITY_STEADFAST] =
    {
        .name = _("Impassible"),
        .description = COMPOUND_STRING("Flinching raises Speed."),
        .aiRating = 2,
    },

    [ABILITY_SNOW_CLOAK] =
    {
        .name = _("Rideau Neige"),
        .description = COMPOUND_STRING(
        #if B_PREFERRED_ICE_WEATHER == B_ICE_WEATHER_HAIL
            "Boosts evasion in hail."),
        #elif B_PREFERRED_ICE_WEATHER == B_ICE_WEATHER_SNOW
            "Boosts evasion in snow."),
        #else
            "Ups evasion in hail or snow."),
        #endif
        .aiRating = 3,
        .breakable = TRUE,
    },

    [ABILITY_GLUTTONY] =
    {
        .name = _("Gloutonnerie"),
        .description = COMPOUND_STRING("Eats Berries at half HP."),
        .aiRating = 3,
    },

    [ABILITY_ANGER_POINT] =
    {
        .name = _("Colérique"),
        .description = COMPOUND_STRING("Maxes Attack when crit."),
        .aiRating = 4,
    },

    [ABILITY_UNBURDEN] =
    {
        .name = _("Délestage"),
        .description = COMPOUND_STRING("Item loss boosts Speed."),
        .aiRating = 7,
    },

    [ABILITY_HEATPROOF] =
    {
        .name = _("Ignifugé"),
        .description = COMPOUND_STRING("Halves Fire-type damage."),
        .aiRating = 5,
        .breakable = TRUE,
    },

    [ABILITY_SIMPLE] =
    {
        .name = _("Simple"),
        .description = COMPOUND_STRING("Doubles stat changes."),
        .aiRating = 8,
        .breakable = TRUE,
    },

    [ABILITY_DRY_SKIN] =
    {
        .name = _("Peau Sèche"),
        .description = COMPOUND_STRING("Heat hurts, Water restores."),
        .aiRating = 6,
        .breakable = TRUE,
    },

    [ABILITY_DOWNLOAD] =
    {
        .name = _("Télécharge"),
        .description = COMPOUND_STRING("Ups offense based on foe."),
        .aiRating = 7,
    },

    [ABILITY_IRON_FIST] =
    {
        .name = _("Poing de Fer"),
        .description = COMPOUND_STRING("Powers up punching moves."),
        .aiRating = 6,
    },

    [ABILITY_POISON_HEAL] =
    {
        .name = _("Soin Poison"),
        .description = COMPOUND_STRING("Restores HP if poisoned."),
        .aiRating = 8,
    },

    [ABILITY_ADAPTABILITY] =
    {
        .name = _("Adaptabilité"),
        .description = COMPOUND_STRING("Powers up same-type moves."),
        .aiRating = 8,
    },

    [ABILITY_SKILL_LINK] =
    {
        .name = _("Multi-Coups"),
        .description = COMPOUND_STRING("Maxes multistrike hits."),
        .aiRating = 7,
    },

    [ABILITY_HYDRATION] =
    {
        .name = _("Hydratation"),
        .description = COMPOUND_STRING("Cures status in rain."),
        .aiRating = 4,
    },

    [ABILITY_SOLAR_POWER] =
    {
        .name = _("Force Soleil"),
        .description = COMPOUND_STRING("Sunlight boosts, but hurts."),
        .aiRating = 3,
    },

    [ABILITY_QUICK_FEET] =
    {
        .name = _("Pied Véloce"),
        .description = COMPOUND_STRING("Boosts Speed if statused."),
        .aiRating = 5,
    },

    [ABILITY_NORMALIZE] =
    {
        .name = _("Normalise"),
        .description = COMPOUND_STRING("Moves become Normal-type."),
        .aiRating = -1,
    },

    [ABILITY_SNIPER] =
    {
        .name = _("Sniper"),
        .description = COMPOUND_STRING("Powers up critical hits."),
        .aiRating = 3,
    },

    [ABILITY_MAGIC_GUARD] =
    {
        .name = _("Garde Magik"),
        .description = COMPOUND_STRING("Only damaged by attacks."),
        .aiRating = 9,
    },

    [ABILITY_NO_GUARD] =
    {
        .name = _("Annule Garde"),
        .description = COMPOUND_STRING("Ensures all attacks land."),
        .aiRating = 8,
    },

    [ABILITY_STALL] =
    {
        .name = _("Frein"),
        .description = COMPOUND_STRING("Always last to use moves."),
        .aiRating = -1,
    },

    [ABILITY_TECHNICIAN] =
    {
        .name = _("Technicien"),
        .description = COMPOUND_STRING("Powers up weak moves."),
        .aiRating = 8,
    },

    [ABILITY_LEAF_GUARD] =
    {
        .name = _("Feuille Garde"),
        .description = COMPOUND_STRING("Blocks status in sunlight."),
        .aiRating = 2,
        .breakable = TRUE,
    },

    [ABILITY_KLUTZ] =
    {
        .name = _("Maladresse"),
        .description = COMPOUND_STRING("Can't use held items."),
        .aiRating = -1,
    },

    [ABILITY_MOLD_BREAKER] =
    {
        .name = _("Brise Moule"),
        .description = COMPOUND_STRING("Moves ignore Abilities."),
        .aiRating = 7,
    },

    [ABILITY_SUPER_LUCK] =
    {
        .name = _("Chanceux"),
        .description = COMPOUND_STRING("Raises critical-hit ratio."),
        .aiRating = 3,
    },

    [ABILITY_AFTERMATH] =
    {
        .name = _("Boom Final"),
        .description = COMPOUND_STRING("Fainting damages the foe."),
        .aiRating = 5,
    },

    [ABILITY_ANTICIPATION] =
    {
        .name = _("Anticipation"),
        .description = COMPOUND_STRING("Senses dangerous moves."),
        .aiRating = 2,
    },

    [ABILITY_FOREWARN] =
    {
        .name = _("Prédiction"),
        .description = COMPOUND_STRING("Determines a foe's move."),
        .aiRating = 2,
    },

    [ABILITY_UNAWARE] =
    {
        .name = _("Inconscient"),
        .description = COMPOUND_STRING("Ignores foes' stat changes."),
        .aiRating = 6,
        .breakable = TRUE,
    },

    [ABILITY_TINTED_LENS] =
    {
        .name = _("Lentiteintée"),
        .description = COMPOUND_STRING("Ups “not very effective”."),
        .aiRating = 7,
    },

    [ABILITY_FILTER] =
    {
        .name = _("Filtre"),
        .description = COMPOUND_STRING("Weakens “supereffective”."),
        .aiRating = 6,
        .breakable = TRUE,
    },

    [ABILITY_SLOW_START] =
    {
        .name = _("Début Calme"),
        .description = COMPOUND_STRING("Takes 5 turns to get going."),
        .aiRating = -2,
    },

    [ABILITY_SCRAPPY] =
    {
        .name = _("Querelleur"),
        .description = COMPOUND_STRING("Normal & Fight. hit Ghosts."),
        .aiRating = 6,
    },

    [ABILITY_STORM_DRAIN] =
    {
        .name = _("Lavabo"),
        .description = COMPOUND_STRING(
        #if B_REDIRECT_ABILITY_IMMUNITY >= GEN_4
            "Draws Water to up Sp. Atk."),
        #else
            "Draws Water-type moves."),
        #endif
        .aiRating = 7,
        .breakable = TRUE,
    },

    [ABILITY_ICE_BODY] =
    {
        .name = _("Corps Gel"),
        .description = COMPOUND_STRING(
        #if B_PREFERRED_ICE_WEATHER == B_ICE_WEATHER_HAIL
            "Slight HP recovery in hail."),
        #elif B_PREFERRED_ICE_WEATHER == B_ICE_WEATHER_SNOW
            "Slight HP recovery in snow."),
        #else
            "HP recovery in hail or snow."),
        #endif
        .aiRating = 3,
    },

    [ABILITY_SOLID_ROCK] =
    {
        .name = _("Solide Roc"),
        .description = COMPOUND_STRING("Weakens “supereffective”."),
        .aiRating = 6,
        .breakable = TRUE,
    },

    [ABILITY_SNOW_WARNING] =
    {
        .name = _("Alerte Neige"),
    #if B_SNOW_WARNING >= GEN_9
        .description = COMPOUND_STRING("Summons snow in battle."),
    #else
        .description = COMPOUND_STRING("Summons hail in battle."),
    #endif
        .aiRating = 8,
    },

    [ABILITY_HONEY_GATHER] =
    {
        .name = _("Cherche Miel"),
        .description = COMPOUND_STRING("May gather Honey."),
        .aiRating = 0,
    },

    [ABILITY_FRISK] =
    {
        .name = _("Fouille"),
        .description = COMPOUND_STRING("Checks a foe's held item."),
        .aiRating = 3,
    },

    [ABILITY_RECKLESS] =
    {
        .name = _("Téméraire"),
        .description = COMPOUND_STRING("Powers up recoil moves."),
        .aiRating = 6,
    },

    [ABILITY_MULTITYPE] =
    {
        .name = _("Multi-Type"),
        .description = COMPOUND_STRING("Matches Plate or Z-Crystal."),
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
        .description = COMPOUND_STRING("Sun ups allies' Atk & Sp.Def."),
        .aiRating = 4,
        .cantBeCopied = TRUE,
        .cantBeTraced = B_UPDATED_ABILITY_DATA >= GEN_5,
        .breakable = TRUE,
    },

    [ABILITY_BAD_DREAMS] =
    {
        .name = _("Mauvais Rêve"),
        .description = COMPOUND_STRING("Damages sleeping foes."),
        .aiRating = 4,
    },

    [ABILITY_PICKPOCKET] =
    {
        .name = _("Pickpocket"),
        .description = COMPOUND_STRING("Steals foe's item if hit."),
        .aiRating = 3,
    },

    [ABILITY_SHEER_FORCE] =
    {
        .name = _("Sans Limite"),
        .description = COMPOUND_STRING("Trades effects for power."),
        .aiRating = 8,
    },

    [ABILITY_CONTRARY] =
    {
        .name = _("Contestation"),
        .description = COMPOUND_STRING("Reverses stat changes."),
        .aiRating = 8,
        .breakable = TRUE,
    },

    [ABILITY_UNNERVE] =
    {
        .name = _("Tension"),
        .description = COMPOUND_STRING("Foes can't eat Berries."),
        .aiRating = 3,
    },

    [ABILITY_DEFIANT] =
    {
        .name = _("Acharné"),
        .description = COMPOUND_STRING("Stat-lowering ups Attack."),
        .aiRating = 5,
    },

    [ABILITY_DEFEATIST] =
    {
        .name = _("Défaitiste"),
        .description = COMPOUND_STRING("Halves offenses at half HP."),
        .aiRating = -1,
    },

    [ABILITY_CURSED_BODY] =
    {
        .name = _("Corps Maudit"),
        .description = COMPOUND_STRING("Contact may disable move."),
        .aiRating = 4,
    },

    [ABILITY_HEALER] =
    {
        .name = _("Cœur Soin"),
        .description = COMPOUND_STRING("May cure ally's status."),
        .aiRating = 0,
    },

    [ABILITY_FRIEND_GUARD] =
    {
        .name = _("Garde-Ami"),
        .description = COMPOUND_STRING("Reduces damage to allies."),
        .aiRating = 0,
        .breakable = TRUE,
    },

    [ABILITY_WEAK_ARMOR] =
    {
        .name = _("Armurouillée"),
        .description = COMPOUND_STRING("Hits change Def and Speed."),
        .aiRating = 2,
    },

    [ABILITY_HEAVY_METAL] =
    {
        .name = _("Heavy Metal"),
        .description = COMPOUND_STRING("Doubles weight."),
        .aiRating = -1,
        .breakable = TRUE,
    },

    [ABILITY_LIGHT_METAL] =
    {
        .name = _("Light Metal"),
        .description = COMPOUND_STRING("Halves weight."),
        .aiRating = 2,
        .breakable = TRUE,
    },

    [ABILITY_MULTISCALE] =
    {
        .name = _("Multiécaille"),
        .description = COMPOUND_STRING("Reduces damage at full HP."),
        .aiRating = 8,
        .breakable = TRUE,
    },

    [ABILITY_TOXIC_BOOST] =
    {
        .name = _("Rage Poison"),
        .description = COMPOUND_STRING("Boosts Attack if poisoned."),
        .aiRating = 6,
    },

    [ABILITY_FLARE_BOOST] =
    {
        .name = _("Rage Brûlure"),
        .description = COMPOUND_STRING("Boosts Sp. Atk if burned."),
        .aiRating = 5,
    },

    [ABILITY_HARVEST] =
    {
        .name = _("Récolte"),
        .description = COMPOUND_STRING("May create another Berry."),
        .aiRating = 5,
    },

    [ABILITY_TELEPATHY] =
    {
        .name = _("Télépathe"),
        .description = COMPOUND_STRING("Dodges ally's attacks."),
        .aiRating = 0,
        .breakable = TRUE,
    },

    [ABILITY_MOODY] =
    {
        .name = _("Lunatique"),
        .description = COMPOUND_STRING("Stats change every turn."),
        .aiRating = 10,
    },

    [ABILITY_OVERCOAT] =
    {
        .name = _("Envelocape"),
        .description = COMPOUND_STRING(
        #if B_POWDER_OVERCOAT >= GEN_6
            "Blocks weather and powder."),
        #else
            "Prevents weather damage."),
        #endif
        .aiRating = 5,
        .breakable = TRUE,
    },

    [ABILITY_POISON_TOUCH] =
    {
        .name = _("Toxitouche"),
        .description = COMPOUND_STRING("Contact moves may poison."),
        .aiRating = 4,
    },

    [ABILITY_REGENERATOR] =
    {
        .name = _("Régé-Force"),
        .description = COMPOUND_STRING("Restores HP upon switch."),
        .aiRating = 8,
    },

    [ABILITY_BIG_PECKS] =
    {
        .name = _("Cœur de Coq"),
        .description = COMPOUND_STRING("Defense can't be lowered."),
        .aiRating = 1,
        .breakable = TRUE,
    },

    [ABILITY_SAND_RUSH] =
    {
        .name = _("Baigne Sable"),
        .description = COMPOUND_STRING("Ups Speed in a sandstorm."),
        .aiRating = 6,
    },

    [ABILITY_WONDER_SKIN] =
    {
        .name = _("Peau Miracle"),
        .description = COMPOUND_STRING("May avoid status moves."),
        .aiRating = 4,
        .breakable = TRUE,
    },

    [ABILITY_ANALYTIC] =
    {
        .name = _("Analyste"),
        .description = COMPOUND_STRING("Boosts power if last to act."),
        .aiRating = 5,
    },

    [ABILITY_ILLUSION] =
    {
        .name = _("Illusion"),
        .description = COMPOUND_STRING("Disguises as a partner."),
        .aiRating = 8,
        .cantBeCopied = TRUE,
        .cantBeSwapped = TRUE,
        .cantBeTraced = TRUE,
    },

    [ABILITY_IMPOSTER] =
    {
        .name = _("Imposteur"),
        .description = COMPOUND_STRING("Transforms into the foe."),
        .aiRating = 9,
        .cantBeCopied = TRUE,
        .cantBeTraced = TRUE,
    },

    [ABILITY_INFILTRATOR] =
    {
        .name = _("Infiltration"),
        .description = COMPOUND_STRING("Passes through barriers."),
        .aiRating = 6,
    },

    [ABILITY_MUMMY] =
    {
        .name = _("Momie"),
        .description = COMPOUND_STRING("Ability spreads on contact."),
        .aiRating = 5,
    },

    [ABILITY_MOXIE] =
    {
        .name = _("Impudence"),
        .description = COMPOUND_STRING("Raises Attack after KOs."),
        .aiRating = 7,
    },

    [ABILITY_JUSTIFIED] =
    {
        .name = _("Cœur Noble"),
        .description = COMPOUND_STRING("Ups Attack if hit by Dark."),
        .aiRating = 4,
    },

    [ABILITY_RATTLED] =
    {
        .name = _("Phobique"),
        .description = COMPOUND_STRING("Raises Speed when scared."),
        .aiRating = 3,
    },

    [ABILITY_MAGIC_BOUNCE] =
    {
        .name = _("Miroir Magik"),
        .description = COMPOUND_STRING("Reflects status moves."),
        .aiRating = 9,
        .breakable = TRUE,
    },

    [ABILITY_SAP_SIPPER] =
    {
        .name = _("Herbivore"),
        .description = COMPOUND_STRING("Ups Attack if hit by Grass."),
        .aiRating = 7,
        .breakable = TRUE,
    },

    [ABILITY_PRANKSTER] =
    {
        .name = _("Farceur"),
        .description = COMPOUND_STRING("Status moves have priority."),
        .aiRating = 8,
    },

    [ABILITY_SAND_FORCE] =
    {
        .name = _("Force Sable"),
        .description = COMPOUND_STRING("Powers up in a sandstorm."),
        .aiRating = 4,
    },

    [ABILITY_IRON_BARBS] =
    {
        .name = _("Épine de Fer"),
        .description = COMPOUND_STRING("Damages foe on contact."),
        .aiRating = 6,
    },

    [ABILITY_ZEN_MODE] =
    {
        .name = _("Mode Transe"),
        .description = COMPOUND_STRING("Changes shape at half HP."),
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
        .description = COMPOUND_STRING("Boosts allies' accuracy."),
        .aiRating = 6,
    },

    [ABILITY_TURBOBLAZE] =
    {
        .name = _("Turbo Brasier"),
        .description = COMPOUND_STRING("Moves ignore Abilities."),
        .aiRating = 7,
    },

    [ABILITY_TERAVOLT] =
    {
        .name = _("Téra-Voltage"),
        .description = COMPOUND_STRING("Moves ignore Abilities."),
        .aiRating = 7,
    },

    [ABILITY_AROMA_VEIL] =
    {
        .name = _("Aroma-Voile"),
        .description = COMPOUND_STRING("Allies can't be move-bound."),
        .aiRating = 3,
        .breakable = TRUE,
    },

    [ABILITY_FLOWER_VEIL] =
    {
        .name = _("Flora-Voile"),
        .description = COMPOUND_STRING("Mist for Grass-type allies."),
        .aiRating = 0,
        .breakable = TRUE,
    },

    [ABILITY_CHEEK_POUCH] =
    {
        .name = _("Bajoues"),
        .description = COMPOUND_STRING("Eating a Berry restores HP."),
        .aiRating = 4,
    },

    [ABILITY_PROTEAN] =
    {
        .name = _("Protéen"),
        .description = COMPOUND_STRING("Changes type to used move."),
        .aiRating = 8,
    },

    [ABILITY_FUR_COAT] =
    {
        .name = _("Toison Épaisse"),
        .description = COMPOUND_STRING("Halves physical damage."),
        .aiRating = 7,
        .breakable = TRUE,
    },

    [ABILITY_MAGICIAN] =
    {
        .name = _("Magicien"),
        .description = COMPOUND_STRING("Steals the foe's held item."),
        .aiRating = 3,
    },

    [ABILITY_BULLETPROOF] =
    {
        .name = _("Pare-Balles"),
        .description = COMPOUND_STRING("Blocks ball and bomb moves."),
        .breakable = TRUE,
        .aiRating = 7,
    },

    [ABILITY_COMPETITIVE] =
    {
        .name = _("Battant"),
        .description = COMPOUND_STRING("Stat-lowering ups Sp. Atk."),
        .aiRating = 5,
    },

    [ABILITY_STRONG_JAW] =
    {
        .name = _("Prognathe"),
        .description = COMPOUND_STRING("Boosts biting moves."),
        .aiRating = 6,
    },

    [ABILITY_REFRIGERATE] =
    {
        .name = _("Peau Gelée"),
        .description = COMPOUND_STRING("Normal moves become Ice."),
        .aiRating = 8,
    },

    [ABILITY_SWEET_VEIL] =
    {
        .name = _("Gluco-Voile"),
        .description = COMPOUND_STRING("Protects allies from sleep."),
        .aiRating = 4,
        .breakable = TRUE,
    },

    [ABILITY_STANCE_CHANGE] =
    {
        .name = _("Déclic Tactique"),
        .description = COMPOUND_STRING("Swaps Forme as it battles."),
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
        .description = COMPOUND_STRING("Flying moves have priority."),
        .aiRating = 6,
    },

    [ABILITY_MEGA_LAUNCHER] =
    {
        .name = _("Méga Blaster"),
        .description = COMPOUND_STRING("Powers up pulse moves."),
        .aiRating = 7,
    },

    [ABILITY_GRASS_PELT] =
    {
        .name = _("Toison Herbue"),
        .description = COMPOUND_STRING("Grassy Terrain ups Defense."),
        .aiRating = 2,
        .breakable = TRUE,
    },

    [ABILITY_SYMBIOSIS] =
    {
        .name = _("Symbiose"),
        .description = COMPOUND_STRING("Passes held item to ally."),
        .aiRating = 0,
    },

    [ABILITY_TOUGH_CLAWS] =
    {
        .name = _("Griffe Dure"),
        .description = COMPOUND_STRING("Powers up contact moves."),
        .aiRating = 7,
    },

    [ABILITY_PIXILATE] =
    {
        .name = _("Peau Féérique"),
        .description = COMPOUND_STRING("Normal moves become Fairy."),
        .aiRating = 8,
    },

    [ABILITY_GOOEY] =
    {
        .name = _("Poisseux"),
        .description = COMPOUND_STRING("Contact lowers foe's Speed."),
        .aiRating = 5,
    },

    [ABILITY_AERILATE] =
    {
        .name = _("Peau Céleste"),
        .description = COMPOUND_STRING("Normal moves become Flying."),
        .aiRating = 8,
    },

    [ABILITY_PARENTAL_BOND] =
    {
        .name = _("Amour Filial"),
        .description = COMPOUND_STRING("Parent & child each attack."),
        .aiRating = 10,
    },

    [ABILITY_DARK_AURA] =
    {
        .name = _("Aura Ténébreuse"),
        .description = COMPOUND_STRING("Ups all Dark-type moves."),
        .aiRating = 6,
        .breakable = B_UPDATED_ABILITY_DATA < GEN_8,
    },

    [ABILITY_FAIRY_AURA] =
    {
        .name = _("Aura Féérique"),
        .description = COMPOUND_STRING("Ups all Fairy-type moves."),
        .aiRating = 6,
        .breakable = B_UPDATED_ABILITY_DATA < GEN_8,
    },

    [ABILITY_AURA_BREAK] =
    {
        .name = _("Aura Inversée"),
        .description = COMPOUND_STRING("Reverses Aura Abilities."),
        .aiRating = 3,
        .breakable = TRUE,
    },

    [ABILITY_PRIMORDIAL_SEA] =
    {
        .name = _("Mer Primaire"),
        .description = COMPOUND_STRING("Summons heavy rain."),
        .aiRating = 10,
    },

    [ABILITY_DESOLATE_LAND] =
    {
        .name = _("Terre Finale"),
        .description = COMPOUND_STRING("Summons extreme sunlight."),
        .aiRating = 10,
    },

    [ABILITY_DELTA_STREAM] =
    {
        .name = _("Souffle Delta"),
        .description = COMPOUND_STRING("Summons strong winds."),
        .aiRating = 10,
    },

    [ABILITY_STAMINA] =
    {
        .name = _("Endurance"),
        .description = COMPOUND_STRING("Raises Defense when hit."),
        .aiRating = 6,
    },

    [ABILITY_WIMP_OUT] =
    {
        .name = _("Escampette"),
        .description = COMPOUND_STRING("Switches out at half HP."),
        .aiRating = 3,
    },

    [ABILITY_EMERGENCY_EXIT] =
    {
        .name = _("Repli Tactique"),
        .description = COMPOUND_STRING("Switches out at half HP."),
        .aiRating = 3,
    },

    [ABILITY_WATER_COMPACTION] =
    {
        .name = _("Sable Humide"),
        .description = COMPOUND_STRING("Ups Defense if hit by Water."),
        .aiRating = 4,
    },

    [ABILITY_MERCILESS] =
    {
        .name = _("Cruauté"),
        .description = COMPOUND_STRING("Critical hits poisoned foes."),
        .aiRating = 4,
    },

    [ABILITY_SHIELDS_DOWN] =
    {
        .name = _("Bouclier-Carcan"),
        .description = COMPOUND_STRING("Shell breaks at half HP."),
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
        .description = COMPOUND_STRING("Stronger on foe's switch in."),
        .aiRating = 6,
    },

    [ABILITY_WATER_BUBBLE] =
    {
        .name = _("Aquabulle"),
        .description = COMPOUND_STRING("Guards from Fire and burns."),
        .aiRating = 8,
        .breakable = TRUE,
    },

    [ABILITY_STEELWORKER] =
    {
        .name = _("Expert Acier"),
        .description = COMPOUND_STRING("Ups Steel-type moves."),
        .aiRating = 6,
    },

    [ABILITY_BERSERK] =
    {
        .name = _("Folle Furie"),
        .description = COMPOUND_STRING("Boosts Sp. Atk at half HP."),
        .aiRating = 5,
    },

    [ABILITY_SLUSH_RUSH] =
    {
        .name = _("Chasse-Neige"),
        .description = COMPOUND_STRING(
        #if B_PREFERRED_ICE_WEATHER == B_ICE_WEATHER_HAIL
            "Boosts Speed in hail."),
        #elif B_PREFERRED_ICE_WEATHER == B_ICE_WEATHER_SNOW
            "Boosts Speed in snow."),
        #else
            "Ups Speed in hail or snow."),
        #endif
        .aiRating = 5,
    },

    [ABILITY_LONG_REACH] =
    {
        .name = _("Longue Portée"),
        .description = COMPOUND_STRING("Moves don't make contact."),
        .aiRating = 3,
    },

    [ABILITY_LIQUID_VOICE] =
    {
        .name = _("Hydrata-Son"),
        .description = COMPOUND_STRING("Sound moves become Water."),
        .aiRating = 5,
    },

    [ABILITY_TRIAGE] =
    {
        .name = _("Prioguérison"),
        .description = COMPOUND_STRING("Healing moves have priority."),
        .aiRating = 7,
    },

    [ABILITY_GALVANIZE] =
    {
        .name = _("Peau Électrique"),
        .description = COMPOUND_STRING("Normal moves turn Electric."),
        .aiRating = 8,
    },

    [ABILITY_SURGE_SURFER] =
    {
        .name = _("Surf Caudal"),
        .description = COMPOUND_STRING("Electric Terrain ups Speed."),
        .aiRating = 4,
    },

    [ABILITY_SCHOOLING] =
    {
        .name = _("Banc"),
        .description = COMPOUND_STRING("Forms a school at high HP."),
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
        .description = COMPOUND_STRING("Shroud protects once."),
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
        .description = COMPOUND_STRING("Changes form after a KO."),
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
        .description = COMPOUND_STRING("Changes Forme at half HP."),
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
        .description = COMPOUND_STRING("Can poison Steel & Poison."),
        .aiRating = 5,
    },

    [ABILITY_COMATOSE] =
    {
        .name = _("Hypersommeil"),
        .description = COMPOUND_STRING("Always drowsing."),
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
        .description = COMPOUND_STRING("Allies block priority moves."),
        .aiRating = 6,
        .breakable = TRUE,
    },

    [ABILITY_INNARDS_OUT] =
    {
        .name = _("Expuls'Organes"),
        .description = COMPOUND_STRING("Damages the foe if KO'd."),
        .aiRating = 5,
    },

    [ABILITY_DANCER] =
    {
        .name = _("Danseuse"),
        .description = COMPOUND_STRING("Dances along with others."),
        .aiRating = 5,
    },

    [ABILITY_BATTERY] =
    {
        .name = _("Batterie"),
        .description = COMPOUND_STRING("Boosts ally's Sp. Atk."),
        .aiRating = 0,
    },

    [ABILITY_FLUFFY] =
    {
        .name = _("Boule de Poils"),
        .description = COMPOUND_STRING("Halves contact, heat hurts."),
        .aiRating = 5,
        .breakable = TRUE,
    },

    [ABILITY_DAZZLING] =
    {
        .name = _("Corps Coloré"),
        .description = COMPOUND_STRING("Allies block priority moves."),
        .aiRating = 5,
        .breakable = TRUE,
    },

    [ABILITY_SOUL_HEART] =
    {
        .name = _("Animacœur"),
        .description = COMPOUND_STRING("Others fainting ups Sp. Atk."),
        .aiRating = 7,
    },

    [ABILITY_TANGLING_HAIR] =
    {
        .name = _("Mèche Rebelle"),
        .description = COMPOUND_STRING("Contact lowers foe's Speed."),
        .aiRating = 5,
    },

    [ABILITY_RECEIVER] =
    {
        .name = _("Receveur"),
        .description = COMPOUND_STRING("Copies fainted ally Ability."),
        .aiRating = 0,
        .cantBeCopied = TRUE,
        .cantBeTraced = TRUE,
    },

    [ABILITY_POWER_OF_ALCHEMY] =
    {
        .name = _("Osmose"),
        .description = COMPOUND_STRING("Copies fainted ally Ability."),
        .aiRating = 0,
        .cantBeCopied = TRUE,
        .cantBeTraced = TRUE,
    },

    [ABILITY_BEAST_BOOST] =
    {
        .name = _("Boost Chimère"),
        .description = COMPOUND_STRING("Raises best stat after KOs."),
        .aiRating = 7,
    },

    [ABILITY_RKS_SYSTEM] =
    {
        .name = _("Système Alpha"),
        .description = COMPOUND_STRING("Matches held memory's type."),
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
        .description = COMPOUND_STRING("Makes the field electrified."),
        .aiRating = 8,
    },

    [ABILITY_PSYCHIC_SURGE] =
    {
        .name = _("Créa-Psy"),
        .description = COMPOUND_STRING("Makes the field weird."),
        .aiRating = 8,
    },

    [ABILITY_MISTY_SURGE] =
    {
        .name = _("Créa-Brume"),
        .description = COMPOUND_STRING("Makes the field misty."),
        .aiRating = 8,
    },

    [ABILITY_GRASSY_SURGE] =
    {
        .name = _("Créa-Herbe"),
        .description = COMPOUND_STRING("Makes the field grassy."),
        .aiRating = 8,
    },

    [ABILITY_FULL_METAL_BODY] =
    {
        .name = _("Métallo-Garde"),
        .description = COMPOUND_STRING("Stats can't be lowered."),
        .aiRating = 4,
    },

    [ABILITY_SHADOW_SHIELD] =
    {
        .name = _("Spectro-Bouclier"),
        .description = COMPOUND_STRING("Reduces damage at full HP."),
        .aiRating = 8,
    },

    [ABILITY_PRISM_ARMOR] =
    {
        .name = _("Prisme-Armure"),
        .description = COMPOUND_STRING("Weakens “supereffective”."),
        .aiRating = 6,
    },

    [ABILITY_NEUROFORCE] =
    {
        .name = _("Cérébro-Force"),
        .description = COMPOUND_STRING("Ups “supereffective” hits."),
        .aiRating = 6,
    },

    [ABILITY_INTREPID_SWORD] =
    {
        .name = _("Lame Indomptable"),
        .description = COMPOUND_STRING(
        #if B_INTREPID_SWORD >= GEN_9
            "Ups Attack on entry once."),
        #else
            "Raises Attack on entry."),
        #endif
        .aiRating = 3,
    },

    [ABILITY_DAUNTLESS_SHIELD] =
    {
        .name = _("Égide Inflexible"),
        .description = COMPOUND_STRING(
        #if B_DAUNTLESS_SHIELD >= GEN_9
            "Ups Defense on entry once."),
        #else
            "Raises Defense on entry."),
        #endif
        .aiRating = 3,
    },

    [ABILITY_LIBERO] =
    {
        .name = _("Libéro"),
        .description = COMPOUND_STRING("Changes type to used move."),
    },

    [ABILITY_BALL_FETCH] =
    {
        .name = _("Ramasse Ball"),
        .description = COMPOUND_STRING("Fetches first failed Ball."),
        .aiRating = 0,
    },

    [ABILITY_COTTON_DOWN] =
    {
        .name = _("Effilochage"),
        .description = COMPOUND_STRING("Lowers others' Speed if hit."),
        .aiRating = 3,
    },

    [ABILITY_PROPELLER_TAIL] =
    {
        .name = _("Propulseur"),
        .description = COMPOUND_STRING("Ignores foes' move-drawing."),
        .aiRating = 2,
    },

    [ABILITY_MIRROR_ARMOR] =
    {
        .name = _("Armure Miroir"),
        .description = COMPOUND_STRING("Bounces stat-lowering."),
        .aiRating = 6,
        .breakable = TRUE,
    },

    [ABILITY_GULP_MISSILE] =
    {
        .name = _("Dégobage"),
        .description = COMPOUND_STRING("Spits when hit after Dive."),
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
        .description = COMPOUND_STRING("Ignores foes' move-drawing."),
        .aiRating = 2,
    },

    [ABILITY_STEAM_ENGINE] =
    {
        .name = _("Turbine"),
        .description = COMPOUND_STRING("Fire & Water hits up Speed."),
        .aiRating = 3,
    },

    [ABILITY_PUNK_ROCK] =
    {
        .name = _("Punk Rock"),
        .description = COMPOUND_STRING("Boosts and resists sound."),
        .aiRating = 2,
        .breakable = TRUE,
    },

    [ABILITY_SAND_SPIT] =
    {
        .name = _("Expul'Sable"),
        .description = COMPOUND_STRING("Summons a sandstorm if hit."),
        .aiRating = 5,
    },

    [ABILITY_ICE_SCALES] =
    {
        .name = _("Écailles Glacées"),
        .description = COMPOUND_STRING("Halves special damage."),
        .aiRating = 7,
        .breakable = TRUE,
    },

    [ABILITY_RIPEN] =
    {
        .name = _("Mûrissement"),
        .description = COMPOUND_STRING("Doubles Berry effects."),
        .aiRating = 4,
    },

    [ABILITY_ICE_FACE] =
    {
        .name = _("Tête de Gel"),
        .description = COMPOUND_STRING(
        #if B_PREFERRED_ICE_WEATHER == B_ICE_WEATHER_HAIL
            "Hail renews one free hit."),
        #elif B_PREFERRED_ICE_WEATHER == B_ICE_WEATHER_SNOW
            "Snow renews one free hit."),
        #else
            "Hail & snow renew free hit."),
        #endif
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
        .description = COMPOUND_STRING("Powers up ally's moves."),
        .aiRating = 2,
    },

    [ABILITY_MIMICRY] =
    {
        .name = _("Mimétisme"),
        .description = COMPOUND_STRING("Changes type on terrain."),
        .aiRating = 2,
    },

    [ABILITY_SCREEN_CLEANER] =
    {
        .name = _("Brise-Barrière"),
        .description = COMPOUND_STRING("Nullifies barriers on entry."),
        .aiRating = 3,
    },

    [ABILITY_STEELY_SPIRIT] =
    {
        .name = _("Boost Acier"),
        .description = COMPOUND_STRING("Ups allies' Steel moves."),
        .aiRating = 2,
    },

    [ABILITY_PERISH_BODY] =
    {
        .name = _("Corps Condamné"),
        .description = COMPOUND_STRING("Foe faints in 3 turns if hit."),
        .aiRating = -1,
    },

    [ABILITY_WANDERING_SPIRIT] =
    {
        .name = _("Âme Vagabonde"),
        .description = COMPOUND_STRING("Swaps Abilities on contact."),
        .aiRating = 2,
    },

    [ABILITY_GORILLA_TACTICS] =
    {
        .name = _("Entêtement"),
        .description = COMPOUND_STRING("Ups Attack and locks move."),
        .aiRating = 4,
    },

    [ABILITY_NEUTRALIZING_GAS] =
    {
        .name = _("Gaz Inhibiteur"),
        .description = COMPOUND_STRING("Nullifies others' Abilities."),
        .aiRating = 5,
        .cantBeCopied = TRUE,
        .cantBeSwapped = TRUE,
        .cantBeTraced = TRUE,
        .failsOnImposter = TRUE,
    },

    [ABILITY_PASTEL_VEIL] =
    {
        .name = _("Voile Pastel"),
        .description = COMPOUND_STRING("Allies can't be poisoned."),
        .aiRating = 4,
        .breakable = TRUE,
    },

    [ABILITY_HUNGER_SWITCH] =
    {
        .name = _("Déclic Fringale"),
        .description = COMPOUND_STRING("Changes form every turn."),
        .aiRating = 2,
        .cantBeCopied = TRUE,
        .cantBeSwapped = TRUE,
        .cantBeTraced = TRUE,
        .failsOnImposter = TRUE,
    },

    [ABILITY_QUICK_DRAW] =
    {
        .name = _("Tir Vif"),
        .description = COMPOUND_STRING("Occasionally moves first."),
        .aiRating = 4,
    },

    [ABILITY_UNSEEN_FIST] =
    {
        .name = _("Poing Invisible"),
        .description = COMPOUND_STRING("Contact evades protection."),
        .aiRating = 6,
    },

    [ABILITY_CURIOUS_MEDICINE] =
    {
        .name = _("Breuvage Suspect"),
        .description = COMPOUND_STRING("Removes ally stat changes."),
        .aiRating = 3,
    },

    [ABILITY_TRANSISTOR] =
    {
        .name = _("Transistor"),
        .description = COMPOUND_STRING("Ups Electric-type moves."),
        .aiRating = 6,
    },

    [ABILITY_DRAGONS_MAW] =
    {
        .name = _("Dent de Dragon"),
        .description = COMPOUND_STRING("Ups Dragon-type moves."),
        .aiRating = 6,
    },

    [ABILITY_CHILLING_NEIGH] =
    {
        .name = _("Blanche Ruade"),
        .description = COMPOUND_STRING("Raises Attack after KOs."),
        .aiRating = 7,
    },

    [ABILITY_GRIM_NEIGH] =
    {
        .name = _("Sombre Ruade"),
        .description = COMPOUND_STRING("Raises Sp. Atk after KOs."),
        .aiRating = 7,
    },

    [ABILITY_AS_ONE_ICE_RIDER] =
    {
        .name = _("Osmose Équine"),
        .description = COMPOUND_STRING("Unnerve and Chilling Neigh."),
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
        .description = COMPOUND_STRING("Unnerve and Grim Neigh."),
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
        .description = COMPOUND_STRING("Ability spreads on contact."),
        .aiRating = 5,
    },

    [ABILITY_SEED_SOWER] =
    {
        .name = _("Semencier"),
        .description = COMPOUND_STRING("Makes field grassy if hit."),
        .aiRating = 5,
    },

    [ABILITY_THERMAL_EXCHANGE] =
    {
        .name = _("Thermodynamique"),
        .description = COMPOUND_STRING("Ups Attack if hit by Fire."),
        .aiRating = 4,
        .breakable = TRUE,
    },

    [ABILITY_ANGER_SHELL] =
    {
        .name = _("Courroupace"),
        .description = COMPOUND_STRING("Changes stats at half HP."),
        .aiRating = 3,
    },

    [ABILITY_PURIFYING_SALT] =
    {
        .name = _("Sel Purificateur"),
        .description = COMPOUND_STRING("Guards from status & Ghost."),
        .aiRating = 6,
        .breakable = TRUE,
    },

    [ABILITY_WELL_BAKED_BODY] =
    {
        .name = _("Bien Cuit"),
        .description = COMPOUND_STRING("Ups Defense if hit by Fire."),
        .aiRating = 5,
        .breakable = TRUE,
    },

    [ABILITY_WIND_RIDER] =
    {
        .name = _("Aéroporté"),
        .description = COMPOUND_STRING("Wind moves raise Attack."),
        .aiRating = 4,
        .breakable = TRUE,
    },

    [ABILITY_GUARD_DOG] =
    {
        .name = _("Chien de Garde"),
        .description = COMPOUND_STRING("Raises Atk if intimidated."),
        .aiRating = 5,
        .breakable = TRUE,
    },

    [ABILITY_ROCKY_PAYLOAD] =
    {
        .name = _("Porte-Roche"),
        .description = COMPOUND_STRING("Powers up Rock-type moves."),
        .aiRating = 6,
    },

    [ABILITY_WIND_POWER] =
    {
        .name = _("Turbine Éolienne"),
        .description = COMPOUND_STRING("Gets charged if hit by wind."),
        .aiRating = 4,
    },

    [ABILITY_ZERO_TO_HERO] =
    {
        .name = _("Supermutation"),
        .description = COMPOUND_STRING("Changes form upon switch."),
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
        .description = COMPOUND_STRING("Commands from ally Dondozo."),
        .aiRating = 10,
        .cantBeCopied = TRUE,
        .cantBeSwapped = TRUE,
        .cantBeTraced = TRUE,
    },

    [ABILITY_ELECTROMORPHOSIS] =
    {
        .name = _("Grecharge"),
        .description = COMPOUND_STRING("Becomes charged when hit."),
        .aiRating = 5,
    },

    [ABILITY_PROTOSYNTHESIS] =
    {
        .name = _("Paléosynthèse"),
        .description = COMPOUND_STRING("Sunlight boosts best stat."),
        .aiRating = 7,
        .cantBeCopied = TRUE,
        .cantBeSwapped = TRUE,
        .cantBeTraced = TRUE,
        .failsOnImposter = TRUE,
    },

    [ABILITY_QUARK_DRIVE] =
    {
        .name = _("Charge Quantique"),
        .description = COMPOUND_STRING("Electr. field ups best stat."),
        .aiRating = 7,
        .cantBeCopied = TRUE,
        .cantBeSwapped = TRUE,
        .cantBeTraced = TRUE,
        .failsOnImposter = TRUE,
    },

    [ABILITY_GOOD_AS_GOLD] =
    {
        .name = _("Corps en Or"),
        .description = COMPOUND_STRING("Immunity to status moves."),
        .aiRating = 8,
        .breakable = TRUE,
    },

    [ABILITY_VESSEL_OF_RUIN] =
    {
        .name = _("Urne du Fléau"),
        .description = COMPOUND_STRING("Reduces others' Sp. Atk."),
        .aiRating = 5,
        .breakable = TRUE,
    },

    [ABILITY_SWORD_OF_RUIN] =
    {
        .name = _("Épée du Fléau"),
        .description = COMPOUND_STRING("Reduces others' Defense."),
        .aiRating = 5,
        .breakable = TRUE,
    },

    [ABILITY_TABLETS_OF_RUIN] =
    {
        .name = _("Bois du Fléau"),
        .description = COMPOUND_STRING("Reduces others' Attack."),
        .aiRating = 5,
        .breakable = TRUE,
    },

    [ABILITY_BEADS_OF_RUIN] =
    {
        .name = _("Perles du Fléau"),
        .description = COMPOUND_STRING("Reduces others' Sp. Def."),
        .aiRating = 5,
        .breakable = TRUE,
    },

    [ABILITY_ORICHALCUM_PULSE] =
    {
        .name = _("Pouls Orichalque"),
        .description = COMPOUND_STRING("Summons sun and ups Atk."),
        .aiRating = 8,
    },

    [ABILITY_HADRON_ENGINE] =
    {
        .name = _("Moteur à Hadrons"),
        .description = COMPOUND_STRING("Electr. Surge & ups Sp. Atk."),
        .aiRating = 8,
    },

    [ABILITY_OPPORTUNIST] =
    {
        .name = _("Opportuniste"),
        .description = COMPOUND_STRING("Copies a foe's stat boost."),
        .aiRating = 5,
    },

    [ABILITY_CUD_CHEW] =
    {
        .name = _("Ruminant"),
        .description = COMPOUND_STRING("Eats Berry again next turn."),
        .aiRating = 4,
    },

    [ABILITY_SHARPNESS] =
    {
        .name = _("Incisif"),
        .description = COMPOUND_STRING("Powers up slicing moves."),
        .aiRating = 7,
    },

    [ABILITY_SUPREME_OVERLORD] =
    {
        .name = _("Général Suprême"),
        .description = COMPOUND_STRING("Fainted partners up power."),
        .aiRating = 6,
    },

    [ABILITY_COSTAR] =
    {
        .name = _("Collab"),
        .description = COMPOUND_STRING("Copies ally's stat changes."),
        .aiRating = 5,
    },

    [ABILITY_TOXIC_DEBRIS] =
    {
        .name = _("Dépôt Toxique"),
        .description = COMPOUND_STRING("Lays poison spikes if hit."),
        .aiRating = 4,
    },

    [ABILITY_ARMOR_TAIL] =
    {
        .name = _("Armure Caudale"),
        .description = COMPOUND_STRING("Allies block priority moves."),
        .aiRating = 5,
        .breakable = TRUE,
    },

    [ABILITY_EARTH_EATER] =
    {
        .name = _("Absorbe-Terre"),
        .description = COMPOUND_STRING("Restores if hit by Ground."),
        .aiRating = 7,
        .breakable = TRUE,
    },

    [ABILITY_MYCELIUM_MIGHT] =
    {
        .name = _("Force Fongique"),
        .description = COMPOUND_STRING("Status moves can't fail."),
        .aiRating = 2,
    },

    [ABILITY_HOSPITALITY] =
    {
        .name = _("Aux Petits Soins"),
        .description = COMPOUND_STRING("Restores ally's HP on entry."),
        .aiRating = 5,
    },

    [ABILITY_MINDS_EYE] =
    {
        .name = _("Œil Révélateur"),
        .description = COMPOUND_STRING("Grants Foresight."),
        .aiRating = 8,
        .breakable = TRUE,
    },

    [ABILITY_EMBODY_ASPECT_TEAL_MASK] =
    {
        .name = _("Force Mémorielle"),
        .description = COMPOUND_STRING("Terastal raises Speed."),
        .aiRating = 6,
        .cantBeCopied = TRUE,
        .cantBeSwapped = TRUE,
        .cantBeTraced = TRUE,
        .failsOnImposter = TRUE,
    },

    [ABILITY_EMBODY_ASPECT_HEARTHFLAME_MASK] =
    {
        .name = _("Force Mémorielle"),
        .description = COMPOUND_STRING("Terastal raises Attack."),
        .aiRating = 6,
        .cantBeCopied = TRUE,
        .cantBeSwapped = TRUE,
        .cantBeTraced = TRUE,
        .failsOnImposter = TRUE,
    },

    [ABILITY_EMBODY_ASPECT_WELLSPRING_MASK] =
    {
        .name = _("Force Mémorielle"),
        .description = COMPOUND_STRING("Terastal raises Sp. Def."),
        .aiRating = 6,
        .cantBeCopied = TRUE,
        .cantBeSwapped = TRUE,
        .cantBeTraced = TRUE,
        .failsOnImposter = TRUE,
    },

    [ABILITY_EMBODY_ASPECT_CORNERSTONE_MASK] =
    {
        .name = _("Force Mémorielle"),
        .description = COMPOUND_STRING("Terastal raises Defense."),
        .aiRating = 6,
        .cantBeCopied = TRUE,
        .cantBeSwapped = TRUE,
        .cantBeTraced = TRUE,
        .failsOnImposter = TRUE,
    },

    [ABILITY_TOXIC_CHAIN] =
    {
        .name = _("Chaîne Toxique"),
        .description = COMPOUND_STRING("Attacks may badly poison."),
        .aiRating = 8,
    },

    [ABILITY_SUPERSWEET_SYRUP] =
    {
        .name = _("Nectar Mielleux"),
        .description = COMPOUND_STRING("Lowers foes' evasion once."),
        .aiRating = 5,
    },

    [ABILITY_TERA_SHIFT] =
    {
        .name = _("Téramorphose"),
        .description = COMPOUND_STRING("Changes form on entry."),
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
        .description = COMPOUND_STRING("Resists all types at full HP."),
        .aiRating = 10,
        .cantBeCopied = TRUE,
        .cantBeSwapped = TRUE,
        .cantBeTraced = TRUE,
        .breakable = TRUE,
    },

    [ABILITY_TERAFORM_ZERO] =
    {
        .name = _("Téraformation 0"),
        .description = COMPOUND_STRING("Zeroes weather and terrain."),
        .aiRating = 10,
        .cantBeCopied = TRUE,
        .cantBeSwapped = TRUE,
        .cantBeTraced = TRUE,
    },

    [ABILITY_POISON_PUPPETEER] =
    {
        .name = _("Emprise Toxique"),
        .description = COMPOUND_STRING("Poisoning also confuses."),
        .aiRating = 8,
        .cantBeCopied = TRUE,
        .cantBeSwapped = TRUE,
        .cantBeTraced = TRUE,
    },

    [ABILITY_PIERCING_DRILL] =
    {
        .name = _("Transperceuse"),
        .description = COMPOUND_STRING("Contact evades protection."),
    },

    [ABILITY_DRAGONIZE] =
    {
        .name = _("Peau Draconique"),
        .description = COMPOUND_STRING("Normal moves turn Dragon."),
    },

    [ABILITY_EELEVATE] =
    {
        .name = _("Eelevate"),
        .description = COMPOUND_STRING("Levitate and Beast Boost."),
    },

    [ABILITY_314] =
    {
        .name = _("-------"),
        .description = COMPOUND_STRING("No special ability."),
    },

    [ABILITY_MEGA_SOL] =
    {
        .name = _("Méga-Soleil"),
        .description = COMPOUND_STRING("Acts like under sun."),
    },

    [ABILITY_FIRE_MANE] =
    {
        .name = _("Fire Mane"),
        .description = COMPOUND_STRING("Ups Fire-type moves."),
    },

    [ABILITY_317] =
    {
        .name = _("-------"),
        .description = COMPOUND_STRING("No special ability."),
    },

    [ABILITY_SPICY_SPRAY] =
    {
        .name = _("Habanéruption"),
        .description = COMPOUND_STRING("Burns the foe when damaged."),
    },

    [ABILITY_AURA_GUARD] =
    {
        .name = _("Aura Guard"),
        .description = COMPOUND_STRING("Unimplemented."),
    },
};
