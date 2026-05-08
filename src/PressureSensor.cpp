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
    : dataPin_(dataPin),
      clockPin_(clockPin),
      forcePerCount_(forcePerCount),
      syringeDiameterMeters_(syringeDiameterMeters),
      offset_(0),
      raw_(0),
      relative_(0),
      forceNewtons_(0.0f),
      pressureBar_(0.0f) {}

void PressureSensor::init() {
    delay(kStartupDelayMs);

    scale_.begin(dataPin_, clockPin_);

    Serial.println("Remove force from sensor...");
    delay(kUnloadDelayMs);

    offset_ = scale_.read_average(kOffsetSampleCount);

    Serial.print("Offset = ");
    Serial.println(offset_);
}

bool PressureSensor::update() {
    if (!scale_.is_ready()) {
        return false;
    }

    raw_ = scale_.read_average(kMeasurementSampleCount);
    relative_ = raw_ - offset_;
    forceNewtons_ = static_cast<float>(relative_) * forcePerCount_;
    pressureBar_ = forceNewtons_ / computeAreaSquareMeters() / kPascalsPerBar;

    return true;
}

long PressureSensor::getRaw() const {
    return raw_;
}

long PressureSensor::getRelative() const {
    return relative_;
}

float PressureSensor::getForceNewtons() const {
    return forceNewtons_;
}

float PressureSensor::getPressureBar() const {
    return pressureBar_;
}

float PressureSensor::computeAreaSquareMeters() const {
    const float radiusMeters = syringeDiameterMeters_ * 0.5f;
    return PI * radiusMeters * radiusMeters;
}
