#include "rgb_status_led_simple.h"
#include "esphome/core/log.h"

namespace esphome {
namespace rgb_status_led_simple {

static const char *const TAG = "rgb_status_led_simple";

RGBStatusLEDSimple::RGBStatusLEDSimple() = default;

void RGBStatusLEDSimple::setup() {
  ESP_LOGCONFIG(TAG, "Setting up RGB Status LED Simple...");
  this->set_rgb_output_(0.0f, 0.0f, 0.0f);
  ESP_LOGCONFIG(TAG, "RGB Status LED Simple setup completed");
}

void RGBStatusLEDSimple::dump_config() {
  ESP_LOGCONFIG(TAG, "RGB Status LED Simple:");
  LOG_PIN("  Red Pin: ", this->red_output_);
  LOG_PIN("  Green Pin: ", this->green_output_);
  LOG_PIN("  Blue Pin: ", this->blue_output_);
  ESP_LOGCONFIG(TAG, "  Supports manual control when no status is active");
}

void RGBStatusLEDSimple::loop() {
  uint32_t app_state = App.get_app_state();
  bool has_status = (app_state & (STATUS_LED_ERROR | STATUS_LED_WARNING)) != 0;
  uint32_t now = millis();

  if (has_status) {
    manual_control_ = false;  // Status takes priority
    if (app_state & STATUS_LED_ERROR) {
      // Fast blink (250ms period, 150ms on)
      bool led_on = (now % 250) < 150;
      set_rgb_output_(error_color_, led_on ? 1.0f : 0.0f);
    } 
    else if (app_state & STATUS_LED_WARNING) {
      // Slow blink (1500ms period, 250ms on)
      bool led_on = (now % 1500) < 250;
      set_rgb_output_(warning_color_, led_on ? 1.0f : 0.0f);
    }
  } 
  else if (manual_control_) {
    // Manual control when no status is active
    set_rgb_output_(manual_color_, manual_brightness_);
  } 
  else {
    // No status and no manual control - turn off
    set_rgb_output_(0.0f, 0.0f, 0.0f, 0.0f);
  }
}

light::LightTraits RGBStatusLEDSimple::get_traits() {
  auto traits = light::LightTraits();
  traits.set_supported_color_modes({light::ColorMode::RGB});
  return traits;
}

void RGBStatusLEDSimple::write_state(light::LightState *state) {
  // Store manual color and brightness
  manual_control_ = true;
  state->current_values_as_rgb(&manual_color_.r, &manual_color_.g, &manual_color_.b, &manual_brightness_);
}

void RGBStatusLEDSimple::set_rgb_output_(const RGBColor &color, float brightness_scale) {
  set_rgb_output_(color.r, color.g, color.b, brightness_scale);
}

void RGBStatusLEDSimple::set_rgb_output_(float r, float g, float b, float brightness_scale) {
  float final_brightness = brightness_ * brightness_scale;
  if (red_output_) red_output_->set_level(r * final_brightness);
  if (green_output_) green_output_->set_level(g * final_brightness);
  if (blue_output_) blue_output_->set_level(b * final_brightness);
}

}  // namespace rgb_status_led_simple
}  // namespace esphome