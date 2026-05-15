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

#include "PAL/PressureSensor.h"

#include <Arduino.h>

#include "UnitConversions.h"

namespace {
// Configuration constants
const uint32_t C_StartupDelay_ms = 1500u;
const uint32_t C_UnloadDelay_ms = 2000u;
// Calibration and measurement parameters
const uint8_t C_OffsetSampleCount = 20u;
const uint8_t C_MeasurementSampleCount = 1u;
// Sensor and syringe parameters
const float C_ForcePerCount = 5.331522e-5f;
}  // namespace

//_______________________________________________________________________________________________

PressureSensor::PressureSensor(const uint8_t dataPin, const uint8_t clockPin)
    : DataPin_(dataPin), ClockPin_(clockPin), Offset_(0), Force_n_(0.0f), Pressure_bar_(0.0f) {}

//_______________________________________________________________________________________________

void PressureSensor::Init() {
    delay(C_StartupDelay_ms);

    Scale_.begin(DataPin_, ClockPin_);

    Tare();
}

//_______________________________________________________________________________________________

void PressureSensor::Tare() {
    // Wait for the sensor to stabilize before computing the offset
    delay(C_UnloadDelay_ms);

    Offset_ = static_cast<int32_t>(Scale_.read_average(C_OffsetSampleCount));
}
//_______________________________________________________________________________________________

bool PressureSensor::Update(const float syringeDiameter_mm) {
    if (!Scale_.is_ready()) {
        return false;
    }

    const int32_t raw = static_cast<int32_t>(Scale_.read_average(C_MeasurementSampleCount));
    const int32_t relative = raw - Offset_;

    Force_n_ = static_cast<float>(relative) * C_ForcePerCount;
    Pressure_bar_ =
        Force_n_ / ComputeAreaSquareMeters(syringeDiameter_mm) / UnitConversions::C_PascalsPerBar;

    return true;
}

//_______________________________________________________________________________________________

float PressureSensor::GetForce_n() const {
    return Force_n_;
}

//_______________________________________________________________________________________________

float PressureSensor::GetPressure_bar() const {
    return Pressure_bar_;
}

//_______________________________________________________________________________________________

float PressureSensor::ComputeAreaSquareMeters(const float syringeDiameter_mm) const {
    const float diameterMeters = syringeDiameter_mm * 0.001f;
    const float radiusMeters = diameterMeters * 0.5f;
    return PI * radiusMeters * radiusMeters;
}
