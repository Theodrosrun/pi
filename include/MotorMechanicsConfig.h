//_______________________________________________________________________________________________
//
// Copyright (C) 2026                      HES-SO Master                         CH-1004 Lausanne
//_______________________________________________________________________________________________
//
// PROJECT  PI
//_______________________________________________________________________________________________
//
//! \file    MotorMechanicsConfig.h
//! \brief   Motor mechanics configuration definitions
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

constexpr MotorMechanicsConfig C_MotorMechanicsConfig = {.ScrewLead_mm_per_rev = 5.0f,
                                                         .MotorFullSteps_per_rev = 200u,
                                                         .Microsteps = 4u};

#endif  // MOTORMECHANICSCONFIG_H