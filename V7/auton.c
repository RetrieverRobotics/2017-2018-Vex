////////////////////////////////////////////////////////
//
// 45 second competition auton
//
////////////////////////////////////////////////////////

// Blue preloads auton
if (autonSelection == BLUE_PRIMARY) {
  writeDebugStreamLine("Blue Preloads running");

  motor[rollers] = ROLLERS_HOLD;
  setLiftHeight(LIFT_SCHMEDIUM);
  resetDrive();
  driveIncremental(0);
  resetGyro();
  pointTurn(getHeading());
  startTask(liftPIDTask);
	startTask(drivePIDTask);

  motor[mogo] = -127;
  wait1Msec(400); // 400
  driveIncremental(48);
  extendMogo();

  waitForPID(drivePID);
  // tardDrive(60);
  // wait1Msec(400);
  // tardDrive(0);

  // writeDebugStreamLine("her77");
  intakeMogo();
  setLiftHeight(LIFT_FLOOR_HEIGHT);
  wait1Msec(300);
  motor[rollers] = ROLLERS_OUT;
///////////cone2
  driveIncremental(8);
  swingOut();
  motor[swing] = -50;
  motor[rollers] = ROLLERS_IN;

  setLiftHeight(LIFT_FLOOR_HEIGHT);
  waitForPID(liftPID);
  wait1Msec(500);
  motor[rollers] = ROLLERS_HOLD;

  setLiftHeight(LIFT_CONE_2);
  waitForPID(liftPID);

  swingIn();
  tardLift(-70);
  wait1Msec(300);
  motor[rollers] = ROLLERS_OUT;
  tardLift(127);
  wait1Msec(100);
  setLiftHeight(LIFT_CONE_2);

/////////////////////////////////////cone 3
  driveIncremental(8);
  swingOut();
  motor[swing] = -50;
  motor[rollers] = ROLLERS_IN;
  // wait1Msec(500);

  setLiftHeight(LIFT_FLOOR_HEIGHT);
  waitForPID(liftPID);
  wait1Msec(500);
  motor[rollers] = ROLLERS_HOLD;

  setLiftHeight(LIFT_CONE_3);
  waitForPID(liftPID);

  swingIn();
  tardLift(-70);
  wait1Msec(300);
  motor[rollers] = ROLLERS_OUT;
  tardLift(127);
  wait1Msec(100);
  tardLift(0);
  motor[rollers] = -ROLLERS_HOLD;

  ///////////////////////////////////dropoff
  driveIncremental(-60);
  waitForPID(drivePID);
  pointTurn(45);
  waitForPID(gyroPID);
  driveIncremental(-26);
  waitForPID(drivePID);

  pointTurn(135);
  waitForPID(gyroPID);

  // setLiftHeight(LIFT_SCHMEDIUM);
  // motor[rollers] = ROLLERS_OUT;

  tardDrive(127);
  wait1Msec(400);
  tardDrive(60);
  wait1Msec(500);
  tardDrive(0);
  extendMogo();

  ///back up
  driveIncremental(-18);
  waitForPID(drivePID);

  pointTurn(135);
  waitForPID(gyroPID);

  tardDrive(25);
  intakeMogo();
  wait1Msec(200);
  tardDrive(0);
  // setGyro(-135);

  ////////////////////////////mogo 2
  driveIncremental(-9);
  waitForPID(drivePID);
  // motor[mogo] = -127;
  // extendMogo();
  pointTurn(45);
  waitForPID(gyroPID);

  driveIncremental(-35);
  extendMogo();
  setLiftHeight(LIFT_FLOOR_HEIGHT);
  waitForPID(liftPID);
  waitForPID(drivePID, 4000);
  swingTurnRight(0);
  waitForPID(gyroPID, 3000);

  tardDrive(-90);
  wait1Msec(600);
  tardDrive(0);

  wait1Msec(600);

  setGyro(0);

  driveIncremental(1);
  setLiftHeight(LIFT_CONE_2);
  waitForPID(drivePID);
  pointTurn(-90);
  extendMogo();
  waitForPID(gyroPID);

  // extendMogo();
  driveIncremental(37);//36
  waitForPID(drivePID);
  intakeMogo();

  ///////////cone4
  driveIncremental(6);
  swingOut();
  motor[swing] = -50;
  motor[rollers] = ROLLERS_IN;

  setLiftHeight(LIFT_FLOOR_HEIGHT);
  waitForPID(liftPID);
  wait1Msec(500);
  motor[rollers] = ROLLERS_HOLD;

  setLiftHeight(LIFT_CONE_2);
  waitForPID(liftPID);

  swingIn();
  tardLift(-70);
  wait1Msec(300);
  motor[rollers] = ROLLERS_OUT;
  tardLift(127);
  wait1Msec(100);
  setLiftHeight(LIFT_CONE_2);

/////////////////////////////////////cone 5
  driveIncremental(6);
  swingOut();
  motor[swing] = -50;
  motor[rollers] = ROLLERS_IN;
  // wait1Msec(500);

  setLiftHeight(LIFT_FLOOR_HEIGHT);
  waitForPID(liftPID);
  wait1Msec(500);
  motor[rollers] = ROLLERS_HOLD;

  setLiftHeight(LIFT_CONE_2);
  waitForPID(liftPID);

  swingIn();
  tardLift(-70);
  wait1Msec(300);
  motor[rollers] = ROLLERS_OUT;
  tardLift(127);
  wait1Msec(100);
  tardLift(0);
  motor[rollers] = -ROLLERS_HOLD;

  ///////////////////////////////////dropoff
  driveIncremental(-60);
  waitForPID(drivePID);
  swingTurnRight(-225);
  waitForPID(gyroPID);

  // setLiftHeight(LIFT_SCHMEDIUM);
  // motor[rollers] = ROLLERS_OUT;
  tardDrive(26);
  extendMogo();
  tardDrive(0);

  //////////////////////////////mogo3
  driveIncremental(-12);
  waitForPID(drivePID);
  swingTurnRight(-315);
  intakeMogo();
  waitForPID(gyroPID);
  driveIncremental(-5);//12?
  waitForPID(drivePID);
  swingTurnRight(-396);
  waitForPID(gyroPID);
  // extendMogo();

  driveIncremental(60);
  waitForPID(drivePID);
  driveIncremental(-24);
  extendMogo();
  waitForPID(drivePID);

  pointTurn(-370);
  waitForPID(gyroPID);
  driveIncremental(24);
  waitForPID(drivePID);
  intakeMogo();

  // driveIncremental(-36);//-28 prev
  // waitForPID(drivePID, 4000);
  // pointTurn(-270);
  // waitForPID(gyroPID);
  //
  // driveIncremental(16);
  // waitForPID(drivePID, 3000);
  //
  // tardDrive(26);
  // extendMogo();
  // wait1Msec(300);
  // tardDrive(0);
  // driveIncremental(-16);


  while(1){wait1Msec(1000);}

}
// Red preloads auton
else if (autonSelection == RED_PRIMARY) {
  writeDebugStreamLine("Red Preloads running");

  motor[rollers] = ROLLERS_HOLD;
  setLiftHeight(LIFT_SCHMEDIUM);
  resetDrive();
  driveIncremental(0);
  resetGyro();
  pointTurn(getHeading());
  startTask(liftPIDTask);
	startTask(drivePIDTask);

  motor[mogo] = -127;
  wait1Msec(400); // 400
  driveIncremental(48);
  extendMogo();

  waitForPID(drivePID);
  // tardDrive(60);
  // wait1Msec(400);
  // tardDrive(0);

  // writeDebugStreamLine("her77");
  intakeMogo();
  setLiftHeight(LIFT_FLOOR_HEIGHT);
  wait1Msec(300);
  motor[rollers] = ROLLERS_OUT;
///////////cone2
  driveIncremental(8);
  swingOut();
  motor[swing] = -50;
  motor[rollers] = ROLLERS_IN;

  setLiftHeight(LIFT_FLOOR_HEIGHT);
  waitForPID(liftPID);
  wait1Msec(500);
  motor[rollers] = ROLLERS_HOLD;

  setLiftHeight(LIFT_CONE_2);
  waitForPID(liftPID);

  swingIn();
  tardLift(-70);
  wait1Msec(300);
  motor[rollers] = ROLLERS_OUT;
  tardLift(127);
  wait1Msec(100);
  setLiftHeight(LIFT_CONE_2);

/////////////////////////////////////cone 3
  driveIncremental(8);
  swingOut();
  motor[swing] = -50;
  motor[rollers] = ROLLERS_IN;
  // wait1Msec(500);

  setLiftHeight(LIFT_FLOOR_HEIGHT);
  waitForPID(liftPID);
  wait1Msec(500);
  motor[rollers] = ROLLERS_HOLD;

  setLiftHeight(LIFT_CONE_3);
  waitForPID(liftPID);

  swingIn();
  tardLift(-70);
  wait1Msec(300);
  motor[rollers] = ROLLERS_OUT;
  tardLift(127);
  wait1Msec(100);
  tardLift(0);
  motor[rollers] = -ROLLERS_HOLD;

  ///////////////////////////////////dropoff
  driveIncremental(-60);
  waitForPID(drivePID);
  pointTurn(-45);
  waitForPID(gyroPID);
  driveIncremental(-26);
  waitForPID(drivePID);

  pointTurn(-135);
  waitForPID(gyroPID);

  // setLiftHeight(LIFT_SCHMEDIUM);
  // motor[rollers] = ROLLERS_OUT;

  tardDrive(127);
  wait1Msec(400);
  tardDrive(60);
  wait1Msec(500);
  tardDrive(0);
  extendMogo();

  ///back up
  driveIncremental(-18);
  waitForPID(drivePID);

  pointTurn(-135);
  waitForPID(gyroPID);

  tardDrive(25);
  intakeMogo();
  wait1Msec(200);
  tardDrive(0);
  // setGyro(-135);

  ////////////////////////////mogo 2
  driveIncremental(-9);
  waitForPID(drivePID);
  // motor[mogo] = -127;
  // extendMogo();
  pointTurn(-45);
  waitForPID(gyroPID);

  driveIncremental(-35);
  extendMogo();
  setLiftHeight(LIFT_FLOOR_HEIGHT);
  waitForPID(liftPID);
  waitForPID(drivePID, 4000);
  swingTurnLeft(0);
  waitForPID(gyroPID, 3000);

  tardDrive(-90);
  wait1Msec(600);
  tardDrive(0);

  wait1Msec(300);

  setGyro(0);

  driveIncremental(1);
  setLiftHeight(LIFT_CONE_2);
  waitForPID(drivePID);
  pointTurn(90);
  extendMogo();
  waitForPID(gyroPID);

  // extendMogo();
  driveIncremental(37);//36
  waitForPID(drivePID);
  intakeMogo();

  ///////////cone4
  driveIncremental(6);
  swingOut();
  motor[swing] = -50;
  motor[rollers] = ROLLERS_IN;

  setLiftHeight(LIFT_FLOOR_HEIGHT);
  waitForPID(liftPID);
  wait1Msec(500);
  motor[rollers] = ROLLERS_HOLD;

  setLiftHeight(LIFT_CONE_2);
  waitForPID(liftPID);

  swingIn();
  tardLift(-70);
  wait1Msec(300);
  motor[rollers] = ROLLERS_OUT;
  tardLift(127);
  wait1Msec(100);
  setLiftHeight(LIFT_CONE_2);

/////////////////////////////////////cone 5
  driveIncremental(6);
  swingOut();
  motor[swing] = -50;
  motor[rollers] = ROLLERS_IN;
  // wait1Msec(500);

  setLiftHeight(LIFT_FLOOR_HEIGHT);
  waitForPID(liftPID);
  wait1Msec(500);
  motor[rollers] = ROLLERS_HOLD;

  setLiftHeight(LIFT_CONE_2);
  waitForPID(liftPID);

  swingIn();
  tardLift(-70);
  wait1Msec(300);
  motor[rollers] = ROLLERS_OUT;
  tardLift(127);
  wait1Msec(100);
  tardLift(0);
  motor[rollers] = -ROLLERS_HOLD;

  ///////////////////////////////////dropoff
  driveIncremental(-60);
  waitForPID(drivePID);
  swingTurnLeft(225);
  waitForPID(gyroPID);

  // setLiftHeight(LIFT_SCHMEDIUM);
  // motor[rollers] = ROLLERS_OUT;
  tardDrive(26);
  extendMogo();
  tardDrive(0);

  //////////////////////////////mogo3
  driveIncremental(-12);
  waitForPID(drivePID);
  swingTurnLeft(315);
  intakeMogo();
  waitForPID(gyroPID);
  driveIncremental(-6);//12?
  waitForPID(drivePID);
  swingTurnLeft(396);
  waitForPID(gyroPID);
  // extendMogo();

  driveIncremental(60);
  waitForPID(drivePID);
  driveIncremental(-24);
  extendMogo();
  waitForPID(drivePID);

  pointTurn(370);
  waitForPID(gyroPID);
  driveIncremental(24);
  waitForPID(drivePID);
  intakeMogo();

  // driveIncremental(-36);//-28 prev
  // waitForPID(drivePID, 4000);
  // pointTurn(-270);
  // waitForPID(gyroPID);
  //
  // driveIncremental(16);
  // waitForPID(drivePID, 3000);
  //
  // tardDrive(26);
  // extendMogo();
  // wait1Msec(300);
  // tardDrive(0);
  // driveIncremental(-16);


  while(1){wait1Msec(1000);}

}
else if (1){

}
