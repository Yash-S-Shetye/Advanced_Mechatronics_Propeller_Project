// Main Code

#include "simpletools.h"                      // Include simple tools
#include "servo.h"                            // Include servo library
#include "ping.h"                             // Include ping sensor library
#include "stdbool.h"                          // Include library for using boolean variables

// Initializing values for black and white areas
int white = 0;
int black = 1;

// Initializing parameters for lcd display
const int BO = 17;
const int ON  = 22;
const int CLR = 12;
const int CRT = 13;

int c_intersection=0;
bool slowdownflag = false; // Flag for slowing down bot in lanes
int pickup, dropoff; // Keeping track of pickup and dropoff locations
int ob_distance=10; // Safe distance from obstacles for detection
int obj_distance=20; // Distance from detected object
int pd_distance; // Variable for storing total distance frompickup to dropoff point

static volatile int distance; // Variable for storing distance from obstacle on cog1

// Initializing pin values for second ultrasonic sensor
const int trigPin = 1;
const int echoPin = 2;

// Variables being changed on different cogs
static volatile long duration;
static volatile int distance2;
static volatile int led;
static volatile bool ledflag;

// Global vars for cogs to share
unsigned int stack[40 + 25]; // Stack vars for cog1`for obstacle detection ping sensor
unsigned int stack1[40 + 25]; // Stack vars for cog2 for led indicating intersections encountered
unsigned int stack2[40 + 25]; // Stack vars for cog3`for object detecting ultrasonic sensor


serial *lcd; // Creating object for lcd

bool finish = false;
bool localfinish = false;

// Initializing variables for servos, IR sensor and obstacle detecting led
const int leftWheel=16;
const int rightWheel=17;
const int ultrasonic=11;
const int IR_ML=4;
const int IR_MR=8;
const int led2=7;


// Declaring functions being used
void avoid_obstacle();
void obstacle(void *dist);
void object(void *dist2);
void led_blink(void *ledPin);
void drive(char i);
void lcd_display(char disp);
bool linefollowing();
void test();
void init();

// Defining initializing function
void init() {
  led=9;
  ledflag=false;
  lcd = serial_open(12, 12, 0, 9600);
  writeChar(lcd, BO);
  writeChar(lcd, ON);
  writeChar(lcd, CLR);
  pause(5);
  dprint(lcd, "Initialized!!");
  pause(1000);
  writeChar(lcd, CLR);
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

// Defining line following function
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

// Defining function for measuring distance from obstacle
void obstacle(void *dist) {
  while(1) {
    distance = ping_cm(ultrasonic); // Get cm distance from Ping)))
    pause(200); // Wait 1/5 second
   }
}

// Defining function for measuring distance from object
void object(void *dist2) {
  while(1) {
    low(trigPin);
    pause(2);
    high(trigPin);
    pause(10);
    low(trigPin);
    duration = pulse_in(echoPin, 1);
    distance2 = duration * 0.034/2;
    pause(200);
  }
}

// Defining function for blinking led when bot encounters intersection
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

// Function for checking for object to be picked up
bool isobject() {
  if(distance2<obj_distance) {
    return true;} 
  else {
    return false;}
}

// Function for checking for static as well as dynamic obstacles
bool isobstacle() {
  if (distance<ob_distance){
    return true;}
  else{
    return false;}
}


//Defining lcd display function
void lcd_display(char disp) {
    switch(disp) {
      case 'o':dprint(lcd, "Object Detected");  // i - Intersection detected
               pause(1000);
               writeChar(lcd, CLR);
               break;
      case 'e':dprint(lcd, "Object Placed");
               pause(2000);
               writeChar(lcd, CLR);
               dprint(lcd, "Distance=%d cm",pd_distance);
               pause(2000);
               writeChar(lcd, CLR);
               break;
      /*case 'd':dprint(lcd, "Distance=%d cm",pd_distance);
               pause(2000);
               writeChar(lcd, CLR);
               break;*/
      default:dprint(lcd, "Error");
              writeChar(lcd, CLR);
              break;
    }       
}

// Defining function for avoiding obstacles
void avoid_obstacle(){
  int c_itsc=0;
  bool isintersection;
  bool ob_flag=false;
  while(!ob_flag) {
    isintersection=!linefollowing();
    while(isobstacle() == true) {
      high(led2);
      drive('s');
    }
    low(led2);                
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
      if(c_intersection==5) {
        drive('f');pause(1000); // Move straight
        ob_flag=true;
        localfinish=true;
      }
      else {        
        drive('f');pause(500);
        drive('l');pause(1000); // Turn left
        if(isobstacle()==true) {
          drive('l');pause(500);
          c_itsc=0;
        }
        else {
            ob_flag=true;
          } 
        }                           
      }
    }    
}

// Main function
int main()
{
  init();
  cogstart(&obstacle, NULL, stack, sizeof(stack));      // Starting cog for detecting obstacles
  cogstart(&object, NULL, stack1, sizeof(stack1));      // Starting cog for blinking led
  cogstart(&led_blink, NULL, stack2, sizeof(stack2));   // Starting cog for detecting objects

  // 1 go through the center course
  localfinish=false;
  while(!localfinish){
    if(!linefollowing()){     //if meets intersection
      ledflag=true;
      c_intersection++;
      //lcd_display('i');
      if(c_intersection<5 && isobstacle()==true) { // If detects obstacle
        drive('f');pause(500);
        drive('l');pause(1000);
        avoid_obstacle();
      }
      else { 
        if(c_intersection==5) { // If reaches last intersection
          drive('f');pause(500);
          drive('r');pause(1000); // Turn right
          localfinish=true;
        }
        else{
          drive('f');pause(1000); // Move straight
        }
      }
    }            
  }

  // 2 go through pick up course
  localfinish=false;
  int c_p=0;
  bool isintersection=false;
  while(!localfinish) {
    isintersection=!linefollowing();
    while(c_p !=0 && isobstacle() == true) {
      high(led2);
      drive('s');
    }
    low(led2);      
    if(isintersection && c_p==0) {
      ledflag=true;
      drive('f');pause(500);
      drive('r');pause(1000); 
      if(isobject()==true){
        //drive('f');pause(500);
        drive('s');
        pause(1000);
        lcd_display('o');
        drive('f');pause(100);
        pickup=5;}
          
      c_p++;
      slowdownflag=true;
    } 
    else if(isintersection && c_p>=1 && c_p<4) {
      ledflag=true;
      if(isobject()==true){
        drive('f');pause(500);
        drive('s');
        pause(1000);
        lcd_display('o');
        drive('f');
        pause(1000);
        pickup=5-c_p;
      }        
      else{
        drive('f');pause(1000);
        }
      c_p++;                      
   }
    else if(isintersection && c_p==4) {
      ledflag=true;
      if(isobject()==true){
        drive('f');pause(500);
        drive('s');
        pause(1000);
        lcd_display('o');
        //drive('f');pause(1000);
        pickup=1;
      }
      slowdownflag=false; 
      //drive('f');pause(500);
      drive('r');pause(1000);    
      c_p++;       
    }
    else if(isintersection && c_p==5){   //cross the center course
      drive('f');pause(1000);
      localfinish=true;
    }
  }

  // 3 go through and stop at drop point
  int c_d=0;
  localfinish=false;
  while(!localfinish){
    isintersection=!linefollowing();
    while(c_d != 0 && isobstacle() == true) {
      high(led2);
      drive('s');
    }
    low(led2);      
    if(isintersection && c_d==0){
      ledflag=true;
      drive('f');pause(500);
      drive('r');pause(1000);
      slowdownflag=true;
      c_d++;
      if(isobject()==true){
        drive('s');pause(1000);
        dropoff=1;
        break;  
      }
    }
    else if(isintersection && isobject()==true){
      ledflag=true;
      // arrived drop point
      if(isobject()==true){
        drive('f');pause(500);
        drive('s');pause(1000);
        c_d++;
        dropoff=c_d;
        break;
      }
    }
    else if(isintersection){
      ledflag=true;
      drive('f');pause(1000);
      c_d++;
    }      
  }

  pd_distance=40*(pickup+dropoff);
  pause(1000);
  lcd_display('e');
  //lcd_display('d');
}
