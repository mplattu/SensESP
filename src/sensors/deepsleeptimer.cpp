#include "deepsleeptimer.h"

#include "Arduino.h"
#include "sensesp.h"

DeepSleepTimer::DeepSleepTimer(uint16_t time_before_sleep, uint16_t time_before_wakeup, String config_path):
  NumericSensor(config_path),
  time_before_sleep{time_before_sleep},
  time_before_wakeup{time_before_wakeup} {

  this->timer = 0;
  this->read_delay = 1000;

  load_configuration();
}

void DeepSleepTimer::update() {
  if (this->time_before_sleep != 0 && this->timer > (this->time_before_sleep*1000)) {
    debugD("DeepSleepTimer puts the device into deepSleep mode for %ld seconds", (long)(this->time_before_wakeup));
    ESP.deepSleep(this->time_before_wakeup * 1000000);
  }

  if (this->timer > this->time_before_sleep*1000) {
    this->emit((this->timer - (this->time_before_sleep * 1000)) / 1000);
  }
  else {
    this->emit(((this->time_before_sleep * 1000) - this->timer) / 1000);
  }
}

void DeepSleepTimer::enable() {
  app.onRepeat(this->read_delay, [this]() { this->update(); });
}

void DeepSleepTimer::get_configuration(JsonObject& root) {
  root["time_before_sleep"] = time_before_sleep;
  root["time_before_wakeup"] = time_before_wakeup;
};

static const char SCHEMA[] PROGMEM = R"###({
    "type": "object",
    "properties": {
        "time_before_sleep": { "title": "Time before sleep", "type": "number", "description": "Time in seconds after sensor is going to deep sleep" },
        "time_before_wakeup": { "title": "Time before wakeup", "type": "number", "description": "Time in seconds after sensor is goit go wake up" }
    }
  })###";

String DeepSleepTimer::get_config_schema() { return FPSTR(SCHEMA); }

bool DeepSleepTimer::set_configuration(const JsonObject& config) {
  time_before_sleep = config["time_before_sleep"];
  time_before_wakeup = config["time_before_wakeup"];
  return true;
}
