#include <Arduino.h>

int led = 13;
int up = 7;
int down = 8;
int clutch = 9;
int launch = 10;
int nls = 11;
int LaunchButton = 22;
int Paddles = 23;
int shift_state = 0;

void setup() {
  Serial.begin(9600);

  // Init pins for LED, Shifting, "Launch Control" & No-lift shift
  pinMode(led, OUTPUT);
  pinMode(up, OUTPUT);
  pinMode(down, OUTPUT);
  pinMode(clutch, OUTPUT);
  pinMode(launch, OUTPUT);
  pinMode(nls, OUTPUT);
}

// Launch function
void Launch() {
  int LaunchValue = analogRead(LaunchButton);
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
    if (PaddleValue < 145) {
      shift_state = 1;
    } else if (LaunchValue < 145) {
      shift_state = 3;
    } else {
      shift_state = 4;
    }

  // Shift state
  case 1:
    if (PaddleValue > 108 && PaddleValue < 117) {
      // Serial.println("Inside up case");
      digitalWrite(nls, HIGH);
      digitalWrite(up, HIGH);
      delay(300);
      shift_state = 2;
    } else {
      // Serial.println("Inside down case");
      digitalWrite(clutch, HIGH);
      digitalWrite(launch, HIGH);
      delay(100);
      digitalWrite(down, HIGH);
      delay(100);
      digitalWrite(clutch, LOW);
      delay(100);
      digitalWrite(launch, LOW);
      shift_state = 2;
    }

  // Hold state
  case 2:
    while (PaddleValue < 145) {
      // Serial.println("Holding shift");
      delay(100);
      shift_state = 2;
    }
    shift_state = 0;

  // Launch state
  case 3:
    // Serial.println("Launch");
    digitalWrite(led, HIGH);
    Launch();
    digitalWrite(led, LOW);
    shift_state = 0;

  // Default state
  case 4:
    // Serial.println("Waiting for input");
    digitalWrite(down, LOW);
    digitalWrite(up, LOW);
    digitalWrite(nls, LOW);
    digitalWrite(clutch, LOW);
    digitalWrite(launch, LOW);
    shift_state = 0;
  }
}
