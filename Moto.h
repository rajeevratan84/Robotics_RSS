// - MotorControl simple -
// This simple example creates a MotorControl handle, hooks the event handlers, then opens and waits for a MotorControl Phidget to be attached.
// Once on has been attaced it will display device information and display any event data read.  The program will then do a short simulation
// of the functionality of the motor by increasing and decreasing the speed of an attached motor.  PLEASE NOTE:  This assignment
// was desgined assuming only one motor attached at motor index 0 (for motorcontrol boards with multiple motors).
//
// Copyright 2008 Phidgets Inc.  All rights reserved.
// This work is licensed under the Creative Commons Attribution 2.5 Canada License. 
// view a copy of this license, visit http://creativecommons.org/licenses/by/2.5/ca/

#include <stdio.h>
#include "wait.h"

int MotoAttachHandler(CPhidgetHandle MC, void *userptr)
{
	int serialNo;
	const char *name;

	CPhidget_getDeviceName (MC, &name);
	CPhidget_getSerialNumber(MC, &serialNo);
	printf("%s %10d attached!\n", name, serialNo);

	return 0;
}

int MotoDetachHandler(CPhidgetHandle MC, void *userptr)
{
	int serialNo;
	const char *name;

	CPhidget_getDeviceName (MC, &name);
	CPhidget_getSerialNumber(MC, &serialNo);
	printf("%s %10d detached!\n", name, serialNo);

	return 0;
}

int MotoErrorHandler(CPhidgetHandle MC, void *userptr, int ErrorCode, const char *Description)
{
	printf("Error handled. %d - %s\n", ErrorCode, Description);
	return 0;
}


int MotoInputChangeHandler(CPhidgetMotorControlHandle MC, void *usrptr, int Index, int State)
{
	printf("Input %d > State: %d\n", Index, State);
	return 0;
}

int MotoVelocityChangeHandler(CPhidgetMotorControlHandle MC, void *usrptr, int Index, double Value)
{
	printf("Motor %d > Current Speed: %f\n", Index, Value);
	return 0;
}

int MotoCurrentChangeHandler(CPhidgetMotorControlHandle MC, void *usrptr, int Index, double Value)
{
	printf("Motor: %d > Current Draw: %f\n", Index, Value);
	return 0;
}

int Motodisplay_properties(CPhidgetMotorControlHandle phid)
{
	int serialNo, version, numInputs, numMotors;
	const char* ptr;

	CPhidget_getDeviceType((CPhidgetHandle)phid, &ptr);
	CPhidget_getSerialNumber((CPhidgetHandle)phid, &serialNo);
	CPhidget_getDeviceVersion((CPhidgetHandle)phid, &version);
	
	CPhidgetMotorControl_getInputCount(phid, &numInputs);
	CPhidgetMotorControl_getMotorCount(phid, &numMotors);

	printf("%s\n", ptr);
	printf("Serial Number: %10d\nVersion: %8d\n", serialNo, version);
	printf("# Inputs: %d\n# Motors: %d\n", numInputs, numMotors);

	return 0;
}

class Moto {

 public:

  CPhidgetMotorControlHandle motoControl;
  int state;

  Moto(){
    int result;
    const char *err;

    motoControl = 0;
    state = 0;
    //create the motor control object
    CPhidgetMotorControl_create(&motoControl);

    //Set the handlers to be run when the device is plugged in or opened from software, unplugged or closed from software, or generates an error.
    CPhidget_set_OnAttach_Handler((CPhidgetHandle)motoControl, MotoAttachHandler, NULL);
    CPhidget_set_OnDetach_Handler((CPhidgetHandle)motoControl, MotoDetachHandler, NULL);
    CPhidget_set_OnError_Handler((CPhidgetHandle)motoControl, MotoErrorHandler, NULL);

    //Registers a callback that will run if an input changes.
    //Requires the handle for the Phidget, the function that will be called, and a arbitrary pointer that will be supplied to the callback function (may be NULL).
    CPhidgetMotorControl_set_OnInputChange_Handler (motoControl, MotoInputChangeHandler, NULL);

    //Registers a callback that will run if a motor changes.
    //Requires the handle for the Phidget, the function that will be called, and a arbitrary pointer that will be supplied to the callback function (may be NULL).
    CPhidgetMotorControl_set_OnVelocityChange_Handler (motoControl, MotoVelocityChangeHandler, NULL);

    //Registers a callback that will run if the current draw changes.
    //Requires the handle for the Phidget, the function that will be called, and a arbitrary pointer that will be supplied to the callback function (may be NULL).
    CPhidgetMotorControl_set_OnCurrentChange_Handler (motoControl, MotoCurrentChangeHandler, NULL);

    //open the motor control for device connections
    CPhidget_open((CPhidgetHandle)motoControl, -1);

    //get the program to wait for a motor control device to be attached
    printf("Waiting for MotorControl to be attached....");
    if((result = CPhidget_waitForAttachment((CPhidgetHandle)motoControl, 10000)))
      {
	CPhidget_getErrorDescription(result, &err);
	printf("Problem waiting for attachment: %s\n", err);
      }

  }

  void forwards()
  {
    state = 1;
    //Step 2: Set acceleration to 100, decrease target speed to 75
    CPhidgetMotorControl_setAcceleration (motoControl, 0, -100.00);
    CPhidgetMotorControl_setVelocity (motoControl, 0, -100.00);
    CPhidgetMotorControl_setAcceleration (motoControl, 1, 100.00);
    CPhidgetMotorControl_setVelocity (motoControl, 1, 70.00);
  }

  void left()
  {
    state = 2;
    //Step 2: Set acceleration to 100, decrease target speed to 75
    CPhidgetMotorControl_setAcceleration (motoControl, 0,  100.00);
    CPhidgetMotorControl_setVelocity (motoControl, 0,  100.00);
    CPhidgetMotorControl_setAcceleration (motoControl, 1, 100.00);
    CPhidgetMotorControl_setVelocity (motoControl, 1, 100.00);
  }

  void turnLeft(double val)
  {
    left();
    wait(val);
    printf("Hello");
    stop();
  }

 void turnRight(double val)
  {
    right();
    wait(val);
    printf("Hello");
    stop();
  }

 void reverse(double val) {
   reverse();
   wait(val);
   stop();
 }

 void forwards(double val) {
   forwards();
   wait(val);
   stop();
 }

  void right()
  {
    state = 3;
    //Step 2: Set acceleration to 100, decrease target speed to 75
    CPhidgetMotorControl_setAcceleration (motoControl, 0,  -100.00);
    CPhidgetMotorControl_setVelocity (motoControl, 0,  -100.00);
    CPhidgetMotorControl_setAcceleration (motoControl, 1, -100.00);
    CPhidgetMotorControl_setVelocity (motoControl, 1, -100.00);
  }

  void reverse()
  {
    state = -1;
    //Step 2: Set acceleration to 100, decrease target speed to 75
    CPhidgetMotorControl_setAcceleration (motoControl, 0,  100.00);
    CPhidgetMotorControl_setVelocity (motoControl, 0,  80.00);
    CPhidgetMotorControl_setAcceleration (motoControl, 1, -100.00);
    CPhidgetMotorControl_setVelocity (motoControl, 1, -100.00);
  }

  void stop()
  {
    state = 0;
    //Step 3: Stop the motor by decreasing speed to 0;
    CPhidgetMotorControl_setVelocity (motoControl, 0, 0.00);
    CPhidgetMotorControl_setAcceleration (motoControl, 0, 0.00);
    CPhidgetMotorControl_setVelocity (motoControl, 1, 0.00);
    CPhidgetMotorControl_setAcceleration (motoControl, 1, 0.00);
  }

  void disconnect()
  {
    //since user input has been read, this is a signal to terminate the program so we will close the phidget and delete the object we created
    CPhidget_close((CPhidgetHandle)motoControl);
    CPhidget_delete((CPhidgetHandle)motoControl);
  }

  int moveState()
  {
    return state;
  }
	
};
