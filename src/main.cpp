#include <Arduino.h>
#include <MS6205.h>
int const shiftRegisterLatchPin = D8;  // GPIO15 = Pin D8 on NodeMCU boards. Pin 12 on 74HC595.
int const shiftRegisterClockPin = D5;  // GPIO14 = Pin D5 on NodeMCU boards. Pin 11 on 74HC595.
int const shiftRegisterDataPin = D7;   // GPIO13 = Pin D7 on NodeMCU boards. Pin 14 on 74HC595.
int const displaySetPositionPin = D6;  // GPIO12 = Pin D6 on NodeMCU boards. Pin 16A on MS6205.
int const displaySetCharacterPin = D4; // GPIO2  = Pin D4 on NodeMCU boards. Pin 16B on MS6205.
int const displayClearPin = D1;        // GPIO5  = Pin D1 on NodeMCU boards. Pin 18A on MS6205.
int const displaySelectPage0Pin = D2;  // GPIO4  = Pin D2 on NodeMCU boards. Pin  2A on MS6205.
int const displaySelectPage1Pin = D3;  // GPIO0  = Pin D3 on NodeMCU boards. Pin  2B on MS6205.
MS6205 display(shiftRegisterLatchPin, shiftRegisterClockPin, shiftRegisterDataPin, displaySetPositionPin, displaySetCharacterPin, displayClearPin);
void setup()
{
  Serial.begin(115200);
  delay(4000);
}
void loop()
{
  display.clear();
  //A
  // B
  //  C
  //   D
  //    E
  //     F
  //      G
  //       H
  //        I
  //         J
  display.setCursor(0, 0);
  display.write("A");
  delay(1000);
  display.setCursor(1, 1);
  display.write("B");
  delay(1000);
  display.setCursor(2, 2);
  display.write("C");
  delay(1000);
  display.setCursor(3, 3);
  display.write("D");
  delay(1000);
  display.setCursor(4, 4);
  display.write("E");
  delay(1000);
  display.setCursor(5, 5);
  display.write("F");
  delay(1000);
  display.setCursor(6, 6);
  display.write("G");
  delay(1000);
  display.setCursor(7, 7);
  display.write("H");
  delay(1000);
  display.setCursor(8, 8);
  display.write("I");
  delay(1000);
  display.setCursor(9, 9);
  display.write("J");
  delay(1000);
}