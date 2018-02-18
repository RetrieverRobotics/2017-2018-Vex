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
  writeDebugStreamLine("Blue Preloads running");
  startingRotationOffset = 0;//180; // facing positive x towards mogo
  SensorValue[claw] = CLAW_CLOSE;
  setArmHeight(ARM_SCHMEDIUM);
	gyroPID.target 		= 0;
	swingPID.target 	= SWING_IN;
	startTask(armPIDTask);
	startTask(drivePIDTask);
	startTask(swingPIDTask);

  //raise arm and put out mobile goal thing all while driving forward
  //arm already rising
  motor[mogo] = -127;
  wait1Msec(400); // 400
  driveIncremental(35); // forward 36 inches
  extendMogo();
  // wait1Msec(300);
  waitForPID(drivePID);

  // driveMode = SLOW_DRIVE;
  // driveSlow(6);
  // wait1Msec(400);
  tardDriveStraight(100);
  wait1Msec(200);
  tardDriveStraight(40);
  wait1Msec(700);
  tardDriveStraight();
  // swingTurnLeft()? tardDrive?
  // waitForPID(gyroPID);
  //once at destination, pick up mobile goal
  intakeMogo();

  //drive back to loading station (line sensor?) while dropping preload
  // driveIncremental(-7); // backward 12 inches
  setArmHeight(ARM_STARTING_HEIGHT); // lower arm to place cone
  waitForPID(armPID);
  // drop cone
  SensorValue[claw] = CLAW_OPEN;
  // waitForPID(drivePID);

  //turn 90 towards preloader
  // swingTurnLeft(270);
  // wait1Msec(2000);
  pointTurn(-30);
  // driveMode = POINT_TURN;
  // gyroPID.target = degToGyro(-30);
  resetDrive();
  // writeDebugStreamLine("%f", drivePID.target)

  // writeDebugStreamLine("here4");
  // pointTurn(-30);
  // wait1Msec(2000);
  // writeDebugStreamLine("here77");

  waitForPID(gyroPID, 3000);
  writeDebugStreamLine("here2");
  // wait1Msec(2000);
  // waitForPID(gyroPID);
  // writeDebugStreamLine("here76");
  // driveIncremental(-6);
  // writeDebugStreamLine("here1");
  // wait1Msec(2000);
  //tardDriveStraight(-100);
  //wait1Msec(100);
  //tardDrive(0);
  driveIncremental(-11);
  waitForPID(drivePID);
  // writeDebugStreamLine("here");
  tardDrive(0);
  // wait1Msec(3000);
  pointTurn(-90);
  // wait1Msec(3000);
  // waitForPID(gyroPID);

  //arm to loading height and drive forward
  swingPID.target = SWING_IN;
  setArmHeight(ARM_PRELOAD_HEIGHT);
  waitForPID(gyroPID);

  // writeDebugStreamLine("here3");
  // tard on the wall
  tardDrive(80);
	wait1Msec(300);
  tardDrive(50);
  wait1Msec(500);
	tardDrive();
  wait1Msec(300);
  resetGyro();

  driveIncremental(-16);
  waitForPID(drivePID);

  // swing out
  // swingPID.target = SWING_OUT;
  // //drive back 6 inches?
  // waitForPID(swingPID);

  // swing to cone loading height, grab, swing to drop height, release
  // do the first few runs until arm differential is needed
  for(int i = 0; i < 1; i++){
    wait1Msec(AUTON_RELOAD_TIME);
    SensorValue[claw] = CLAW_CLOSE;
    wait1Msec(CLAW_CLOSE_TIME);

    swingPID.target = SWING_IN;
    waitForPID(swingPID, 5000);
    SensorValue[claw] = CLAW_OPEN;
    wait1Msec(CLAW_OPEN_TIME);
    swingPID.target = SWING_OUT;
    waitForSensor(swing, SWING_OUT, 200, 1000);
    //wait1Msec(SWING_OUT_TIME);
  }


  // repeat 13x plus 1 extra for a total of 14
  for(int i = 0; i < AUTON_STACK_COUNT; i++){
    wait1Msec(AUTON_RELOAD_TIME);
    SensorValue[claw] = CLAW_CLOSE;
    wait1Msec(CLAW_CLOSE_TIME);

    // add 2.5 inches each go
    newHeight = ARM_PRELOAD_HEIGHT + (ARM_TICKS_PER_INCH * INCHES_PER_CONE * i);
    setArmHeight(newHeight);
    // wait until arm a specific distance from target
    // while the error of the arm is bigger than SWING_ACTIVATION_DIST
    //waitForSensor(swing,
    while(fabs(armPID.target - getArmHeight()) > SWING_ACTIVATION_DIST)
      wait1Msec(10);

    swingPID.target = SWING_IN;
    waitForPID(armPID);
    waitForPID(swingPID);
     //wait1Msec(100);
    // tard down
    //tardLift(-60);
    //wait1Msec(100);
    //tardLift();

    // stop here on the last one
    if(i == AUTON_STACK_COUNT-1)
      break;

    SensorValue[claw] = CLAW_OPEN;
    wait1Msec(CLAW_OPEN_TIME);
    setArmHeight(newHeight + ARM_CLEAR_CONE); // clear the cone
    waitForPID(armPID, 2000);

    // wait for swing to clear cone
    //while (SensorValue[swingPot] < SWING_CLEAR_CONE)
      //wait1Msec(10);

    swingPID.target = SWING_OUT;

    setArmHeight(ARM_PRELOAD_HEIGHT);
    waitForSensor(swing, SWING_OUT, 200, 1000);
    //wait1Msec(SWING_OUT_TIME);
    waitForPID(armPID, 2000);
    //wait1Msec(100);
  }

  //drive back and finesse some points
  pointTurn(-160);
  resetDrive();
  waitForPID(gyroPID, 3000);
  driveIncremental(12);//15);
  waitForPID(drivePID, 2000);
  swingTurnRight(-135);
  waitForPID(gyroPID, 2000);
  wait1Msec(500);
  // swingTurnLeft(315);

  writeDebugStreamLine("hererer");
  // tardLiftStraight(-20);
  swingPID.enabled = false;
  // tardLiftStraight(-10);
  motor[mogo] = 127;
  wait1Msec(600);
  motor[swing] = 30;
	extendMogo();
  SensorValue[claw] = CLAW_OPEN;
	motor[swing] = -30;
	tardLiftStraight(0);
	wait1Msec(CLAW_OPEN_TIME);
	motor[swing] = 0;

	// tardDrive(-60,-60,300);
	driveIncremental(-4); // drive back enough to release mogo
	waitForPID(drivePID);

  tardDrive(-50);
  wait1Msec(3000);
  tardDrive(0);



  // startingRotationOffset = 0; // facing positive x towards mogo
  // SensorValue[claw] = CLAW_CLOSE;
	// setArmHeight(ARM_SCHMEDIUM);
	// gyroPID.target 		= 0;
	// swingPID.target 	= SWING_IN;
	// startTask(armPIDTask);
	// startTask(drivePIDTask);
	// startTask(swingPIDTask);
  //
  // //raise arm and put out mobile goal thing all while driving forward
  // //arm already rising
  // extendMogo();
  // // while(1){}
  // driveIncremental(36); // forward 48 inches
  // // wait1Msec(300);
  // waitForPID(drivePID);
  // // swingTurnLeft()? tardDrive?
  // // waitForPID(gyroPID);
  // //once at destination, pick up mobile goal
  // intakeMogo();
  //
  // //drive back to loading station (line sensor?) while dropping preload
  // driveIncremental(-12); // backward 12 inches
  // armPID.target = ARM_STARTING_HEIGHT; // lower arm to place cone
  // waitForPID(armPID);
  // // drop cone
  // SensorValue[claw] = CLAW_OPEN;
  // waitForPID(drivePID);
  //
  // //turn 90 towards preloader
  // pointTurn(-90);
  // //arm to loading height and drive forward
  // swingPID.target = SWING_90;
  // setArmHeight(ARM_PRELOAD_HEIGHT);
  // waitForPID(gyroPID);
  //
  // // tard on the wall
  // tardDrive(127);
	// wait1Msec(700);
	// tardDrive();
  //
  // // swing out
  // swingPID.target = SWING_OUT;
  // //drive back 6 inches?
  // waitForPID(swingPID);
  //
  // // swing to cone loading height, grab, swing to drop height, release
  // // do the first few runs until arm differential is needed
  // for(int i = 0; i < 1; i++){
  //   wait1Msec(AUTON_RELOAD_TIME);
  //   SensorValue[claw] = CLAW_CLOSE;
  //   wait1Msec(CLAW_CLOSE_TIME);
  //
  //   swingPID.target = SWING_IN;
  //   waitForPID(swingPID, 5000);
  //   SensorValue[claw] = CLAW_OPEN;
  //   wait1Msec(CLAW_OPEN_TIME);
  //   swingPID.target = SWING_OUT;
  //   waitForSensor(swing, SWING_OUT, 200, 1000);
  //   //wait1Msec(SWING_OUT_TIME);
  // }
  //
  //
  // // repeat 13x plus 1 extra for a total of 14
  // for(int i = 0; i < AUTON_STACK_COUNT; i++){
  //   wait1Msec(AUTON_RELOAD_TIME);
  //   SensorValue[claw] = CLAW_CLOSE;
  //   wait1Msec(CLAW_CLOSE_TIME);
  //
  //   // add 2.5 inches each go
  //   newHeight = ARM_PRELOAD_HEIGHT + (ARM_TICKS_PER_INCH * INCHES_PER_CONE * i);
  //   setArmHeight(newHeight);
  //   // wait until arm a specific distance from target
  //   // while the error of the arm is bigger than SWING_ACTIVATION_DIST
  //   //waitForSensor(swing,
  //   while(fabs(armPID.target - getArmHeight()) > SWING_ACTIVATION_DIST)
  //     wait1Msec(10);
  //
  //   swingPID.target = SWING_IN;
  //   waitForPID(armPID);
  //   waitForPID(swingPID);
  //    //wait1Msec(100);
  //   // tard down
  //   //tardLift(-60);
  //   //wait1Msec(100);
  //   //tardLift();
  //
  //   // stop here on the last one
  //   if(i == AUTON_STACK_COUNT)
  //     break;
  //
  //   SensorValue[claw] = CLAW_OPEN;
  //   wait1Msec(CLAW_OPEN_TIME);
  //   setArmHeight(newHeight + ARM_CLEAR_CONE); // clear the cone
  //   waitForPID(armPID, 2000);
  //
  //   // wait for swing to clear cone
  //   //while (SensorValue[swingPot] < SWING_CLEAR_CONE)
  //     //wait1Msec(10);
  //
  //   swingPID.target = SWING_OUT;
  //
  //   setArmHeight(ARM_PRELOAD_HEIGHT);
  //   waitForSensor(swing, SWING_OUT, 200, 1000);
  //   //wait1Msec(SWING_OUT_TIME);
  //   waitForPID(armPID, 2000);
  //   //wait1Msec(100);
  // }
  //
  // //drive back and finesse some points
  // pointTurn(-203);
  // driveIncremental(54);
  // pointTurn(-135);
  // // tard on the pipe
	// tardDrive(127);
	// wait1Msec(1000);
	// tardDrive();
	// extendMogo();
	// // tardDrive(-60,-60,300);
	// driveIncremental(-4); // drive back enough to release mogo
	// waitForPID(drivePID);
	// // intakeMogo();

}

// Red preloads auton
if (autonSelection == RED_PRELOAD) {
  writeDebugStreamLine("Red Preloads running");
  startingRotationOffset = 0;//180; // facing positive x towards mogo
  SensorValue[claw] = CLAW_CLOSE;
  setArmHeight(ARM_SCHMEDIUM);
	gyroPID.target 		= 0;
	swingPID.target 	= SWING_IN;
	startTask(armPIDTask);
	startTask(drivePIDTask);
	startTask(swingPIDTask);

  //raise arm and put out mobile goal thing all while driving forward
  //arm already rising
  motor[mogo] = -127;
  wait1Msec(400); // 400
  driveIncremental(35); // forward 36 inches
  extendMogo();
  // wait1Msec(300);
  waitForPID(drivePID);

  // driveMode = SLOW_DRIVE;
  // driveSlow(6);
  // wait1Msec(400);
  tardDriveStraight(100);
  wait1Msec(200);
  tardDriveStraight(40);
  wait1Msec(700);
  tardDriveStraight();
  // swingTurnLeft()? tardDrive?
  // waitForPID(gyroPID);
  //once at destination, pick up mobile goal
  intakeMogo();

  //drive back to loading station (line sensor?) while dropping preload
  // driveIncremental(-7); // backward 12 inches
  setArmHeight(ARM_STARTING_HEIGHT); // lower arm to place cone
  waitForPID(armPID);
  // drop cone
  SensorValue[claw] = CLAW_OPEN;
  // waitForPID(drivePID);

  //turn 90 towards preloader
  // swingTurnLeft(270);
  // wait1Msec(2000);
  pointTurn(30);
  // driveMode = POINT_TURN;
  // gyroPID.target = degToGyro(-30);
  resetDrive();
  // writeDebugStreamLine("%f", drivePID.target)

  // writeDebugStreamLine("here4");
  // pointTurn(-30);
  // wait1Msec(2000);
  // writeDebugStreamLine("here77");

  waitForPID(gyroPID, 3000);
  writeDebugStreamLine("here2");
  // wait1Msec(2000);
  // waitForPID(gyroPID);
  // writeDebugStreamLine("here76");
  // driveIncremental(-6);
  // writeDebugStreamLine("here1");
  // wait1Msec(2000);
  //tardDriveStraight(-100);
  //wait1Msec(100);
  //tardDrive(0);
  driveIncremental(-11);
  waitForPID(drivePID);
  // writeDebugStreamLine("here");
  tardDrive(0);
  // wait1Msec(3000);
  pointTurn(90);
  // wait1Msec(3000);
  // waitForPID(gyroPID);

  //arm to loading height and drive forward
  swingPID.target = SWING_IN;
  setArmHeight(ARM_PRELOAD_HEIGHT);
  waitForPID(gyroPID);

  // writeDebugStreamLine("here3");
  // tard on the wall
  tardDrive(80);
	wait1Msec(300);
  tardDrive(50);
  wait1Msec(500);
	tardDrive();
  wait1Msec(300);
  resetGyro();

  driveIncremental(-16);
  waitForPID(drivePID);

  // swing out
  // swingPID.target = SWING_OUT;
  // //drive back 6 inches?
  // waitForPID(swingPID);

  // swing to cone loading height, grab, swing to drop height, release
  // do the first few runs until arm differential is needed
  for(int i = 0; i < 1; i++){
    wait1Msec(AUTON_RELOAD_TIME);
    SensorValue[claw] = CLAW_CLOSE;
    wait1Msec(CLAW_CLOSE_TIME);

    swingPID.target = SWING_IN;
    waitForPID(swingPID, 5000);
    SensorValue[claw] = CLAW_OPEN;
    wait1Msec(CLAW_OPEN_TIME);
    swingPID.target = SWING_OUT;
    waitForSensor(swing, SWING_OUT, 200, 1000);
    //wait1Msec(SWING_OUT_TIME);
  }


  // repeat 13x plus 1 extra for a total of 14
  for(int i = 0; i < AUTON_STACK_COUNT; i++){
    wait1Msec(AUTON_RELOAD_TIME);
    SensorValue[claw] = CLAW_CLOSE;
    wait1Msec(CLAW_CLOSE_TIME);

    // add 2.5 inches each go
    newHeight = ARM_PRELOAD_HEIGHT + (ARM_TICKS_PER_INCH * INCHES_PER_CONE * i);
    setArmHeight(newHeight);
    // wait until arm a specific distance from target
    // while the error of the arm is bigger than SWING_ACTIVATION_DIST
    //waitForSensor(swing,
    while(fabs(armPID.target - getArmHeight()) > SWING_ACTIVATION_DIST)
      wait1Msec(10);

    swingPID.target = SWING_IN;
    waitForPID(armPID);
    waitForPID(swingPID);
     //wait1Msec(100);
    // tard down
    //tardLift(-60);
    //wait1Msec(100);
    //tardLift();

    // stop here on the last one
    if(i == AUTON_STACK_COUNT-1)
      break;

    SensorValue[claw] = CLAW_OPEN;
    wait1Msec(CLAW_OPEN_TIME);
    setArmHeight(newHeight + ARM_CLEAR_CONE); // clear the cone
    waitForPID(armPID, 2000);

    // wait for swing to clear cone
    //while (SensorValue[swingPot] < SWING_CLEAR_CONE)
      //wait1Msec(10);

    swingPID.target = SWING_OUT;

    setArmHeight(ARM_PRELOAD_HEIGHT);
    waitForSensor(swing, SWING_OUT, 200, 1000);
    //wait1Msec(SWING_OUT_TIME);
    waitForPID(armPID, 2000);
    //wait1Msec(100);
  }

  //drive back and finesse some points
  pointTurn(160);
  resetDrive();
  waitForPID(gyroPID, 3000);
  driveIncremental(12);//15);
  waitForPID(drivePID, 2000);
  swingTurnRight(135);
  waitForPID(gyroPID, 2000);
  wait1Msec(500);
  // swingTurnLeft(315);

  writeDebugStreamLine("hererer");
  // tardLiftStraight(-20);
  swingPID.enabled = false;
  // tardLiftStraight(-10);
  motor[mogo] = 127;
  wait1Msec(600);
  motor[swing] = 30;
	extendMogo();
  SensorValue[claw] = CLAW_OPEN;
	motor[swing] = -30;
	tardLiftStraight(0);
	wait1Msec(CLAW_OPEN_TIME);
	motor[swing] = 0;

	// tardDrive(-60,-60,300);
	driveIncremental(-4); // drive back enough to release mogo
	waitForPID(drivePID);

  tardDrive(-50);
  wait1Msec(3000);
  tardDrive(0);
}
else if (autonSelection == BLUE_MOGOS){
  startingRotationOffset = 0;//180; // facing positive x towards mogo
  SensorValue[claw] = CLAW_CLOSE;
  setArmHeight(ARM_SCHMEDIUM);
	gyroPID.target 		= 0;
	swingPID.target 	= SWING_IN;
  SensorValue[mogoFlip] = FLIPPER_IN;
	startTask(armPIDTask);
	startTask(drivePIDTask);
	startTask(swingPIDTask);

  driveIncremental(22);
  waitForPID(drivePID);
  pointTurn(-90);
  waitForPID(gyroPID);

  tardDrive(-127);
  wait1Msec(1000);
  tardDrive();

  SensorValue[mogoFlip] = FLIPPER_OUT;
  wait1Msec(500);
  tardDrive(127);
  wait1Msec(1000);
  tardDrive();

}
