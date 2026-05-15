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
const uint32_t C_SerialBaudRate = 9600u;  //!< Serial baud rate
const uint32_t C_LoopDelay_ms = 10u;      //!< Loop delay in milliseconds

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
        case Keypad::Button::Left: {
            LCD_.Display("Left coronary", "in progress...");

            const InjectionControl::StopReason stopReason = LeftCoronaryInjectionControl_.Run();

            switch (stopReason) {
                case InjectionControl::StopReason::TargetPressureReached:
                    LCD_.Display(
                        "Target reached",
                        String(LeftCoronaryInjectionControl_.GetLastPressure_bar(), 2) + " bar");
                    break;

                case InjectionControl::StopReason::SafetyPressureReached:
                    LCD_.Display(
                        "Safety stop",
                        String(LeftCoronaryInjectionControl_.GetLastPressure_bar(), 2) + " bar");
                    break;

                case InjectionControl::StopReason::MaximumStepsReached:
                    LCD_.Display("Safety stop", "Max steps");
                    break;

                default:
                    LCD_.Display("Injection", "stopped");
                    break;
            }

            break;
        }

        case Keypad::Button::Right: {
            LCD_.Display("Right coronary", "in progress...");

            const InjectionControl::StopReason stopReason = RightCoronaryInjectionControl_.Run();

            switch (stopReason) {
                case InjectionControl::StopReason::TargetPressureReached:
                    LCD_.Display(
                        "Target reached",
                        String(RightCoronaryInjectionControl_.GetLastPressure_bar(), 2) + " bar");
                    break;

                case InjectionControl::StopReason::SafetyPressureReached:
                    LCD_.Display(
                        "Safety stop",
                        String(RightCoronaryInjectionControl_.GetLastPressure_bar(), 2) + " bar");
                    break;

                case InjectionControl::StopReason::MaximumStepsReached:
                    LCD_.Display("Safety stop", "Max steps");
                    break;

                default:
                    LCD_.Display("Injection", "stopped");
                    break;
            }

            break;
        }

        case Keypad::Button::Up: {
            LCD_.Display("Manual move", "Forward");

            StepperMotor_.Enable();
            StepperMotor_.SetDirection(StepperMotor::Direction::Forward);

            while (Keypad_.GetPressedButton() == Keypad::Button::Up) {
                StepperMotor_.Step(500u);
                Keypad_.Update();
            }

            StepperMotor_.Disable();
            break;
        }

        case Keypad::Button::Down: {
            LCD_.Display("Manual move", "Reverse");

            StepperMotor_.Enable();
            StepperMotor_.SetDirection(StepperMotor::Direction::Reverse);

            while (Keypad_.GetPressedButton() == Keypad::Button::Down) {
                StepperMotor_.Step(500u);
                Keypad_.Update();
            }

            StepperMotor_.Disable();
            break;
        }

        default:
            break;
    }

    if (PressureSensor_.Update(C_LeftCoronaryInjectionConfig.SyringeDiameter_mm)) {
        LCD_.Display("Pression:", String(PressureSensor_.GetPressure_bar(), 3) + " bar");
    }

    delay(C_LoopDelay_ms);
}
