#pragma once

#include "esphome/core/component.h"
#include "esphome/components/binary_sensor/binary_sensor.h"
#include "esphome/components/qmk/qmk_hub.h"

#define QMK_SWITCHES 32

namespace esphome {
namespace qmk {

class QMKKey : public Component, public binary_sensor::BinarySensor, public QMKDataReceiver {
  protected:
    uint8_t key_id_;
  public:
    void setup() override;
    void process(uint8_t data) override;

    void set_key_id(uint8_t key_id);
};

} // namespace qmk
} // namespace esphome