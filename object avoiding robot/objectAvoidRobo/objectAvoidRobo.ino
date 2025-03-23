//please subscribe our youtube channel for more projects https://www.youtube.com/c/Technicalromboz

#include <AFMotor.h>        // Motor shield library
#include <NewPing.h>        // Ultrasonic sensor function library
#include <Servo.h>          // Servo motor library

// Motor connections
AF_DCMotor leftBackMotor(3);   // M3
AF_DCMotor leftFrontMotor(4);  // M4
AF_DCMotor rightBackMotor(2);  // M2
AF_DCMotor rightFrontMotor(1); // M1

// Sensor pins
#define trig_pin  A2 // Analog input 2
#define echo_pin  A1 // Analog input 1

#define maximum_distance  200
boolean goesForward = false;
int distance = 100;

NewPing sonar(trig_pin, echo_pin, maximum_distance); // Sensor function
Servo servo_motor; // Our servo name

void setup(){
  Serial.begin(9600); // Initialize serial communication for debugging
  // Initialize motors
  leftBackMotor.setSpeed(255);    // Set initial speed (max 255)
  leftFrontMotor.setSpeed(255);
  rightBackMotor.setSpeed(255);
  rightFrontMotor.setSpeed(255);
  
  servo_motor.attach(10); // Our servo pin (SER1 on the motor shield)

  servo_motor.write(110);
  delay(200);
  distance = readPing();
  delay(10);
  distance = readPing();
  delay(10);
  distance = readPing();
  delay(10);
  distance = readPing();
  delay(10);

  Serial.println("Setup complete");
}

void loop(){
  int distanceRight = 0;
  int distanceLeft = 0;
  delay(50);

  Serial.print("Distance: ");
  Serial.println(distance);

  if (distance <= 20){
    Serial.println("Obstacle detected, moving backward");
    moveBackward();
    delay(40);
    distanceRight = lookRight();
    delay(30);
    distanceLeft = lookLeft();
    delay(30);

    Serial.print("Distance Right: ");
    Serial.println(distanceRight);
    Serial.print("Distance Left: ");
    Serial.println(distanceLeft);

    if (distance >= distanceLeft){
      Serial.println("Turning Right");
      turnRight();
    }
    else{
      Serial.println("Turning Left");
      turnLeft();
    }
  }
  else{
    Serial.println("Moving Forward");
    moveForward(); 
  }
  distance = readPing();
}

int lookRight(){  
  servo_motor.write(50);
  delay(50);
  int distance = readPing();
  delay(10);
  servo_motor.write(115);
  return distance;
}

int lookLeft(){
  servo_motor.write(170);
  delay(50);
  int distance = readPing();
  delay(10);
  servo_motor.write(115);
  return distance;
}

int readPing(){
  delay(70);
  int cm = sonar.ping_cm();
  if (cm == 0){
    cm = 250;
  }
  return cm;
}

void moveForward(){
  if (!goesForward){
    goesForward = true;
    Serial.println("Setting motors to move forward");

    leftBackMotor.run(FORWARD);
    leftFrontMotor.run(FORWARD);
    rightBackMotor.run(FORWARD);
    rightFrontMotor.run(FORWARD);
  }
}

void moveBackward(){
  goesForward = false;
  Serial.println("Setting motors to move backward");

  leftBackMotor.run(BACKWARD);
  leftFrontMotor.run(BACKWARD);
  rightBackMotor.run(BACKWARD);
  rightFrontMotor.run(BACKWARD);
}

void turnRight(){
  Serial.println("Setting motors to turn right");

  leftBackMotor.run(FORWARD);
  leftFrontMotor.run(FORWARD);
  rightBackMotor.run(BACKWARD);
  rightFrontMotor.run(BACKWARD);
  
  delay(500);
}

void turnLeft(){
  Serial.println("Setting motors to turn left");

  leftBackMotor.run(BACKWARD);
  leftFrontMotor.run(BACKWARD);
  rightBackMotor.run(FORWARD);
  rightFrontMotor.run(FORWARD);

  delay(50);
}

