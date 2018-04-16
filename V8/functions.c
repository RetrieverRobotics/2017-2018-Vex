//////////////////////////////////////////////////////////////////////////
//
// Functions for Retriever Robotics 2017-2018 In The Zone
//
//////////////////////////////////////////////////////////////////////////
#pragma systemFile // get rid of unused function warnings

//----------------------------------------------------------------------------
// Helper functions
//----------------------------------------------------------------------------

// average the L&R pots for lift height
float getLiftHeight() {
	//nmotorEncoder[liftL]
	return SensorValue[liftPot];
	//return (SensorValue[liftPotL] + scalePotRToL(SensorValue[liftPotR])) / 2;
}

// returns robot heading in degrees
float getHeading() {
	return ((float)(SensorValue[gyro] + gyroOffset) / 10);
}

float degToGyro(float degrees) {
	return degrees * 10;// - gyroOffset;
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

int lim127(int power, int limit = 127) {
	return (abs(power) > limit ? sgn(power) * limit : power);
}

void setLift(int setPow) {
	motor[lift] = setPow;
}

int deadband(int pow1, int limit) {
	return abs(pow1) < limit ? 0 : pow1;
}

void tankDrive(int lPow, int rPow, bool straight = true) {
	// account for drive deadbands
	lPow = abs(lPow) < DRIVE_DEADBAND ? 0 : lPow;
	rPow = abs(rPow) < DRIVE_DEADBAND ? 0 : rPow;

	if(straight){
		lPow = round(lPow * DRIVE_LEFT_MULTIPLIER);//(float)
	}

	// for demoing
	// rPow  = rPow / 2;
	// lPow  = lPow /2;

	// account for drive deadbands and set power
	motor[driveRF]  = rPow;
	motor[driveRBT] = rPow;
	motor[driveRBB] = rPow;

	motor[driveLF] 	= lPow;
	motor[driveLBT] = lPow;
	motor[driveLBB] = lPow;
}

int getLDriveEnc(){
	return nMotorEncoder[driveLBB];
}

int getRDriveEnc(){
	return -nMotorEncoder[driveRBB];
}

void resetDrive() {
	drivePID.target = (getLDriveEnc() + getRDriveEnc()) / 2;
}

void resetGyro() {
	gyroPID.target = SensorValue[gyro];
}

//change gyroOffset such that getHeading() will return angle
void setGyro(float angle) {
	gyroOffset = 10*angle - SensorValue[gyro];
}

void smackVcat() {}

//----------------------------------------------------------------------------
// Movement functions
//----------------------------------------------------------------------------

void mogoGadget() {
	SensorValue[mogoGrabber] = MOGO_PISTONS_IN;
}

void nogoGadget() {
	SensorValue[mogoGrabber] = MOGO_PISTONS_OUT;
}

void intakeMogo() {
	motor[mogo] = 127; // positive for in
	mogoGadget();
	wait1Msec(MOGO_INTAKE_TIME);
	motor[mogo] = 0;
}

void extendMogo(bool yeet = true) {
	motor[mogo] = -127; // negative for out
	if(yeet){
		nogoGadget();
	}
	wait1Msec(MOGO_EXTEND_TIME);
	motor[mogo] = -50;
	wait1Msec(MOGO_SECONDARY_WAIT_TIME);
	motor[mogo] = 0;
}

void swingIn(){
	motor[swing] = 127;
	wait1Msec(SWING_IN_TIME);
	motor[swing] = SWING_HOLD_IN_POW;
}

void swingOut(){
	motor[swing] = -127;
	wait1Msec(SWING_OUT_TIME);
	motor[swing] = -SWING_HOLD_OUT_POW;
}

void setLiftHeight(int height){
	liftMode = LIFT_PID_CONTROL;
	liftPID.target = height;
}

void tardLift(int setPow = 0) {
	liftMode = LIFT_TARD;
	setLift(setPow);
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
void driveIncremental(float distance) {
	driveMode = POINT_TURN;
	//convert inches to drive ticks
	distance = distance * DRIVE_TPI;

	//add distance to current position
	resetDrive();
	drivePID.target += distance;
}

void driveSlow(float distance, float maxPow){
	driveMode = SLOW_DRIVE;
	slowDrivePow = maxPow;
	//convert inches to drive ticks
	distance = distance * DRIVE_TPI;

	//add distance to current position
	resetDrive();
	drivePID.target += distance;
}

// absolute counterclockwise swing turn using left side
void swingTurnLeft(float degrees) {
	driveMode = SWING_LEFT;

	drivePID.target = getRDriveEnc();
	gyroPID.target = degToGyro(degrees);
}

// Inc counterclockwise swing turn using left side
void swingTurnLeftInc(float degrees) {
	driveMode = SWING_LEFT;

	drivePID.target = getRDriveEnc();
	gyroPID.target += degToGyro(degrees);
}

// absolute counterclockwise swing turn using right side
void swingTurnRight(float degrees) {
	driveMode = SWING_RIGHT;

	drivePID.target = getLDriveEnc();
	gyroPID.target = degToGyro(degrees);
}

// Inc counterclockwise swing turn using right side
void swingTurnRightInc(float degrees) {
	driveMode = SWING_RIGHT;

	drivePID.target = getLDriveEnc();
	gyroPID.target += degToGyro(degrees);
}

// absolute counterclockwise point turn using gyro position
void pointTurn(float degrees) {
	driveMode = POINT_TURN;

	resetDrive();
	gyroPID.target = degToGyro(degrees);
}

void pointTurnInc(float degrees) {
	driveMode = POINT_TURN;

	resetDrive();
	// resetGyro();
	gyroPID.target += degToGyro(degrees);
}

int polyDrive(float input, float power) {
	return sgn(input) * pow(fabs(input),power) / pow(127,power-1);
}

// tank drive with slew and deadbands to help with overheating
task driveSlew() {
	int leftDrive = 0;
	int rightDrive = 0;

	#ifdef DRIVE_ALT_STICKS
	int yIn = 0;
	int rotIn = 0;
	#endif

	while(true) {
		#ifdef POLY_DRIVE
		leftDrive = slew(
			polyDrive(vexRT[Ch3], POLY_DRIVE_DEGREE)
			, leftDrive
			, DRIVE_SLEW_RATE
		);
		rightDrive = slew(
			polyDrive(vexRT[Ch2], POLY_DRIVE_DEGREE)
			, rightDrive
			, DRIVE_SLEW_RATE
		);

		#else

		#ifdef DRIVE_ALT_STICKS

		yIn = deadband(vexRT[Ch3], JOYSTICKS_DEADBAND);
		rotIn = deadband(vexRT[Ch1], JOYSTICKS_DEADBAND);

		leftDrive = slew(
			round((yIn + rotIn) * 100. / 127.)
			, leftDrive
			, DRIVE_SLEW_RATE
		);

		rightDrive = slew(
			round((yIn - rotIn) * 100. / 127.)
			, rightDrive
			,	DRIVE_SLEW_RATE
		);

		#else
		leftDrive 	= slew(round(vexRT[Ch3]*100./127.), 	leftDrive,  DRIVE_SLEW_RATE);
		rightDrive 	= slew(round(vexRT[Ch2]*100./127.),  rightDrive, 	DRIVE_SLEW_RATE);
		#endif

		#endif

		tankDrive(leftDrive, rightDrive);
		wait1Msec(DRIVE_SLEW_TIME);
	}
}

// displays T1 time within 10 ms
task displayTime() {
	clearTimer(T1);

	bLCDBacklight = true;
	string displayString;
	displayLCDCenteredString(0, "Run Time:");

	while(true) {
		sprintf(displayString, "%1.2fs", time1[T1] / 1000.);
		displayLCDCenteredString(1, displayString);
		writeDebugStreamLine(displayString);
		wait1Msec(50);
	}
}

task stackCone() {
	swingIn();
	tardLift(-127);
	wait1Msec(300);
  motor[rollers] = ROLLERS_OUT;
  tardLift(127);
  wait1Msec(150);
  tardLift(0);
	swingOut();
	motor[rollers] = ROLLERS_IN;
}

//returns true if timeout happens
bool waitForPID(PIDStruct &PIDVar, int timeout) {
	int startTime = nSysTime;

	while (true) {
		wait1Msec(PIDVar.loopTime);
		if(fabs(PIDVar.error) < PIDVar.errorThreshold){
			return false;
		}
		if(nSysTime - startTime > timeout){
			return true;
		}
	}
	return false;
}

void waitForPID(PIDStruct &PIDVar){
	while (true) {
		wait1Msec(PIDVar.loopTime);
		if(fabs(PIDVar.error) < PIDVar.errorThreshold){
			return;
		}
	}
	return;
}

void waitForSensor(int sensor, int target, int threshold, int timeout){
	int startTime = nSysTime;

	while(true) {
		wait1Msec(SENSOR_POLL_TIME);
		if(abs(target - SensorValue[sensor]) < threshold){
			return;
		}
		if(nSysTime - startTime > timeout){
			return;
		}
	}
	return;
}

void waitForSensor(int sensor, int target, int threshold){
	while(true) {
		wait1Msec(SENSOR_POLL_TIME);
		if(abs(target - SensorValue[sensor]) < threshold){
			return;
		}
	}
	return;
}

//----------------------------------------------------------------------------
// PID tasks and functions
//----------------------------------------------------------------------------

// The heavy lifter for PID tasks
void updatePIDVar(PIDStruct &PIDVar) {
	// only run if enabled
	if(PIDVar.enabled) {
		float proportional = 0;
		float derivative = 0;

		PIDVar.error = PIDVar.target - PIDVar.input;

		// Proportional
		proportional = PIDVar.kP * PIDVar.error;
		// Integral
		if (fabs(PIDVar.error) <= PIDVar.integralActiveZone) {
			PIDVar.integral += PIDVar.kI * PIDVar.error * PIDVar.loopTime;
			// limit I
			if(fabs(PIDVar.integral) >= PIDVar.integralLimit)
				PIDVar.integral = sgn(PIDVar.integral) * PIDVar.integralLimit;
		}
		else {
			PIDVar.integral = 0;
		}
		// Derivative
		derivative = PIDVar.kD * (PIDVar.inputLast - PIDVar.input) / PIDVar.loopTime; // D

		PIDVar.output = round(proportional + PIDVar.integral + derivative);
		// deadband
		PIDVar.output = abs(PIDVar.output) < PIDVar.deadband ? 0 : PIDVar.output;

		PIDVar.previousError = PIDVar.error;
		PIDVar.inputLast = PIDVar.input;

		if(PIDVar.debug) {
			writeDebugStream("%f\t%f\t%f\t%f\n", PIDVar.error, proportional, PIDVar.integral, derivative);
		}
	}
	// if disabled set output to 0
	else {
		PIDVar.output = 0;
	}
}

// operates on the average of right and left for distance.
// uses raw gyro to maintain rotation
task drivePIDTask() {
	float lastGyro = SensorValue[gyro];
	// int gyroOffset = 0; // for gyro rollover
	int gyroDelta;
	int driveOut = 0;
	updatePIDVar(drivePID);
	updatePIDVar(gyroPID);

	while(true) {
		// decide which encoders to use for input based on turning mode
		if(driveMode == SWING_LEFT)
			drivePID.input = getRDriveEnc();
		else if(driveMode == SWING_RIGHT)
			drivePID.input = getLDriveEnc();
		else if(driveMode == POINT_TURN)
			drivePID.input = (getLDriveEnc() + getRDriveEnc()) / 2;
		// no input for tard modes

		gyroPID.input = SensorValue[gyro] + gyroOffset;

		// account for gyro rollover
		gyroDelta = SensorValue[gyro] - lastGyro;
		if(gyroDelta > 1800) {
			// writeDebugStreamLine("yeet");
			gyroPID.input -= gyroOffset; // get rid of old gyroOffset
			gyroOffset -= 3600;
			gyroPID.input += gyroOffset; // add back new gyroOffset
		}
		else if(gyroDelta < -1800) {
			// writeDebugStreamLine("Here78");
			gyroPID.input -= gyroOffset; // get rid of old gyroOffset
			gyroOffset += 3600;
			gyroPID.input += gyroOffset; // add back new gyroOffset
		}

		// writeDebugStreamLine("%i", gyroPID.input);
		// writeDebugStreamLine("%i", gyroPID.error);
		// writeDebugStream("R:%i\tL:%i\t", getRDriveEnc(), getLDriveEnc());

		updatePIDVar(drivePID);
		updatePIDVar(gyroPID);
		// gyroPID.output = 0;
		driveOut = lim127(drivePID.output, 90);

		if(driveMode == POINT_TURN) {
			// combine PID outputs and lim127 so the gyro has more influence
			tankDrive(
				lim127(driveOut - lim127((gyroPID.output + 1 * gyroPID.output*abs(driveOut)/90),90),90),
				lim127(driveOut + lim127((gyroPID.output + 1 * gyroPID.output*abs(driveOut)/90),90),90)
			);
		}
		else if (driveMode == SLOW_DRIVE){
			tankDrive(
				lim127(lim127(drivePID.output, slowDrivePow) - gyroPID.output, slowDrivePow),
				lim127(lim127(drivePID.output, slowDrivePow) + gyroPID.output, slowDrivePow)
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

// set height with liftPID.target
task liftPIDTask() {
	liftPID.input = getLiftHeight();
	updatePIDVar(liftPID);

	while(true) {
		// average the 2 pots for height
		liftPID.input = getLiftHeight();

		updatePIDVar(liftPID);

		if (liftMode == LIFT_PID_CONTROL) {
			setLift(lim127(liftPID.output));
		}

		wait1Msec(liftPID.loopTime);
	}
}

// yeetsauce
task swingPIDTask() {
	int currSwing = 0;

	swingPID.input = SensorValue[swingPot];
	updatePIDVar(swingPID);

	while (true) {
		currSwing = SensorValue[swingPot];

		if (swingPID.enabled) {
			swingPID.input = currSwing;
			updatePIDVar(swingPID);
			SensorValue[swing] = swingPID.output;
		}

		wait1Msec(swingPID.loopTime);
	}
}

#ifdef sploof
#warning "sploof mode activated"
#endif
