# Haddock-specific Notes

## Sensors

### `salon_environment`

Equipment

 * Wemos D1 ESP8266
 * BMP280 temperature and atm pressure sensor
 * Using `ESP.deepSleep()` (c.f. `sensors/deepsleeptimer`) as temperature tends to travel upwards while the device is continuously on

Wiring

```
Wemos D1  BMP240
     3v3  VCC
     3v3  CSB
       G  GND
       G  SDO
      D1  SCL
      D2  SDA
  D0-RST  (jumper for ESP8266 deep sleep)
```

Compilation

 * `make upload_salon_environment`

Configuration

 * Both temperature and pressure sensor have a linear transform which can be configured from the sensor UI (`PressureTransform` and `TemperatureTransform`)
 * To avoid the warming device affecting the temperature measurement the deep sleep feature can be configured at the UI (`DeepSleepTimer`)
  * A closed connection between `D0` and `RST` is required for deep sleep
  * If you need the device to stay on open the connection - N.B.! The device still reboots when `ESP.deepSleep()` is called
  * Setting `Time before sleep` to `0` disables deep sleep (this is the default)

References

 * https://www.instructables.com/Library-for-BMP280/

### `temp_outdoor`, `temp_engineroom`, `temp_batt1`, `temp_batt2`, `temp_batt3`

The first attempt to measure machine room temperatures was done using MAX31855 and
thermocouple sensors. However, the sensors got a lot of noise from the machine room
lightning and had to be abandoned.

Equipment
 * Wemos D1 ESP8266
 * DS18B20
 * resistor 4,7k

Wiring

![See images/sensor_temp_ds18b20.png](images/sensor_temp_ds18b20.png)

Compilation

 * `make upload_temp_engineroom`
 * `make upload_temp_batt1` ... `3`

Configuration

 * Adjust temperature value at `Temp/LinearTransform`

### `current_consumption`

Equipment

 * Wemos D1 ESP8266
 * ADS1115 AD converter
 * Shunt 40A 75mV

Wiring

```
Wemos D1  ADS1115
--------  -------
     5v  VCC
      G  GND
      G  ADDR
     D1  SCL
     D2  SDA
     A0  shunt
     A1  shunt
```

Compilation

 * `make upload_current_consumption`

Configuration

 * Adjust current transform line parameters at `Current/LinearTransform`
 * The theoretical multipliers are:
  * 1.0 for 100mV shunt
  * 0,57 for 75mV shunt (default)
  * 2.0 for 50mV shunt

### `electricity_solar`, `electricity_batt1`, `electricity_batt2`, `electricity_batt3`

Electricity sensor measures voltage using INA226 (0-32V) and current using ADS1115 and 75mV shunt.

Equipment

 * Wemos D1 ESP8266
 * INA226
 * ADS1115 AD converter
 * Shunt 40A 75mV

Wiring

```
Wemos D1  ADS1115  INA226
--------  -------  ------
      5v  VCC      VCC
       G  GND      GND
       G  ADDR
      D1  SCL      SCL
      D2  SDA      SDA
      A0  shunt
      A1  shunt
```

![See images/sensor_electricity.png](images/sensor_electricity.png)

N.B.! Use INA229 instead of INA219 for 24V system.

Compilation

 * `make upload_electricity_solar`
 * `make upload_electricity_batt1`
 * `make upload_electricity_batt2`
 * `make upload_electricity_batt3`

Configuration

 * Both voltage and current sensor has linear transform
 * Adjust current transform line parameters at `Current/LinearTransform`
 * The theoretical multipliers are:
   * 1.0 for 100mV shunt
   * 0,57 for 75mV shunt (default)
   * 2.0 for 50mV shunt

References

* Converting raw measurements to volts: https://forums.adafruit.com/viewtopic.php?t=73330
  ```
                                                   ADS1015  ADS1115
                                                   -------  -------
  GAIN_TWOTHIRDS  // 2/3x gain +/- 6.144V  1 bit = 3mV      0.1875mV
  GAIN_ONE        // 1x gain   +/- 4.096V  1 bit = 2mV      0.125mV
  GAIN_TWO        // 2x gain   +/- 2.048V  1 bit = 1mV      0.0625mV
  GAIN_FOUR       // 4x gain   +/- 1.024V  1 bit = 0.5mV    0.03125mV
  GAIN_EIGHT      // 8x gain   +/- 0.512V  1 bit = 0.25mV   0.015625mV
  GAIN_SIXTEEN    // 16x gain  +/- 0.256V  1 bit = 0.125mV  0.0078125mV
  ```

### `tank_fuel`, `tank_fresh` and `tank_black`

Equipment

  * Wemos D1 ESP8266
  * Fuel and fresh tank gauge: Standard 0-180 ohm tank level sensor (ECMS, Wema)
    * 0 ohms = empty
    * 180 ohms = full
  * Black water gauge: Wema 240-30 ohm tank level sensor
    * 240 ohm = empty
    * 30 ohms = full

Wiring

![See images/sensor_tank.png](images/sensor_tank.png)

Compilation

  * `make upload_tank_fuel`
  * `make upload_tank_fresh`
  * `make upload_tank_black`

Configuration

  * The raw values can be read from `/Tank/LinearTransformBeforeCurve`
  * The mapping from raw min-max range can be configured at `/Tank/CurveInterpolator`
  * By default, the curve interpolator model has values for 0, 25, 50 75 and 100%

### `revs_prop_stb`, `revs_prop_port`

Measures propulsion (propellor axel) revolutions. The unit is RPMs instead of standard SI unit Hz since
SignalK server Instrument panel did not offer a dedicated Hz-to-RPM gauge.

Equipment

  * Wemos D1 ESP8266
  * Hall Sensor A3144
  * Resistor 10 kOhm

Wiring

![See images/sensor_revs.png](images/sensor_revs.png)

Compilation

 * `make upload_revs_prop_stb`
 * `make upload_revs_prop_port`

Configuration

 * `/Freq/Frequency` has default value of 60 as the sensor reports RPMs and hot Hz
