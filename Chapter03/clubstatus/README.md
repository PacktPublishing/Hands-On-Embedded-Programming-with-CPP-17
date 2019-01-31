# Club Status Service #

This project is a simple C++-based service that monitors a number of switches or similar, changing the status of four relays and publishing on an MQTT topic whenever the state of these two switches changes.

It is currently in use at the [Entropia hackerspace](https://entropia.de/Club-Status) as the club status service, setting club status lights and turning the power in the club on or off depending on whether the club status switch is set to 'on' or not.

## Features ##

* Interrupt-based switch monitoring.
* HTTP server with REST API for viewing the club status.
* MQTT client for status updates.
* Support for PCAL9535A-based i2c relay boards (e.g.: [http://wiki.seeedstudio.com/Raspberry_Pi_Relay_Board_v1.0/](http://wiki.seeedstudio.com/Raspberry_Pi_Relay_Board_v1.0/ "Raspberry Pi Relay Board v1.0"))
* Logging over MQTT.

## Relay ##

At Entropia we use the above linked relay board, with four relays (channel 1-4). The project is currently configured to use the channels as follows:

1. Club power.
2. Green light (Club open; both switches 'on').
3. Yellow light (Club partially closed; one switch still 'on').
4. Red light (Club closed; both switches 'off').

## GPIO switches ##

The project is hard-wired to expect the following two switches on its GPIO pins:

* BCM GPIO 17 (#11, WiringPi 0) => Door lock switch.
* BCM GPIO 4 (#7, WiringPi 7)   => Club status switch.

The door lock switch in combination with the status switch produces the following truth table:

* Status `HIGH`, Lock `LOW` &nbsp;=> Power `ON`, &nbsp; Light `GREEN`.
* Status `LOW`, &nbsp; Lock `LOW` &nbsp;=> Power `OFF`, Light `YELLOW`.
* Status `LOW`, &nbsp; Lock `HIGH` => Power `OFF`, Light `RED`.
* Status `HIGH`, Lock `HIGH` => Power `ON`, &nbsp; Light `YELLOW` + `RED`.

## Building ##

This service is meant to be run on a Raspberry Pi SBC (Single Board Computer). It is easiest to compile this project on there as well, which is why the following instructions assume that one is building on a variety of Raspberry Pi.

**Required tools:**

* GCC (with C++11 support).
* Make. 

**Project dependencies:**

* libmosquittopp
* POCO
* WiringPi


Command line: `sudo apt install libpoco-dev libmosquittopp-dev wiringpi`


After ensuring that all dependencies are installed on the Raspberry Pi system, one can compile the project by executing `make` in the project folder. This will create the binary (`clubstatus`).

## Configuring ##

A configuration file (`config.ini`) is provided with the project. Simply change it to fit one's environment. Of note are:

* clubStatusTopic - The MQTT topic that status updates will be published on (ASCII '1' is club open, '0' is club closed).
* Relay - With 'active' one sets whether a relay board is connected ('0' false, '1' true. Default '1').
* Relay - With 'address' the i2c address of the relay board (i2c GPIO expander device) is set. One can use hexadecimal or decimal here. Default is 0x20.

## Running ##

A sample Systemd service file is provided with the project. One can update the paths in the file and install the service as per Systemd instructions. After this it runs as a regular system service.

When the service is running, one can navigate to the configured HTTP address with a browser and get a self-updating status page with the current status of the club.

One can also obtain the status via MQTT, by publishing on `/club/status`, which will get a response on `/club/status/response`. (Not yet implemented).

## Logging ##

In addition the logging services provided by running the application as a Systemd service, logging is also provided over MQTT for a subset of messages. The MQTT topics for this are:

* `/log/fatal`
* `/log/error`
* `/log/warning`
* `/log/info`
* `/log/debug`

The payload contains a string with the format:

`ClubStatus <log level>: <message>`

## Notes ##

* This project was developed for a single environment and hardware demands. It may not fit a different environment, but PRs are always welcome.
* Any connected switches must be fully debounced. A Raspberry Debounce HAT project can be found at: [https://github.com/MayaPosch/DebounceHat](https://github.com/MayaPosch/DebounceHat "Debounce HAT"). At Entropia we currently use a prototype version of that board.