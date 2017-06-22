#include <TimeLib.h>
#include <TimeAlarms.h>
#include <Operations.h>
#include <SST.h>
#define numberOfSectors (sizeof(sectors)/sizeof(SST))

const char* FIRMWARE_VERSION = "v1.0.0";
const char* DEVICE_ID = "g2";
const long unsigned int BAUD_RATE = 115200; // 9600, 19200, 28800, 31250, 57600, 115200
const int BUF_SIZE = 200;
int _RE = 4;    // 0 : Receiving
int DE = 5;     // 1 : Transmitting

const int NUM_IN_PORTS = 2;
const int inPorts[NUM_IN_PORTS] = {
    // Arduino, Port, Pin
    2,      //  PD2,  4
    3       //  PD3,  5
};

const int NUM_OUT_PORTS = 8;
const int outPorts[NUM_OUT_PORTS] = {
    // Arduino, Port, Pin
    14,     //  PC0,  23
    15,     //  PC1,  24
    16,     //  PC2,  25
    17,     //  PC3,  26
    18,     //  PC4,  27
    19,     //  PC5,  28

    8,      //  PB0,  14
    9       //  PB1,  15
};

void stateChangeCb(int portIdx, enum SstState portState) {
    digitalWrite(outPorts[portIdx], portState==ON ? HIGH : LOW);
}

SST sectors[] = {
    SST(0, stateChangeCb),
    SST(1, stateChangeCb),
    SST(2, stateChangeCb),
    SST(3, stateChangeCb),
    
    SST(0, stateChangeCb),
    SST(1, stateChangeCb),
    SST(2, stateChangeCb),
    SST(3, stateChangeCb),

    SST(0, stateChangeCb),
    SST(1, stateChangeCb),
    SST(2, stateChangeCb),
    SST(3, stateChangeCb),

    SST(0, stateChangeCb),
    SST(1, stateChangeCb),
    SST(2, stateChangeCb),
    SST(3, stateChangeCb)
};

Operations operations(_RE, DE, DEVICE_ID, BAUD_RATE, BUF_SIZE);

void getVersion(const char* buf) {
    operations.responseOk(buf, FIRMWARE_VERSION);
};

void resetSectors(void) {
    // Update the sectors one-by-one
    for(int s=0; s<numberOfSectors; s++) {
        sectors[s].reset();
    }
}

void resetPorts(void) {
    // Set port modes, and reset out ports
    for ( int p = 0; p < NUM_IN_PORTS; p++) {
        pinMode(inPorts[p], INPUT);
    }

    for ( int p = 0; p < NUM_OUT_PORTS; p++) {
        pinMode(outPorts[p], OUTPUT);
        digitalWrite(outPorts[p], LOW);
    }
}

void reset(const char* buf) {
    resetPorts();
    resetSectors();
    setupSectors();
    operations.responseOk(buf, 0L);
}

void setPort(const char* buf) {
    char deviceName[64];
    char command[64];
    int portIdx = 0;
    char portStateStr[64];
    bool portState = false;
    int result = sscanf(buf, "%s %s %d %s", deviceName, command, &portIdx, portStateStr);

    if (result != 4) {
        operations.responseNotOk(buf, MSG_WRONG_ARGUMENTS);
    } else {
        portState = (strcmp(portStateStr, "HIGH") == 0) ? HIGH : LOW;
        digitalWrite(outPorts[portIdx], portState);
        operations.responseOk(buf, 0L);
    }
};

void getPort(const char* buf) {
    char respBuf[64];

    int portIdx = 0;
    digitalRead(inPorts[portIdx]);
    sprintf(respBuf, "%s", digitalRead(inPorts[portIdx]) ? "HIGH" : "LOW");
    operations.responseOk(buf, respBuf);
};

void setTime(const char* buf) {
    char deviceName[64];
    char command[64];
    int hours;
    int minutes;
    int seconds;
    int days;
    int months;
    int years;
    int result = sscanf(buf, "%s %s %d %d %d %d %d %d", &deviceName, &command, &hours, &minutes, &seconds, &days, &months, &years);

    if (result != 8) {
        operations.responseNotOk(buf, MSG_WRONG_ARGUMENTS);
    } else {
        setTime(hours, minutes, seconds, days, months, years);
        operations.responseOk(buf, 0L);
    }
}

void getTime(const char* buf) {
    char respBuf[64];
    sprintf(respBuf, "%d %d %d %d %d %d", hour(), minute(), second(), day(), month(), year());
    operations.responseOk(buf, respBuf);
}

void getStatus(const char* buf) {
    char respBuf[64] = "";
    char sectorStatus[20];

    for(int s=0; s<numberOfSectors; s++) {
        sprintf(sectorStatus, " %d", sectors[s].getState());
        strcat(respBuf, sectorStatus);
    }
    operations.responseOk(buf, respBuf);
}

void setSector(const char* buf) {
    char respBuf[64];
    char deviceName[64];
    char command[64];
    int sectorIdx;
    int portIdx;
    enum SstMode mode;
    int fromHour;
    int fromMinute;
    int toHour;
    int toMinute;
    int result = sscanf(buf, "%s %s %d %d %d %d %d %d %d", &deviceName, &command, &sectorIdx, &portIdx, &mode, &fromHour, &fromMinute, &toHour, &toMinute);

    if (result != 9) {
        operations.responseNotOk(buf, MSG_WRONG_ARGUMENTS);
    } else {

        sectors[sectorIdx].set(portIdx, mode, fromHour, fromMinute, toHour, toMinute);
        sprintf(respBuf, "%d %d %d %d %d %d %d", sectorIdx, portIdx, mode, fromHour, fromMinute, toHour, toMinute);
        operations.responseOk(buf, 0L);
    }
}

void getSector(const char* buf) {
    char respBuf[64];
    char deviceName[64];
    char command[64];
    int sectorIdx;
    int result = sscanf(buf, "%s %s %d", &deviceName, &command, &sectorIdx);

    if (result != 3) {
        operations.responseNotOk(buf, MSG_WRONG_ARGUMENTS);
    } else {
        int portIdx;
        enum SstMode mode;
        enum SstState state;
        int fromHour;
        int fromMinute;
        int toHour;
        int toMinute;

        sectors[sectorIdx].get(portIdx, mode, state, fromHour, fromMinute, toHour, toMinute);
        sprintf(respBuf, "%d %d %d %d %d %d %d", portIdx, mode, state, fromHour, fromMinute, toHour, toMinute);
        operations.responseOk(buf, respBuf);
    }
}

OPERATION gpioOperations[] = {
    {  "getVersion",         &getVersion         },
    {  "reset",              &reset              },
    {  "setPort",            &setPort            },
    {  "getPort",            &getPort            },
    {  "setTime",            &setTime            },
    {  "getTime",            &getTime            },
    {  "getStatus",          &getStatus          },
    {  "setSector",          &setSector          },
    {  "getSector",          &getSector          },
    {  0L,                   0L                  }
};

void updateSectors(void) {
    // Update the sectors one-by-one
    for(int s=0; s<numberOfSectors; s++) {
        sectors[s].update();
    }

    // Flash the led to see if it is working, only for development env.
    digitalWrite(outPorts[6], HIGH);
    Alarm.delay(100);
    digitalWrite(outPorts[6], LOW);
}

void setupSectors() {
    /* Setup some initial config to spare manual time during installation */
    /* v1
    sectors[0].set(0, AUTO, 4, 0, 4, 59);
    sectors[1].set(1, AUTO, 5, 0, 5, 59);
    sectors[2].set(2, AUTO, 6, 0, 6, 59);
    sectors[3].set(3, AUTO, 7, 0, 7, 59);
    */

    // v2
    sectors[0].set(0, AUTO, 4,  0, 4, 14);
    sectors[1].set(1, AUTO, 4, 15, 4, 29);
    sectors[2].set(2, AUTO, 4, 30, 4, 44);
    sectors[3].set(3, AUTO, 4, 45, 4, 59);

    sectors[4].set(0, AUTO, 5,  0, 5, 14);
    sectors[5].set(1, AUTO, 5, 15, 5, 29);
    sectors[6].set(2, AUTO, 5, 30, 5, 44);
    sectors[7].set(3, AUTO, 5, 45, 5, 59);

    sectors[8].set(0, AUTO, 6,  0, 6, 14);
    sectors[9].set(1, AUTO, 6, 15, 6, 29);
    sectors[10].set(2, AUTO, 6, 30, 6, 44);
    sectors[11].set(3, AUTO, 6, 45, 6, 59);

    sectors[12].set(0, AUTO, 7,  0, 7, 14);
    sectors[13].set(1, AUTO, 7, 15, 7, 29);
    sectors[14].set(2, AUTO, 7, 30, 7, 44);
    sectors[15].set(3, AUTO, 7, 45, 7, 59);

    setTime(3, 59, 55, 12, 6, 17);

    Alarm.timerRepeat(1, updateSectors);
}

void setup() {
    resetPorts();
    operations.init(gpioOperations);
    setupSectors();
}

void loop() {
    operations.checkRequest();
    updateSectors();
    Alarm.delay(100);
}
