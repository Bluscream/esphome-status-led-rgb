"""
ESPHome RGB Status LED Simple Component

A minimal component that works exactly like ESPHome's internal status_led
but with RGB light support.

Author: Bluscream
License: MIT
Updated: 2026-01-30 04:25:00 UTC
"""

import esphome.codegen as cg
from esphome.components import light

# Component metadata
CODEOWNERS = ["@esphome/core"]
AUTO_LOAD = ["light"]

# Import the light platform
from .light import CONFIG_SCHEMA, to_code

# Register the component
rgb_status_led_simple_ns = cg.esphome_ns.namespace("rgb_status_led_simple")
RGBStatusLEDSimple = rgb_status_led_simple_ns.class_("RGBStatusLEDSimple", light.LightOutput)
