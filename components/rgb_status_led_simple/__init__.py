"""
ESPHome RGB Status LED Simple Component

A minimal component that works exactly like ESPHome's internal status_led
but with RGB light support.

Author: Bluscream
License: MIT
Updated: 2026-01-30 04:28:00 UTC
"""

import esphome.codegen as cg
from esphome.components import light

# Component metadata
CODEOWNERS = ["@esphome/core"]
AUTO_LOAD = ["light"]

# Import everything from light.py to avoid circular imports
from .light import *
