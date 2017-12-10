#include <stdio.h>
#include <stdlib.h>
#include <xbee.h>

//For xbee_setup
extern const char *mode = "xbeeZB";
extern char *device = "/dev/ttyAMA0";
extern int baudrate = 9600;
extern int LogLevel =  100; //0:disable Log, 100:enable Log
extern int Brocast = 1; //0:For Brocast or Coordinator, 1:Send to cooedinator
	