#include "rtc.h"

static long int currentTime = 0L;
static long int turnOver = DAY; // Default turn over is one day

extern void rtc_setTurnOver(long int newTurnOver) {
    turnOver = newTurnOver;
}

long int rtc_tick(void) {
    currentTime++;
    if (currentTime >= turnOver) {
        currentTime = 0L;
    }
    return currentTime;
}

void rtc_setCurrentTime(long int time) {
    currentTime = time;
}
long int rtc_getCurrentTime(void) {
    return currentTime;
}