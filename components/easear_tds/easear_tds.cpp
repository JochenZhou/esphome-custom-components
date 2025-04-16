#include "easear_tds.h"
#include "esphome/core/log.h"

namespace esphome {
namespace easear_tds {

static const char *const TAG = "easear_tds";

void EASEARTDSComponent::dump_config() {
  ESP_LOGCONFIG(TAG, "EASEAR TDS Sensor:");
  LOG_SENSOR("  ", "TDS Channel 1", source_tds_1_);
  LOG_SENSOR("  ", "Temperature Channel 1", temperature_1_);
  LOG_SENSOR("  ", "TDS Channel 2", source_tds_2_);
  LOG_SENSOR("  ", "Temperature Channel 2", temperature_2_);
}

void EASEARTDSComponent::update() {
  if (current_channel_ == 1) {
    write_array(CH1_QUERY_CMD, sizeof(CH1_QUERY_CMD));
    current_channel_ = 2;
  } else {
    write_array(CH2_QUERY_CMD, sizeof(CH2_QUERY_CMD));
    current_channel_ = 1;
  }
  flush();
}

void EASEARTDSComponent::loop() {
  const uint32_t now = millis();
  static uint32_t last_rx_time = 0;

  while (available()) {
    uint8_t byte = read();
    rx_buffer_.push_back(byte);
    last_rx_time = now;

    // Frame validation
    if (rx_buffer_.size() == 1 && rx_buffer_[0] != 0x55) {
      rx_buffer_.clear();
      continue;
    }

    // Process complete frame
    if (rx_buffer_.size() == 11) {
      uint8_t checksum = 0;
      for (int i = 0; i < 10; i++) checksum += rx_buffer_[i];

      if (checksum == rx_buffer_[10]) {
        float tds = (rx_buffer_[5] + rx_buffer_[4] * 256) * 0.1f * 0.47f;
        float temp = (rx_buffer_[7] + rx_buffer_[6] * 256) * 0.1f;

        if (rx_buffer_[3] == 1) {
          if (source_tds_1_ != nullptr) source_tds_1_->publish_state(tds);
          if (temperature_1_ != nullptr) temperature_1_->publish_state(temp);
        } else {
          if (source_tds_2_ != nullptr) source_tds_2_->publish_state(tds);
          if (temperature_2_ != nullptr) temperature_2_->publish_state(temp);
        }
      } else {
        ESP_LOGE(TAG, "Checksum error on channel %d", current_channel_);
      }
      rx_buffer_.clear();
    }
  }

  // Timeout handling
  if (!rx_buffer_.empty() && (now - last_rx_time > 1000)) {
    rx_buffer_.clear();
  }
}

}  // namespace easear_tds
}  // namespace esphome