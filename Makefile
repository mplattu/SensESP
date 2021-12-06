build_salon_environment:
	pio run --environment salon_environment

upload_salon_environment:
	pio run --environment salon_environment -t upload

build_current_solar:
	pio run --environment current_solar

upload_current_solar:
	pio run --environment current_solar -t upload

build_random_engine_port:
	pio run --environment random_engine_port

upload_random_engine_port:
	pio run --environment random_engine_port -t upload

build_random_engine_stb:
	pio run --environment random_engine_stb

upload_random_engine_stb:
	pio run --environment random_engine_stb -t upload

monitor:
	pio device monitor

clean:
	rm -fR .pio/
