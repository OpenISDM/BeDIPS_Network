#include <stdio.h>
#include <stdlib.h>
#include <xbee.h>

//For xbee_setup
extern const char *mode = "xbeeZB";
extern char *device = "/dev/ttyAMA0";
extern int baudrate = 9600;
extern int LogLevel =  100; //0:disable Log, 100:enable Log
extern int Brocast = 1; //0:For Brocast or Coordinator, 1:Send to cooedinator


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
void myCB(struct xbee *xbee, struct xbee_con *con, struct xbee_pkt **pkt, void **data);
