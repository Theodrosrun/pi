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

//! \brief Motor and linear axis mechanics configuration
struct MotorMechanicsConfig {
    float ScrewLead_mm_per_rev;       //!< Linear travel per motor revolution in millimeters
    uint32_t MotorFullSteps_per_rev;  //!< Number of full steps per motor revolution
    uint32_t Microsteps;              //!< Driver microstep setting
};

//! \brief Motor motion profile configuration
struct MotorMotionConfig {
    uint32_t StartPulseWidth_us;       //!< Initial pulse width in microseconds
    uint32_t AccelerationStep_us;      //!< Pulse width reduction step in microseconds
    uint32_t AccelerationPeriodSteps;  //!< Number of motor steps between acceleration updates
};

//! \brief Default motor mechanics configuration
constexpr MotorMechanicsConfig C_MotorMechanicsConfig = {.ScrewLead_mm_per_rev = 5.0f,
                                                         .MotorFullSteps_per_rev = 200u,
                                                         .Microsteps = 4u};

//! \brief Default motor motion profile configuration
constexpr MotorMotionConfig C_MotorMotionConfig = {.StartPulseWidth_us = 800u,
                                                   .AccelerationStep_us = 1u,
                                                   .AccelerationPeriodSteps = 5u};

#endif  // MOTORMECHANICSCONFIG_H