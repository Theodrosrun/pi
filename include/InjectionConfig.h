//_______________________________________________________________________________________________
//
// Copyright (C) 2026                      HES-SO Master                         CH-1004 Lausanne
//_______________________________________________________________________________________________
//
// PROJECT  PI
//_______________________________________________________________________________________________
//
//! \file    InjectionConfig.h
//! \brief   Injection configuration definitions
//!
//! \author  Theodros Mulugeta
//_______________________________________________________________________________________________

#ifndef INJECTIONCONFIG_H
#define INJECTIONCONFIG_H

#include <stdint.h>

#include "StepperMotor.h"

//! \brief Injection configuration parameters
struct InjectionConfig {
    // Pressure limits
    float TargetPressure_bar;
    float SafetyPressure_bar;

    // Syringe parameters
    float SyringeVolume_ml;
    float SyringeStroke_mm;
    float TargetFlow_ml_s;

    // Motion profile
    uint32_t StartPulseWidth_us;
    uint32_t AccelerationStep_us;
    uint32_t AccelerationPeriodSteps;
    uint32_t PressureCheckPeriodSteps;

    StepperMotor::Direction Direction;
};

constexpr InjectionConfig C_DefaultInjectionConfig = {
    .TargetPressure_bar = 2.0f,
    .SafetyPressure_bar = 4.0f,

    .SyringeVolume_ml = 30.0f,
    .SyringeStroke_mm = 100.0f,
    .TargetFlow_ml_s = 6.0f,

    .StartPulseWidth_us = 800u,
    .AccelerationStep_us = 1u,
    .AccelerationPeriodSteps = 5u,
    .PressureCheckPeriodSteps = 100000u,

    .Direction = StepperMotor::Direction::Forward};

#endif  // INJECTIONCONFIG_H
