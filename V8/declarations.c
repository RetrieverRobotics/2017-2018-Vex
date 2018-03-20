//////////////////////////////////////////////////
//
// Global variables and declarations for functions
//
//////////////////////////////////////////////////

// define statements that shouldn't need to be altered
#define LEFT_LCD 1
#define CENTER_LCD 2
#define RIGHT_LCD 4

#define DEFAULT 0 // for auton selection
#define BLUE_PRIMARY 1
#define RED_PRIMARY 2
#define STATIONARY_CONE 3
#define RAM_AUTON 4
#define NUM_AUTONS 5

#define X_COORD 0
#define Y_COORD 1

#define POINT_TURN 0
#define SWING_LEFT 1
#define SWING_RIGHT 2
#define TARD 3
#define TARD_ACTIVE_ROT 4
#define SLOW_DRIVE 5

#define LIFT_PID_CONTROL 0
#define LIFT_TARD 1

typedef struct PIDStruct {
	bool debug;
	bool enabled;
	float target;
	float error;
	float previousError;
	float integral;
	int 	output;
	int 	deadband;
	float input;
	float inputLast;
	float errorThreshold;

	float kP;
	float kI;
	float kD;
	float integralLimit;
	float integralActiveZone;

	int 	loopTime;

}PIDStruct;

PIDStruct drivePID, gyroPID, liftPID, swingPID;

int autonSelection = DEFAULT;
float gyroOffset = 0;
short driveMode = POINT_TURN;
short liftMode = LIFT_PID_CONTROL;
int slowDrivePow = 60;

void setLift(int setPow);
bool waitForPID(PIDStruct PIDVar, int timeout);
void waitForPID(PIDStruct PIDVar);
task liftPIDTask();
