//_______________________________________________________________________________________________
//
// Copyright (C) 2026                      HES-SO Master                         CH-1004 Lausanne
//_______________________________________________________________________________________________
//
// PROJECT  PI
//_______________________________________________________________________________________________
//
//! \file    LCDDisplay.cpp
//! \brief   Peripheral Abstraction Layer LCD display class
//!
//! \author  Theodros Mulugeta
//_______________________________________________________________________________________________

#include "LCDDisplay.h"

#include <Arduino.h>

namespace {
// Configuration
const uint8_t C_LCDColumns = 16u;
const uint8_t C_LCDRows = 2u;
const uint16_t C_LCDStartupDelay_ms = 200u;
// Button thresholds
const int C_ButtonRightThreshold = 50;
const int C_ButtonUpThreshold = 200;
const int C_ButtonDownThreshold = 400;
const int C_ButtonLeftThreshold = 650;
const int C_ButtonSelectThreshold = 900;
}  // namespace

LCDDisplay::LCDDisplay(uint8_t registerSelectPin,
                       uint8_t enablePin,
                       uint8_t dataPin4,
                       uint8_t dataPin5,
                       uint8_t dataPin6,
                       uint8_t dataPin7,
                       uint8_t buttonPin)
    : LCD_(registerSelectPin, enablePin, dataPin4, dataPin5, dataPin6, dataPin7),
      ButtonPin_(buttonPin),
      ButtonValue_(1023),
      ButtonName_("NONE") {}

//_______________________________________________________________________________________________

void LCDDisplay::Init() {
    // Initialize the LCD with the specified number of columns and rows
    LCD_.begin(C_LCDColumns, C_LCDRows);
    // Wait for the LCD to initialize
    delay(C_LCDStartupDelay_ms);
    // Clear the LCD display
    LCD_.clear();
}

//_______________________________________________________________________________________________

void LCDDisplay::Update() {
    ButtonValue_ = analogRead(ButtonPin_);
    ButtonName_ = DecodeButtonName(ButtonValue_);
}

//_______________________________________________________________________________________________

void LCDDisplay::Display(const String& firstLine, const String& secondLine) {
    DisplayRow(0, firstLine);
    DisplayRow(1, secondLine);
}

//_______________________________________________________________________________________________

int LCDDisplay::GetButtonValue() const {
    return ButtonValue_;
}

//_______________________________________________________________________________________________

const char* LCDDisplay::GetButtonName() const {
    return ButtonName_;
}

//_______________________________________________________________________________________________

void LCDDisplay::DisplayRow(uint8_t row, const String& text) {
    LCD_.setCursor(0, row);

    String line = text;
    while (line.length() < C_LCDColumns) {
        line += " ";
    }

    LCD_.print(line.substring(0, C_LCDColumns));
}

//_______________________________________________________________________________________________

const char* LCDDisplay::DecodeButtonName(int value) const {
    if (value < C_ButtonRightThreshold) {
        return "RIGHT";
    }
    if (value < C_ButtonUpThreshold) {
        return "UP";
    }
    if (value < C_ButtonDownThreshold) {
        return "DOWN";
    }
    if (value < C_ButtonLeftThreshold) {
        return "LEFT";
    }
    if (value < C_ButtonSelectThreshold) {
        return "SELECT";
    }

    return "NONE";
}
