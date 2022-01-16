#ifndef _MAX31855_thermocouple_H_
#define _MAX31855_thermocouple_H_
#include "Adafruit_MAX31855.h"
#include "sensor.h"


/**
 * @brief Reads the temperature from an Adafruit MAX31855 (or compatible) thermocouple
 * breakout board. Communicates with the ESP via SPI.
 *
 * @param read_delay How often to read the sensor, in ms.
 * @param config_path The path to the sensor in the Config UI.
 **/
class MAX31855Thermocouple : public NumericSensor {
 public:
  MAX31855Thermocouple(uint read_delay, String config_path = "");

  Adafruit_MAX31855* max31855_;
  void enable() override final;

 private:
  uint read_delay_;
  bool sensor_detected_ = true;
  virtual void get_configuration(JsonObject& doc) override;
  virtual bool set_configuration(const JsonObject& config) override;
  virtual String get_config_schema() override;
};

#endif
