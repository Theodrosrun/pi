#include <Arduino.h>
#include <LiquidCrystal.h>

LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

String getButtonName(int value) {
    if (value < 50) return "RIGHT";
    if (value < 200) return "UP";
    if (value < 400) return "DOWN";
    if (value < 650) return "LEFT";
    if (value < 900) return "SELECT";
    return "NONE";
}

void setup() {
    Serial.begin(9600);

    lcd.begin(16, 2);
    delay(200);
    lcd.clear();

    lcd.setCursor(0, 0);
    lcd.print("LCD Keypad");
    lcd.setCursor(0, 1);
    lcd.print("Ready");
}

void loop() {
    int value = analogRead(A0);
    String button = getButtonName(value);

    Serial.print("Button = ");
    Serial.println(button);

    lcd.setCursor(0, 0);
    lcd.print("Button:        ");

    lcd.setCursor(0, 1);
    lcd.print(button);
    lcd.print("          ");

    delay(200);
}
