//_______________________________________________________________________________________________
//
// Copyright (C) 2026                      HES-SO Master                         CH-1004 Lausanne
//_______________________________________________________________________________________________
//
// PROJECT  PI
//_______________________________________________________________________________________________
//
//! \file    PressureSensor.cpp
//! \brief   Peripheral Abstraction Layer Pressure Sensor class
//!
//! \author  Theodros Mulugeta
//_______________________________________________________________________________________________

#include "PressureSensor.h"

namespace {

constexpr unsigned long kStartupDelayMs = 1500;
constexpr unsigned long kUnloadDelayMs = 2000;
constexpr uint8_t kOffsetSampleCount = 20;
constexpr uint8_t kMeasurementSampleCount = 5;
constexpr float kPascalsPerBar = 100000.0f;

}  // namespace

PressureSensor::PressureSensor(uint8_t dataPin,
                               uint8_t clockPin,
                               float forcePerCount,
                               float syringeDiameterMeters)
    : DataPin_(dataPin),
      ClockPin_(clockPin),
      ForcePerCount_(forcePerCount),
      SyringeDiameterMeters_(syringeDiameterMeters),
      Offset_(0),
      Raw_(0),
      Relative_(0),
      ForceNewtons_(0.0f),
      PressureBar_(0.0f) {}

void PressureSensor::init() {
    delay(kStartupDelayMs);

    Scale_.begin(DataPin_, ClockPin_);

    Serial.println("Remove force from sensor...");
    delay(kUnloadDelayMs);

    Offset_ = Scale_.read_average(kOffsetSampleCount);

    Serial.print("Offset = ");
    Serial.println(Offset_);
}

bool PressureSensor::update() {
    if (!Scale_.is_ready()) {
        return false;
    }

    Raw_ = Scale_.read_average(kMeasurementSampleCount);
    Relative_ = Raw_ - Offset_;
    ForceNewtons_ = static_cast<float>(Relative_) * ForcePerCount_;
    PressureBar_ = ForceNewtons_ / computeAreaSquareMeters() / kPascalsPerBar;

    return true;
}

long PressureSensor::getRaw() const {
    return Raw_;
}

long PressureSensor::getRelative() const {
    return Relative_;
}

float PressureSensor::getForceNewtons() const {
    return ForceNewtons_;
}

float PressureSensor::getPressureBar() const {
    return PressureBar_;
}

float PressureSensor::computeAreaSquareMeters() const {
    const float radiusMeters = SyringeDiameterMeters_ * 0.5f;
    return PI * radiusMeters * radiusMeters;
}
