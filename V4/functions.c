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
float scalePotRToL(float rArmPot) {
	return .9193 * SensorValue[armPotR] - 30;
	// return .9584 * SensorValue[armPotR] - 18;
}

// average the L&R pots for arm height
float getArmHeight() {
	return (SensorValue[armPotL] + scalePotRToL(SensorValue[armPotR])) / 2;
}

// returns robot heading in degrees
float getHeading() {
	return (SensorValue[gyro] / 10) + startingRotationOffset;
}

float degToGyro(float degrees) {
	return (degrees - startingRotationOffset) * 10;
}

int slew (int value, int lastValue,  int slewRate)  {
	if (abs(value - lastValue) > slewRate)  {
		if(value > lastValue)
			return lastValue + slewRate;
		else
			return lastValue - slewRate;
	}
	return value;
}

//rotates x, y coordinates into new system offset by theta
float *rotateCoords(float xCoord, float yCoord, float theta) {
	float returnCoords[2];

	returnCoords[X_COORD] = xCoord * cos(theta) + yCoord * sin(theta);
	returnCoords[Y_COORD] = -xCoord * sin(theta) + yCoord * cos(theta);

	return returnCoords;
}

int lim127(int power) {
	return (abs(power) > 127 ? sgn(power) * 127 : power);
}

void setLiftL(int setPow) {
	motor[liftL] = setPow;
}

void setLiftR(int setPow) {
	motor[liftR] = setPow;
}

void setLift(int setPow) {
	setLiftL(setPow);
	setLiftR(setPow);
}

void tankDrive(int lPow, int rPow) {
	// account for drive deadbands
	lPow = abs(lPow) < DRIVE_DEADBAND ? 0 : lPow;
	rPow = abs(rPow) < DRIVE_DEADBAND ? 0 : rPow;

	// account for drive deadbands and set power
	motor[driveL1] = lPow;
	motor[driveL2] = lPow;

	motor[driveR1]  = rPow;
	motor[driveR2]  = rPow;
}

int getLDriveEnc(){
	return -SensorValue[driveLEnc];
}

int getRDriveEnc(){
	return SensorValue[driveREnc];
}

void resetDrive() {
	drivePID.target = (getLDriveEnc() + getRDriveEnc()) / 2;
}

void resetGyro() {
	gyroPID.target = SensorValue[gyro];
}

void smackVcat() {}

//----------------------------------------------------------------------------
// Movement functions
//----------------------------------------------------------------------------

void intakeMogo() {
	motor[mogo] = 127; // positive for in
	wait1Msec(MOGO_INTAKE_TIME);
	motor[mogo] = 0;
}

void extendMogo() {
	motor[mogo] = -127; // negative for out
	wait1Msec(MOGO_EXTEND_TIME);
	motor[mogo] = -50;
	wait1Msec(MOGO_SECONDARY_WAIT_TIME);
	motor[mogo] = 0;
}

// void stackCone(int currStackHeight) {
//
// }

void placeStack(){

}

void setArmHeight(int height){
	armMode = ARM_PID_CONTROL;
	armPID.target = height;
}

void tardLift(int setPow = 0) {
	armMode = ARM_TARD;
	setLift(setPow);
}

void tardLiftStraight(int setPow = 0){
	armMode = ARM_TARD_ACTIVE_CENTERING;
	armPID.target = setPow;
}

void tardDrive(int left, int right) {
	driveMode = TARD;
	tankDrive(left, right);
}
// 1 arg will assume left = right
void tardDrive(int setPow = 0) {
	tardDrive(setPow, setPow);
}

// tard drive with active rotation correction
void tardDriveStraight(int setPow = 0) {
	driveMode = TARD_ACTIVE_ROT;
	drivePID.target = setPow;
}

// drives forward distance inches using drivePID, active gyro
// based on previous drivePID.target
void driveIncremental(float distance, bool incremental = true) {
	driveMode = POINT_TURN;
	//convert inches to drive ticks
	distance = distance * DRIVE_TPI;
	if (incremental) {
		resetDrive();
	}
	//add distance to current position
	drivePID.target += distance;
}

// absolute counterclockwise swing turn using left side
void swingTurnLeft(float degrees, bool incremental = false) {
	driveMode = SWING_LEFT;

	drivePID.target = getRDriveEnc();
	if (incremental) {
		resetGyro();
		gyroPID.target += degToGyro(degrees);
	}
	else {
		gyroPID.target = degToGyro(degrees);
	}
}

// absolute counterclockwise swing turn using right side
void swingTurnRight(float degrees, bool incremental = false) {
	driveMode = SWING_RIGHT;

	drivePID.target = getLDriveEnc();
	if (incremental) {
		resetGyro();
		gyroPID.target += degToGyro(degrees);
	}
	else {
		gyroPID.target = degToGyro(degrees);
	}
}

// absolute counterclockwise point turn using gyro position
void pointTurn(float degrees, bool incremental = false) {
	driveMode = POINT_TURN;

	if (incremental) {
		resetGyro();
		gyroPID.target += degToGyro(degrees);
	}
	else {
		gyroPID.target = degToGyro(degrees);
	}
}

// tank drive with slew and deadbands to help with overheating
task driveSlew() {
	int leftDrive = 0;
	int rightDrive = 0;
	while(true) {
		// writeDebugStreamLine("%i", leftDrive);
		leftDrive 	= slew(vexRT[Ch3], 	leftDrive,  DRIVE_SLEW_RATE);
		rightDrive 	= slew(vexRT[Ch2], rightDrive, 	DRIVE_SLEW_RATE);
		tankDrive(leftDrive, rightDrive);
		wait1Msec(DRIVE_SLEW_TIME);
	}
}

// displays T1 time within 10 ms
task displayTime() {
	bLCDBacklight = true;
	string displayString;
	displayLCDCenteredString(0, "Run Time:");

	while(true) {
		sprintf(displayString, "%1.2fs", time1[T1] / 1000.);
		displayLCDCenteredString(1, displayString);
		wait1Msec(10);
	}
}

//----------------------------------------------------------------------------
// PID tasks and functions
//----------------------------------------------------------------------------

// The heavy lifter for PID tasks
void updatePIDVar(PIDStruct *PIDVar) {
	// only run if enabled
	if(PIDVar->enabled) {
		float proportional = 0;
		float derivative = 0;
		float error = 0;

		error = PIDVar->target - PIDVar->input;

		proportional = PIDVar->kP * error; // P
		// Check active zone for I
		if (fabs(error) <= PIDVar->integralActiveZone) {
			PIDVar->integral += PIDVar->kI * error * PIDVar->loopTime; // I
			if(fabs(PIDVar->integral) >= PIDVar->integralLimit) // limit I
				PIDVar->integral = sgn(PIDVar->integral) * PIDVar->integralLimit;
		}
		else {
			PIDVar->integral = 0;
		}
		derivative = PIDVar->kD * (error - PIDVar->previousError) / PIDVar->loopTime; // D

		PIDVar->output = round(proportional + PIDVar->integral + derivative);
		// deadband
		PIDVar->output = abs(PIDVar->output) < PIDVar->deadband ? 0 : PIDVar->output;

		PIDVar->previousError = error;

		if(PIDVar->debug) {
			writeDebugStream("%f\t%f\t%f\t%f\n", error, proportional, PIDVar->integral, derivative);
		}
	}
	// if disabled set output to 0
	else {
		PIDVar->output = 0;
	}
}

// wait until position close to target and velocity close to  0
// checkspeed default true
void waitForPID(PIDStruct PIDVar, bool checkSpeed) {
	float error;
	float inputLast = PIDVar.input;

	while (true) {
		wait1Msec(PIDVar.loopTime);

		error = PIDVar.target - PIDVar.input;

		#ifdef DEBUG_PID_WAIT_FUNC
		writeDebugStream("%f\t", error);
		writeDebugStream("%f\n", (PIDVar.input-inputLast) / PIDVar.loopTime);
		#endif

		//if position within desired range of target
		if (fabs(error) < PIDVar.errorThreshold) {
			// if the speed is close to 0
			#ifdef WAIT_FOR_PID_CHECK_SPEED
			if (checkSpeed) {
				if (fabs((PIDVar.input-inputLast) / PIDVar.loopTime) < PIDVar.speedThreshold) {
					//100Msec wait for good measure
					// wait1Msec(100);
					break; // exit loop
				}
			}
			else {
				break;
			}
			#else
			break;
			#endif
		}

		inputLast = PIDVar.input;
	}
	return;
}

// operates on the average of right and left for distance.
// uses raw gyro to maintain rotation
task drivePIDTask() {
	float lastGyro = SensorValue[gyro];
	static int offset; // for gyro rollover
	int gyroDelta;
	updatePIDVar(&drivePID);
	updatePIDVar(&gyroPID);

	while(true) {
		// decide which encoders to use for input based on turning mode
		if(driveMode == SWING_LEFT)
			drivePID.input = getRDriveEnc();
		else if(driveMode == SWING_RIGHT)
			drivePID.input = getLDriveEnc();
		else if(driveMode == POINT_TURN)
			drivePID.input = (getLDriveEnc() + getRDriveEnc()) / 2;
		// no input for tard modes

		gyroPID.input = SensorValue[gyro] + offset;

		// account for gyro rollover
		gyroDelta = gyroPID.input - lastGyro;
		if(gyroDelta > 1800) {
			gyroPID.input -= offset; // get rid of old offset
			offset -= 3600;
			gyroPID.input += offset; // add back new offset
		}
		else if(gyroDelta < -1800) {
			gyroPID.input -= offset; // get rid of old offset
			offset += 3600;
			gyroPID.input += offset; // add back new offset
		}

		updatePIDVar(&drivePID);
		updatePIDVar(&gyroPID);
		// gyroPID.output = 0;

		if(driveMode == POINT_TURN) {
			// combine PID outputs and lim127 so the gyro has more influence
			tankDrive(
				lim127(lim127(drivePID.output) - gyroPID.output),
				lim127(lim127(drivePID.output) + gyroPID.output)
			);
		}
		else if (driveMode == SWING_LEFT) {
			tankDrive(
				-lim127(gyroPID.output),
				lim127(drivePID.output)
			);
		}
		else if(driveMode == SWING_RIGHT) {
			tankDrive(
				lim127(drivePID.output),
				lim127(gyroPID.output)
			);
		}
		else if(driveMode == TARD_ACTIVE_ROT) {
			// math inside the lim127 so it can correctly compensate
			tankDrive(
				lim127(drivePID.target - gyroPID.output),
				lim127(drivePID.target + gyroPID.output)
			);
		}
		// dont do anything on tard mode


		lastGyro = SensorValue[gyro];
		wait1Msec(drivePID.loopTime);
	}
}

// average of right and left arm pot used
// cross coupling to keep heights the same
// set height with armPID.target
task armPIDTask() {
	armCrossCouplePID.target = 0;
	armPID.input = getArmHeight();
	armCrossCouplePID.input = SensorValue[armPotL] - scalePotRToL(SensorValue[armPotR]);
	updatePIDVar(&armPID);
	updatePIDVar(&armCrossCouplePID);

	while(true) {
		// average the 2 pots for height
		armPID.input = getArmHeight();
		// cross couple input is the difference between arm heights
		armCrossCouplePID.input = SensorValue[armPotL] - scalePotRToL(SensorValue[armPotR]);

		updatePIDVar(&armPID);
		#ifdef ARM_CENTERING_OFF
		armCrossCouplePID.output = 0;
		#else
		updatePIDVar(&armCrossCouplePID);
		#endif

		if (armMode == ARM_PID_CONTROL) {
			// arm cross couple can compensate no more than 127
			setLiftL( lim127(armPID.output) + lim127(armCrossCouplePID.output) );
			setLiftR( lim127(armPID.output) - lim127(armCrossCouplePID.output) );
		}
		else if (armMode == ARM_TARD_ACTIVE_CENTERING){
			// arm cross couple can compensate no more than 127
			setLiftL( lim127(armPID.target) + lim127(armCrossCouplePID.output) );
			setLiftR( lim127(armPID.target) - lim127(armCrossCouplePID.output) );
		}

		wait1Msec(armPID.loopTime);
	}
}

// yeetsauce
task swingPIDTask() {
	swingPID.input = SensorValue[swingPot];
	updatePIDVar(&swingPID);

	while (true) {
		if (swingPID.enabled) {
			swingPID.input = SensorValue[swingPot];
			updatePIDVar(&swingPID);
			motor[swing] = -swingPID.output;
		}

		wait1Msec(swingPID.loopTime);
	}
}

#ifdef sploof
#warning "sploof mode activated"
#endif
