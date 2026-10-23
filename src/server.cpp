/*
 * TODO: Detecção de erro das rodas e sensor de movimentação por encoder
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/stat.h>
#include "libmotor.hpp"

typedef char byte;

#define SOCKET_NAME "/var/run/motor.sock"
#define BUFFER_SIZE 5

#define FUNC_FORWARD  0x00 /* Move forward */
#define FUNC_BACKWARD 0x01 /* Move backwad */
#define FUNC_STOP     0x02 /* Break */
#define FUNC_IDLE     0x03 /* Set to idle */
#define FUNC_CLKWISE  0x04 /* Turn clockwise */
#define FUNC_CCKWISE  0x05 /* Turn counterclockwise */
#define FUNC_SETSPEED 0x06 /* Change speed */
#define FUNC_GETSPEED 0x07 /* Get defined speed */
#define FUNC_CHKRGTSP 0x08 /* Get right wheel speed from sensors */
#define FUNC_CHKLFTSP 0x08 /* Get left wheel speed from sensors */

void closure();
void treatData(byte *data);

struct sockaddr_un addr;
int listen_sock, data;
byte *buffer;
motor::driver motor_drv;

int main() {
	int ret;
	int bfs = BUFFER_SIZE * sizeof(byte);
	
	printf("Initializing motor driver server... \n");
	
	if (atexit(closure) != 0){
		perror("Error registering atexit");
		exit(EXIT_FAILURE);
	}
	
	unlink(SOCKET_NAME);
	
	listen_sock = socket(AF_UNIX, SOCK_SEQPACKET, 0);
	if (listen_sock == -1) {
		perror("Error creating server socket");
		exit(EXIT_FAILURE);
	}
	
	memset(&addr, 0, sizeof(struct sockaddr_un));
	addr.sun_family = AF_UNIX;
	strcpy(addr.sun_path, SOCKET_NAME);
	ret = bind(listen_sock, (const struct sockaddr *) &addr, 
		sizeof(struct sockaddr_un));
	if (ret == -1) {
		perror("Error binding socket");
		exit(EXIT_FAILURE);
	}
	
	ret = chmod(SOCKET_NAME, 
		S_IRUSR|S_IWUSR|
		S_IRGRP|S_IWGRP|
		S_IROTH|S_IWOTH
	);
	if (ret == -1) {
		perror("Error chmoding socket");
		exit(EXIT_FAILURE);
	}
	
	ret = listen(listen_sock, 5);
	if (ret == -1) {
		perror("Error listening to socket");
		exit(EXIT_FAILURE);
	}
	
	/* FIXME: Detectar erro */
	motor_drv.init();
	
	printf("Motor driver server started\n");
	
	while (true) {
			data = accept(listen_sock, NULL, NULL);
			if (data == -1) {
				perror("Error accepting data");
				exit(EXIT_FAILURE);
			}
			
			buffer = (byte*) malloc(bfs);
			while (true) {
				memset(buffer, 0, bfs);
				ret = recv(data, buffer, bfs, 0);
				if (ret == -1){
					perror("Error receiving data");
					break;
				}
				if (ret == 0){
					break;
				}
				
				treatData(buffer);
			}
			
			free(buffer);
			close(data);
	}
	
}

void closure() {
	printf("Closing...");
	close(listen_sock);
	unlink(SOCKET_NAME);
	
	/* FIXME: Verificar se o driver foi iniciado */
	motor_drv.idle();
	motor_drv.free();
	
	return;
}

void treatData(byte *data){
	printf("Received command %#04x\n", data[0]);
	
	switch(data[0]){
		case FUNC_FORWARD:
			motor_drv.forward();
		break;
		
		case FUNC_BACKWARD:
			motor_drv.backward();
		break;
		
		case FUNC_CLKWISE:
			motor_drv.clockwise();
		break;
		
		case FUNC_CCKWISE:
			motor_drv.counterclockwise();
		break; 
		
		case FUNC_STOP:
			motor_drv.stop();
		break;
		
		case FUNC_IDLE:
			motor_drv.idle();
		break;
	}
}
