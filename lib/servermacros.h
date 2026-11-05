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
