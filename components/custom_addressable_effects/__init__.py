import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components.light.types import AddressableLightEffect
from esphome.components.light.effects import register_addressable_effect

from esphome.const import (
    CONF_NAME,
    CONF_RED,
    CONF_GREEN,
    CONF_BLUE,
    CONF_WHITE,
)

CONF_STARS_PROBABILITY = "stars_probability"

# TwinkleFox configuration
CONF_TWINKLE_SPEED = "twinkle_speed"
CONF_TWINKLE_DENSITY = "twinkle_density"
CONF_COOL_LIKE_INCANDESCENT = "cool_like_incandescent"
CONF_PALETTE = "palette"

CONF_CYCLE_S = "cycle_s"
CONF_SCALE = "scale"

# ColorTwinkles configuration
CONF_STARTING_BRIGHTNESS = "starting_brightness"
CONF_FADE_IN_SPEED = "fade_in_speed"
CONF_FADE_OUT_SPEED = "fade_out_speed"
CONF_DENSITY = "density"
CONF_COLOR_TWINKLES_PALETTE = "palette"

light_ns = cg.esphome_ns.namespace("light")
AddressableStarsEffect = light_ns.class_("AddressableStarsEffect", AddressableLightEffect)

ColorStruct = cg.esphome_ns.struct("Color")
AddressableColorStarsEffectColor = light_ns.struct("AddressableColorStarsEffectColor")

AddressableTwinkleFoxEffect = light_ns.class_("AddressableTwinkleFoxEffect", AddressableLightEffect)
AddressableBlendsEffect = light_ns.class_("AddressableBlendsEffect", AddressableLightEffect)

# TwinkleFox palette enum
TwinkleFoxPaletteType = light_ns.enum("TwinkleFoxPaletteType")
TWINKLEFOX_PALETTES = {
    "party_colors": TwinkleFoxPaletteType.PALETTE_PARTY_COLORS,
    "ocean_colors": TwinkleFoxPaletteType.PALETTE_OCEAN_COLORS,
    "lava_colors": TwinkleFoxPaletteType.PALETTE_LAVA_COLORS,
    "forest_colors": TwinkleFoxPaletteType.PALETTE_FOREST_COLORS,
    "rainbow_colors": TwinkleFoxPaletteType.PALETTE_RAINBOW_COLORS,
    "snow_colors": TwinkleFoxPaletteType.PALETTE_SNOW_COLORS,
    "holly_colors": TwinkleFoxPaletteType.PALETTE_HOLLY_COLORS,
    "ice_colors": TwinkleFoxPaletteType.PALETTE_ICE_COLORS,
    "fairy_light": TwinkleFoxPaletteType.PALETTE_FAIRY_LIGHT,
    "retro_c9": TwinkleFoxPaletteType.PALETTE_RETRO_C9,
}

# ColorTwinkles palette enum
ColorTwinklesPaletteType = light_ns.enum("ColorTwinklesPaletteType")
COLOR_TWINKLES_PALETTES = {
    "cloud_colors": ColorTwinklesPaletteType.COLOR_TWINKLES_PALETTE_CLOUD_COLORS,
    "rainbow_colors": ColorTwinklesPaletteType.COLOR_TWINKLES_PALETTE_RAINBOW_COLORS,
    "snow_colors": ColorTwinklesPaletteType.COLOR_TWINKLES_PALETTE_SNOW_COLORS,
    "incandescent": ColorTwinklesPaletteType.COLOR_TWINKLES_PALETTE_INCANDESCENT,
    "party_colors": ColorTwinklesPaletteType.COLOR_TWINKLES_PALETTE_PARTY_COLORS,
    "ocean_colors": ColorTwinklesPaletteType.COLOR_TWINKLES_PALETTE_OCEAN_COLORS,
    "forest_colors": ColorTwinklesPaletteType.COLOR_TWINKLES_PALETTE_FOREST_COLORS,
    "lava_colors": ColorTwinklesPaletteType.COLOR_TWINKLES_PALETTE_LAVA_COLORS,
}


CONFIG_SCHEMA = cv.All(cv.Schema({}))

@register_addressable_effect(
    "addressable_stars",
    AddressableStarsEffect,
    "Stars",
    {
        cv.Optional(CONF_STARS_PROBABILITY, default="10%"): cv.percentage,
    },
)
async def addressable_stars_effect_to_code(config, effect_id):
    var = cg.new_Pvariable(effect_id, config[CONF_NAME])
    cg.add(var.set_stars_probability(config[CONF_STARS_PROBABILITY]))
    return var

@register_addressable_effect(
    "addressable_twinklefox",
    AddressableTwinkleFoxEffect,
    "TwinkleFox",
    {
        cv.Optional(CONF_TWINKLE_SPEED, default=4): cv.int_range(min=1, max=8),
        cv.Optional(CONF_TWINKLE_DENSITY, default=5): cv.int_range(min=1, max=8),
        cv.Optional(CONF_COOL_LIKE_INCANDESCENT, default=True): cv.boolean,
        cv.Optional(CONF_PALETTE, default="party_colors"): cv.templatable(cv.string),
    },
)
async def addressable_twinklefox_effect_to_code(config, effect_id):
    var = cg.new_Pvariable(effect_id, config[CONF_NAME])
    cg.add(var.set_twinkle_speed(config[CONF_TWINKLE_SPEED]))
    cg.add(var.set_twinkle_density(config[CONF_TWINKLE_DENSITY]))
    cg.add(var.set_cool_like_incandescent(config[CONF_COOL_LIKE_INCANDESCENT]))
    if isinstance(config[CONF_PALETTE], str) and config[CONF_PALETTE].lower() in TWINKLEFOX_PALETTES:
        cg.add(var.set_palette(TWINKLEFOX_PALETTES[config[CONF_PALETTE].lower()]))
    else:
        cg.add(var.set_palette(config[CONF_PALETTE]))
    return var


@register_addressable_effect(
    "addressable_blends",
    AddressableBlendsEffect,
    "Palette Blends",
    {
        cv.Optional(CONF_CYCLE_S, default=20): cv.int_range(min=0, max=20),
        cv.Optional(CONF_SCALE, default=8): cv.int_range(min=1, max=64),
        cv.Optional(CONF_COLOR_TWINKLES_PALETTE, default="rainbow_colors"): cv.templatable(cv.string),
    },
)
async def addressable_blends_effect_to_code(config, effect_id):
    var = cg.new_Pvariable(effect_id, config[CONF_NAME])
    cg.add(var.set_cycle_s(config[CONF_CYCLE_S]))
    cg.add(var.set_scale(config[CONF_SCALE]))
    if isinstance(config[CONF_COLOR_TWINKLES_PALETTE], str) and config[CONF_COLOR_TWINKLES_PALETTE].lower() in COLOR_TWINKLES_PALETTES:
        cg.add(var.set_palette(COLOR_TWINKLES_PALETTES[config[CONF_COLOR_TWINKLES_PALETTE].lower()]))
    else:
        cg.add(var.set_palette(config[CONF_COLOR_TWINKLES_PALETTE]))
    return var