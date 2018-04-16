#pragma config(UserModel, "ports.c")

task main()
{
#define P1 83
	motor[driveLF] 	= P1;
	motor[driveLBT] = P1;
	motor[driveLBB] = P1;
#define P2 90
	motor[driveRF]  = P2;
	motor[driveRBT] = P2;
	motor[driveRBB] = P2;

	while(true){wait1Msec(1000);}


}
