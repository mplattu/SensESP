#ifdef HADDOCK_SENSOR_ELECTRICITY_BATT3_BEEF

/*
SKMetadata* metadataCurrent = new SKMetadata();
metadataCurrent->units_ = "A";
metadataCurrent->description_ = "Battery 3 Current";
metadataCurrent->display_name_ = "Current (Batt 3)";
metadataCurrent->short_name_ = "Current (Batt 3)";

adsGain_t gain = GAIN_SIXTEEN;
ADS1115* ads1115 = new ADS1115(0x48, gain);

auto* current =
  new ADS1x15RawValue (ads1115, channels_0_1, 5000, "/Current/Measurement");
current
  ->connect_to(new Linear(256.0 / 32768.0, 0, ""))
  ->connect_to(new Linear(0.46, 0, "/Current/LinearTransform"))
  ->connect_to(new SKOutputNumber("electrical.batteries.battery3.current", "/Current/Sk", metadataCurrent));
*/

auto* ina226 = new INA226x(0x40);

SKMetadata* metadataVoltage = new SKMetadata();
metadataVoltage->units_ = "V";
metadataVoltage->description_ = "Battery 3 Voltage";
metadataVoltage->display_name_ = "Voltage (Batt 3)";
metadataVoltage->short_name_ = "Volt (Batt 3)";

auto* voltage = new INA226Value(ina226, 5000, "/Voltage/Measurement");
voltage
  ->connect_to(new Linear(1, 0, "/Voltage/LinearTransform"))
  ->connect_to(new SKOutputNumber("electrical.batteries.battery3.voltage", "", metadataVoltage));

#endif
