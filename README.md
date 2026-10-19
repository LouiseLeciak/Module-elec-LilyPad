# Lily's Pad

## Quick start

### How to flash

> [!warning]
> To flash you need `avr-gcc`, `avr-objcopy`, `avr-dude`

```bash
$> make flash
```

### How to start

Plug and switch the Lily's Pad on, have fun !

## Overview

<img width="977" height="1286" alt="ddbd350f-061c-47f3-a756-0a21d2c326e7_E3B3C5B3-A24D-4E5D-87FF-49D93A3F66E2" src="https://github.com/user-attachments/assets/b9f07d69-60a5-4935-91db-9a3ed593b332" />

Lily's Pad is a simple interactive and educational object primarily intended for
children, teachers/educational teams, and families

It allows users to familiarize themselves with the alphabet and numbers in
French Sign Language (FSL) or British Sign Language (BSL), as chosen.

Lily's Pad is also accessible for hearing users who are curious or who have
hearing-impaired loved ones they would like to communicate with.

## Pinout

### ATmega2560 - Power and quartz

| Name  |       Pin(s)        |
| :---: | :-----------------: |
|  GND  | 11, 32, 62, 81, 99  |
|  VCC  | 10, 31, 61, 80, 100 |
| XTAL1 |         34          |
| XTAL2 |         33          |

### ATmega2560 - Resets

|       Name        |   Pin(s)   |
| :---------------: | :--------: |
|   RESET ATmega    | 30 (RESET) |
| RESET Main screen |  7 (PE5)   |
|    RESET Eyes     |  17 (PH5)  |

### ATMega2560 - AVR ISP

| Name  |   Pin(s)   |
| :---: | :--------: |
|  SCK  |  20 (PB1)  |
| MOSI  |  21 (PB2)  |
| MISO  |  22 (PB3)  |
| RESET | 30 (RESET) |

### ATmega2560 - SPI

|         Name         |  Pin(s)  |
| :------------------: | :------: |
|          SS          | 19 (PB0) |
|         SCK          | 20 (PB1) |
|         MOSI         | 21 (PB2) |
|         MISO         | 22 (PB3) |
| Data/Command Screens | 18 (PH6) |
|    SS Main screen    | 6 (PE4)  |
|     SS Left eye      | 16 (PH4) |
|     SS Right eye     | 15 (PH3) |
|        SS SD         | 9 (PE7)  |
|    Backlight/LED     | 8 (PE6)  |

### ATmega2560 - UART

| Name | Pin(s)  |
| :--: | :-----: |
| RX0  | 2 (PE0) |
| TX0  | 3 (PE1) |

### ATmega2560 - Keyboard rows

| Name |  Pin(s)  |
| :--: | :------: |
|  C1  | 57 (PC4) |
|  C2  | 58 (PC5) |
|  C3  | 59 (PC6) |
|  C4  | 60 (PC7) |
|  C5  | 63 (PJ0) |
|  C6  | 64 (PJ1) |
|  C7  | 65 (PJ2) |
|  C8  | 66 (PJ3) |
|  C9  | 67 (PJ4) |
| C10  | 68 (PJ5) |

### ATmega2560 - Keyboard lines

| Name |  Pin(s)  |
| :--: | :------: |
|  R1  | 53 (PC0) |
|  R2  | 54 (PC1) |
|  R3  | 55 (PC2) |
|  R4  | 56 (PC3) |

### ATmega2560 - Rotary encoder and slide switch

|      Name      |  Pin(s)  |
| :------------: | :------: |
|      SDA       | 44 (PD1) |
|      SCL       | 43 (PD0) |
| Slide switch 1 | 69 (PJ6) |
| Slide switch 2 | 70 (PG2) |
| Slide switch 3 | 71 (PA7) |

## Update firmware

Download update.sh in the latest releases section and run it.
The script can take one argument for the theme.

```bash
# cd inside the directory where update.sh is located
$> chmod +x update.sh
$> ./update.sh <DEVICE PORT> <OPTIONAL: THEME>
```

## Features

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

## Generate Doxygen static website

> [!warning]
> To use this, you need doxygen installed on your system.

Run the following command:

```
make doc
```

## Small presentation video

[Video](https://youtu.be/IG8Irz5PA7s?is=ZnnKCJjxfxXaESvo)

## Documentation

|         DOC          |                               Lien                               |
| :------------------: | :--------------------------------------------------------------: |
|    Specifications    | [CDC Lily's Pad](https://docs.google.com/document/d/1O8xM8pXDJNO5zp6k8G5bc-3bWXDQlbUf1931MdxXfck/edit?tab=t.0#heading=h.v1wg8mrqdu4q)                |
|      Prototype       | [Prototypage](https://docs.google.com/document/d/1rZbFsAjX72JkaI_FbCfIp4b-ht4Qgn11_CjMCb2TPow/edit?tab=t.0)                      |
| Problemes&Solutions  | [Problèmes & Solutions](https://docs.google.com/document/d/1paDHATY-Msma2sKo5jREeCfTP951xgNia56kSwROvPU/edit?tab=t.0#heading=h.edi7yypwj845)  |
|      User guide      | [User guide](https://docs.google.com/document/d/11DVUBJalhqJB5Wq1R7DtcMrKvT6OBZJyWIw1Wtvbu1M/edit?tab=t.0#heading=h.yum7i86vcbuq)       |
