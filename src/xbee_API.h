#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <xbee.h>
#include "pkt_Queue.h"

#ifdef xbee_Serial_H

extern char* xbee_device;
extern int xbee_baudrate;

#else

char* xbee_device;
int xbee_baudrate;

#endif


#ifndef xbee_API_H
#define xbee_API_H

char* xbee_mode;


extern char* xbee_device;

extern int xbee_baudrate;

//0:disable Log, 100:enable Log
int LogLevel;

unsigned char Local_Address[8];

/* A variable to get error code */
xbee_err ret;

/* A variable txRet get Tx return value */
//unsigned char txRet;

xbee_err xbee_initial(struct xbee** xbee, pkt_ptr pkt_Queue);

/* Connector for setup xbee connection
 *
 * conMode:
 *    "Data"        : Send and Receive Data
 *    "GetAddress"  : Get Local Address
 */
xbee_err xbee_connector(struct xbee** xbee, struct xbee_con** con, pkt_ptr pkt_Queue);

/* CallBack for Data Received */
void CallBack(struct xbee *xbee, struct xbee_con *con, struct xbee_pkt **pkt
, void **data);

#endif
