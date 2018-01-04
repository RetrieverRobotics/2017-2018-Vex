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
static bool bSwingManual = true;
bool   bSwingToggle = true;
bool   bPrevPressed = false;
bool   bArmHeightRecorded = true;
bool   bFlagRecordArm = false;
int    armSetPow = 0;
int    currArmHeight = 0;
int    lastArmHeight = 0;
float  deltaHeight = 0;
float  lastDeltaHeight = 0;
// everything for the drive in usrctrl handled in this task.
startTask(driveSlew);
swingPID.target = SensorValue[swingPot];
startTask(swingPIDTask);
armPID.target = getArmHeight();
startTask(armPIDTask);


while (true) {
  // Display battery level
  clearLCDLine(0);
  clearLCDLine(1);
  sprintf(displayString, "Main: %2.2fV", nAvgBatteryLevel/1000.);
  displayLCDCenteredString(0, displayString);
  sprintf(displayString, "Backup: %2.2fV", BackupBatteryLevel/1000.);
  displayLCDCenteredString(1, displayString);

  currArmHeight = getArmHeight();

  //--------------------------------------------------------------------------------
  // Lift
  //--------------------------------------------------------------------------------

  // right joystick y axis on slave controller controls lift
  armSetPow = vexRT[Ch2Xmtr2];
  // hold position inside deabands
  if (armSetPow < ARM_DEADBAND) {
    if (bPrevPressed)
      tardLift(0);
    // only turn on PIDs if the arm is up
    if (currArmHeight > ARM_BLOCK_MOGO) {
      // clear timer once immediately after button release
      if (bPrevPressed) {
        clearTimer(T2);
        bArmHeightRecorded = false; // set up to turn on pid later
        lastDeltaHeight = currArmHeight - lastArmHeight;
        deltaHeight = lastDeltaHeight;
      }
      else {
        deltaHeight = currArmHeight - lastArmHeight;
      }

      // look for local extrema of the height function and record position there
      bFlagRecordArm = false;
      if (sgn(deltaHeight) != sgn(lastDeltaHeight))
        bFlagRecordArm = true;
      if (deltaHeight == 0)
        bFlagRecordArm = true;
      // timeout after 500 ms
      if (time1[T2] > 500)
        bFlagRecordArm = true;

      if (!bArmHeightRecorded && bFlagRecordArm) {
        // record height and turn armPID on with cross couple
        setArmHeight(currArmHeight);
        bArmHeightRecorded = true;
      }
      lastDeltaHeight = deltaHeight;
    }

    bPrevPressed = false;
  }
  else {
    tardLift(armSetPow);
    bPrevPressed = true;
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
      motor[swing] = 127;
    }
    // swing out on 7U
    else if (vexRT[Btn7UXmtr2]) {
      motor[swing] = -127;
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

  // if  arm is below a certain height, dont use mogo
  if ( currArmHeight > ARM_BLOCK_MOGO ) {
    // in on 7D
    if (vexRT[Btn5D]) {
      motor[mogo] = 127;
    }
    //out on 7U
    else if (vexRT[Btn5U]) {
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

  lastArmHeight = currArmHeight;
  wait1Msec(20);
}
