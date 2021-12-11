build_salon_environment:
	pio run --environment salon_environment

upload_salon_environment:
	pio run --environment salon_environment -t upload

build_current_solar:
	pio run --environment current_solar

upload_current_solar:
	pio run --environment current_solar -t upload

build_voltage_charge:
	pio run --environment voltage_charge

upload_voltage_charge:
	pio run --environment voltage_charge -t upload

build_tank_fuel:
	pio run --environment tank_fuel

upload_tank_fuel:
	pio run --environment tank_fuel -t upload

monitor:
	pio device monitor

clean:
	rm -fR .pio/
