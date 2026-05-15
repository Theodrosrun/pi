//_______________________________________________________________________________________________
//
// Copyright (C) 2026                      HES-SO Master                         CH-1004 Lausanne
//_______________________________________________________________________________________________
//
// PROJECT  PI
//_______________________________________________________________________________________________
//
//! \file    InjectionControl.h
//! \brief   Injection control class
//!
//! \author  Theodros Mulugeta
//_______________________________________________________________________________________________

#ifndef INJECTIONCONTROL_H
#define INJECTIONCONTROL_H

#include <stdint.h>

#include "InjectionConfig.h"
#include "PressureSensor.h"
#include "StepperMotor.h"

//! \brief Controls saline injection using a stepper motor and pressure feedback
class InjectionControl {
   public:
    //! \brief Injection stop reason
    enum class StopReason : uint8_t {
        None,
        TargetPressureReached,
        SafetyPressureReached,
        MaximumStepsReached
    };

    //! \brief Constructor
    InjectionControl(StepperMotor& stepperMotor,
                     PressureSensor& pressureSensor,
                     const InjectionConfig& config);

    //! \brief Runs one complete injection sequence
    //! \return Stop reason
    StopReason Run();

    //! \brief Stops the injection immediately
    void Stop();

    //! \brief Gets the last stop reason
    //! \return Last stop reason
    StopReason GetStopReason() const;

    //! \brief Gets the last measured pressure
    //! \return Last pressure in bar
    float GetLastPressure_bar() const;

    //! \brief Gets the number of steps performed during the last injection
    //! \return Step count
    uint32_t GetStepCount() const;

   private:
    //! \brief Updates the acceleration ramp
    //! \param targetPulseWidth_us Target pulse width in microseconds for the current step
    void UpdateRamp(const uint32_t targetPulseWidth_us);

    //! \brief Computes the pulse width for the current step based on the motion profile
    uint32_t ComputePulseWidth_us() const;

    uint32_t ComputeMaximumSteps() const;

   private:
    StepperMotor& StepperMotor_;
    PressureSensor& PressureSensor_;

    InjectionConfig Config_;

    StopReason StopReason_;
    float LastPressure_bar_;
    uint32_t StepCount_;
    uint32_t CurrentPulseWidth_us_;
    bool Running_;
};

#endif  // INJECTIONCONTROL_H
