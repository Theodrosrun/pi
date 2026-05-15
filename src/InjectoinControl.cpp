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

#include <Arduino.h>

#include "InjectionControl.h"

InjectionControl::InjectionControl(StepperMotor& stepperMotor,
                                   PressureSensor& pressureSensor,
                                   const InjectionConfig& config)
    : StepperMotor_(stepperMotor),
      PressureSensor_(pressureSensor),
      Config_(config),
      StopReason_(StopReason::None),
      LastPressure_bar_(0.0f),
      StepCount_(0u),
      CurrentPulseWidth_us_(config.StartPulseWidth_us),
      Running_(false) {}

//_______________________________________________________________________________________________

InjectionControl::StopReason InjectionControl::Run() {
    StopReason_ = StopReason::None;
    LastPressure_bar_ = 0.0f;
    StepCount_ = 0u;
    CurrentPulseWidth_us_ = Config_.StartPulseWidth_us;
    Running_ = true;

    const uint32_t targetPulseWidth_us = ComputePulseWidth_us();

    const uint32_t syringeMaximumSteps = ComputeMaximumSteps();
    const uint32_t maximumSteps = min(syringeMaximumSteps, Config_.MaximumSteps);

    StepperMotor_.Enable();
    StepperMotor_.SetDirection(Config_.Direction);

    while (Running_) {
        if (StepCount_ >= maximumSteps) {
            StopReason_ = StopReason::MaximumStepsReached;
            break;
        }

        StepperMotor_.Step(CurrentPulseWidth_us_);
        ++StepCount_;

        UpdateRamp(targetPulseWidth_us);

        if (Config_.PressureCheckPeriodSteps != 0u &&
            (StepCount_ % Config_.PressureCheckPeriodSteps) == 0u) {
            if (PressureSensor_.Update()) {
                LastPressure_bar_ = PressureSensor_.GetPressure_bar();

                if (LastPressure_bar_ >= Config_.SafetyPressure_bar) {
                    StopReason_ = StopReason::SafetyPressureReached;
                    break;
                }

                if (LastPressure_bar_ >= Config_.TargetPressure_bar) {
                    StopReason_ = StopReason::TargetPressureReached;
                    break;
                }
            }
        }
    }

    StepperMotor_.Disable();
    Running_ = false;

    return StopReason_;
}

//_______________________________________________________________________________________________

void InjectionControl::Stop() {
    Running_ = false;
    StepperMotor_.Disable();
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
    if (Config_.AccelerationPeriodSteps == 0u) {
        return;
    }

    if ((StepCount_ % Config_.AccelerationPeriodSteps) != 0u) {
        return;
    }

    if (CurrentPulseWidth_us_ > (targetPulseWidth_us + Config_.AccelerationStep_us)) {
        CurrentPulseWidth_us_ -= Config_.AccelerationStep_us;
    } else {
        CurrentPulseWidth_us_ = targetPulseWidth_us;
    }
}

//_______________________________________________________________________________________________

uint32_t InjectionControl::ComputePulseWidth_us() const {
    const float ml_per_mm = Config_.SyringeVolume_ml / Config_.SyringeStroke_mm;
    const float targetSpeed_mm_s = Config_.TargetFlow_ml_s / ml_per_mm;

    const float pulses_per_rev =
        static_cast<float>(Config_.MotorFullSteps_per_rev * Config_.Microsteps);

    const float pulses_per_mm = pulses_per_rev / Config_.ScrewLead_mm_per_rev;
    const float pulses_per_s = targetSpeed_mm_s * pulses_per_mm;

    const float pulseWidth_us = 1000000.0f / (2.0f * pulses_per_s);

    return static_cast<uint32_t>(pulseWidth_us);
}

//_______________________________________________________________________________________________

uint32_t InjectionControl::ComputeMaximumSteps() const {
    const float pulses_per_rev =
        static_cast<float>(Config_.MotorFullSteps_per_rev * Config_.Microsteps);

    const float pulses_per_mm = pulses_per_rev / Config_.ScrewLead_mm_per_rev;
    const float maximumSteps = Config_.SyringeStroke_mm * pulses_per_mm;

    return static_cast<uint32_t>(maximumSteps);
}