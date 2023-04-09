// Main Code

#include "simpletools.h"                      // Include simple tools
#include "servo.h"                            // Include servo library
#include "ping.h"                             // Include ping sensor library

int white = 0;
int black = 1;

serial *lcd;

const int ON  = 22;
const int CLR = 12;

//Creating a robot class containing all the functions that will be used
class Robot {
  private:
  //Defining required variables
  const int leftwheel=2;
  const int rightwheel=3;
  const int ultrasonic=11;
  const int TxPin = 12;
  const int IR_ML=4;
  const int IR_MR=8;
  const int led=7;
  
  
  public:
  //Declaring all the required functions
  void INIT();
  bool isobstacle();
  void drive(char i);
  void lcd_display(char disp);
  bool linefollowing();
  void test();
};

//Initialization
void Robot::INIT(){
  lcd = serial_open(12, 12, 0, 9600);
  writeChar(lcd, ON);
  writeChar(lcd, CLR);
  pause(5);
  dprint(lcd, "Initialized");
  pause(1000);
}

//Defining getDistance function

//Defining drive function
void Robot::drive(char i) {
  switch(i){
  // f, b, l, r, s means forward, backward, left, right, and stop
    case 'f':servo_speed(leftWheel, 100);servo_speed(rightWheel, -100);delay(20);break;
    case 'b':servo_speed(leftWheel, -100);servo_speed(rightWheel, 100);break;
    case 'l':servo_speed(leftWheel, 100);servo_speed(rightWheel, 100);break;
    case 'r':servo_speed(leftWheel, -100);servo_speed(rightWheel, -100);break;
    case 's':servo_speed(leftWheel, 0);servo_speed(rightWheel, 0);break;
    default:Serial.println("Unclear command for motors");break;
  }
}

//run this method continuesly to follow the line until the robot meets an intersection
bool Robot::linefollowing(){
  int SL = input(IR_ML);
  int SR = input(IR_MR);
  if (SL == white && SR == white) {
    //forward
    drive('f');return true;
  }
  else if (SL == black && SR== white) {
    //turn left
    drive('l');return true;
  }
  else if (SL == white &&  SR == black) {
    // Turn right
    drive('r');return true;
  }
  else {
    // Stop
    drive('s');return false;
  }
}

//Defining lcd display function
void Robot::lcd_display(char disp) {
  writeChar(lcd, CLR);
  pause(5);
  
  switch(disp) {
    case 'i':dprint(lcd, "Intersection");  // i - Intersection detected
             writeChar(lcd, LINE2);
             dprint(lcd, "Detected");
             pause(1000);
             break; 
    default:Serial.println("Unclear command for display");break;
  }
}

//for test purpose
void Robot::test(){
  
}

int main()                                    // Main function
{
  //create robot object
  Robot rob;
 
  while(1)
  {
    
    
  }  
}
