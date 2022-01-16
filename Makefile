build_salon_environment:
	pio run --environment salon_environment

upload_salon_environment:
	pio run --environment salon_environment -t upload

build_current_consumption:
	pio run --environment current_consumption

upload_current_consumption:
	pio run --environment current_consumption -t upload

build_electricity_solar:
	pio run --environment electricity_solar

upload_electricity_solar:
	pio run --environment electricity_solar -t upload

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
