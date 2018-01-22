#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <xbee.h>
#include "setting.h"

int main(void) {
	
	struct xbee *xbee;
	
	struct xbee_con *con;
	
	/* The address stored the destination MAC of xbee */
	struct xbee_conAddress address;
	
	/* The setting for xbee */
	struct xbee_conSettings settings;
	
	/* A variable to get error code */
	xbee_err ret;
	
	/* A variable te get Tx return value */
	unsigned char txRet;
	
	/* Initialize Queue for packets */ 
	init_Packet_Queue();
	
	/*-----------------------------Configuration for xbee---------------------------------*/
	/*------------------------------------------------------------------------------------*/

	/* xbee_setup(struct xbee **retXbee, const char *mode, char *device, int baudrate) */
	
	printf("Start Connecting to xbee\n");
	if ((ret = xbee_setup(&xbee, mode, device, baudrate)) != XBEE_ENONE) {
		printf("Connection Failed\nret: %d (%s)\n", ret, xbee_errorToStr(ret));
		return ret;
	}
	printf("xbee Connected\n");
	
	printf("Start Setting up Log Level\n");
	/* Setup Log Level 0:disable Log, 100:enable Log */
	if ((ret = xbee_logLevelSet(xbee,LogLevel)) != XBEE_ENONE) {
		printf("Setting Failed\nret: %d (%s)\n", ret, xbee_errorToStr(ret));
		return ret;
	}
	printf("Setting Log Level Successed\nLog Level : %d\n",LogLevel);
	
	/*-----------------------Configuration connection in AT mode--------------------------*/
	/* In this mode we aim to get the address of xbee.                                    */
	/*------------------------------------------------------------------------------------*/
	
	printf("Start establising Connection to xbee\nMode : GetAddress\nStart Connecting...\n");
	/* connector for Get Address */
	xbee_connector(&xbee,&con,"GetAddress");
	printf("Connector Established\n");
	
	printf("Creating memery space for Local_Address\n");
	Local_Address = malloc(sizeof(char)*8);
	printf("memery space Created\n");
	
	printf("Start Get Local Address...\n");

	/*----------------------------------Local Address---------------------------------------*/
	/*-SH(Serial Number High): Read the high 32 bits of the module's unique 64-bit address.-*/
	/*--SL(Serial Number Low): Read the low 32 bits of the module's unique 64-bit address.--*/
	/*--------------------------------------------------------------------------------------*/

	printf("Start sending AT Command: SH\n");
	/* send the AT command 'SH' */
	xbee_conTx(con, &txRet, "SH");
	printf("SH sended\n");
	
	float count = 0;
	
	while(get_address == 3){
		usleep(500000);
		if(count == 10){
			xbee_conTx(con,&txRet,"SH");
			printf("SH Resended\n");
			count = 0;
		}
		else{
			count += 0.5;
			printf("Waitung for SH...(%.1fs)\n",count);
		}
	}

	printf("SH Received\n");
	
	get_address = 1;	
	
	printf("Start Sending AT Command: SL\n");
	/* send the AT command 'SL' */
	xbee_conTx(con, &txRet, "SL");
	printf("SL sended\n");
	
	count = 0;	
	
	while(get_address == 1){
		usleep(500000);
		if(count == 10){
			xbee_conTx(con,&txRet,"SL");
			printf("SL resended\n");
			count = 0;
		}
		else{
			count += 0.5;
			printf("Waiting for SL...(%.1fs)\n",count);
		}
	}
	printf("SL Received\n");

	printf("Exiting Local AT Mode...\n");
	/* shutdown the connection */
	if ((ret = xbee_conEnd(con)) != XBEE_ENONE) {
		printf("Failed to exit Local AT Mode\n");
		xbee_log(xbee, -1, "xbee_conEnd() returned: %d", ret);
		return ret;
	}
	printf("Local AT Exited\n");
	
	/*--------------------Configuration for connection in Data mode-----------------------*/
	/* In this mode we aim to get Data.                                                   */
	/*------------------------------------------------------------------------------------*/
	
	printf("Start Sending Local_Address to Gateway\n");
		
	printf("Establishing Connection...\n");
	xbee_connector(&xbee,&con,"Data");
	printf("Connection Succesfully Established\n");
	
	/* start the chain reaction! */
	
	addpkt("Gateway","Data","AAAAA");

	while(1) {
		
		/* Pointer point_to_CallBack will store the callback function. If pointer point_to_CallBack is NULL, break the Loop */
		void *point_to_CallBack;
		
		if ((ret = xbee_conCallbackGet(con, (xbee_t_conCallback*)&point_to_CallBack)) != XBEE_ENONE) {
			xbee_log(xbee, -1, "xbee_conCallbackGet() returned: %d", ret);
			return ret;
		}
		
		if (point_to_CallBack == NULL){
			printf("Stop Xbee...\n");
			break;
		
		
		}	
		
		/* If there are remain some packet need to send in the Queue, send the packet */
		if(front->next != NULL){
			
			/* shutdown the connection */
			if ((ret = xbee_conEnd(con)) != XBEE_ENONE) {
				xbee_log(xbee, -1, "xbee_conEnd() returned: %d", ret);
				return ret;
			}

			xbee_connector(&xbee, &con, front->next->type);

			xbee_conTx(con, NULL, front->next->content);
			
			delq();
		}
		else{
			printf("Queue is NULL\n");		
		}
		usleep(2000000);
	}
	
	/* Close connection */
	if ((ret = xbee_conEnd(con)) != XBEE_ENONE) {
		xbee_log(xbee, -1, "xbee_conEnd() returned: %d", ret);
	return ret;
	}

	printf("Stop connection Successed\n");
	/* Close xbee */
	xbee_shutdown(xbee);
	printf("Shutdown Xbee Successed\n");

	return 0;
}

xbee_err xbee_connector(struct xbee **xbee, struct xbee_con **con, char *conMode){
	
	struct xbee_conAddress address;
	
	struct xbee_conSettings settings;
	
	xbee_err ret;
		
	/* Setup a default adrress. 
	 * If it is a coordinator, it might be brocast.
	 * If it is a router, it might be connect to coordinator as default.
	 */
	memset(&address, 0, sizeof(address));
	address.addr64_enabled = 1;
	
	/* If the packet Queue still remain some packet, continue to fill address */
	if(front->next != NULL){
		Fill_Address(front->next->address,address.addr64);
	}

	if(strcmp(conMode,"GetAddress") == 0){
		if ((ret = xbee_conNew(*xbee, con, "Local AT", NULL)) != XBEE_ENONE) {
			xbee_log(*xbee, -1, "xbee_conNew() returned: %d (%s)", ret, xbee_errorToStr(ret));
			return ret;
		}


		/* Set CallBack Function to call CallBack if packet receive. */
		if ((ret = xbee_conCallbackSet(*con, CallBack_for_get_address, NULL)) != XBEE_ENONE) {
			xbee_log(*xbee, -1, "xbee_conCallbackSet() returned: %d", ret);
			return ret;
		}
	}

	else if(strcmp(conMode, "Data") == 0){
		if((ret = xbee_conNew(*xbee, con, "Data", &address)) != XBEE_ENONE) {
			xbee_log(*xbee, -1, "xbee_conNew() returned: %d (%s)", ret, xbee_errorToStr(ret));
			return ret;
		}
		
		/* Set CallBack Function to call CallBack if packet received */
		if((ret = xbee_conCallbackSet(*con, CallBack,NULL)) != XBEE_ENONE) {
			xbee_log(*xbee, -1, "xbee_conCallbackSet() returned: %d", ret);
			return ret;
		}
	}

	else{
		printf("Error conMode Error\n");
		return XBEE_EFAILED;
	}
	
	/* if settings.catchAll = 1, then all packets will receive */
	if (xbee_conSettings(*con, NULL, &settings) != XBEE_ENONE) return ret; 
	
	settings.catchAll = 1; 
	
	if (xbee_conSettings(*con, &settings, NULL) != XBEE_ENONE) return ret;
	
	if ((ret = xbee_conDataSet(*con, *xbee, NULL)) != XBEE_ENONE) {
		xbee_log(*xbee, -1, "xbee_conDataSet() returned: %d", ret);
		return ret;
	}
	
	if(front->next != NULL){
		printf("//////Packet Information//////\n");
		printf("Dest Address: %c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c\n",front->next->address[0],front->next->address[1],front->next->address[2],front->next->address[3],front->next->address[4],front->next->address[5],front->next->address[6],front->next->address[7],front->next->address[8],front->next->address[9],front->next->address[10],front->next->address[11],front->next->address[12],front->next->address[13],front->next->address[14],front->next->address[15]);
		printf("Type        : %s\n",front->next->type);
		printf("Content     : %s\n",front->next->content);
	}
	
	return XBEE_ENONE;
}

void Fill_Address(char *raw,unsigned char addr[8]){	
	sscanf(raw, "%2x%2x%2x%2x%2x%2x%2x%2x", &addr[0], &addr[1], &addr[2], &addr[3], &addr[4], &addr[5], &addr[6], &addr[7]);
}

void AddressCopy(char *raw,char *dest, int size){
	if(raw == "Gateway"){
		raw = "0000000000000000";
	}
	if(raw == "Brocast"){
		raw = "000000000000FFFF";
	}
	memcpy(dest,raw,size);
}

/* 
 * this is the callback function...
 * it will be executed once for each packet that is recieved on an associated connection 
 */
 //A flag for check if all part of address are get. 2 send first address 1 send second address 0 success
void CallBack_for_get_address(struct xbee *xbee, struct xbee_con *con, struct xbee_pkt **pkt, void **data) {
	printf("Enter CallBack\n");
	if(strcmp(((*pkt)->conType),("Local AT"))==0){
		printf("Start Get Address\n");
		if(get_address == 3){
			printf("Receiving SH\n");
			AddressCopy(((*pkt)->data),Local_Address,4);
			get_address = 2;
			return;
		}
		
		if(get_address == 1){
			printf("Receiving SL\n");
			AddressCopy(((*pkt)->data),&Local_Address[4],4);
			
			printf("Local_Address: %02x%02x%02x%02x%02x%02x%02x%02x\n", Local_Address[0], Local_Address[1], Local_Address[2], Local_Address[3], Local_Address[4], Local_Address[5], Local_Address[6], Local_Address[7]);
			
			get_address = 0;
		}
	}
}

/* 
 * this is the callback function...
 * it will be executed once for each packet that is recieved on an associated connection 
 * if settings.catchAll = 1 then it will be executed for every packet.
 */
void CallBack(struct xbee *xbee, struct xbee_con *con, struct xbee_pkt **pkt, void **data) {
	printf("Enter CallBack Data\n");
	if ((*pkt)->dataLen > 0) {
		/* if data[0] == '@', callback will be end. */ 
		if ((*pkt)->data[0] == '@') {
			xbee_conCallbackSet(con, NULL, NULL);
			printf("*** DISABLED CALLBACK... ***\n");
		}
		
		xbee_log(xbee, -1, "rx: [%s]\n", (*pkt)->data);
		
		/* 
		 * If data is received, how to deal with the data.
		 * AT? Data?
		 */
		printf("Received Data: %s\n",((*pkt)->data));
		
	}
}

void init_Packet_Queue() { 
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
        	printf("Packet Queue is empty!\n"); 
        	return; 
    	} 

    	tmpnode = front->next; 
    	front->next = tmpnode->next; 
    	free(tmpnode);
} 
