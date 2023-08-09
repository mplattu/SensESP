#ifndef _ina226_H_
#define _ina226_H_

#include <INA.h>

#include "sensor.h"

class INA226x : public Sensor {
 public:
  INA226x(uint8_t addr = 0x40, uint32_t shunt_micro_ohm = 100000, uint16_t maximum_amps = 1);
  INA_Class* ada_ina226_;
};


/**
 * @brief INA219Value reads and outputs the specified value of an INA219 sensor.
 *
 * @param ina219 A pointer to an instance of an INA219.
 *
 * @param val_type The type of value you're reading:
 *      TYPE_BUS_VOLTAGE or TYPE_CURRENT. Default is TYPE_BUS_VOLTAGE (0).
 *
 * @param read_delay How often to read the sensor, in ms. Default is 500.
 *
 * @param config_path Path in the Config UI to configure read_delay
 */
class INA226Value : public NumericSensor {
 public:
  INA226Value(INA226x* ina226, uint sensor_type = 0, uint read_delay = 500, String config_path = "");
  void enable() override final;
  INA226x* ina226_;
  static const uint TYPE_BUS_VOLTAGE = 0;
  static const uint TYPE_CURRENT = 1;

 private:
  uint sensor_type_;
  uint read_delay_;
  virtual void get_configuration(JsonObject& doc) override;
  virtual bool set_configuration(const JsonObject& config) override;
  virtual String get_config_schema() override;
};

// FIXME: Uncomment the following once the PIO Xtensa toolchain is updated
// [[deprecated("Use INA219Value instead.")]]
typedef INA226Value INA226value;

#endif