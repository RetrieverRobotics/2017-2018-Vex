#pragma config(UserModel, "ports.c")

task main()
{
#define P1 83
	motor[driveLF] 	= 83;
	motor[driveLBT] = 83;
	motor[driveLBB] = 83;
#define P2 90
	motor[driveRF]  = P2;
	motor[driveRBT] = P2;
	motor[driveRBB] = P2;

	while(true){wait1Msec(1000);}


}
