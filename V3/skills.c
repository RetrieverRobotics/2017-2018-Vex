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
#pragma competitionControl(Competition)\

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

//skills; red side facing mogo although it might not matter
task autonomous(){
	writeDebugStreamLine("auton - skills");

	int oldDrive;

	clearTimer(T1);
	SensorValue[driveREnc] = 0;//zero encoders
	SensorValue[driveLEnc] = 0;
	startingRotationOffset = 0; // facing positive x towards mogo

	SensorValue[claw] = CLAW_CLOSE;
	armPID.target 		= ARM_SCHMEDIUM;
	gyroPID.target 		= 0;
	swingPID.target 	= SWING_IN;
	startTask(armPIDTask);
	startTask(drivePIDTask);
	startTask(swingPIDTask);

	//TODO: bump reset on stationary goal?
	// mogo 1 - 20 pt red
	// drive to 1st mogo
	driveIncremental(48);
	wait1Msec(300);
	extendMogo();
	waitForPID(drivePID);
	// grab 1st mogo and drop it off
	intakeMogo();
	driveIncremental(-48); // drive backwards towards start
	waitForPID(drivePID);
	swingTurnRight(-135);
	waitForPID(gyroPID);
	// slam reset on the pipe
	tardDrive(127,127,1000);
	extendMogo();
	// tardDrive(-127,-127,300);
	driveIncremental(-4); // drive back enough to release mogo
	waitForPID(drivePID);
	intakeMogo();

	// mogo 2 - 10 pt red
	driveIncremental(-24); // cross pipe
	waitForPID(drivePID);
	swingTurnRight(45); // swing turn 180 degrees towards next mogo
	extendMogo();
	waitForPID(gyroPID);
	driveIncremental(24);
	waitForPID(drivePID);
	intakeMogo(); // pick up next mogo
	// turn around and drop off
	pointTurn(-135);
	waitForPID(gyroPID);
	driveIncremental(36);
	extendMogo();
	driveIncremental(-5);
	waitForPID(drivePID);
	intakeMogo();

  // mogo 3 - 20 pt blue
	pointTurn(45); // face next mogo
	extendMogo();
	waitForPID(gyroPID);
	oldDrive = drivePID.input;
	driveIncremental(100); // go all the way across the field
	// wait for mogo halfway
	while(DRIVE_TPI*(drivePID.input-oldDrive) <  80) wait1Msec(drivePID.loopTime);
	intakeMogo();
	waitForPID(drivePID);
	// recenter and face 20 pt zone
	swingTurnRight(90);
	waitForPID(gyroPID);
	driveIncremental(8);
	waitForPID(drivePID);
	swingTurnLeft(45);
	waitForPID(gyroPID);
	// slam reset on pipe and drop in 20 pt zone
	tardDrive(127,127,1000);
	extendMogo();
	driveIncremental(-4); // drive back enough to release mogo
	waitForPID(drivePID);
	intakeMogo();

	// mogo 4 - 10 pt blue
	driveIncremental(-24); // cross pipe
	waitForPID(drivePID);
	swingTurnRight(225); // swing turn 180 degrees towards next mogo
	extendMogo();
	waitForPID(gyroPID);
	driveIncremental(24);
	waitForPID(drivePID);
	intakeMogo(); // pick up next mogo
	// turn around and drop off
	pointTurn(45);
	waitForPID(gyroPID);
	driveIncremental(36);
	extendMogo();
	driveIncremental(-5);
	waitForPID(drivePID);
	intakeMogo();

	// mogo 5 - 10 point red
	pointTurn(225); // face next mogo
	extendMogo();
	waitForPID(gyroPID);
	oldDrive = drivePID.input;
	driveIncremental(140); // go all the way across the field again
	// wait for mogo halfway
	while(DRIVE_TPI*(drivePID.input-oldDrive) <  80) wait1Msec(drivePID.loopTime);
	intakeMogo();
	waitForPID(drivePID);
	extendMogo();
	driveIncremental(-5);
	waitForPID(drivePID);
	intakeMogo();

}

///////////////////////////////////////////////////////////////////////////////////
// USERCONTROL
///////////////////////////////////////////////////////////////////////////////////

task usercontrol(){
	writeDebugStreamLine("usrctrl");

	#include "usercontrol-singleController.c"
	// #include "usercontrol-2Controller.c"
}//END usercontrol()
