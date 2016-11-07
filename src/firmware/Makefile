# ATtiny25
# MMCU = attiny25
#PARTNO = t25

# ATtiny2313
# MMCU = attiny2313
# PARTNO = t2313

# ATmega168
MMCU = atmega168
PARTNO = m168
F_CPU = 4000000

# ATmega328P
# MMCU = atmega328
# PARTNO = m328p
# F_CPU = 4000000

# Build the firmware
%.hex: %.axf
	avr-objcopy -j .text -j .data -j .eeprom -O ihex ${<} ${@}

FMODULES = rtc

# rtc.o: rtc.c
# 	echo compile $^ $@
# 	avr-gcc -c -Wall -gdwarf-2 -Os -std=gnu99 \
# 			-mmcu=${MMCU} \
# 			-DF_CPU=${F_CPU} \
# 			-DMMCU=\"${MMCU}\" \
# 			-fno-inline-small-functions \
# 			-ffunction-sections -fdata-sections \
# 			-Wl,--relax,--gc-sections \
# 			-Wl,--undefined=_mmcu,--section-start=.mmcu=0x910000 \
# 			-I/usr/lib/avr/include \
# 			-I/usr/lib/avr/include/avr \
# 			-I/usr/local/include/simavr \
# 			-I/usr/local/include/simavr/avr \
# 			$^ -o $@

%.axf: %.c rtc.c sst.c
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
all: firmware.axf firmware.hex

.PHONY: clean
clean:
	rm -rf *.o *.hex *.a *.s *.sim *.axf *.vcd .*.swo .*.swp .*.swm .*.swn

.PHONY: burn
burn:
	sudo avrdude -p $(PARTNO) -c avrispmkII -P usb -e -U flash:w:firmware.hex -F
