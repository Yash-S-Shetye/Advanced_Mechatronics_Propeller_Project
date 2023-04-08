// Line follower test code

#include "simpletools.h" // Include simpletools library
#include "servo.h" // Include servo library

#define LEFT_SENSOR 4 // Define pin number for left sensor
#define RIGHT_SENSOR 8 // Define pin number for right sensor

int leftServo = 2;
int rightServo = 3;


int main() {

  while (1) { // Loop infinitely
    int left_IR = input(LEFT_SENSOR); // Read value from left sensor
    int right_IR = input(RIGHT_SENSOR); // Read value from right sensor
    
    if (left_IR == 1 && right_IR == 0) { // If left sensor detects line and right sensor does not
      // Turn left
      servo_speed(leftSevo, 100); // Set left motor speed to maximum in reverse direction
      servo_speed(rightServo, -100); // Set right motor speed to maximum in forward direction
    } else if (left_IR == 0 && right_IR == 1) { // If right sensor detects line and left sensor does not
      // Turn right
      servo_speed(leftServo, -100); // Set left motor speed to maximum in forward direction
      servo_set(rightServo, 100); // Set right motor speed to maximum in reverse direction
    } else if (left_IR == 0 && right_IR == 0) { // If both sensors do not detect line
      // Move forward
      servo_speed(leftServo, 100); // Set left motor speed to maximum in forward direction
      servo_speed(rightServo, 100); // Set right motor speed to maximum in forward direction
    } else { // If both sensors detect line
      // Stop
      servo_set(leftServo, 0); // Set left motor speed to zero
      servo_set(leftServo, 0); // Set right motor speed to zero
    }
  }
}
