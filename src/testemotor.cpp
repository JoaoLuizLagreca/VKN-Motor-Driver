#include <stdio.h>
#include <unistd.h>
#include "libmotor.hpp"

int main(){
	motor::driver motor_drv;
	
	printf("Frente\n");
	motor_drv.forward();
	sleep(3);
	printf("Trás\n");
	motor_drv.backward();
	sleep(3);
	printf("Freio\n");
	motor_drv.stop();
	sleep(3);
	printf("Descanso\n");
	motor_drv.idle();
	
	motor_drv.free();
	return 0;
}
