//_______________________________________________________________________________________________
//
// Copyright (C) 2026                      HES-SO Master                         CH-1004 Lausanne
//_______________________________________________________________________________________________
//
// PROJECT  PI
//_______________________________________________________________________________________________
//
//! \file    main.cpp
//! \brief   Main entry point for application
//!
//! \author  Theodros Mulugeta
//_______________________________________________________________________________________________

#include <Arduino.h>

#include "Keypad.h"
#include "LCD.h"
#include "PinConfig.h"
#include "PressureSensor.h"
#include "StepperMotor.h"

namespace {
// Configuration constants
const uint32_t C_SerialBaudRate = 9600u;  //!< Serial baud rate
const uint32_t C_LoopDelay_ms = 300u;     //!< Loop delay in milliseconds

// Peripheral instances
Keypad Keypad_(KEYPAD_PIN);  //!< Keypad
LCD LCD_(LCD_RS_PIN,
         LCD_ENABLE_PIN,
         LCD_DATA4_PIN,
         LCD_DATA5_PIN,
         LCD_DATA6_PIN,
         LCD_DATA7_PIN);                                          //!< LCD
PressureSensor PressureSensor_(HX711_DATA_PIN, HX711_CLOCK_PIN);  //!< Pressure Sensor
StepperMotor StepperMotor_(STEPPER_MOTOR_ENABLE_PIN,
                           STEPPER_MOTOR_DIRECTION_PIN,
                           STEPPER_MOTOR_PULSE_PIN);  //!< Stepper Motor
}  // namespace

//_______________________________________________________________________________________________

void setup() {
    Serial.begin(C_SerialBaudRate);

    PressureSensor_.Init();

    LCD_.Init();
    LCD_.Display("LCD Keypad", "Ready");

    StepperMotor_.Init();
    StepperMotor_.Disable();
}

//_______________________________________________________________________________________________

void loop() {
    Keypad_.Update();

    if (Keypad_.GetPressedButton() == Keypad::Button::Right) {
        StepperMotor_.Enable();
        StepperMotor_.SetDirection(StepperMotor::Direction::Forward);
        StepperMotor_.Steps(1600u, 1000u);
        StepperMotor_.Disable();
    }

    if (PressureSensor_.Update()) {
        LCD_.Display("Pression:", String(PressureSensor_.GetPressure_bar(), 3) + " bar");
    }

    delay(C_LoopDelay_ms);
}
