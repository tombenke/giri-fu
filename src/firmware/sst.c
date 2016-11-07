#include <avr/io.h>
#include "sst.h"
#include "port.h"

extern void sst_activate(struct sst* self, long int currentTime) {
    if (self->phase == STOPPED) {
        if (currentTime == self->start) {
            self->phase = STARTED;
            SetPin(self->port, self->bit);
            // SetPin(self->port, PC3);
        }
    } else if (self->phase == STARTED) {
        if (currentTime == self->stop) {
            self->phase = STOPPED;
            ClearPin(self->port, self->bit);
            // ClearPin(self->port, PC3);
        }
    }
}
