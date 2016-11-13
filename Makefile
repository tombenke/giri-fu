# Makefile

# Select the target MCU, by uncommenting the appropriate lines below
# ATtiny25
#export MMCU=attiny25
#export PARTNO=t25

# ATtiny2313
#export MMCU=attiny2313
#export PARTNO=t2313

# ATmega168
export MMCU=atmega168
export PARTNO=m168

# ATmega328P
#export MMCU=atmega328
#export PARTNO=m328p

# Set the clock frequency for CPU
export F_CPU=4000000

# The dist folder where the result artifacts will be placed as a result of build
export DIST=${CURDIR}/dist

all:	build-simulator build-firmware

build-simulator:
	$(MAKE) -C src/simulator

build-firmware:
	$(MAKE) -e -C src/firmware

clean-all:	clean clean-dist

clean:
	$(MAKE) -C src/simulator clean
	$(MAKE) -C src/firmware clean

clean-dist:
	rm -rf ${DIST}/*

	
.PHONY: burn
burn:
	sudo avrdude -p $(PARTNO) -c avrispmkII -P usb -e -U flash:w:${DIST}/firmware.hex -F
