///////////////////////////////////////////////////////////////////////////////
//
// Dynamic usercontrol decides which button layout to use based on whether
// the slave controller is connected.
//
///////////////////////////////////////////////////////////////////////////////

// everything that can be done agnostic of controller type
clearTimer(T1);
SensorValue[driveLEnc] = 0;
SensorValue[driveREnc] = 0;
bool bSwingManual = true;
bool bSwingToggle = true;
TVexReceiverState bSlaveAttached;
TVexReceiverState lastReceiveState = nVexRCReceiveState & vrXmit2;
string displayString;
// everything for the drive in usrctrl handled in this task.
startTask(driveSlew);
swingPID.target = SensorValue[swingPot];
startTask(swingPIDTask);
armPID.target = getArmHeight();

// controller specific initilizations
// check for slave controller
if (nVexRCReceiveState & vrXmit2) {
  // everything for the lift in usrctrl is handled in this task
  startTask(usrCtrlArmPID2);
}
else {
  // everything for the lift in usrctrl is handled in this task
  startTask(usrCtrlArmPID1);
}

while (true) {
  bSlaveAttached = nVexRCReceiveState & vrXmit2;

  // check if second controller is connected
  if (bSlaveAttached) {
    if(lastReceiveState != bSlaveAttached) {
      // new controller added, make changes
      stopTask(usrCtrlArmPID1);
      startTask(usrCtrlArmPID2);
    }
    #include "usercontrol-2Controller.c"
  }
  else {
    if (lastReceiveState != bSlaveAttached) {
      // controller removed, make changes
      stopTask(usrCtrlArmPID2);
      startTask(usrCtrlArmPID1);
    }
    #include "usercontrol-singleController.c"
  }
  lastReceiveState = nVexRCReceiveState & vrXmit2;
}//END while
