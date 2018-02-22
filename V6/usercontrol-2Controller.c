////////////////////////////////////////////////////////
//
// Usercontrol utilizing the slave controller
//
////////////////////////////////////////////////////////

clearTimer(T1);
clearTimer(T2);
SensorValue[driveLEnc] = 0;
SensorValue[driveREnc] = 0;
string displayString;
bool   bSwingManual = true;
bool   bSwingToggle = true;
bool   bLiftJustUsed = false;
bool   bLiftHeightRecorded = false;
bool   bFlagRecordLift = false;
int    liftSetPow = 0;
int    currLiftHeight = getLiftHeight();
int    lastLiftHeight = currLiftHeight;
float  deltaHeight = 0;
float  lastDeltaHeight = 0;
// everything for the drive in usrctrl handled in this task.
swingPID.target = SensorValue[swingPot];
tardLift(0);
startTask(driveSlew);
startTask(swingPIDTask);
startTask(liftPIDTask);

bLCDBacklight = false;

while (true) {
  // Display battery level
  clearLCDLine(0);
  clearLCDLine(1);
  sprintf(displayString, "Main: %2.2fV", nAvgBatteryLevel/1000.);
  displayLCDCenteredString(0, displayString);
  sprintf(displayString, "Backup: %2.2fV", BackupBatteryLevel/1000.);
  displayLCDCenteredString(1, displayString);

  currLiftHeight = getLiftHeight();
  deltaHeight = currLiftHeight - lastLiftHeight;

  //--------------------------------------------------------------------------------
  // Lift
  //--------------------------------------------------------------------------------

  // right joystick y axis on slave controller controls lift
  liftSetPow = vexRT[Ch2Xmtr2];
  // hold position inside deabands
  if (fabs(liftSetPow) < ARM_DEADBAND) {
    // clear timer once immediately after button release
    if (bLiftJustUsed) {
      clearTimer(T2);
      bLiftHeightRecorded = false; // set up to turn on pid later
      tardLiftStraight(0);
    }

    // check conditions to set lift height
    bFlagRecordLift = false;
    // enable conditions
    // look for local extrema of the height function and record position there
    // if (sgn(deltaHeight) != sgn(lastDeltaHeight)) bFlagRecordLift = true;
    // if (deltaHeight == 0) bFlagRecordLift = true;
    if (vexRT[Btn8RXmtr2]) bFlagRecordLift = true;
    // timeout after 500 ms
    // if (time1[T2] > 500) bFlagRecordLift = true;

    // disable conditions
    // only turn on PIDs if the lift is up
    if (currLiftHeight < ARM_BLOCK_MOGO) bFlagRecordLift = false;

    if (!bLiftHeightRecorded && bFlagRecordLift) {
      setLiftHeight(currLiftHeight);
      bLiftHeightRecorded = true;
    }

    bLiftJustUsed = false;
  }
  else {
    tardLiftStraight(liftSetPow);
    bLiftJustUsed = true;
  }

  //--------------------------------------------------------------------------------
  // Swing
  //--------------------------------------------------------------------------------

  //switch between manual and PID for swing on 7L
  if (vexRT[Btn7LXmtr2]) {
    if (bSwingToggle) {
      bSwingManual = !bSwingManual;
      bSwingToggle = false;
    }
  }
  else {
    bSwingToggle = true;
  }

  // if  manual control is selected
  if (bSwingManual) {
    swingPID.enabled = false;
    // swing in on 7D
    if (vexRT[Btn7DXmtr2]) {
      motor[swing] = -127;
    }
    // swing out on 7U
    else if (vexRT[Btn7UXmtr2]) {
      motor[swing] = 127;
    }
    else {
      // swing on Ch3Xmtr2
      motor[swing] = vexRT[Ch3Xmtr2];
    }
  }
  // otherwise use PID control
  else {
    swingPID.enabled = true;
    // in on 7D
    if (vexRT[Btn7DXmtr2]) {
      swingPID.target = SWING_IN;
    }
    // out on 7U
    else if (vexRT[Btn7UXmtr2]) {
      // SensorValue[claw] = CLAW_OPEN;
      swingPID.target = SWING_OUT;
    }
    // 90 on 7R
    else if (vexRT[Btn7RXmtr2]) {
      swingPID.target = SWING_FLOOR;
    }
    else if (vexRT[Btn5UXmtr2]) {
      swingPID.target = SWING_90;
    }
  }

  //--------------------------------------------------------------------------------
  // Claw
  //--------------------------------------------------------------------------------

  // close on 6U
  if (vexRT[Btn6U] || vexRT[Btn6UXmtr2]) {
    SensorValue[claw] = CLAW_CLOSE;
  }
  // open on 6D
  else if (vexRT[Btn6D] || vexRT[Btn6DXmtr2]) {
    SensorValue[claw] = CLAW_OPEN;
  }

  //--------------------------------------------------------------------------------
  // Mogo
  //--------------------------------------------------------------------------------

  // if  lift is below a certain height, dont use mogo
  if ( currLiftHeight > ARM_BLOCK_MOGO ) {
    // in on 7D
    if (vexRT[Btn5D]) {
      motor[mogo] = -127;
    }
    //out on 7U
    else if (vexRT[Btn5U]) {
      motor[mogo] = 127;
    }
    else {
      motor[mogo] = 0;
    }
  }
  else {
    motor[mogo] = 0;
  }

  //--------------------------------------------------------------------------------
  // Mogo flipper
  //--------------------------------------------------------------------------------

  if (vexRT[Btn8U]) {
    SensorValue[mogoFlip] = FLIPPER_IN;
  }
  else if (vexRT[Btn8D]) {
    SensorValue[mogoFlip] = FLIPPER_OUT;
  }

  //--------------------------------------------------------------------------------
  // Macro buttons
  //--------------------------------------------------------------------------------

  // floor height
  // release and go up then back to floor height after tard down
  // loader height

  lastLiftHeight = currLiftHeight;
  lastDeltaHeight = deltaHeight;

  wait1Msec(20);
}
