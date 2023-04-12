// IR sensors test code

#include "simpletools.h"                      // Include simple tools

#define LEFT_SENSOR 4 // Define pin number for left sensor
#define RIGHT_SENSOR 8 // Define pin number for right sensor

int main()                                    // Main function
{
  while(1) {
    
    int left_val = input(LEFT_SENSOR); // Read value from left sensor
    int right_val = input(RIGHT_SENSOR); // Read value from right sensor
    pause(500);
    printf("Left Sensor = %d Right Sensor = %d",left_val,right_val);
  }      
}
