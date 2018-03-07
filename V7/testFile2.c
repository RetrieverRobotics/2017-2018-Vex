#pragma config(UserModel, "ports.c")

task main()
{

	motor[driveLF] 	= 83;
	motor[driveLBT] = 83;
	motor[driveLBB] = 83;

	motor[driveRF]  = 90;
	motor[driveRBT] = 90;
	motor[driveRBB] = 90;

	while(true){wait1Msec(1000);}


}
