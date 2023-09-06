SHELL := /bin/bash

all: build compiledb
	@echo "Building Platformio TARGET_ENVS: ${TARGET_ENVS}"

pwm_simple:
	TARGET_ENVS=pwm_simple make upload

pwm_simple_register:
	 TARGET_ENVS=pwm_simple_register make upload

pwm_timer_register:
	 TARGET_ENVS=pwm_timer_register make upload

build: 
	pio -f -c vim run

upload:
	pio -f -c vim run --target upload --environment ${TARGET_ENVS}

clean:
	pio -f -c vim run --target clean --environment ${TARGET_ENVS}

cleanall:
	pio -f -c vim run --target cleanall --environment ${TARGET_ENVS}

program:
	pio -f -c vim run --target program --environment ${TARGET_ENVS}

uploadfs:
	pio -f -c vim run --target uploadfs --environment ${TARGET_ENVS}

update:
	pio -f -c vim update --environment ${TARGET_ENVS}

compiledb:
	@rm -rf ./compile_commands.json
	pio -f -c vim run --target compiledb

monitor:
	pio device monitor
