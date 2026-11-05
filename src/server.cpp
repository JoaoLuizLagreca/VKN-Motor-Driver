/*
 * TODO: Detecção de erro das rodas e sensor de movimentação por encoder
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/stat.h>
#include <driver.hpp>
#include <servermacros.h>

typedef unsigned char byte;

void closure();
void treatData(byte *data);

struct sockaddr_un addr;
int listen_sock, client;
byte *buffer;
motor::driver motor_drv;
byte drv_mode = DRIVER_MODE_BASIC;

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
			client = accept(listen_sock, NULL, NULL);
			if (client == -1) {
				perror("Error accepting data");
				continue;
			}
			
			buffer = (byte*) malloc(bfs);
			while (true) {
				memset(buffer, 0, bfs);
				ret = recv(client, buffer, bfs, 0);
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
			close(client);
	}
	
}

void closure() {
	printf("Closing...");
	close(listen_sock);
	unlink(SOCKET_NAME);
	
	/* FIXME: Verificar se o driver foi iniciado */
	motor_drv.setWheelMode(MOTOR_A, WHEEL_MODE_IDLE);
	motor_drv.setWheelMode(MOTOR_B, WHEEL_MODE_IDLE);
	motor_drv.free();
	
	return;
}

void treatData(byte *data){
	printf("Received command %#04x\n", data[0]);
	
	switch(data[0]){
		case FUNC_SETMODE:
			if(data[1] < 0x00 || data[1] > 0x01) {
				perror("Invalid driver mode");
				return;
			}

			drv_mode = data[1];
			printf("Setting functin mode to %#04x\n", data[1]);
		break;

		case FUNC_GETMODE:
			byte *r_buf = (byte *)malloc(sizeof(byte));
			r_buf[0] = drv_mode;

			int ret = send(client, r_buf, sizeof(char), MSG_EOR);
			if (ret == -1) {
				perror("GETMODE Error responding client");
			}

			free(r_buf);
		break;
	}
}
