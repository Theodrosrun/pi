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
    float TargetPressure_bar;  //!< Target pressure in bar
    float SafetyPressure_bar;  //!< Safety pressure limit in bar
    float TargetFlow_ml_s;     //!< Target injection flow in milliliters per second
    float SyringeVolume_ml;    //!< Syringe volume in milliliters
    float SyringeStroke_mm;    //!< Syringe piston stroke in millimeters
    float SyringeDiameter_mm;  //!< Syringe inner diameter in millimeters
};

//! \brief Left coronary injection configuration
constexpr InjectionConfig C_LeftCoronaryInjectionConfig = {.TargetPressure_bar = 2.0f,
                                                           .SafetyPressure_bar = 2.0f,
                                                           .TargetFlow_ml_s = 6.0f,
                                                           .SyringeVolume_ml = 30.0f,
                                                           .SyringeStroke_mm = 100.0f,
                                                           .SyringeDiameter_mm = 10.0f};

//! \brief Right coronary injection configuration
constexpr InjectionConfig C_RightCoronaryInjectionConfig = {.TargetPressure_bar = 2.0f,
                                                            .SafetyPressure_bar = 2.0f,
                                                            .TargetFlow_ml_s = 3.0f,
                                                            .SyringeVolume_ml = 30.0f,
                                                            .SyringeStroke_mm = 100.0f,
                                                            .SyringeDiameter_mm = 10.0f};
#endif  // INJECTIONCONFIG_H
