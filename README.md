# Lily's Pad

## Quick start

### How to flash : 
make flash
### How to start : 
turn on Lily's pad, have fun !

## Overview

<img width="977" height="1286" alt="ddbd350f-061c-47f3-a756-0a21d2c326e7_E3B3C5B3-A24D-4E5D-87FF-49D93A3F66E2" src="https://github.com/user-attachments/assets/b9f07d69-60a5-4935-91db-9a3ed593b332" />

## Lily's Pad </br>

is a simple interactive and educational object primarily intended for
children, teachers/educational teams, and families

It allows users to familiarize themselves with the alphabet and numbers in
French Sign Language (FSL) or English Sign Language (ESL), as chosen.

Lily's Pad is also accessible for hearing users who are curious or who have
hearing-impaired loved ones they would like to communicate with.

## Pinout

### ATmega2560 - Power and quartz

GND - 11, 32, 62, 81, 99
VCC - 10, 31, 61, 80, 100
XTAL1 - 34
XTAL2 - 33

### ATmega2560 - Resets

RESET ATmega - 30/RESET
RESET main screen - 7/PE5
RESET eyes - 17/PH5

### ATMega2560 - AVR ISP

SCK - 20/PB1
MOSI - 21/PB2
MISO - 22/PB3
RESET - 30/RESET


### ATmega2560 - SPI

SS (Slave Select) - 19/PB0
SCK - 20/PB1
MOSI - 21/PB2
MISO - 22/PB3
Data/Command Screens - 18/PH6
Chip select main screen - 6/PE4
Chip select left eye - 16/PH4
Chip select right eye - 15/PH3
Chip select SD - 9/PE7
BLK/LED - 8/PE6


### ATmega2560 - UART


RX0 - 2/PE0/RXD0
TX0 - 3/PE1/TXD0


### ATmega2560 - Keyboard rows

C1 - 57/PC4
C2 - 58/PC5
C3 - 59/PC6
C4 - 60/PC7
C5 - 63/PJ0
C6 - 64/PJ1
C7 - 65/PJ2
C8 - 66/PJ3
C9 - 67/PJ4
C10 - 68/PJ5

### ATmega2560 - Keyboard lines


R1 - 53/PC0
R2 - 54/PC1
R3 - 55/PC2
R4 - 56/PC3


### ATmega2560 - Rotary encoder and slide switch


i2c expander-SDA - 44/PD1
i2c expander-SCL - 43/PD0
Slide switch 1 - 69/PJ6
Slide switch 2 - 70/PG2
Slide switch 3 - 71/PA7


## Update firmware

Download update.sh in the latest releases section and run it. The script can take one argument for the theme.

```bash
# cd inside the directory where update.sh is located
$> chmod +x update.sh
$> ./update.sh <DEVICE PORT> <OPTIONAL: THEME>
```

## I. Features

An LCD screen connected to a keyboard featuring letters and numbers.
A rotary encoder allowing users to scroll through FSL/ASL signs on the screen.
A clickable rotary encoder to confirm an input.
A USB Type-C port to power the device.
A power button to turn the device on/off.
A 3D-printed casing around the screen.
A 3D-printed base in which the keyboard is embedded.
A slide switch to change language, English (ASL) or French (FSL).
Two small LCD screens to display animated eyes.
Translation/game/alphabet modes.

## How to use this repository

### Generate Doxygen static website

Run the following command:

```
make doc
```

## Small presentation video

<https://youtu.be/-Ymx-qcXOJo>
