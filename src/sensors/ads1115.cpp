#include "ads1115.h"

#include "Arduino.h"
#include "sensesp.h"

ADS1115::ADS1115(uint pin, float voltage_divider_multiplier, uint read_delay, String config_path)
    : NumericSensor(config_path),
      pin{pin},
      multiplier{voltage_divider_multiplier},
      read_delay{read_delay} {
  this->adc = ADS1115_WE();

  Wire.begin();
  if (! this->adc.init()) {
    while(1) {
      debugE("Could not init ADS1115");
      delay(1000);
    }
  }

  this->adc.setVoltageRange_mV(ADS1115_RANGE_4096);
  this->adc.setMeasureMode(ADS1115_CONTINUOUS);

  load_configuration();
}

void ADS1115::update() {
  switch (this->pin) {
    case 0:
      this->adc.setCompareChannels(ADS1115_COMP_0_GND);
      break;
    case 1:
      this->adc.setCompareChannels(ADS1115_COMP_1_GND);
      break;
    case 2:
      this->adc.setCompareChannels(ADS1115_COMP_2_GND);
      break;
    case 3:
      this->adc.setCompareChannels(ADS1115_COMP_3_GND);
      break;
    default:
      debugE("Trying to read an unknown pin %d", this->pin);
      this->emit(-1);
    }

  // Wait until channel comparison has been set
  delay(100);

  float measuredVoltage = 0.0;
  measuredVoltage = this->adc.getResult_V() * this->multiplier;

  this->emit(measuredVoltage);
}

void ADS1115::enable() {
  app.onRepeat(read_delay, [this]() { this->update(); });
}

void ADS1115::get_configuration(JsonObject& root) {
  root["pin"] = pin;
  root["multiplier"] = multiplier;
  root["read_delay"] = read_delay;
};

static const char SCHEMA[] PROGMEM = R"###({
    "type": "object",
    "properties": {
        "pin": { "title": "Pin to read (1-4)", "type": "number", "description": "ADS1115 pin to read (1-4)" },
        "multiplier": { "title": "Multiplier", "type": "number", "description": "Voltage divider multiplier" },
        "read_delay": { "title": "Read delay", "type": "number", "description": "Number of milliseconds between each new random number" }
    }
  })###";

String ADS1115::get_config_schema() { return FPSTR(SCHEMA); }

bool ADS1115::set_configuration(const JsonObject& config) {
  String expected[] = {"read_delay"};
  for (auto str : expected) {
    if (!config.containsKey(str)) {
      return false;
    }
  }
  read_delay = config["read_delay"];
  multiplier = config["multiplier"];
  pin = config["pin"];
  return true;
}
