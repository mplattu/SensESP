#ifdef HADDOCK_SENSOR_ELECTRICITY_BATT1_INA226_BEEF

// Shunt is 2000 micro-ohm, maximum amps is 20
auto* ina226 = new INA226x(0x40, 2000, 20);

SKMetadata* metadataVoltage = new SKMetadata();
metadataVoltage->units_ = "V";
metadataVoltage->description_ = "Battery 1 Charge Voltage";
metadataVoltage->display_name_ = "Charge Voltage (Batt 1)";
metadataVoltage->short_name_ = "Chg Volt (Batt 1)";

auto* voltage = new INA226Value(ina226, INA226Value::TYPE_BUS_VOLTAGE, 5000, "/Voltage/Measurement");
voltage
  ->connect_to(new Linear(1, 0, "/Voltage/LinearTransform"))
  ->connect_to(new SKOutputNumber("electrical.batteries.battery1.voltage", "", metadataVoltage));

SKMetadata* metadataCurrent = new SKMetadata();
metadataCurrent->units_ = "A";
metadataCurrent->description_ = "Battery 1 Charge Current";
metadataCurrent->display_name_ = "Charge Current (Batt 1)";
metadataCurrent->short_name_ = "Chg Amps (Batt 1)";

auto* current = new INA226Value(ina226, INA226Value::TYPE_CURRENT, 5000, "/Current/Measurement");
current
  ->connect_to(new Linear(1, 0, "/Current/LinearTransform"))
  ->connect_to(new SKOutputNumber("electrical.batteries.battery1.current", "", metadataCurrent));

#endif
