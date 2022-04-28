build_salon_environment:
	pio run --environment salon_environment

upload_salon_environment:
	pio run --environment salon_environment -t upload

build_temp_outdoor:
	pio run --environment temp_outdoor

upload_temp_outdoor:
	pio run --environment temp_outdoor -t upload

build_temp_engineroom:
	pio run --environment temp_engineroom

upload_temp_engineroom:
	pio run --environment temp_engineroom -t upload

build_temp_batt1:
	pio run --environment temp_batt1

upload_temp_batt1:
	pio run --environment temp_batt1 -t upload

build_temp_batt2:
	pio run --environment temp_batt2

upload_temp_batt2:
	pio run --environment temp_batt2 -t upload

build_temp_batt3:
	pio run --environment temp_batt3

upload_temp_batt3:
	pio run --environment temp_batt3 -t upload

build_current_consumption:
	pio run --environment current_consumption

upload_current_consumption:
	pio run --environment current_consumption -t upload

build_electricity_solar:
	pio run --environment electricity_solar

upload_electricity_solar:
	pio run --environment electricity_solar -t upload

build_electricity_batt1:
	pio run --environment electricity_batt1

upload_electricity_batt1:
	pio run --environment electricity_batt1 -t upload

build_electricity_batt2:
	pio run --environment electricity_batt2

upload_electricity_batt2:
	pio run --environment electricity_batt2 -t upload

build_electricity_batt3:
	pio run --environment electricity_batt3

upload_electricity_batt3:
	pio run --environment electricity_batt3 -t upload

build_tank_fuel:
	pio run --environment tank_fuel

upload_tank_fuel:
	pio run --environment tank_fuel -t upload

build_tank_black:
	pio run --environment tank_black

upload_tank_black:
	pio run --environment tank_black -t upload

build_tank_fresh:
	pio run --environment tank_fresh

upload_tank_fresh:
	pio run --environment tank_fresh -t upload

build_revs_prop_stb:
	pio run --environment revs_prop_stb

upload_revs_prop_stb:
	pio run --environment revs_prop_stb -t upload

build_revs_prop_port:
	pio run --environment revs_prop_port

upload_revs_prop_port:
	pio run --environment revs_prop_port -t upload

monitor:
	pio device monitor

clean:
	rm -fR .pio/
