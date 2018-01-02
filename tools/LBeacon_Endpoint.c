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
	
	init_Queue();
	
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
	
	xbee_connector(&xbee,&con,"Local AT");
	
	Local_Address = malloc(sizeof(char)*8);
	/* send the AT command 'SH','SL' */
	xbee_conTx(con, &txRet, "SH");
	printf("SH sended\n");
	usleep(50000);
	while(get_address != 0){
		if(get_address == 2){
			xbee_conTx(con, &txRet, "SL");
			printf("SL sended\n");
			get_address = 1;
		}
		usleep(50000);
	}
	
	/* shutdown the connection */
	if ((ret = xbee_conEnd(con)) != XBEE_ENONE) {
		xbee_log(xbee, -1, "xbee_conEnd() returned: %d", ret);
		return ret;
	}
	
	/*--------------------Configuration for connection in Data mode-----------------------*/
	/* In this mode we aim to get Data.                                                   */
	/*------------------------------------------------------------------------------------*/
	
	xbee_connector(&xbee,&con,"Data");
		
	/* start the chain reaction! */
	while(1) {
		void *p;
		if ((ret = xbee_conCallbackGet(con, (xbee_t_conCallback*)&p)) != XBEE_ENONE) {
			xbee_log(xbee, -1, "xbee_conCallbackGet() returned: %d", ret);
			return ret;
		}
		if (p == NULL) break;
		printf("Start addpkt\n");
        
		addpkt("0013A20040E2E01D", "Data", "AABBAAA");   //Test Data

        printf("End addpkt\n");
		if(front->next != NULL){
			/* shutdown the connection */
			if ((ret = xbee_conEnd(con)) != XBEE_ENONE) {
				xbee_log(xbee, -1, "xbee_conEnd() returned: %d", ret);
				return ret;
			}
			xbee_connector(&xbee,&con,"Data");
			printf("//////Packet Information//////\n");
			printf("Dest Address: %2x%2x%2x%2x%2x%2x%2x%2x\n",front->next->address[0],front->next->address[1],front->next->address[2],front->next->address[3],front->next->address[4],front->next->address[5],front->next->address[6],front->next->address[7]);
			printf("%s\n",front->next->type);
			printf("%s\n",front->next->content);
			xbee_conTx(con, NULL, front->next->content);
			usleep(500000);
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

xbee_err xbee_connector(struct xbee **xbee,	struct xbee_con **con, char *conMode){
	struct xbee_conAddress address;
	struct xbee_conSettings settings;
	xbee_err ret;
	
	/* Setup a default adrress. 
	 * If it is a coordinator, it might be brocast.
	 * If it is a router, it might be connect to coordinator as default.
	 */
	memset(&address, 0, sizeof(address));
	address.addr64_enabled = 1;	
	if((strcmp(conMode,"Local AT") != 0)&&(front->next != NULL)){
		printf("Specific Address\n");
		char* tmp = (front->next->address);
		printf("The address we decide to set is %c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c.\n",tmp[0],tmp[1],tmp[2],tmp[3],tmp[4],tmp[5],tmp[6],tmp[7],tmp[8],tmp[9],tmp[10],tmp[11],tmp[12],tmp[13],tmp[14],tmp[15]);
		Fill_Address(front->next->address,address.addr64);
	}
	else if((front->next == NULL)||(strcmp(conMode,"Local AT") == 0)){
		if(Brocast == 1)
			Fill_Address("000000000000FFFF",address.addr64);
	}

	printf("The address we set is %2x%2x%2x%2x%2x%2x%2x%2x.\n", address.addr64[0], address.addr64[1], address.addr64[2], address.addr64[3], address.addr64[4], address.addr64[5], address.addr64[6], address.addr64[7]);
	
	if(strcmp(conMode,"Local AT") == 0){
		/* create a new AT connection to the local XBee in order to get the address of the xbee. */
		if ((ret = xbee_conNew(*xbee, con, conMode, NULL)) != XBEE_ENONE) {
			xbee_log(*xbee, -1, "xbee_conNew() returned: %d (%s)", ret, xbee_errorToStr(ret));
			return ret;
		}
	
		/*
	 	 * configure a callback for the connection of AT mode
	 	 * this function is called every time a packet for this connection is recieved 
	 	 */
		if ((ret = xbee_conCallbackSet(*con, CallBack_for_get_address, NULL)) != XBEE_ENONE) {
			xbee_log(*xbee, -1, "xbee_conCallbackSet() returned: %d", ret);
			return ret;
		}
	}
	else if(strcmp(conMode,"Data") == 0){
		if ((ret = xbee_conNew(*xbee, con, conMode, &address)) != XBEE_ENONE) {
			xbee_log(*xbee, -1, "xbee_conNew() returned: %d (%s)", ret, xbee_errorToStr(ret));
			return ret;
		}
	
		/* Set CallBack Function to call CallBack_for_Data if packet receive. */
		if ((ret = xbee_conCallbackSet(*con, CallBack_for_Data, NULL)) != XBEE_ENONE) {
			xbee_log(*xbee, -1, "xbee_conCallbackSet() returned: %d", ret);
			return ret;
		}
	}
	
	/* if settings.catchAll = 1, then all packets will receive */
	if (xbee_conSettings(*con, NULL, &settings) != XBEE_ENONE) return ret; 
	
	settings.catchAll = 1; 
	
	if (xbee_conSettings(*con, &settings, NULL) != XBEE_ENONE) return ret;
	
	if ((ret = xbee_conDataSet(*con, *xbee, NULL)) != XBEE_ENONE) {
		xbee_log(*xbee, -1, "xbee_conDataSet() returned: %d", ret);
		return ret;
	}
	
	return XBEE_ENONE;
}

void Fill_Address(char *raw,unsigned char addr[8]){
	printf("raw in Fill_Address: %c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c\n", raw[0],raw[1],raw[2],raw[3],raw[4],raw[5],raw[6],raw[7],raw[8],raw[9],raw[10],raw[11],raw[12],raw[13],raw[14],raw[15]);
	sscanf(raw, "%2x%2x%2x%2x%2x%2x%2x%2x", &addr[0], &addr[1], &addr[2], &addr[3], &addr[4], &addr[5], &addr[6], &addr[7]);
	printf("addr in Fill_Address: %2x%2x%2x%2x%2x%2x%2x%2x\n",addr[0],addr[1],addr[2],addr[3],addr[4],addr[5],addr[6],addr[7]);
}

void AddressCopy(char *raw,char *dest, int size){
	memcpy(dest,raw,size);
	//sscanf(raw, "%2x%2x%2x%2x%2x%2x%2x%2x", &dest[0], &dest[1], &dest[2], &dest[3],&dest[4], &dest[5], &dest[6], &dest[7]);
	printf("raw Address Copy to dest Address\n");
	printf("raw: %2x%2x%2x%2x\n", raw[0], raw[1], raw[2], raw[3]);
	printf("dest: %2x%2x%2x%2x\n", dest[0], dest[1], dest[2], dest[3]);
}

/* 
 * this is the callback function...
 * it will be executed once for each packet that is recieved on an associated connection 
 */
 //A flag for check if all part of address are get. 2 send first address 1 send second address 0 success
void CallBack_for_get_address(struct xbee *xbee, struct xbee_con *con, struct xbee_pkt **pkt, void **data) {
	if ((*pkt)->dataLen == 0) {
		printf("too short...\n");
		return;
	}
	if(strcmp(((*pkt)->conType),("Local AT"))==0){
		if(get_address == 3){
			AddressCopy(((*pkt)->data),Local_Address,4);
			get_address = 2;
		}else if(get_address == 1){
			AddressCopy(((*pkt)->data),&Local_Address[4],4);
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
void CallBack_for_Data(struct xbee *xbee, struct xbee_con *con, struct xbee_pkt **pkt, void **data) {
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
	newpkt->address = malloc(sizeof(char)*16);
	AddressCopy(raw_addr,newpkt->address,16);
	newpkt -> content = content;
    newpkt->next = NULL; 
    rear->next = newpkt; 
    rear = newpkt; 
} 

/* A function for delete a sended Packet in queue */
void delq() { 
    pPkt* tmpnode; 

    if(front->next == NULL) { 
        printf("Queue is empty!\n"); 
        return; 
    } 

    tmpnode = front->next; 
    front->next = tmpnode->next; 
    free(tmpnode);
} 
