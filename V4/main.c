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
	// clearTimer(T1);
	// startTask(auton);
	// startTask(usercontrol);

	// string displayString;
	// sprintf(displayString, "%i\n%i\n%i", vrDisabled, vrAutonomousMode, vrCompetitionSwitch);
	// writeDebugStreamLine(displayString);

	#include "pre_auton.c"
}//END pre_auton

///////////////////////////////////////////////////////////////////////////////////
// AUTONOMOUS
///////////////////////////////////////////////////////////////////////////////////

task autonomous(){
	writeDebugStreamLine("auton");

	startTask(displayTime);
	#include "auton.c"
	stopTask(displayTime);
}

///////////////////////////////////////////////////////////////////////////////////
// USERCONTROL
///////////////////////////////////////////////////////////////////////////////////

task usercontrol(){
	writeDebugStreamLine("usrctrl");

	//motor[liftR] = 60;
	//motor[driveR2] = 60;
	//pointTurn(180);
	//startTask(drivePIDTask);

	//swingPID.target = SWING_90;
	//startTask(swingPIDTask);

	//motor[liftL] = 60;
	//while(true){wait1Msec(1000);}
	// while(true){
	//// 	motor[driveL1] = 69;
	//// 	writeDebugStreamLine("%i", vexRT[Ch3]);
	// 	wait1Msec(10);
	// } // for testing code above here

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
