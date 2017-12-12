/////////////////////
// Declarations for functions
/////////////////////

// define statements that shouldn't need to be altered
#define LEFT_LCD 1
#define CENTER_LCD 2
#define RIGHT_LCD 4
#define GET_HEADING() ((SensorValue[gyro] / 10) + 90 + startingRotationOffset)
#define BLUE_PRELOAD 1
#define CLAW_OPEN 0
#define CLAW_CLOSE 1

//void initAllPIDsToZero();
//void initAllPIDsToZero(PIDStruct *PIDVars[7]);
void setLift(int setPow);
void waitUntilPIDAtTarget(PIDStruct PIDVar);
