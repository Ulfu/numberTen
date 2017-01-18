/*
  This example code is in the public domain.
*/

// constants won't change. They're used here to
// set pin numbers:
const int potentiometerPin = 0;      // the number of the potentiometer pin
const int steeringWheelPin = 1;

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

boolean poLeft;
boolean poRight;

void setup() {
  
  //setup left motor
  pinMode(enablePinLeft, OUTPUT);
  pinMode(rMotorOne, OUTPUT);
  pinMode(rMotorTwo, OUTPUT);
  //setup right motor
  pinMode(enablePinRight, OUTPUT);
  pinMode(lMotorOne, OUTPUT);
  pinMode(lMotorTwo, OUTPUT);
  
  Serial.begin(9600);
}

void loop() {
  //Serial.println(potentiometerRead);
  
  Direction();
  gas();
  forward(poLeft, poRight); 
  steer();
}
void Direction() {
  potentiometerRead = analogRead(potentiometerPin);
  potentiometerRead = map(potentiometerRead, 0, 1023, -255, 255);
  
  if (potentiometerRead < 0) {
    potentiometerRead = -potentiometerRead;
    poRight = false;
    }
  else {
    poRight = true;
    }
  poLeft = poRight;
  
  if (potentiometerRead > 255){
    potentiometerRead -= 255;
    poLeft = !poRight;
    }
  }
void gas() {
  //turnPercent
  int Speed = potentiometerRead;
  int rightSpeed;
  int leftSpeed;
  turnPercent *= 2;
  //abs(turnPercent) <= 100
  if (abs(turnPercent) >= 100) {
    if (turnPercent <= 0) {
      turnPercent += 100;
    }
    else {
      turnPercent -= 100;
    }
  }
  if (turnPercent <= 0) {
    leftSpeed = Speed;
    rightSpeed = Speed (1 + turnPercent / 100);
  }
  if (turnPercent > 0) {
    rightSpeed = Speed;
    leftSpeed = Speed (1 - turnPercent / 100);   
  }
  //Serial.println(leftSpeed);
  //Serial.println(rightSpeed);
  analogWrite(enablePinLeft, leftSpeed);
  analogWrite(enablePinRight, rightSpeed);
}

void steer() {
  Serial.println(turnPercent);
  
  turnPercent = analogRead(steeringWheelPin);
  turnPercent = map(turnPercent, 0, 1024, -100, 100);
  turnPercent *= 2;
}

void forward(boolean left, boolean right) {
  //Set the H-bridge in direction forward
  digitalWrite(rMotorOne, !left);
  digitalWrite(rMotorTwo, left);

  digitalWrite(lMotorOne, right);
  digitalWrite(lMotorTwo, !right);
}

