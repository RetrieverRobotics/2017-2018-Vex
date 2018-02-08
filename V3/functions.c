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
	return 1.15*SensorValue[armPotR] - 409;
}

// average the L&R pots for arm height
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

void setLiftL(int setPow){
	motor[liftL] = setPow;
}

void setLiftR(int setPow){
	motor[liftR] = setPow;
}

void setLift(int setPow){
	setLiftL(setPow);
	setLiftR(setPow);
}

void tankDrive(int lPow, int rPow){
	// account for drive deadbands
	lPow = abs(lPow) < DRIVE_DEADBAND ? 0 : lPow;
	rPow = abs(rPow) < DRIVE_DEADBAND ? 0 : rPow;

	// account for drive deadbands and set power
	motor[driveL1] = lPow;
	motor[driveL2] = lPow;

	motor[driveR1]  = rPow;
	motor[driveR2]  = rPow;
}

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

void smackVcat(){}

//----------------------------------------------------------------------------
// Movement functions
//----------------------------------------------------------------------------

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

void stackCone(int currStackHeight){

}

void tardLift(int setPow, int waitTime){
	stopTask(armPIDTask);
	setLift(setPow);
	wait1Msec(waitTime);
	startTask(armPIDTask);
}

void tardDrive(int left, int right, int waitTime){
	driveMode = TARD;
	drivePIDsOn(false);
	tankDrive(left,right);
	wait1Msec(waitTime);
	tankDrive(0,0);
	drivePIDsOn(true);
}

void tardDriveActiveRot(int left, int right, int waitTime){
	driveMode = TARD_ACTIVE_ROT;
	drivePIDsOn(false);
	tankDrive(left,right);
	wait1Msec(waitTime);
	tankDrive(0,0);
	drivePIDsOn(true);
}

// drives forward distance inches using drivePID
void driveIncremental(float distance){
	driveMode = POINT_TURN;
	// TODO: check if drive PID started, if not start now and end it after
	// if getTaskPriority(drivePIDTask) > 0:
	// 		then itsProbablyRunning

	//convert inches to drive ticks
	distance = distance * DRIVE_TPI;

	//add distance to current position
	drivePID.target = ((SensorValue[driveLEnc] + SensorValue[driveREnc]) / 2) + distance;
}

// counterclockwise swing turn using left side
void swingTurnLeft(float degrees){
	driveMode = SWING_LEFT;
	drivePID.target = SensorValue[driveREnc];
	gyroPID.target 	= degToGyro(degrees);
}

// counterclockwise swing turn using right side
void swingTurnRight(float degrees){
	driveMode = SWING_RIGHT;
	drivePID.target = SensorValue[driveLEnc];
	gyroPID.target 	= degToGyro(degrees);
}

// counterclockwise point turn using absolute gyro position
void pointTurn(float degrees){
	driveMode = POINT_TURN;
	gyroPID.target = degToGyro(degrees);
}

// counterclockwise incremental turn
void turnCounterClockwise(float degrees){
	driveMode = POINT_TURN;
	gyroPID.target = SensorValue[gyro] + degToGyro(degrees);
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

// tank drive with slew and deadbands to help with overheating
task driveSlew(){
	int leftDrive;
	int rightDrive;
	while(true){
		leftDrive 	= slew(vexRT[Ch3], 	leftDrive,  1);
		rightDrive 	= slew(vexRT[Ch2], rightDrive, 	1);
		tankDrive(leftDrive, rightDrive);
		wait1Msec(DRIVE_SLEW_TIME);
	}
}

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
		// deadband
		PIDVar->output = abs(PIDVar->output) < PIDVar->deadband ? 0 : PIDVar->output;

		PIDVar->previousError = error;

		if(PIDVar->debug){
			writeDebugStream("%f\t%f\t%f\t%f\n", error, proportional, PIDVar->integral, derivative);
		}
	}
	// if disabled set output to 0
	else{
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
	static int offset = 0;
	int gyroDelta;

	while(true){
		// decide which encoders to use for input based on turning mode
		if(driveMode == SWING_LEFT)
			drivePID.input = SensorValue[driveREnc];
		else if(driveMode == SWING_RIGHT)
			drivePID.input = SensorValue[driveLEnc];
		else if(driveMode == POINT_TURN)
			drivePID.input = (SensorValue[driveLEnc] + SensorValue[driveREnc]) / 2;
		// no input for tard modes

		gyroPID.input = SensorValue[gyro] + offset;

		// account for gyro rollover
		gyroDelta = gyroPID.input - lastGyro;
		if(gyroDelta > 1800){
			gyroPID.input -= offset; // get rid of old offset
			offset -= 3600;
			gyroPID.input += offset; // add back new offset
		}
		else if(gyroDelta < -1800){
			gyroPID.input -= offset; // get rid of old offset
			offset += 3600;
			gyroPID.input += offset; // add back new offset
		}

		updatePIDVar(&drivePID);
		updatePIDVar(&gyroPID);

		if(driveMode == SWING_LEFT)
			tankDrive( -lim127(gyroPID.output), lim127(drivePID.output) );
		else if(driveMode == SWING_RIGHT)
			tankDrive( lim127(drivePID.output), lim127(gyroPID.output) );
		else if(driveMode == POINT_TURN){
			// combine PID outputs and limit each pid output to 127 so they have equal influence on the motors
			tankDrive(
				lim127(drivePID.output) - lim127(gyroPID.output),
				lim127(drivePID.output) + lim127(gyroPID.output)
			);
		}
		else if(driveMode == TARD_ACTIVE_ROT)
			tankDrive( -lim127(gyroPID.output), lim127(gyroPID.output) );
		// dont do anything on tard mode


		lastGyro = SensorValue[gyro];
		wait1Msec(drivePID.loopTime);
	}
}

// void driveAbsolute(float xCoord, float yCoord){
// 	int heading = getHeading();
//
// 	// gyro target = find angle to target
// 	// globalDrivePID.target = 			 * DRIVE_TPI;
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

		setLiftL( lim127(armPID.output) + lim127(armCrossCouplePID.output) );

		setLiftR( lim127(armPID.output) - lim127(armCrossCouplePID.output) );

		wait1Msec(armPID.loopTime);
	}
}

// average of right and left arm pot used
// cross coupling to keep heights the same
// takes input from controller to allow for usercontrol
task usrCtrlArmPID1(){
	int setPow = 0;
	float inputLast = 0;
	bool bPrevPressed = false;
	bool bSetArmHeight = false;
	float deltaHeight, lastDeltaHeight;
	armCrossCouplePID.target = 0;
	clearTimer(T2);

	while(true){
		armPID.input = getArmHeight();
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
					lastDeltaHeight = armPID.input-inputLast;
				}

				deltaHeight = armPID.input-inputLast;

				// look for local extrema of the height function and record position there
				// timeout after 500 ms
				if( bSetArmHeight && (sgn(deltaHeight)!=sgn(lastDeltaHeight) || deltaHeight==0 || time1[T2]>500) ){
					armPID.target = armPID.input;
					armPID.enabled = true;
					armCrossCouplePID.enabled = true;
					bSetArmHeight = false;
				}
				lastDeltaHeight = deltaHeight;
			}

			bPrevPressed = false;
		}

		#ifndef USERCONTROL_ARM_PID_OFF
		updatePIDVar(&armPID);
		updatePIDVar(&armCrossCouplePID);
		#endif

		setLiftL( setPow + lim127(armPID.output) + lim127(armCrossCouplePID.output) );

		setLiftR( setPow + lim127(armPID.output) - lim127(armCrossCouplePID.output) );

		inputLast = armPID.input;
		wait1Msec(armPID.loopTime);
	}
}

task usrCtrlArmPID2(){
	int setPow = 0;
	float inputLast = 0;
	bool bPrevPressed = false;
	bool bSetArmHeight = false;
	float deltaHeight, lastDeltaHeight;
	armCrossCouplePID.target = 0;
	clearTimer(T2);

	while(true){
		armPID.input = getArmHeight();
		// cross couple input is the difference between arm heights
		armCrossCouplePID.input = SensorValue[armPotL] - scalePotRToL(SensorValue[armPotR]);

		setPow = vexRT[Ch2Xmtr2]; // right joystick y axis on slave controller
		// hold position inside deabands
		if(setPow < ARM_DEADBAND){
			setPow = 0;
			//only turn on PIDS if the arm is up
			if(armPID.input > ARM_BLOCK_MOGO){
				// clear timer once immediately after button release
				if(bPrevPressed){
					clearTimer(T2);
					bSetArmHeight = true; // set up to turn on pid later
					lastDeltaHeight = armPID.input-inputLast;
				}

				deltaHeight = armPID.input-inputLast;

				// look for local extrema of the height function and record position there
				// timeout after 500 ms
				if( bSetArmHeight && (sgn(deltaHeight)!=sgn(lastDeltaHeight) || deltaHeight==0 || time1[T2]>500) ){
					armPID.target = armPID.input;
					armPID.enabled = true;
					armCrossCouplePID.enabled = true;
					bSetArmHeight = false;
				}
				lastDeltaHeight = deltaHeight;
			}

			bPrevPressed = false;
		}
		else{
			armPID.enabled = false;
			armCrossCouplePID.enabled = false;
			bPrevPressed = true;
		}

		#ifndef USERCONTROL_ARM_PID_OFF
		updatePIDVar(&armPID);
		updatePIDVar(&armCrossCouplePID);
		#endif

		setLiftL( setPow + lim127(armPID.output) + lim127(armCrossCouplePID.output) );

		setLiftR( setPow + lim127(armPID.output) - lim127(armCrossCouplePID.output) );

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
