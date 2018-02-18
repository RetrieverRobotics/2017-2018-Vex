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
// #define WAIT_FOR_PID_CHECK_SPEED

#define DAB_TIME 13
#define SENSOR_POLL_TIME 20

#define AUTON_RELOAD_TIME 0 // ms waited for human placement of preload cones
// number of times to stack preloads in auton
// there are 13 preloads
#define AUTON_STACK_COUNT 9
#define INCHES_PER_CONE 2.4 // inches added with each cone

#define ARM_SCHMEDIUM 500//?
#define ARM_PRELOAD_HEIGHT 200//?
#define ARM_BLOCK_MOGO 350 // 1200
#define ARM_TICKS_PER_INCH 38
#define ARM_DEADBAND 10
#define ARM_STARTING_HEIGHT 100//?
// distance the arm must go up for the swing to clear the cone
// in armPot ticks
#define ARM_CLEAR_CONE 250//?

#define SWING_IN 600
#define SWING_OUT 3000//2500 // preloads
#define SWING_FLOOR 3400
#define SWING_90 1500//?
// the max distance away from the armPID target that the swing should turn on
// in armPot ticks
#define SWING_ACTIVATION_DIST 700//?
// swing position where it has completely cleared the cone
// in swingPot ticks
#define SWING_CLEAR_CONE 1900//?
#define SWING_OUT_TIME 700

#define CLAW_OPEN_TIME 200//? // the time in ms for the claw to open
#define CLAW_CLOSE_TIME 200
#define CLAW_OPEN 1
#define CLAW_CLOSE 0

#define FLIPPER_OUT 1
#define FLIPPER_IN 0

#define MOGO_EXTEND_TIME 800
#define MOGO_INTAKE_TIME 1500
#define MOGO_SECONDARY_WAIT_TIME 100

#define DRIVE_DEADBAND 15 // amount of power where drive wont move
#define DRIVE_SLEW_TIME 15 // update period for drive slew rate
#define DRIVE_SLEW_RATE 15 // maximum motor power change allowed per DRIVE_SLEW_TIME ms
#define DRIVE_TPI 36 //24 // 28
// 392 * (1/pi*D)

// PID tuning variables
void initPIDVars() {
	drivePID.kP = .75;//.75; 		// P
	drivePID.kI = 0.001;//.0007;//0.002; // I
	drivePID.kD = 60;//50;//80;
	drivePID.enabled = true;
	drivePID.debug = false;
	drivePID.integralLimit = 127;
	drivePID.integralActiveZone = 40./drivePID.kP;
	drivePID.loopTime = 50; // ms
	drivePID.errorThreshold = 50;

	gyroPID.kP = .7; // P
	gyroPID.kI = 0.0009;//.002; // I
	gyroPID.kD = 65;//80; // D
	gyroPID.enabled = true;
	gyroPID.debug = true;
	gyroPID.integralLimit = 127;
	gyroPID.integralActiveZone = 60./gyroPID.kP;
	gyroPID.loopTime = drivePID.loopTime;//50; // ms
	gyroPID.errorThreshold = 100; // 10 degree of error
	gyroPID.target = SensorValue[gyro];

	armPID.kP = .3; 			// P
	armPID.kI = .0009;//0.0009; // I
	armPID.kD = 30;//50;//30;//7; 				// D
	armPID.enabled = true;
	armPID.debug = false;
	armPID.integralLimit = 127;
	armPID.integralActiveZone = 60./armPID.kP;
	armPID.loopTime = 50; // ms
	armPID.errorThreshold = 200;
	armPID.deadband = 10;

	armCrossCouplePID.kP = .6;//.4;//.1;//.1 			// P
	armCrossCouplePID.kI = 0;//0.0004;//0.0001;//0.000060; 	// I
	armCrossCouplePID.kD = 20;//60;//10;//20;//40; 				// D
	armCrossCouplePID.enabled = true;
	armCrossCouplePID.debug = false;
	armCrossCouplePID.integralLimit = 127;
	armCrossCouplePID.integralActiveZone = 40./armCrossCouplePID.kP;
	armCrossCouplePID.loopTime = armPID.loopTime; // ms
	armCrossCouplePID.errorThreshold = 50;
	armCrossCouplePID.deadband = 10;//20;

	swingPID.kP = .13;//.2 // P
	swingPID.kI = 0.0004;//.0004 // I
	swingPID.kD = 10;//20 // D
	swingPID.enabled = true;
	swingPID.debug = false;
	swingPID.integralLimit = 127;
	swingPID.integralActiveZone = 60./swingPID.kP;
	swingPID.loopTime = 30;//20; // ms
	swingPID.errorThreshold = 100;
	swingPID.deadband = 10;
}
