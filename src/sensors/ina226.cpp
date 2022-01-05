#include "ina226.h"

#include "sensesp.h"
//#include "i2c_tools.h"
#include <RemoteDebug.h>

// INA219 represents an ADAfruit (or compatible) INA219 High Side DC Current
// Sensor.
INA226x::INA226x(uint8_t addr)
    : Sensor() {
  // Shunt resistance in Micro-Ohm, e.g. 100000 is 0.1 Ohm
  //const uint32_t SHUNT_MICRO_OHM = 1000000;  ///< Shunt resistance in Micro-Ohm, e.g. 100000 is 0.1 Ohm
  const uint32_t SHUNT_MICRO_OHM = 100000;  ///< Shunt resistance in Micro-Ohm, e.g. 100000 is 0.1 Ohm

  // Max expected amps, clamped from 1A to a max of 1022A
  //const uint16_t MAXIMUM_AMPS = 2;          ///< Max expected amps, clamped from 1A to a max of 1022A
  const uint16_t MAXIMUM_AMPS = 1;          ///< Max expected amps, clamped from 1A to a max of 1022A

  ada_ina226_ = new INA_Class();

  ada_ina226_->begin(MAXIMUM_AMPS, SHUNT_MICRO_OHM, addr);
  ada_ina226_->setBusConversion(8500);             // Maximum conversion time 8.244ms
  ada_ina226_->setShuntConversion(8500);           // Maximum conversion time 8.244ms
  ada_ina226_->setAveraging(128);                  // Average each reading n-times
  ada_ina226_->setMode(INA_MODE_CONTINUOUS_BUS);   // Bus measured continuously
}

// INA219Value reads and outputs the specified type of value of a INA219 sensor
INA226Value::INA226Value(INA226x* ina226,
                         uint read_delay, String config_path)
    : NumericSensor(config_path),
      ina226_{ina226},
      read_delay_{read_delay} {
  load_configuration();
}

void INA226Value::enable() {
  app.onRepeat(read_delay_, [this]() {
  output = ina226_->ada_ina226_->getBusMilliVolts(0) / 1000.0;
    notify();
  });
}

void INA226Value::get_configuration(JsonObject& root) {
  root["read_delay"] = read_delay_;
}

static const char SCHEMA[] PROGMEM = R"###({
    "type": "object",
    "properties": {
        "read_delay": { "title": "Read delay", "type": "number", "description": "The time, in milliseconds, between each read of the input" }
    }
  })###";

String INA226Value::get_config_schema() { return FPSTR(SCHEMA); }

bool INA226Value::set_configuration(const JsonObject& config) {
  String expected[] = {"read_delay"};
  for (auto str : expected) {
    if (!config.containsKey(str)) {
      return false;
    }
  }
  read_delay_ = config["read_delay"];
  return true;
}
