#include <Arduino.h>
#include <EEPROM.h>
#include <Metro.h>

// Pins
int led = 13;
int up = 7;
int down = 8;
int clutch = 9;
int launch = 10;
int nls = 11;
int LaunchButton = 22;
int Paddles = 23;

// Other variables
// #define DEBUG_MODE
int shift_state;
Metro write_timer = Metro(1000);

// Currently only using a uint16_t and two bytes, which is still good up to
// 65535 and makes this code more complecated than it needs to be, but wanted to
// show how it works with unions for encoding any data type
union {
  uint16_t i;
  byte b[2];
} total_shifts;

void setup() {
#ifdef DEBUG_MODE
  Serial.begin(9600);
#endif

  // Init pins for LED, Shifting, "Launch Control" & No-lift shift
  pinMode(led, OUTPUT);
  pinMode(up, OUTPUT);
  pinMode(down, OUTPUT);
  pinMode(clutch, OUTPUT);
  pinMode(launch, OUTPUT);
  pinMode(nls, OUTPUT);

  shift_state = 3;
  digitalWrite(led, HIGH);

  // Read the amount of shifts out of memory
  for (int i = 0; i < 2; i++) {
    total_shifts.b[i] = EEPROM.read(i);
  }
}

void Read(int PaddleValue, int LaunchValue) {
  if (PaddleValue < 145) {
    shift_state = 1;
  } else if (LaunchValue > 115 && LaunchValue < 126) {
    shift_state = 2;
  } else {
    shift_state = 3;
  }
}

void Wait() {
#ifdef DEBUG_MODE
  Serial.println("Wait");
#endif
  digitalWrite(down, LOW);
  digitalWrite(up, LOW);
  digitalWrite(nls, LOW);
  digitalWrite(clutch, LOW);
  digitalWrite(launch, LOW);
}

void ShiftDown(int PaddleValue) {
#ifdef DEBUG_MODE
  Serial.println("Inside down function");
#endif
  digitalWrite(clutch, HIGH);
  digitalWrite(launch, HIGH);
  delay(100);
  digitalWrite(down, HIGH);
  delay(100);
  digitalWrite(clutch, LOW);
  delay(100);
  digitalWrite(launch, LOW);
  while (PaddleValue < 145) {
#ifdef DEBUG_MODE
    Serial.println("Down is being held");
#endif
    Wait();
    PaddleValue = analogRead(Paddles);
  }
}

void ShiftUp(int PaddleValue) {
#ifdef DEBUG_MODE
  Serial.println("Inside up function");
#endif
  digitalWrite(nls, HIGH);
  digitalWrite(up, HIGH);
  delay(300);
  while (PaddleValue < 145) {
#ifdef DEBUG_MODE
    Serial.println("Up is being held");
#endif
    Wait();
    PaddleValue = analogRead(Paddles);
  }
}

// Launch function
void Launch(int LaunchValue) {
  while (LaunchValue > 115 && LaunchValue < 126) {
    digitalWrite(clutch, HIGH);
    digitalWrite(launch, HIGH);
    LaunchValue = analogRead(LaunchButton);
  }
#ifdef DEBUG_MODE
  Serial.println("Go fast");
#endif
  digitalWrite(launch, LOW);
  delay(2000);
  digitalWrite(clutch, LOW);
}

void loop() {
  // Read and print LaunchButton value & Paddles value
  int LaunchValue = analogRead(LaunchButton);
  int PaddleValue = analogRead(Paddles);

  // Write the total amount of shifts if timer has ticked, this is done because
  // according to this https://docs.arduino.cc/learn/built-in-libraries/eeprom/,
  // an EEPROM write takes 3.3 ms to complete, and I didn't want it to block up
  // the state machine
  if (write_timer.check()) {
    for (int i = 0; i < 2; i++) {
      EEPROM.write(i, total_shifts.b[i]);
    }
  }

  switch (shift_state) {
  // Read state
  case 0:
    Read(PaddleValue, LaunchValue);
    break;

  // Shift state
  case 1:
#ifdef DEBUG_MODE
    Serial.println(PaddleValue);
#endif

    // 152 is stock reading
    // 121 is the Launch Button
    // 112 is the Up Paddle
    // 101 is the Down Paddle
    if (PaddleValue > 107 && PaddleValue < 117) {
      total_shifts.i += 1;
      ShiftUp(PaddleValue);
    } else if (PaddleValue > 96 && PaddleValue < 106) {
      total_shifts.i += 1;
      ShiftDown(PaddleValue);
    } else {
#ifdef DEBUG_MODE
      Serial.println("Failed to read shift value");
#endif
      shift_state = 0;
    }
    break;

  // Launch state
  case 2:
#ifdef DEBUG_MODE
    Serial.println(LaunchValue);
#endif
    Launch(LaunchValue);
    shift_state = 3;
    break;

  // Wait state
  case 3:
    Wait();
    shift_state = 0;
    break;
  }
}
