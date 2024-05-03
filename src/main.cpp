#include <Arduino.h>
#include <EEPROM.h>

#include "main.hpp"

ps shifter;

void setup() {
  Serial.begin(9600);

  shifter.init();
}

void loop() {
  shifter.read();

  shifter.rest();
}
