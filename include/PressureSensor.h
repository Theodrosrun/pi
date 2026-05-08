//_______________________________________________________________________________________________
//
// Copyright (C) 2026                      HES-SO Master                         CH-1004 Lausanne
//_______________________________________________________________________________________________
//
// PROJECT  PI
//_______________________________________________________________________________________________
//
//! \file    PressureSensor.h
//! \brief   Peripheral Abstraction Layer Pressure Sensor class
//!
//! \author  Theodros Mulugeta
//_______________________________________________________________________________________________

#ifndef PRESSURE_SENSOR_H
#define PRESSURE_SENSOR_H

#include <Arduino.h>
#include <HX711.h>

class PressureSensor {
   public:
    PressureSensor(uint8_t dataPin,
                   uint8_t clockPin,
                   float forcePerCount,
                   float syringeDiameterMeters);

    void init();
    bool update();
    long getRaw() const;
    long getRelative() const;
    float getForceNewtons() const;
    float getPressureBar() const;

   private:
    float computeAreaSquareMeters() const;

    HX711 Scale_;
    const uint8_t DataPin_;
    const uint8_t ClockPin_;
    const float ForcePerCount_;
    const float SyringeDiameterMeters_;
    long Offset_;
    long Raw_;
    long Relative_;
    float ForceNewtons_;
    float PressureBar_;
};

#endif  // PRESSURE_SENSOR_H
