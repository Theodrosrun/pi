//_______________________________________________________________________________________________
//
// Copyright (C) 2026                      HES-SO Master                         CH-1004 Lausanne
//_______________________________________________________________________________________________
//
// PROJECT  PI
//_______________________________________________________________________________________________
//
//! \file    MotorConfig.h
//! \brief   Motor mechanics and motion configuration definitions
//!
//! \author  Theodros Mulugeta
//_______________________________________________________________________________________________

#ifndef MOTORMECHANICSCONFIG_H
#define MOTORMECHANICSCONFIG_H

#include <stdint.h>

struct MotorMechanicsConfig {
    float ScrewLead_mm_per_rev;
    uint32_t MotorFullSteps_per_rev;
    uint32_t Microsteps;
};

struct MotorMotionConfig {
    uint32_t StartPulseWidth_us;
    uint32_t AccelerationStep_us;
    uint32_t AccelerationPeriodSteps;
};

constexpr MotorMechanicsConfig C_MotorMechanicsConfig = {.ScrewLead_mm_per_rev = 5.0f,
                                                         .MotorFullSteps_per_rev = 200u,
                                                         .Microsteps = 4u};

constexpr MotorMotionConfig C_MotorMotionConfig = {.StartPulseWidth_us = 800u,
                                                   .AccelerationStep_us = 1u,
                                                   .AccelerationPeriodSteps = 5u};

#endif  // MOTORMECHANICSCONFIG_H