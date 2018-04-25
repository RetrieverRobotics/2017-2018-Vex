////////////////////////////////////////////////////////////////
//
// Variables for anyone to modify for tuning
//
////////////////////////////////////////////////////////////////

// etc constants
#define sploof

#define DAB_TIME 13
#define SENSOR_POLL_TIME 20

#define AUTON_RELOAD_TIME 0 // ms waited for human placement of preload cones
// number of times to stack preloads in auton
// there are 13 preloads
#define AUTON_STACK_COUNT 9
#define INCHES_PER_CONE 2.4 // inches added with each cone

#define LIFT_SCHMEDIUM 600//?
#define LIFT_HIGHER 1000
#define LIFT_PRELOAD_HEIGHT 500//?
#define LIFT_FLOOR_HEIGHT 360//350
#define LIFT_BLOCK_MOGO 600 // 1200
#define LIFT_TICKS_PER_INCH 38
#define LIFT_DEADBAND 10
#define LIFT_STARTING_HEIGHT 400//?
// distance the lift must go up for the swing to clear the cone
// in liftPot ticks
#define LIFT_CLEAR_CONE 550//?
#define LIFT_CONE_2 550
#define LIFT_CONE_3 600
#define LIFT_CONE_4 700
#define LIFT_DROP_DELTA 50
#define LIFT_STATIONARY 1000
#define LIFT_HOLD_DOWN_POW -127

#define SWING_OUT_TIME 500
#define SWING_IN_TIME 400
#define SWING_HOLD_IN_POW 15//15
#define SWING_HOLD_OUT_POW 20//15

#define FLIPPER_OUT 1
#define FLIPPER_IN 0

#define MOGO_EXTEND_TIME 1000//1100//1000
#define MOGO_INTAKE_TIME 1000//1500
#define MOGO_SECONDARY_WAIT_TIME 0//100

#define DRIVE_DEADBAND 10 // amount of power where drive wont move
#define DRIVE_SLEW_TIME 15 // update period for drive slew rate
#define DRIVE_SLEW_RATE 127//15 // maximum motor power change allowed per DRIVE_SLEW_TIME ms
#define DRIVE_TPI 30
// 392 * (1/pi*D)
// #define POLY_DRIVE // if this is defined, poly drive will be in effect
#define POLY_DRIVE_DEGREE 1.5//1.5
// if this is defined, left stick is power and right stick is turn
// #define DRIVE_ALT_STICKS
#define DRIVE_LEFT_MULTIPLIER 0.94444444444

#define JOYSTICKS_DEADBAND 10

#define ROLLERS_IN 127
#define ROLLERS_OUT -127
#define ROLLERS_HOLD 20

#define PID_SETTLE_TIME 0 // ms

#define MOGO_PISTONS_IN 0
#define MOGO_PISTONS_OUT 1

#define CONE_PICKUP_WAIT_TIME 500
#define CONE_RELEASE_TIME 500
#define CONE_SECONDARY_RELEASE_TIME 10

// PID tuning variables
void initPIDVars() {
	drivePID.kP = .31;
	drivePID.kI = 0.0007;//0.001;
	drivePID.kD = 39;//37//60;
	drivePID.enabled = true;
	drivePID.debug = false;
	drivePID.integralLimit = 127;
	drivePID.integralActiveZone = 40./drivePID.kP;
	drivePID.loopTime = 50; // ms
	drivePID.errorThreshold = 30;//50

	gyroPID.kP = .3;//.25;//.15;//.7;
	gyroPID.kI = 0.0004;//.002//0.0009;
	gyroPID.kD = 35;//21//18//65;
	gyroPID.enabled = true;
	gyroPID.debug = true;
	gyroPID.integralLimit = 127;
	gyroPID.integralActiveZone = 40./gyroPID.kP;
	gyroPID.loopTime = drivePID.loopTime;//50; // ms
	gyroPID.errorThreshold = 30;//50//100 // 5 degree of error
	gyroPID.target = SensorValue[gyro];

	liftPID.kP = .3;
	liftPID.kI = 0.0005;
	liftPID.kD = 40;
	liftPID.enabled = true;
	liftPID.debug = false;
	liftPID.integralLimit = 127;
	liftPID.integralActiveZone = 60./liftPID.kP;
	liftPID.loopTime = 50; // ms
	liftPID.errorThreshold = 200;
	liftPID.deadband = 10;

	swingPID.kP = .13;
	swingPID.kI = 0.0004;
	swingPID.kD = 10;
	swingPID.enabled = true;
	swingPID.debug = false;
	swingPID.integralLimit = 127;
	swingPID.integralActiveZone = 60./swingPID.kP;
	swingPID.loopTime = 30;//20; // ms
	swingPID.errorThreshold = 100;
	swingPID.deadband = 10;
}
