/*
  This example code is in the public domain.
*/

// constants won't change. They're used here to
// set pin numbers:
const int potentiometerPin = 0;      // the number of the potentiometer pin
const int leftButton = 11;
const int rightButton = 10;

//Define steering pins for left motor
const int enablePinLeft = 3;
const int lMotorOne = 4;
const int lMotorTwo = 5;

//Define steering pins for right motor
const int enablePinRight = 6;
const int rMotorOne = 7;
const int rMotorTwo = 8;

int potentiometerRead;
int turnPercent = 0;
boolean leftButtonVal = 1;
boolean rightButtonVal = 1;

int lastPressed = 0;

void setup() {
  // initialize the button pins as an input pullup:
  pinMode(potentiometerPin, INPUT);
  
  //setup left motor
  pinMode(enablePinLeft, OUTPUT);
  pinMode(rMotorOne, OUTPUT);
  pinMode(rMotorTwo, OUTPUT);
  //setup right motor
  pinMode(enablePinRight, OUTPUT);
  pinMode(lMotorOne, OUTPUT);
  pinMode(lMotorTwo, OUTPUT);
  
  pinMode(leftButton, INPUT_PULLUP);
  pinMode(rightButton, INPUT_PULLUP);
  Serial.begin(9600);
}

void loop() {
  potentiometerRead = analogRead(potentiometerPin);
  potentiometerRead = map(potentiometerRead, 0, 1023, 0, 255);
  //Serial.println(potentiometerRead);

  forward(true); 
  gas();
  steer();
}

void gas() {
  //turnPercent
  int Speed = potentiometerRead;
  int rightSpeed;
  int leftSpeed;
  if (turnPercent <= 0) {
    leftSpeed = Speed;
    rightSpeed = Speed + (Speed * turnPercent / 100);
  }
  if (turnPercent > 0) {
    rightSpeed = Speed;
    leftSpeed = Speed - (Speed * turnPercent / 100);
  }
  //Serial.println(leftSpeed);
  //Serial.println(rightSpeed);
  analogWrite(enablePinLeft, leftSpeed);
  analogWrite(enablePinRight, rightSpeed);
}

void steer() {
  Serial.println(turnPercent);
  leftButtonVal = digitalRead(leftButton);
  rightButtonVal = digitalRead(rightButton);
  if (millis() - lastPressed > 300 ) {
    if (!leftButtonVal) {
      turnPercent += 10;
      lastPressed = millis();
    }
    if (!rightButtonVal) {
      turnPercent -= 10;
      lastPressed = millis();
    }
  }
  turnPercent = constrain(turnPercent, -100, 100);
}

void forward(boolean yes) {
  //Set the H-bridge in direction forward
  digitalWrite(rMotorOne, !yes);
  digitalWrite(rMotorTwo, yes);

  digitalWrite(lMotorOne, yes);
  digitalWrite(lMotorTwo, !yes);
}

void backwards() {
  
}

void rotate(boolean right) {
  digitalWrite(rMotorOne, right);
  digitalWrite(rMotorTwo, !right);

  digitalWrite(lMotorOne, right);
  digitalWrite(lMotorTwo, !right);
}

