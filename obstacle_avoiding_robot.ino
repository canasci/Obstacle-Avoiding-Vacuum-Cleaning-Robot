// Define pin numbers for the ultrasonic sensors
const int trigPin1 = 2;
const int echoPin1 = 3;
const int trigPin2 = 10;
const int echoPin2 = 11;
const int trigPin3 = 12;
const int echoPin3 = 13;

// Define the variables of the ultrasonic sensors
const int array_length = 3;               // Length of the array which will store the data obtained from the ultrasonic sensors
long duration[array_length] = {0, 0, 0};  // Declare the duration array, to be used in obtaining the data from the ultrasonic sensors; index 0: left, index 1: front, index 2: right sensor
long distance[array_length] = {0, 0, 0};  // Declare the distance array, to be used in obtaining the data from the ultrasonic sensors; index 0: left, index 1: front, index 2: right sensor
int trigPin[array_length] = {trigPin1, trigPin2, trigPin3}; // Declare the array that stores the trigger pin numbers
int echoPin[array_length] = {echoPin1, echoPin2, echoPin3}; // Declare the array that stores the echo pin numbers

// Define pins for MOTOR A (REAR LEFT MOTOR)
const int enableA = 9;
const int input1 = 8;
const int input2 = 7;

// Define pins for MOTOR B (REAR RIGHT MOTOR)
const int enableB = 5;
const int input3 = 6;
const int input4 = 4;

void setup() {
  // Ultrasonic sensor pins are defined and set
  pinMode(trigPin1, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin1, INPUT); // Sets the echoPin as an Input
  pinMode(trigPin2, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin2, INPUT); // Sets the echoPin as an Input
  pinMode(trigPin3, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin3, INPUT); // Sets the echoPin as an Input

  // Motor control pins are defined and set
  pinMode(enableA, OUTPUT);
  pinMode(enableB, OUTPUT);
  pinMode(input1, OUTPUT);
  pinMode(input2, OUTPUT);
  pinMode(input3, OUTPUT);
  pinMode(input4, OUTPUT);

  Serial.begin(9600); // Starts the serial communication
}

// Function definitions, to be used in loop()

void UltrasonicSensor() {    // This function sends soundwaves from the sensors to the environment, reads and stores the data in arrays
  for (int x = 0; x < array_length; x = x + 1) {
    // Clears trigPin[x]
    digitalWrite(trigPin[x], LOW);
    delayMicroseconds(2);
    // Sets trigPin[x] on HIGH state for 10 micro seconds
    digitalWrite(trigPin[x], HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin[x], LOW);
    // Reads echoPin[x], returns the sound wave travel time in microseconds
    duration[x] = pulseIn(echoPin[x], HIGH); // microseconds
    // Calculating the distance
    distance[x] = duration[x] * 0.034 / 2; // in cm
    // Prints the distance on the Serial Monitor
    Serial.print("Distance ");
    Serial.println(x);
    Serial.println(distance[x]);
    delay(150);   // wair for 150 milliseconds
  }
}
void motorsOnForward() {    // This function turns the motors on and configures them in forward direction
  // Turn motor A on and configure it in forward direction
  digitalWrite(input1, HIGH);
  digitalWrite(input2, LOW);
  // Turn motor B on and configure it in forward direction
  digitalWrite(input3, LOW);
  digitalWrite(input4, HIGH);
}
void motorsEnabledForward() {   // This function drives the motors, to be used in forward direction
  // Set speed of both motors to 180 out of possible range 0-255
  analogWrite(enableA, 180);
  analogWrite(enableB, 180);
}
void motorsOff() {    // This function turns the motors off
  // Turn off both of the motors
  digitalWrite(input1, LOW);
  digitalWrite(input2, LOW);
  digitalWrite(input3, LOW);
  digitalWrite(input4, LOW);
  delay(1500);  // wait for 1.5 seconds
}
void motorsOnReverse() {    // This function turns the motors on and configures them in reverse direction
  // Turn motor A on and configure it in reverse direction
  digitalWrite(input1, LOW);
  digitalWrite(input2, HIGH);
  // Turn motor B on and configure it in reverse direction
  digitalWrite(input3, HIGH);
  digitalWrite(input4, LOW);
}
void motorsEnabledReverse() {   // This function drives the motors, to be used in reverse direction
  // Set the reverse speed to 150 out of possible range 0-255
  analogWrite(enableA, 150);
  analogWrite(enableB, 150);
}
void motorAoff() {    // This function turns motor A off
  // Turn motor A off
  digitalWrite(input1, LOW);
  digitalWrite(input2, LOW);
  delay(1500);  // wait for 1.5 seconds
}
void motorBoff() {    // This function turns motor B off
  digitalWrite(input3, LOW);
  digitalWrite(input4, LOW);
  delay(1500);
}
void motorAOnForward() {    // This function turns motor A on and configures it in forward direction
  // Turn motor A on in forward direction
  digitalWrite(input1, HIGH);
  digitalWrite(input2, LOW);
}
void motorBOnForward() {    // This function turns motor B on and configures it in forward direction
  // Turn motor B on in forward direction
  digitalWrite(input3, LOW);
  digitalWrite(input4, HIGH);
}
void turnRight() {    // This function drives motor A only, to make the turn right
  analogWrite(enableA, 255);
}
void turnLeft() {   // This function drives motor B only, to make the turn right
  analogWrite(enableB, 255);
}

void loop() {
  delay(1000);    // wait for 1 second
  UltrasonicSensor();   // calls the function to obtain the data from the ultrasonic sensors
  if (distance[1] >= 30) {  // if the forward clearance >= 30 cm...
    motorsOnForward();
    while (distance[1] >= 30) {   // motors are driven forward and the front clearance is checked until it detects an obstacle nearby
      motorsEnabledForward();
      UltrasonicSensor();
    }
    motorsOff();
  }
  else if (distance[1] < 30) {  // if the forward clearance < 30 cm, the robot has o couple of maneuver options
    if ((distance[0] >= 30) && (distance[2] >= 30)) {   // if both the left and right hand sides of the robot are clear(distances >=30)...
      int randNum = random(0, 2);   // randomize an integer between [0,2)
      if (randNum == 0) {   // if the randomized number is 0...
        motorBOnForward();
        while (distance[1] < 30) {  // the robot turns left and checks the distance until the front is clear
          turnLeft();
          UltrasonicSensor();
        }
        if (distance[1] >= 30) {  // if the front is clear, turn motor B off
          motorBoff();
        }
      }
      else if (randNum == 1) {  // if the randomized number is 1..
        motorAOnForward();
        while (distance[1] < 30) {  // the robot turns right and checks the distance until the front is clear
          turnRight();
          UltrasonicSensor();
        }
        if (distance[1] >= 30) {  // if the front is clear, turn motor A off
          motorAoff();
        }
      }
    }
    else if ((distance[0] < 30) && (distance[2] < 30)) {  // if both the left and right hand sides of the robot are blocked(distances <30)...
      motorsOnReverse();
      while ((distance[0] < 30) && (distance[2] < 30)) {  // the motor is driven reverse while both the left and right hand sides of the robot remain blocked(distances <30)...
        motorsEnabledReverse();
        UltrasonicSensor();
      }
      if ((distance[0] >= 30) || (distance[2] >= 30)) {   // if either the left or right hand side is clear, motors are turned off...
        motorsOff;
        if ((distance[0] >= 30) && (distance[2] < 30)) {  // if the left is clear and right is blocked...
          motorBOnForward();
          while (distance[1] < 30) {  // the robot turns left and checks the distance until the front is clear
            turnLeft();
            UltrasonicSensor();
          }
          if (distance[1] >= 30) {  // if the front is clear, turn motor B off
            motorBoff();
          }
        }
        else if ((distance[0] < 30) && (distance[2] >= 30)) {  // if the left is blocked and right is clear...
          motorAOnForward();
          while (distance[1] < 30) {  // the robot turns right and checks the distance until the front is clear
            turnRight();
            UltrasonicSensor();
          }
          if (distance[1] >= 30) {  // if the front is clear, turn motor A off
            motorAoff();
          }
        }
        else if ((distance[0] >= 30) && (distance[2] >= 30)) {  // if both the left and right hand sides are clear...
          int randNum = random(0, 2);   // randomize an integer between [0,2)
          if (randNum == 0) {   // if the randomized number is 0...
            motorBOnForward();
            while (distance[1] < 30) {  // the robot turns left and checks the distance until the front is clear
              turnLeft();
              UltrasonicSensor();
            }
            if (distance[1] >= 30) {  // if the front is clear, turn motor B off
              motorBoff();
            }
          }
          else if (randNum == 1) {  // if the randomized number is 1...
            motorAOnForward();
            while (distance[1] < 30) {  // the robot turns right and checks the distance until the front is clear
              turnRight();
              UltrasonicSensor();
            }
            if (distance[1] >= 30) {  // if the front is clear, turn motor A off
              motorAoff();
            }
          }
        }
      }
    }
    else if ((distance[0] < 30) && (distance[2] >= 30)) { // if the left is blocked and right is clear...
      motorAOnForward();
      while (distance[1] < 30) {  // the robot turns right and checks the distance until the front is clear
        turnRight();
        UltrasonicSensor();
      }
      if (distance[1] >= 30) {  // if the front is clear, turn motor A off
        motorAoff();
      }
    }
    else if ((distance[0] >= 30) && (distance[2] < 30)) { // if the left is clear and right is blocked...
      motorBOnForward();
      while (distance[1] < 30) {  // the robot turns left and checks the distance until the front is clear
        turnLeft();
        UltrasonicSensor();
      }
      if (distance[1] >= 30) {  // if the front is clear, turn motor B off
        motorBoff();
      }
    }
  }
}
