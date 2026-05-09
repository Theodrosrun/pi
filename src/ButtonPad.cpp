//_______________________________________________________________________________________________
//
// Copyright (C) 2026                      HES-SO Master                         CH-1004 Lausanne
//_______________________________________________________________________________________________
//
// PROJECT  PI
//_______________________________________________________________________________________________
//
//! \file    ButtonPad.cpp
//! \brief   Peripheral Abstraction Layer button pad class
//!
//! \author  Theodros Mulugeta
//_______________________________________________________________________________________________

#include "ButtonPad.h"

#include <Arduino.h>

namespace {
// Button thresholds
const int32_t C_ButtonRightThreshold = 50;
const int32_t C_ButtonUpThreshold = 200;
const int32_t C_ButtonDownThreshold = 400;
const int32_t C_ButtonLeftThreshold = 650;
const int32_t C_ButtonSelectThreshold = 900;
}  // namespace

ButtonPad::ButtonPad(const uint8_t buttonPin)
    : ButtonPin_(buttonPin), ButtonPressed_(Button::None) {}

//_______________________________________________________________________________________________

void ButtonPad::Update() {
    const int32_t buttonValue = static_cast<int32_t>(analogRead(ButtonPin_));
    ButtonPressed_ = DecodePressedButton(buttonValue);
}

//_______________________________________________________________________________________________

ButtonPad::Button ButtonPad::GetPressedButton() const {
    return ButtonPressed_;
}

//_______________________________________________________________________________________________

const char* ButtonPad::GetPressedButtonName() const {
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

ButtonPad::Button ButtonPad::DecodePressedButton(const int32_t value) const {
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
