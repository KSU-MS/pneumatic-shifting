#include <Arduino.h>

int led = 13;
int up = 7;
int down = 8;
int clutch = 9;
int launch = 10;
int nls = 11;
int LaunchButton = 22;
int Paddles = 23;
int countup = 0;
int countuprel = 1;
int countdown = 0;
int countdownrel = 1;

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
  Serial.println("Launch");
  int LaunchValue = analogRead(LaunchButton);
  // Set sensor to read Green only
  while (LaunchValue > 115 && LaunchValue < 126) {
    digitalWrite(clutch, HIGH);
    digitalWrite(launch, HIGH);
    Serial.println("inside");
    int LaunchValue = analogRead(LaunchButton);
    if (LaunchValue > 145 && LaunchValue < 155) {
      Serial.println("break");
      break;
    }
  }
  digitalWrite(launch, LOW);
  delay(2000);
  digitalWrite(clutch, LOW);
}

void loop() {
  // 152 is stock reading
  // 121 is the Launch Button
  // 112 is the Up Paddle
  // 101 is the Down Paddle
  delay(50);

  digitalWrite(led, HIGH);

  // Read and print LaunchButton value
  int LaunchValue = analogRead(LaunchButton);
  Serial.println(LaunchValue);

  // Read and print Paddles value
  int PaddleValue = analogRead(Paddles);
  Serial.println(Paddles);

  // Up Shift (112)
  if (PaddleValue > 108 && PaddleValue < 117 && countuprel > countup) {
    // Serial.println("Inside up loop");
    digitalWrite(nls, HIGH);
    digitalWrite(up, HIGH);
    delay(300);
    countup++;
  }

  // Wait for release
  else if (PaddleValue > 145 && countup == countuprel) {
    // Serial.println("Hold up loop");
    countuprel++;
    Serial.println(countuprel);
    delay(100);
  }

  // Down Shift (101)
  else if (PaddleValue > 96 && PaddleValue < 106 && countdownrel > countdown) {
    // Serial.println("Inside down loop");
    digitalWrite(clutch, HIGH);
    digitalWrite(launch, HIGH);
    delay(100);
    digitalWrite(down, HIGH);
    delay(100);
    digitalWrite(clutch, LOW);
    delay(100);
    digitalWrite(launch, LOW);
    countdown++;
  }

  // Wait for release
  else if (PaddleValue > 145 && countdown == countdownrel) {
    // Serial.println("Hold down loop");
    countdownrel++;
    delay(100);
  }

  // LaunchControl
  else if (LaunchValue > 115 && LaunchValue < 126) {
    delay(1000);
    Launch();
  }

  // Default state
  else {
    digitalWrite(down, LOW);
    digitalWrite(up, LOW);
    digitalWrite(nls, LOW);
    digitalWrite(clutch, LOW);
    digitalWrite(launch, LOW);
  }
}
