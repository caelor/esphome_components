#include "esphome/core/log.h"
#include "esphome/core/helpers.h"
#include "esphome/core/application.h"
#include "esphome/core/defines.h"

#include "qmk_hub.h"

namespace esphome {
namespace qmk {

static const char* TAG = "qmk";

static uint8_t proto_heartbeat[4] = { 0,0,0,2 };
static uint8_t proto_sync[8] = { 0,0,0,0, 0,0,0,1 };



void QMKHub::setup() {}

void QMKHub::loop() {
  // notification of key presses (+ requests for resync)
  if (this->available() > 0) {
    uint8_t data;
    this->read_byte(&data);

    if (data == 0) {
      // sync reset
      ESP_LOGD(TAG, "Resync requested");
      this->write_array(proto_sync, sizeof(proto_sync));
      ESP_LOGD(TAG, "Resync sent");
    }
    else {
      // keypress
      ESP_LOGD(TAG, "Received QMK message %d", data);
      for (auto *key: this->keys_) {
        key->process(data);
      }
    }
  }
}

void QMKHub::update() {
  this->write_array(proto_heartbeat, sizeof(proto_heartbeat));
  ESP_LOGD(TAG, "Sent heartbeat");
}

void QMKHub::register_receiver(QMKDataReceiver *sensor) {
  this->keys_.push_back(sensor);
}

void QMKHub::send_to_keyboard(uint8_t b1, uint8_t b2, uint8_t b3, uint8_t b4) {
  if (b1 == 0) { return; }

  uint8_t buf[4] = { b1, b2, b3, b4 };
  this->write_array(buf, sizeof(buf));
  ESP_LOGD(TAG, "Sent LED %d state (r=%d, g=%d, b=%d)", b1, b2, b3, b4);
}


}
}
