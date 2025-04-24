#include "voltas_climate.h"

namespace esphome {
namespace voltas {

VoltasClimate::VoltasClimate(uint16_t tx_pin) : tx_pin_(tx_pin) {}

void VoltasClimate::setup() {
  ir_.emplace(tx_pin_);
  ir_->begin();
}

climate::ClimateTraits VoltasClimate::traits() { …same code… }

void VoltasClimate::control(const climate::ClimateCall &call) { …same code… }

}  // namespace voltas
}  // namespace esphome
