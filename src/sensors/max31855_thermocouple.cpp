#include "max31855_thermocouple.h"

#include "Arduino.h"
#include "sensesp.h"

MAX31855Thermocouple::MAX31855Thermocouple(uint read_delay, String config_path)
    : NumericSensor(config_path),
      read_delay_{read_delay} {
  load_configuration();
  max31855_ = new Adafruit_MAX31855(D7, D6, D5);
  if (!max31855_->begin()) {
    sensor_detected_ = false;
    debugW("No MAX31855 detected: check wiring.");
    return;
  }
}

void MAX31855Thermocouple::enable() {
  // Must be at least 500 to allow time for temperature "conversion".
  if (!sensor_detected_) {
    debugE("MAX31855 not enabled: no sensor detected.");
    return;
  }
  else {
    if (read_delay_ < 500) {
      read_delay_ = 500;
    }
    app.onRepeat(read_delay_, [this]() {
      max31855_->readInternal();
      app.onDelay(450, [this]() {
        double temp = max31855_->readCelsius();
        if (isnan(temp)) {
          debugW("Check your thermocouple");
          temp = 0.0;
        }
        else {
          // Convert to Kelvins
          temp = temp + 273.15;
        }
        this->emit((float)temp);
      });
    });
  }
}

void MAX31855Thermocouple::get_configuration(JsonObject& root) {
  root["read_delay"] = read_delay_;
};

static const char SCHEMA[] PROGMEM = R"###({
    "type": "object",
    "properties": {
        "read_delay": { "title": "Read delay", "type": "number", "description": "Number of milliseconds between each thermocouple read " }
    }
  })###";

String MAX31855Thermocouple::get_config_schema() { return FPSTR(SCHEMA); }

bool MAX31855Thermocouple::set_configuration(const JsonObject& config) {
  String expected[] = {"read_delay"};
  for (auto str : expected) {
    if (!config.containsKey(str)) {
      return false;
    }
  }
  read_delay_ = config["read_delay"];
  return true;
}
