//////////////////////////////////////////////////////////////////////////
//
// Functions for Retriever Robotics 2017-2018 In The Zone
//
//////////////////////////////////////////////////////////////////////////

void driveIncremental(float distance){
	// TODO: check if drive PID started, if not start now and end it after
	// if getTaskPriority(drivePIDTask) > 0:
	// 		then itsProbablyRunning

	driveLPID.target = nMotorEncoder[driveL] + distance;
	driveRPID.target = nMotorEncoder[driveR] + distance;

	waitUntilPIDAtTarget(driveLPID);
	waitUntilPIDAtTarget(driveRPID);

}

void turnClockwise(float degrees){
	gyroPID.target += degrees;
}

void swingTurnLeft(float degrees){
	//turn off gyro PID
	gyroPID.enabled = false;

	//driveRPID.target = nMotorEncoder[driveR] + encoderDistance;

	//turn gyro PID back on at the new position
	gyroPID.target = SensorValue[gyro];
	gyroPID.enabled = true;
}

void driveAbsolute(float xCoord, float yCoord){
	int heading = GET_HEADING();

}

void turnAbsolute(float degrees){
	gyroPID.target = degrees;
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
		rotation = GET_HEADING();
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

void waitUntilPIDAtTarget(PIDStruct PIDVar){
	//wait until position at target and velocity = 0
}

void initAllPIDsToZero(){
	// iterate through each PID variable (drive, lift, etc)
	for(int i; i < MAX_PID_VARS; i++){
		PIDVars[i]->debug = false;
		PIDVars[i]->enabled = true;
		PIDVars[i]->target = 0;
		PIDVars[i]->previousError = 0;
		PIDVars[i]->integral = 0;
		PIDVars[i]->output = 0;
		PIDVars[i]->input = 0;
		PIDVars[i]->Kp = 0;
		PIDVars[i]->Ki = 0;
		PIDVars[i]->Kd = 0;
		PIDVars[i]->integralLimit = 0;
		PIDVars[i]->integralActiveZone = 0;
		PIDVars[i]->loopTime = 0;
	}
}

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
			writeDebugStream("%f\t%f\t%f\n", proportional, PIDVar->integral, derivative);
		}
	}
}

task drivePIDTask(){
	while(true){
		driveLPID.input = nMotorEncoder[driveL];
		driveRPID.input = nMotorEncoder[driveR];
		gyroPID.input = GET_HEADING();//SensorValue[gyro];

		updatePIDVar(&driveLPID);
		updatePIDVar(&driveRPID);
		updatePIDVar(&gyroPID);

		// combine PID outputs and limit each pid output to 127
		// so they have equal influence on the motors
		motor[driveL] = lim127(driveLPID.output) - lim127(gyroPID.output);
		motor[driveR] = lim127(driveRPID.output) + lim127(gyroPID.output);

		wait1Msec(driveLPID.loopTime);
	}
}

task armPIDTask(){
	while(true){
		//current arm height is average of the 2 pots
		armPID.input = (SensorValue[armPotL] + SensorValue[armPotR]) / 2;
		armCrossCouplePID.input = SensorValue[armPotL] - SensorValue[armPotR] + ARM_POT_OFFSET;

		updatePIDVar(&armPID);
		updatePIDVar(&armCrossCouplePID);

		motor[liftTl] = lim127(armPID.output) + lim127(armCrossCouplePID.output);
		motor[liftBl] = lim127(armPID.output) + lim127(armCrossCouplePID.output);

		motor[liftTr] = lim127(armPID.output) - lim127(armCrossCouplePID.output);
		motor[liftBr] = lim127(armPID.output) - lim127(armCrossCouplePID.output);

		wait1Msec(armPID.loopTime);
	}
}

task usrCtrlArmPID(){
	int setPow = 0;
	while(true){
		armCrossCouplePID.input = SensorValue[armPotL] - SensorValue[armPotR] + ARM_POT_OFFSET;

		updatePIDVar(&armCrossCouplePID);

		// up on 5U
		if(vexRT[Btn5U]){
			setPow = 127;
		}
		// down on 5D
		else if(vexRT[Btn5D]){
			setPow = -127;
		}

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
