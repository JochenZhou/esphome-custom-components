#pragma once

#include "esphome/core/component.h"
#include "esphome/components/uart/uart.h"
#include "esphome/components/sensor/sensor.h"

namespace esphome {
namespace easear_tds {

static const uint8_t CH1_QUERY_CMD[] = {0x55, 0x07, 0x05, 0x01, 0x00, 0x00, 0x00, 0x62};
static const uint8_t CH2_QUERY_CMD[] = {0x55, 0x07, 0x05, 0x02, 0x00, 0x00, 0x00, 0x63};

class EASEARTDSComponent : public uart::UARTDevice, public PollingComponent {
 public:
  void dump_config() override;
  void update() override;
  void loop() override;
  float get_setup_priority() const override { return setup_priority::DATA; }
  
  // Channel 1
  void set_source_tds_sensor_1(sensor::Sensor *sensor) { source_tds_1_ = sensor; }
  void set_temperature_sensor_1(sensor::Sensor *sensor) { temperature_1_ = sensor; }
  
  // Channel 2
  void set_source_tds_sensor_2(sensor::Sensor *sensor) { source_tds_2_ = sensor; }
  void set_temperature_sensor_2(sensor::Sensor *sensor) { temperature_2_ = sensor; }

 protected:
  std::vector<uint8_t> rx_buffer_;
  uint8_t current_channel_{1};

  sensor::Sensor *source_tds_1_{nullptr};
  sensor::Sensor *temperature_1_{nullptr};
  sensor::Sensor *source_tds_2_{nullptr};
  sensor::Sensor *temperature_2_{nullptr};
};

}  // namespace easear_tds
}  // namespace esphome