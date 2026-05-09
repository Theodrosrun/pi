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

#include "ButtonPad.h"
#include "LCD.h"
#include "PinConfig.h"
#include "PressureSensor.h"

namespace {
// Configuration constants
const uint32_t C_SerialBaudRate = 9600u;  //!< Serial baud rate
const uint32_t C_LoopDelay_ms = 300u;     //!< Loop delay in milliseconds

// Peripheral instances
ButtonPad ButtonPad_(BUTTON_PAD_PIN);  //!< Button pad
LCD LCD_(LCD_RS_PIN,
         LCD_ENABLE_PIN,
         LCD_DATA4_PIN,
         LCD_DATA5_PIN,
         LCD_DATA6_PIN,
         LCD_DATA7_PIN);                                          //!< LCD keypad shield
PressureSensor PressureSensor_(HX711_DATA_PIN, HX711_CLOCK_PIN);  //!< Pressure sensor

}  // namespace

//_______________________________________________________________________________________________

void setup() {
    Serial.begin(C_SerialBaudRate);

    PressureSensor_.Init();

    LCD_.Init();
    LCD_.Display("LCD Keypad", "Ready");
}

//_______________________________________________________________________________________________

void loop() {
    ButtonPad_.Update();

    Serial.print("Button = ");
    Serial.println(ButtonPad_.GetPressedButtonName());

    if (PressureSensor_.Update()) {
        Serial.print("Raw = ");
        Serial.print(PressureSensor_.GetRaw());

        Serial.print(" | Relative = ");
        Serial.print(PressureSensor_.GetRelative());

        Serial.print(" | Force = ");
        Serial.print(PressureSensor_.GetForce_n(), 3);
        Serial.print(" N");

        Serial.print(" | Pressure = ");
        Serial.print(PressureSensor_.GetPressure_bar(), 4);
        Serial.println(" bar");

        LCD_.Display("Pression:", String(PressureSensor_.GetPressure_bar(), 3) + " bar");
    }

    delay(C_LoopDelay_ms);
}
