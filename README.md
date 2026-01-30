# ESPHome RGB Status LED Components

Two RGB status LED components for ESPHome with different complexity levels.

## 🎯 Components

### 🚀 RGB Status LED (Full)
**Component:** `rgb_status_led`

Advanced event-driven status monitoring with 11 states:
- Error, Warning, Boot, WiFi, API, OTA, etc.
- Multiple effects: None, Blink, Pulse
- Event-driven YAML configuration
- Full customization per state

**Use:** Advanced monitoring, production deployments

### 🔧 RGB Status LED Simple  
**Component:** `rgb_status_led_simple`

Minimal component like vanilla `status_led`:
- Error & Warning states only
- Exact vanilla timing & behavior
- Manual RGB control when idle
- Minimal configuration

**Use:** Basic monitoring, learning, simple setups

## 🚀 Quick Install

```yaml
external_components:
  - source: github://Bluscream/esphome-status-led-rgb@main
    components: [rgb_status_led, rgb_status_led_simple]

# Choose one:
light:
  - platform: rgb_status_led        # Full version
    # OR
  - platform: rgb_status_led_simple # Simple version
    id: system_status_led
    red: out_led_r
    green: out_led_g
    blue: out_led_b
```

## 📋 Quick Comparison

| Feature | Full | Simple |
|---------|------|--------|
| **States** | 11 (Error, Warning, Boot, WiFi, API, OTA...) | 2 (Error, Warning only) |
| **Effects** | None, Blink, Pulse | Blink only |
| **Config** | Event-driven | Minimal like vanilla |
| **Learning** | Moderate | None |
| **Memory** | ~200 bytes | ~100 bytes |
| **Use Case** | Advanced monitoring | Basic monitoring |

## 🎯 Which to Use?

**Choose Full (`rgb_status_led`) if:**
- ✅ Need comprehensive status monitoring
- ✅ Want WiFi/API/OTA indication
- ✅ Need multiple visual effects
- ✅ Production deployment

**Choose Simple (`rgb_status_led_simple`) if:**
- ✅ Just need error/warning indication
- ✅ Want vanilla status_led behavior with RGB
- ✅ Manual color control is important
- ✅ Minimal configuration preferred
- ✅ Learning ESPHome components

## 🔧 Event Automations

Both components need the same automations:

```yaml
wifi:
  on_connect: 
    then:
      - lambda: 'id(system_status_led).set_wifi_connected(true);'
  on_disconnect: 
    then:
      - lambda: 'id(system_status_led).set_wifi_connected(false);'

api:
  on_client_connected:
    then:
      - lambda: 'id(system_status_led).set_api_connected(true);'
  on_client_disconnected:
    then:
      - lambda: 'id(system_status_led).set_api_connected(false);'

ota:
  on_begin:
    then:
      - lambda: 'id(system_status_led).set_ota_begin();'
  on_progress:
    then:
      - lambda: 'id(system_status_led).set_ota_progress();'
  on_end:
    then:
      - lambda: 'id(system_status_led).set_ota_end();'
  on_error:
    then:
      - lambda: 'id(system_status_led).set_ota_error();'
```

## 📁 Repository Structure

```
esphome-status-led-rgb/
├── components/
│   ├── rgb_status_led/           # Full version
│   └── rgb_status_led_simple/    # Simple version
└── README.md                      # This file
```

## 🔧 Technical Details

Both components use ESPHome's internal flags:
- **Error**: `STATUS_LED_ERROR` (bit 4) - Fast blink (250ms, 60% duty)
- **Warning**: `STATUS_LED_WARNING` (bit 3) - Slow blink (1500ms, 17% duty)

## 📚 More Info

- **Full Component Docs:** `components/rgb_status_led_simple/README.md`
- **Simple Component Example:** `components/rgb_status_led_simple/example-simple.yaml`
- **ESPHome Documentation:** https://esphome.io/

## 📄 License

MIT License - see LICENSE file for details.
