////////////////////////////////////////////////////////////////
//
// Variables for anyone to modify for tuning
//
////////////////////////////////////////////////////////////////

// etc constants
#define sploof

// uncommenting this will turn off PID control of the arm in usercontrol
// #define ARM_CENTERING_OFF
// uncommenting this will output debug information for the waitForPID function
// #define DEBUG_PID_WAIT_FUNC
// if this is defined then the waitForPID() function will wait until the speed
// aswell as the error are close to zero
#define WAIT_FOR_PID_CHECK_SPEED

#define AUTON_RELOAD_TIME 0 // ms waited for human placement of preload cones
// number of times to stack preloads in auton
// there are 13 preloads
#define AUTON_STACK_COUNT 13
#define INCHES_PER_CONE 2.5 // inches added with each cone

#define ARM_SCHMEDIUM 500//?
#define ARM_PRELOAD_HEIGHT 500//?
#define ARM_BLOCK_MOGO 0 // 1200
#define ARM_TICKS_PER_INCH 38
#define ARM_DEADBAND 10
#define ARM_STARTING_HEIGHT 100//?
// distance the arm must go up for the swing to clear the cone
// in armPot ticks
#define ARM_CLEAR_CONE 300//?

#define SWING_IN 4000
#define SWING_OUT 1400
#define SWING_90 1900//?
// the max distance away from the armPID target that the swing should turn on
// in armPot ticks
#define SWING_ACTIVATION_DIST 500//?
// swing position where it has completely cleared the cone
// in swingPot ticks
#define SWING_CLEAR_CONE 1900//?

#define CLAW_OPEN_TIME 200//? // the time in ms for the claw to open
#define CLAW_OPEN 0
#define CLAW_CLOSE 1

#define MOGO_EXTEND_TIME 700
#define MOGO_INTAKE_TIME 900
#define MOGO_SECONDARY_WAIT_TIME 100

#define DRIVE_DEADBAND 15 // amount of power where drive wont move
#define DRIVE_SLEW_TIME 15 // update period for drive slew rate
#define DRIVE_SLEW_RATE 15 // maximum motor power change allowed per DRIVE_SLEW_TIME ms
#define DRIVE_TPI 34.5 // 28
// 392 * (1/pi*D)

// PID tuning variables
void initPIDVars() {
	drivePID.kP = .75; 		// P
	drivePID.kI = .0005;//0.002; // I
	drivePID.kD = 50;//80;
	drivePID.enabled = true;
	drivePID.debug = false;
	drivePID.integralLimit = 127;
	drivePID.integralActiveZone = 127./drivePID.kP;
	drivePID.loopTime = 50; // ms
	drivePID.errorThreshold = 50;
	drivePID.speedThreshold = 0.4;

	gyroPID.kP = .5; // P
	gyroPID.kI = 0.0009;//.002; // I
	gyroPID.kD = 65;//80; // D
	gyroPID.enabled = true;
	gyroPID.debug = false;
	gyroPID.integralLimit = 127;
	gyroPID.integralActiveZone = 60./gyroPID.kP;
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
	armPID.loopTime = 30; // ms
	armPID.errorThreshold = 50;
	armPID.speedThreshold = 0.1;//7;
	armPID.deadband = 15;

	armCrossCouplePID.kP = .3;//.1;//.1 			// P
	armCrossCouplePID.kI = 0;//0.0001;//0.000060; 	// I
	armCrossCouplePID.kD = 10;//10;//20;//40; 				// D
	armCrossCouplePID.enabled = true;
	armCrossCouplePID.debug = true;
	armCrossCouplePID.integralLimit = 127;
	armCrossCouplePID.integralActiveZone = 40./armCrossCouplePID.kP;
	armCrossCouplePID.loopTime = armPID.loopTime; // ms
	armCrossCouplePID.errorThreshold = 50;
	armCrossCouplePID.speedThreshold = 10;
	armCrossCouplePID.deadband = 15;

	swingPID.kP = .1; // P
	swingPID.kI = 0.0002;//0.0002; // I
	swingPID.kD = 5; // D
	swingPID.enabled = true;
	swingPID.debug = true;
	swingPID.integralLimit = 127;
	swingPID.integralActiveZone = 60./swingPID.kP;
	swingPID.loopTime = 30;//20; // ms
	swingPID.errorThreshold = 50;
	swingPID.speedThreshold = 10;
	swingPID.deadband = 10;
}
