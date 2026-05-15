//_______________________________________________________________________________________________
//
// Copyright (C) 2026                      HES-SO Master                         CH-1004 Lausanne
//_______________________________________________________________________________________________
//
// PROJECT  PI
//_______________________________________________________________________________________________
//
//! \file    InjectionControl.cpp
//! \brief   Injection control class
//!
//! \author  Theodros Mulugeta
//_______________________________________________________________________________________________

#include "Control/InjectionControl.h"

#include <Arduino.h>

namespace {
// Configuration constants
const uint32_t C_PressureCheckPeriodSteps = 100u;  // 31 ms
}  // namespace

//_______________________________________________________________________________________________

InjectionControl::InjectionControl(StepperMotor& stepperMotor,
                                   PressureSensor& pressureSensor,
                                   const InjectionConfig& injectionConfig,
                                   const MotorMechanicsConfig& motorMechanicsConfig,
                                   const MotorMotionConfig& motorMotionConfig)
    : StepperMotor_(stepperMotor),
      PressureSensor_(pressureSensor),
      InjectionConfig_(injectionConfig),
      MotorMechanicsConfig_(motorMechanicsConfig),
      MotorMotionConfig_(motorMotionConfig),
      StepCount_(0u),
      LastPressure_bar_(0.0f),
      CurrentPulseWidth_us_(MotorMotionConfig_.StartPulseWidth_us),
      Running_(false),
      StopReason_(StopReason::None),
      TargetPulseWidth_us_(ComputePulseWidth_us()),
      MaximumSteps_(ComputeMaximumSteps()) {}

//_______________________________________________________________________________________________

InjectionControl::StopReason InjectionControl::Execute() {
    // Reset state
    StepCount_ = 0u;
    LastPressure_bar_ = 0.0f;
    CurrentPulseWidth_us_ = MotorMotionConfig_.StartPulseWidth_us;
    Running_ = true;
    StopReason_ = StopReason::None;

    // Set motor direction and enable motor
    StepperMotor_.SetDirection(StepperMotor::Direction::Forward);
    StepperMotor_.Enable();

    while (Running_) {
        // Check if maximum steps is reached
        if (StepCount_ >= MaximumSteps_) {
            StopReason_ = StopReason::MaximumStepsReached;
            break;
        }

        // Generate step pulse
        StepperMotor_.Step(CurrentPulseWidth_us_);
        ++StepCount_;

        // Update acceleration ramp
        UpdateRamp(TargetPulseWidth_us_);

        // Check pressure at defined intervals
        if (PressureShouldBeChecked() &&
            PressureSensor_.Update(InjectionConfig_.SyringeDiameter_mm)) {
            LastPressure_bar_ = PressureSensor_.GetPressure_bar();

            // Check if safety pressure is reached
            if (LastPressure_bar_ >= InjectionConfig_.SafetyPressure_bar) {
                StopReason_ = StopReason::SafetyPressureReached;
                break;
            }

            // Check if target pressure is reached
            if (LastPressure_bar_ >= InjectionConfig_.TargetPressure_bar) {
                StopReason_ = StopReason::TargetPressureReached;
                break;
            }
        }
    }

    // Disable motor and exit
    StepperMotor_.Disable();
    Running_ = false;

    return StopReason_;
}

//_______________________________________________________________________________________________

InjectionControl::StopReason InjectionControl::GetStopReason() const {
    return StopReason_;
}

//_______________________________________________________________________________________________

float InjectionControl::GetLastPressure_bar() const {
    return LastPressure_bar_;
}

//_______________________________________________________________________________________________

uint32_t InjectionControl::GetStepCount() const {
    return StepCount_;
}

//_______________________________________________________________________________________________

void InjectionControl::UpdateRamp(const uint32_t targetPulseWidth_us) {
    if (MotorMotionConfig_.AccelerationPeriodSteps == 0u) {
        return;
    }

    if ((StepCount_ % MotorMotionConfig_.AccelerationPeriodSteps) != 0u) {
        return;
    }

    if (CurrentPulseWidth_us_ > (targetPulseWidth_us + MotorMotionConfig_.AccelerationStep_us)) {
        CurrentPulseWidth_us_ -= MotorMotionConfig_.AccelerationStep_us;
    } else {
        CurrentPulseWidth_us_ = targetPulseWidth_us;
    }
}

//_______________________________________________________________________________________________

bool InjectionControl::PressureShouldBeChecked() const {
    return (C_PressureCheckPeriodSteps != 0u) && ((StepCount_ % C_PressureCheckPeriodSteps) == 0u);
}

//_______________________________________________________________________________________________

uint32_t InjectionControl::ComputePulseWidth_us() const {
    // Injected volume for 1 mm of piston travel.
    const float ml_per_mm = InjectionConfig_.SyringeVolume_ml / InjectionConfig_.SyringeStroke_mm;

    // Required piston speed to reach the target flow rate.
    const float targetSpeed_mm_s = InjectionConfig_.TargetFlow_ml_s / ml_per_mm;

    // Number of commanded steps required for one motor revolution.
    const float steps_per_rev = static_cast<float>(MotorMechanicsConfig_.MotorFullSteps_per_rev *
                                                   MotorMechanicsConfig_.Microsteps);

    // Number of commanded steps required for 1 mm of linear travel.
    const float steps_per_mm = steps_per_rev / MotorMechanicsConfig_.ScrewLead_mm_per_rev;

    // Required commanded step frequency to reach the target piston speed.
    const float steps_per_s = targetSpeed_mm_s * steps_per_mm;

    // Step() generates one commanded step using two delays:
    // one active delay and one idle delay.
    // Therefore, pulseWidth_us is half of the full commanded-step period.
    const float pulseWidth_us = 1000000.0f / (2.0f * steps_per_s);

    return static_cast<uint32_t>(pulseWidth_us);
}

//_______________________________________________________________________________________________

uint32_t InjectionControl::ComputeMaximumSteps() const {
    // Number of steps required for one motor revolution.
    const float steps_per_rev = static_cast<float>(MotorMechanicsConfig_.MotorFullSteps_per_rev *
                                                   MotorMechanicsConfig_.Microsteps);

    // Number of steps required for 1 mm of linear travel.
    const float steps_per_mm = steps_per_rev / MotorMechanicsConfig_.ScrewLead_mm_per_rev;

    // Maximum number of steps required to travel the full syringe stroke.
    const float maximumSteps = InjectionConfig_.SyringeStroke_mm * steps_per_mm;

    return static_cast<uint32_t>(maximumSteps);
}
