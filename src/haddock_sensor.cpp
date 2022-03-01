#ifndef SENSOR_MAIN
#define SENSOR_MAIN

#ifdef SENSOR_TYPE_BMP280
#include "sensors/deepsleeptimer.h"
#include "sensors/bmp280.h"
#include "transforms/linear.h"
#endif

#ifdef SENSOR_TYPE_MAX31855
#include "sensors/max31855_thermocouple.h"
#include "transforms/linear.h"
#endif

#ifdef HADDOCK_SENSOR_TYPE_CURRENT
#include "sensors/ads1x15.h"
#include "transforms/linear.h"
#endif

#ifdef HADDOCK_SENSOR_TYPE_ELECTRICITY
#include "sensors/ads1x15.h"
#include "sensors/ina226.h"
#include "transforms/linear.h"
#endif

#ifdef HADDOCK_SENSOR_TYPE_TANK
#include "sensors/analog_input.h"
#include "transforms/linear.h"
#include "transforms/curveinterpolator.h"
#endif

#ifdef HADDOCK_SENSOR_TYPE_REVS
#include "sensors/digital_input.h"
#include "transforms/frequency.h"
#endif

#include <Arduino.h>

#include "sensesp_app.h"
#include "sensesp_app_builder.h"
#include "signalk/signalk_output.h"

#ifdef HADDOCK_SENSOR_TANK_FUEL
class CurveInterpolatorTankFuel : public CurveInterpolator {

public:
  CurveInterpolatorTankFuel(String config_path = "") : CurveInterpolator(NULL, config_path) {
    clear_samples();
    add_sample(CurveInterpolator::Sample(728, 1.0));
    add_sample(CurveInterpolator::Sample(621, 0.75));
    add_sample(CurveInterpolator::Sample(538, 0.5));
    add_sample(CurveInterpolator::Sample(377, 0.25));
    add_sample(CurveInterpolator::Sample(50, 0.0));
  }
};
#endif

#ifdef HADDOCK_SENSOR_TANK_BLACK
class CurveInterpolatorTankBlack : public CurveInterpolator {

public:
  CurveInterpolatorTankBlack(String config_path = "") : CurveInterpolator(NULL, config_path) {
    clear_samples();
    add_sample(CurveInterpolator::Sample(257, 1.0));  // 30 ohm
    add_sample(CurveInterpolator::Sample(378, 0.75)); // 52 ohm
    add_sample(CurveInterpolator::Sample(556, 0.5));  // 105 ohm
    add_sample(CurveInterpolator::Sample(666, 0.25)); // 158 ohm
    add_sample(CurveInterpolator::Sample(770, 0.0));  // 240 ohm
  }
};
#endif

#ifdef HADDOCK_SENSOR_TANK_FRESH
class CurveInterpolatorTankFresh : public CurveInterpolator {

public:
  CurveInterpolatorTankFresh(String config_path = "") : CurveInterpolator(NULL, config_path) {
    clear_samples();
    add_sample(CurveInterpolator::Sample(728, 1.0));
    add_sample(CurveInterpolator::Sample(621, 0.75));
    add_sample(CurveInterpolator::Sample(538, 0.5));
    add_sample(CurveInterpolator::Sample(377, 0.25));
    add_sample(CurveInterpolator::Sample(50, 0.0));
  }
};
#endif

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

#ifdef HADDOCK_LOW_PERFORMANCE_SENSOR
  sensesp_app = builder.set_standard_sensors(IP_ADDRESS)
#else
  sensesp_app = builder.set_standard_sensors()
#endif
    ->set_hostname(HOSTNAME)
    ->set_sk_server(SERVER_ADDRESS, SERVER_PORT) // Don't specify server address or port
    ->set_wifi(WIFI_NAME, WIFI_PASS)
    ->set_system_status_led(systemStatusLed)
    ->get_app();

#ifdef SENSOR_SALON_ENVIRONMENT
  auto* deepsleeptimer = new DeepSleepTimer(0, 600, "/Inside/DeepSleepTimer");
  deepsleeptimer
    ->connect_to(new SKOutputNumber("environment.inside.timebeforesleep"));

  SKMetadata* metadata_temperature = new SKMetadata();
  metadata_temperature->units_ = "K";
  metadata_temperature->description_ = "Salon Temperature";
  metadata_temperature->display_name_ = "Salon Temperature";
  metadata_temperature->short_name_ = "Salon Temp";

  auto* bmp280 = new BMP280(0x76);
  const uint read_delay = 10000;            // every 10 seconds
  const uint pressure_read_delay = 20000;   // every 20 seconds

  auto* bmp_temperature =
    new BMP280Value(bmp280, BMP280Value::temperature, read_delay, "/Inside/Temperature");
  bmp_temperature
    ->connect_to(new Linear(1, -2.0, "/Inside/TemperatureTransform"))
    ->connect_to(new SKOutputNumber("environment.inside.temperature", "", metadata_temperature));

  SKMetadata* metadata_pressure = new SKMetadata();
  metadata_pressure->units_ = "Pa";
  metadata_pressure->description_ = "Salon Pressure";
  metadata_pressure->display_name_ = "Salon Pressure";
  metadata_pressure->short_name_ = "Salon Pressure";

  auto* bmp_pressure =
    new BMP280Value(bmp280, BMP280Value::pressure, pressure_read_delay, "/Inside/Pressure");
  bmp_pressure
    ->connect_to(new Linear(1, 0, "/Inside/PressureTransform"))
    ->connect_to(new SKOutputNumber("environment.inside.pressure", "", metadata_pressure));
#endif

#ifdef SENSOR_TEMP_ENGINEROOM
  SKMetadata* metadata = new SKMetadata();
  metadata->units_ = "K";
  metadata->description_ = "Engime Room Temperature";
  metadata->display_name_ = "Engine Room Temperature";
  metadata->short_name_ = "Engine Room Temp";

  auto* sensor = new MAX31855Thermocouple(1000);
  sensor
    ->connect_to(new Linear(1, 0, "/Temp/LinearTransform"))
    ->connect_to(new SKOutputNumber("environment.engineroom.temperature", "", metadata));
#endif

#ifdef SENSOR_TEMP_BATT1
  SKMetadata* metadata = new SKMetadata();
  metadata->units_ = "K";
  metadata->description_ = "Battery 1 Temperature";
  metadata->display_name_ = "Battery 1 Temperature";
  metadata->short_name_ = "Batt 1 Temp";

  auto* sensor = new MAX31855Thermocouple(1000);
  sensor
    ->connect_to(new Linear(1, 0, "/Temp/LinearTransform"))
    ->connect_to(new SKOutputNumber("electrical.batteries.battery1.temperature", "", metadata));
#endif

#ifdef SENSOR_TEMP_BATT2
  SKMetadata* metadata = new SKMetadata();
  metadata->units_ = "K";
  metadata->description_ = "Battery 2 Temperature";
  metadata->display_name_ = "Battery 2 Temperature";
  metadata->short_name_ = "Batt 2 Temp";

  auto* sensor = new MAX31855Thermocouple(1000);
  sensor
    ->connect_to(new Linear(1, 0, "/Temp/LinearTransform"))
    ->connect_to(new SKOutputNumber("electrical.batteries.battery2.temperature", "", metadata));
#endif

#ifdef SENSOR_TEMP_BATT3
  SKMetadata* metadata = new SKMetadata();
  metadata->units_ = "K";
  metadata->description_ = "Battery 3 Temperature";
  metadata->display_name_ = "Battery 3 Temperature";
  metadata->short_name_ = "Batt 3 Temp";

  auto* sensor = new MAX31855Thermocouple(1000);
  sensor
    ->connect_to(new Linear(1, 0, "/Temp/LinearTransform"))
    ->connect_to(new SKOutputNumber("electrical.batteries.battery3.temperature", "", metadata));
#endif

#ifdef HADDOCK_SENSOR_CURRENT_CONSUMPTION
  SKMetadata* metadata = new SKMetadata();
  metadata->units_ = "A";
  metadata->description_ = "24V DC Consumption Current";
  metadata->display_name_ = "Current (Consumption)";
  metadata->short_name_ = "Current (Cons)";

  adsGain_t gain = GAIN_SIXTEEN;
  ADS1115* ads1115 = new ADS1115(0x48, gain);

  auto* current =
    new ADS1x15RawValue (ads1115, channels_0_1, 500, "/Current/Measurement");
  current
    ->connect_to(new Linear(256.0 / 32768.0, 0, ""))
    ->connect_to(new Linear(-0.53, 0, "/Current/LinearTransform"))
    ->connect_to(new SKOutputNumber("electrical.dc.current", "/Current/Sk", metadata));
#endif

#ifdef HADDOCK_SENSOR_ELECTRICITY_SOLAR
#define HADDOCK_SENSOR_ELECTRICITY_SOLAR_BEEF
#include "haddock_sensor_electricity_solar.cpp"
#endif

#ifdef HADDOCK_SENSOR_ELECTRICITY_BATT1
#define HADDOCK_SENSOR_ELECTRICITY_BATT1_BEEF
#include "haddock_sensor_electricity_batt1.cpp"
#endif

#ifdef HADDOCK_SENSOR_ELECTRICITY_BATT2
#define HADDOCK_SENSOR_ELECTRICITY_BATT2_BEEF
#include "haddock_sensor_electricity_batt2.cpp"
#endif

#ifdef HADDOCK_SENSOR_ELECTRICITY_BATT3
#define HADDOCK_SENSOR_ELECTRICITY_BATT3_BEEF
#include "haddock_sensor_electricity_batt3.cpp"
#endif

#ifdef HADDOCK_SENSOR_TANK_FUEL
  SKMetadata* metadata = new SKMetadata();
  metadata->units_ = "ratio";
  metadata->description_ = "Fuel Tank Level";
  metadata->display_name_ = "Fuel Tank Level";
  metadata->short_name_ = "Fuel Level";

  auto* input = new AnalogInput();
  input
    ->connect_to(new Linear(1.0, 0.0, "/Tank/LinearTransformBeforeCurve"))
    ->connect_to(new CurveInterpolatorTankFuel("/Tank/CurveInterpolator"))
    ->connect_to(new SKOutputNumber("tanks.fuel.0.currentLevel", "/Tank/Sk", metadata));
#endif

#ifdef HADDOCK_SENSOR_TANK_BLACK
  SKMetadata* metadata = new SKMetadata();
  metadata->units_ = "ratio";
  metadata->description_ = "Waste (Black Water) Tank Level";
  metadata->display_name_ = "Waste Tank Level";
  metadata->short_name_ = "Waste Level";

  auto* input = new AnalogInput(A0, 1000, "/Tank/Input");
  input
    ->connect_to(new Linear(1.0, 0.0, "/Tank/LinearTransformBeforeCurve"))
    ->connect_to(new CurveInterpolatorTankBlack("/Tank/CurveInterpolator"))
    ->connect_to(new SKOutputNumber("tanks.blackWater.0.currentLevel", "/Tank/Sk", metadata));
#endif

#ifdef HADDOCK_SENSOR_TANK_FRESH
  SKMetadata* metadata = new SKMetadata();
  metadata->units_ = "ratio";
  metadata->description_ = "Fresh Water Tank Level";
  metadata->display_name_ = "Fresh Water Tank Level";
  metadata->short_name_ = "Water Level";

  auto* input = new AnalogInput(A0, 1000, "/Tank/Input");
  input
    ->connect_to(new Linear(1.0, 0.0, "/Tank/LinearTransformBeforeCurve"))
    ->connect_to(new CurveInterpolatorTankFresh("/Tank/CurveInterpolator"))
    ->connect_to(new SKOutputNumber("tanks.freshWater.0.currentLevel", "/Tank/Sk", metadata));
#endif

#ifdef HADDOCK_SENSOR_REVS_PROP_STB
  SKMetadata* metadata = new SKMetadata();
  metadata->units_ = "rpm";
  metadata->description_ = "Propulsion Revolutions (Starboard)";
  metadata->display_name_ = "Prop Revs (Starboard)";
  metadata->short_name_ = "Prop Revs (Stb)";

  int hz_to_rpms = 60.0;

  auto* sensor = new DigitalInputCounter(D5, INPUT, RISING, 500, "/Freq/Input");
  sensor
    ->connect_to(new Frequency(hz_to_rpms, "/Freq/Frequency")) // Hall sensor gives one signal on each round = 1 Hz
    ->connect_to(new SKOutputNumber("propulsion.stb.revolutions", "/Freq/Sk", metadata));
#endif

#ifdef HADDOCK_SENSOR_REVS_PROP_PORT
  SKMetadata* metadata = new SKMetadata();
  metadata->units_ = "rpm";
  metadata->description_ = "Propulsion Revolutions (Port)";
  metadata->display_name_ = "Prop Revs (Port)";
  metadata->short_name_ = "Prop Revs (Port)";

  int hz_to_rpms = 60.0;

  auto* sensor = new DigitalInputCounter(D5, INPUT, RISING, 500, "/Freq/Input");
  sensor
    ->connect_to(new Frequency(hz_to_rpms, "/Freq/Frequency")) // Hall sensor gives one signal on each round = 1 Hz
    ->connect_to(new SKOutputNumber("propulsion.port.revolutions", "/Freq/Sk", metadata));
#endif

  // Start the SensESP application running
  sensesp_app->enable();
});

#endif
