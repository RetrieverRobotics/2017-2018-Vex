////////////////////////////////////////////////////////////////
//
// Variables for anyone to modify for tuning
//
////////////////////////////////////////////////////////////////

// etc constants
#define sploof

// uncommenting this will turn off PID control of the arm in usercontrol
#define ARM_CENTERING_OFF
// uncommenting this will output debug information for the waitForPID function
// #define DEBUG_PID_WAIT_FUNC

#define AUTON_RELOAD_TIME 0 // ms waited for human placement of preload cones
#define INCHES_PER_CONE 2.5 // inches added with each cone

#define COORDINATE_MONITORING_PERIOD 5 // ms

#define ARM_SCHMEDIUM 1300
#define ARM_PRELOAD_HEIGHT 1300
#define ARM_BLOCK_MOGO 1200
#define ARM_TICKS_PER_INCH 38
#define ARM_DEADBAND 10
#define ARM_STARTING_HEIGHT 1000

#define SWING_IN 3400
#define SWING_OUT 550
#define SWING_90 1900//?
// the max distance away from the armPID target that the swing should turn on
// in armPot ticks
#define SWING_ACTIVATION_DIST 500//?
// distance the arm must go up for the swing to clear the cone
// in armPot ticks
#define SWING_CLEAR_CONE 300//?

#define MOGO_EXTEND_TIME 700
#define MOGO_INTAKE_TIME 900
#define MOGO_SECONDARY_WAIT_TIME 100

#define DRIVE_DEADBAND 10 // amount of power where drive wont move
#define DRIVE_SLEW_TIME 7 // update period for drive slew rate
#define DRIVE_TPI 22.75 // 28
// 392 * (1/pi*D)

// PID tuning variables
void initPIDVars(){
	drivePID.kP = .75; 		// P
	drivePID.kI = .0005;//0.002; // I
	drivePID.kD = 50;//80;
	drivePID.enabled = true;
	drivePID.debug = false;
	drivePID.integralLimit = 127;
	drivePID.integralActiveZone = 127./drivePID.kP;
	drivePID.loopTime = 50; // ms
	drivePID.errorThreshold = 20;
	drivePID.speedThreshold = 0.4;

	gyroPID.kP = 1; // P
	gyroPID.kI = .002; // I
	gyroPID.kD = 80; // D
	gyroPID.enabled = true;
	gyroPID.debug = false;
	gyroPID.integralLimit = 127;
	gyroPID.integralActiveZone = 127./gyroPID.kP;
	gyroPID.loopTime = drivePID.loopTime;//50; // ms
	gyroPID.errorThreshold = 20; // 2 degree of error
	gyroPID.speedThreshold = 0.045; // no more than .045 degree per millisecond or 45 degrees per second
	gyroPID.target = SensorValue[gyro];

	armPID.kP = .6; 			// P
	armPID.kI = 0.0009; // I
	armPID.kD = 50;//30;//7; 				// D
	armPID.enabled = true;
	armPID.debug = false;
	armPID.integralLimit = 127;
	armPID.integralActiveZone = 127./armPID.kP;
	armPID.loopTime = 50; // ms
	armPID.errorThreshold = 50;
	armPID.speedThreshold = 0.1;//7;
	armPID.deadband = 15;

	armCrossCouplePID.kP = .1;//.1;//.1 			// P
	armCrossCouplePID.kI = 0;//0.0001;//0.000060; 	// I
	armCrossCouplePID.kD = 10;//10;//20;//40; 				// D
	armCrossCouplePID.enabled = true;
	armCrossCouplePID.debug = false;
	armCrossCouplePID.integralLimit = 40;
	armCrossCouplePID.integralActiveZone = 20./armCrossCouplePID.kP;
	armCrossCouplePID.loopTime = armPID.loopTime; // ms
	armCrossCouplePID.errorThreshold = 50;
	armCrossCouplePID.speedThreshold = 10;
	armCrossCouplePID.deadband = 15;

	swingPID.kP = .2; // P
	swingPID.kI = 0.0001; // I
	swingPID.kD = 10; // D
	swingPID.enabled = true;
	swingPID.debug = false;
	swingPID.integralLimit = 127;
	swingPID.integralActiveZone = 127./swingPID.kP;
	swingPID.loopTime = 10;//20; // ms
	swingPID.errorThreshold = 50;
	swingPID.speedThreshold = 10;
	swingPID.deadband = 10;
}
