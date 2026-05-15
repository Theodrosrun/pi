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

#include "Config/InjectionConfig.h"
#include "Config/MotorConfig.h"
#include "PAL/PressureSensor.h"
#include "PAL/StepperMotor.h"

//! \brief Controls saline injection using a stepper motor and pressure feedback
class InjectionControl {
   public:
    //! \brief Injection stop reason identifiers
    enum class StopReason : uint8_t {
        None,                   //!< No stop reason defined
        TargetPressureReached,  //!< Target pressure has been reached
        SafetyPressureReached,  //!< Safety pressure limit has been reached
        MaximumStepsReached     //!< Maximum allowed number of steps has been reached
    };

    //! \brief Constructor
    InjectionControl(
        StepperMotor& stepperMotor,                        //!< Stepper motor instance
        PressureSensor& pressureSensor,                    //!< Pressure sensor instance
        const InjectionConfig& injectionConfig,            //!< Injection configuration
        const MotorMechanicsConfig& motorMechanicsConfig,  //!< Motor mechanics configuration
        const MotorMotionConfig& motorMotionConfig         //!< Motor motion profile configuration
    );

    //! \brief Runs one complete injection sequence
    //! \return Stop reason at the end of the injection
    StopReason Run();

    //! \brief Gets the last stop reason
    //! \return Last stop reason
    StopReason GetStopReason() const;

    //! \brief Gets the last measured pressure
    //! \return Last measured pressure in bar
    float GetLastPressure_bar() const;

    //! \brief Gets the number of steps performed during the last injection
    //! \return Number of motor steps performed
    uint32_t GetStepCount() const;

   private:
    //! \brief Updates the motor acceleration ramp
    //! \param targetPulseWidth_us Target pulse width in microseconds
    void UpdateRamp(const uint32_t targetPulseWidth_us);

    //! \brief Indicates whether pressure should be checked at the current step
    //! \return True if the configured pressure check period has been reached
    bool PressureShouldBeChecked() const;

    //! \brief Computes the target pulse width from syringe, flow and motor parameters
    //! \return Target pulse width in microseconds
    uint32_t ComputePulseWidth_us() const;

    //! \brief Computes the maximum number of steps from syringe stroke and motor mechanics
    //! \return Maximum number of motor steps for the configured syringe stroke
    uint32_t ComputeMaximumSteps() const;

   private:
    StepperMotor& StepperMotor_;                        //!< Stepper motor instance
    PressureSensor& PressureSensor_;                    //!< Pressure sensor instance
    const InjectionConfig& InjectionConfig_;            //!< Injection configuration
    const MotorMechanicsConfig& MotorMechanicsConfig_;  //!< Motor mechanics configuration
    const MotorMotionConfig& MotorMotionConfig_;        //!< Motor motion profile configuration

    uint32_t StepCount_;             //!< Number of steps performed during the current injection
    float LastPressure_bar_;         //!< Last measured pressure in bar
    uint32_t CurrentPulseWidth_us_;  //!< Current pulse width in microseconds
    bool Running_;                   //!< Injection running state
    StopReason StopReason_;          //!< Last stop reason
    const uint32_t TargetPulseWidth_us_;  //!< Computed target pulse width in microseconds
    const uint32_t MaximumSteps_;         //!< Computed maximum number of motor steps
};

#endif  // INJECTIONCONTROL_H
