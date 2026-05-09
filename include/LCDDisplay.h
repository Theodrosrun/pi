//_______________________________________________________________________________________________
//
// Copyright (C) 2026                      HES-SO Master                         CH-1004 Lausanne
//_______________________________________________________________________________________________
//
// PROJECT  PI
//_______________________________________________________________________________________________
//
//! \file    LCDDisplay.h
//! \brief   Peripheral Abstraction Layer LCD display class
//!
//! \author  Theodros Mulugeta
//_______________________________________________________________________________________________

#ifndef LCDDISPLAY_H
#define LCDDISPLAY_H

#include <LiquidCrystal.h>
#include <stdint.h>

//! \brief LCD display class
class LCDDisplay {
   public:
    //! \brief Constructor
    LCDDisplay(uint8_t registerSelectPin,  //!< LCD RS pin
               uint8_t enablePin,          //!< LCD enable pin
               uint8_t dataPin4,           //!< LCD data pin D4
               uint8_t dataPin5,           //!< LCD data pin D5
               uint8_t dataPin6,           //!< LCD data pin D6
               uint8_t dataPin7,           //!< LCD data pin D7
               uint8_t buttonPin           //!< LCD keypad analog button pin
    );

    //! \brief Initializes the LCD
    void Init();

    //! \brief Updates the button state
    void Update();

    //! \brief Displays the two LCD lines
    //! \param firstLine Text to display on the first line
    //! \param secondLine Text to display on the second line. If empty, the line is cleared.
    void Display(const String& firstLine, const String& secondLine = "");

    //! \brief Gets the current button analog value
    //! \return Raw analog button value
    int GetButtonValue() const;

    //! \brief Gets the current button name
    //! \return Current button name
    const char* GetButtonName() const;

   private:
    //! \brief Displays a padded line on the LCD
    //! \param row LCD row index
    //! \param text Text to display
    void DisplayRow(uint8_t row, const String& text);

    //! \brief Decodes a raw button reading
    //! \param value Raw analog button value
    //! \return Button name matching the current threshold
    const char* DecodeButtonName(int value) const;

    LiquidCrystal LCD_;        //!< LCD driver instance
    const uint8_t ButtonPin_;  //!< LCD keypad analog button pin
    int ButtonValue_;          //!< Last raw button value
    const char* ButtonName_;   //!< Last decoded button name
};

#endif  // LCDDISPLAY_H
