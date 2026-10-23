#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include "servermacros.h"

typedef char byte;

struct sockaddr_un addr;
int talk_sock;


void sendCommand(byte comm){
	int ret;
	byte *buffer;
	buffer = (byte*) malloc(sizeof(byte));
	buffer[0] = comm;
	
	send(talk_sock, buffer, sizeof(byte), MSG_EOR);
	
	free(buffer);
}

int main(){
	int ret;
	
	talk_sock = socket(AF_UNIX, SOCK_SEQPACKET, 0);
	if(talk_sock == -1) {
		perror("Error creating client socket");
		exit(EXIT_FAILURE);
	}
	
	memset(&addr, 0, sizeof(struct sockaddr_un));
	addr.sun_family = AF_UNIX;
	strcpy(addr.sun_path, SOCKET_NAME);
	
	ret = connect(talk_sock, (const struct sockaddr *) &addr,
		sizeof(struct sockaddr_un));
	if (ret == -1) {
		perror("Error connecting to server socket");
		exit(EXIT_FAILURE);
	}
	
	printf("Frente\n");
	sendCommand(FUNC_FORWARD);
	sleep(3);
	printf("Trás\n");
	sendCommand(FUNC_BACKWARD);
	sleep(3);
	printf("Freio\n");
	sendCommand(FUNC_STOP);
	sleep(3);
	printf("Descanso\n");
	sendCommand(FUNC_IDLE);
	
	close(talk_sock);
	return 0;
}
