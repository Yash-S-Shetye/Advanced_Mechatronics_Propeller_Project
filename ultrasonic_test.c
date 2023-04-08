/*
  Test Ping.c

  Test the PING))) sensor before using it to navigate with the ActivityBot.

*/

#include "simpletools.h"                      // Include simpletools header
#include "ping.h"                             // Include ping header

int distance;                                 // Declare distance variable

int main()                                    // main function
{
  while(1)                                    // Repeat indefinitely
  {
    distance = ping_cm(11);                    // Get cm distance from Ping)))

    print("%c distance = %d%c cm",            // Display distance
           HOME, distance, CLREOL);           

    pause(200);                               // Wait 1/5 second
  }
}
