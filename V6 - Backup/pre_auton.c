////////////////////////////////////////////////////////
//
// Auton selection code
//
////////////////////////////////////////////////////////

clearLCDLine(0);
clearLCDLine(1);
displayLCDCenteredString(0, "yeet");
bLCDBacklight = true;
short nLCDButtons_last = 0;
string autonSelectionString[NUM_AUTONS] = {"0: Default", "1: Blue Primary", "2: Red Primary", "3: Post Cone", "4: Ram"};

// only run if a button is pushed to prevent hanging up in a reset
if(nLCDButtons > 0){
  displayLCDCenteredString(0, "Auton Selection");
  displayLCDCenteredString(1, "Activated");
  //wait for button release
  while(nLCDButtons > 0)
    wait1Msec(10);

  while(nLCDButtons != CENTER_LCD){
    if			(nLCDButtons == RIGHT_LCD && nLCDButtons_last != RIGHT_LCD)	autonSelection++;
    else if	(nLCDButtons == LEFT_LCD 	&& nLCDButtons_last != LEFT_LCD)	autonSelection--;

    if			(autonSelection >= NUM_AUTONS) autonSelection = NUM_AUTONS - 1;
    else if (autonSelection < 0) 					 autonSelection = 0;
    displayLCDCenteredString(1, autonSelectionString[autonSelection]); // display the selected zone

    nLCDButtons_last = nLCDButtons;
    wait1Msec(10);
  }//end while

  clearLCDLine(0);
  clearLCDLine(1);
  //Reconfigure gyro Analog Port as a Gyro sensor and allow time for ROBOTC to calibrate it
  displayLCDCenteredString(0,"Init Calibration");
  displayLCDCenteredString(1,"Do not move bot");
  wait1Msec(1000);
  displayLCDCenteredString(0,"Calibrating...");
  SensorType[gyro] = sensorNone;
  wait1Msec(10);
  SensorType[gyro] = sensorGyro;
  wait1Msec(1300);
  //Adjust SensorScale to correct the scaling for your gyro
  SensorScale[gyro] = 138.5;//138.85;//138;
  clearLCDLine(0);
  clearLCDLine(1);

  displayLCDCenteredString(0,"RReady");
  bLCDBacklight = false;
}//end buttons > 0
else {
  displayLCDCenteredString(0, "No Auton");
  displayLCDCenteredString(1, "Selected");
}
initPIDVars();
