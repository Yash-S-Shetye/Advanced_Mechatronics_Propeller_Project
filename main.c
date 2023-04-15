// Main Code

#include "simpletools.h"                      // Include simple tools
#include "servo.h"                            // Include servo library
#include "ping.h"                             // Include ping sensor library
#include "stdbool.h"                          // Include boolean variables library

int white = 0;
int black = 1;
//const int ON  = 22;
//const int CLR = 12;
int c_intersection=0;
static volatile int distance; // For ultrasonic sensor detecting obstacles
bool slowdownflag = false; // flag to initiate slow down of bot motion
static volatile bool ledflag; // flag to turn on led blinking
//static volatile bool i_flag;
const int trigPin; // trig pin of ultrasonic sensor detecting object to be picked up
const int echoPin; // echo pin of ultrasonic sensor detecting object to be picked up
long duration; // to calculate time elapsed after sending pulse from trig pin and echo pin receiving it
int distance2; // distance from object to be picked up

// Global vars for cogs to share
unsigned int stack[40 + 25]; // Stack var for cog1 for obstacle detection ultrasonic sensor
unsigned int stack1[40 + 25]; // Stack var for cog2 for intersection detection led
//unsigned int stack2[40 + 25];

int ob_distance=20; // safe distance from obstacle or object

serial *lcd;  // creating lcd object

bool finish = false;
bool localfinish = false;
bool exit_loop=false;
const int leftWheel=16;
const int rightWheel=17;
const int ultrasonic=11;
const int TxPin = 12;
const int IR_ML=4;
const int IR_MR=8;
static volatile int led;

void obstacle(void *dist);
void led_blink(void *ledPin);
void drive(char i);
//void lcd_display(void *disp);
bool linefollowing();
void test();
void init();

// Initializing everything
void init() {
  /*lcd = serial_open(12, 12, 0, 9600);
  writeChar(lcd, ON);
  writeChar(lcd, CLR);
  pause(10);
  dprint(lcd, "Initialized");
  pause(2000);*/
  led=9;
  ledflag=false;
  //i_flag=false;
}  

//Defining drive function
void drive(char i) {
  int speed;
  if(slowdownflag==true)
    speed=30;
  else
    speed=50;
  switch(i){
  // f, b, l, r, s means forward, backward, left, right, and stop
    case 'f':servo_speed(leftWheel, speed);servo_speed(rightWheel, -speed);pause(20);break;
    case 'b':servo_speed(leftWheel, -speed);servo_speed(rightWheel, speed);pause(20);break;
    case 'l':servo_speed(leftWheel, -speed);servo_speed(rightWheel, -speed);pause(20);break;
    case 'r':servo_speed(leftWheel, speed);servo_speed(rightWheel, speed);pause(20);break;
    case 's':servo_speed(leftWheel, 0);servo_speed(rightWheel, 0);pause(20);break;
    default:print("Unclear command for motors");break;
  }
}

//run this method continuesly to follow the line until the robot meets an intersection
bool linefollowing(){
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

// function checking for obstacles
bool isobstacle() {
  if (distance<ob_distance){
    return true;}
  else{
    return false;}
}


//Defining lcd display function
/*void lcd_display(void *disp)
{
  while(1) {
    const int ON = 22;
    const int CLR = 12;
    const int CRT = 13;
    lcd = serial_open(12, 12, 0, 9600);
    writeChar(lcd, ON);
    writeChar(lcd, CLR);
    pause(10);
    dprint(lcd, "Intialized!!");
    pause(1000);
    writeChar(lcd, CLR);

    if(i_flag==true)
    {
      dprint(lcd, "Intersection");
      writeChar(lcd, CRT);
      dprint(lcd, "Detected");
      writeChar(lcd, CLR);
      i_flag=false;
    }
  }      
}*/

//for test purpose
void test(){
}

// Cog1 function for getting distance from obstacle
void obstacle(void *dist) {
  while(1) {
    distance = ping_cm(ultrasonic); // Get cm distance from Ping)))
    pause(200); // Wait 1/5 second
   }
}

// Cog2 function for blinking led
void led_blink(void *ledPin) {
  while(1) {
    if(ledflag==true) {
      high(led);
      pause(500);
      low(led);
      pause(500);
      ledflag=false;
    } 
  }
}

// Function for getting distance from object to be picked up
void ultrasonic2() {
  low(trigPin);
  pasue(2);
  high(trigPin);
  pause(10);
  low(trigPin);
  duration = pulse_in(echoPin, 1);
  distance2 = duration * 0.034/2;
}

// Function for checking for object to be picked up
bool isobject() {
  if(distance2<20)
    return true;
  else
    return false;
}              

// Main function
int main()                                    
{
  init();
  
  cogstart(&obstacle, NULL, stack, sizeof(stack)); // Starting cog for detecting obstacles
  cogstart(&led_blink, NULL, stack1, sizeof(stack1)); // Starting cog for blinking led
  while(1)
  {
    //if(!finish){  //global check flag, will set to be true when finish parking
    //1st step: follow the line and enter the intersection
    exit_loop=false;
    while(!exit_loop) {
      localfinish=false;
      while(!localfinish){
        if(!linefollowing()){     //if meet intersection
          c_intersection++;
          ledflag=true;
          //i_flag=true;
          if(isobstacle()==true) {
            drive('f');pause(500);
            drive('l');pause(1000);
            localfinish=true;
          }            
          else {
            if(c_intersection==5) {
              localfinish=true;
            }
            else {
              drive('f');pause(1000); // Move straight
            }              
        }
      }
    }
      if(c_intersection==5 && localfinish==true) {exit_loop=true;}
      else {
      // Code for bypass motion after detecting obstacle in center lane
      localfinish=false;
      int c_itsc=0;
      bool isintersection=false;
      while(!localfinish){
          //move the robot
          isintersection=!linefollowing();
        
        // Keep checking for dynamic obstacles and stop after detecting them till obstacle moves away
        while(isobstacle() == true) {
          drive('s');
        }
                  
        if(isintersection && c_itsc==0) { //if meet intersection
          ledflag=true;
          drive('f');pause(500); 
          drive('r');pause(1000); //turn 90
          slowdownflag=true;
          c_itsc++;
        }
        else if(isintersection && c_itsc==1){
          ledflag=true;
          slowdownflag=false;
          drive('f');pause(500);
          drive('r');pause(1000); // Turn right
          c_itsc++;
        }
        else if(isintersection && c_itsc==2){
          ledflag=true;
          c_intersection++;
          drive('f');pause(500);
          drive('l');pause(1000); // Turn left
          if(isobstacle()==true) {
            drive('l');pause(500);
            c_itsc=0;
          }
          else {
            localfinish=true;
          }                        
        }
      }
    }      
   }      
      
    // Check all the pick up locations
    localfinish=false;
    int c_p=0;
    bool isintersection=false;
    while(!localfinish) {
      isintersection=!linefollowing();
      
      // Keep checking for dynamic obstacles and stop after detecting them till obstacle moves away
      while(isobstacle() == true) {
          drive('s');
      }
      
      if(isintersection && c_p==0) {
        ledflag=true;
        drive('f');pause(500);
        drive('r');pause(1000);
        c_p++;
      }
      else if(isintersection && c_p==1) {
        ledflag=true;
        drive('f');pause(500);
        drive('r');pause(1000);
        c_p++;
        slowdownflag=true;
      }
      else if(isintersection && c_p>1 && c_p<5) {
        ledflag=true;
        drive('f');pause(1000);
        c_p++;        
      }
      else if(isintersection && c_p==5) {
        ledflag=true;
        slowdownflag=false;
        drive('f');pause(500);
        drive('r');pause(1000);
        localfinish=true;
      }
    }               
    //}
  }             
}



