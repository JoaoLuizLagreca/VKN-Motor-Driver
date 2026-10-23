#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>

#define SOCKET_NAME "/var/run/motor.sock"

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
