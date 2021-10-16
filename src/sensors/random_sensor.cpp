#include "random_sensor.h"

#include "Arduino.h"
#include "sensesp.h"

RandomSensor::RandomSensor(uint min_limit, uint max_limit, uint read_delay, String config_path)
    : NumericSensor(config_path),
      min_limit{min_limit},
      max_limit{max_limit},
      read_delay{read_delay} {
  this->current_value = min_limit;
  this->increment = 1;
  load_configuration();
}

void RandomSensor::update() {
  // Possibility 1:50 to change the course of the value
  long randomNumber = random(50);
  if (randomNumber < 1) {
    this->increment = - this->increment;
  }

  this->current_value = this->current_value + this->increment;

  if (this->current_value < this->min_limit) {
    this->current_value = this->min_limit;
    this->increment = - this->increment;
  }
  if (this->current_value > this->max_limit) {
    this->current_value = this->max_limit;
    this->increment = - this->increment;
  }

  this->emit(this->current_value);
}

void RandomSensor::enable() {
  app.onRepeat(read_delay, [this]() { this->update(); });
}

void RandomSensor::get_configuration(JsonObject& root) {
  root["min_limit"] = min_limit;
  root["max_limit"] = max_limit;
  root["read_delay"] = read_delay;
};

static const char SCHEMA[] PROGMEM = R"###({
    "type": "object",
    "properties": {
        "min_limit": { "title": "Minimum number", "type": "number", "description": "Minimum random number" },
        "max_limit": { "title": "Maximum number", "type": "number", "description": "Maximum random number" },
        "read_delay": { "title": "Read delay", "type": "number", "description": "Number of milliseconds between each new random number" }
    }
  })###";

String RandomSensor::get_config_schema() { return FPSTR(SCHEMA); }

bool RandomSensor::set_configuration(const JsonObject& config) {
  String expected[] = {"read_delay"};
  for (auto str : expected) {
    if (!config.containsKey(str)) {
      return false;
    }
  }
  read_delay = config["read_delay"];
  min_limit = config["min_limit"];
  max_limit = config["max_limit"];
  return true;
}
