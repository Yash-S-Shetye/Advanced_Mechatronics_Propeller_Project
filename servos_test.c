// Test code for testing continuous rotation servos

#include "simpletools.h" // Include simpletools
#include "servo.h" // Include servo library
int main() // Main function
{
  while(1) // loop infinitely {
    
    // Forward motion
    servo_speed(2, 100);
    servo_speed(3, -100); 
    pause(2000);
    
    // Stop 
    servo_speed(2, 0);
    servo_speed(3, 0); 
    pause(2000);
    
    // Left
    servo_speed(2, 100);
    servo_speed(3, 100); 
    pause(2000);
    
    // Stop
    servo_speed(2, 0);
    servo_speed(3, 0); 
    pause(2000);
    
    // Right
    servo_speed(2, -100);
    servo_speed(3, -100); 
    pause(2000);
    
    // Stop
    servo_speed(2, 0);
    servo_speed(3, 0); 
    pause(2000);
    
    // Backward
    servo_speed(2, -100);
    servo_speed(3, 100); 
    pause(2000);
    
    // Stop
    servo_spin(2, 0);
    servo_spin(3, 0); 
    pause(2000);
  }
}      
