#include <Arduino.h>

int led = 13;
int up = 7;
int down = 8;
int clutch = 9;
int launch = 10;
int nls = 11;
int LaunchButton = 22;
int Paddles = 23;
int shift_state;

void setup() {
  // Serial.begin(9600);

  // Init pins for LED, Shifting, "Launch Control" & No-lift shift
  pinMode(led, OUTPUT);
  pinMode(up, OUTPUT);
  pinMode(down, OUTPUT);
  pinMode(clutch, OUTPUT);
  pinMode(launch, OUTPUT);
  pinMode(nls, OUTPUT);

  shift_state = 4;
}

void Wait() {
  // Serial.println("Wait");
  digitalWrite(down, LOW);
  digitalWrite(up, LOW);
  digitalWrite(nls, LOW);
  digitalWrite(clutch, LOW);
  digitalWrite(launch, LOW);
}

void Hold(int PaddleValue) {
  while (PaddleValue < 145) {
    Serial.println("Hold");
    delay(100);
    PaddleValue = digitalRead(PaddleValue);
    Wait();
  }
}

void Read(int PaddleValue, int LaunchValue) {
  if (PaddleValue < 145) {
    shift_state = 1;
  } else if (LaunchValue > 115 && LaunchValue < 126) {
    shift_state = 3;
  } else {
    shift_state = 4;
  }
}

// Launch function
void Launch(int LaunchValue) {
  while (LaunchValue > 115 && LaunchValue < 126) {
    digitalWrite(clutch, HIGH);
    digitalWrite(launch, HIGH);
    LaunchValue = analogRead(LaunchButton);
  }
  // Serial.println("Go fast");
  digitalWrite(launch, LOW);
  delay(2000);
  digitalWrite(clutch, LOW);
}

void ShiftDown() {
  // Serial.println("Inside down function");
  digitalWrite(clutch, HIGH);
  digitalWrite(launch, HIGH);
  delay(100);
  digitalWrite(down, HIGH);
  delay(100);
  digitalWrite(clutch, LOW);
  delay(100);
  digitalWrite(launch, LOW);
}

void ShiftUp() {
  // Serial.println("Inside up function");
  digitalWrite(nls, HIGH);
  digitalWrite(up, HIGH);
  delay(300);
}

void loop() {
  delay(50);

  // Read and print LaunchButton value & Paddles value
  int LaunchValue = analogRead(LaunchButton);
  // Serial.println(LaunchValue);
  int PaddleValue = analogRead(Paddles);
  // Serial.println(Paddles);

  // 152 is stock reading
  // 121 is the Launch Button
  // 112 is the Up Paddle
  // 101 is the Down Paddle

  switch (shift_state) {
  // Read state
  case 0:
    Read(PaddleValue, LaunchValue);
    break;

  // Shift state
  case 1:
    if (PaddleValue > 107 && PaddleValue < 117) {
      ShiftUp();
      shift_state = 2;
    } else {
      ShiftDown();
      shift_state = 2;
    }
    break;

  // Hold state
  case 2:
    Hold(PaddleValue);
    shift_state = 4;
    break;

  // Launch state
  case 3:
    Launch(LaunchValue);
    shift_state = 4;
    break;

  // Wait state
  case 4:
    Wait();
    shift_state = 0;
    break;
  }
}
