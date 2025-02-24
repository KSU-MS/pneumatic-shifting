// BTW you can delete this file, the whole point of Git is so that if you ever
// *really* fuck some shit up, you can always revert back to the last working
// commit, that and you can pull up old commits on github on a secondary monitor
// or background tabs so you can still refrence old code ~ Chance

#include "main.hpp"
#include <Arduino.h>
uint16_t Paddle_Value;
uint16_t Launch_Value;

// TEMPORARY VARIABLES, WILL BE SET UP OR REMOVED ONCE SIGNAL STRENGTHS ARE
// FOUND
int AcceptedLaunchValue = 1;
int AcceptedPaddleValueUp = 1;
int AcceptedPaddleValueDown = 1;
int c = 0;

// Reset for up shifts
void upPinReset() {
  digitalWrite(UP_PIN, LOW);
  digitalWrite(NO_LIFT_SHIFT, LOW);
  digitalWrite(DOWN_PIN, LOW);
  digitalWrite(CLUTCH_PIN, LOW);
  digitalWrite(LAUNCH_CONTROL, LOW);
}

// Reset for down shifts
void downPinReset() {}

class state_machine {
private:
  enum states { rest = 0, up_shift = 1, down_shift = 2, launch = 3 };

  states current_state = rest;

public:
  void next_state();
  inline void set_state(uint8_t next_target) {
    this->current_state = next_target;
  };
};

void state_machine::next_state() {
  switch (this->current_state) {
  case rest:
    /* code */

    // check analog pins
    if (fella == true) {
      set_state(1);
    }
    break;

  default:
    break;
  }
}

void setup() {
  pinMode(UP_PIN, OUTPUT);
  pinMode(NO_LIFT_SHIFT, OUTPUT);
  pinMode(LAUNCH_CONTROL, OUTPUT);
  pinMode(DOWN_PIN, OUTPUT);
  pinMode(CLUTCH_PIN, OUTPUT);

  // Low Pin Setup to prevent accidental shifts
  digitalWrite(UP_PIN, LOW);
  digitalWrite(NO_LIFT_SHIFT, LOW);
  digitalWrite(LAUNCH_CONTROL, LOW);
  digitalWrite(DOWN_PIN, LOW);
  digitalWrite(CLUTCH_PIN, LOW);
}

void loop() {
  // Button Reads
  Paddle_Value = analogRead(PADDLE_SIGNAL);
  Launch_Value = analogRead(LAUNCH_BUTTON_SIGNAL);

  // Launch Control
  while (Launch_Value == AcceptedLaunchValue /*Temp*/) {
    digitalWrite(CLUTCH_PIN, HIGH);
    Launch_Value = analogRead(
        LAUNCH_BUTTON_SIGNAL); /*Re-reads the button to update the signal and
                                  see if it is still pressed*/
  }
  digitalWrite(CLUTCH_PIN, LOW);

  // Paddle Shifting Up
  while (Paddle_Value == AcceptedPaddleValueUp /*Temp*/) {
    digitalWrite(NO_LIFT_SHIFT, HIGH);
    digitalWrite(UP_PIN, HIGH);
    int upSignal = analogRead(UP_SIGNAL);
    // Loop to wait for the cylinder to reach up
    while (upSignal != 1 /*Temp*/) {
      upSignal = analogRead(UP_SIGNAL);
      Serial.print("Waiting on pin to reach height");
      c = c + 1;
      // Safety Measure in case the sensor never reads the value and prevents
      // infinite loop.
      if (c == 100000 /*Temp, Value To Be Decided Later*/) {
        c = 0;
        break;
      }
    }
  }
  upPinReset();

  // Paddle Shifting Down
  while (Paddle_Value == AcceptedPaddleValueDown /*Temp*/) {
    digitalWrite(CLUTCH_PIN, HIGH);
    digitalWrite(DOWN_PIN, HIGH);
    int downSignal = analogRead(DOWN_SIGNAL);
    // Loop to wait for the cylinder to reach up
    while (downSignal != 1 /*Temp*/) {
      downSignal = analogRead(DOWN_SIGNAL);
      Serial.print("Waiting on pin to reach height");
      c = c + 1;
      // Safety Measure in case the sensor never reads the value and prevents
      // infinite loop.
      if (c = 100000 /*Temp, Value To Be Decided Later*/) {
        c = 0;
        break;
      }
    }
  }
  downPinReset(); // Reseting the down pins
}
