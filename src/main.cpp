#include <Arduino.h>

int led = 13;
int up = 7;
int down = 8;
int clutch = 9;
int launch = 10;
int nls = 11;
int LaunchButton = 22;
int Paddles = 23;
int LaunchControl = 0;
int countup = 0;
int countuprel = 1;
int countdown = 0;
int countdownrel = 1;
//test
void setup() {
  // initialize the digital pin as an output.
  Serial.begin(9600);
  pinMode(led, OUTPUT);
  pinMode(up, OUTPUT);
  pinMode(down, OUTPUT);
  pinMode(clutch, OUTPUT);
  pinMode(launch, OUTPUT);
  pinMode(nls, OUTPUT);
  analogRead(LaunchButton);  
  analogRead(Paddles); 
 
}
int LaunchValue = analogRead(LaunchButton);
int PaddleValue = analogRead(Paddles);

int Launch() {
 Serial.println("Launch");
 int LaunchValue = analogRead(LaunchButton);
  // Set sensor to read Green only
  while(LaunchValue > 115 && LaunchValue < 126){
  digitalWrite(clutch, HIGH);
  digitalWrite(launch, HIGH);
  Serial.println("inside");
  int LaunchValue = analogRead(LaunchButton);
  if (LaunchValue > 145 && LaunchValue < 155){
    Serial.println("break");
     break;
    }
  }digitalWrite(launch, LOW);
  delay(2000);
    digitalWrite(clutch, LOW);
  return 0;
 
}


void loop() {

Serial.println("Launch");

 // 152 is stock reading
 // 121 is the Launch Button
 // 112 is the Up Paddle
 // 101 is the Down Paddle
delay(50);

  digitalWrite(led, HIGH);

  int LaunchValue = analogRead(LaunchButton);
  // print out the value you read:
  Serial.println(LaunchValue);
  
  int PaddleValue = analogRead(Paddles);

  //delay(10);
  // print out the value you read:
 // Serial.println(PaddleValue);

 // Up Shift
 
  if (PaddleValue > 108 && PaddleValue < 117 && countuprel > countup) {
// 112 is the Up Paddle
    digitalWrite(nls, HIGH );
    digitalWrite(up, HIGH );
    //Serial.println("insideuploop");
    //delay(1000);
    countup++;
    
  }
  
  
  else if (PaddleValue > 145 && countup == countuprel) {
  countuprel++;
  Serial.println(countuprel);
  //Serial.println("insideupelseloop");
  delay(100);
  }
  

//Down Shift

  else if (PaddleValue > 96 && PaddleValue < 106 && countdownrel > countdown) {
    // 101 is the Down Paddle
    digitalWrite(clutch, HIGH);
    digitalWrite(launch, HIGH);
    //Serial.println("test");
    delay(100);
    digitalWrite(down, HIGH );
    delay(100);
    digitalWrite(clutch, LOW);
    delay(100);
    digitalWrite(launch,LOW);
    countdown++;
  }


   else if (PaddleValue > 145 && countdown == countdownrel) {
  countdownrel++;
  //Serial.println(countuprel);
  //Serial.println("insideupelseloop");
  delay(100);
  }

//LaunchControl

  else if (LaunchValue > 115 && LaunchValue < 126) {
   Serial.println("Launch else if");
   delay(1000);
   LaunchControl = Launch();
 }
 
  else{

    digitalWrite(down, LOW  );
    digitalWrite(up, LOW  );
    digitalWrite(nls, LOW );
    digitalWrite(clutch, LOW);
    digitalWrite(launch, LOW);
  }
  
}