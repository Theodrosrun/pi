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
      State_(State::None),
      TargetPulseWidth_us_(ComputePulseWidth_us()),
      MaximumSteps_(ComputeMaximumSteps()) {}

//_______________________________________________________________________________________________

InjectionControl::State InjectionControl::Execute() {
    // Reset state
    StepCount_ = 0u;
    LastPressure_bar_ = 0.0f;
    CurrentPulseWidth_us_ = MotorMotionConfig_.StartPulseWidth_us;
    Running_ = true;
    State_ = State::None;

    // Set motor direction and enable motor
    StepperMotor_.SetDirection(StepperMotor::Direction::Forward);
    StepperMotor_.Enable();

    while (Running_) {
        // Check if maximum steps is reached
        if (StepCount_ >= MaximumSteps_) {
            State_ = State::MaximumStepsReached;
            break;
        }

        // Generate step pulse
        StepperMotor_.Step(CurrentPulseWidth_us_);
        ++StepCount_;

        // Update acceleration ramp
        UpdateMotorAcceleration(TargetPulseWidth_us_);

        // Check pressure at defined intervals
        if (PressureShouldBeChecked() &&
            PressureSensor_.Update(InjectionConfig_.SyringeDiameter_mm)) {
            LastPressure_bar_ = PressureSensor_.GetPressure_bar();

            // Check if safety pressure is reached
            if (LastPressure_bar_ >= InjectionConfig_.SafetyPressure_bar) {
                State_ = State::SafetyPressureReached;
                break;
            }

            // Check if target pressure is reached
            if (LastPressure_bar_ >= InjectionConfig_.TargetPressure_bar) {
                State_ = State::TargetPressureReached;
                break;
            }
        }
    }

    // Disable motor and exit
    StepperMotor_.Disable();
    Running_ = false;

    return State_;
}

//_______________________________________________________________________________________________

float InjectionControl::GetLastPressure_bar() const {
    return LastPressure_bar_;
}

//_______________________________________________________________________________________________

InjectionControl::State InjectionControl::GetState() const {
    return State_;
}

//_______________________________________________________________________________________________

void InjectionControl::UpdateMotorAcceleration(const uint32_t targetPulseWidth_us) {
    // Disable acceleration ramp if no update period is configured.
    if (MotorMotionConfig_.AccelerationPeriodSteps == 0u) {
        return;
    }

    // Update the ramp only every AccelerationPeriodSteps commanded steps.
    if ((StepCount_ % MotorMotionConfig_.AccelerationPeriodSteps) != 0u) {
        return;
    }

    // Decrease the pulse width progressively to increase the motor speed, smaller pulse width means
    // a higher step frequency.
    if (CurrentPulseWidth_us_ > (targetPulseWidth_us + MotorMotionConfig_.AccelerationStep_us)) {
        CurrentPulseWidth_us_ -= MotorMotionConfig_.AccelerationStep_us;
    }
    // Clamp the pulse width to the target value to avoid going faster than requested.
    else {
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
