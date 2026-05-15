//_______________________________________________________________________________________________
//
// Copyright (C) 2026                      HES-SO Master                         CH-1004 Lausanne
//_______________________________________________________________________________________________
//
// PROJECT  PI
//_______________________________________________________________________________________________
//
//! \file    Keypad.h
//! \brief   Peripheral Abstraction Layer keypad class
//!
//! \author  Theodros Mulugeta
//_______________________________________________________________________________________________

#ifndef KEYPAD_H
#define KEYPAD_H

#include <stdint.h>

//! \brief Keypad class
class Keypad {
   public:
    //! \brief Button identifiers
    enum class Button : uint8_t { None, Right, Up, Down, Left, Select };

    //! \brief Constructor
    Keypad(const uint8_t buttonPin  //!< Button analog input pin
    );

    //! \brief Updates the button state
    void Update();

    //! \brief Gets the current pressed button
    //! \return Current pressed button identifier
    Button GetPressedButton() const;

    //! \brief Gets the current pressed button name
    //! \return Current pressed button name
    const char* GetPressedButtonName() const;

   private:
    //! \brief Decodes a raw button reading
    //! \param value Raw analog button value
    //! \return Button identifier matching the current threshold
    Button DecodePressedButton(const int32_t value) const;

   private:
    const uint8_t ButtonPin_;  //!< Button analog input pin

    Button ButtonPressed_;  //!< Last decoded button
};

#endif  // KEYPAD_H
