#ifndef _deepsleeptimer_sensor_H_
#define _deepsleeptimer_sensor_H_

#include <elapsedMillis.h>

#include "sensor.h"

class DeepSleepTimer : public NumericSensor {
public:
 DeepSleepTimer(uint16_t time_before_sleep = 0, uint16_t time_before_wakeup = 0, String config_path = "");
 void enable() override final;

private:
 uint16_t time_before_sleep;
 uint16_t time_before_wakeup;
 elapsedMillis timer;
 uint read_delay;
 virtual void get_configuration(JsonObject& doc) override;
 virtual bool set_configuration(const JsonObject& config) override;
 virtual String get_config_schema() override;
 void update();
};


#endif
