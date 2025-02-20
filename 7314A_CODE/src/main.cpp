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
motor rollerbelt1(PORT16, ratio18_1, true);
motor rollerbelt2(PORT8, ratio18_1, false);
motor rollerbelt3(PORT7, ratio18_1, true);
motor Arm(PORT11, ratio6_1, true);    
digital_out Clamp1 = digital_out(Brain.ThreeWirePort.A);
digital_out Clamp2 = digital_out(Brain.ThreeWirePort.H);
inertial Gyro = inertial (PORT13); 


// Functions
void drive(int Lspeed,int Rspeed, int wt){  //drive function
  FrontLeft.spin(forward, Lspeed, pct);
  BackLeft.spin(forward, Lspeed, pct);
  FrontRight.spin(forward, Rspeed, pct);
  BackRight.spin(forward, Rspeed, pct);
  wait(wt, msec);
}

void driveBrake(){ //brake drive
  FrontLeft.stop(brake);
  BackLeft.stop(brake);
  FrontRight.stop(brake);
  BackRight.stop(brake);
}

void gyroTurn(float target)
{
		float heading = 0.0; //initialize a variable for heading
		float accuracy = 2.0; //how accurate to make the turn in degrees
		float error = target-heading;
		float kp = 5.0;
		float speed = kp * error;
		Gyro.setRotation(0.0, degrees);  //reset Gyro to zero degrees
		
		while(fabs(error) >= accuracy)
		{
			speed=kp*error;
			drive(speed * error/error, speed * error/error, 10); //turn right at speed
			heading = Gyro.rotation();  //measure the heading of the robot
			error = target - heading;  //calculate error
		}
			driveBrake();  //stop the drive
}
void gyroTurnRight(float target) {
  float heading=0.0; //initialize a variable for heading
  Gyro.setRotation(0.0, degrees);  //reset Gyro to zero degrees
  while(heading<=target)
  {
  drive(50, -50, 10); //turn right at half speed
  heading=Gyro.rotation(deg);  //measure the heading of the robot
  }
  drive(0, 0, 0);  //stop the drive
}

void gyroTurnLeft(float target) {
  float heading=0.0; //initialize a variable for heading
  Gyro.setRotation(0.0, degrees);  //reset Gyro to zero degrees
  while(heading<=target)
  {
  drive(-50, 50, 10); //turn left at half speed
  heading= -Gyro.rotation(deg);  //measure the heading of the robot
  }
  drive(0, 0, 0);  //stop the drive
}


void inchDrive(float target)
{
		float x = 0.0;
		float accuracy = 1.0; //how accurate to make the turn in degrees
		float error = target - x;
		float kp = 7.06;
		float speed = kp * error;
		FrontLeft.setPosition(0, rev);
		
		while(fabs(error) >= accuracy)
		{
			speed = kp * error;
			drive(speed, speed, 10); //turn right at speed
			x = FrontLeft.position(rev) * 3.25 * 3.14 * 0.6;
			error = target - x;  //calculate error
		}
			driveBrake();  //stop the drive
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
  // Clamp1.set(true); 
  // Clamp2.set(true);
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

void autonGetFirstMogo(void) {
  Clamp1.set(false);
  Clamp2.set(false);
  inchDrive(-29);
  wait(500, msec);
  Clamp1.set(true);
  Clamp2.set(true);
  wait(500, msec);
  rollerbelt2.spin(forward, 100, pct);
  wait(2500, msec);
  // rollerbelt2.stop(brake);
  // wait(250, msec);
}

void autonMogoToLadder(void){
  // wait(250, msec);
  gyroTurnRight(50);
  inchDrive(22);
  wait(250, msec);
  //Arm.spin(forward, 25, pct);
}

void autonLeftTurn(void){
  inchDrive(-15);
  wait(250, msec);
  gyroTurnLeft(28);
  inchDrive(15);
}
void autonRightTurn(void){
  inchDrive(-15);
  wait(250, msec);
  gyroTurnRight(28);
  inchDrive(15);
}
void autonomous(void) {
  autonGetFirstMogo();
  autonRightTurn();
  
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
  bool armflag = false; 
  bool flag = false;
  int rbspeed = 100;
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
        rollerbelt1.spin(forward, 100, pct); 
        rollerbelt2.spin(forward, 100, pct);
        rollerbelt3.spin(forward, 100, pct);
        flag = true;
      }
    
      else if (flag == true){
        rollerbelt1.stop(brake); 
        rollerbelt2.stop(brake);
        rollerbelt3.stop(brake);
        flag = false;
      }
      while (Controller.ButtonA.pressing()) {
        wait(10, msec);      
        }
      }
    else if(Controller.ButtonB.pressing()) {
      if (flag==true){
        rbspeed = -rbspeed;
        rollerbelt1.stop(brake);
        rollerbelt2.stop(brake);
        rollerbelt3.stop(brake);
        wait(20, msec);

        rollerbelt1.spin(reverse, rbspeed, pct);
        rollerbelt2.spin(reverse, rbspeed, pct);
        rollerbelt3.spin(reverse, rbspeed, pct);
       }
        while (Controller.ButtonB.pressing()) {
          wait(10, msec);      
        }

      }

      if(Controller.ButtonR1.pressing()) {
        while (Controller.ButtonR1.pressing()){
          Arm.spin(forward, 65, pct);
        }
        Arm.stop(brake);
      }
      if(Controller.ButtonR2.pressing()) {
        while (Controller.ButtonR2.pressing()){
          Arm.spin(forward, -65, pct);
        }
        Arm.stop(brake);
      }
  }
}


int main() { // set up the competition functions and callbacks.
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