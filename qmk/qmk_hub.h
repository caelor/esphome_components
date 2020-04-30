#pragma once

#include "esphome/core/component.h"
#include "esphome/components/uart/uart.h"

namespace esphome {
namespace qmk {

class QMKDataReceiver {
  public:
    virtual void process(uint8_t data);
};


class QMKHub : public PollingComponent, public uart::UARTDevice {
  protected:
    std::vector<QMKDataReceiver *> keys_;

  public:
    // PollingComponent methods
    void setup() override;
    void loop() override;
    void update() override;

    // QMKDataServer methods
    void register_receiver(QMKDataReceiver *sensor);
    void send_to_keyboard(uint8_t b1, uint8_t b2, uint8_t b3, uint8_t b4);
};

} // namespace qmk
} // namespace esphome