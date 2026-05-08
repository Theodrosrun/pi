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

#include "PinConfig.h"
#include "PressureSensor.h"

namespace {

constexpr float kForcePerCount = 9.81f / 50000.0f;
constexpr float kSyringeDiameterMeters = 0.010f;
constexpr unsigned long kLoopDelayMs = 300;

PressureSensor PressureSensor_(HX711_DATA_PIN,
                               HX711_CLOCK_PIN,
                               kForcePerCount,
                               kSyringeDiameterMeters);

}  // namespace

void setup() {
    Serial.begin(9600);
    PressureSensor_.Init();
}

void loop() {
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
    } else {
        Serial.println("HX711 not ready");
    }

    delay(kLoopDelayMs);
}
