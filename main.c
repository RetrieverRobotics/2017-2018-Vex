#pragma config(I2C_Usage, I2C1, i2cSensors)
#pragma config(Sensor, in1,    armPot,         sensorPotentiometer)
#pragma config(Sensor, in2,    mogoPot,        sensorPotentiometer)
#pragma config(Sensor, in3,    swingPot,       sensorPotentiometer)
#pragma config(Sensor, dgtl1,  claw,           sensorDigitalOut)
#pragma config(Sensor, I2C_1,  ,               sensorQuadEncoderOnI2CPort,    , AutoAssign )
#pragma config(Sensor, I2C_2,  ,               sensorQuadEncoderOnI2CPort,    , AutoAssign )
#pragma config(Motor,  port2,           driveL,        tmotorVex393HighSpeed_MC29, openLoop, encoderPort, I2C_1)
#pragma config(Motor,  port3,           liftBl,        tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port4,           swing,         tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port5,           liftTr,        tmotorVex393_MC29, openLoop, reversed)
#pragma config(Motor,  port6,           driveR,        tmotorVex393HighSpeed_MC29, openLoop, encoderPort, I2C_2)
#pragma config(Motor,  port7,           liftTl,        tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port8,           liftBr,        tmotorVex393_MC29, openLoop, reversed)
#pragma config(Motor,  port9,           mogo,          tmotorVex393_MC29, openLoop)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

// NOTES FOR WIRING:
//
// Y's:
// port | name
// -------------
// 2		|	driveL
// 4A		|	Swing
// 6C		|	driveR
// 9		|	mogo
//
// -----------DRIVE------------
// farthest back drive motors are red to red
// the next drive motors are red to black
//
// -----------LIFT------------
// All wires red to red
//
// -----------SWING------------
// right motor is red to black
// left motor is red to red
//
// -----------MOGO------------
// right motor is red to black
// left motor is red to red


// PROGRAMMING NOTES:
// -----------SWING------------
// positive is up/intake
//
// -----------MOGO------------
// positive is up/intake

// This code is for the VEX cortex platform
#pragma platform(VEX2)
#pragma competitionControl(Competition)

typedef struct PIDStruct{
	bool debug;
	float target;
	float previousError;
	float integral;
	float output;
	int 	input;

	float Kp;
	float Ki;
	float Kd;

	float integralLimit;
	float integralActiveZone;

	int 	loopTime;

}PIDStruct;

PIDStruct driveLPID;
PIDStruct driveRPID;
PIDStruct armPID;
PIDStruct mogoPID;
PIDStruct swingPID;
#define MAX_PID_VARS 5

// array storing pointers to the PID variables for ease of access in PID functions
PIDStruct *PIDVars[MAX_PID_VARS] = {&driveLPID, &driveRPID, &armPID, &mogoPID, &swingPID};

//Main competition background code...do not modify!
#include "Vex_Competition_Includes.c"
//#include "Vex.c"
#include "functions.c"


void pre_auton()
{
	clearDebugStream();
	initPIDVars();
	#include "variablesForKent.c"

	bStopTasksBetweenModes = true;

	// Set bDisplayCompetitionStatusOnLcd to false if you don't want the LCD
	// used by the competition include file, for example, you might want
	// to display your team name on the LCD in this function.
	// bDisplayCompetitionStatusOnLcd = false;

}


task autonomous()
{

	// Remove this function call once you have "real" code.
	AutonomousCodePlaceholderForTesting();
}


task usercontrol()
{

	while (true)
	{
		//--------------------------------------------------------------------------------
		// Drive
		//--------------------------------------------------------------------------------

		// tank drive
		motor[driveL] = vexRT[Ch3];
		motor[driveR] = vexRT[Ch2];

		//--------------------------------------------------------------------------------
		// Lift
		//--------------------------------------------------------------------------------

		// up on 5U
		if(vexRT[Btn5U]){
			setLift(127);
		}
		// down on 5D
		else if(vexRT[Btn5D]){
			setLift(-127);
		}
		else{
			setLift(0);
		}

		//--------------------------------------------------------------------------------
		// Swing
		//--------------------------------------------------------------------------------

		// up on 8U
		if(vexRT[Btn8U]){
			motor[swing] = 127;
		}
		// down on 8D
		else if(vexRT[Btn8D]){
			motor[swing] = -127;
		}
		else{
			motor[swing] = 0;
		}

		//--------------------------------------------------------------------------------
		// Claw
		//--------------------------------------------------------------------------------

		// close on 6U
		if(vexRT[Btn6U]){
			SensorValue[claw] = 1;
		}
		// open on 6D
		else if(vexRT[Btn6D]){
			SensorValue[claw] = 0;
		}

		//--------------------------------------------------------------------------------
		// Mogo
		//--------------------------------------------------------------------------------

		// in on 7U
		if(vexRT[Btn7U]){
			motor[mogo] = 127;
		}
		//out on 7D
		else if(vexRT[Btn7D]){
			motor[mogo] = -127;
		}
		else{
			motor[mogo] = 0;
		}

		wait1Msec(10);
	}//END while()
}//END usercontrol()
