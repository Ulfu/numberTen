/*
  This example code is in the public domain.
*/

// constants won't change. They're used here to
// set pin numbers:
const int potentiometerPin = 0;      // the number of the potentiometer pin
const int steeringWheelPin = 1;
const int echoPin = 11;
const int trigPin = 12;

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

boolean poFor;
boolean poLeft;
boolean poRight;

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
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
  if (ping() < 10 && potentiometerRead > 0) {
    Stop();
  }
  else {
    gas();
    forward(poLeft, poRight); 
    steer();
    prints();
  }
}

void Direction() {
  potentiometerRead = analogRead(potentiometerPin);
  potentiometerRead = map(potentiometerRead, 0, 1023, -255, 255);
  
  /*
  if (potentiometerRead < 0) {
    potentiometerRead = -potentiometerRead;
    poFor = true;
    }
  else {
    poFor = false;
    }*/
  }
  
void gas() {
  //turnPercent
  int Speed = potentiometerRead;
  int rightSpeed;
  int leftSpeed;
  //abs(turnPercent) <= 100
  if (turnPercent < -50){ poLeft = 0;
  }
  else poLeft = 1;
  if (turnPercent > 50) { poRight = 0;
  }
  else poRight = 1;

  if (Speed < 0) {
    poLeft = !poLeft;
    poRight = !poRight;
    Speed = -Speed;
  }
  
  if (turnPercent <= 0) {
    leftSpeed = Speed;
    rightSpeed = Speed * abs(turnPercent + 50) / 100;
  }
  if (turnPercent > 0) {
    rightSpeed = Speed;
    leftSpeed = Speed * abs(turnPercent - 50) / 100;  
  }
  
  Serial.print(leftSpeed);
  Serial.print("left Speed\t");
  Serial.print(rightSpeed);
  Serial.print("right Speed\t");
  analogWrite(enablePinLeft, leftSpeed);
  analogWrite(enablePinRight, rightSpeed);
}

void steer() {
  //Serial.println(turnPercent);
  
  turnPercent = analogRead(steeringWheelPin);
  turnPercent = map(turnPercent, 0, 1023, -100, 100);
}

void forward(boolean left, boolean right) {
  //Set the H-bridge in direction forward
  digitalWrite(rMotorOne, !left);
  digitalWrite(rMotorTwo, left);

  digitalWrite(lMotorOne, !right);
  digitalWrite(lMotorTwo, right);
}

void prints(){
  Serial.print(potentiometerRead);
  Serial.print("gas\t");
  Serial.print(turnPercent);
  Serial.print("ratt\t");
  //Serial.print(potentiometerRead);
  //Serial.print("vänster gas\t");
  Serial.print(poLeft);
  Serial.print("left polaritet \t");
  //Serial.print(potentiometerRead);
  //Serial.print("höger gas \t");
  Serial.print(poRight);
  Serial.println("right polaritet \t");
  
}

long ping() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(5);
  digitalWrite(trigPin, LOW);

  long microSeconds = pulseIn(echoPin, HIGH);
  
  long cm = microSecondsTocm(microSeconds);
  return cm;
  }
  
long microSecondsTocm(long microseconds) {
  return microseconds / 29 / 2;
}

void Stop() {
  digitalWrite(rMotorOne, HIGH);
  digitalWrite(rMotorTwo, HIGH);

  digitalWrite(lMotorOne, HIGH);
  digitalWrite(lMotorTwo, HIGH);
}

