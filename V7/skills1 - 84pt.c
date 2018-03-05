#pragma config(UserModel, "ports.c")
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

// This code is for the VEX cortex platform
#pragma platform(VEX2)
#pragma competitionControl(Competition)

#pragma debuggerWindows("debugStream");
#pragma debuggerWindows("vexCompetitionControl");
#pragma debuggerWindows("VexLCD");

//Main competition background code...do not modify!
// #include "Vex_Competition_Includes.c"
#warning "skills"
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

	///// pre auton accelerated for testing
	// Calibrate gyro
	displayLCDCenteredString(0,"Calibrating...");
  SensorType[gyro] = sensorNone;
  wait1Msec(10);
  SensorType[gyro] = sensorGyro;
  wait1Msec(1300);
  //Adjust SensorScale to correct the scaling for your gyro
  SensorScale[gyro] = 138.5;
  clearLCDLine(0);
  clearLCDLine(1);

  initPIDVars();
	bLCDBacklight = false;
  /////

	displayLCDCenteredString(0,"Red Right");

//#include "pre_auton.c"
}//END pre_auton

///////////////////////////////////////////////////////////////////////////////////
// AUTONOMOUS
///////////////////////////////////////////////////////////////////////////////////

// skills - red side right
task autonomous(){
	writeDebugStreamLine("auton - skills");

	// initialization
	motor[rollers] = ROLLERS_HOLD;
  setLiftHeight(LIFT_SCHMEDIUM);
  resetDrive();
  driveIncremental(0);
  // resetGyro();
	setGyro(0);
	pointTurn(0);
  startTask(liftPIDTask);
	startTask(drivePIDTask);

	wait1Msec(200);

//////////////////mogo1
	extendMogo();
	driveIncremental(36);
	waitForPID(drivePID);
	intakeMogo();
	setLiftHeight(LIFT_FLOOR_HEIGHT);
  wait1Msec(300);
  motor[rollers] = ROLLERS_OUT;
	driveIncremental(-31);
	waitForPID(drivePID);
	setLiftHeight(LIFT_SCHMEDIUM);
	motor[rollers] = 0;
	pointTurn(90);
	waitForPID(gyroPID);

	driveIncremental(-20);
	waitForPID(drivePID);
	pointTurn(180);
	waitForPID(gyroPID);

////dropoff
	nogoGadget();
	tardDrive(127);
  wait1Msec(400);
  tardDrive(60);
  wait1Msec(500);
  tardDrive(0);
	wait1Msec(300);
	writeDebugStreamLine("1gyroOffset: %f\t gyro: %i", gyroOffset, SensorValue[gyro]);
	setGyro(180);//////////////////////////////
	writeDebugStreamLine("gyroOffset: %f\t gyro: %i", gyroOffset, SensorValue[gyro]);
  // extendMogo();

	// tardDrive(-60);
	// wait1Msec(300);
	// tardDrive(127);
	// wait1Msec(700);
	// tardDrive(0);
	// wait1Msec(300);

	///back up
  driveIncremental(-18);//-20
  waitForPID(drivePID);

	//////////////////mogo2
	pointTurn(90);
	waitForPID(gyroPID);
	driveIncremental(-22);//-21
	waitForPID(drivePID);

	pointTurn(0);
	waitForPID(gyroPID);

	tardDrive(-40);
	// wait1Msec(600);
	extendMogo();
	tardDrive(0);
	wait1Msec(200);
	writeDebugStreamLine("2gyroOffset: %f\t gyro: %i", gyroOffset, SensorValue[gyro]);
	setGyro(0);
	writeDebugStreamLine("gyroOffset: %f\t gyro: %i", gyroOffset, SensorValue[gyro]);
	wait1Msec(20);

	driveIncremental(36);
	waitForPID(drivePID);
	intakeMogo();
	driveIncremental(-31);
	waitForPID(drivePID);
	pointTurn(180);
	waitForPID(gyroPID);

	nogoGadget();

	driveIncremental(6);
	waitForPID(drivePID);

	tardDrive(26);
	extendMogo();
	tardDrive(0);

	driveIncremental(-12);
	waitForPID(drivePID);

	//////////////////////mogo 3
	pointTurn(360);
	// intakeMogo();
	waitForPID(gyroPID);

	tardDrive(-50);
	wait1Msec(1000);
	tardDrive(0);
	wait1Msec(200);
	writeDebugStreamLine("3gyroOffset: %f\t gyro: %i\t heading:%f", gyroOffset, SensorValue[gyro],getHeading());
	setGyro(360);
	writeDebugStreamLine("3gyroOffset: %f\t gyro: %i\t heading:%f", gyroOffset, SensorValue[gyro],getHeading());

	// extendMogo();
	driveIncremental(68);
	waitForPID(drivePID);
	intakeMogo();

	driveIncremental(26);
	waitForPID(drivePID);
	pointTurn(270);
	waitForPID(gyroPID);

	driveIncremental(-25);
	waitForPID(drivePID);
	pointTurn(360);
	waitForPID(gyroPID);

////dropoff
	nogoGadget();
	tardDrive(127);
  wait1Msec(600);
  tardDrive(60);
  wait1Msec(500);
  tardDrive(0);
	wait1Msec(300);
	writeDebugStreamLine("4gyroOffset: %f\t gyro: %i\t heading:%f", gyroOffset, SensorValue[gyro],getHeading());
	setGyro(0);
	writeDebugStreamLine("4gyroOffset: %f\t gyro: %i\t heading:%f", gyroOffset, SensorValue[gyro],getHeading());
  // extendMogo();

	// tardDrive(-60);
	// wait1Msec(300);
	// tardDrive(127);
	// wait1Msec(700);
	// tardDrive(0);
	// wait1Msec(300);

	///back up
  driveIncremental(-20);
  waitForPID(drivePID);

	pointTurn(0);
  waitForPID(gyroPID);

  tardDrive(25);
  intakeMogo();
  wait1Msec(200);
  tardDrive(0);

	///back up
	driveIncremental(-20);
	waitForPID(drivePID);

///////////////////////////////////mogo 4

	pointTurn(90);
	waitForPID(gyroPID);
	driveIncremental(12);
	waitForPID(drivePID);

	pointTurn(180);
	waitForPID(gyroPID);

	extendMogo();
	driveIncremental(36);
	waitForPID(drivePID);
	intakeMogo();
	driveIncremental(-31);
	waitForPID(drivePID);
	pointTurn(360);
	waitForPID(gyroPID);

	tardDrive(26);
	extendMogo();
	tardDrive(0);

	nogoGadget();

	// driveIncremental(6);
	// waitForPID(drivePID);

	driveIncremental(-8);
	waitForPID(drivePID);

//NORMAL AUTON
	////////////////////////////////mogo 5
	pointTurn(270);
	waitForPID(gyroPID);

	driveIncremental(-6);
	waitForPID(drivePID, 2000);
	swingTurnRight(225);
	waitForPID(gyroPID, 2000);

	tardDrive(-90);
  wait1Msec(600);
  tardDrive(0);

	wait1Msec(300);
  setGyro(225);

	driveIncremental(1);
  waitForPID(drivePID);
  pointTurn(135);
  // extendMogo();
	waitForPID(gyroPID);

	driveIncremental(37);//36
  waitForPID(drivePID);
  intakeMogo();

/////dropoff
	driveIncremental(-48);
	waitForPID(drivePID);

	pointTurn(90);
	waitForPID(gyroPID);
	driveIncremental(-12);
	waitForPID(drivePID);

	pointTurn(0);
	waitForPID(gyroPID);

	tardDrive(26);
  extendMogo();
  tardDrive(0);

////////////////////////////////////////mogo 6
	driveIncremental(-8);
	waitForPID(drivePID);

	pointTurn(91);
	waitForPID(gyroPID);
//
	driveIncremental(-35);
  // extendMogo();
  waitForPID(drivePID, 4000);
  swingTurnLeft(135);
  waitForPID(gyroPID, 3000);

  tardDrive(-90);
  wait1Msec(600);
  tardDrive(0);

  wait1Msec(300);
  setGyro(135);

  driveIncremental(1);
  waitForPID(drivePID);
  pointTurn(225);
  // extendMogo();
  waitForPID(gyroPID);

  // extendMogo();
  driveIncremental(37);//36
  waitForPID(drivePID);
  intakeMogo();

////dropoff
  driveIncremental(-48);
  waitForPID(drivePID);
  swingTurnLeft(360);
  waitForPID(gyroPID);

  // setLiftHeight(LIFT_SCHMEDIUM);
  // motor[rollers] = ROLLERS_OUT;
  tardDrive(26);
  extendMogo();
  tardDrive(0);

	driveIncremental(-12);

	while(1){wait1Msec(1000);}

}

///////////////////////////////////////////////////////////////////////////////////
// USERCONTROL
///////////////////////////////////////////////////////////////////////////////////

task usercontrol(){
	writeDebugStreamLine("usrctrl");

	// check if second controller is connected
	if (nVexRCReceiveState & vrXmit2) {
	  #include "usercontrol-2Controller.c"
	}
	else {
	  #include "usercontrol-singleController.c"
	}

}//END usercontrol()