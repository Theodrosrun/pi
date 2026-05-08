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

#include "PressureSensor.h"

namespace {

constexpr uint8_t kHx711DataPin = 12;
constexpr uint8_t kHx711ClockPin = 13;
constexpr float kForcePerCount = 9.81f / 50000.0f;
constexpr float kSyringeDiameterMeters = 0.010f;
constexpr unsigned long kLoopDelayMs = 300;

PressureSensor PressureSensor_(kHx711DataPin,
                               kHx711ClockPin,
                               kForcePerCount,
                               kSyringeDiameterMeters);

}  // namespace

void setup() {
    Serial.begin(9600);
    PressureSensor_.init();
}

void loop() {
    if (PressureSensor_.update()) {
        Serial.print("Raw = ");
        Serial.print(PressureSensor_.getRaw());

        Serial.print(" | Relative = ");
        Serial.print(PressureSensor_.getRelative());

        Serial.print(" | Force = ");
        Serial.print(PressureSensor_.getForceNewtons(), 3);
        Serial.print(" N");

        Serial.print(" | Pressure = ");
        Serial.print(PressureSensor_.getPressureBar(), 4);
        Serial.println(" bar");
    } else {
        Serial.println("HX711 not ready");
    }

    delay(kLoopDelayMs);
}
