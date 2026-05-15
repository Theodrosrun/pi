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

//! \brief Injection configuration parameters
struct InjectionConfig {
    // Pressure limits
    float TargetPressure_bar;
    float SafetyPressure_bar;

    // Syringe parameters
    float SyringeVolume_ml;
    float SyringeStroke_mm;
    float TargetFlow_ml_s;
};

constexpr InjectionConfig C_LeftCoronaryInjectionConfig = {.TargetPressure_bar = 2.0f,
                                                           .SafetyPressure_bar = 2.0f,

                                                           .SyringeVolume_ml = 30.0f,
                                                           .SyringeStroke_mm = 100.0f,
                                                           .TargetFlow_ml_s = 6.0f};

constexpr InjectionConfig C_RightCoronaryInjectionConfig = {.TargetPressure_bar = 2.0f,
                                                            .SafetyPressure_bar = 2.0f,

                                                            .SyringeVolume_ml = 30.0f,
                                                            .SyringeStroke_mm = 100.0f,
                                                            .TargetFlow_ml_s = 3.0f};

#endif  // INJECTIONCONFIG_H
