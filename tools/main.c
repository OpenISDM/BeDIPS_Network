#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <xbee.h>
#include "setting.h"


/* queue for sending data */
struct pkt_info{
	char *type; 				//"Data" or "AT"
	unsigned char address[8]; 	//Brocast:000000000000FFFF;Coordinator:0000000000000000
	char *content; 				// "Data":Data;"AT":Command for dest device (the address you assigned) e.g for AT: NICoordinator
	struct pkt_info* next;
};

typedef struct pkt_info* spkt_info;

struct pkt_pointer{
	spkt_info Queue;
	spkt_info tail;
	int packet_len;
};

void AddressToChar(char *raw,unsigned char addr[8]);
struct pkt_pointer newPacket(struct pkt_pointer pkt_ptr, char *raw_addr, char *type,char *content);
struct pkt_pointer freeSendedPacket(struct pkt_pointer pkt_ptr);


void AddressToChar(char *raw,unsigned char addr[8]){
	sscanf(raw, "%2x%2x%2x%2x%2x%2x%2x%2x", &addr[0], &addr[1], &addr[2], &addr[3],
											&addr[4], &addr[5], &addr[6], &addr[7]   );
	//printf("%2x%\n", addr[5]);
}

/* A function for create new packet in queue */
struct pkt_pointer newPacket(struct pkt_pointer pkt_ptr, char *raw_addr, char *type,char *content){
	//printf("Init\n%d\n",*packet_len);
	spkt_info tmp = (spkt_info) malloc(sizeof(spkt_info));
	tmp -> type = type;
	AddressToChar(raw_addr,tmp->address);
	tmp -> content = content;
	
	//printf("%s %2x %s\n",tmp->type,tmp->address[5],tmp->content);
	
	if(pkt_ptr.packet_len == 0){
		printf("Success\n");
		pkt_ptr.Queue = tmp;
		//printf("%s %2x %s\n",pkt_ptr.Queue->type,pkt_ptr.Queue->address[5],pkt_ptr.Queue->content);
		
		pkt_ptr.tail = tmp;
		//printf("%s %2x %s\n",pkt_ptr.tail->type,pkt_ptr.tail->address[5],pkt_ptr.tail->content);
		
	}else{
		pkt_ptr.tail->next = tmp;
		pkt_ptr.tail = pkt_ptr.tail->next;
		//printf("%s %2x %s\n",pkt_ptr.tail->type,pkt_ptr.tail->address[5],pkt_ptr.tail->content);
		//printf("haha\n");
	}
	printf("func end\n");
	pkt_ptr.packet_len++;
	return pkt_ptr;
}

/* A function for delete a sended Packet in queue */
struct pkt_pointer freeSendedPacket(struct pkt_pointer pkt_ptr){
	if(pkt_ptr.packet_len == 0){
		printf("Error\n");
	}else{
		if(pkt_ptr.packet_len == 1){
			free(pkt_ptr.Queue);
			pkt_ptr.Queue = NULL;
			pkt_ptr.tail = NULL;
		}else{
			spkt_info tmp = pkt_ptr.Queue;
			pkt_ptr.Queue = &pkt_ptr.Queue->next;
			//free(tmp);
		}
		pkt_ptr.packet_len --;
	} 
}

/* 
 * this is the callback function...
 * it will be executed once for each packet that is recieved on an associated connection 
 * if settings.catchAll = 1 then it will be executed for every packet.
 */
void myCB(struct xbee *xbee, struct xbee_con *con, struct xbee_pkt **pkt, void **data) {
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

int main(void) {
	struct xbee *xbee;
	struct xbee_con *con,*con_AT;
	struct xbee_conAddress address;
	xbee_err ret;
	struct xbee_conSettings settings;
	
	/*Queue for Sending data */
	struct pkt_pointer pkt_ptr;
	pkt_ptr.Queue = NULL;
	pkt_ptr.tail = NULL;
	pkt_ptr.packet_len = 0;
	pkt_ptr = newPacket(pkt_ptr,"0000000000000000", "Data", "AABBAAA");
	/* 
	 * xbee_setup(struct xbee **retXbee, const char *mode, char *device, int baudrate) 
	 *
	 */
	if ((ret = xbee_setup(&xbee, *mode, *device, baudrate)) != XBEE_ENONE) {
		printf("ret: %d (%s)\n", ret, xbee_errorToStr(ret));
		return ret;
	}
	
	/* Setup Log Level */
	if ((ret = xbee_logLevelSet(xbee,LogLevel)) != XBEE_ENONE) {
		printf("ret: %d (%s)\n", ret, xbee_errorToStr(ret));
		return ret;
	}
	
	/* Setup a adrress */
	memset(&address, 0, sizeof(address));
	address.addr64_enabled = 1;
	if(Brocast == 1)
		AddressToChar("000000000000FFFF",&address.addr64)
		//sscanf("000000000000FFFF", "%2x%2x%2x%2x%2x%2x%2x%2x", &address.addr64[0], &address.addr64[1], &address.addr64[2], &address.addr64[3],
		//										 				 &address.addr64[4], &address.addr64[5], &address.addr64[6], &address.addr64[7]   );

	if ((ret = xbee_conNew(xbee, &con, "Data", &address)) != XBEE_ENONE) {
		xbee_log(xbee, -1, "xbee_conNew() returned: %d (%s)", ret, xbee_errorToStr(ret));
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
	
	/* Set CallBack Function to call myCB if packet receive. */
	if ((ret = xbee_conCallbackSet(con, myCB, NULL)) != XBEE_ENONE) {
		xbee_log(xbee, -1, "xbee_conCallbackSet() returned: %d", ret);
		return ret;
	}

	/* if need to inform something to coordinator or brocast to all device when start listen or receive packet then you can un-comment and type text in "text" to send it */
	/* xbee_conTx(con, NULL, "text"); */

	/* start the chain reaction! */
	while(1) {
		void *p;

		if ((ret = xbee_conCallbackGet(con, (xbee_t_conCallback*)&p)) != XBEE_ENONE) {
			xbee_log(xbee, -1, "xbee_conCallbackGet() returned: %d", ret);
			return ret;
		}
		if (p == NULL) break;
		usleep(500000);
		
		if(pkt_ptr.packet_len > 0){
			xbee_conTx(con, NULL, pkt_ptr.Queue->content);
			pkt_ptr = freeSendedPacket(pkt_ptr);
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
