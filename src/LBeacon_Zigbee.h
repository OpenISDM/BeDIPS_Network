#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <xbee.h>
#include "./serial/Serial.h"
#include "./pkt/pkt_Queue.h"

#define xbee_mode "xbeeZB"

#define xbee_device "/dev/ttyAMA0"

#define xbee_baudrate 9600

//0:disable Log, 100:enable Log
#define LogLevel 100

//A 64-bit extended PAN ID for join Network(16 number)
#define PAN_ID "0000000000000000"

#define xbee_Serial_buffer 50

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
