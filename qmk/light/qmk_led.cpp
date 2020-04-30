#include "esphome/core/log.h"

#include "esphome/components/qmk/qmk_hub.h"
#include "qmk_led.h"

namespace esphome {
namespace qmk {

static const char* TAG = "qmk";

void QMKLED::setup() {}

light::LightTraits QMKLED::get_traits() {
  auto traits = light::LightTraits();
  traits.set_supports_brightness(true);
  traits.set_supports_rgb(true);
  return traits;
}
void QMKLED::write_state(light::LightState *state) {
  if (this->parent_ == nullptr) { 
    ESP_LOGD(TAG, "Can't send LED state without a parent QMK hub");
    return; 
  }

  float fred, fgreen, fblue;
  state->current_values_as_rgb(&fred, &fgreen, &fblue);

  uint8_t red = round(fred * 255.0);
  uint8_t green = round(fgreen * 255.0);
  uint8_t blue = round(fblue * 255.0);

  this->parent_->send_to_keyboard(this->led_id_, red, green, blue);
}

void QMKLED::set_led_id(uint8_t led_id) {
  this->led_id_ = led_id;
}
void QMKLED::set_parent_hub(QMKHub *parent) {
  this->parent_ = parent;
}

}
}
