#include "esphome/core/log.h"

#include "esphome/components/qmk/qmk_hub.h"
#include "qmk_key.h"

namespace esphome {
namespace qmk {

static const char* TAG = "qmk";

void QMKKey::setup() {}
void QMKKey::process(uint8_t data) {
  if (data == 0) { return; }
  if (data > QMK_SWITCHES * 2) { return; }

  if (data > QMK_SWITCHES) {
    if (data - QMK_SWITCHES == this->key_id_) {
      this->publish_state(false); // key release
      ESP_LOGD(TAG, "Key %d released", data - QMK_SWITCHES);
    }
  }
  else {
    if (data == this->key_id_) {
      this->publish_state(true); // key press
      ESP_LOGD(TAG, "Key %d pressed", data);
    }
  }
}

void QMKKey::set_key_id(uint8_t key_id) {
  this->key_id_ = key_id;
}


}
}
