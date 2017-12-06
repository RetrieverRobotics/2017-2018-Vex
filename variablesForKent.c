////////////////////////////////////////////////////////////////
//
// Variables for anyone to modify for tuning
//
////////////////////////////////////////////////////////////////

// etc constants
#define COORDINATE_MONITORING_PERIOD 5 // ms
#define ARM_POT_OFFSET 150

// PID tuning variables
void initPIDVars(){
	// set everything to 0 initially
	initAllPIDsToZero();

	driveLPID.Kp = 0.36; 		// P
	driveLPID.Ki = 0.00040; // I
	driveLPID.Kd = 8; 			// D
	driveLPID.integralLimit = 50;
	driveLPID.integralActiveZone = 127/driveLPID.Kp;
	driveLPID.loopTime = 50; // ms
	driveLPID.debug = false;

	driveRPID = driveLPID;
	driveRPID.debug = false;

	gyroPID.Kp = 0; // P
	gyroPID.Ki = 0; // I
	gyroPID.Kd = 0; // D
	gyroPID.integralLimit = 50;
	//gyroPID.integralActiveZone = 127/gyroPID.Kp;
	gyroPID.loopTime = 50; // ms
	gyroPID.debug = false;
	gyroPID.target = startingRotationOffset;

	armPID.Kp = 0.1; 			// P
	armPID.Ki = 0.000060; // I
	armPID.Kd = 7; 				// D
	armPID.integralLimit = 50;
	armPID.integralActiveZone = 127/armPID.Kp;
	armPID.loopTime = 50; // ms
	armPID.debug = false;

	armCrossCouplePID.Kp = 0.1; 			// P
	armCrossCouplePID.Ki = 0.000060; 	// I
	armCrossCouplePID.Kd = 7; 				// D
	armCrossCouplePID.integralLimit = 50;
	armCrossCouplePID.integralActiveZone = 127/armCrossCouplePID.Kp;
	armCrossCouplePID.loopTime = 50; // ms
	armCrossCouplePID.debug = false;

	mogoPID.Kp = 0; // P
	mogoPID.Ki = 0; // I
	mogoPID.Kd = 0; // D
	mogoPID.integralLimit = 50;
	//mogoPID.integralActiveZone = 127/mogoPID.Kp;
	mogoPID.loopTime = 50; // ms
	mogoPID.debug = true;

	swingPID.Kp = 0; // P
	swingPID.Ki = 0; // I
	swingPID.Kd = 0; // D
	swingPID.integralLimit = 50;
	//swingPID.integralActiveZone = 127/swingPID.Kp;
	swingPID.loopTime = 50; // ms
	swingPID.debug = false;
}
