////////////////////////////////////////////////////////////////
//
// Variables for anyone to modify for tuning
//
////////////////////////////////////////////////////////////////
#pragma systemFile

// etc constants
#define sploof

//uncommenting this will turn off PID control of the arm in usercontrol
#define USERCONTROL_ARM_PID_OFF

#define COORDINATE_MONITORING_PERIOD 5 // ms

#define ARM_POT_OFFSET 80
// amount that right side is bigger than left side
// smaller values tilts it right
// bigger values tilts it left

#define ARM_SCHMEDIUM 1300
#define ARM_PRELOAD_HEIGHT 1300
#define ARM_BLOCK_MOGO 1200
#define ARM_TICKS_PER_INCH 38
#define ARM_DEADBAND 10

#define SWING_IN 3400
#define SWING_OUT 550
#define SWING_90 1900//?

#define MOGO_EXTEND_TIME 700
#define MOGO_INTAKE_TIME 900
#define MOGO_SECONDARY_WAIT_TIME 100

#define DRIVE_DEADBAND 10 // amount of power where drive wont move
#define DRIVE_SLEW_TIME 7 // update period for drive slew rate
#define DRIVE_TPI 22.75 // 28
// 392 * (1/pi*D)

// PID tuning variables
void initPIDVars(){

	drivePID.Kp = .75; 		// P
	drivePID.Ki = .0005;//0.002; // I
	drivePID.Kd = 50;//80;
	drivePID.enabled = true;
	drivePID.debug = false;
	drivePID.integralLimit = 127;
	drivePID.integralActiveZone = 127./drivePID.Kp;
	drivePID.loopTime = 50; // ms
	drivePID.errorThreshold = 20;
	drivePID.speedThreshold = 0.4;

	gyroPID.Kp = 1; // P
	gyroPID.Ki = .002; // I
	gyroPID.Kd = 80; // D
	gyroPID.enabled = true;
	gyroPID.debug = true;
	gyroPID.integralLimit = 127;
	gyroPID.integralActiveZone = 127./gyroPID.Kp;
	gyroPID.loopTime = drivePID.loopTime;//50; // ms
	gyroPID.errorThreshold = 20; // 2 degree of error
	gyroPID.speedThreshold = 0.045; // no more than .045 degree per millisecond or 45 degrees per second
	gyroPID.target = SensorValue[gyro];

	armPID.Kp = .6; 			// P
	armPID.Ki = 0.0009; // I
	armPID.Kd = 50;//30;//7; 				// D
	armPID.enabled = true;
	armPID.debug = false;
	armPID.integralLimit = 50;
	armPID.integralActiveZone = 127./armPID.Kp;
	armPID.loopTime = 50; // ms
	armPID.errorThreshold = 50;
	armPID.speedThreshold = 0.1;//7;
	armPID.deadband = 15;

	armCrossCouplePID.Kp = .1;//.1;//.1 			// P
	armCrossCouplePID.Ki = 0;//0.0001;//0.000060; 	// I
	armCrossCouplePID.Kd = 10;//10;//20;//40; 				// D
	armCrossCouplePID.enabled = true;
	armCrossCouplePID.debug = true;
	armCrossCouplePID.integralLimit = 40;
	armCrossCouplePID.integralActiveZone = 20./armCrossCouplePID.Kp;
	armCrossCouplePID.loopTime = armPID.loopTime; // ms
	armCrossCouplePID.errorThreshold = 50;
	armCrossCouplePID.speedThreshold = 10;
	armCrossCouplePID.deadband = 15;

	swingPID.Kp = .2; // P
	swingPID.Ki = 0.0001; // I
	swingPID.Kd = 10; // D
	swingPID.enabled = true;
	swingPID.debug = false;
	swingPID.integralLimit = 50;
	swingPID.integralActiveZone = 127./swingPID.Kp;
	swingPID.loopTime = 10;//20; // ms
	swingPID.errorThreshold = 50;
	swingPID.speedThreshold = 10;
	swingPID.deadband = 10;
}
