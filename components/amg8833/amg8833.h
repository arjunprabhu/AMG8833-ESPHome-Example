#pragma once

#include "esphome/core/component.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/i2c/i2c.h"

namespace esphome {
namespace amg8833 {

// AMG8833 Register addresses
static const uint8_t AMG88xx_PIXEL_OFFSET = 0x80;
static const uint8_t AMG88xx_THERMISTOR_LOW = 0x0E;
static const uint8_t AMG88xx_THERMISTOR_HIGH = 0x0F;
static const uint8_t AMG88xx_POWER_CONTROL = 0x00;
static const uint8_t AMG88xx_NORMAL_MODE = 0x00;

class AMG8833Component : public PollingComponent, public i2c::I2CDevice {
 public:
  void setup() override;
  void update() override;
  void dump_config() override;
  float get_setup_priority() const override { return setup_priority::DATA; }

  void set_min_temperature_sensor(sensor::Sensor *sensor) { min_temperature_sensor_ = sensor; }
  void set_max_temperature_sensor(sensor::Sensor *sensor) { max_temperature_sensor_ = sensor; }
  void set_average_temperature_sensor(sensor::Sensor *sensor) { average_temperature_sensor_ = sensor; }
  void set_thermistor_sensor(sensor::Sensor *sensor) { thermistor_sensor_ = sensor; }

 protected:
  sensor::Sensor *min_temperature_sensor_{nullptr};
  sensor::Sensor *max_temperature_sensor_{nullptr};
  sensor::Sensor *average_temperature_sensor_{nullptr};
  sensor::Sensor *thermistor_sensor_{nullptr};
  
  float read_thermistor_();
  void read_pixels_(float *pixels);
  int16_t read_16bit_(uint8_t reg);
};

}  // namespace amg8833
}  // namespace esphome