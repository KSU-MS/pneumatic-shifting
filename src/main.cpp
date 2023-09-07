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
  Serial.begin(9600);

  // Init pins for LED, Shifting, "Launch Control" & No-lift shift
  pinMode(led, OUTPUT);
  pinMode(up, OUTPUT);
  pinMode(down, OUTPUT);
  pinMode(clutch, OUTPUT);
  pinMode(launch, OUTPUT);
  pinMode(nls, OUTPUT);

  shift_state = 3;
  digitalWrite(led, HIGH);
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
  Serial.println("Wait");
  digitalWrite(down, LOW);
  digitalWrite(up, LOW);
  digitalWrite(nls, LOW);
  digitalWrite(clutch, LOW);
  digitalWrite(launch, LOW);
}

// Launch function
void Launch(int LaunchValue) {
  while (LaunchValue > 115 && LaunchValue < 126) {
    digitalWrite(clutch, HIGH);
    digitalWrite(launch, HIGH);
    LaunchValue = analogRead(LaunchButton);
  }
  Serial.println("Go fast");
  digitalWrite(launch, LOW);
  delay(2000);
  digitalWrite(clutch, LOW);
}

void ShiftDown(int PaddleValue) {
  Serial.println("Inside down function");
  digitalWrite(clutch, HIGH);
  digitalWrite(launch, HIGH);
  delay(100);
  digitalWrite(down, HIGH);
  delay(100);
  digitalWrite(clutch, LOW);
  delay(100);
  digitalWrite(launch, LOW);
  while (PaddleValue < 145) {
    Serial.println("Hold");
    Wait();
    PaddleValue = analogRead(Paddles);
  }
}

void ShiftUp(int PaddleValue) {
  Serial.println("Inside up function");
  digitalWrite(nls, HIGH);
  digitalWrite(up, HIGH);
  delay(300);
  while (PaddleValue < 145) {
    Serial.println("Hold");
    Wait();
    PaddleValue = analogRead(Paddles);
  }
}

void loop() {
  // Read and print LaunchButton value & Paddles value
  int LaunchValue = analogRead(LaunchButton);
  int PaddleValue = analogRead(Paddles);

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
    Serial.println(PaddleValue);
    delay(15);
    Serial.println(PaddleValue);
    if (PaddleValue > 107 && PaddleValue < 117) {
      ShiftUp(PaddleValue);
    } else if (PaddleValue > 96 && PaddleValue < 106) {
      ShiftDown(PaddleValue);
    } else {
      Serial.println("Failed to read shift value");
      shift_state = 0;
    }
    break;

  // Launch state
  case 2:
    Serial.println(LaunchValue);
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
