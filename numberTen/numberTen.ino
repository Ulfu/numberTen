
// seting up pin numbers:
const int gasPin = 0;      // this is the pin for the gas
const int steeringWheelPin = 1; // this is the pin for the steering wheel
const int echoPin = 11; // this is the pin connected to the echo pin on the ultrasound distance sensor. This pin sends a high singal. The signal length depends on the distance to the reflecting object. 
const int trigPin = 12; // this is the pin connected to the trigger pin on the ultrasound distance sensor. Seting this pin to high will send a ultrasound pulse

//Define steering pins for the left motor
const int enablePinLeft = 3; //use this pin to control the motor speed
const int lMotorOne = 4;  //use these to control rotation direction
const int lMotorTwo = 5;

//Define steering pins for the right motor
const int enablePinRight = 6; //use this pin to control the motor speed
const int rMotorOne = 7;//use these to control rotation direction
const int rMotorTwo = 8;

int gasRead;
int turnPercent = 0;

boolean poLeft;
boolean poRight;

void setup() {
  //setup correct pin mode to the corresponding pin
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
  
  Serial.begin(9600); //Start serial communication at 9600 baud rate (9600 bits/s).
}

void loop() {
  //Serial.println(gasRead);
  //Direction();
  int dist = ping(); //Save the distance measured from the ultrasound sensor
  if (dist < 10) gasRead = -255; //If distance is less than 
  else if (dist > 15) gasRead = 255;
  else gasRead = 0;
    gas();
    forward(poLeft, poRight); 
    steer();
    prints();
}

void Direction() {
  gasRead = analogRead(gasPin);
  gasRead = map(gasRead, 0, 1023, -255, 255);
  }
  
void gas() {
  //turnPercent
  int Speed = gasRead;
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
  Serial.print(gasRead);
  Serial.print("gas\t");
  Serial.print(turnPercent);
  Serial.print("ratt\t");
  //Serial.print(gasRead);
  //Serial.print("vänster gas\t");
  Serial.print(poLeft);
  Serial.print("left polaritet \t");
  //Serial.print(gasRead);
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

