# ESPHome RGB Status LED Simple Component

A minimal component that works exactly like vanilla status_led:
- **2 Status States**: Error, Warning only (like vanilla)
- **Vanilla Compatibility**: Exact timing and behavior
- **RGB Light Support**: Shows RGB colors for status indication
- **Minimal Configuration**: Simple setup like vanilla status_led
- **Lightweight**: Minimal memory and CPU usage

**Use Case:** Basic status monitoring, learning, simple deployments

## 🎯 Purpose

This component provides the simplest possible RGB status LED that:
- Works **exactly** like ESPHome's vanilla `status_led`
- Monitors only **error** and **warning** states (no extra features)
- Uses **ESPHome-compatible timing** (250ms/60% error, 1500ms/17% warning)
- **No event automations needed** - automatically detects system state

## 🚀 Quick Start

### 1. Add to your ESPHome configuration

```yaml
# Add the external component
external_components:
  - source: github://Bluscream/esphome-rgb-status-led@main
    components: [rgb_status_led_simple]

# Configure the RGB Status LED Simple
light:
  - platform: rgb_status_led_simple
    id: system_status_led
    name: "System Status LED"
    red: out_led_r
    green: out_led_g
    blue: out_led_b
    
    # Simple color configuration
    error_color:
      red: 100%
      green: 0%
      blue: 0%
    warning_color:
      red: 100%
      green: 50%
      blue: 0%
    
    # Timing matches vanilla status_led exactly
    error_blink_speed: 250ms
    warning_blink_speed: 1500ms
    brightness: 80%

# Define your RGB outputs
output:
  - platform: ledc
    id: out_led_r
    pin: GPIO4
    channel: 0
  - platform: ledc
    id: out_led_g
    pin: GPIO19
    channel: 1
  - platform: ledc
    id: out_led_b
    pin: GPIO18
    channel: 2
```

### 2. That's it! No automations needed

The component automatically:
- ✅ Detects ESPHome system errors and warnings
- ✅ Shows them with configured colors and exact vanilla timing
- ✅ Returns to your manual control when no status is active

## 📋 Behavior

### Status Indications (Priority-based)

| State | Color | Effect | Timing | When Active |
|-------|-------|--------|--------|------------|
| **Error** | Red (configurable) | Fast blink | 250ms period, 150ms on (60% duty) | System errors detected |
| **Warning** | Orange (configurable) | Slow blink | 1500ms period, 250ms on (17% duty) | System warnings detected |
| **No Status** | User controlled | User defined | User controlled | No errors or warnings |

### Manual Control

When no system errors or warnings are active:
- ✅ **Full RGB control** via Home Assistant
- ✅ **Brightness control** 
- ✅ **Color temperature** (if supported)
- ✅ **Effects** (if supported by your light)
- ✅ **Automations** work normally

## ⚙️ Configuration Options

### Required

| Option | Type | Description |
|--------|------|-------------|
| `red` | output | Red channel output |
| `green` | output | Green channel output |
| `blue` | output | Blue channel output |

### Optional

| Option | Default | Description |
|--------|---------|-------------|
| `error_color` | `{red: 100%, green: 0%, blue: 0%}` | RGB color for error state |
| `warning_color` | `{red: 100%, green: 50%, blue: 0%}` | RGB color for warning state |
| `error_blink_speed` | `250ms` | Blink period for error (matches vanilla) |
| `warning_blink_speed` | `1500ms` | Blink period for warning (matches vanilla) |
| `brightness` | `1.0` | Global brightness multiplier |

## 🎨 Manual Control Examples

### Home Assistant Dashboard
```yaml
# Set purple when no status
service: light.turn_on
target:
  entity_id: light.system_status_led
data:
  rgb_color: [128, 0, 128]
  brightness: 50
```

### ESPHome Automation
```yaml
automation:
  - trigger:
      - platform: time_pattern
        seconds: 0
    action:
      - light.turn_on:
          id: system_status_led
          rgb: [0, 100, 50]  # Cyan
          brightness: 30%
    condition:
      # Only apply when no system errors/warnings
      - lambda: 'return (App.get_app_state() & (STATUS_LED_ERROR | STATUS_LED_WARNING)) == 0;'
```

### Time-based Color Changes
```yaml
# Different colors at different times
automation:
  - trigger:
      - platform: time
        at: "06:00:00"
    action:
      - light.turn_on:
          id: system_status_led
          rgb: [255, 165, 0]  # Orange for morning
    condition:
      - lambda: 'return (App.get_app_state() & (STATUS_LED_ERROR | STATUS_LED_WARNING)) == 0;'
      
  - trigger:
      - platform: time
        at: "20:00:00"
    action:
      - light.turn_on:
          id: system_status_led
          rgb: [0, 0, 139]  # Dark blue for evening
    condition:
      - lambda: 'return (App.get_app_state() & (STATUS_LED_ERROR | STATUS_LED_WARNING)) == 0;'
```

## 🔧 Technical Details

### ESPHome Compatibility

This component uses the exact same flags and timing as ESPHome's internal `status_led`:

```cpp
// Uses the same flags as internal status_led
uint32_t app_state = App.get_app_state();
if ((app_state & STATUS_LED_ERROR) != 0u) { ... }    // Bit 4 (0x10)
if ((app_state & STATUS_LED_WARNING) != 0u) { ... }  // Bit 3 (0x08)

// Exact timing match
// Error: 250ms period, 150ms on (60% duty cycle)
millis() % 250u < 150u

// Warning: 1500ms period, 250ms on (17% duty cycle)  
millis() % 1500u < 250u
```

### Priority System

- **ERROR** (bit 4) takes precedence over **WARNING** (bit 3)
- Status indications always override user control
- User control resumes immediately when status clears

### Memory Footprint

- **RAM Usage**: ~100 bytes (minimal state tracking)
- **Flash Usage**: ~4KB (compiled component)
- **CPU Overhead**: Minimal (state checks only in loop)

## 🆚 Comparison: Simple vs Full Version

| Feature | Simple Version | Full Version |
|---------|----------------|-------------|
| **Status States** | Error, Warning only | Error, Warning, Boot, WiFi, API, OTA |
| **Configuration** | Minimal (like vanilla) | Event-driven with many options |
| **Effects** | Blink only | None, Blink, Pulse |
| **Manual Control** | Full RGB when no status | Full RGB + priority management |
| **Complexity** | Very simple | Advanced |
| **Use Case** | Basic status indication | Advanced status visualization |
| **Learning Curve** | None | Moderate |

## 📁 Files Structure

```
components/
└── rgb_status_led_simple/
    ├── __init__.py                    # Python configuration
    ├── rgb_status_led_simple.h         # C++ header
    ├── rgb_status_led_simple.cpp       # C++ implementation
    ├── example-simple.yaml            # Usage example
    └── README.md                       # This file
```

## 🎯 When to Use Simple Version

**Perfect for:**
- Basic status monitoring (errors/warnings only)
- Users who want vanilla status_led behavior with RGB
- Projects where manual color control is important
- Minimal configuration preference
- Learning ESPHome component development

**Consider Full Version for:**
- Advanced status visualization
- Connection state tracking (WiFi, API)
- OTA progress indication
- Complex priority management
- Production deployments with comprehensive monitoring

## 📚 Additional Resources

- [ESPHome Documentation](https://esphome.io/)
- [Internal status_led Component](https://esphome.io/components/status_led)
- [Full RGB Status LED Component](https://github.com/Bluscream/esphome-rgb-status-led)
- [ESPHome External Components](https://esphome.io/components/external_components/)
