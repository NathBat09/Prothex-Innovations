#include <Servo.h>

// Defining pins and thresholds
const int EMGP_PIN = A0;
const int THRESHOLD = 200;
const int LIMITER = 600;

Servo servo1;
const int SERVO_PIN = 9; //PWM pin 

// Moving Average info
// I chose a 5 period for effective smoothing yet optimal response time, periods such as 7 or 10 may be considered
int total = 0;
const int readingsAmt = 5;
int movingAvg;
int readingIndex = 0;
int readings[readingsAmt];

// Necessary Functions
void preProcess(int &emgVal);
void moveServo(Servo s, int emgVal);

void setup() {
  Serial.begin(9600);
  servo1.attach(SERVO_PIN);
  servo1.write(0); // Initial position
  delay(1000); // Wait for servo to initialize

}

void loop() {
  // Read the EMG signal
  int emgVal = analogRead(EMP_PIN);
  // Print for debugging
  Serial.print("EMG Value: ");
  Serial.println(emgVal);

  preProcess(emgVal);
  moveServo(servo1, emgVal);

  delay(50);
}

void moveServo(Servo servo, int emgVal){
  int servoAngle;

  // If the signal is within a range, move the motor a corresponding value
  if(THRESHOLD <= emgVal && emgVal <= LIMITER) {
    servoAngle = map(emgVal, THRESHOLD, LIMITER, 0, 90);
  } 
  // Cap the signal
  else if (LIMITER < emgVal){
    servoAngle = 90;
  } 
  // If the signal is too weak ignore it
  else {
    servoAngle = 0;
  }

  // Move motor accordingly
  servo.write(servoAngle);
  
  Serial.print("Servo Angle: ");
  Serial.println(servoAngle); // For debugging
}

void preProcess(int &emgVal){
  // Placeholder
  // Calculate the moving average
  total -= readings[readingIndex];
  total += emgVal;
  readings[readingIndex] = emgVal;
  readingIndex = (readingIndex + 1) % readingsAmt;
  movingAvg = total / readingsAmt;
  emgVal = movingAvg;
}
