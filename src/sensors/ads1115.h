#ifndef _ads1115_H_
#define _ads1115_H_

#include <ADS1115_WE.h>
#include <Wire.h>

#include "sensor.h"

class ADS1115 : public NumericSensor {
public:
 ADS1115(uint pin = 0, float voltage_divider_multiplier = 1.0, uint read_delay = 200, String config_path = "");
 void enable() override final;

private:
 uint pin;
 float multiplier;
 uint read_delay;
 ADS1115_WE adc;
 bool adc_initialized;
 virtual void get_configuration(JsonObject& doc) override;
 virtual bool set_configuration(const JsonObject& config) override;
 virtual String get_config_schema() override;
 void update();
};


#endif
