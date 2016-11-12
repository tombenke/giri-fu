# giri-fu

## Intro
This is the firmware and hardware of the giri functional unit.
`giri-fu` is an AVR MCU, which includes simple start-stop timer logic for some channels to switch on and off relays.

This firmware is written for the [giri-fu-hw](https://github.com/tombenke/giri-fu-hw) hardware unit.

## Prerequisites
In order to use the project, you need the software and hardware tools listed below.

### Required software
- make
- gcc
- avr-gcc
- avrdude

### Required hardware
- AVRISPmkII

### Optional software
- simavr
- GTKWave

## TODO

### In-progress

### Backlog

- Setup unit testing (cppunit, or min)

- Setup Doxygen

- Implement the serial communication module for the AVR chip

- Implement a simple commander for the AVR chip

## References

- [giri-fu-hw v1.0 project](https://github.com/tombenke/giri-fu-hw/tree/v1.0.0)
