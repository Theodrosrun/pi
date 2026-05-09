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

namespace {
const uint8_t C_LCDColumns = 16u;
const uint8_t C_LCDRows = 2u;
const uint16_t C_LCDStartupDelay_ms = 200u;
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
    LCD_.begin(C_LCDColumns, C_LCDRows);
    delay(C_LCDStartupDelay_ms);
    LCD_.clear();
}

//_______________________________________________________________________________________________

void LCDDisplay::Update() {
    ButtonValue_ = analogRead(ButtonPin_);
    ButtonName_ = DecodeButtonName(ButtonValue_);
}

//_______________________________________________________________________________________________

void LCDDisplay::PrintLine(uint8_t row, const String& text) {
    LCD_.setCursor(0, row);

    String line = text;
    while (line.length() < C_LCDColumns) {
        line += " ";
    }

    LCD_.print(line.substring(0, C_LCDColumns));
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

const char* LCDDisplay::DecodeButtonName(int value) const {
    if (value < 50) {
        return "RIGHT";
    }
    if (value < 200) {
        return "UP";
    }
    if (value < 400) {
        return "DOWN";
    }
    if (value < 650) {
        return "LEFT";
    }
    if (value < 900) {
        return "SELECT";
    }

    return "NONE";
}
