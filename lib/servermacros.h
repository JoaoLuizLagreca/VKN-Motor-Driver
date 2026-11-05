#define SOCKET_NAME "/var/run/motor.sock"
#define BUFFER_SIZE 5

/* Function modes */
#define DRIVER_MODE_BASIC 0x00
#define DRIVER_MODE_ADVCD 0x01

/* Basic controls */
#define FUNC_SETMODE      0x00 /* Set function mode */
#define FUNC_GETMODE      0x01 /* Get function mode */
#define FUNC_FORWARD      0x02 /* Move forward */
#define FUNC_BACKWARD     0x03 /* Move backwad */
#define FUNC_STOP         0x04 /* Break */
#define FUNC_IDLE         0x05 /* Set to idle */
#define FUNC_CLKWISE      0x06 /* Turn clockwise */
#define FUNC_CCKWISE      0x07 /* Turn counterclockwise */
#define FUNC_SETSPEED     0x08 /* Change speed */
#define FUNC_GETSPEED     0x09 /* Get defined speed */
#define FUNC_SETANGLE     0x10 /* Set turn angle */
#define FUNC_GETANGLE     0x11 /* Get turn angle */

/* Advanced controls */
#define FUNC_CHKSPEED     0x12 /* Get left and right wheel speed from sensors */
#define FUNC_SLWSPEED     0x13 /* Set left wheel speed */
#define FUNC_GLWSPEED     0x14 /* Get left wheel speed */
#define FUNC_SRWSPEED     0x15 /* Set right wheel speed */
#define FUNC_GRWSPEED     0x16 /* Get right wheel speed */
#define FUNC_SLWMODE      0x17 /* Set left wheel mode */
#define FUNC_GLWMODE      0x18 /* Get left wheel mode */
#define FUNC_SRWMODE      0x19 /* Set right wheel mode */
#define FUNC_GRWMODE      0x20 /* Get right wheel mode */
