# Arduino-Based Theremin Emulation

An Arduino-based Theremin emulation using an HC-SR04 ultrasonic sensor to control MIDI pitch through hand-distance sensing.

## Overview

This project implements a basic Theremin-like musical instrument using an Arduino Uno and an HC-SR04 ultrasonic sensor. The distance of a hand from the sensor is mapped to eight discrete musical notes, which are transmitted as MIDI messages to a software instrument.

The system uses distance averaging and hysteresis to reduce fluctuations in sensor readings and prevent rapid switching between notes.

## Hardware

- Arduino Uno
- HC-SR04 ultrasonic distance sensor
- Breadboard and jumper wires

## How It Works

- **Distance sensing:** The HC-SR04 measures the distance of the hand from the sensor within a defined range of 5–50 cm.
- **Distance averaging:** Five ultrasonic measurements are taken and averaged to reduce fluctuations in distance readings. Invalid readings are discarded.
- **Note mapping:** The playable distance range is divided into eight zones, each corresponding to a predefined MIDI note.
- **Hysteresis:** A 2 cm margin is introduced at zone boundaries to reduce rapid note switching caused by small fluctuations in hand position.
- **MIDI transmission:** MIDI Note On and Note Off messages are transmitted over serial and routed to a software instrument.

## Note Mapping

The eight predefined MIDI notes span C3 to C4:

| MIDI Note | Pitch |
|---|---|
| 48 | C3 |
| 49 | C♯3 |
| 52 | E3 |
| 53 | F3 |
| 55 | G3 |
| 56 | G♯3 |
| 59 | B3 |
| 60 | C4 |

## Circuit Connections

| HC-SR04 Pin | Arduino Uno |
|---|---|
| VCC | 5V |
| GND | GND |
| TRIG | Digital Pin 9 |
| ECHO | Digital Pin 10 |

## Software

- Arduino IDE
- Hairless MIDI / Serial MIDI Bridge
- GarageBand (software instrument)

The Arduino communicates through serial at 115200 baud, with the serial MIDI bridge used to route the MIDI messages to GarageBand.

## Code

The Arduino sketch is available in [`Final_Arduino_MIDI_Theremin_Code.ino`](Final_Arduino_MIDI_Theremin_Code.ino).


