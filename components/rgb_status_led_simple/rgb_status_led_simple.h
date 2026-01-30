#pragma once

#include "esphome/core/component.h"
#include "esphome/core/hal.h"
#include "esphome/components/output/float_output.h"
#include "esphome/components/light/light_output.h"
#include "esphome/core/application.h"

namespace esphome {
namespace rgb_status_led_simple {

class RGBStatusLEDSimple : public light::LightOutput, public Component {
 public:
  RGBStatusLEDSimple();

  // Component lifecycle
  void setup() override;
  void dump_config() override;
  void loop() override;
  float get_setup_priority() const override;
  float get_loop_priority() const override;

  // Light output interface
  light::LightTraits get_traits() override;
  void write_state(light::LightState *state) override;

  // Configuration methods
  void set_red_output(output::FloatOutput *output) { red_output_ = output; }
  void set_green_output(output::FloatOutput *output) { green_output_ = output; }
  void set_blue_output(output::FloatOutput *output) { blue_output_ = output; }
  
  void set_error_color(float r, float g, float b) { error_color_ = {r, g, b}; }
  void set_warning_color(float r, float g, float b) { warning_color_ = {r, g, b}; }
  
  void set_error_blink_speed(uint32_t speed) { error_blink_speed_ = speed; }
  void set_warning_blink_speed(uint32_t speed) { warning_blink_speed_ = speed; }
  void set_brightness(float brightness) { brightness_ = brightness; }

 protected:
  /// @brief Tag for logging
  static const char *const TAG;

  // Hardware output components
  output::FloatOutput *red_output_{nullptr};
  output::FloatOutput *green_output_{nullptr};
  output::FloatOutput *blue_output_{nullptr};

  /**
   * @brief RGB color structure
   */
  struct RGBColor {
    float r, g, b;
    RGBColor(float red = 0, float green = 0, float blue = 0) : r(red), g(green), b(blue) {}
  };
  
  // Color definitions for status states
  RGBColor error_color_{1.0f, 0.0f, 0.0f};     ///< Red for errors
  RGBColor warning_color_{1.0f, 0.5f, 0.0f};   ///< Orange for warnings

  // Timing configuration - matches ESPHome internal status_led exactly
  uint32_t error_blink_speed_{250};     ///< Error blink period in milliseconds
  uint32_t warning_blink_speed_{1500};  ///< Warning blink period in milliseconds
  float brightness_{1.0f};              ///< Global brightness multiplier (0.0 to 1.0)

  // State management
  bool is_blink_on_{false};             ///< Current blink state (on/off)
  bool manual_control_{false};          ///< Whether manual control is active
  RGBColor manual_color_{0.0f, 0.0f, 0.0f}; ///< Manually set color
  float manual_brightness_{1.0f};       ///< Manually set brightness

  // Internal methods
  void set_rgb_output_(const RGBColor &color, float brightness_scale = 1.0f);
  void set_rgb_output_(float r, float g, float b, float brightness_scale = 1.0f);
};

}  // namespace rgb_status_led_simple
}  // namespace esphome