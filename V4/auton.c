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
if(true){//autonSelection == BLUE_PRELOAD){//make separate function or file
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
  armHeight(ARM_PRELOAD_HEIGHT);
  waitForPID(gyroPID);

  // tard on the wall
  tardDrive(127);
	wait1Msec(700);
	tardDrive();

  //drive back 6 inches?

  // swing to cone loading height, claw, swing to drop height, release claw
  // do the first few runs until arm differential is needed
  for(int i = 0; i < 2; i++){
    swingPID.target = SWING_OUT;
    waitForPID(swingPID);
    wait1Msec(AUTON_RELOAD_TIME);
    SensorValue[claw] = CLAW_CLOSE;

    swingPID.target = SWING_IN;
    waitForPID(swingPID);
    SensorValue[claw] = CLAW_OPEN;
  }


  // repeat 13x plus 1 extra for a total of 14; add the arm += Cone_Differential
  for(int i = 0; i < 13; i++){
    // add 2.5 inches each go
    newHeight = ARM_PRELOAD_HEIGHT + (ARM_TICKS_PER_INCH * INCHES_PER_CONE * i);

    armHeight(ARM_PRELOAD_HEIGHT);
    waitForPID(swingPID);
    waitForPID(armPID);
    wait1Msec(AUTON_RELOAD_TIME);
    SensorValue[claw] = CLAW_CLOSE;

    armHeight(newHeight);
    // wait until arm a specific distance from target
    while(fabs(armPID.target - getArmHeight()) > SWING_ACTIVATION_DIST)
      wait1Msec(10);

    swingPID.target = SWING_IN;
    waitForPID(armPID);
    waitForPID(swingPID);
    //tard down
    tardLift(-127);
    wait1Msec(100);
    tardLift();

    SensorValue[claw] = CLAW_OPEN;
    wait1Msec(200);
    armHeight(newHeight + SWING_CLEAR_CONE); // clear the cone
    swingPID.target = SWING_OUT;

    // wait for swing to clear cone
    //while
  }

  //drive back and finesse some points
}
