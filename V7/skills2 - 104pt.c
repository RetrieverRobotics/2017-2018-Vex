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
	writeDebugStreamLine("here1");
  SensorType[gyro] = sensorNone;
  wait1Msec(10);
  SensorType[gyro] = sensorGyro;
  wait1Msec(1300);
  //Adjust SensorScale to correct the scaling for your gyro
  SensorScale[gyro] = 138.5;
  clearLCDLine(0);
  clearLCDLine(1);

  writeDebugStreamLine("here2");

  initPIDVars();
	bLCDBacklight = false;
  /////

	writeDebugStreamLine("here3");

	displayLCDCenteredString(0,"Red Left");

//#include "pre_auton.c"
}//END pre_auton

///////////////////////////////////////////////////////////////////////////////////
// AUTONOMOUS
///////////////////////////////////////////////////////////////////////////////////

// skills - blue side facing mogo
task autonomous(){
	writeDebugStreamLine("auton - skills");

	// initialization
	motor[rollers] = ROLLERS_HOLD;
  setLiftHeight(LIFT_SCHMEDIUM);
	nMotorEncoder[driveLBB] = 0;
	nMotorEncoder[driveRBB] = 0;
  resetDrive();
  // resetGyro();
	setGyro(0);
	pointTurn(0);
	driveIncremental(0);
	startTask(drivePIDTask);
  startTask(liftPIDTask);

	// wait1Msec(200);

//////////////////mogo1
	motor[mogo] = -127;
	wait1Msec(400); // 400
	driveIncremental(48);
	extendMogo();
	waitForPID(drivePID);

	intakeMogo();

	////dropoff
	driveIncremental(-48);

	setLiftHeight(LIFT_FLOOR_HEIGHT);
  wait1Msec(300);
  motor[rollers] = ROLLERS_OUT;
	wait1Msec(500);
	setLiftHeight(LIFT_SCHMEDIUM);
	motor[rollers] = 0;

	waitForPID(drivePID, 4000);

	pointTurnInc(45);
	waitForPID(gyroPID);
	driveIncremental(-31);
	waitForPID(drivePID);
	pointTurnInc(90);
	waitForPID(gyroPID);

	nogoGadget();
	tardDrive(127);
  wait1Msec(400);
  tardDrive(60);
  wait1Msec(500);
  tardDrive(0);
	wait1Msec(300);
	// setGyro(135);

	///back up
  driveIncremental(-18);//18);
  waitForPID(drivePID,3000);

////////////////////////mogo 2
	motor[mogo] = -127;
	pointTurnInc(-90);
	waitForPID(gyroPID);
	driveIncremental(8);//12
	// extendMogo();
	waitForPID(drivePID);
	motor[mogo] = 0;
	pointTurnInc(-90);
	waitForPID(gyroPID);

	driveIncremental(34);
	waitForPID(drivePID, 4000);
	intakeMogo();
	driveIncremental(-36);//31);
	waitForPID(drivePID, 3000);

	tardDrive(-50);
	wait1Msec(500);
	tardDrive(0);
	resetGyro();

	driveIncremental(3);
	waitForPID(drivePID);

	// pointTurnInc(-180);
	pointTurnInc(-354);
	wait1Msec(400);
	nogoGadget();
	wait1Msec(200);
	motor[mogo] = -127;
	// wait1Msec(400);
	waitForPID(gyroPID);
	wait1Msec(5);
	motor[mogo] = 0;

	////dropoff

	// tardDrive(127);
	// wait1Msec(300);
	// tardDrive(-127);
	// wait1Msec(300);
	// tardDrive(0);

	// driveIncremental(6);
	// waitForPID(drivePID);

	// tardDrive(26);
	// extendMogo();
	// tardDrive(0);

	// driveIncremental(-12);
	// waitForPID(drivePID);

///////////////////////mogo 3
	// pointTurnInc(-177);
	// waitForPID(gyroPID);

	// extendMogo();///this?
	driveIncremental(58);
	waitForPID(drivePID);
	driveIncremental(47);
	motor[mogo] = 127;
	// motor[mogo] = -127;
	wait1Msec(500);
	motor[mogo] = 0;
	waitForPID(drivePID);
	motor[mogo] = -127;
	// motor[mogo] = 0;

	////dropoff
	// tardDrive(26);
	// extendMogo();
	// tardDrive(0);

	driveIncremental(-14);//-18
	wait1Msec(300);
	motor[mogo] = 0;
	waitForPID(drivePID);

////////////////////////////////mogo 4
	pointTurnInc(45);
	intakeMogo();
	waitForPID(gyroPID);
	driveIncremental(12);
	waitForPID(drivePID, 2000);
	pointTurnInc(90);
	waitForPID(gyroPID);
	motor[mogo] = -127;
	wait1Msec(200);
	driveIncremental(41.984652397463);
	extendMogo();
	waitForPID(drivePID);
	intakeMogo();

	////dropoff
	driveIncremental(-52.79456132);
	waitForPID(drivePID);

	pointTurnInc(-39);
	waitForPID(gyroPID);
	driveIncremental(-32);
	waitForPID(drivePID);
	pointTurnInc(-90);
	waitForPID(gyroPID);

	nogoGadget();
	tardDrive(127);
  wait1Msec(400);
  tardDrive(60);
  wait1Msec(500);
  tardDrive(0);
	wait1Msec(300);
	// setGyro(45);

	///back up
  driveIncremental(-20);
  waitForPID(drivePID, 3000);

/////////////////////mogo 5
	pointTurnInc(-100);
	waitForPID(gyroPID);
	driveIncremental(32);
	waitForPID(drivePID, 4000);
	pointTurnInc(-34);
	waitForPID(gyroPID);
	motor[mogo] = -127;
	wait1Msec(450);
	driveIncremental(32);
	extendMogo();
	waitForPID(drivePID);
	intakeMogo();

	/////dropoff
	driveIncremental(-50);
	waitForPID(drivePID, 4000);

	pointTurnInc(44);
	waitForPID(gyroPID);
	driveIncremental(-27.78946513256799846512);//-27.789
	waitForPID(drivePID);

	nogoGadget();
	pointTurnInc(360);
	motor[mogo] = -127;
	wait1Msec(400);
	motor[mogo] = 127;
	wait1Msec(400);
	motor[mogo] = 0;
	waitForPID(gyroPID);

	// tardDrive(26);
  // extendMogo();
  // tardDrive(0);
	//
	// driveIncremental(-8);
	// waitForPID(drivePID);

/////////////////////mogo 6
	//mogo2 code:
		// pointTurnInc(-90);
		// waitForPID(gyroPID);
		driveIncremental(8.325879422699955);//8.3257
		waitForPID(drivePID);
		pointTurnInc(-90);
		extendMogo();
		waitForPID(gyroPID);

		driveIncremental(30);
		waitForPID(drivePID, 4000);
		intakeMogo();
		driveIncremental(-36);//31);
		waitForPID(drivePID, 3000);

		tardDrive(-50);
		wait1Msec(500);
		tardDrive(0);
		resetGyro();

		driveIncremental(3);
		waitForPID(drivePID);

		// pointTurnInc(-180);
		pointTurnInc(-355.9764531256);
		wait1Msec(400);
		nogoGadget();
		wait1Msec(200);
		motor[mogo] = -127;
		//wait1Msec(400);
		waitForPID(gyroPID);
		wait1Msec(5);
		motor[mogo] = 0;
	//////end mogo 2 code


	// swingTurnLeftInc(-180);
	// motor[mogo] = 127;
	// wait1Msec(400);
	// motor[mogo] = -127;
	// wait1Msec(400);
	// waitForPID(gyroPID);
	//
	// driveIncremental(24);
	// waitForPID(drivePID, 4000);
	// intakeMogo();
	// driveIncremental(-31);
	// waitForPID(drivePID);
	//
	// pointTurnInc(180);
	// waitForPID(gyroPID);
	//
	// ////dropoff
	// nogoGadget();
	//
	// driveIncremental(6);
	// waitForPID(drivePID);
	//
	// tardDrive(26);
	// extendMogo();
	// tardDrive(0);
	//
	// driveIncremental(-12);
	// waitForPID(drivePID);

///////////////////////////mogo 7
	// pointTurnInc(-179);
	// waitForPID(gyroPID);

	driveIncremental(58);
	waitForPID(drivePID);
	driveIncremental(48);
	motor[mogo] = 127;
	// motor[mogo] = -127;
	wait1Msec(500);
	motor[mogo] = 0;
	waitForPID(drivePID);
	motor[mogo] = -127;
	//motor[mogo] = -127;
	// motor[mogo] = -127;
	//wait1Msec(580);
	//motor[mogo] = 0;
	//motor[mogo] = 127;
	//wait1Msec(500);
	waitForPID(drivePID);
	// motor[mogo] = 0;

	////dropoff
	// tardDrive(26);
	// extendMogo();
	// tardDrive(0);

	driveIncremental(-12);
	wait1Msec(300);
	motor[mogo] = 0;
	waitForPID(drivePID);

/////////////////////mogo 8
	pointTurnInc(120);
	motor[mogo] = -127;
	waitForPID(gyroPID);
	motor[mogo] = 0;
	driveIncremental(38);
	waitForPID(drivePID, 4000);
	pointTurnInc(5);
	intakeMogo();
	waitForPID(gyroPID);
	// extendMogo();
	// driveIncremental(32);
	// waitForPID(drivePID);
	// intakeMogo();

	/////dropoff
	driveIncremental(-70);
	waitForPID(drivePID, 4000);

	// pointTurnInc(-44);
	// waitForPID(gyroPID);
	// driveIncremental(-27.78946513256799846512);
	// waitForPID(drivePID);

	pointTurnInc(-180);
	wait1Msec(400);
	motor[mogo] = -127;
	nogoGadget();
	wait1Msec(400);
	motor[mogo] = 127;
	wait1Msec(400);
	motor[mogo] = 0;
	waitForPID(gyroPID, 5000);

////////////////////////park
	// pointTurnInc(130);
	// waitForPID(gyroPID, 5000);
	driveIncremental(-78);


	while(1){wait1Msec(1000);}
}

///////////////////////////////////////////////////////////////////////////////////
// USERCONTROL
///////////////////////////////////////////////////////////////////////////////////

task usercontrol(){
	writeDebugStreamLine("usrctrl");

	// writeDebugStreamLine("auton - skills");

	// initialization
	motor[rollers] = ROLLERS_HOLD;
  setLiftHeight(LIFT_SCHMEDIUM);
	nMotorEncoder[driveLBB] = 0;
	nMotorEncoder[driveRBB] = 0;
  resetDrive();
  // resetGyro();
	setGyro(0);
	pointTurn(0);
	driveIncremental(0);
	startTask(drivePIDTask);
  startTask(liftPIDTask);

	// wait1Msec(200);

//////////////////mogo1
	motor[mogo] = -127;
	wait1Msec(400); // 400
	driveIncremental(48);
	extendMogo();
	waitForPID(drivePID);

	intakeMogo();

	////dropoff
	driveIncremental(-48);

	setLiftHeight(LIFT_FLOOR_HEIGHT);
  wait1Msec(300);
  motor[rollers] = ROLLERS_OUT;
	wait1Msec(500);
	setLiftHeight(LIFT_SCHMEDIUM);
	motor[rollers] = 0;

	waitForPID(drivePID, 4000);

	pointTurnInc(45);
	waitForPID(gyroPID);
	driveIncremental(-31);
	waitForPID(drivePID);
	pointTurnInc(90);
	waitForPID(gyroPID);

	nogoGadget();
	tardDrive(127);
  wait1Msec(400);
  tardDrive(60);
  wait1Msec(500);
  tardDrive(0);
	wait1Msec(300);
	// setGyro(135);

	///back up
  driveIncremental(-18);//18);
  waitForPID(drivePID,3000);

////////////////////////mogo 2
	motor[mogo] = -127;
	pointTurnInc(-90);
	waitForPID(gyroPID);
	driveIncremental(8);//12
	// extendMogo();
	waitForPID(drivePID);
	motor[mogo] = 0;
	pointTurnInc(-90);
	waitForPID(gyroPID);

	driveIncremental(34);
	waitForPID(drivePID, 4000);
	intakeMogo();
	driveIncremental(-36);//31);
	waitForPID(drivePID, 3000);

	tardDrive(-50);
	wait1Msec(500);
	tardDrive(0);
	resetGyro();

	driveIncremental(3);
	waitForPID(drivePID);

	// pointTurnInc(-180);
	pointTurnInc(-354);
	wait1Msec(400);
	nogoGadget();
	wait1Msec(200);
	motor[mogo] = -127;
	// wait1Msec(400);
	waitForPID(gyroPID);
	wait1Msec(5);
	motor[mogo] = 0;

	////dropoff

	// tardDrive(127);
	// wait1Msec(300);
	// tardDrive(-127);
	// wait1Msec(300);
	// tardDrive(0);

	// driveIncremental(6);
	// waitForPID(drivePID);

	// tardDrive(26);
	// extendMogo();
	// tardDrive(0);

	// driveIncremental(-12);
	// waitForPID(drivePID);

///////////////////////mogo 3
	// pointTurnInc(-177);
	// waitForPID(gyroPID);

	// extendMogo();///this?
	driveIncremental(58);
	waitForPID(drivePID);
	driveIncremental(47);
	motor[mogo] = 127;
	// motor[mogo] = -127;
	wait1Msec(500);
	motor[mogo] = 0;
	waitForPID(drivePID);
	motor[mogo] = -127;
	// motor[mogo] = 0;

	////dropoff
	// tardDrive(26);
	// extendMogo();
	// tardDrive(0);

	driveIncremental(-14);//-18
	wait1Msec(300);
	motor[mogo] = 0;
	waitForPID(drivePID);

////////////////////////////////mogo 4
	pointTurnInc(45);
	intakeMogo();
	waitForPID(gyroPID);
	driveIncremental(12);
	waitForPID(drivePID, 2000);
	pointTurnInc(90);
	waitForPID(gyroPID);
	motor[mogo] = -127;
	wait1Msec(200);
	driveIncremental(41.984652397463);
	extendMogo();
	waitForPID(drivePID);
	intakeMogo();

	////dropoff
	driveIncremental(-52.79456132);
	waitForPID(drivePID);

	pointTurnInc(-39);
	waitForPID(gyroPID);
	driveIncremental(-32);
	waitForPID(drivePID);
	pointTurnInc(-90);
	waitForPID(gyroPID);

	nogoGadget();
	tardDrive(127);
  wait1Msec(400);
  tardDrive(60);
  wait1Msec(500);
  tardDrive(0);
	wait1Msec(300);
	// setGyro(45);

	///back up
  driveIncremental(-20);
  waitForPID(drivePID, 3000);

/////////////////////mogo 5
	pointTurnInc(-100);
	waitForPID(gyroPID);
	driveIncremental(32);
	waitForPID(drivePID, 4000);
	pointTurnInc(-34);
	waitForPID(gyroPID);
	motor[mogo] = -127;
	wait1Msec(450);
	driveIncremental(32);
	extendMogo();
	waitForPID(drivePID);
	intakeMogo();

	/////dropoff
	driveIncremental(-50);
	waitForPID(drivePID, 4000);

	pointTurnInc(44);
	waitForPID(gyroPID);
	driveIncremental(-27.78946513256799846512);//-27.789
	waitForPID(drivePID);

	nogoGadget();
	pointTurnInc(360);
	motor[mogo] = -127;
	wait1Msec(400);
	motor[mogo] = 127;
	wait1Msec(400);
	motor[mogo] = 0;
	waitForPID(gyroPID);

	// tardDrive(26);
  // extendMogo();
  // tardDrive(0);
	//
	// driveIncremental(-8);
	// waitForPID(drivePID);

/////////////////////mogo 6
	//mogo2 code:
		// pointTurnInc(-90);
		// waitForPID(gyroPID);
		driveIncremental(8.325879422699955);//8.3257
		waitForPID(drivePID);
		pointTurnInc(-90);
		extendMogo();
		waitForPID(gyroPID);

		driveIncremental(30);
		waitForPID(drivePID, 4000);
		intakeMogo();
		driveIncremental(-36);//31);
		waitForPID(drivePID, 3000);

		tardDrive(-50);
		wait1Msec(500);
		tardDrive(0);
		resetGyro();

		driveIncremental(3);
		waitForPID(drivePID);

		// pointTurnInc(-180);
		pointTurnInc(-355.9764531256);
		wait1Msec(400);
		nogoGadget();
		wait1Msec(200);
		motor[mogo] = -127;
		//wait1Msec(400);
		waitForPID(gyroPID);
		wait1Msec(5);
		motor[mogo] = 0;
	//////end mogo 2 code


	// swingTurnLeftInc(-180);
	// motor[mogo] = 127;
	// wait1Msec(400);
	// motor[mogo] = -127;
	// wait1Msec(400);
	// waitForPID(gyroPID);
	//
	// driveIncremental(24);
	// waitForPID(drivePID, 4000);
	// intakeMogo();
	// driveIncremental(-31);
	// waitForPID(drivePID);
	//
	// pointTurnInc(180);
	// waitForPID(gyroPID);
	//
	// ////dropoff
	// nogoGadget();
	//
	// driveIncremental(6);
	// waitForPID(drivePID);
	//
	// tardDrive(26);
	// extendMogo();
	// tardDrive(0);
	//
	// driveIncremental(-12);
	// waitForPID(drivePID);

///////////////////////////mogo 7
	// pointTurnInc(-179);
	// waitForPID(gyroPID);

	driveIncremental(58);
	waitForPID(drivePID);
	driveIncremental(48);
	motor[mogo] = 127;
	// motor[mogo] = -127;
	wait1Msec(500);
	motor[mogo] = 0;
	waitForPID(drivePID);
	motor[mogo] = -127;
	//motor[mogo] = -127;
	// motor[mogo] = -127;
	//wait1Msec(580);
	//motor[mogo] = 0;
	//motor[mogo] = 127;
	//wait1Msec(500);
	waitForPID(drivePID);
	// motor[mogo] = 0;

	////dropoff
	// tardDrive(26);
	// extendMogo();
	// tardDrive(0);

	driveIncremental(-12);
	wait1Msec(300);
	motor[mogo] = 0;
	waitForPID(drivePID);

/////////////////////mogo 8
	pointTurnInc(120);
	motor[mogo] = -127;
	waitForPID(gyroPID);
	motor[mogo] = 0;
	driveIncremental(38);
	waitForPID(drivePID, 4000);
	pointTurnInc(5);
	intakeMogo();
	waitForPID(gyroPID);
	// extendMogo();
	// driveIncremental(32);
	// waitForPID(drivePID);
	// intakeMogo();

	/////dropoff
	driveIncremental(-70);
	waitForPID(drivePID, 4000);

	// pointTurnInc(-44);
	// waitForPID(gyroPID);
	// driveIncremental(-27.78946513256799846512);
	// waitForPID(drivePID);

	pointTurnInc(-180);
	wait1Msec(400);
	motor[mogo] = -127;
	nogoGadget();
	wait1Msec(400);
	motor[mogo] = 127;
	wait1Msec(400);
	motor[mogo] = 0;
	waitForPID(gyroPID, 5000);

////////////////////////park
	// pointTurnInc(130);
	// waitForPID(gyroPID, 5000);
	driveIncremental(-78);


	while(1){wait1Msec(1000);}

}//END usercontrol()
