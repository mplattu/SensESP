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

#ifdef HADDOCK_SENSOR_TYPE_VOLTAGE
#include "sensors/ads1115.h"
#include "transforms/linear.h"
#endif

#ifdef HADDOCK_SENSOR_TYPE_TANK
#include "sensors/analog_input.h"
#include "transforms/linear.h"
#include "transforms/map_ratio.h"
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

  SystemStatusLed* systemStatusLed = new SystemStatusLed(LED_BUILTIN);

  sensesp_app = builder.set_standard_sensors()
    ->set_hostname(HOSTNAME)
    ->set_sk_server(SERVER_ADDRESS, SERVER_PORT) // Don't specify server address or port
    ->set_wifi(WIFI_NAME, WIFI_PASS)
    ->set_system_status_led(systemStatusLed)
    ->get_app();

#ifdef SENSOR_SALON_ENVIRONMENT
  auto* deepsleeptimer = new DeepSleepTimer(0, 600, "/Inside/DeepSleepTimer");
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

#ifdef HADDOCK_SENSOR_VOLTAGE_CHARGE
  uint vdiv_r1 = 82000;
  uint vdiv_r2 = 15000;
  float vdiv_multiplier = ((float)(vdiv_r1) + (float)(vdiv_r2)) / (float)(vdiv_r2);
  uint read_delay = 10000;

  SKMetadata* metadata_0 = new SKMetadata();
  metadata_0->units_ = "V";
  metadata_0->description_ = "Battery 1 Voltage";
  metadata_0->display_name_ = "Voltage (Batt 1)";
  metadata_0->short_name_ = "Volt (Batt 1)";

  auto* voltage_0 = new ADS1115(0, vdiv_multiplier, read_delay);
  voltage_0
    ->connect_to(new Linear(1, 0, "/Voltage_Batt_1/LinearTransform"))
    ->connect_to(new SKOutputNumber("electrical.batteries.battery1.voltage", "", metadata_0));

  SKMetadata* metadata_1 = new SKMetadata();
  metadata_1->units_ = "V";
  metadata_1->description_ = "Battery 2 Voltage";
  metadata_1->display_name_ = "Voltage (Batt 2)";
  metadata_1->short_name_ = "Volt (Batt 2)";

  auto* voltage_1 = new ADS1115(1, vdiv_multiplier, read_delay);
  voltage_1
    ->connect_to(new Linear(1, 0, "/Voltage_Batt_2/LinearTransform"))
    ->connect_to(new SKOutputNumber("electrical.batteries.battery2.voltage", "", metadata_1));

  SKMetadata* metadata_2 = new SKMetadata();
  metadata_2->units_ = "V";
  metadata_2->description_ = "Battery 3 Voltage";
  metadata_2->display_name_ = "Voltage (Batt 3)";
  metadata_2->short_name_ = "Volt (Batt 3)";

  auto* voltage_2 = new ADS1115(2, vdiv_multiplier, read_delay);
  voltage_2
    ->connect_to(new Linear(1, 0, "/Voltage_Batt_3/LinearTransform"))
    ->connect_to(new SKOutputNumber("electrical.batteries.battery3.voltage", "", metadata_2));

  SKMetadata* metadata_3 = new SKMetadata();
  metadata_3->units_ = "V";
  metadata_3->description_ = "Solar Charge Voltage";
  metadata_3->display_name_ = "Voltage (Solar)";
  metadata_3->short_name_ = "Volt (Solar)";

  auto* voltage_3 = new ADS1115(3, vdiv_multiplier, read_delay);
  voltage_3
    ->connect_to(new Linear(1, 0, "/Voltage_Solar/LinearTransform"))
    ->connect_to(new SKOutputNumber("electrical.solar.flybridge.voltage", "", metadata_3));
#endif

#ifdef HADDOCK_SENSOR_TANK_FUEL
  SKMetadata* metadata = new SKMetadata();
  metadata->units_ = "ratio";
  metadata->description_ = "Fuel Tank Level";
  metadata->display_name_ = "Fuel Tank Level";
  metadata->short_name_ = "Fuel Level";

  auto* input = new AnalogInput();
  input
    ->connect_to(new Linear(1.0, 0.0, "/Tank/LinearTransformBeforeMap"))
    ->connect_to(new MapRatio(1020.0, 327.0, "/Tank/Map"))
    ->connect_to(new Linear(1.0, 0.0, "/Tank/LinearTransform"))
    ->connect_to(new SKOutputNumber("tanks.fuel.0.currentLevel", "/Tank/Sk", metadata));
#endif

  // Start the SensESP application running
  sensesp_app->enable();
});

#endif
