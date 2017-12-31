



clearTimer(T1);
SensorValue[driveLEnc] = 0;
SensorValue[driveREnc] = 0;
bool bSwingManual = true;
bool bSwingToggle = true;
string displayString;

armPID.target = getArmHeight();
// everything for the lift in usrctrl is handled in this task
// check if second controller is connected
if(nVexRCReceiveState & vrXmit2)
  startTask(usrCtrlArmPID2);
else
  startTask(usrCtrlArmPID1);

// everything for the drive in usrctrl handled in this task.
startTask(driveSlew);

swingPID.target = SensorValue[swingPot];
startTask(swingPIDTask);

currReceiveState = nVexRCReceiveState & vrXmit2;
while(true){
  // check if second controller is connected
  if(currReceiveState){
    if(lastReceiveState != currReceiveState){
      // new controller added, make changes
      stopTask(usrCtrlArmPID1);
      startTask(userCtrlArmPID2);
    }
    #include "usercontrol-2Controller.c"
  }
  else{
    if(lastReceiveState != currReceiveState){
      // new controller added, make changes
      stopTask(usrCtrlArmPID2);
      startTask(userCtrlArmPID1);
    }
    #include "usercontrol-singleController.c"
  }
  lastReceiveState = nVexRCReceiveState & vrXmit2;
}
