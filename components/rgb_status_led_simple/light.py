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

# Configuration keys
CONF_ERROR_COLOR = "error_color"
CONF_WARNING_COLOR = "warning_color"
CONF_ERROR_BLINK_SPEED = "error_blink_speed"
CONF_WARNING_BLINK_SPEED = "warning_blink_speed"

# Namespace for the component
rgb_status_led_simple_ns = cg.esphome_ns.namespace("rgb_status_led_simple")
RGBStatusLEDSimple = rgb_status_led_simple_ns.class_("RGBStatusLEDSimple", light.LightOutput)

# Schema for RGB color configuration
ColorSchema = cv.Schema({
    cv.Required(CONF_RED): cv.percentage,
    cv.Required(CONF_GREEN): cv.percentage,
    cv.Required(CONF_BLUE): cv.percentage,
})

# Schema for the component
CONFIG_SCHEMA = light.RGB_LIGHT_SCHEMA.extend(
    {
        cv.GenerateID(): cv.declare_id(RGBStatusLEDSimple),
        cv.Required(CONF_RED): cv.use_id(output.FloatOutput),
        cv.Required(CONF_GREEN): cv.use_id(output.FloatOutput),
        cv.Required(CONF_BLUE): cv.use_id(output.FloatOutput),
        cv.Optional(CONF_ERROR_COLOR, default={CONF_RED: 1.0, CONF_GREEN: 0.0, CONF_BLUE: 0.0}): ColorSchema,
        cv.Optional(CONF_WARNING_COLOR, default={CONF_RED: 1.0, CONF_GREEN: 0.5, CONF_BLUE: 0.0}): ColorSchema,
        cv.Optional(CONF_ERROR_BLINK_SPEED, default="250ms"): cv.positive_time_period_milliseconds,
        cv.Optional(CONF_WARNING_BLINK_SPEED, default="1500ms"): cv.positive_time_period_milliseconds,
        cv.Optional(CONF_BRIGHTNESS, default=1.0): cv.percentage,
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
    
    # Set error color
    error_color = config[CONF_ERROR_COLOR]
    cg.add(var.set_error_color(
        error_color[CONF_RED],
        error_color[CONF_GREEN],
        error_color[CONF_BLUE]
    ))
    
    # Set warning color
    warning_color = config[CONF_WARNING_COLOR]
    cg.add(var.set_warning_color(
        warning_color[CONF_RED],
        warning_color[CONF_GREEN],
        warning_color[CONF_BLUE]
    ))
    
    # Set timing and brightness
    cg.add(var.set_error_blink_speed(int(config[CONF_ERROR_BLINK_SPEED])))
    cg.add(var.set_warning_blink_speed(int(config[CONF_WARNING_BLINK_SPEED])))
    cg.add(var.set_brightness(config[CONF_BRIGHTNESS]))
    
    # Register the light
    await light.register_light(var, config)