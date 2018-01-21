////////////////////////////////////////////////////////
//
// Usercontrol with a single controller
// gives basic control enough to maneuver the robot.
// Not intended for competition use.
//
////////////////////////////////////////////////////////

clearTimer(T1);
clearTimer(T2);
SensorValue[driveLEnc] = 0;
SensorValue[driveREnc] = 0;
string displayString;
static bool bSwingManual = true;
bool   bSwingToggle = true;
bool   bJustPressed = false;
bool   bArmHeightRecorded = false;
bool   bFlagRecordArm = false;
bool   liftMacroSet = true;
int    currArmHeight = getArmHeight();
int    lastArmHeight = currArmHeight;
float  deltaHeight = 0;
float  lastDeltaHeight = 0;
// everything for the drive in usrctrl is handled in this task.
swingPID.target = SensorValue[swingPot];
armPID.target = getArmHeight();
startTask(driveSlew);
startTask(swingPIDTask);
startTask(armPIDTask);

// setArmHeight(ARM_SCHMEDIUM);
while (true) {
  // Display battery level
  clearLCDLine(0);
  clearLCDLine(1);
  sprintf(displayString, "Main: %2.2fV", nAvgBatteryLevel/1000.);
  displayLCDCenteredString(0, displayString);
  sprintf(displayString, "Backup: %2.2fV", BackupBatteryLevel/1000.);
  displayLCDCenteredString(1, displayString);

  currArmHeight = getArmHeight();
  deltaHeight = currArmHeight - lastArmHeight;

  //--------------------------------------------------------------------------------
  // Lift
  //--------------------------------------------------------------------------------

  // this btn starts a lift macro to raise to schmedium height
  if (vexRT[Btn7L]){
    setArmHeight(ARM_SCHMEDIUM);
    liftMacroSet = true;
  }
  // unset macros on these btns
  else if (vexRT[Btn7U] || vexRT[Btn7D]) {
    tardLiftStraight(0);
    liftMacroSet = false;
  }

  if (!liftMacroSet) {
    // up on 5U
    if (vexRT[Btn7U]) {
      tardLiftStraight(127);
      bJustPressed = true;
    }
    // down on 5D
    else if (vexRT[Btn7D]) {
      tardLiftStraight(-127);
      bJustPressed = true;
    }
    else {
      if (!bArmHeightRecorded || bJustPressed)
        tardLiftStraight(0);

      // only turn on PIDS if the arm is up
      if (currArmHeight > ARM_BLOCK_MOGO) {
        // clear timer once immediately after button release
        if (bJustPressed) {
          clearTimer(T2);
          bArmHeightRecorded = false; // set up to turn on pid later
        }

        // look for local extrema of the height function and record position there
        bFlagRecordArm = false;
        // if (!bJustPressed) {
        if (sgn(deltaHeight) != sgn(lastDeltaHeight))
          bFlagRecordArm = true;
        if (deltaHeight == 0)
          bFlagRecordArm = true;
        // timeout after 500 ms
        if (time1[T2] > 500)
          bFlagRecordArm = true;

        if (!bArmHeightRecorded && bFlagRecordArm) {
          // record height and turn armPID on with cross couple
          // writeDebugStreamLine("armset");
          // setArmHeight(currArmHeight);
          bArmHeightRecorded = true;
        }
      }

      bJustPressed = false;
    }
  }

  //--------------------------------------------------------------------------------
  // Swing
  //--------------------------------------------------------------------------------

  // switch between manual and PID for swing on 8L
  if (vexRT[Btn8L]) {
    if (bSwingToggle) {
      bSwingManual = !bSwingManual;
      bSwingToggle = false;
    }
  }
  else {
    bSwingToggle = true;
  }

  // if manual control is selected
  if (bSwingManual) {
    swingPID.enabled = false;
    // swing in on 8D
    if (vexRT[Btn8D]){
      motor[swing] = 127;
    }
    // swing out on 8U
    else if (vexRT[Btn8U]) {
      motor[swing] = -127;
    }
    else {
      motor[swing] = 0;
    }
  }
  // otherwise use PID control
  else {
    swingPID.enabled = true;
    // in on 8D
    if (vexRT[Btn8D]) {
      swingPID.target = SWING_IN;
    }
    // out on 8U
    else if (vexRT[Btn8U]) {
      swingPID.target = SWING_OUT;
    }
    // 90 on 8R
    else if (vexRT[Btn8R]) {
      swingPID.target = SWING_90;
    }
  }

  //--------------------------------------------------------------------------------
  // Claw
  //--------------------------------------------------------------------------------

  // close on 6U
  if (vexRT[Btn6U]) {
    SensorValue[claw] = CLAW_CLOSE;
  }
  // open on 6D
  else if (vexRT[Btn6D]) {
    SensorValue[claw] = CLAW_OPEN;
  }

  //--------------------------------------------------------------------------------
  // Mogo
  //--------------------------------------------------------------------------------

  // if arm is below a certain height, dont use mogo
  if (currArmHeight > ARM_BLOCK_MOGO) {
    // in on 7D
    if (vexRT[Btn5U]) {
      motor[mogo] = 127;
    }
    //out on 7U
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

  lastArmHeight = currArmHeight;
  lastDeltaHeight = deltaHeight;

  wait1Msec(20);
}
