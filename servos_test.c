// Test code for testing continuous rotation servos

#include "simpletools.h" // Include simpletools
#include "servo.h" // Include servo library
int main() // Main function
{
  while(1) {
    
    // Forward motion
    servo_speed(16, 50);
    servo_speed(17, -50); 
    pause(1000);
    
    // Stop 
    servo_speed(16, 0);
    servo_speed(17, 0); 
    pause(1000);
    
    // Left
    servo_speed(16, -50);
    servo_speed(17, -50); 
    pause(1000);
    
    // Stop
    servo_speed(16, 0);
    servo_speed(17, 0); 
    pause(1000);
    
    // Right
    servo_speed(16, 50);
    servo_speed(17, 50); 
    pause(1000);
    
    // Stop
    servo_speed(16, 0);
    servo_speed(17, 0); 
    pause(1000);
    
    // Backward
    servo_speed(16, -50);
    servo_speed(17, 50); 
    pause(1000);
    
    // Stop
    servo_speed(16, 0);
    servo_speed(17, 0); 
    pause(1000);
  }
}    
