# Build the firmware
${DIST}/%.hex: ${DIST}/%.axf
	avr-objcopy -j .text -j .data -j .eeprom -O ihex ${<} ${@}

FMODULES = rtc

${DIST}/%.axf: %.c rtc.c sst.c
	echo "Building firmware for $(MMCU) / $(F_CPU) Hz"
	avr-gcc -Wall -gdwarf-2 -Os -std=gnu99 \
			-mmcu=${MMCU} \
			-DF_CPU=${F_CPU} \
			-DMMCU=\"${MMCU}\" \
			-fno-inline-small-functions \
			-ffunction-sections -fdata-sections \
			-Wl,--relax,--gc-sections \
			-Wl,--undefined=_mmcu,--section-start=.mmcu=0x910000 \
			-I/usr/lib/avr/include \
			-I/usr/lib/avr/include/avr \
			-I/usr/local/include/simavr \
			-I/usr/local/include/simavr/avr \
			$^ -o $@

# Build all
all: ${DIST}/firmware.axf ${DIST}/firmware.hex

.PHONY: clean
clean:
	rm -rf *.o *.hex *.a *.s *.sim *.axf *.vcd .*.swo .*.swp .*.swm .*.swn
