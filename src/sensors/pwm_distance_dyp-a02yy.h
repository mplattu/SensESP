#ifndef _PWM_DISTANCE_DYPA02YY_H_
#define _PWM_DISTANCE_DYPA02YY_H_

#include "sensor.h"
#include <Arduino.h>
/**
 * @brief Reads a DYP-A02YY PWM sensor which has only data line. The code
 * is based on https://swanrobotics.com/projects/gp2d12_project/
 *
 * @param input_pin The pin that reads the measurement (defaults to A0)
 *
 * @param read_delay The time between reads of the sensor, in ms. Defaults to 1000.
 *
 * @param config_path The path to configure read_delay in the Config UI.
 **/
class PwmDistanceDypa02yy : public NumericSensor {
 public:
  PwmDistanceDypa02yy(
    int8_t input_pin = A0,
    uint read_delay = 1000,
    String config_path = ""
  );
  void enable() override final;

 private:
  int8_t input_pin;
  uint read_delay;
  int16_t bilge_zero_level;
  void update();
  virtual void get_configuration(JsonObject& doc) override;
  virtual bool set_configuration(const JsonObject& config) override;
  virtual String get_config_schema() override;
};

#endif
