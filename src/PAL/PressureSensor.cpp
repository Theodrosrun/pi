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
const uint8_t C_MeasurementSampleCount = 5u;
// Sensor and syringe parameters
const float C_ForcePerCount = 5.331522e-5f;
const float C_SyringeDiameterMeters = 0.010f;
}  // namespace

PressureSensor::PressureSensor(const uint8_t dataPin, const uint8_t clockPin)
    : DataPin_(dataPin),
      ClockPin_(clockPin),
      Offset_(0),
      Raw_(0),
      Relative_(0),
      Force_n_(0.0f),
      Pressure_bar_(0.0f) {}

//_______________________________________________________________________________________________

void PressureSensor::Init() {
    delay(C_StartupDelay_ms);

    Scale_.begin(DataPin_, ClockPin_);

    Serial.println("Remove force from sensor...");
    delay(C_UnloadDelay_ms);

    Offset_ = static_cast<int32_t>(Scale_.read_average(C_OffsetSampleCount));

    Serial.print("Offset = ");
    Serial.println(Offset_);
}

//_______________________________________________________________________________________________

bool PressureSensor::Update() {
    if (!Scale_.is_ready()) {
        return false;
    }

    Raw_ = static_cast<int32_t>(Scale_.read_average(C_MeasurementSampleCount));
    Relative_ = Raw_ - Offset_;
    Force_n_ = static_cast<float>(Relative_) * C_ForcePerCount;
    Pressure_bar_ = Force_n_ / ComputeAreaSquareMeters() / UnitConversions::C_PascalsPerBar;

    return true;
}

//_______________________________________________________________________________________________

int32_t PressureSensor::GetRaw() const {
    return Raw_;
}

//_______________________________________________________________________________________________

int32_t PressureSensor::GetRelative() const {
    return Relative_;
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

float PressureSensor::ComputeAreaSquareMeters() const {
    const float radiusMeters = C_SyringeDiameterMeters * 0.5f;
    return PI * radiusMeters * radiusMeters;
}
