void setLift(int setPow){
	motor[liftTl] = setPow;
	motor[liftTr] = setPow;
	motor[liftBl] = setPow;
	motor[liftBr] = setPow;
}

void initPIDVars(){
	// iterate through each PID variable (drive, lift, etc)
	for(int i; i < MAX_PID_VARS; i++){
		PIDVars[i]->debug = false;
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

task drivePIDTask(){
	while(true){
		driveLPID.input = nMotorEncoder[driveL];
		driveRPID.input = nMotorEncoder[driveR];

		updatePIDVar(&driveLPID);
		updatePIDVar(&driveRPID);

		motor[driveL] = driveLPID.output;
		motor[driveR] = driveRPID.output;

		wait1Msec(driveLPID.loopTime);
	}
}

task armPIDTask(){
	while(true){
		armPID.input = SensorValue[armPot];
		updatePIDVar(&armPID);
		setLift(armPID.output);

		wait1Msec(armPID.loopTime);
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
