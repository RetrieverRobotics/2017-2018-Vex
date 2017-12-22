//////////////////////////////////////////////////////////////////////////
//
// Functions for Retriever Robotics 2017-2018 In The Zone
//
//////////////////////////////////////////////////////////////////////////

#pragma systemFile // get rid of unused function warnings

//----------------------------------------------------------------------------
// Helper functions
//----------------------------------------------------------------------------

// converts values of right pot scaled to left pot
float scalePotRToL(float rArmPot){
	return 1.1407*SensorValue[armPotR] - 398.251;
	//1.5
}

float getArmHeight(){
	return (SensorValue[armPotL] + scalePotRToL(SensorValue[armPotR])) / 2;
}

float getHeading(){
	return (SensorValue[gyro] / 10) + startingRotationOffset;
}

float degToGyro(float degrees){
	return (degrees - startingRotationOffset) * 10;
}

int slew (int value, int lastValue,  int slewRate) {
	if(abs(value - lastValue) > slewRate) {
		if(value > lastValue)
			return lastValue + slewRate;
		else
			return lastValue - slewRate;
	}
	return value;
}

//rotates x, y coordinates into new system offset by theta
float *rotateCoords(float xCoord, float yCoord, float theta){
	float returnCoords[2];

	returnCoords[X_COORD] = xCoord * cos(theta) + yCoord * sin(theta);
	returnCoords[Y_COORD] = -xCoord * sin(theta) + yCoord * cos(theta);

	return returnCoords;
}

int lim127(int power){
	return(abs(power) > 127 ? sgn(power) * 127 : power);
}

void setLift(int setPow){
	motor[liftTL] = setPow;
	motor[liftBL] = setPow;
	motor[liftR]	= setPow;
}

void smackVcat(){}

void drivePIDsOn(bool on){
	if(on){
		// turn drive PIDS back on set to where we are currently
		drivePID.target = (SensorValue[driveLEnc] + SensorValue[driveREnc]) / 2;

		drivePID.enabled = true;
	}
	else{
		drivePID.enabled = false;
	}
}

//----------------------------------------------------------------------------
// Movement functions
//----------------------------------------------------------------------------

void tankDrive(int lPow, int rPow){
	// account for drive deadbands and set power
	motor[driveBL] = abs(lPow) < DRIVE_DEADBAND ? 0 : lPow;
	motor[driveFL] = abs(lPow) < DRIVE_DEADBAND ? 0 : lPow;

	motor[driveR]  = abs(rPow) < DRIVE_DEADBAND ? 0 : lPow;
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

// drives forward distance inches using drivePID
void driveIncremental(float distance){
	// TODO: check if drive PID started, if not start now and end it after
	// if getTaskPriority(drivePIDTask) > 0:
	// 		then itsProbablyRunning

	//convert inches to drive ticks
	distance = distance * DRIVE_TICKS_PER_INCH;

	//add distance to current position
	drivePID.target = ((SensorValue[driveLEnc] + SensorValue[driveREnc]) / 2) + distance;
}

//unnecessary?
void turnCounterClockwise(float degrees){
	//turn off distance PIDS so they dont resist
	// drivePIDsOn(false);

	gyroPID.target += degToGyro(degrees);

	waitForPID(gyroPID);

	// turn drive PIDS back on
	// drivePIDsOn(true);
}

//unnecessary?
void turnAbsolute(float degrees){
	//turn off distance PIDS so they dont resist
	// drivePIDsOn(false);

	gyroPID.target = degToGyro(degrees);

	waitForPID(gyroPID);

	// turn drive PIDS back on
	// drivePIDsOn(true);
}

// not currently used
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
		leftEnc  = SensorValue[driveLEnc];
		rightEnc = SensorValue[driveREnc];

		deltaLeft  = leftEnc 	- previousLeft;
		deltaRight = rightEnc - previousRight;

		aveDist = (deltaLeft + deltaRight) / 2; // average distance covered

		// integrate position

		globalXPos += aveDist * cosDegrees(rotation);
		globalYPos += aveDist * sinDegrees(rotation);

		previousLeft = leftEnc;
		previousRight = rightEnc;

		wait1Msec(COORDINATE_MONITORING_PERIOD);
	}//END while(true)
}//END coordinateMonitoring

//----------------------------------------------------------------------------
// PID tasks and functions
//----------------------------------------------------------------------------

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
	// if disabled set output to 0
	else{
		PIDVar->output = 0;
	}
	if(abs(PIDVar->output) <= PIDVar->deadband){
		PIDVar->output = 0;
	}
}

// wait until position close to target and velocity close to  0
void waitForPID(PIDStruct PIDVar){
	float error;
	float inputLast = PIDVar.input;

	while(true){
		wait1Msec(PIDVar.loopTime);

		error = PIDVar.target - PIDVar.input;
		writeDebugStream("%f\t", error);
		writeDebugStream("%f\n", (PIDVar.input-inputLast) / PIDVar.loopTime);

		//if pos within desired range of target
		if(fabs(error) < PIDVar.errorThreshold){
			// if the speed is close to 0
			if(fabs((PIDVar.input-inputLast) / PIDVar.loopTime) < PIDVar.speedThreshold){
				//100Msec wait for good measure
				wait1Msec(100);
				return; // exit loop
			}
		}

		inputLast = PIDVar.input;
	}
	return;
}

// operates on the average of right and left for distance.
// uses raw gyro to maintain rotation
task drivePIDTask(){
	float lastGyro = SensorValue[gyro];

	while(true){
    // average of left and right for average distance travelled.
		drivePID.input = (SensorValue[driveLEnc] + SensorValue[driveREnc]) / 2;
		gyroPID.input = SensorValue[gyro];

		// account for rollover
		//if(gyroPID.input - lastGyro > 1800)
		//	gyroPID.input -= 3600;
		//if(gyroPID.input - lastGyro < 1800)
		//	gyroPID.input += 3600;
		//if gyro rolls over, it goes to zero, but going back past zero goes back to 3600 instead of negative

		updatePIDVar(&drivePID);
		updatePIDVar(&gyroPID);

		// combine PID outputs and limit each pid output to 127
		// so they have equal influence on the motors
		tankDrive(
			lim127(drivePID.output) - lim127(gyroPID.output),
			lim127(drivePID.output) + lim127(gyroPID.output)
		);

		lastGyro = SensorValue[gyro];

		wait1Msec(drivePID.loopTime);
	}
}

// void driveAbsolute(float xCoord, float yCoord){
// 	int heading = getHeading();
//
// 	// gyro target = find angle to target
// 	// globalDrivePID.target = 			 * DRIVE_TICKS_PER_INCH;
//
//
// }

// task globalDrivePIDTask(){
// 	while(true){
// 		newCoords = rotateCoords(globalXPos, globalYPos, );
//
// 		globalDrivePID.input = rotated thing;//globalXPos;
//
// 		// rotate current global coordinates until one axis creates a line that contains both the robot and the target point on it.  Then input = current axis value, and target = distance to the target on the new coord system.
//
//
// 		wait1Msec(globalDrivePID.loopTime);
// 	}
// }

// average of right and left arm pot used
// cross coupling to keep heights the same
// set height with armPID.target
task armPIDTask(){
	armCrossCouplePID.target = 0;

	while(true){
		// average the 2 pots for height
		armPID.input = (SensorValue[armPotL] + scalePotRToL(SensorValue[armPotR])) / 2;
		// cross couple input is the difference between arm heights
		armCrossCouplePID.input = SensorValue[armPotL] - scalePotRToL(SensorValue[armPotR]);

		updatePIDVar(&armPID);
		updatePIDVar(&armCrossCouplePID);

		motor[liftTL] = lim127(armPID.output) + lim127(armCrossCouplePID.output);
		motor[liftBL] = lim127(armPID.output) + lim127(armCrossCouplePID.output);

		motor[liftR] 	= lim127(armPID.output) - lim127(armCrossCouplePID.output);

		wait1Msec(armPID.loopTime);
	}
}

// average of right and left arm pot used
// cross coupling to keep heights the same
// takes input from controller to allow for usercontrol
task usrCtrlArmPID(){
	int setPow = 0;
	float inputLast = 0;
	bool bPrevPressed = false;
	bool bSetArmHeight = false;
	armCrossCouplePID.target = 0;
	clearTimer(T2);

	while(true){
		// average the 2 pots for height
		armPID.input = (SensorValue[armPotL] + scalePotRToL(SensorValue[armPotR])) / 2;
		// cross couple input is the difference between arm heights
		armCrossCouplePID.input = SensorValue[armPotL] - scalePotRToL(SensorValue[armPotR]);

		if(vexRT[Btn5U]){
			armPID.enabled = false;
			armCrossCouplePID.enabled = false;
			setPow = 127;
			bPrevPressed = true;
		}
		// down on 5D
		else if(vexRT[Btn5D]){
			armPID.enabled = false;
			armCrossCouplePID.enabled = false;
			setPow = -127;
			bPrevPressed = true;
		}
		else{
			setPow = 0;
			//only turn on PIDS if the arm is up
			if(armPID.input > ARM_BLOCK_MOGO){
				// clear timer once immediately after button release
				if(bPrevPressed){
					clearTimer(T2);
					bSetArmHeight = true; // set up to turn on pid later
				}

				//TODO: make if velocity not close to zero

				// set armPID target to current sensorValue only once after the desired time passes

				// Record arm height after either speed reaches 0 or .5 sec passes, whichever comes first
				if(bSetArmHeight && ((fabs((armPID.input-inputLast) / armPID.loopTime) < .05) || time1[T2] > 500)){
					armPID.target = armPID.input;
					armPID.enabled = true;
					armCrossCouplePID.enabled = true;
					bSetArmHeight = false;
				}
			}

			bPrevPressed = false;
		}

		#ifndef USERCONTROL_ARM_PID_OFF
		updatePIDVar(&armPID);
		updatePIDVar(&armCrossCouplePID);
		#endif

		motor[liftTL] = setPow + lim127(armPID.output) + lim127(armCrossCouplePID.output);
		motor[liftBL] = setPow + lim127(armPID.output) + lim127(armCrossCouplePID.output);

		motor[liftR] 	= setPow + lim127(armPID.output) - lim127(armCrossCouplePID.output);

		inputLast = armPID.input;
		wait1Msec(armPID.loopTime);
	}
}

//yeetsauce
task swingPIDTask(){
	while(true){
		swingPID.input = SensorValue[swingPot];
		updatePIDVar(&swingPID);
		motor[swing] = swingPID.output;

		wait1Msec(swingPID.loopTime);
	}
}

#ifdef sploof
#warning "sploof mode activated"
#endif
