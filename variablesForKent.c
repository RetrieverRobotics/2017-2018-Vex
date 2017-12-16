////////////////////////////////////////////////////////////////
//
// Variables for anyone to modify for tuning
//
////////////////////////////////////////////////////////////////

// etc constants
#define COORDINATE_MONITORING_PERIOD 5 // ms
typedef float duck;
duck sploof = 7;

#define ARM_POT_OFFSET 80
// amount that right side is bigger than left side
// smaller values tilts it right
// bigger values tilts it left

#define ARM_SCHMEDIUM 1300
#define ARM_PRELOAD_HEIGHT 1300
#define ARM_TICKS_PER_INCH 50//?

#define SWING_IN 3400//?
#define SWING_OUT 550//
#define SWING_90 90//?

#define MOGO_EXTEND_TIME 700
#define MOGO_INTAKE_TIME 900
#define MOGO_SECONDARY_WAIT_TIME 100

#define DRIVE_TICKS_PER_INCH 28// 392 * (1/pi*D)

// PID tuning variables
void initPIDVars(){

	driveLPID.enabled = true;
	driveLPID.Kp = .75; 		// P
	driveLPID.Ki = .0005;//0.002; // I
	driveLPID.Kd = 50;//80;
	driveLPID.integralLimit = 127;
	driveLPID.integralActiveZone = 127./driveLPID.Kp;
	driveLPID.loopTime = 50; // ms
	driveLPID.debug = true;
	driveLPID.errorThreshold = 20;
	driveLPID.speedThreshold = 0.4;

	driveRPID.enabled = false;
	driveRPID.Kp = driveLPID.Kp; 		// P
	driveRPID.Ki = driveLPID.Ki; // I
	driveRPID.Kd = driveLPID.Kd; 			// D
	driveRPID.integralLimit = driveLPID.integralLimit;
	driveRPID.integralActiveZone = 127./driveRPID.Kp;
	driveRPID.loopTime = driveLPID.loopTime; // ms
	driveRPID.debug = false;
	driveRPID.errorThreshold = driveLPID.errorThreshold;
	driveRPID.speedThreshold = driveLPID.speedThreshold;

	gyroPID.enabled = true;
	gyroPID.Kp = 1; // P
	gyroPID.Ki = .002; // I
	gyroPID.Kd = 80; // D
	gyroPID.integralLimit = 127;
	gyroPID.integralActiveZone = 127./gyroPID.Kp;
	gyroPID.loopTime = 50; // ms
	gyroPID.debug = false;
	gyroPID.target = startingRotationOffset;
	gyroPID.errorThreshold = 50; // 5 degree of error
	gyroPID.speedThreshold = 0.045; // no more than .045 degree per millisecond or 45 degrees per second

	armPID.enabled = true;
	armPID.Kp = 0.2; 			// P
	armPID.Ki = 0.00020; // I
	armPID.Kd = 7; 				// D
	armPID.integralLimit = 50;
	armPID.integralActiveZone = 127./armPID.Kp;
	armPID.loopTime = 10; // ms
	armPID.debug = false;
	armPID.errorThreshold = 10;
	armPID.speedThreshold = 7;

	armCrossCouplePID.enabled = true;
	armCrossCouplePID.Kp = .1;//.1 			// P
	armCrossCouplePID.Ki = 0.0005;//0.000060; 	// I
	armCrossCouplePID.Kd = 40; 				// D
	armCrossCouplePID.integralLimit = 40;
	armCrossCouplePID.integralActiveZone = 40./armCrossCouplePID.Kp;
	armCrossCouplePID.loopTime = 10; // ms
	armCrossCouplePID.debug = true;
	armCrossCouplePID.errorThreshold = 50;
	armCrossCouplePID.speedThreshold = 10;

	mogoPID.enabled = true;
	mogoPID.Kp = 1; // P
	mogoPID.Ki = 0; // I
	mogoPID.Kd = 0; // D
	mogoPID.integralLimit = 50;
	mogoPID.integralActiveZone = 127./mogoPID.Kp;
	mogoPID.loopTime = 10; // ms
	mogoPID.debug = false;

	swingPID.enabled = true;
	swingPID.Kp = .2; // P
	swingPID.Ki = 0.0001; // I
	swingPID.Kd = 10; // D
	swingPID.integralLimit = 50;
	swingPID.integralActiveZone = 127./swingPID.Kp;
	swingPID.loopTime = 10; // ms
	swingPID.debug = false;
}
