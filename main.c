// Main Code

#include "simpletools.h"                      // Include simple tools
#include "servo.h"                            // Include servo library
#include "ping.h"  
#include "stdbool.h"                           // Include ping sensor library

int white = 0;
int black = 1;
const int ON  = 22;
const int CLR = 12;
int c_intersection=0;
int ob_distance=20;

//serial *lcd;

bool finish = false;
bool localfinish = false;
const int leftWheel=16;
const int rightWheel=17;
const int ultrasonic=11;
const int TxPin = 12;
const int IR_ML=4;
const int IR_MR=8;
const int led=7;
  
void drive(char i);
void lcd_display(char disp);
bool linefollowing();
void test();

//Defining getDistance function

//Defining drive function
void drive(char i) {
  switch(i){
  // f, b, l, r, s means forward, backward, left, right, and stop
    case 'f':servo_speed(leftWheel, 50);servo_speed(rightWheel, -50);pause(20);break;
    case 'b':servo_speed(leftWheel, -50);servo_speed(rightWheel, 50);break;
    case 'l':servo_speed(leftWheel, -50);servo_speed(rightWheel, -50);break;
    case 'r':servo_speed(leftWheel, 50);servo_speed(rightWheel, 50);break;
    case 's':servo_speed(leftWheel, 0);servo_speed(rightWheel, 0);break;
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

bool isobstacle() {
  long duration;
  int distance = ping_cm(11);
  
  
  if (distance<ob_distance){
    return true;}
  else{
    return false;}
}


//Defining lcd display function
/*void lcd_display(char disp) {
  writeChar(lcd, CLR);
  pause(5);
  
  switch(disp) {
    case 'i':dprint(lcd, "Intersection");  // i - Intersection detected
             writeChar(lcd, LINE2);
             dprint(lcd, "Detected");
             pause(1000);
             break; 
    default:print("Unclear command for display");break;
  }
}*/

//for test purpose
void test(){
  
}

int main()                                    // Main function
{ 
  while(1)
  {
    //if(!finish){  //global check flag, will set to be true when finish parking
    //1st step: follow the line and enter the intersection
      localfinish=false;
      while(!localfinish){
        if(!linefollowing()){     //if meet intersection
          //lcd_display('i');
          drive('f');pause(500); 
          drive('r');pause(1000); //turn right
          c_intersection++; //record  the intersections have been passed
          localfinish=true;
        }          
      }        
  
      localfinish=false;
      int c_itsc=0;
      bool isintersection=false;
      while(!localfinish){
          //move the robot
          isintersection=!linefollowing();
          /*bool isobj=rob.isobstacle()
          if(isintersection && c_itsc==0 && isobj==true) { //if meet intersection
          high(led);delay(1000);low(led);delay(1000);rob.drive('s');delay(2000);
        }
        else if(isintersection && c_itsc==0 && isobj==false){
            rob.drive('f');delay(500); 
            rob.drive('r');delay(2000); //turn 180
            c_itsc++;
        }
        else if(isintersection && c_itsc==1) {
          rob.drive('f');delay(100);
          //rob.drive('r');delay(1000);//Turn right
          c_itsc++;
          localfinish=true;
        }*/  
        if(isintersection && c_itsc==0) { //if meet intersection
          drive('f');pause(500); 
          drive('r');pause(2000); //turn 180
          c_itsc++;
        }
        else if(isintersection && c_itsc==2){
          drive('f');pause(500);
          drive('r');pause(1000);//turn 180
          c_itsc++;
        }
        else if(isintersection && c_itsc==1){
          drive('f');pause(1000);//pass the intersection
          c_itsc++;
        }
        else if(isintersection && c_itsc==3){
          drive('f');pause(500);
          drive('l');pause(1000);//turn left
          localfinish=true;
        }
      }
    //}
  }             
}
