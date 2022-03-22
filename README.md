[![CircleCI](https://circleci.com/gh/vi7/arduino-remote-button/tree/master.svg?style=svg)](https://circleci.com/gh/vi7/arduino-remote-button/tree/master)

Touch button with remote control
================================

The idea behind this project is to convert any button or switch in your house into the touch sensor with remote control over RF using simple resource efficient hardware.

Requirements
------------

### Hardware

* Digispark ATTiny85 USB (or compatible clone) with micronucleus bootloader v2.5+
* [TTP223 capacitive touch sensor](./resources/TTP223_Capacitive_Touch_Sensor_HCSENS0039_Diagram.png)
* 315MHz/433MHz ASK RF receiver. Superheterodyne type receiver is recommended to improve stability and range

### Software

[PlatformIO](https://docs.platformio.org/en/latest/what-is-platformio.html)
