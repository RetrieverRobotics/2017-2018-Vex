#pragma config(UserModel, "ports.c")

task main()
{
	while(1){
		motor[swing] = vexRT[Ch2];
	}


}
