#include <stdio.h>
#include <stdlib.h>
#include <xbee.h>

//For xbee_setup

const char *mode = "xbeeZB";

char *device = "/dev/ttyAMA0";

int baudrate = 9600;

//0:disable Log, 100:enable Log
int LogLevel =  0; 

// A flag for check if all part of address are get. 
// 3 send first address 
// 2 send second address 
// 0 success
int get_address = 3; 		

char* Local_Address = "";

/* packet format in the Queue */
struct pkt { 
	
	//"Data"
	char *type;
	
	// Brocast:     000000000000FFFF;
	// Coordinator: 0000000000000000              
	char *address;
	
	// Data
	char *content;          
    	struct pkt *next; 
}; 

typedef struct pkt pPkt; 

pPkt *front, *rear;

/* Connector for setup xbee connection 
 *
 * conMode:
 *    "Data"        : Send and Receive Data
 *    "GetAddress" `: Get Local Address
 */
xbee_err xbee_connector(struct xbee **xbee,	struct xbee_con **con, char *conMode);

/* Create Packet Queue Header */
void init_Packet_Queue();

/* Add new Packet to the end of Queue */ 
void addpkt(char *raw_addr, char *type,char *content);

/* Delete the end of Queue */ 
void delq();

/* Fill the address from raw(char) to addr(Hex) */
void Fill_Address(char *raw,unsigned char addr[8]);

/* Copy raw(char) to dest(char) for addpkt */
void AddressCopy(char *raw,char *dest,int size);

/* CallBack for Data Received */
void CallBack(struct xbee *xbee, struct xbee_con *con, struct xbee_pkt **pkt, void **data);

/* CallBack for Get Local Address */
void CallBack_for_get_address(struct xbee *xbee, struct xbee_con *con, struct xbee_pkt **pkt, void **data);
