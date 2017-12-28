#include <stdio.h>
#include <stdlib.h>

//For xbee_setup
const char *mode = "xbeeZB";
char *device = "/dev/ttyAMA0";
int baudrate = 9600;
int LogLevel =  0; //0:disable Log, 100:enable Log
int Brocast = 1; 	//0:For Brocast or a Coordinator, 1:Send to cooedinator or a endpoint
int get_address = 3; //A flag for check if all part of address are get. 3 send first address 2 send second address 0 success
char* Local_Address = "";
/* queue for sending data */
struct pkt { 
	char *type;                 //"Data" or "AT"
	unsigned char address[8];   //Brocast:000000000000FFFF;Coordinator:0000000000000000
	char *content;              // "Data":Data;"AT":Command for dest device (the address you assigned) e.g for AT: NICoordinator
    struct pkt *next; 
}; 

typedef struct pkt pPkt; 

pPkt *front, *rear;

void init_Queue(); 
void addpkt(char *raw_addr, char *type,char *content); 
void delq();
void AddressToChar(char *raw,unsigned char addr[8]);
void AddressCopyHalf(char *raw,char *dest);
void myCB_Data(struct xbee *xbee, struct xbee_con *con, struct xbee_pkt **pkt, void **data);
void myCB_AT(struct xbee *xbee, struct xbee_con *con, struct xbee_pkt **pkt, void **data);
