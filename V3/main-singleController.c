#pragma config(I2C_Usage, I2C1, i2cSensors)
#pragma config(Sensor, in1,    armPotL,        sensorPotentiometer)
#pragma config(Sensor, in2,    gyro,           sensorGyro)
#pragma config(Sensor, in4,    mogoPot,        sensorPotentiometer)
#pragma config(Sensor, in5,    swingPot,       sensorPotentiometer)
#pragma config(Sensor, in6,    armPotR,        sensorPotentiometer)
#pragma config(Sensor, dgtl1,  claw,           sensorDigitalOut)
#pragma config(Sensor, dgtl2,  ,               sensorSONAR_inch)
#pragma config(Sensor, I2C_1,  driveLEnc,      sensorQuadEncoderOnI2CPort,    , AutoAssign )
#pragma config(Sensor, I2C_2,  driveREnc,      sensorQuadEncoderOnI2CPort,    , AutoAssign )
#pragma config(Motor,  port2,           liftR,         tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port3,           driveR2,       tmotorVex393HighSpeed_MC29, openLoop)
#pragma config(Motor,  port4,           swing,         tmotorVex393HighSpeed_MC29, openLoop)
#pragma config(Motor,  port5,           driveL1,       tmotorVex393HighSpeed_MC29, openLoop, encoderPort, I2C_1)
#pragma config(Motor,  port6,           driveR1,       tmotorVex393HighSpeed_MC29, openLoop, encoderPort, I2C_2)
#pragma config(Motor,  port7,           liftL,         tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port8,           driveL2,       tmotorVex393HighSpeed_MC29, openLoop, reversed)
#pragma config(Motor,  port9,           mogo,          tmotorVex393_MC29, openLoop, reversed)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

// This code is for the VEX cortex platform
#pragma platform(VEX2)
#pragma competitionControl(Competition)

#pragma debuggerWindows("debugStream");
#pragma debuggerWindows("vexCompetitionControl");
#pragma debuggerWindows("VexLCD");

//Main competition background code...do not modify!
// #include "Vex_Competition_Includes.c"
#include "vex.c"
#include "declarations.c"
#include "variablesForKent.c"
#include "functions.c"

///////////////////////////////////////////////////////////////////////////////////
// PRE AUTON
///////////////////////////////////////////////////////////////////////////////////

void pre_auton(){

	bDisplayCompetitionStatusOnLcd = false;
	bStopTasksBetweenModes = true;
	clearDebugStream();

	#include "pre_auton.c"
}//END pre_auton

///////////////////////////////////////////////////////////////////////////////////
// AUTONOMOUS
///////////////////////////////////////////////////////////////////////////////////

task autonomous(){
	writeDebugStreamLine("auton");

	#include "auton.c"
}

///////////////////////////////////////////////////////////////////////////////////
// USERCONTROL
///////////////////////////////////////////////////////////////////////////////////

task usercontrol(){
	writeDebugStreamLine("usrctrl");

	//armPID.target = 2000;//getArmHeight();
	//startTask(usrCtrl1ArmPID);

	//driveIncremental(12);
	//drivePID.target = 300;
	//gyroPID.target = 0;
	//gyroPID.target = -900;
	//startTask(drivePIDTask);
	//startTask(autonomous);

	//armPID.target = 2000;//getArmHeight();
	//startTask(armPIDTask);
	//waitForPID(armPID);
	//while(true){ wait1Msec(1000); } // for testing code above here

	#include "usercontrol-singleController.c"
}//END usercontrol()
