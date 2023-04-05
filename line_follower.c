// Line follower test code

#include "simpletools.h" // Include simpletools library
#include "servo.h" // Include servo library

#define LEFT_SENSOR 4 // Define pin number for left sensor
#define RIGHT_SENSOR 8 // Define pin number for right sensor


int main() {

  while (1) { // Loop infinitely
    int left_val = input(LEFT_SENSOR); // Read value from left sensor
    int right_val = input(RIGHT_SENSOR); // Read value from right sensor
    
    if (left_val == 1 && right_val == 0) { // If left sensor detects line and right sensor does not
      // Turn left
      servo_speed(2, 100); // Set left motor speed to maximum in reverse direction
      servo_speed(3, -100); // Set right motor speed to maximum in forward direction
    } else if (left_val == 0 && right_val == 1) { // If right sensor detects line and left sensor does not
      // Turn right
      servo_speed(2, -100); // Set left motor speed to maximum in forward direction
      servo_set(3, 100); // Set right motor speed to maximum in reverse direction
    } else if (left_val == 0 && right_val == 0) { // If both sensors do not detect line
      // Move forward
      servo_speed(2, 100); // Set left motor speed to maximum in forward direction
      servo_speed(3, 100); // Set right motor speed to maximum in forward direction
    } else { // If both sensors detect line
      // Stop
      servo_set(2, 0); // Set left motor speed to zero
      servo_set(3, 0); // Set right motor speed to zero
    }
  }
}
