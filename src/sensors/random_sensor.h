#ifndef _random_sensor_H_
#define _random_sensor_H_

#include "sensor.h"

class RandomSensor : public NumericSensor {
public:
 RandomSensor(uint min_limit = 0, uint max_limit = 100, uint read_delay = 200, String config_path = "");
 void enable() override final;

private:
 uint min_limit;
 uint max_limit;
 uint read_delay;
 uint current_value;
 int increment;
 virtual void get_configuration(JsonObject& doc) override;
 virtual bool set_configuration(const JsonObject& config) override;
 virtual String get_config_schema() override;
 void update();
};


#endif
