// Debug flag
#define DEBUG_MODE
#include <Arduino.h>

#include "pins.hpp"

// The Pneumatic Class
class ps {
  // Holders
  int pVal;
  int lVal;

  // Shift functions
  void shift_up();
  void shift_down();
  void launch_con();

public:
  // Control functions
  void init();
  void read();
  void rest();
};

void ps::init() {
  // Init pins for LED, Shifting, "Launch Control" & No-lift shift
  pinMode(led, OUTPUT);
  pinMode(up, OUTPUT);
  pinMode(down, OUTPUT);
  pinMode(clutch, OUTPUT);
  pinMode(launch, OUTPUT);
  pinMode(nls, OUTPUT);

  // Flip the LED high for sign of life
  digitalWrite(led, HIGH);
}

void ps::read() {
  pVal = analogRead(Paddles);
  lVal = analogRead(LaunchButton);

  // 152 is stock reading
  // 121 is the Launch Button
  // 112 is the Up Paddle
  // 101 is the Down Paddle
  if (pVal < 145) {
    delay(5);
    if (pVal > 107 && pVal < 117) {
      shift_up();
    } else if (pVal > 96 && pVal < 106) {
      shift_down();
    }
  } else if (lVal > 115 && lVal < 126) {
    delay(5);
    launch_con();
  }
}

void ps::rest() {
  digitalWrite(down, LOW);
  digitalWrite(up, LOW);
  digitalWrite(nls, LOW);
  digitalWrite(clutch, LOW);
  digitalWrite(launch, LOW);
}

void ps::shift_up() {
  digitalWrite(nls, HIGH);
  digitalWrite(up, HIGH);
  delay(300);
  while (pVal < 145) {
#ifdef DEBUG_MODE
    Serial.println("Up is being held");
#endif
    rest();
    pVal = analogRead(Paddles);
  }
}

void ps::shift_down() {
  digitalWrite(clutch, HIGH);
  digitalWrite(launch, HIGH);
  delay(100);
  digitalWrite(down, HIGH);
  delay(100);
  digitalWrite(clutch, LOW);
  delay(100);
  digitalWrite(launch, LOW);
  while (pVal < 145) {
#ifdef DEBUG_MODE
    Serial.println("Down is being held");
#endif
    rest();
    pVal = analogRead(Paddles);
  }
}

void ps::launch_con() {
  while (lVal > 115 && lVal < 126) {
    digitalWrite(clutch, HIGH);
    digitalWrite(launch, HIGH);
    lVal = analogRead(LaunchButton);
  }
#ifdef DEBUG_MODE
  Serial.println("Go fast");
#endif
  digitalWrite(launch, LOW);
  delay(2000);
  digitalWrite(clutch, LOW);
}
