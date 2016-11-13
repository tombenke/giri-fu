CPPFLAGS = -O2 -g -fPIC --std=gnu99 -Wall \
	-I. \
	-I/usr/include/libdrm \
	-I/usr/local/include/simavr \
	-I/usr/local/include/simavr/avr

LDFLAGS += -L/usr/lib 
LDFLAGS += -L/usr/local/lib
LDFLAGS += -lsimavr -lelf

# Build the simulation board
%.o: %.c
	@$(CC) $(CPPFLAGS) \
	$< -c -o $@

${DIST}/simulator: simulator.o
	gcc ${CFLAGS} ${LFLAGS} -o $@ $^ $(LDFLAGS)

# Build all
all: ${DIST}/simulator

.PHONY: clean
clean:
	rm -rf simulator *.o *.hex *.a *.s *.sim *.axf *.vcd .*.swo .*.swp .*.swm .*.swn
