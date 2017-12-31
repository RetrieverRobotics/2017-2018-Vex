////////////////////////////////////////////////////////
//
// Usercontrol utilizing the slave controller
//
////////////////////////////////////////////////////////

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

//switch between manual and PID for swing on 7L
if(vexRT[Btn7LXmtr2]){
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
  // swing in on 7D
  if(vexRT[Btn7DXmtr2]){
    motor[swing] = 127;
  }
  // swing out on 7U
  else if(vexRT[Btn7UXmtr2]){
    motor[swing] = -127;
  }
  else{
    motor[swing] = 0;
  }
}
// otherwise use PID control
else{
  swingPID.enabled = true;
  // in on 7D
  if(vexRT[Btn7DXmtr2]){
    swingPID.target = SWING_IN;
  }
  // out on 7U
  else if(vexRT[Btn7UXmtr2]){
    // SensorValue[claw] = CLAW_OPEN;
    swingPID.target = SWING_OUT;
  }
  // 90 on 7R
  else if(vexRT[Btn7RXmtr2]){
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
  if(vexRT[Btn5D]){
    motor[mogo] = 127;
  }
  //out on 7U
  else if(vexRT[Btn5U]){
    motor[mogo] = -127;
  }
  else{
    motor[mogo] = 0;
  }
}
else{
  motor[mogo] = 0;
}

//--------------------------------------------------------------------------------
// Macro buttons
//--------------------------------------------------------------------------------

// floor height
// release and go up then back to floor height after tard down
// loader height

wait1Msec(20);
