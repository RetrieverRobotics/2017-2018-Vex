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
int    currLiftHeight = getLiftHeight();
int    lastLiftHeight = currLiftHeight;
float  deltaHeight = 0;
float  lastDeltaHeight = 0;
bool rollerIdleIn = true;
int swingTime = nSysTime;
bool b7U2Toggle = true;
bool b7D2Toggle = true;
bool liftMacroSet = false;
bool swingInToggle = false;
bool swingOutToggle = false;
bool bRollerMacro = false;

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
  // Macro buttons
  //--------------------------------------------------------------------------------
  // if (!bSwingManual && vexRT[Btn7RXmtr2]) {
  //   if(b7R2Toggle){
  //     startTask(stackCone);
  //     b7R2Toggle = false;
  //   }
  //   swingInToggle = false;
  //   swingOutToggle = false;
  //
  //   liftMacroSet = true;
  //   bRollerMacro = true;
  // }
  // else{
  //   b7R2Toggle = true;
  // }

  // floor height
  // release and go up then back to floor height after tard down
  // loader height

  //--------------------------------------------------------------------------------
  // Lift
  //--------------------------------------------------------------------------------

  // go to schmedium
  if (vexRT[Btn8LXmtr2]){
    setLiftHeight(LIFT_SCHMEDIUM);
    liftMacroSet = true;
  }
  //go to floor height
  // else if(vexRT[Btn8DXmtr2]) {
  //   setLiftHeight(LIFT_FLOOR_HEIGHT);
  //   liftMacroSet = true;
  // }
  //hold lift down const pow
  else if(vexRT[Btn5DXmtr2]) {
    tardLift(LIFT_HOLD_DOWN_POW);
    liftMacroSet = true;
  }
  // hold position
  else if(vexRT[Btn8RXmtr2]) {
    setLiftHeight(currLiftHeight);
    liftMacroSet = true;
  }
  // unset macros on these btns
  else if (abs(vexRT[Ch2Xmtr2]) > LIFT_DEADBAND) {
    if (liftMacroSet) {
      tardLift(0);
      liftMacroSet = false;
    }
  }

  if(!liftMacroSet) {
    // right joystick y axis on slave controller controls lift
    tardLift(vexRT[Ch2Xmtr2]);
  }//END liftmacroset

  //--------------------------------------------------------------------------------
  // Rollers
  //--------------------------------------------------------------------------------
  if (vexRT[Btn6UXmtr2] || vexRT[Btn6DXmtr2]){
    bRollerMacro = false;
  }

  if (!bRollerMacro) {
    // take higher precedence to outtake so you can hold intake and quick override outtake
    // out on 6D
    if (vexRT[Btn6DXmtr2]) {
      motor[rollers] = ROLLERS_OUT;
      rollerIdleIn = false;
    }
    // in on 6U
    else if (vexRT[Btn6UXmtr2]) {
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
  }
  //--------------------------------------------------------------------------------
  // Swing
  //--------------------------------------------------------------------------------

  //switch between manual and PID for swing on 7L
  if (vexRT[Btn7LXmtr2]) {
    // if (bSwingToggle) {
      // bSwingManual = !bSwingManual;
      bSwingManual = false;
    //   bSwingToggle = false;
    // }
  }
  else if (vexRT[Btn7RXmtr2]) {
    bSwingManual = true;
    // bSwingToggle = true;
  }

  // if  manual control is selected
  if (bSwingManual) {
    swingPID.enabled = false;
    // swing in on 7U
    if (vexRT[Btn7DXmtr2]) {
      setSwing(127);
    }
    // swing out on 7D
    else if (vexRT[Btn7UXmtr2]) {
      setSwing(-127);
    }
    else {
      // swing on Ch3Xmtr2
      setSwing(deadband(-vexRT[Ch3Xmtr2], 15));
    }
  }

  //////////////////////////////
  // otherwise use ?!not?PID? control
  //////////////////////////////
  else {
    ////////////////////////////toggle buttons
    // in on 7D
    if (vexRT[Btn7DXmtr2]) {
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
    if (vexRT[Btn7UXmtr2]) {
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

  if(vexRT[Btn6U]) {
    mogoGadget();
  }
  else if(vexRT[Btn6D]){
    nogoGadget();
  }

  // if  lift is below a certain height, dont use mogo
  // if ( currLiftHeight > LIFT_BLOCK_MOGO ) {
    // in on 5U
    if (vexRT[Btn5U]) {
      motor[mogo] = 127;
      // only close pneumatics if not holding the open button
      if(!vexRT[Btn6D]){
        mogoGadget();
      }
    }
    //out on 5D
    else if (vexRT[Btn5D]) {
      motor[mogo] = -127;
      // only open pneumatics if not holding the open button
      if(!vexRT[Btn6U]){
        nogoGadget();
      }
    }
    else {
      motor[mogo] = 0;
    }
  // }
  // else {
  //   motor[mogo] = 0;
  // }

  lastLiftHeight = currLiftHeight;
  lastDeltaHeight = deltaHeight;

  wait1Msec(20);
}
