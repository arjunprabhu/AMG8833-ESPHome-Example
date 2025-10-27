#include "amg8833.h"
#include "esphome/core/log.h"

namespace esphome {
namespace amg8833 {

static const char *const TAG = "amg8833";

void AMG8833Component::setup() {
  ESP_LOGCONFIG(TAG, "Setting up AMG8833...");
  
  // Wake up sensor - write 0x00 to power control register
  if (!this->write_byte(AMG88xx_POWER_CONTROL, AMG88xx_NORMAL_MODE)) {
    ESP_LOGE(TAG, "Failed to wake up AMG8833!");
    this->mark_failed();
    return;
  }
  
  delay(50);  // Wait for sensor to wake up
  ESP_LOGCONFIG(TAG, "AMG8833 initialized successfully");
}

int16_t AMG8833Component::read_16bit_(uint8_t reg) {
  uint8_t low, high;
  if (!this->read_byte(reg, &low) || !this->read_byte(reg + 1, &high)) {
    return 0;
  }
  int16_t raw = ((uint16_t)high << 8) | low;
  return raw;
}

float AMG8833Component::read_thermistor_() {
  int16_t raw = this->read_16bit_(AMG88xx_THERMISTOR_LOW);
  // Convert to signed 12-bit value
  if (raw & 0x800) {
    raw = -(raw & 0x7FF);
  }
  return raw * 0.0625;  // 0.0625°C per bit
}

void AMG8833Component::read_pixels_(float *pixels) {
  uint8_t bytes[128];  // 64 pixels * 2 bytes each
  
  if (!this->read_bytes(AMG88xx_PIXEL_OFFSET, bytes, 128)) {
    ESP_LOGE(TAG, "Failed to read pixel data");
    return;
  }
  
  for (int i = 0; i < 64; i++) {
    int16_t raw = ((uint16_t)bytes[i * 2 + 1] << 8) | bytes[i * 2];
    
    // Convert to signed 12-bit value
    if (raw & 0x800) {
      raw = -(raw & 0x7FF);
    }
    
    pixels[i] = raw * 0.25;  // 0.25°C per bit
  }
}

void AMG8833Component::update() {
  float pixels[64];
  this->read_pixels_(pixels);
  
  float min_val = pixels[0];
  float max_val = pixels[0];
  float sum = 0;
  
  for (int i = 0; i < 64; i++) {
    if (pixels[i] < min_val) min_val = pixels[i];
    if (pixels[i] > max_val) max_val = pixels[i];
    sum += pixels[i];
  }
  
  float avg_val = sum / 64.0;
  
  if (this->min_temperature_sensor_ != nullptr)
    this->min_temperature_sensor_->publish_state(min_val);
  if (this->max_temperature_sensor_ != nullptr)
    this->max_temperature_sensor_->publish_state(max_val);
  if (this->average_temperature_sensor_ != nullptr)
    this->average_temperature_sensor_->publish_state(avg_val);
  if (this->thermistor_sensor_ != nullptr)
    this->thermistor_sensor_->publish_state(this->read_thermistor_());
    
  ESP_LOGD(TAG, "Min: %.1f°C, Max: %.1f°C, Avg: %.1f°C", min_val, max_val, avg_val);
}

void AMG8833Component::dump_config() {
  ESP_LOGCONFIG(TAG, "AMG8833:");
  LOG_I2C_DEVICE(this);
  LOG_UPDATE_INTERVAL(this);
  LOG_SENSOR("  ", "Min Temperature", this->min_temperature_sensor_);
  LOG_SENSOR("  ", "Max Temperature", this->max_temperature_sensor_);
  LOG_SENSOR("  ", "Average Temperature", this->average_temperature_sensor_);
  LOG_SENSOR("  ", "Thermistor", this->thermistor_sensor_);
}

}  // namespace amg8833
}  // namespace esphome