////////////////////////////////////////////////////////
//
// 45 second competition auton
//
////////////////////////////////////////////////////////

// Blue preloads auton
if (autonSelection == BLUE_PRIMARY) {
  writeDebugStreamLine("Blue Primary running");

  // initialization
	motor[rollers] = ROLLERS_HOLD;
  setLiftHeight(LIFT_STATIONARY);
	nMotorEncoder[driveLBB] = 0;
	nMotorEncoder[driveRBB] = 0;
  resetDrive();
	setGyro(0);
	pointTurn(0);
	driveIncremental(0);
	startTask(drivePIDTask);
  startTask(liftPIDTask);

  ////////////////////////////stationary cone
  driveIncremental(18);
  swingOut();
  waitForPID(drivePID);
  tardLift(-60);
  wait1Msec(500);
  motor[rollers] = ROLLERS_OUT;

  setLiftHeight(LIFT_STATIONARY);
  driveIncremental(-18);
  waitForPID(drivePID);
  setLiftHeight(LIFT_SCHMEDIUM);
  pointTurnInc(90);
  waitForPID(gyroPID);

////////////////////////////////mogo 1
  motor[mogo] = -127;
  wait1Msec(400); // 400
  driveIncremental(48);
  extendMogo();
  waitForPID(drivePID);

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
  wait1Msec(CONE_PICKUP_WAIT_TIME);
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
  wait1Msec(CONE_PICKUP_WAIT_TIME);
  motor[rollers] = ROLLERS_HOLD;


  ///////////////////////////////////dropoff
  driveIncremental(-60);

  setLiftHeight(LIFT_CONE_3);
  waitForPID(liftPID);
  swingIn();
  tardLift(-70);
  wait1Msec(600);
  tardLift(-20);

  waitForPID(drivePID);

  pointTurnInc(45);
  waitForPID(gyroPID);
  driveIncremental(-31);//-26
  waitForPID(drivePID);
  pointTurnInc(90);
  waitForPID(gyroPID);


  tardDrive(127);
  wait1Msec(400);
  setLiftHeight(LIFT_CONE_3);
  motor[rollers] = ROLLERS_OUT;
  tardDrive(60);
  wait1Msec(500);
  tardDrive(0);
  extendMogo();

  ///back up
  driveIncremental(-18);
  waitForPID(drivePID,3000);

  waitForPID(gyroPID);

  // tardDrive(25);
  // intakeMogo();
  // wait1Msec(500);
  // tardDrive(0);
  // setGyro(-135);

  ////////////////////////////mogo 2
  // driveIncremental(-9);
  // waitForPID(drivePID);
  // motor[mogo] = -127;
  // extendMogo();
  pointTurnInc(-90);
  setLiftHeight(LIFT_FLOOR_HEIGHT);
  waitForPID(gyroPID);

  driveIncremental(-35);
  extendMogo();
  waitForPID(drivePID, 4000);
  waitForPID(liftPID, 2000);
  swingTurnRightInc(-45);
  waitForPID(gyroPID, 3000);

  tardDrive(-80);//-90
  wait1Msec(600);
  tardDrive(0);

  wait1Msec(600);

  resetGyro();

  // driveIncremental(1);
  // waitForPID(drivePID);
  // pointTurnInc(-90);
  swingTurnLeftInc(-90);
  setLiftHeight(LIFT_CONE_2);
  waitForPID(gyroPID);

  driveIncremental(37);
  waitForPID(drivePID);
  intakeMogo();

  ///////////cone4
  driveIncremental(6);
  swingOut();
  motor[swing] = -50;
  motor[rollers] = ROLLERS_IN;

  setLiftHeight(LIFT_FLOOR_HEIGHT);
  waitForPID(liftPID, 2000);
  wait1Msec(CONE_PICKUP_WAIT_TIME);
  motor[rollers] = ROLLERS_HOLD;

  setLiftHeight(LIFT_CONE_2);//CONE_1 ?
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
  waitForPID(liftPID, 3000);
  wait1Msec(CONE_PICKUP_WAIT_TIME);
  motor[rollers] = ROLLERS_HOLD;

  ///////////////////////////////////dropoff
  driveIncremental(-60);

  setLiftHeight(LIFT_CONE_2);
  waitForPID(liftPID);
  swingIn();
  tardLift(-70);
  wait1Msec(600);
  tardLift(-20);

  waitForPID(drivePID);

  swingTurnRightInc(-135);
  waitForPID(gyroPID);

  // setLiftHeight(LIFT_SCHMEDIUM);
  // motor[rollers] = ROLLERS_OUT;
  tardDrive(26);
  extendMogo();
  tardDrive(0);

  //////////////////////////////mogo3
  driveIncremental(-12);
  waitForPID(drivePID);
  swingTurnRightInc(-45);
  intakeMogo();
  waitForPID(gyroPID);
  driveIncremental(-24);
  waitForPID(drivePID);
  pointTurnInc(-135);
  waitForPID(gyroPID);
  // extendMogo();

  driveIncremental(60);
  waitForPID(drivePID);
  driveIncremental(-24);
  extendMogo();
  waitForPID(drivePID);

  pointTurnInc(45);
  waitForPID(gyroPID);
  driveIncremental(24);
  waitForPID(drivePID);
  intakeMogo();

  //OPTIONAL YEET CODE
  //****//
  pointTurnInc(-45);
  waitForPID(gyroPID);

  driveIncremental(-40);
  waitForPID(drivePID);

  swingTurnLeftInc(-180);
  waitForPID(gyroPID);

  extendMogo();

  //****//

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




  while(1){wait1Msec(1000);}

}
else if (1){

}
