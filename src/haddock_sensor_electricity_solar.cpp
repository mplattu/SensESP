#ifdef HADDOCK_SENSOR_ELECTRICITY_SOLAR_BEEF

SKMetadata* metadataCurrent = new SKMetadata();
metadataCurrent->units_ = "A";
metadataCurrent->description_ = "Solar Panel Charging Current";
metadataCurrent->display_name_ = "Current (Solar)";
metadataCurrent->short_name_ = "Current (Solar)";

adsGain_t gain = GAIN_SIXTEEN;
ADS1115* ads1115 = new ADS1115(0x48, gain);

auto* current =
  new ADS1x15RawValue (ads1115, channels_0_1, 5000, "/Current/Measurement");
current
  ->connect_to(new Linear(256.0 / 32768.0, 0, ""))
  ->connect_to(new Linear(0.46, 0, "/Current/LinearTransform"))
  ->connect_to(new SKOutputNumber("electrical.solar.flybridge.current", "/Current/Sk", metadataCurrent));

auto* ina226 = new INA226x(0x40);

SKMetadata* metadataVoltage = new SKMetadata();
metadataVoltage->units_ = "V";
metadataVoltage->description_ = "Solar Charge Voltage";
metadataVoltage->display_name_ = "Voltage (Solar)";
metadataVoltage->short_name_ = "Volt (Solar)";

auto* voltage = new INA226Value(ina226, 5000, "/Voltage/Measurement");
voltage
  ->connect_to(new Linear(1, 0, "/Voltage/LinearTransform"))
  ->connect_to(new SKOutputNumber("electrical.solar.flybridge.voltage", "", metadataVoltage));

#endif
