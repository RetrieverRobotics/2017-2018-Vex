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
  driveIncremental(15);//16
  waitForPID(drivePID, 3000);
  waitForPID(liftPID);
  swingOut();
  tardLift(-60);
  wait1Msec(400);
  motor[rollers] = ROLLERS_OUT;

  setLiftHeight(LIFT_STATIONARY);
  swingIn();
  driveIncremental(-16);//-18
  waitForPID(drivePID);
  setLiftHeight(LIFT_SCHMEDIUM);
  pointTurnInc(90); //90
  waitForPID(gyroPID);

////////////////////////////////mogo 1
  motor[mogo] = -127;
  wait1Msec(400); // 400
  driveIncremental(45);//48
  extendMogo();
  waitForPID(drivePID);

  intakeMogo();

  //////////////////////////////////////cone2 (first mogo)
  setLiftHeight(LIFT_FLOOR_HEIGHT);
  driveIncremental(6);//8
  swingOut();
  motor[swing] = -50;
  motor[rollers] = ROLLERS_IN;

  waitForPID(drivePID, 2000);
  waitForPID(liftPID, 2000);

  wait1Msec(CONE_PICKUP_WAIT_TIME);
  motor[rollers] = ROLLERS_HOLD;

  setLiftHeight(LIFT_CONE_2);
  waitForPID(liftPID, 2000);

  swingIn();
  tardLift(-70);
  wait1Msec(CONE_RELEASE_TIME);
  motor[rollers] = ROLLERS_OUT;
  wait1Msec(CONE_SECONDARY_RELEASE_TIME);
  tardLift(127);
  wait1Msec(100);
  setLiftHeight(LIFT_CONE_2);

/////////////////////////////////////cone 3
  driveIncremental(8);//8
  waitForPID(liftPID, 2000);
  swingOut();
  motor[swing] = -50;
  motor[rollers] = ROLLERS_IN;
  // wait1Msec(500);

  if(waitForPID(drivePID, 2000)){
    // do something?
    motor[rollers] = 0;
    motor[swing] = 0;
    driveIncremental(-12);
    waitForPID(drivePID, 2000);
    swingIn();
    wait1Msec(1000);
    driveIncremental(-54);
  }
  else{

    setLiftHeight(LIFT_FLOOR_HEIGHT);
    waitForPID(liftPID, 2000);
    wait1Msec(CONE_PICKUP_WAIT_TIME);
    motor[rollers] = ROLLERS_HOLD;

    setLiftHeight(LIFT_CONE_3);
    waitForPID(liftPID, 2000);
    swingIn();
    tardLift(-70);
    wait1Msec(CONE_RELEASE_TIME);
    tardLift(-20);
    motor[rollers] = ROLLERS_OUT;
    wait1Msec(CONE_SECONDARY_RELEASE_TIME);
    setLiftHeight(LIFT_CONE_3);

  /////////////////////////////////////cone 3.1
    driveIncremental(8);//8
    waitForPID(liftPID);
    swingOut();
    motor[swing] = -50;
    motor[rollers] = ROLLERS_IN;
    // wait1Msec(500);

    waitForPID(drivePID, 2000);

    setLiftHeight(LIFT_FLOOR_HEIGHT);
    waitForPID(liftPID, 2000);
    wait1Msec(CONE_PICKUP_WAIT_TIME);
    motor[rollers] = ROLLERS_HOLD;

    ///////////////////////////////////dropoff
    driveIncremental(-74);//-60
  }

  setLiftHeight(LIFT_CONE_4);
  waitForPID(liftPID);
  swingIn();
  tardLift(-70);
  wait1Msec(CONE_RELEASE_TIME);
  tardLift(-20);
  motor[rollers] = ROLLERS_OUT;
  setLiftHeight(LIFT_SCHMEDIUM);
  // writeDebugStreamLine("here6677");

  waitForPID(drivePID, 4000);
  motor[rollers] = 0;

  pointTurnInc(44);//45
  waitForPID(gyroPID);
  driveIncremental(-31);//-36//-26
  waitForPID(drivePID);
  pointTurnInc(95);//97///90
  waitForPID(gyroPID);


  // tardDrive(127);
  // wait1Msec(400);
  // setLiftHeight(LIFT_CONE_3);
  // // motor[rollers] = ROLLERS_OUT;
  // tardDrive(60);
  // wait1Msec(500);
  // tardDrive(0);
  // extendMogo();

  nogoGadget();
	tardDrive(127);
  wait1Msec(700);
  tardDrive(60);
  wait1Msec(300);
  tardDrive(0);
	wait1Msec(300);

  ///back up
  driveIncremental(-18);
  waitForPID(drivePID,3000);

  // waitForPID(gyroPID);

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
  motor[mogo] = -127;
  pointTurnInc(-83);//-90
  setLiftHeight(LIFT_FLOOR_HEIGHT);
  waitForPID(gyroPID);

  driveIncremental(-28);//-35
  // extendMogo();
  wait1Msec(500);
  motor[mogo] = 0;
  waitForPID(drivePID, 4000);
  waitForPID(liftPID, 2000);
  swingTurnRightInc(-50);
  waitForPID(gyroPID, 3000);

  tardDrive(-80);//-90
  wait1Msec(600);
  tardDrive(0);

  wait1Msec(600);

  resetGyro();

  // driveIncremental(1);
  // waitForPID(drivePID, 2000);
  pointTurnInc(-90.5);//90
  setLiftHeight(LIFT_CONE_2);
  waitForPID(gyroPID);

  driveIncremental(42);//44//37
  waitForPID(drivePID);
  intakeMogo();

  ///////////cone4
  driveIncremental(7);//8
  swingOut();
  motor[swing] = -50;
  motor[rollers] = ROLLERS_IN;

  waitForPID(drivePID);

  setLiftHeight(LIFT_FLOOR_HEIGHT);
  waitForPID(liftPID);
  wait1Msec(CONE_PICKUP_WAIT_TIME);
  motor[rollers] = ROLLERS_HOLD;

  setLiftHeight(LIFT_CONE_2);
  waitForPID(liftPID);

  swingIn();
  tardLift(-70);
  wait1Msec(CONE_RELEASE_TIME);
  motor[rollers] = ROLLERS_OUT;
  wait1Msec(CONE_SECONDARY_RELEASE_TIME);
  tardLift(127);
  wait1Msec(100);
  setLiftHeight(LIFT_CONE_2);

/////////////////////////////////////cone 5
  driveIncremental(5);//8
  waitForPID(liftPID);
  swingOut();
  motor[swing] = -50;
  motor[rollers] = ROLLERS_IN;
  // wait1Msec(500);

  waitForPID(drivePID);

  setLiftHeight(LIFT_FLOOR_HEIGHT);
  waitForPID(liftPID);
  wait1Msec(CONE_PICKUP_WAIT_TIME);
  motor[rollers] = ROLLERS_HOLD;

  ///////////////////////////////////dropoff
  driveIncremental(-70);//-60

  setLiftHeight(LIFT_CONE_2);
  waitForPID(liftPID);
  swingIn();
  tardLift(-70);
  wait1Msec(CONE_RELEASE_TIME);
  tardLift(-20);
  motor[rollers] = ROLLERS_OUT;
  wait1Msec(CONE_SECONDARY_RELEASE_TIME);
  setLiftHeight(LIFT_SCHMEDIUM);

  waitForPID(drivePID, 5000);
  motor[rollers] = 0;


  swingTurnRightInc(-135);
  waitForPID(gyroPID);

  // setLiftHeight(LIFT_SCHMEDIUM);
  // motor[rollers] = ROLLERS_OUT;
  tardDrive(26);
  extendMogo();
  tardDrive(0);

  //////////////////////////////mogo3
  driveIncremental(-8);
  waitForPID(drivePID);
  pointTurnInc(-55);//-45
  intakeMogo();
  waitForPID(gyroPID);
  driveIncremental(-50);//-50//-36
  waitForPID(drivePID);
  pointTurnInc(-120);
  waitForPID(gyroPID);
  // extendMogo();

  driveIncremental(42);
  waitForPID(drivePID);
  driveIncremental(-25);//-24
  extendMogo();
  waitForPID(drivePID);

  pointTurnInc(37);//35
  waitForPID(gyroPID);
  driveIncremental(24);
  waitForPID(drivePID);
  intakeMogo();

  //OPTIONAL YEET CODE
  //****//
  pointTurnInc(-53);
  waitForPID(gyroPID);

  driveIncremental(-40);
  setLiftHeight(LIFT_FLOOR_HEIGHT);
  motor[rollers] = ROLLERS_IN;
  swingOut();
  waitForPID(drivePID);

  tardDrive(0);

  // swingTurnLeftInc(-186);//-180
  // waitForPID(gyroPID);
  //
  // driveIncremental(46);//36
  // nogoGadget();
  // setLiftHeight(LIFT_SCHMEDIUM);
  // swingIn();
  // waitForPID(drivePID, 3000);
  //
  // tardDrive(-127);
  // wait1Msec(300);
  // tardDrive(0);

  // extendMogo();

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
  driveIncremental(15);//16
  waitForPID(drivePID, 3000);
  waitForPID(liftPID);
  swingOut();
  tardLift(-60);
  wait1Msec(400);
  motor[rollers] = ROLLERS_OUT;

  setLiftHeight(LIFT_STATIONARY);
  swingIn();
  driveIncremental(-16);//-18
  waitForPID(drivePID);
  setLiftHeight(LIFT_SCHMEDIUM);
  pointTurnInc(-90); //90
  waitForPID(gyroPID);

////////////////////////////////mogo 1
  motor[mogo] = -127;
  wait1Msec(400); // 400
  driveIncremental(45);//48
  extendMogo();
  waitForPID(drivePID);

  intakeMogo();

  //////////////////////////////////////cone2 (first mogo)
  setLiftHeight(LIFT_FLOOR_HEIGHT);
  driveIncremental(6);//8
  swingOut();
  motor[swing] = -50;
  motor[rollers] = ROLLERS_IN;

  waitForPID(drivePID, 2000);
  waitForPID(liftPID, 2000);

  wait1Msec(CONE_PICKUP_WAIT_TIME);
  motor[rollers] = ROLLERS_HOLD;

  setLiftHeight(LIFT_CONE_2);
  waitForPID(liftPID, 2000);

  swingIn();
  tardLift(-70);
  wait1Msec(CONE_RELEASE_TIME);
  motor[rollers] = ROLLERS_OUT;
  wait1Msec(CONE_SECONDARY_RELEASE_TIME);
  tardLift(127);
  wait1Msec(100);
  setLiftHeight(LIFT_CONE_2);

/////////////////////////////////////cone 3
  driveIncremental(8);//8
  waitForPID(liftPID, 2000);
  swingOut();
  motor[swing] = -50;
  motor[rollers] = ROLLERS_IN;
  // wait1Msec(500);

  if(waitForPID(drivePID, 2000)){
    // do something?
    motor[rollers] = 0;
    motor[swing] = 0;
    driveIncremental(-12);
    waitForPID(drivePID, 2000);
    swingIn();
    wait1Msec(1000);
    driveIncremental(-54);
  }
  else{

    setLiftHeight(LIFT_FLOOR_HEIGHT);
    waitForPID(liftPID, 2000);
    wait1Msec(CONE_PICKUP_WAIT_TIME);
    motor[rollers] = ROLLERS_HOLD;

    setLiftHeight(LIFT_CONE_3);
    waitForPID(liftPID, 2000);
    swingIn();
    tardLift(-70);
    wait1Msec(CONE_RELEASE_TIME);
    tardLift(-20);
    motor[rollers] = ROLLERS_OUT;
    wait1Msec(CONE_SECONDARY_RELEASE_TIME);
    setLiftHeight(LIFT_CONE_3);

  /////////////////////////////////////cone 3.1
    driveIncremental(8);//8
    waitForPID(liftPID);
    swingOut();
    motor[swing] = -50;
    motor[rollers] = ROLLERS_IN;
    // wait1Msec(500);

    waitForPID(drivePID, 2000);

    setLiftHeight(LIFT_FLOOR_HEIGHT);
    waitForPID(liftPID, 2000);
    wait1Msec(CONE_PICKUP_WAIT_TIME);
    motor[rollers] = ROLLERS_HOLD;

    ///////////////////////////////////dropoff
    driveIncremental(-74);//-60
  }

  setLiftHeight(LIFT_CONE_4);
  waitForPID(liftPID);
  swingIn();
  tardLift(-70);
  wait1Msec(CONE_RELEASE_TIME);
  tardLift(-20);
  motor[rollers] = ROLLERS_OUT;
  setLiftHeight(LIFT_SCHMEDIUM);
  // writeDebugStreamLine("here6677");

  waitForPID(drivePID, 4000);
  motor[rollers] = 0;

  pointTurnInc(-44);//45
  waitForPID(gyroPID);
  driveIncremental(-31);//-36//-26
  waitForPID(drivePID);
  pointTurnInc(-95);//97///90
  waitForPID(gyroPID);


  // tardDrive(127);
  // wait1Msec(400);
  // setLiftHeight(LIFT_CONE_3);
  // // motor[rollers] = ROLLERS_OUT;
  // tardDrive(60);
  // wait1Msec(500);
  // tardDrive(0);
  // extendMogo();

  nogoGadget();
	tardDrive(127);
  wait1Msec(700);
  tardDrive(60);
  wait1Msec(300);
  tardDrive(0);
	wait1Msec(300);

  ///back up
  driveIncremental(-18);
  waitForPID(drivePID,3000);

  // waitForPID(gyroPID);

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
  motor[mogo] = -127;
  pointTurnInc(83);//-90
  setLiftHeight(LIFT_FLOOR_HEIGHT);
  waitForPID(gyroPID);

  driveIncremental(-28);//-35
  // extendMogo();
  wait1Msec(500);
  motor[mogo] = 0;
  waitForPID(drivePID, 4000);
  waitForPID(liftPID, 2000);
  swingTurnLeftInc(50);
  waitForPID(gyroPID, 3000);

  tardDrive(-80);//-90
  wait1Msec(600);
  tardDrive(0);

  wait1Msec(600);

  resetGyro();

  // driveIncremental(1);
  // waitForPID(drivePID, 2000);
  pointTurnInc(90.5);//90
  setLiftHeight(LIFT_CONE_2);
  waitForPID(gyroPID);

  driveIncremental(42);//44//37
  waitForPID(drivePID);
  intakeMogo();

  ///////////cone4
  driveIncremental(7);//8
  swingOut();
  motor[swing] = -50;
  motor[rollers] = ROLLERS_IN;

  waitForPID(drivePID);

  setLiftHeight(LIFT_FLOOR_HEIGHT);
  waitForPID(liftPID);
  wait1Msec(CONE_PICKUP_WAIT_TIME);
  motor[rollers] = ROLLERS_HOLD;

  setLiftHeight(LIFT_CONE_2);
  waitForPID(liftPID);

  swingIn();
  tardLift(-70);
  wait1Msec(CONE_RELEASE_TIME);
  motor[rollers] = ROLLERS_OUT;
  wait1Msec(CONE_SECONDARY_RELEASE_TIME);
  tardLift(127);
  wait1Msec(100);
  setLiftHeight(LIFT_CONE_2);

/////////////////////////////////////cone 5
  driveIncremental(5);//8
  waitForPID(liftPID);
  swingOut();
  motor[swing] = -50;
  motor[rollers] = ROLLERS_IN;
  // wait1Msec(500);

  waitForPID(drivePID);

  setLiftHeight(LIFT_FLOOR_HEIGHT);
  waitForPID(liftPID);
  wait1Msec(CONE_PICKUP_WAIT_TIME);
  motor[rollers] = ROLLERS_HOLD;

  ///////////////////////////////////dropoff
  driveIncremental(-70);//-60

  setLiftHeight(LIFT_CONE_2);
  waitForPID(liftPID);
  swingIn();
  tardLift(-70);
  wait1Msec(CONE_RELEASE_TIME);
  tardLift(-20);
  motor[rollers] = ROLLERS_OUT;
  wait1Msec(CONE_SECONDARY_RELEASE_TIME);
  setLiftHeight(LIFT_SCHMEDIUM);

  waitForPID(drivePID, 5000);
  motor[rollers] = 0;


  swingTurnLeftInc(135);
  waitForPID(gyroPID);

  // setLiftHeight(LIFT_SCHMEDIUM);
  // motor[rollers] = ROLLERS_OUT;
  tardDrive(26);
  extendMogo();
  tardDrive(0);

  //////////////////////////////mogo3
  driveIncremental(-8);
  waitForPID(drivePID);
  pointTurnInc(55);//-45
  intakeMogo();
  waitForPID(gyroPID);
  driveIncremental(-50);//-50//-36
  waitForPID(drivePID);
  pointTurnInc(120);
  waitForPID(gyroPID);
  // extendMogo();

  driveIncremental(42);
  waitForPID(drivePID);
  driveIncremental(-25);//-24
  extendMogo();
  waitForPID(drivePID);

  pointTurnInc(-37);//35
  waitForPID(gyroPID);
  driveIncremental(24);
  waitForPID(drivePID);
  intakeMogo();

  //OPTIONAL YEET CODE
  //****//
  pointTurnInc(53);
  waitForPID(gyroPID);

  driveIncremental(-40);
  setLiftHeight(LIFT_FLOOR_HEIGHT);
  motor[rollers] = ROLLERS_IN;
  swingOut();
  waitForPID(drivePID);

  tardDrive(0);

  // swingTurnRightInc(186);//-180
  // waitForPID(gyroPID);
  //
  // driveIncremental(46);//36
  // nogoGadget();
  // setLiftHeight(LIFT_SCHMEDIUM);
  // swingIn();
  // waitForPID(drivePID, 3000);
  //
  // tardDrive(-127);
  // wait1Msec(300);
  // tardDrive(0);

  // extendMogo();

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
else if (autonSelection == DEFAULT){
  writeDebugStreamLine("stationary cone running");

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
  driveIncremental(15);//16
  waitForPID(drivePID, 3000);
  waitForPID(liftPID);
  swingOut();
  tardLift(-60);
  wait1Msec(400);
  motor[rollers] = ROLLERS_OUT;

  setLiftHeight(LIFT_STATIONARY);
  swingIn();
  driveIncremental(-16);//-18
  waitForPID(drivePID);
  setLiftHeight(LIFT_SCHMEDIUM);
}
