# giri-fu v2.0

The `giri-fu` is a simple garden irrigator functional unit, running on a single chip, micro controller unit.

This repository holds the firmware of this unit,
and it is written for the [giri-fu-hw v2.0](https://github.com/tombenke/giri-fu-hw/tree/v2.0.1) hardware unit.

Note: There is an older (v1) version of this software, which compatible with both the hardware version
[v1.0](https://github.com/tombenke/giri-fu-hw/tree/v1.0.0) and [v2.0](https://github.com/tombenke/giri-fu-hw/tree/v2.0.1).

The picture below shows [the logical functional block diagram of the v1.1 unit](docs/giri_fu_v1.1_FBD.png?raw=true):

![the functional block diagram of the giri-fu v1.1](docs/giri_fu_v1.1_FBD.png?raw=true)

## Features

- Three working modes: INACIVE, MANUAL, AUTO.
- Remote control via simple commands through RS-485 serial line using Request/Response conversation.
- Direct, preprogrammed control of max 6 valves with built-in start-and-stop timers.
- Repeats the same timing pattern every 24 hours.
- Timing starts after reset, with a preprogrammed time offset (no built-in RTC support).

## Infrastructure: [giri-fu-hw v2.0](https://github.com/tombenke/giri-fu-hw/tree/v2.0.1)

## Platform

### Language/runtime

- C
- Arduino
- Libraries used:
    - TimeLib
    - TimeAlarms
    - Operations
    - SST

### Tools

In order to use the project, you need the software and hardware tools listed below.

#### Hardware tool
- AVRISPmkII programmer

#### Software tools
- Arduino IDE
- avrdude

Node:
Use [Dockerized Arduino IDE](https://hub.docker.com/r/tombenke/darduino/) in case you do nont want to install the IDE onto your machine.
In order to do this, you need to have [Docker](https://www.docker.com/) installed.

### Build

1. Install the required libraries uder the `libraries` folder of the Arduino IDE.
2. Clone the project, and open the giri-fu.ino file with the Arduino IDE.
3. Define the name of the device (`DEVICE_ID`), and the baud-rate (`BAUD_RATE`) in the `giri-fu.ino` file.
4. Verify and Compile.
5. Connect the programmer to the computer and the giri-fu-hw module.
6. Upload the compiled code using the IDE.

## Usage

- Build the binaries and the hex file.
- Burn the hex file onto the hardware module.
- Start control period with power-on.
- (Re-)program the start-stop timing, and RTC offset time for reset using the commands (see the next section for details).

### Commands

You can talk to the functional unit via RS-485 serial line.
You can use a normal terminal emulator, such as [GTKTerm](http://gtkterm.feige.net/), to do this.

Each functional unit has a unique ID, a name that you can use to identify, and it responds to a predefined set of commands.
For the sake of simplicity, let's suppose we call our unit 'g2'.

The functional unit only responds to commands that contains it name.
The response can be either success or failure (OK, or NOK). in case of failur the unit also tells the reason of failure in its answer.

The detailed format of the commands depend on the actual meaning, and can vary on command by command,
however there is a generic format that every command/response pair follows:

Request:

    <device-id> <command> [<parameter-1> ...]

Response (succeful execution):

    <device-id> <command> [<parameter-1> ...] - OK

Response (unsuccesful execution)

    <device-id> <command> [<parameter-1> ...] - NOK: <reason of failure>

An example for successful execution:

    g2 getVersion
    g2 getVersion v1.0.0 - OK

An example for failure:

    g2 getVersio
    g2 getVersio - NOK : UNKNOWN OPERATION

#### getVersion

Returns with the actual version of the firmware installed on to the functional unit.

format:

    <device-id> getVersion

example:

    g2 getVersion
    g2 getVersion v1.0.0 - OK

#### reset

format:

    <device-id> reset

example:

     g2 reset
     g2 reset - OK

#### setPort

format:

    <device-id> setPort <portIdx>

example:

    g2 setPort 4 HIGH
    g2 setPort 4 HIGH - OK

#### getPort

format:

    <device-id> getPort <portIdx> <state>

example:

     g2 getPort 3
     g2 getPort 3 LOW - OK

#### setTime

format:

    <device-id> setTime <hour> <minute> <second> <day> <month> <year>

example:

    g2 setTime 17 5 0 1 5 2017
    g2 setTime 17 5 0 1 5 2017 - OK

#### getTime

format:

    <device-id> getTime

example:

    g2 getTime
    g2 getTime 13 21 3 1 5 2017 - OK

#### getStatus

format:

     <device-id> getStatus

example:

     g2 getStatus ... - OK

#### setSector

format:

     <device-id> setSector <sectorIdx> <portIdx> <mode> <fromHour> <fromMinute> <toHour> <toMinute>

The mode parameter can be one of the following values:

- 0 : INACTIVE,
- 1 : MANUAL,
- 2 : AUTO.

example:

     g2 setSector 3 3 2 5 0 6 0
     g2 setSector 3 3 2 5 0 6 0 - OK


#### getSector

format:

    <device-id> getSector <sectorIdx>

example:

    g2 getSector 3
    g2 getSector 3 3 2 0 5 0 6 0 - OK

In case the port was not set previously via the `setSector` command, the default mode is `INACTIVE`:

    g2 getSector 2
    g2 getSector 2 2 0 0 0 0 0 0 - OK

## References

- [giri-fu-hw v.2.0 project](https://github.com/tombenke/giri-fu-hw/tree/v2.0.0)
- [AVR Libc Home Page](http://www.nongnu.org/avr-libc/)
- [Dockerized Arduino IDE on GitHub](https://github.com/tombenke/darduino)
- [Dockerized Arduino IDE on Docker Hub](https://hub.docker.com/r/tombenke/darduino/)
- [Docker](https://www.docker.com/)
- [GTKTerm](http://gtkterm.feige.net/)
