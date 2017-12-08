#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <xbee.h>

/* 
 * this is the callback function...
 * it will be executed once for each packet that is recieved on an associated connection 
 */
void myCB(struct xbee *xbee, struct xbee_con *con, struct xbee_pkt **pkt, void **data) {
	if ((*pkt)->dataLen > 0) {
		if ((*pkt)->data[0] == '@') {
			xbee_conCallbackSet(con, NULL, NULL);
			printf("*** DISABLED CALLBACK... ***\n");
		}
		printf("rx: [%s]\n", (*pkt)->data);
	}
	printf("tx: %d\n", xbee_conTx(con, NULL, "Hello\r\n"));
}

int main(void) {
	void *d;
	struct xbee *xbee;
	struct xbee_con *con;
	struct xbee_conAddress address;
	xbee_err ret;
	
	if ((ret = xbee_setup(&xbee, "xbeeZB", "/dev/ttyAMA0", 9600)) != XBEE_ENONE) {
		printf("ret: %d (%s)\n", ret, xbee_errorToStr(ret));
		return ret;
	}

	/* 
	 * this is the 64-bit address of the remote XBee module
	 * it should be entered with the MSB first
	 * if you need to brocast please set  64-bit address to "000000000000FFFF"
	 */

	memset(&address, 0, sizeof(address));
	address.addr64_enabled = 1;

	sscanf( "000000000000FFFF" , "%2x%2x%2x%2x%2x%2x%2x%2x", &address.addr64[0], &address.addr64[1], &address.addr64[2], &address.addr64[3], 
											   &address.addr64[4], &address.addr64[5], &address.addr64[6], &address.addr64[7]	);

	/*
		address.addr64[0] = 0x00;
		address.addr64[1] = 0x13;
		address.addr64[2] = 0xA2;
		address.addr64[3] = 0x00;
		address.addr64[4] = 0x40;
		address.addr64[5] = 0x89;
		address.addr64[6] = 0x16;
		address.addr64[7] = 0x5F;
	 */

	if ((ret = xbee_conNew(xbee, &con, "Data", &address)) != XBEE_ENONE) {
		xbee_log(xbee, -1, "xbee_conNew() returned: %d (%s)", ret, xbee_errorToStr(ret));
		return ret;
	}

	if ((ret = xbee_conDataSet(con, xbee, NULL)) != XBEE_ENONE) {
		xbee_log(xbee, -1, "xbee_conDataSet() returned: %d", ret);
		return ret;
	}

	if ((ret = xbee_conCallbackSet(con, myCB, NULL)) != XBEE_ENONE) {
		xbee_log(xbee, -1, "xbee_conCallbackSet() returned: %d", ret);
		return ret;
	}

	/* start the chain reaction! */
	xbee_conTx(con, NULL, "Hello\r\n");

	while(1) {
		void *p;

		if ((ret = xbee_conCallbackGet(con, (xbee_t_conCallback*)&p)) != XBEE_ENONE) {
			xbee_log(xbee, -1, "xbee_conCallbackGet() returned: %d", ret);
			return ret;
		}

		if (p == NULL) break;
		/* give the callback a chance to run */
		usleep(1000000);
	}

	/* shutdown the connection */
	if ((ret = xbee_conEnd(con)) != XBEE_ENONE) {
		xbee_log(xbee, -1, "xbee_conEnd() returned: %d", ret);
		return ret;
	}

	/* shutdown libxbee */
	xbee_shutdown(xbee);

	return 0;
}
