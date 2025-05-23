#pragma once
#include "esphome.h"
#include <IRremoteESP8266.h>
#include <IRsend.h>
#include <ir_Voltas.h>

namespace esphome {
namespace voltas {

class VoltasClimate : public Component, public climate::Climate {
 public:
  explicit VoltasClimate(uint16_t tx_pin)      // tx_pin supplied from YAML
      : tx_pin_(tx_pin), ir_(tx_pin) {}

  void setup() override {
    ir_.begin();
    publish_state();        // ← add this
  }
  void loop()  override {}                         // nothing periodic

  // -------- advertised features --------
  climate::ClimateTraits traits() override {
    auto traits = climate::ClimateTraits();
    traits.set_supported_modes({
      climate::CLIMATE_MODE_OFF,
      climate::CLIMATE_MODE_COOL,
      climate::CLIMATE_MODE_HEAT,
      climate::CLIMATE_MODE_DRY,
      climate::CLIMATE_MODE_FAN_ONLY});
    traits.set_supported_fan_modes({
      climate::CLIMATE_FAN_AUTO,
      climate::CLIMATE_FAN_LOW,
      climate::CLIMATE_FAN_MEDIUM,
      climate::CLIMATE_FAN_HIGH});
    traits.set_supported_swing_modes({
      climate::CLIMATE_SWING_OFF,
      climate::CLIMATE_SWING_VERTICAL});
    traits.set_visual_min_temperature(16);
    traits.set_visual_max_temperature(30);
    traits.set_visual_temperature_step(1);
    return traits;
  }

  // -------- apply commands from Home-Assistant --------
  void control(const climate::ClimateCall &call) override {
    // update local state
    if (call.get_mode())                this->mode = *call.get_mode();
    if (call.get_target_temperature())  this->target_temperature = *call.get_target_temperature();
    if (call.get_fan_mode())            this->fan_mode = *call.get_fan_mode();
    if (call.get_swing_mode())          this->swing_mode = *call.get_swing_mode();

    // map to IRVoltas
    ir_.setModel(kVoltas122LZF);
    ir_.setPower(this->mode != climate::CLIMATE_MODE_OFF);

    switch (this->mode) {
      case climate::CLIMATE_MODE_COOL:      ir_.setMode(kVoltasCool); break;
      case climate::CLIMATE_MODE_HEAT:      ir_.setMode(kVoltasHeat); break;
      case climate::CLIMATE_MODE_DRY:       ir_.setMode(kVoltasDry);  break;
      case climate::CLIMATE_MODE_FAN_ONLY:  ir_.setMode(kVoltasFan);  break;
      default:                              ir_.setMode(kVoltasCool); break;
    }

    ir_.setTemp(static_cast<uint8_t>(roundf(this->target_temperature)));

    if      (this->fan_mode == climate::CLIMATE_FAN_LOW)    ir_.setFan(kVoltasFanLow);
    else if (this->fan_mode == climate::CLIMATE_FAN_MEDIUM) ir_.setFan(kVoltasFanMed);
    else if (this->fan_mode == climate::CLIMATE_FAN_HIGH)   ir_.setFan(kVoltasFanHigh);
    else                                                    ir_.setFan(kVoltasFanAuto);

    ir_.setSwingV(this->swing_mode == climate::CLIMATE_SWING_VERTICAL);

    // send burst twice for reliability
    for (int i = 0; i < 2; i++) { ir_.send(); delay(40); }

    publish_state();            // report updated state to HA
  }

 protected:
  uint16_t tx_pin_;
  IRVoltas ir_;
};

}  // namespace voltas
}  // namespace esphome