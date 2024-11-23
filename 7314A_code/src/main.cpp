/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       student                                                   */
/*    Created:      9/28/2024, 10:19:31 AM                                    */
/*    Description:  V5 project                                                */
/*                                                                            */
/*----------------------------------------------------------------------------*/

#include "vex.h"

using namespace vex;

// A global instance of competition
competition Competition;
// Brain and motors
brain Brain; 
controller Controller;
motor FrontLeft(PORT1, ratio6_1, true);
motor BackLeft(PORT2, ratio6_1, true);
motor FrontRight(PORT9, ratio6_1, false);
motor BackRight(PORT10, ratio6_1, false);
motor Conveyor(PORT5, ratio18_1, true);
motor Roller(PORT8, ratio18_1, true);
motor Arm(PORT11, ratio6_1, true);
digital_out Clamp1 = digital_out(Brain.ThreeWirePort.A);
digital_out Clamp2 = digital_out(Brain.ThreeWirePort.H);
inertial Gyro = inertial (PORT13); 

/*global variables 
float pi = 3.14; 
float dia = 3.25; 
float gearRatio = 36.0/60.0; */


 /*void inchDrive(float target){ 

  float x = 0; 
  FrontLeft.setPosition(0, rev); 
  x = FrontLeft.position(rev)*dia*pi*gearRatio; 

  if (target >= 0 ){
  while (x <= target ) { 
    drive(50, 50, 10); 
    x = FrontLeft.position(rev)*dia*pi*gearRatio; 
    Brain.Screen.printAt(10, 20, "inches = %0.2f", x); 
  }
  }
  else if (target <0){ 
    while (x <=fabs(target)){
      drive(-50, -50, 10); 
      x = -FrontLeft.position(rev)*dia*pi*gearRatio;
      Brain.Screen.printAt(10, 20, "inches = %0.2f", x); 

    }
  }
  drive(0, 0, 0);

 }
*/

// Functions
void drive(int Lspeed,int Rspeed, int wt){ 
  FrontLeft.spin(forward, Lspeed, pct);
  BackLeft.spin(forward, Lspeed, pct);
  FrontRight.spin(forward, Rspeed, pct);
  BackRight.spin(forward, Rspeed, pct);
  wait(wt, msec);
}
void driveBrake() {
  FrontLeft.stop(brake);
  BackLeft.stop(brake);
  FrontRight.stop(brake);
  BackRight.stop(brake);

}

// define your global instances of motors and other devices here

/*---------------------------------------------------------------------------*/
/*                          Pre-Autonomous Functions                         */
/*                                                                           */
/*  You may want to perform some actions before the competition starts.      */
/*  Do them in the following function.  You must return from this function   */
/*  or the autonomous and usercontrol tasks will not be started.  This       */
/*  function is only called once after the V5 has been powered on and        */
/*  not every time that the robot is disabled.                               */
/*---------------------------------------------------------------------------*/

void pre_auton(void) {
  Clamp1.set(true); 
  Clamp2.set(true);
}
/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                              Autonomous Task                              */
/*                                                                           */
/*  This task is used to control your robot during the autonomous phase of   */
/*  a VEX Competition.                                                       */
/*                                                                           */
/*  You must modify the code to add your own robot specific commands here.   */
/*---------------------------------------------------------------------------*/

void autonomous(void) {
  drive(-100, -100, 650);
  driveBrake();
  Clamp1.set(false);
  Clamp2.set(false);
  Conveyor.spin(forward);
  wait(1000, msec);
  Conveyor.stop(brake);
}

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                              User Control Task                            */
/*                                                                           */
/*  This task is used to control your robot during the user control phase of */
/*  a VEX Competition.                                                       */
/*                                                                           */
/*  You must modify the code to add your own robot specific commands here.   */
/*---------------------------------------------------------------------------*/

void usercontrol(void) {
  bool flag = false;
  int rollerspeed = 60;
  int conveyorspeed = 50;
  Brain.resetTimer();
  while (1) {

    
    int Lspeed = Controller.Axis3.position();
    int Rspeed = Controller.Axis2.position();
    drive(Lspeed, Rspeed, 10);
    if(Controller.ButtonL1.pressing()) {
      Clamp1.set(true); 
      Clamp2.set(true); 
    }
    if(Controller.ButtonL2.pressing()) {
      Clamp1.set(false); 
      Clamp2.set(false); 
    }
    if(Controller.ButtonA.pressing()) {
      if(flag == false){
        Roller.spin(forward, 60, pct); 
        Conveyor.spin(forward, 50, pct);
        flag = true;
      }
    
      else if (flag == true){
        Roller.stop(brake); 
        Conveyor.stop(brake);
        flag = false;
      }
      while (Controller.ButtonA.pressing()) {
        wait(10, msec);      
        }
      }
    else if(Controller.ButtonB.pressing()) {
      if (flag==true){
        rollerspeed = -rollerspeed;
        conveyorspeed = -conveyorspeed;
        Roller.stop(brake);
        Conveyor.stop(brake);
        wait(20, msec);
        Roller.spin(forward, rollerspeed, pct); 
        Conveyor.spin(forward, conveyorspeed, pct);
       }
            while (Controller.ButtonB.pressing()) {
        wait(10, msec);      
        }

      }
    
    if(Controller.ButtonR1.pressing()) {
      Arm.spin(forward, 100, pct);
    }
    else if(Controller.ButtonR2.pressing()) {
      Arm.spin(forward, -100, pct);
    }
    else{
      Arm.stop(brake);
    }
    }
  }


//
// Main will set up the competition functions and callbacks.
//
int main() {
  // Set up callbacks for autonomous and driver control periods.
  Competition.autonomous(autonomous);
  Competition.drivercontrol(usercontrol);

  // Run the pre-autonomous function.
  pre_auton();

  // Prevent main from exiting with an infinite loop.
  while (true) {
    wait(100, msec);
  }
}