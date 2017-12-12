////////////////////////////////////////////////////////////////
//
// Variables for anyone to modify for tuning
//
////////////////////////////////////////////////////////////////

// etc constants
#define COORDINATE_MONITORING_PERIOD 5 // ms

#define ARM_POT_OFFSET 80
// amount that right side is bigger than left side
// smaller values tilts it right
// bigger values tilts it left

#define ARM_SCHMEDIUM 1700
#define ARM_PRELOAD_HEIGHT 1700
#define ARM_TICKS_PER_INCH 50//?

#define SWING_IN 7//?
#define SWING_OUT 7//?
#define SWING_90 90//?

#define MOGO_EXTEND_TIME 700
#define MOGO_INTAKE_TIME 700
#define MOGO_SECONDARY_WAIT_TIME 100

#define DRIVE_TICKS_PER_INCH = 31.1944
// 392 * (1/pi*D)

// PID tuning variables
void initPIDVars(){

	driveLPID.Kp = 0.36; 		// P
	driveLPID.Ki = 0.00040; // I
	driveLPID.Kd = 8; 			// D
	driveLPID.integralLimit = 50;
	driveLPID.integralActiveZone = 127./driveLPID.Kp;
	driveLPID.loopTime = 50; // ms
	driveLPID.debug = false;
	driveLPID.errorThreshold = 50;
	driveLPID.speedThreshold = 7;

	driveRPID = driveLPID;
	driveRPID.debug = false;

	gyroPID.Kp = 1; // P
	gyroPID.Ki = 0; // I
	gyroPID.Kd = 0; // D
	gyroPID.integralLimit = 50;
	gyroPID.integralActiveZone = 127./gyroPID.Kp;
	gyroPID.loopTime = 50; // ms
	gyroPID.debug = false;
	gyroPID.target = startingRotationOffset;
	gyroPID.errorThreshold = 1; // 1 degree of error
	gyroPID.speedThreshold = 0.045; // no more than .045 degree per millisecond or 45 degrees per second

	armPID.Kp = 0.2; 			// P
	armPID.Ki = 0.00020; // I
	armPID.Kd = 7; 				// D
	armPID.integralLimit = 50;
	armPID.integralActiveZone = 127./armPID.Kp;
	armPID.loopTime = 50; // ms
	armPID.debug = true;
	armPID.errorThreshold = 50;
	armPID.speedThreshold = 7;

	armCrossCouplePID.Kp = .7;//.1 			// P
	armCrossCouplePID.Ki = 0.0005;//0.000060; 	// I
	armCrossCouplePID.Kd = 20; 				// D
	armCrossCouplePID.integralLimit = 1270;
	armCrossCouplePID.integralActiveZone = 127./armCrossCouplePID.Kp;
	armCrossCouplePID.loopTime = 50; // ms
	armCrossCouplePID.debug = true;
	armCrossCouplePID.errorThreshold = 50;
	armCrossCouplePID.speedThreshold = 10;

	mogoPID.Kp = 1; // P
	mogoPID.Ki = 0; // I
	mogoPID.Kd = 0; // D
	mogoPID.integralLimit = 50;
	mogoPID.integralActiveZone = 127./mogoPID.Kp;
	mogoPID.loopTime = 50; // ms
	mogoPID.debug = true;

	swingPID.Kp = 1; // P
	swingPID.Ki = 0; // I
	swingPID.Kd = 0; // D
	swingPID.integralLimit = 50;
	swingPID.integralActiveZone = 127./swingPID.Kp;
	swingPID.loopTime = 50; // ms
	swingPID.debug = false;
}
