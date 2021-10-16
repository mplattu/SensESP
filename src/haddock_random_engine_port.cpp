#ifndef SENSOR_MAIN
#define SENSOR_MAIN

#include "sensors/random_sensor.h"

#include <Arduino.h>

#include "sensesp_app.h"
#include "sensesp_app_builder.h"
#include "signalk/signalk_output.h"
#include "transforms/linear.h"

// SensESP builds upon the ReactESP framework. Every ReactESP application
// defines an "app" object vs defining a "main()" method.
ReactESP app([]() {

// Some initialization boilerplate when in debug mode...
#ifndef SERIAL_DEBUG_DISABLED
  SetupSerialDebug(115200);
#endif

  // Create the global SensESPApp() object.
  SensESPAppBuilder builder;

  sensesp_app = builder.set_standard_sensors(UPTIME)
    ->set_hostname("RandomSensorEnginePort")
    ->set_sk_server(SERVER_ADDRESS, SERVER_PORT) // Don't specify server address or port
    ->set_wifi(WIFI_NAME, WIFI_PASS)
    //->set_led_pin(13);
//    ->set_led_blinker(true, 1000, 2500, 4000);
    ->get_app();

  // The "Signal K path" identifies this sensor to the Signal K server. Leaving
  // this blank would indicate this particular sensor (or transform) does not
  // broadcast Signal K data.
  // To find valid Signal K Paths that fits your need you look at this link:
  // https://signalk.org/specification/1.4.0/doc/vesselsBranch.html
  const char* sk_path = "propulsion.port.revolutions";

  // The "Configuration path" is combined with "/config" to formulate a URL
  // used by the RESTful API for retrieving or setting configuration data.
  // It is ALSO used to specify a path to the SPIFFS file system
  // where configuration data is saved on the MCU board. It should
  // ALWAYS start with a forward slash if specified. If left blank,
  // that indicates this sensor or transform does not have any
  // configuration to save, or that you're not interested in doing
  // run-time configuration.
  const char* analog_in_config_path = "/random_sensor";

  uint min_limit = 0;
  uint max_limit = 4000;
  uint read_delay = 500;

  auto* random_sensor = new RandomSensor(min_limit, max_limit, read_delay, analog_in_config_path);

  random_sensor->connect_to(new SKOutputNumber(sk_path));

  // Start the SensESP application running
  sensesp_app->enable();
});

#endif
