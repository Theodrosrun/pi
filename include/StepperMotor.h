//_______________________________________________________________________________________________
//
// Copyright (C) 2026                      HES-SO Master                         CH-1004 Lausanne
//_______________________________________________________________________________________________
//
// PROJECT  PI
//_______________________________________________________________________________________________
//
//! \file    StepperMotor.h
//! \brief   Peripheral Abstraction Layer stepper motor class
//!
//! \author  Theodros Mulugeta
//_______________________________________________________________________________________________

#ifndef STEPPERMOTOR_H
#define STEPPERMOTOR_H

#include <stdint.h>

//! \brief Stepper motor class for PUL/DIR/ENA interfaces
class StepperMotor {
   public:
    //! \brief Rotation direction identifiers
    enum class Direction : uint8_t { Forward, Reverse };

    //! \brief Constructor
    StepperMotor(const uint8_t pulsePin,      //!< Arduino pin connected to PUL-
                 const uint8_t directionPin,  //!< Arduino pin connected to DIR-
                 const uint8_t enablePin      //!< Arduino pin connected to ENA-
    );

    //! \brief Initializes the pins
    void Init();

    //! \brief Enables the motor
    void Enable();

    //! \brief Disables the motor
    void Disable();

    //! \brief Indicates whether the motor is enabled
    //! \return True if the motor is enabled, false otherwise
    bool IsEnabled() const;

    //! \brief Sets the motor rotation direction
    //! \param direction Direction command to apply
    void SetDirection(const Direction direction);

    //! \brief Gets the configured direction
    //! \return Current direction command
    Direction GetDirection() const;

    //! \brief Generates one step pulse
    //! \param pulseWidth_us Pulse active duration in microseconds
    void Step(const uint32_t pulseWidth_us = 10u);

    //! \brief Generates multiple step pulses
    //! \param stepCount Number of steps to generate
    //! \param pulseWidth_us Pulse active duration in microseconds
    void Steps(const uint32_t stepCount, const uint32_t pulseWidth_us);

   private:
    const uint8_t PulsePin_;      //!< Arduino pin connected to PUL-
    const uint8_t DirectionPin_;  //!< Arduino pin connected to DIR-
    const uint8_t EnablePin_;     //!< Arduino pin connected to ENA-
    Direction Direction_;         //!< Current direction command
    bool Enabled_;                //!< Current enable state
};

#endif  // STEPPERMOTOR_H
