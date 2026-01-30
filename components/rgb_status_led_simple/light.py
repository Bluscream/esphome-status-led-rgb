"""
ESPHome RGB Status LED Simple Component

A minimal component that works like ESPHome's internal status_led
but with RGB light support and manual control when no status is active.

Author: Bluscream
License: MIT
Updated: 2026-01-30 04:40:00 UTC
"""

import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import light, output
from esphome.const import CONF_ID, CONF_RED, CONF_GREEN, CONF_BLUE, CONF_BRIGHTNESS

# Component metadata
CODEOWNERS = ["@esphome/core"]
AUTO_LOAD = ["light"]

# Namespace for the component
rgb_status_led_simple_ns = cg.esphome_ns.namespace("rgb_status_led_simple")
RGBStatusLEDSimple = rgb_status_led_simple_ns.class_("RGBStatusLEDSimple", light.LightOutput)

# Schema for the component
CONFIG_SCHEMA = light.RGB_LIGHT_SCHEMA.extend(
    {
        cv.GenerateID(): cv.declare_id(RGBStatusLEDSimple),
        cv.Required(CONF_RED): cv.use_id(output.FloatOutput),
        cv.Required(CONF_GREEN): cv.use_id(output.FloatOutput),
        cv.Required(CONF_BLUE): cv.use_id(output.FloatOutput),
    }
).extend(cv.COMPONENT_SCHEMA)

async def to_code(config):
    # Create the light output
    var = cg.new_Pvariable(config[CONF_ID])
    
    # Set the output pins
    red = await cg.get_variable(config[CONF_RED])
    green = await cg.get_variable(config[CONF_GREEN])
    blue = await cg.get_variable(config[CONF_BLUE])
    
    cg.add(var.set_red_output(red))
    cg.add(var.set_green_output(green))
    cg.add(var.set_blue_output(blue))
    
    # Register the light
    await light.register_light(var, config)