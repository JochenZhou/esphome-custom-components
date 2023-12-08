#pragma once

#include <utility>

#include "esphome/core/component.h"
#include "esphome/components/uart/uart.h"
#include "esphome/components/sensor/sensor.h"

namespace esphome {
namespace easear_tds {

static const uint8_t URAT_QUERY_COMMAND[] ={0x55,0x07,0x05,0x02,0x00,0x00,0x00,0x63};


class EASEARTDSComponent : public uart::UARTDevice, public PollingComponent {
 public:
  void dump_config() override;
  void update() override;
  void loop() override;
  float get_setup_priority() const override { return setup_priority::DATA; }
  
  void set_source_tds_sensor(sensor::Sensor *source_tds_sensor) { source_tds_sensor_ = source_tds_sensor; }
  void set_temperature_sensor(sensor::Sensor *temperature_sensor) { temperature_sensor_ = temperature_sensor; }

 protected:

  std::vector<uint8_t> bytes;

  sensor::Sensor *source_tds_sensor_{nullptr};
  sensor::Sensor *temperature_sensor_{nullptr};
};


}  // namespace ssw_tds
}  // namespace esphome
