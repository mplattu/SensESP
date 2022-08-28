#include "pwm_distance_dyp-a02yy.h"

#include "sensesp.h"

PwmDistanceDypa02yy::PwmDistanceDypa02yy(int8_t input_pin,
                                       uint read_delay, String config_path)
    : NumericSensor(config_path),
      input_pin{input_pin},
      read_delay{read_delay} {
  this->read_delay = read_delay;
  this->input_pin = input_pin;
  pinMode(this->input_pin, INPUT);
  load_configuration();
}

void PwmDistanceDypa02yy::update() {
  uint16_t value = analogRead (this->input_pin);

  if (value < 10) value = 10;
  int16_t distance = (int16_t) ((67870.0 / (value - 3.0)) - 40.0);

  int16_t level = this->bilge_zero_level - distance;

  this->emit(level);
}

void PwmDistanceDypa02yy::enable() {
  app.onRepeat(read_delay, [this]() { this->update(); });
}

void PwmDistanceDypa02yy::get_configuration(JsonObject& root) {
  root["read_delay"] = this->read_delay;
  root["bilge_zero_level"] = this->bilge_zero_level;
};

static const char SCHEMA[] PROGMEM = R"###({
    "type": "object",
    "properties": {
        "read_delay": { "title": "Read delay", "type": "number", "description": "Number of milliseconds between each read " },
        "bilge_zero_level": { "title": "Bilge zero level", "type": "number", "description": "Bilge water zero level (mm from sensor)" }
    }
  })###";

String PwmDistanceDypa02yy::get_config_schema() { return FPSTR(SCHEMA); }

bool PwmDistanceDypa02yy::set_configuration(const JsonObject& config) {
  String expected[] = {"read_delay"};
  for (auto str : expected) {
    if (!config.containsKey(str)) {
      return false;
    }
  }
  this->read_delay = config["read_delay"];
  this->bilge_zero_level = config["bilge_zero_level"];
  return true;
}
