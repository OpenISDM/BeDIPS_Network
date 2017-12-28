#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <xbee.h>
#include "setting.h"

int main(void) {
	struct xbee *xbee;
	struct xbee_con *con;
	struct xbee_conAddress address;
	struct xbee_conSettings settings;
	xbee_err ret;
	unsigned char txRet;

	/*-----------------------------Configuration for xbee---------------------------------*/
	/*------------------------------------------------------------------------------------*/

	/* xbee_setup(struct xbee **retXbee, const char *mode, char *device, int baudrate) */
	if ((ret = xbee_setup(&xbee, mode, device, baudrate)) != XBEE_ENONE) {
		printf("ret: %d (%s)\n", ret, xbee_errorToStr(ret));
		return ret;
	}

	/* Setup Log Level 0:disable Log, 100:enable Log */
	if ((ret = xbee_logLevelSet(xbee,LogLevel)) != XBEE_ENONE) {
		printf("ret: %d (%s)\n", ret, xbee_errorToStr(ret));
		return ret;
	}
	
	/*----------------------Configuration for connection in AT mode-----------------------*/
	/* In this mode we aim to get the address of xbee.                                    */
	/*------------------------------------------------------------------------------------*/
	
	/* Setup a default adrress. 
	 * If it is a coordinator, it might be brocast.
	 * If it is a router, it might be connect to coordinator as default.
	 */
	memset(&address, 0, sizeof(address));
	address.addr64_enabled = 1;
	if(Brocast == 1)
		AddressToChar("000000000000FFFF",address.addr64);

	/* create a new AT connection to the local XBee in order to get the address of the xbee. */
	if ((ret = xbee_conNew(xbee, &con, "Local AT", NULL)) != XBEE_ENONE) {
		xbee_log(xbee, -1, "xbee_conNew() returned: %d (%s)", ret, xbee_errorToStr(ret));
		return ret;
	}
	
	/*
	 * configure a callback for the connection of AT mode
	 * this function is called every time a packet for this connection is recieved 
	 */
	if ((ret = xbee_conCallbackSet(con, myCB_AT, NULL)) != XBEE_ENONE) {
		xbee_log(xbee, -1, "xbee_conCallbackSet() returned: %d", ret);
		return ret;
	}
	
	/* if settings.catchAll = 1, then all packets will receive */
	if (xbee_conSettings(con, NULL, &settings) != XBEE_ENONE) return ret; 
	settings.catchAll = 0; 
	
	if (xbee_conSettings(con, &settings, NULL) != XBEE_ENONE) return ret;
	
	if ((ret = xbee_conDataSet(con, xbee, NULL)) != XBEE_ENONE) {
		xbee_log(xbee, -1, "xbee_conDataSet() returned: %d", ret);
		return ret;
	}
	
	Local_Address = malloc(sizeof(char)*8);
	/* send the AT command 'SH','SL' */
	xbee_conTx(con, &txRet, "SH");
	printf("SH sended\n");
	usleep(500000);
	while(get_address != 0){
		if(get_address == 2){
			xbee_conTx(con, &txRet, "SL");
			printf("SL sended\n");
			get_address = 1;
		}
		usleep(500000);
	}
	
	/* shutdown the connection */
	if ((ret = xbee_conEnd(con)) != XBEE_ENONE) {
		xbee_log(xbee, -1, "xbee_conEnd() returned: %d", ret);
		return ret;
	}
	
	printf("The Address of this device is %s",Local_Address);

	/*--------------------Configuration for connection in Data mode-----------------------*/
	/* In this mode we aim to get Data.                                                   */
	/*------------------------------------------------------------------------------------*/
	if ((ret = xbee_conNew(xbee, &con, "Data", &address)) != XBEE_ENONE) {
		xbee_log(xbee, -1, "xbee_conNew() returned: %d (%s)", ret, xbee_errorToStr(ret));
		return ret;
	}
	
	/* Set CallBack Function to call myCB_Data if packet receive. */
	if ((ret = xbee_conCallbackSet(con, myCB_Data, NULL)) != XBEE_ENONE) {
		xbee_log(xbee, -1, "xbee_conCallbackSet() returned: %d", ret);
		return ret;
	}
	
	/* if settings.catchAll = 1, then all packets will receive */
	if (xbee_conSettings(con, NULL, &settings) != XBEE_ENONE) return ret; 
	settings.catchAll = 1; 

	if (xbee_conSettings(con, &settings, NULL) != XBEE_ENONE) return ret;
	
	if ((ret = xbee_conDataSet(con, xbee, NULL)) != XBEE_ENONE) {
		xbee_log(xbee, -1, "xbee_conDataSet() returned: %d", ret);
		return ret;
	}
	
	init_Queue();
	
	/* start the chain reaction! */
	while(1) {
		void *p;
		if ((ret = xbee_conCallbackGet(con, (xbee_t_conCallback*)&p)) != XBEE_ENONE) {
			xbee_log(xbee, -1, "xbee_conCallbackGet() returned: %d", ret);
			return ret;
		}
		if (p == NULL) break;
		printf("Start addpkt\n");
        addpkt("0000000000000000", "Data", "AABBAAA");
        printf("End addpkt\n");
		usleep(500000);
		if(front->next != NULL){
			printf("%s\n",front->next->content);
			xbee_conTx(con, NULL, front->next->content);
			usleep(5000000);
			delq();
		}
		usleep(500000);
	}
	
	/* Close connection */
	if ((ret = xbee_conEnd(con)) != XBEE_ENONE) {
		xbee_log(xbee, -1, "xbee_conEnd() returned: %d", ret);
		return ret;
	}
	
	/* Close xbee */
	xbee_shutdown(xbee);

	return 0;
}

void AddressToChar(char *raw,unsigned char addr[8]){

	sscanf(raw, "%2x%2x%2x%2x%2x%2x%2x%2x", &addr[0], &addr[1], &addr[2], &addr[3],
											&addr[4], &addr[5], &addr[6], &addr[7]   );
}

void AddressCopyHalf(char *raw,char *dest){
	memcpy(dest,raw,4);
	printf("raw Address Copy to dest Address\n");
	printf("raw: %2x%2x%2x%2x\n", raw[0], raw[1], raw[2], raw[3]);
	printf("dest: %2x%2x%2x%2x\n", dest[0], dest[1], dest[2], dest[3]);
}

/* 
 * this is the callback function...
 * it will be executed once for each packet that is recieved on an associated connection 
 */
 //A flag for check if all part of address are get. 2 send first address 1 send second address 0 success
void myCB_AT(struct xbee *xbee, struct xbee_con *con, struct xbee_pkt **pkt, void **data) {
	if ((*pkt)->dataLen == 0) {
		printf("too short...\n");
		return;
	}
	if(strcmp(((*pkt)->conType),("Local AT"))==0){
		if(get_address == 3){
			AddressCopyHalf(((*pkt)->data),Local_Address);
			get_address = 2;
		}else if(get_address == 1){
			AddressCopyHalf(((*pkt)->data),&Local_Address[4]);
			printf("Local_Address: %2x%2x%2x%2x%2x%2x%2x%2x\n", Local_Address[0], Local_Address[1], Local_Address[2], Local_Address[3], Local_Address[4], Local_Address[5], Local_Address[6], Local_Address[7]);
			get_address = 0;
		}
	}
}

/* 
 * this is the callback function...
 * it will be executed once for each packet that is recieved on an associated connection 
 * if settings.catchAll = 1 then it will be executed for every packet.
 */
void myCB_Data(struct xbee *xbee, struct xbee_con *con, struct xbee_pkt **pkt, void **data) {
	if ((*pkt)->dataLen > 0) {
		/* if data[0] == '@', callback will be end. */ 
		if ((*pkt)->data[0] == '@') {
			xbee_conCallbackSet(con, NULL, NULL);
			printf("*** DISABLED CALLBACK... ***\n");
		}
		
		xbee_log(xbee, -1, "rx: [%s]\n", (*pkt)->data);
		
		/* 
		 * If data is received, how to deal with the data. 
		 */
		
	}
	/* printf("tx: %d\n", xbee_conTx(con, NULL, "Succcess\r\n")); */
}
	
void init_Queue() { 
    front = rear = (pPkt*) malloc(sizeof(pPkt)); 
    front->next = rear->next = NULL; 
} 

/* A function for create new packet in queue */
void addpkt(char *raw_addr, char *type,char *content ) {
    pPkt *newpkt; 
    newpkt = (pPkt*) malloc(sizeof(pPkt)); 

    if(front->next == NULL) {
    	front->next = newpkt;
	}
	
	newpkt -> type = type;
	AddressToChar(raw_addr,newpkt->address);
	newpkt -> content = content;
    newpkt->next = NULL; 
    rear->next = newpkt; 
    rear = newpkt; 
} 

/* A function for delete a sended Packet in queue */
void delq() { 
    pPkt* tmpnode; 

    if(front->next == NULL) { 
        printf("\n佇列已空！"); 
        return; 
    } 

    tmpnode = front->next; 
    front->next = tmpnode->next; 
    free(tmpnode);
} 
