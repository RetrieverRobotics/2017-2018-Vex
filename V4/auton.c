////////////////////////////////////////////////////////
//
// 45 second competition auton
//
////////////////////////////////////////////////////////

//zero encoders
SensorValue[driveREnc] = 0;
SensorValue[driveLEnc] = 0;
clearTimer(T1);

int newHeight = 0;

// Blue preloads auton
if (autonSelection == BLUE_PRELOAD) {
  startingRotationOffset = 0; // facing positive x towards mogo
  SensorValue[claw] = CLAW_CLOSE;
	armPID.target 		= ARM_SCHMEDIUM;
	gyroPID.target 		= 0;
	swingPID.target 	= SWING_IN;
	startTask(armPIDTask);
	startTask(drivePIDTask);
	startTask(swingPIDTask);

  //raise arm and put out mobile goal thing all while driving forward
  //arm already rising
  driveIncremental(48); // forward 48 inches
  wait1Msec(300);
  extendMogo();
  waitForPID(drivePID);
  // swingTurnLeft()? tardDrive?
  // waitForPID(gyroPID);
  //once at destination, pick up mobile goal
  intakeMogo();

  //drive back to loading station (line sensor?) while dropping preload
  driveIncremental(-12); // backward 12 inches
  armPID.target = ARM_STARTING_HEIGHT; // lower arm to place cone
  waitForPID(armPID);
  // drop cone
  SensorValue[claw] = CLAW_OPEN;
  waitForPID(drivePID);

  //turn 90 towards preloader
  pointTurn(-90);
  //arm to loading height and drive forward
  swingPID.target = SWING_90;
  setArmHeight(ARM_PRELOAD_HEIGHT);
  waitForPID(gyroPID);

  // tard on the wall
  tardDrive(127);
	wait1Msec(700);
	tardDrive();

  // swing out
  swingPID.target = SWING_OUT;
  //drive back 6 inches?
  waitForPID(swingPID);

  // swing to cone loading height, grab, swing to drop height, release
  // do the first few runs until arm differential is needed
  for(int i = 0; i < 2; i++){
    wait1Msec(AUTON_RELOAD_TIME);
    SensorValue[claw] = CLAW_CLOSE;

    swingPID.target = SWING_IN;
    waitForPID(swingPID);
    SensorValue[claw] = CLAW_OPEN;
    wait1Msec(CLAW_OPEN_TIME);
    swingPID.target = SWING_OUT;
    waitForPID(swingPID);
  }


  // repeat 13x plus 1 extra for a total of 14
  for(int i = 0; i < AUTON_STACK_COUNT; i++){
    wait1Msec(AUTON_RELOAD_TIME);
    SensorValue[claw] = CLAW_CLOSE;

    // add 2.5 inches each go
    newHeight = ARM_PRELOAD_HEIGHT + (ARM_TICKS_PER_INCH * INCHES_PER_CONE * i);
    setArmHeight(newHeight);
    // wait until arm a specific distance from target
    // while the error of the arm is bigger than SWING_ACTIVATION_DIST
    while(fabs(armPID.target - getArmHeight()) > SWING_ACTIVATION_DIST)
      wait1Msec(10);

    swingPID.target = SWING_IN;
    waitForPID(armPID);
    waitForPID(swingPID);
    //tard down
    tardLift(-127);
    wait1Msec(100);
    tardLift();

    // stop here on the last one
    if(i == AUTON_STACK_COUNT)
      break;

    SensorValue[claw] = CLAW_OPEN;
    wait1Msec(CLAW_OPEN_TIME);
    setArmHeight(newHeight + ARM_CLEAR_CONE); // clear the cone
    swingPID.target = SWING_OUT;

    // wait for swing to clear cone
    while (SensorValue[swingPot] > SWING_CLEAR_CONE)
      wait1Msec(10);

    setArmHeight(ARM_PRELOAD_HEIGHT);
    waitForPID(swingPID);
    waitForPID(armPID);
  }

  //drive back and finesse some points
  pointTurn(-203);
  driveIncremental(54);
  pointTurn(-135);
  // tard on the pipe
	tardDrive(127);
	wait1Msec(1000);
	tardDrive();
	extendMogo();
	// tardDrive(-60,-60,300);
	driveIncremental(-4); // drive back enough to release mogo
	waitForPID(drivePID);
	// intakeMogo();

}

// Red preloads auton
if (autonSelection == RED_PRELOAD) {
  writeDebugStreamLine("Red Preloads running");
  startingRotationOffset = 180; // facing positive x towards mogo
  SensorValue[claw] = CLAW_CLOSE;
	armPID.target 		= ARM_SCHMEDIUM;
	gyroPID.target 		= 0;
	swingPID.target 	= SWING_IN;
	startTask(armPIDTask);
	startTask(drivePIDTask);
	startTask(swingPIDTask);

  //raise arm and put out mobile goal thing all while driving forward
  //arm already rising
  driveIncremental(48); // forward 48 inches
  wait1Msec(300);
  extendMogo();
  waitForPID(drivePID);
  // swingTurnLeft()? tardDrive?
  // waitForPID(gyroPID);
  //once at destination, pick up mobile goal
  intakeMogo();

  //drive back to loading station (line sensor?) while dropping preload
  driveIncremental(-12); // backward 12 inches
  armPID.target = ARM_STARTING_HEIGHT; // lower arm to place cone
  waitForPID(armPID);
  // drop cone
  SensorValue[claw] = CLAW_OPEN;
  waitForPID(drivePID);

  //turn 90 towards preloader
  pointTurn(270);
  //arm to loading height and drive forward
  swingPID.target = SWING_90;
  setArmHeight(ARM_PRELOAD_HEIGHT);
  waitForPID(gyroPID);

  // tard on the wall
  tardDrive(127);
	wait1Msec(700);
	tardDrive();

  // swing out
  swingPID.target = SWING_OUT;
  //drive back 6 inches?
  waitForPID(swingPID);

  // swing to cone loading height, grab, swing to drop height, release
  // do the first few runs until arm differential is needed
  for(int i = 0; i < 2; i++){
    wait1Msec(AUTON_RELOAD_TIME);
    SensorValue[claw] = CLAW_CLOSE;

    swingPID.target = SWING_IN;
    waitForPID(swingPID);
    SensorValue[claw] = CLAW_OPEN;
    wait1Msec(CLAW_OPEN_TIME);
    swingPID.target = SWING_OUT;
    waitForPID(swingPID);
  }


  // repeat 13x plus 1 extra for a total of 14
  for(int i = 0; i < AUTON_STACK_COUNT; i++){
    wait1Msec(AUTON_RELOAD_TIME);
    SensorValue[claw] = CLAW_CLOSE;

    // add 2.5 inches each go
    newHeight = ARM_PRELOAD_HEIGHT + (ARM_TICKS_PER_INCH * INCHES_PER_CONE * i);
    setArmHeight(newHeight);
    // wait until arm a specific distance from target
    // while the error of the arm is bigger than SWING_ACTIVATION_DIST
    while(fabs(armPID.target - getArmHeight()) > SWING_ACTIVATION_DIST)
      wait1Msec(10);

    swingPID.target = SWING_IN;
    waitForPID(armPID);
    waitForPID(swingPID);
    //tard down
    tardLift(-127);
    wait1Msec(100);
    tardLift();

    // stop here on the last one
    if(i == AUTON_STACK_COUNT)
      break;

    SensorValue[claw] = CLAW_OPEN;
    wait1Msec(CLAW_OPEN_TIME);
    setArmHeight(newHeight + ARM_CLEAR_CONE); // clear the cone
    swingPID.target = SWING_OUT;

    // wait for swing to clear cone
    while (SensorValue[swingPot] > SWING_CLEAR_CONE)
      wait1Msec(10);

    setArmHeight(ARM_PRELOAD_HEIGHT);
    waitForPID(swingPID);
    waitForPID(armPID);
  }

  //drive back and finesse some points
  pointTurn(383);
  driveIncremental(54);
  pointTurn(315);
  // tard on the pipe
	tardDrive(127);
	wait1Msec(1000);
	tardDrive();
	extendMogo();
	// tardDrive(-60,-60,300);
	driveIncremental(-4); // drive back enough to release mogo
	waitForPID(drivePID);
	// intakeMogo();

}
