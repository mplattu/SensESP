#ifndef SENSOR_MAIN
#define SENSOR_MAIN

#ifdef SENSOR_TYPE_RANDOM
#include "sensors/random_sensor.h"
#endif

#include <Arduino.h>

#include "sensesp_app.h"
#include "sensesp_app_builder.h"
#include "signalk/signalk_output.h"

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
    ->set_hostname(HOSTNAME)
    ->set_sk_server(SERVER_ADDRESS, SERVER_PORT) // Don't specify server address or port
    ->set_wifi(WIFI_NAME, WIFI_PASS)
    //->set_led_pin(13);
//    ->set_led_blinker(true, 1000, 2500, 4000);
    ->get_app();

#ifdef SENSOR_RANDOM_ENGINE_PORT
  const char* sk_path = "propulsion.port.revolutions";
  const char* analog_in_config_path = "/random_sensor";

  SKMetadata* metadata = new SKMetadata();
  metadata->units_ = "Hz";
  metadata->description_ = "Propeller Revolutions Port Side";
  metadata->display_name_ = "Propeller Port Side";
  metadata->short_name_ = "Propeller Port";

  uint min_limit = 0;
  uint max_limit = 4000;
  uint read_delay = 500;

  auto* random_sensor = new RandomSensor(min_limit, max_limit, read_delay, analog_in_config_path);

  random_sensor->connect_to(new SKOutputNumber(sk_path, analog_in_config_path, metadata));
#endif

#ifdef SENSOR_RANDOM_ENGINE_STB
  const char* sk_path = "propulsion.stb.revolutions";
  const char* analog_in_config_path = "/random_sensor";

  SKMetadata* metadata = new SKMetadata();
  metadata->units_ = "Hz";
  metadata->description_ = "Propeller Revolutions Starboard Side";
  metadata->display_name_ = "Propeller Stb Side";
  metadata->short_name_ = "Propeller Stb";

  uint min_limit = 0;
  uint max_limit = 4000;
  uint read_delay = 500;

  auto* random_sensor = new RandomSensor(min_limit, max_limit, read_delay, analog_in_config_path);

  random_sensor->connect_to(new SKOutputNumber(sk_path, analog_in_config_path, metadata));
#endif

  // Start the SensESP application running
  sensesp_app->enable();
});

#endif
