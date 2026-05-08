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

    HX711 scale_;
    const uint8_t dataPin_;
    const uint8_t clockPin_;
    const float forcePerCount_;
    const float syringeDiameterMeters_;
    long offset_;
    long raw_;
    long relative_;
    float forceNewtons_;
    float pressureBar_;
};

#endif  // PRESSURE_SENSOR_H
