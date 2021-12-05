# Haddock-specific Notes

## Sensors

### `salon_environment`

Equipment

 * Wemos D1 ESP8266
 * BMP280 temperature and atm pressure sensor
 * Using `ESP.deepSleep()` (c.f. `sensors/deepsleeptimer`) as temperature tends to travel upwards while the device is continuously on

Compilation

 * `make upload_salon_environment`

Configuration

 * Both temperature and pressure sensor have a linear transform which can be configured from the sensor UI (`PressureTransform` and `TemperatureTransform`)
 * To avoid the warming device affecting the temperature measurement the deep sleep feature can be configured at the UI (`DeepSleepTimer`)
  * A closed connection between `D0` and `RST` is required for deep sleep
  * If you need the device to stay on open the connection
  * Note! The device still reboots when `ESP.deepSleep()` is called

References

 * https://www.instructables.com/Library-for-BMP280/
