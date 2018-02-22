////////////////////////////////////////////////////////
//
// Usercontrol with a single controller.
// For skills.
//
////////////////////////////////////////////////////////

clearTimer(T1);
clearTimer(T2);
SensorValue[driveLEnc] = 0;
SensorValue[driveREnc] = 0;
string displayString;
bool   bLiftJustUsed = false;
bool   bLiftHeightRecorded = false;
bool   bFlagRecordLift = false;
bool   liftMacroSet = false;
int    currLiftHeight = getLiftHeight();
int    lastLiftHeight = currLiftHeight;
float  deltaHeight = 0;
float  lastDeltaHeight = 0;
swingPID.target = SensorValue[swingPot];
tardLift(0);
// everything for the drive in usrctrl is handled in this task.
startTask(driveSlew);
startTask(swingPIDTask);
startTask(liftPIDTask);

bLCDBacklight = false;

// setLiftHeight(ARM_SCHMEDIUM);
while (true) {
  // tankDrive(vexRT[Ch3], vexRT[Ch2]);

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

  // this btn starts a lift macro to raise to schmedium height
  if (vexRT[Btn7L]){
    setLiftHeight(ARM_SCHMEDIUM);
    liftMacroSet = true;
  }
  // unset macros on these btns
  else if (vexRT[Btn7U] || vexRT[Btn7D]) {
    if (liftMacroSet) {
      tardLift(0);
      liftMacroSet = false;
    }
  }

  if (!liftMacroSet) {
    // up on 5U
    if (vexRT[Btn7U]) {
      tardLift(127);
      bLiftJustUsed = true;
    }
    // down on 5D
    else if (vexRT[Btn7D]) {
      tardLift(-127);
      bLiftJustUsed = true;
    }
    else {
      // clear timer once immediately after button release
      if (bLiftJustUsed) {
        clearTimer(T2);
        bLiftHeightRecorded = false; // set up to turn on pid later
        tardLift(0);
      }

      // check conditions to set lift height
      bFlagRecordLift = false;
      // enable conditions
      if (vexRT[Btn7R]) bFlagRecordLift = true;
      // look for local extrema of the height function and record position there
      // if (sgn(deltaHeight) != sgn(lastDeltaHeight)) bFlagRecordLift = true;
      // if (deltaHeight == 0) bFlagRecordLift = true;
      // timeout after 500 ms
      // if (time1[T2] > 500) bFlagRecordLift = true;

      // disable conditions
      // only turn on PIDs if the lift is up
      // if (currLiftHeight < ARM_BLOCK_MOGO) bFlagRecordLift = false;

      if (!bLiftHeightRecorded && bFlagRecordLift) {
        setLiftHeight(currLiftHeight);
        bLiftHeightRecorded = true;
      }

      bLiftJustUsed = false;
    }
  }//END liftMacroSet

  //--------------------------------------------------------------------------------
  // Rollers
  //--------------------------------------------------------------------------------

  // in on 8D
  if (vexRT[Btn8D]){
    motor[rollers] = -60;
  }
  // out on 8U
  else if (vexRT[Btn8U]) {
    motor[rollers] = 127;
  }
  else {
    motor[swing] = 0;
  }

  //--------------------------------------------------------------------------------
  // Swing
  //--------------------------------------------------------------------------------

  // close on 6U
  if (vexRT[Btn6U]) {
    SensorValue[swing] = SWING_IN;
  }
  // open on 6D
  else if (vexRT[Btn6D]) {
    SensorValue[swing] = SWING_OUT;
  }

  //--------------------------------------------------------------------------------
  // Mogo
  //--------------------------------------------------------------------------------

  // if lift is below a certain height, dont use mogo
  //if (currLiftHeight > ARM_BLOCK_MOGO) {
    // in on 7D
    if (vexRT[Btn5U]) {
      motor[mogo] = -127;
    }
    //out on 7U
    else if (vexRT[Btn5D]) {
      motor[mogo] = 127;
    }
    else {
      motor[mogo] = 0;
    }
  //}
  //else {
  //  motor[mogo] = 0;
  //}

  lastLiftHeight = currLiftHeight;
  lastDeltaHeight = deltaHeight;

  wait1Msec(20);
}
