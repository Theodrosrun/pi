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

#include "Config/InjectionConfig.h"
#include "Config/MotorConfig.h"
#include "Config/PinConfig.h"
#include "Control/InjectionControl.h"
#include "PAL/Keypad.h"
#include "PAL/LCD.h"
#include "PAL/PressureSensor.h"
#include "PAL/StepperMotor.h"

namespace {
// Configuration constants
const uint32_t C_SerialBaudRate = 9600u;          //!< Serial baud rate
const uint32_t C_LoopDelay_ms = 10u;              //!< Loop delay in milliseconds
const uint32_t C_ManualMovePulseWidth_us = 500u;  //!< Manual move pulse width in microseconds

// Peripheral instances
Keypad Keypad_(KEYPAD_PIN);  //!< Keypad

LCD LCD_(LCD_RS_PIN,
         LCD_ENABLE_PIN,
         LCD_DATA4_PIN,
         LCD_DATA5_PIN,
         LCD_DATA6_PIN,
         LCD_DATA7_PIN);  //!< LCD

PressureSensor PressureSensor_(HX711_DATA_PIN, HX711_CLOCK_PIN);  //!< Pressure Sensor

StepperMotor StepperMotor_(STEPPER_MOTOR_ENABLE_PIN,
                           STEPPER_MOTOR_DIRECTION_PIN,
                           STEPPER_MOTOR_PULSE_PIN);  //!< Stepper Motor

// Control instances
InjectionControl LeftCoronaryInjectionControl_(
    StepperMotor_,
    PressureSensor_,
    C_LeftCoronaryInjectionConfig,
    C_MotorMechanicsConfig,
    C_MotorMotionConfig);  //!< Left coronary injection control

InjectionControl RightCoronaryInjectionControl_(
    StepperMotor_,
    PressureSensor_,
    C_RightCoronaryInjectionConfig,
    C_MotorMechanicsConfig,
    C_MotorMotionConfig);  //!< Right coronary injection control

// Helper functions
void ExecuteInjection(InjectionControl& injectionControl, const char* line) {
    LCD_.Display(line, "in progress...");
    injectionControl.Execute();
}

void MoveMotorManually(const StepperMotor::Direction direction,
                       const Keypad::Button button,
                       const char* line) {
    LCD_.Display("Manual move", line);

    StepperMotor_.Enable();
    StepperMotor_.SetDirection(direction);

    while (Keypad_.GetPressedButton() == button) {
        StepperMotor_.Step(C_ManualMovePulseWidth_us);
        Keypad_.Update();
    }

    StepperMotor_.Disable();
}
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

    switch (Keypad_.GetPressedButton()) {
        case Keypad::Button::Left:
            ExecuteInjection(LeftCoronaryInjectionControl_, "Left coronary");
            break;

        case Keypad::Button::Right:
            ExecuteInjection(RightCoronaryInjectionControl_, "Right coronary");
            break;

        case Keypad::Button::Up:
            MoveMotorManually(StepperMotor::Direction::Forward, Keypad::Button::Up, "Forward");
            break;

        case Keypad::Button::Down:
            MoveMotorManually(StepperMotor::Direction::Reverse, Keypad::Button::Down, "Reverse");
            break;

        default:
            break;
    }

    if (PressureSensor_.TryUpdatePressure(C_LeftCoronaryInjectionConfig.SyringeDiameter_mm)) {
        LCD_.Display("Pression:", String(PressureSensor_.GetPressure_bar(), 3) + " bar");
    }

    delay(C_LoopDelay_ms);
}
