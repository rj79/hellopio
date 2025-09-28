ENV=lolin_s3_pico

.PHONY: build
build:
	pio run -e $(ENV)

.PHONY: clean
clean:
	pio run -t clean