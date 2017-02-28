
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
  Direction(); //The thrust. This is used to decide what direction the vehicle will drive
  if (ping() < 10 && gasRead > 0) { //Get the distance measured from the ultrasound sensor and compare.
    Stop(); //If distance is less than 10cm and direction is forward then stop the vehicle. This allows the user to put in reverse
  }
  else {  //Otherwise run the regular procedure
    gas(); 
    forward(poLeft, poRight); 
    steer();
    prints();
  }
}

void Direction() { //Get the thrust. Although the direction is not set by this function
  gasRead = analogRead(gasPin); //Read the value
  gasRead = map(gasRead, 0, 1023, -255, 255); //Map to value. Max and min is chosen at 255 because of the maximum value in the function analogWrite 

  }
  
void gas() {  //
  //turnPercent
  int Speed = gasRead;  
  int rightSpeed;
  int leftSpeed;
  //abs(turnPercent) <= 100
  if (turnPercent < -50){ poLeft = 0; //If 50 percent or more left turn then shift polarity.
  }
  else poLeft = 1;
  if (turnPercent > 50) { poRight = 0; //If 50 percent or more right turn then shift polarity.
  }
  else poRight = 1;

  if (Speed < 0) { // If Speed is less than 0 Then reverse all polarities (make the vehicle reverse)
    poLeft = !poLeft;
    poRight = !poRight;
    Speed = -Speed;//Make speed a positive value
  }
  
  if (turnPercent <= 0) { //Adjusts the speed of the right motor. Set full speed to the left motor
    leftSpeed = Speed;
    rightSpeed = Speed * abs(turnPercent + 50) / 100; //By using abs() the motor will stand still when turnPercent equals -50
  }
  if (turnPercent > 0) {  //Adjusts the speed of the left motor. Set full speed to the right motor
    rightSpeed = Speed;
    leftSpeed = Speed * abs(turnPercent - 50) / 100;  
  }
  
  Serial.print(leftSpeed);  //Print values
  Serial.print("left Speed\t");
  Serial.print(rightSpeed);
  Serial.print("right Speed\t");
  analogWrite(enablePinLeft, leftSpeed);
  analogWrite(enablePinRight, rightSpeed);
}

void steer() {  //Read the value of steering potentiometer
  //Serial.println(turnPercent);
  
  turnPercent = analogRead(steeringWheelPin);
  turnPercent = map(turnPercent, 0, 1023, -100, 100); //-100 and 100 is used as a percentage when divided by 100
}

void forward(boolean left, boolean right) { 
  //Set the H-bridge in direction
  digitalWrite(rMotorOne, !left); //set the negated value for voltage difference
  digitalWrite(rMotorTwo, left);  //The direction is now set for left motor

  digitalWrite(lMotorOne, !right);
  digitalWrite(lMotorTwo, right); //The direction is now set for right motor
}

void prints(){//This function is for debugging. Prints values to serial
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

long ping() {//This function sends out a ultrasonic pulse and returns the distance to nearest object
  digitalWrite(trigPin, LOW);//Make a clean pulse
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);//Send out the pulse
  delayMicroseconds(5);
  digitalWrite(trigPin, LOW);//stop the pulse

  long microSeconds = pulseIn(echoPin, HIGH); //Get the distance measurd in time
  
  long cm = microSecondsTocm(microSeconds); //Convert the time it took into cm
  return cm;
  }
  
long microSecondsTocm(long microseconds) {
  return microseconds / 29 / 2; //1/29 is the pace and divide by two because the soundwave traveld back and forth.
}

void Stop() {//Make the vehichle stand still by locking the motors
  digitalWrite(rMotorOne, HIGH);
  digitalWrite(rMotorTwo, HIGH);

  digitalWrite(lMotorOne, HIGH);
  digitalWrite(lMotorTwo, HIGH);
}

