# Makefile

all:	build-simulator build-firmware

build-simulator:
	$(MAKE) -C src/simulator

build-firmware:
	$(MAKE) -C src/firmware

clean:
	$(MAKE) -C src/simulator clean
	$(MAKE) -C src/firmware clean
	rm -rf *.vcd
