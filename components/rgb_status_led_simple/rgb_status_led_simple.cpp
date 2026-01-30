#include "rgb_status_led_simple.h"
#include "esphome/core/log.h"
#include "esphome/core/application.h"

namespace esphome {
namespace rgb_status_led_simple {

static const char *const TAG = "rgb_status_led_simple";

void RGBStatusLEDSimple::setup() {
  ESP_LOGCONFIG(TAG, "Setting up RGB Status LED Simple...");
  this->set_rgb_output_(0.0f, 0.0f, 0.0f, 0.0f);  // Start with LED off
  ESP_LOGCONFIG(TAG, "RGB Status LED Simple setup completed");
}

void RGBStatusLEDSimple::dump_config() {
  ESP_LOGCONFIG(TAG, "RGB Status LED Simple:");
  LOG_PIN("  Red Pin: ", this->red_output_);
  LOG_PIN("  Green Pin: ", this->green_output_);
  LOG_PIN("  Blue Pin: ", this->blue_output_);
  ESP_LOGCONFIG(TAG, "  Error Color: R=%.1f%%, G=%.1f%%, B=%.1f%%", 
                error_color_.r * 100, error_color_.g * 100, error_color_.b * 100);
  ESP_LOGCONFIG(TAG, "  Warning Color: R=%.1f%%, G=%.1f%%, B=%.1f%%", 
                warning_color_.r * 100, warning_color_.g * 100, warning_color_.b * 100);
  ESP_LOGCONFIG(TAG, "  Error Blink Speed: %ums", error_blink_speed_);
  ESP_LOGCONFIG(TAG, "  Warning Blink Speed: %ums", warning_blink_speed_);
  ESP_LOGCONFIG(TAG, "  Brightness: %.0f%%", brightness_ * 100);
  ESP_LOGCONFIG(TAG, "  Supports manual control when no status is active");
}

void RGBStatusLEDSimple::loop() {
  uint32_t app_state = App.get_app_state();
  bool has_status = (app_state & (STATUS_LED_ERROR | STATUS_LED_WARNING)) != 0;
  uint32_t now = millis();

  if (has_status) {
    // Status takes priority
    if (app_state & STATUS_LED_ERROR) {
      // Fast blink with error color
      bool led_on = (now % error_blink_speed_) < (error_blink_speed_ * 3 / 5);  // 60% duty cycle
      set_rgb_output_(
        error_color_.r,
        error_color_.g,
        error_color_.b,
        led_on ? 1.0f : 0.0f
      );
    } 
    else if (app_state & STATUS_LED_WARNING) {
      // Slow blink with warning color
      bool led_on = (now % warning_blink_speed_) < (warning_blink_speed_ / 6);  // ~17% duty cycle
      set_rgb_output_(
        warning_color_.r,
        warning_color_.g,
        warning_color_.b,
        led_on ? 1.0f : 0.0f
      );
    }
  } 
  else if (lightstate_ != nullptr) {
    // No status - restore manual state
    bool state;
    lightstate_->current_values_as_binary(&state);
    if (state) {
      // Use the last manual color and brightness
      set_rgb_output_(
        manual_color_.r,
        manual_color_.g,
        manual_color_.b,
        manual_brightness_
      );
    } else {
      // Turn off
      set_rgb_output_(0.0f, 0.0f, 0.0f, 0.0f);
    }
  }
}

light::LightTraits RGBStatusLEDSimple::get_traits() {
  auto traits = light::LightTraits();
  traits.set_supported_color_modes({light::ColorMode::RGB});
  return traits;
}

void RGBStatusLEDSimple::write_state(light::LightState *state) {
  // Store the light state for later use
  lightstate_ = state;
  
  // Update manual color and brightness
  state->current_values_as_rgb(
    &manual_color_.r,
    &manual_color_.g,
    &manual_color_.b,
    &manual_brightness_
  );
  
  // If no status is active, apply the new state immediately
  if ((App.get_app_state() & (STATUS_LED_ERROR | STATUS_LED_WARNING)) == 0) {
    bool binary;
    state->current_values_as_binary(&binary);
    if (binary) {
      set_rgb_output_(
        manual_color_.r,
        manual_color_.g,
        manual_color_.b,
        manual_brightness_
      );
    } else {
      set_rgb_output_(0.0f, 0.0f, 0.0f, 0.0f);
    }
  }
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