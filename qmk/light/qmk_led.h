#pragma once

#include "esphome/core/component.h"
#include "esphome/components/light/light_output.h"
#include "esphome/components/qmk/qmk_hub.h"

namespace esphome {
namespace qmk {

class QMKLED : public Component, public light::LightOutput {
  protected:
    uint8_t led_id_;
    QMKHub *parent_ = nullptr;
  public:
    void setup() override;
    light::LightTraits get_traits() override;
    void write_state(light::LightState *state) override;

    void set_led_id(uint8_t key_id);
    void set_parent_hub(QMKHub *parent);
};

} // namespace qmk
} // namespace esphome