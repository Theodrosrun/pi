//_______________________________________________________________________________________________
//
// Copyright (C) 2026                      HES-SO Master                         CH-1004 Lausanne
//_______________________________________________________________________________________________
//
// PROJECT  PI
//_______________________________________________________________________________________________
//
//! \file    PinConfig.h
//! \brief   Pin configuration for the project
//!
//! \author  Theodros Mulugeta
//_______________________________________________________________________________________________

#ifndef PINCONFIG_H
#define PINCONFIG_H

// Pin definitions for HX711 load cell amplifier
#define HX711_DATA_PIN 12
#define HX711_CLOCK_PIN 13

// Pin definitions for LCD
#define LCD_RS_PIN 8
#define LCD_ENABLE_PIN 9
#define LCD_DATA4_PIN 4
#define LCD_DATA5_PIN 5
#define LCD_DATA6_PIN 6
#define LCD_DATA7_PIN 7

// Pin definitions for keypad
#define KEYPAD_PIN A0

// Pin definitions for stepper motor driver
#define STEPPER_MOTOR_PULSE_PIN 2
#define STEPPER_MOTOR_DIRECTION_PIN 3
#define STEPPER_MOTOR_ENABLE_PIN 10

#endif  // PINCONFIG_H
