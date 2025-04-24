#pragma once
#include "esphome.h"
#include <IRremoteESP8266.h>
#include <IRsend.h>
#include <ir_Voltas.h>

namespace esphome {
namespace voltas {

class VoltasClimate : public Component, public climate::Climate {
 public:
  VoltasClimate(uint16_t tx_pin);
  void setup() override;
  void loop() override {}
  climate::ClimateTraits traits() override;
  void control(const climate::ClimateCall &call) override;
 private:
  uint16_t tx_pin_;
  optional<IRVoltas> ir_;
};

}  // namespace voltas
}  // namespace esphome
