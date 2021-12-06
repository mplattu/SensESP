build_salon_environment:
	pio run --environment salon_environment

upload_salon_environment:
	pio run --environment salon_environment -t upload

build_current_solar:
	pio run --environment current_solar

upload_current_solar:
	pio run --environment current_solar -t upload

monitor:
	pio device monitor

clean:
	rm -fR .pio/
