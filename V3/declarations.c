//////////////////////////////////////////////////
//
// Global variables and declarations for functions
//
//////////////////////////////////////////////////
//#pragma systemFile

// define statements that shouldn't need to be altered
#define LEFT_LCD 1
#define CENTER_LCD 2
#define RIGHT_LCD 4

#define DEFAULT 0
#define BLUE_PRELOAD 1
#define RED_PRELOAD 2

#define CLAW_OPEN 0
#define CLAW_CLOSE 1

#define X_COORD 0
#define Y_COORD 1

#define SWING_LEFT -1
#define POINT_TURN 0
#define SWING_RIGHT 1
#define TARD 2
#define TARD_ACTIVE_ROT 3

typedef struct PIDStruct{
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

	float Kp;
	float Ki;
	float Kd;
	float integralLimit;
	float integralActiveZone;

	int 	loopTime;

}PIDStruct;

PIDStruct drivePID, gyroPID, armPID, armCrossCouplePID, swingPID;

int autonSelection = DEFAULT;
float globalXPos = 0;
float globalYPos = 0;
float startingRotationOffset = 90; // 90 is facing positive y (trig style)
short driveMode = POINT_TURN;


void setLift(int setPow);
void waitForPID(PIDStruct PIDVar);
task armPIDTask();
