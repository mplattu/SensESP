#ifndef SENSOR_MAIN
#define SENSOR_MAIN

#ifdef SENSOR_TYPE_BMP280
#include "sensors/deepsleeptimer.h"
#include "sensors/bmp280.h"
#include "transforms/linear.h"
#endif

#ifdef HADDOCK_SENSOR_TYPE_CURRENT
#include "sensors/ads1x15.h"
#include "transforms/linear.h"
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

#ifdef SENSOR_SALON_ENVIRONMENT
  auto* deepsleeptimer = new DeepSleepTimer(300,60, "/Inside/DeepSleepTimer");
  deepsleeptimer
    ->connect_to(new SKOutputNumber("environment.inside.timebeforesleep"));

  auto* bmp280 = new BMP280(0x76);
  const uint read_delay = 1000;            // once per second
  const uint pressure_read_delay = 60000;  // once per minute

  auto* bmp_temperature =
    new BMP280Value(bmp280, BMP280Value::temperature, read_delay, "/Inside/Temperature");
  bmp_temperature
    ->connect_to(new Linear(1, -2.0, "/Inside/TemperatureTransform"))
    ->connect_to(new SKOutputNumber("environment.inside.temperature"));

  auto* bmp_pressure =
    new BMP280Value(bmp280, BMP280Value::pressure, pressure_read_delay, "/Inside/Pressure");
  bmp_pressure
    ->connect_to(new Linear(1, 0, "/Inside/PressureTransform"))
    ->connect_to(new SKOutputNumber("environment.inside.pressure"));
#endif

#ifdef HADDOCK_SENSOR_CURRENT_SOLAR
  SKMetadata* metadata = new SKMetadata();
  metadata->units_ = "A";
  metadata->description_ = "Solar Panel Charging Current";
  metadata->display_name_ = "Current (Solar)";
  metadata->short_name_ = "Current (Solar)";

  adsGain_t gain = GAIN_SIXTEEN;
  ADS1115* ads1115 = new ADS1115(0x48, gain);

  auto* current =
    new ADS1x15RawValue (ads1115, channels_0_1, 500, "/Current/Measurement");
  current
    ->connect_to(new Linear(256.0 / 32768.0, 0, ""))
    ->connect_to(new Linear(0.57, 0, "/Current/LinearTransform"))
    ->connect_to(new SKOutputNumber("electrical.solar.flybridge.current", "/Current/Sk", metadata));
#endif

  // Start the SensESP application running
  sensesp_app->enable();
});

#endif
