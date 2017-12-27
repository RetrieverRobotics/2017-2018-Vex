////////////////////////////////////////////////////////
//
// 45 second competition auton
//
////////////////////////////////////////////////////////

//zero encoders
SensorValue[driveREnc] = 0;
SensorValue[driveLEnc]	= 0;
clearTimer(T1);

// Blue preloads auton
if(true){//autonSelection == BLUE_PRELOAD){//make separate function or file
  startingRotationOffset = 0; // facing positive x towards mogo
  SensorValue[claw] = CLAW_CLOSE;
  armPID.target = ARM_SCHMEDIUM;
  startTask(armPIDTask);
  gyroPID.target = 0;
  startTask(drivePIDTask);
  swingPID.target = SWING_IN;
  startTask(swingPIDTask);

  //raise arm and put out mobile goal thing all while driving forward
  //arm already rising
  driveIncremental(48); // forward 48 inches
  wait1Msec(300);
  extendMogo();

  waitForPID(drivePID);

  //once at destination, pick up mobile goal
  intakeMogo();

  //drive back to loading station (line sensor?) while dropping preload
  driveIncremental(-12); // backward 12 inches
  //armPID.target = STARTING_HEIGHT; // lower arm to place cone

  waitForPID(drivePID);
  waitForPID(armPID);

  // drop cone
  SensorValue[claw] = CLAW_OPEN;

  //turn 90 towards preloader
  pointTurn(-90);
  //arm to loading height and drive forward
  armPID.target = ARM_PRELOAD_HEIGHT;
  swingPID.target = SWING_90;
  waitForPID(gyroPID);

  // turn drive PIDs off and give a basic constant power to slam wall
  drivePIDsOn(false);
  tankDrive(127, 127);
  wait1Msec(700);
  drivePIDsOn(true);

  //drive back 6 inches?

  //swing to cone loading height, claw, swing to drop height, release claw
  // do the first few runs until arm differential is needed
  for(int i = 0; i < 2; i++){
    swingPID.target = SWING_OUT;
    waitForPID(swingPID);
    wait1Msec(200); // insert reasonable wait time for humans to place cone
    SensorValue[claw] = CLAW_CLOSE;

    swingPID.target = SWING_IN;
    waitForPID(swingPID);
    SensorValue[claw] = CLAW_OPEN;
  }


  // repeat 13x plus 1 extra for a total of 14; add the arm += Cone_Differential
  for(int i = 0; i < 13; i++){
    swingPID.target = SWING_OUT;
    armPID.target = ARM_PRELOAD_HEIGHT;
    waitForPID(swingPID);
    waitForPID(armPID);
    wait1Msec(200); // insert reasonable wait time for humans to place cone
    SensorValue[claw] = CLAW_CLOSE;

    // TODO: make function that converts 2.5 inches to arm pot ticks
    armPID.target = ARM_PRELOAD_HEIGHT + (2.5 * i); // add 2.5 inches each go
    // wait until arm a specific distance from target
    //while(fabs(armPID.target - armPID.input) < SWING_ACTIVATE_ON_ARM_VAL);
    swingPID.target = SWING_IN;
    waitForPID(swingPID);
    waitForPID(armPID);
    SensorValue[claw] = CLAW_OPEN;
  }

  //drive back and finesse some points
}
