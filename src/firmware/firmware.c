/*
    firmware.c
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include "rtc.h"
#include "sst.h"
#include "port.h"

#define NORMAL_MODE 0
// #define TEST_MODE 1

// #define REAL_TIME 0
#define SIMULATION_TIME 1

// for linker, emulator, and programmer's sake
#include "avr_mcu_section.h"
AVR_MCU(F_CPU, MMCU);

volatile long runtime;

#ifdef NORMAL_MODE
struct sst sectors[5] = {
    { TIME(5,0,0), TIME(5,59,0), &PORTC, PC0, STOPPED },
    { TIME(6,0,0), TIME(6,59,0), &PORTC, PC1, STOPPED },
    { TIME(7,0,0), TIME(7,59,0), &PORTC, PC2, STOPPED },
    { TIME(8,0,0), TIME(8,59,0), &PORTC, PC3, STOPPED },
    { TIME(9,0,0), TIME(9,59,0), &PORTC, PC4, STOPPED }
};
#endif
#ifdef TEST_MODE
struct sst sectors[5] = {
    { TIME(0,0,1), TIME(0,0,10), &PORTC, PC0, STOPPED },
    { TIME(0,0,11), TIME(0,0,20), &PORTC, PC1, STOPPED },
    { TIME(0,0,21), TIME(0,0,30), &PORTC, PC2, STOPPED },
    { TIME(0,0,31), TIME(0,0,40), &PORTC, PC3, STOPPED },
    { TIME(0,0,41), TIME(0,0,50), &PORTC, PC4, STOPPED }
};
#endif

void do_tasks( void ) {
    long currentTime = rtc_tick();
    for (int i=0; i<5; i++) {
        sst_activate(&sectors[i], currentTime);
    }
}


ISR(TIMER1_COMPA_vect) {
    runtime++;
    do_tasks();
};


void setupTimer() {

    // Configure timer 1 for CTC mode
    TCCR1B |= (1 << WGM12 );

#ifdef REAL_TIME
    // Set up timer: clk/256 (No prescaling) 4M / 256 = 15625
    TCCR1B |= (1 << CS12);

    // Set CTC compare value to 1Hz at 4MHz AVR clock, with a prescaler of 256
    OCR1A = 15624;
#endif
#ifdef SIMULATION_TIME
    // Set up timer: clk/8
    // TCCR1B |= (1 << CS11);
    // Set up timer: clk/0
    TCCR1B |= (1 << CS10);

    // Set CTC compare value to 10000Hz at 4MHz AVR clock, with a prescaler of 8
    OCR1A = 399;
#endif
    // Enable CTC interrupt
    TIMSK1 = (1 << OCIE1A);
}


void setupPorts() {
    DDRB=0xff;  // all PORT B output
    PORTB = 0x00;   // Write 0 to each bits of PORT B

    DDRC=0xff;  // all PORT C output
    PORTC = 0x00;   // Write 0 to each bits of PORT C
}

void setup() {
    setupPorts();
    setupTimer();

    // Enable interrupts in general
    sei();
}

int main() {

    setup();
    runtime = 0L;

#ifdef NORMAL_MODE
    rtc_setCurrentTime(19L*HOUR);
    // while(runtime<(2L*DAY)) {
    while(1) {
        sleep_mode();
    }
#endif
#ifdef TEST_MODE
    rtc_setTurnOver(60L*SECOND);
    // while(runtime<(1L*MINUTE)) {
    while(1) {
        sleep_mode();
    }
#endif

    cli();
    sleep_mode();
}
