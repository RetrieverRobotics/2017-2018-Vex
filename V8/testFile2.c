#pragma config(UserModel, "ports.c")

task main()
{
#define L 85
	motor[driveFL] 	= L;
	motor[driveBL] = L;
#define R 90
	motor[driveFR]  = R;
	motor[driveBR] = R;

	while(true){wait1Msec(1000);}


}
