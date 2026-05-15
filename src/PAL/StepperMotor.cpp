//_______________________________________________________________________________________________
//
// Copyright (C) 2026                      HES-SO Master                         CH-1004 Lausanne
//_______________________________________________________________________________________________
//
// PROJECT  PI
//_______________________________________________________________________________________________
//
//! \file    StepperMotor.cpp
//! \brief   Peripheral Abstraction Layer stepper motor class
//!
//! \author  Theodros Mulugeta
//_______________________________________________________________________________________________

#include "PAL/StepperMotor.h"

#include <Arduino.h>

StepperMotor::StepperMotor(const uint8_t enablePin,
                           const uint8_t directionPin,
                           const uint8_t pulsePin)
    : EnablePin_(enablePin),
      DirectionPin_(directionPin),
      PulsePin_(pulsePin),
      Enabled_(false),
      Direction_(Direction::Forward) {}

//_______________________________________________________________________________________________

void StepperMotor::Init() {
    pinMode(EnablePin_, OUTPUT);
    pinMode(DirectionPin_, OUTPUT);
    pinMode(PulsePin_, OUTPUT);

    Disable();
    SetDirection(Direction_);
    digitalWrite(PulsePin_, HIGH);
}

//_______________________________________________________________________________________________

void StepperMotor::Enable() {
    digitalWrite(EnablePin_, HIGH);
    Enabled_ = true;
}

//_______________________________________________________________________________________________

void StepperMotor::Disable() {
    digitalWrite(EnablePin_, LOW);
    Enabled_ = false;
}

//_______________________________________________________________________________________________

bool StepperMotor::IsEnabled() const {
    return Enabled_;
}

//_______________________________________________________________________________________________

void StepperMotor::SetDirection(const Direction direction) {
    digitalWrite(DirectionPin_, ((direction == Direction::Forward) ? LOW : HIGH));
    Direction_ = direction;
}

//_______________________________________________________________________________________________

StepperMotor::Direction StepperMotor::GetDirection() const {
    return Direction_;
}

//_______________________________________________________________________________________________

void StepperMotor::Step(const uint32_t pulseWidth_us) {
    if (!IsEnabled()) {
        return;
    }

    // Start the active-low pulse
    digitalWrite(PulsePin_, LOW);

    // Keep the pulse active long enough for the driver
    delayMicroseconds(pulseWidth_us);

    // End the pulse and return to idle state
    digitalWrite(PulsePin_, HIGH);

    // Wait before sending the next pulse
    delayMicroseconds(pulseWidth_us);
}

//_______________________________________________________________________________________________

void StepperMotor::Steps(const uint32_t stepCount, const uint32_t pulseWidth_us) {
    for (uint32_t stepIndex = 0; stepIndex < stepCount; ++stepIndex) {
        Step(pulseWidth_us);
    }
}
