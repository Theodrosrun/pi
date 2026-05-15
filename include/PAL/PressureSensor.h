//_______________________________________________________________________________________________
//
// Copyright (C) 2026                      HES-SO Master                         CH-1004 Lausanne
//_______________________________________________________________________________________________
//
// PROJECT  PI
//_______________________________________________________________________________________________
//
//! \file    PressureSensor.h
//! \brief   Peripheral Abstraction Layer Pressure Sensor class
//!
//! \author  Theodros Mulugeta
//_______________________________________________________________________________________________

#ifndef PRESSURESENSOR_H
#define PRESSURESENSOR_H

#include <HX711.h>
#include <stdint.h>

//! \brief Pressure Sensor class
class PressureSensor {
   public:
    //! \brief Constructor
    PressureSensor(const uint8_t dataPin,  //!< The data pin connected to the HX711
                   const uint8_t clockPin  //!< The clock pin connected to the HX711
    );

    //! \brief Initializes the sensor
    void Init();

    //! \brief Performs sensor tare using the current unloaded force
    void Tare();

    //! \brief Updates the sensor readings
    //! \param syringeDiameter_mm Syringe inner diameter in millimeters
    //! \return True if the sensor is ready and the update was successful, false otherwise
    bool Update(const float syringeDiameter_mm);

    //! \brief Gets the computed force in [N]
    //! \return The computed force in [N]
    float GetForce_n() const;

    //! \brief Gets the computed pressure in [bars]
    //! \return The computed pressure in [bars]
    float GetPressure_bar() const;

   private:
    //! \brief Computes the area of the syringe in square meters
    //! \param syringeDiameter_mm Syringe inner diameter in millimeters
    //! \return The area of the syringe in square meters
    float ComputeAreaSquareMeters(const float syringeDiameter_mm) const;

   private:
    const uint8_t DataPin_;   //!< The data pin connected to the HX711
    const uint8_t ClockPin_;  //!< The clock pin connected to the HX711

    HX711 Scale_;         //!< The HX711 load cell amplifier instance
    int32_t Offset_;      //!< The offset value for the sensor
    float Force_n_;       //!< The computed force in [N]
    float Pressure_bar_;  //!< The computed pressure in [bars]
};

#endif  // PRESSURESENSOR_H
