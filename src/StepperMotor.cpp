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

#include "StepperMotor.h"

#include <Arduino.h>

StepperMotor::StepperMotor(const uint8_t pulsePin,
                           const uint8_t directionPin,
                           const uint8_t enablePin)
    : PulsePin_(pulsePin),
      DirectionPin_(directionPin),
      EnablePin_(enablePin),
      Direction_(Direction::Forward),
      Enabled_(false) {}

//_______________________________________________________________________________________________

void StepperMotor::Init() {
    pinMode(PulsePin_, OUTPUT);
    pinMode(DirectionPin_, OUTPUT);
    pinMode(EnablePin_, OUTPUT);

    digitalWrite(PulsePin_, HIGH);
    SetDirection(Direction_);
    Disable();
}

//_______________________________________________________________________________________________

void StepperMotor::Enable() {
    digitalWrite(EnablePin_, LOW);
    Enabled_ = true;
}

//_______________________________________________________________________________________________

void StepperMotor::Disable() {
    digitalWrite(EnablePin_, HIGH);
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

    digitalWrite(PulsePin_, LOW);
    delayMicroseconds(pulseWidth_us);
    digitalWrite(PulsePin_, HIGH);
    delayMicroseconds(pulseWidth_us);
}

//_______________________________________________________________________________________________

void StepperMotor::Steps(const uint32_t stepCount, const uint32_t pulseWidth_us) {
    for (uint32_t stepIndex = 0; stepIndex < stepCount; ++stepIndex) {
        Step(pulseWidth_us);
    }
}
