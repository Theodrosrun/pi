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
// Configuration constants
const uint8_t C_LCDColumns = 16u;
const uint8_t C_LCDRows = 2u;
const uint16_t C_LCDStartupDelay_ms = 200u;
}  // namespace

LCDDisplay::LCDDisplay(const uint8_t registerSelectPin,
                       const uint8_t enablePin,
                       const uint8_t dataPin4,
                       const uint8_t dataPin5,
                       const uint8_t dataPin6,
                       const uint8_t dataPin7)
    : LCDController_(registerSelectPin, enablePin, dataPin4, dataPin5, dataPin6, dataPin7) {}

//_______________________________________________________________________________________________

void LCDDisplay::Init() {
    // Initialize the LCD with the specified number of columns and rows
    LCDController_.begin(C_LCDColumns, C_LCDRows);
    // Wait for the LCD to initialize
    delay(C_LCDStartupDelay_ms);
    // Clear the LCD display
    LCDController_.clear();
}

//_______________________________________________________________________________________________

void LCDDisplay::Display(const String& firstLine, const String& secondLine) {
    DisplayRow(0, firstLine);
    DisplayRow(1, secondLine);
}

//_______________________________________________________________________________________________

void LCDDisplay::DisplayRow(const uint8_t row, const String& text) {
    String line = text;

    LCDController_.setCursor(0, row);

    // Pad the line with spaces to ensure it fills the entire row
    while (line.length() < C_LCDColumns) {
        line += " ";
    }

    LCDController_.print(line.substring(0, C_LCDColumns));
}
