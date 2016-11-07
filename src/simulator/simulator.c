/**
    board.c - The simulator program.

    Copyright 2013 Tamás Benke <tombenke@gmail.com>

    This file is part of the archetype project.
    It is a template for simulator boards, using the simavr tool.
 */

#include <stdlib.h>
#include <stdio.h>
#include <libgen.h>
#include <pthread.h>

#include "sim_avr.h"
#include "avr_ioport.h"
#include "sim_elf.h"
#include "sim_gdb.h"
#include "sim_vcd_file.h"


/**
 * Setup the MCU for simulation.
 * @param  fname The name of the firmware file.
 * @return       The MCU descriptor has been setup for simulation.
 */
avr_t* mcu_setup(const char* fname) {
    avr_t* avr;
    elf_firmware_t f;

    printf("Firmware pathname is %s\n", fname);
    elf_read_firmware(fname, &f);

    printf("firmware %s f=%d mmcu=%s\n", fname, (int)f.frequency, f.mmcu);

    avr = avr_make_mcu_by_name(f.mmcu);
    if (!avr) {
        fprintf(stderr, "ERR: AVR '%s' not known\n", f.mmcu);
        exit(1);
    }
    avr_init(avr);
    avr_load_firmware(avr, &f);
    return avr;
}


/**
 * VCD file initialization
 * This will allow you to create a "wave" file and display it in gtkwave.
 * @param avr      Pointer to the MCU descriptor.
 * @param vcd_file Pointer ot the VCD descriptor structure, used for recording.
 * @param fname    The name of the output VCD file.
 */
void vcd_setup(avr_t* avr, avr_vcd_t* vcd_file, const char* fname) {
    avr_vcd_init(avr, fname, vcd_file, 100000 /* usec */);
    avr_vcd_add_signal(vcd_file, 
        avr_io_getirq(avr, AVR_IOCTL_IOPORT_GETIRQ('B'), IOPORT_IRQ_PIN_ALL), 8 /* bits */ ,
        "portb" );
    avr_vcd_add_signal(vcd_file, 
        avr_io_getirq(avr, AVR_IOCTL_IOPORT_GETIRQ('C'), IOPORT_IRQ_PIN_ALL), 8 /* bits */ ,
        "portc" );
}


/**
 * The main function of the simulator program
 * @param  argc Number of arguments.
 * @param  argv Array of arguments.
 * @return
 */
int main(int argc, char *argv[])
{
    avr_t* avr = NULL;
    avr_vcd_t vcd_file;

    // avr = mcu_setup("timer1_isr.axf");
    // avr = mcu_setup("timer1.axf");
    //avr = mcu_setup("../fu/firmware.axf");
    avr = mcu_setup(argv[1]);
    vcd_setup(avr, &vcd_file, "gtkwave_output.vcd");

    printf("Starting VCD trace\n");
    avr_vcd_start(&vcd_file);

    int state = cpu_Running;
    while ((state != cpu_Done) && (state != cpu_Crashed)) {
        state = avr_run(avr);
    }

    printf("Stopping VCD trace\n");
    avr_vcd_stop(&vcd_file);
}
