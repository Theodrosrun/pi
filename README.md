# PI

Arduino/PlatformIO project for pressure measurement with an HX711 sensor, a 16x2 LCD, and a keypad.

## Architecture

- `PressureSensor`: reads the HX711 and computes the pressure in bar
- `LCD`: manages the 16x2 display
- `Keypad`: reads and decodes keypad buttons
- `PinConfig.h`: centralizes pin definitions
- `main.cpp`: wires the modules together

## Hardware

- Arduino Uno R4 Minima
- HX711
- Force sensor / load cell
- 16x2 LCD with keypad shield

## Pin Configuration

Defined in `include/PinConfig.h`:

- HX711 `DATA` : `12`
- HX711 `CLOCK` : `13`
- LCD `RS` : `8`
- LCD `ENABLE` : `9`
- LCD `D4` : `4`
- LCD `D5` : `5`
- LCD `D6` : `6`
- LCD `D7` : `7`
- Analog keypad : `A0`

## Useful Commands

From the project root:

```bash
platformio run
platformio run --target upload
platformio device monitor
```

## PlatformIO Environment

- environment: `uno_r4_minima`
- framework: `arduino`
- monitor speed: `9600`
