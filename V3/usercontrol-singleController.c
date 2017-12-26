////////////////////////////////////////////////////////
//
// Usercontrol with a single controller
//
////////////////////////////////////////////////////////

clearTimer(T1);
SensorValue[driveLEnc] = 0;
SensorValue[driveREnc] = 0;
bool bSwingManual = true;
bool bSwingToggle = true;
string displayString;

armPID.target = getArmHeight();
// everything for the lift in usrctrl is handled in this task
startTask(usrCtrl1ArmPID);

// everything for the drive in usrctrl handled in this task.
startTask(driveSlew);

swingPID.target = SensorValue[swingPot];
startTask(swingPIDTask);

while ("Kent is driving"){
  if("we haven't won yet")
    smackVcat();

  // Display battery level
  clearLCDLine(0);
  clearLCDLine(1);
  sprintf(displayString, "Main: %2.2fV", nAvgBatteryLevel/1000.);
  displayLCDCenteredString(0, displayString);
  sprintf(displayString, "Backup: %2.2fV", BackupBatteryLevel/1000.);
  displayLCDCenteredString(1, displayString);

  //--------------------------------------------------------------------------------
  // Swing
  //--------------------------------------------------------------------------------

  //switch between manual and PID for swing on 8L
  if(vexRT[Btn8L]){
    if(bSwingToggle){
      bSwingManual = !bSwingManual;
      bSwingToggle = false;
    }
  }
  else{
    bSwingManual = true;
  }

  // if manual control is selected
  if(bSwingManual){
    swingPID.enabled = false;
    // swing in on 8D
    if(vexRT[Btn8D]){
      motor[swing] = 127;
    }
    // swing out on 8U
    else if(vexRT[Btn8U]){
      motor[swing] = -127;
    }
    else{
      motor[swing] = 0;
    }
  }
  // otherwise use PID control
  else{
    swingPID.enabled = true;
    // in on 8D
    if(vexRT[Btn8D]){
      swingPID.target = SWING_IN;
    }
    // out on 8U
    else if(vexRT[Btn8U]){
      swingPID.target = SWING_OUT;
    }
    // 90 on 8R
    else if(vexRT[Btn8R]){
      swingPID.target = SWING_90;
    }
  }

  //--------------------------------------------------------------------------------
  // Claw
  //--------------------------------------------------------------------------------

  // close on 6U
  if(vexRT[Btn6U]){
    SensorValue[claw] = CLAW_CLOSE;
  }
  // open on 6D
  else if(vexRT[Btn6D]){
    SensorValue[claw] = CLAW_OPEN;
  }

  //--------------------------------------------------------------------------------
  // Mogo
  //--------------------------------------------------------------------------------

  // if arm is below a certain height, dont use mogo
  if( getArmHeight() > ARM_BLOCK_MOGO ){
    // in on 7D
    if(vexRT[Btn7D]){
      motor[mogo] = 127;
    }
    //out on 7U
    else if(vexRT[Btn7U]){
      motor[mogo] = -127;
    }
    else{
      motor[mogo] = 0;
    }
  }
  else{
    motor[mogo] = 0;
  }

  wait1Msec(20);

}//END while()
