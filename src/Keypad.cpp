//_______________________________________________________________________________________________
//
// Copyright (C) 2026                      HES-SO Master                         CH-1004 Lausanne
//_______________________________________________________________________________________________
//
// PROJECT  PI
//_______________________________________________________________________________________________
//
//! \file    Keypad.cpp
//! \brief   Peripheral Abstraction Layer keypad class
//!
//! \author  Theodros Mulugeta
//_______________________________________________________________________________________________

#include "Keypad.h"

#include <Arduino.h>

namespace {
// Button thresholds
const int32_t C_ButtonRightThreshold = 50;
const int32_t C_ButtonUpThreshold = 200;
const int32_t C_ButtonDownThreshold = 400;
const int32_t C_ButtonLeftThreshold = 650;
const int32_t C_ButtonSelectThreshold = 900;
}  // namespace

Keypad::Keypad(const uint8_t buttonPin) : ButtonPin_(buttonPin), ButtonPressed_(Button::None) {}

//_______________________________________________________________________________________________

void Keypad::Update() {
    const int32_t buttonValue = static_cast<int32_t>(analogRead(ButtonPin_));
    ButtonPressed_ = DecodePressedButton(buttonValue);
}

//_______________________________________________________________________________________________

Keypad::Button Keypad::GetPressedButton() const {
    return ButtonPressed_;
}

//_______________________________________________________________________________________________

const char* Keypad::GetPressedButtonName() const {
    if (ButtonPressed_ == Button::Right) {
        return "Right";
    }
    if (ButtonPressed_ == Button::Up) {
        return "Up";
    }
    if (ButtonPressed_ == Button::Down) {
        return "Down";
    }
    if (ButtonPressed_ == Button::Left) {
        return "Left";
    }
    if (ButtonPressed_ == Button::Select) {
        return "Select";
    }

    return "None";
}

//_______________________________________________________________________________________________

Keypad::Button Keypad::DecodePressedButton(const int32_t value) const {
    if (value < C_ButtonRightThreshold) {
        return Button::Right;
    }
    if (value < C_ButtonUpThreshold) {
        return Button::Up;
    }
    if (value < C_ButtonDownThreshold) {
        return Button::Down;
    }
    if (value < C_ButtonLeftThreshold) {
        return Button::Left;
    }
    if (value < C_ButtonSelectThreshold) {
        return Button::Select;
    }

    return Button::None;
}
