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
// bool rollerToggle = true;
// bool rollersIdle = true;
bool   bSwingManual = false;
bool rollerIdleIn = true;
bool   bSwingToggle = true;
int swingTime = nSysTime;
bool b7U2Toggle = true;
bool b7D2Toggle = true;
bool swingInToggle = false;
bool swingOutToggle = false;

swingPID.target = SensorValue[swingPot];
tardLift(0);
// everything for the drive in usrctrl is handled in this task.
startTask(driveSlew);
// startTask(swingPIDTask);
startTask(liftPIDTask);

bLCDBacklight = false;

// setLiftHeight(LIFT_SCHMEDIUM);
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
  if (vexRT[Btn8L]){
    setLiftHeight(LIFT_SCHMEDIUM);
    liftMacroSet = true;
  }
  // unset macros on these btns
  else if (vexRT[Btn8U] || vexRT[Btn8D]) {
    if (liftMacroSet) {
      tardLift(0);
      liftMacroSet = false;
    }
  }

  if (!liftMacroSet) {
    // up on 8U
    if (vexRT[Btn8U]) {
      tardLift(127);
      bLiftJustUsed = true;
    }
    // down on 8D
    else if (vexRT[Btn8D]) {
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
      if (vexRT[Btn8R]) bFlagRecordLift = true;
      // look for local extrema of the height function and record position there
      // if (sgn(deltaHeight) != sgn(lastDeltaHeight)) bFlagRecordLift = true;
      // if (deltaHeight == 0) bFlagRecordLift = true;
      // timeout after 500 ms
      // if (time1[T2] > 500) bFlagRecordLift = true;

      // disable conditions
      // only turn on PIDs if the lift is up
      // if (currLiftHeight < LIFT_BLOCK_MOGO) bFlagRecordLift = false;

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

  // if(vexRT[Btn6U] && vexRT[Btn6D]) {
  //   if(rollerToggle) {
  //     rollersIdle = !rollersIdle;
  //     rollerToggle = false;
  //   }
  // }
  // else{
  //   rollerToggle = true;
  // }

  // take higher precedence to outtake so you can hold intake and quick override outtake
  // out on 6D
  if (vexRT[Btn6D]) {
    motor[rollers] = ROLLERS_OUT;
    rollerIdleIn = false;
  }
  // in on 6U
  else if (vexRT[Btn6U]) {
    motor[rollers] = ROLLERS_IN;
    rollerIdleIn = true;
  }
  else {
    if(rollerIdleIn) {
      motor[rollers] = ROLLERS_HOLD;
    }
    else {
      motor[rollers] = 0;
    }
  }

  //--------------------------------------------------------------------------------
  // Swing
  //--------------------------------------------------------------------------------

  //switch between manual and PID for swing on 7L
  if (vexRT[Btn7L]) {
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
    // swing in on 7U
    if (vexRT[Btn7D]) {
      setSwing(127);
    }
    // swing out on 7D
    else if (vexRT[Btn7U]) {
      setSwing(-127);
    }
    else {
      // swing on Ch3
      setSwing(0);
    }
  }

  //////////////////////////////
  // otherwise use ?!not?PID? control
  //////////////////////////////
  else {
    ////////////////////////////toggle buttons
    // in on 7D
    if (vexRT[Btn7D]) {
      if(b7D2Toggle){
        swingTime = nSysTime;
        b7D2Toggle = false;
      }
      swingInToggle = true;
      swingOutToggle = false;
    }
    else {
      b7D2Toggle = true;
    }
    // out on 7U
    if (vexRT[Btn7U]) {
      if(b7U2Toggle){
        swingTime = nSysTime;
        b7U2Toggle = false;
      }
      swingInToggle = false;
      swingOutToggle = true;
    }
    else {
      b7U2Toggle = true;
    }
/////////////////////////macros
    if (swingInToggle) {
      if(nSysTime - swingTime < SWING_IN_TIME) {
        setSwing(127);
      }
      else {
        setSwing(SWING_HOLD_IN_POW);
      }
    }
    else if (swingOutToggle) {
      if(nSysTime - swingTime < SWING_OUT_TIME) {
        setSwing(-127);
      }
      else {
        setSwing(-SWING_HOLD_OUT_POW);
      }
    }

  }//END not manual control

  //--------------------------------------------------------------------------------
  // Mogo
  //--------------------------------------------------------------------------------

  // if lift is below a certain height, dont use mogo
  //if (currLiftHeight > LIFT_BLOCK_MOGO) {
    // in on 5U
    if (vexRT[Btn5U]) {
      motor[mogo] = 127;
      mogoGadget();
    }
    //out on 5D
    else if (vexRT[Btn5D]) {
      motor[mogo] = -127;
      nogoGadget();
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
