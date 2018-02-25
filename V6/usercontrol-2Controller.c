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
bool   bSwingManual = false;
bool   bSwingToggle = true;
bool   bLiftJustUsed = false;
bool   bLiftHeightRecorded = false;
bool   bFlagRecordLift = false;
int    liftSetPow = 0;
int    currLiftHeight = getLiftHeight();
int    lastLiftHeight = currLiftHeight;
float  deltaHeight = 0;
float  lastDeltaHeight = 0;
bool rollerIdleIn = true;
int swingTime = 0;
bool b7U2Toggle = true;
bool b7D2Toggle = true;
bool liftMacroSet = false;
bool swingUpToggle = false;

swingPID.target = SensorValue[swingPot];
tardLift(0);
// everything for the drive in usrctrl handled in this task.
startTask(driveSlew);
// startTask(swingPIDTask);
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

  if (vexRT[Btn8LXmtr2]){
    setLiftHeight(LIFT_SCHMEDIUM);
    liftMacroSet = true;
  }
  else if(vexRT[Btn8DXmtr2]) {
    setLiftHeight(LIFT_SCHMEDIUM);
    liftMacroSet = true;
  }
  // unset macros on these btns
  else if (vexRT[Ch2Xmtr2] > LIFT_DEADBAND) {
    if (liftMacroSet) {
      tardLift(0);
      liftMacroSet = false;
    }
  }

  if(!liftMacroSet) {
    // right joystick y axis on slave controller controls lift
    liftSetPow = vexRT[Ch2Xmtr2];
    // hold position inside deabands
    if (fabs(liftSetPow) < LIFT_DEADBAND) {
      // clear timer once immediately after button release
      if (bLiftJustUsed) {
        clearTimer(T2);
        bLiftHeightRecorded = false; // set up to turn on pid later
        tardLift(0);
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
      if (currLiftHeight < LIFT_BLOCK_MOGO) bFlagRecordLift = false;

      if (!bLiftHeightRecorded && bFlagRecordLift) {
        setLiftHeight(currLiftHeight);
        bLiftHeightRecorded = true;
      }

      bLiftJustUsed = false;
    }
    else {
      tardLift(liftSetPow);
      bLiftJustUsed = true;
    }
  }//END liftmacroset

  //--------------------------------------------------------------------------------
  // Rollers
  //--------------------------------------------------------------------------------

  // in on 6U
  if (vexRT[Btn6U] || vexRT[Btn6UXmtr2]) {
    motor[rollers] = ROLLERS_IN;
    rollerIdleIn = true;
  }
  // out on 6D
  else if (vexRT[Btn6D] || vexRT[Btn6DXmtr2]) {
    motor[rollers] = ROLLERS_OUT;
    rollerIdleIn = false;
  }
  else {
    if(rollerIdleIn) {
      motor[rollers] = ROLLERS_HOLD;
    }
    else {
      motor[rollers] = -ROLLERS_HOLD;
    }
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
    // swing in on 7U
    if (vexRT[Btn7DXmtr2]) {
      motor[swing] = 127;
    }
    // swing out on 7D
    else if (vexRT[Btn7UXmtr2]) {
      motor[swing] = -127;
    }
    else {
      // swing on Ch3Xmtr2
      motor[swing] = -vexRT[Ch3Xmtr2];
    }
  }
  // otherwise use PID control
  else {
    // swingPID.enabled = true;
    // in on 7U
    if (vexRT[Btn7DXmtr2]) {
      if(b7U2Toggle){
        swingTime = nSysTime;
        b7U2Toggle = false;
      }
      swingUpToggle = true;
    }
    // out on 7D
    else if (vexRT[Btn7UXmtr2]) {
      if(b7D2Toggle){
        swingTime = nSysTime;
        b7D2Toggle = false;
      }
      swingUpToggle = false;
    }

    if(swingUpToggle) {
      if(nSysTime - swingTime < SWING_IN_TIME) {
        motor[swing] = 127;
      }
      else {
        motor[swing] = SWING_HOLD_POW;
      }
    }

    else {
      if(nSysTime - swingTime < SWING_OUT_TIME) {
        motor[swing] = -127;
      }
      else {
        motor[swing] = -SWING_HOLD_POW;
      }
    }

    if(!vexRT[Btn7UXmtr2]) {
      b7U2Toggle = true;
    }
    if (!vexRT[Btn7DXmtr2]) {
      b7D2Toggle = true;
    }
  }

  //--------------------------------------------------------------------------------
  // Mogo
  //--------------------------------------------------------------------------------

  // if  lift is below a certain height, dont use mogo
  if ( currLiftHeight > LIFT_BLOCK_MOGO ) {
    // in on 5U
    if (vexRT[Btn5U]) {
      motor[mogo] = 127;
    }
    //out on 5D
    else if (vexRT[Btn5D]) {
      motor[mogo] = -127;
    }
    else {
      motor[mogo] = 0;
    }
  }
  else {
    motor[mogo] = 0;
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
