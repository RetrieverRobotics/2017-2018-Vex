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
#define BLUE_PRELOAD 1
#define RED_PRELOAD 2
#define NUM_AUTONS 3

#define X_COORD 0
#define Y_COORD 1

#define POINT_TURN 0
#define SWING_LEFT 1
#define SWING_RIGHT 2
#define TARD 3
#define TARD_ACTIVE_ROT 4

#define ARM_PID_CONTROL 0
#define ARM_TARD 1
#define ARM_TARD_ACTIVE_CENTERING 2

typedef struct PIDStruct {
	bool debug;
	bool enabled;
	float target;
	float previousError;
	float integral;
	int 	output;
	int 	deadband;
	float input;
	float errorThreshold;
	float speedThreshold;

	float kP;
	float kI;
	float kD;
	float integralLimit;
	float integralActiveZone;

	int 	loopTime;

}PIDStruct;

PIDStruct drivePID, gyroPID, armPID, armCrossCouplePID, swingPID;

int autonSelection = DEFAULT;
float startingRotationOffset = 90; // 90 is facing positive y (trig style)
short driveMode = POINT_TURN;
short armMode = ARM_PID_CONTROL;


void setLift(int setPow);
void waitForPID(PIDStruct PIDVar, bool checkSpeed = true);
task armPIDTask();
