////////////////////////////////////////////////////////////////
//
// Variables for anyone to modify for tuning
//
////////////////////////////////////////////////////////////////

// etc constants
#define sploof

// uncommenting this will turn off PID control of the lift in usercontrol
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
// distance the lift must go up for the swing to clear the cone
// in liftPot ticks
#define ARM_CLEAR_CONE 250//?

#define SWING_IN 1
#define SWING_OUT 0
#define SWING_CLEAR_CONE 1900//?
#define SWING_OUT_TIME 700

#define FLIPPER_OUT 1
#define FLIPPER_IN 0

#define MOGO_EXTEND_TIME 800
#define MOGO_INTAKE_TIME 1500
#define MOGO_SECONDARY_WAIT_TIME 100

#define DRIVE_DEADBAND 5 // amount of power where drive wont move
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

	liftPID.kP = .3; 			// P
	liftPID.kI = .0009;//0.0009; // I
	liftPID.kD = 30;//50;//30;//7; 				// D
	liftPID.enabled = true;
	liftPID.debug = false;
	liftPID.integralLimit = 127;
	liftPID.integralActiveZone = 60./liftPID.kP;
	liftPID.loopTime = 50; // ms
	liftPID.errorThreshold = 200;
	liftPID.deadband = 10;

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
