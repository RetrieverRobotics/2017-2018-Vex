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
#warning "main"
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

//#include "pre_auton.c"
}//END pre_auton

///////////////////////////////////////////////////////////////////////////////////
// AUTONOMOUS
///////////////////////////////////////////////////////////////////////////////////

task autonomous(){
	writeDebugStreamLine("auton");

	//////////////////////////////////////
	//driveIncremental(18);
	//pointTurn(90);
	//startTask(drivePIDTask);

	//while(true){wait1Msec(1000);}
	//////////////////////////////////////

	autonSelection = BLUE_DEFAULT;

	//startTask(displayTime);

	#include "auton.c"

	stopTask(displayTime);
}

///////////////////////////////////////////////////////////////////////////////////
// USERCONTROL
///////////////////////////////////////////////////////////////////////////////////

task usercontrol(){
	writeDebugStreamLine("usrctrl");


	//setLiftHeight(700);
	//startTask(liftPIDTask);
	//setLift(60);
	//wait1Msec(1000);
	//setLift(0);

//while(1){
//	motor[swing] = vexRT[Ch2];
//}
	//while(true){wait1Msec(1000);}
	// for testing code above here

	// check if second controller is connected
	if (nVexRCReceiveState & vrXmit2) {
		#include "usercontrol-2Controller.c"
	}
	else {
		#include "usercontrol-singleController.c"
	}

	if("we haven't won yet")
		smackVcat();

}//END usercontrol()
