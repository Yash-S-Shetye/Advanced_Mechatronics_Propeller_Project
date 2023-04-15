// Code to test second ultrasonic sensor

#include "simpletools.h"                      // Include simple tools

const int trigPin=9;
const int echoPin=10;
long duration;
int distance;

int main()                                    // Main function
{ 
  while(1)
  {
    // Clears the trigPin
    low(trigPin);
    pasue(2);
    // Sets the trigPin on HIGH state for 10 micro seconds
    high(trigPin);
    pause(10);
    low(trigPin);
    // Reads the echoPin, returns the sound wave travel time in microseconds
    duration = pulse_in(echoPin, 1);
    // Calculating the distance
    distance = duration * 0.034/2;
    // Prints the distance on the Serial Monitor
    print("Distance %d", distance);
    pause(1000);
  }      
}
