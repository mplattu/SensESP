#ifdef HADDOCK_SENSOR_ELECTRICITY_CONSUMPTION_BEEF

// Shunt is 2000 micro-ohm, maximum amps is 20
auto* ina226 = new INA226x(0x40, 2000, 20);

SKMetadata* metadataVoltage = new SKMetadata();
metadataVoltage->units_ = "V";
metadataVoltage->description_ = "Consumed Voltage";
metadataVoltage->display_name_ = "Voltage (Consumed)";
metadataVoltage->short_name_ = "Volt (Consumed)";

auto* voltage = new INA226Value(ina226, INA226Value::TYPE_BUS_VOLTAGE, 5000, "/Voltage/Measurement");
voltage
  ->connect_to(new Linear(1, 0, "/Voltage/LinearTransform"))
  ->connect_to(new SKOutputNumber("electrical.dc.voltage", "", metadataVoltage));

SKMetadata* metadataCurrent = new SKMetadata();
metadataCurrent->units_ = "A";
metadataCurrent->description_ = "Consumed Current";
metadataCurrent->display_name_ = "Current (Consumed)";
metadataCurrent->short_name_ = "Amps (Consumed)";

auto* current = new INA226Value(ina226, INA226Value::TYPE_CURRENT, 5000, "/Current/Measurement");
current
  ->connect_to(new Linear(1, 0, "/Current/LinearTransform"))
  ->connect_to(new SKOutputNumber("electrical.dc.current", "", metadataCurrent));

#endif
