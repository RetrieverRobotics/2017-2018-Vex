//////////////////////////////////////////////////////////////////////////
//
// Functions for Retriever Robotics 2017-2018 In The Zone
//
//////////////////////////////////////////////////////////////////////////

float getHeading(){
	return((SensorValue[gyro] / 10) + startingRotationOffset);
}

float degToGyro(float degrees){
	return (degrees - startingRotationOffset) * 10;
}

void smackVcat(){}

void drivePIDsOn(bool on){
	if(on){
		// turn drive PIDS back on set to where we are currently
		driveLPID.target = nMotorEncoder[driveL];
		driveRPID.target = nMotorEncoder[driveR];

		driveLPID.enabled = true;
		driveRPID.enabled = true;
	}
	else{
		driveLPID.enabled = false;
		driveRPID.enabled = false;
	}
}

void stackCone(int currStackHeight){

}

void intakeMogo(){
	motor[mogo] = 127; // positive for in
	wait1Msec(MOGO_INTAKE_TIME);
	motor[mogo] = 0;
}

void extendMogo(){
	motor[mogo] = -127; // negative for out
	wait1Msec(MOGO_EXTEND_TIME);
	motor[mogo] = -50;
	wait1Msec(MOGO_SECONDARY_WAIT_TIME);
	motor[mogo] = 0;
}

void driveIncremental(float distance){
	// TODO: check if drive PID started, if not start now and end it after
	// if getTaskPriority(drivePIDTask) > 0:
	// 		then itsProbablyRunning
	distance = distance * DRIVE_TICKS_PER_INCH;

	driveLPID.target = nMotorEncoder[driveL] + distance;
	//driveRPID.target = nMotorEncoder[driveR] + distance;
}

void turnCounterClockwise(float degrees){
	//turn off distance PIDS so they dont resist
	drivePIDsOn(false);

	gyroPID.target += degToGyro(degrees);

	waitUntilPIDAtTarget(gyroPID);

	// turn drive PIDS back on
	drivePIDsOn(true);
}

// void swingTurnLeft(float degrees){
// 	//turn off gyro PID
// 	gyroPID.enabled = false;
//
// 	//driveRPID.target = nMotorEncoder[driveR] + encoderDistance;
//
// 	//turn gyro PID back on at the new position
// 	//gyroPID.target = getHeading();
// 	gyroPID.enabled = true;
// }

//void driveAbsolute(float xCoord, float yCoord){
//	int heading = getHeading();

//}

void turnAbsolute(float degrees){
	//turn off distance PIDS so they dont resist
	drivePIDsOn(false);

	gyroPID.target = degToGyro(degrees);

	waitUntilPIDAtTarget(gyroPID);

	// turn drive PIDS back on
	drivePIDsOn(true);
}

void setLift(int setPow){
	motor[liftTl] = setPow;
	motor[liftTr] = setPow;
	motor[liftBl] = setPow;
	motor[liftBr] = setPow;
}

int lim127(int power){
	return(abs(power) > 127 ? sgn(power) * 127 : power);
}

//coordinate monitoring with gyro sensors and L&R drive encoders
task coordinateMonitoring(){
	float rotation;
	int leftEnc;
	int rightEnc;
	int deltaLeft;
	int deltaRight;
	int previousLeft;
	int previousRight;
	float aveDist;

	while(true){
		rotation = getHeading();
		leftEnc  = nMotorEncoder[driveL];
		rightEnc = nMotorEncoder[driveR];

		deltaLeft  = leftEnc 	- previousLeft;
		deltaRight = rightEnc - previousRight;

		aveDist = (deltaLeft + deltaRight) / 2; // average distance covered

		//integrate position

		globalXPos += aveDist * cosDegrees(rotation);
		globalYPos += aveDist * sinDegrees(rotation);

		previousLeft = leftEnc;
		previousRight = rightEnc;

		wait1Msec(COORDINATE_MONITORING_PERIOD);
	}//END while(true)
}//END coordinateMonitoring

/////////////////////////////////////////////////////////////
// PID functions
/////////////////////////////////////////////////////////////

//wait until position at target and velocity = 0
void waitUntilPIDAtTarget(PIDStruct PIDVar){
	float error;
	float inputLast = PIDVar.input;

	while(true){
		wait1Msec(PIDVar.loopTime);

		error = PIDVar.target - PIDVar.input;
		//writeDebugStream("%f\t", error);
		//writeDebugStream("%f\n", (PIDVar.input-inputLast) / PIDVar.loopTime);

		//if pos within desired range of target
		if(fabs(error) < PIDVar.errorThreshold){
			// if the speed is close to 0
			if(fabs((PIDVar.input-inputLast) / PIDVar.loopTime) < PIDVar.speedThreshold){
				return; // exit loop
			}
		}

		inputLast = PIDVar.input;
	}
	return;
}

//void initAllPIDsToZero(PIDStruct *PIDVars[7]){
//	writeDebugStreamLine("here3");
//	//testerino2->integral = 7778;
//	// iterate through each PID variable (drive, lift, etc)
//	for(int i = 0; i < MAX_PID_VARS; i++){
//		PIDVars[i]->debug = false;
//		PIDVars[i]->enabled = true;
//		PIDVars[i]->target = 0;
//		PIDVars[i]->previousError = 0;
//		PIDVars[i]->integral = 777777;
//		PIDVars[i]->output = 0;
//		PIDVars[i]->input = 0;
//		PIDVars[i]->errorThreshold = 0;
//		PIDVars[i]->speedThreshold = 0;
//		PIDVars[i]->Kp = 0;
//		PIDVars[i]->Ki = 0;
//		PIDVars[i]->Kd = 0;
//		PIDVars[i]->integralLimit = 0;
//		PIDVars[i]->integralActiveZone = 0;
//		PIDVars[i]->loopTime = 0;
//		writeDebugStreamLine("%i",i);
//	}
//}

// The heavy lifter for PID tasks
void updatePIDVar(PIDStruct *PIDVar){
	// only run if enabled
	if(PIDVar->enabled){
		float proportional = 0;
		float derivative = 0;
		float error = 0;

		error = PIDVar->target - PIDVar->input;

		proportional = PIDVar->Kp * error; // P
		if(fabs(error) <= PIDVar->integralActiveZone){ // Active zone for I
			PIDVar->integral += PIDVar->Ki * error * PIDVar->loopTime; // I
			if(fabs(PIDVar->integral) >= PIDVar->integralLimit) // limit I
				PIDVar->integral = sgn(PIDVar->integral) * PIDVar->integralLimit;
		}
		else
			PIDVar->integral = 0;
		derivative = PIDVar->Kd * (error - PIDVar->previousError) / PIDVar->loopTime; // D

		PIDVar->output = round(proportional + PIDVar->integral + derivative);
		PIDVar->previousError = error;

		if(PIDVar->debug){
			writeDebugStream("%f\t%f\t%f\t%f\n", error, proportional, PIDVar->integral, derivative);
		}
	}
}

task drivePIDTask(){
	while(true){
		driveLPID.input = nMotorEncoder[driveL];
		//writeDebugStream("%f\t",driveLPID.input);
		driveRPID.input = nMotorEncoder[driveR];
		gyroPID.input = SensorValue[gyro];

		updatePIDVar(&driveLPID);
		updatePIDVar(&driveRPID);
		updatePIDVar(&gyroPID);

		// combine PID outputs and limit each pid output to 127
		// so they have equal influence on the motors
		motor[driveL] = lim127(driveLPID.output) - lim127(gyroPID.output);
		motor[driveR] = lim127(driveLPID.output) + lim127(gyroPID.output);

		wait1Msec(driveLPID.loopTime);
	}
}

//task globalDrivePIDTask(){
//	while(true){
//		globalDrivePID.input = globalXPos;



//		wait1Msec(globalDrivePID.loopTime);
//	}
//}

task armPIDTask(){
	while(true){
		//use left pot for height
		armPID.input = SensorValue[armPotL];
		//armCrossCouplePID.input = SensorValue[armPotL] - SensorValue[armPotR] + ARM_POT_OFFSET;

		updatePIDVar(&armPID);
		//updatePIDVar(&armCrossCouplePID);

		motor[liftTl] = lim127(armPID.output);// + lim127(armCrossCouplePID.output);
		motor[liftBl] = lim127(armPID.output);// + lim127(armCrossCouplePID.output);

		motor[liftTr] = lim127(armPID.output);// - lim127(armCrossCouplePID.output);
		motor[liftBr] = lim127(armPID.output);// - lim127(armCrossCouplePID.output);

		wait1Msec(armPID.loopTime);
	}
}

task usrCtrlArmPID(){
	int setPow = 0;
	int armPotOffset = 0;
	while(true){
		//armPotOffset = ((SensorValue[armPotL] + SensorValue[armPotR]) / 2) /
		armCrossCouplePID.input = SensorValue[armPotL] - (1.1407*SensorValue[armPotR] - 398.251);
		//SensorValue[armPotL] - (SensorValue[armPotR] * 0.93) + armPotOffset;

		// smaller values tilts it right
		// bigger values tilts it left

		updatePIDVar(&armCrossCouplePID);

		// up on 5U
		if(vexRT[Btn5U]){
			setPow = 127;
		}
		// down on 5D
		else if(vexRT[Btn5D]){
			setPow = -127;
		}
		else{
			setPow = 0;
		}
		//setPow = 127;

		motor[liftTl] = setPow + lim127(armCrossCouplePID.output);
		motor[liftBl] = setPow + lim127(armCrossCouplePID.output);

		motor[liftTr] = setPow - lim127(armCrossCouplePID.output);
		motor[liftBr] = setPow - lim127(armCrossCouplePID.output);

		wait1Msec(armCrossCouplePID.loopTime);
	}
}

task mogoPIDTask(){
	while(true){
		mogoPID.input = SensorValue[mogoPot];
		updatePIDVar(&mogoPID);
		motor[mogo] = mogoPID.output;

		wait1Msec(mogoPID.loopTime);
	}
}

task swingPIDTask(){
	while(true){
		swingPID.input = SensorValue[swingPot];
		updatePIDVar(&swingPID);
		motor[swing] = swingPID.output;

		wait1Msec(swingPID.loopTime);
	}
}
