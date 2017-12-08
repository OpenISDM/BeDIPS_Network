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
	if ((*pkt)->dataLen == 0) {
		printf("too short...\n");
		return;
	}
	printf("rx: [%s]\n", (*pkt)->data);
}

int main(void) {
	void *d;
	struct xbee *xbee;
	struct xbee_con *con;
	struct xbee_conAddress address;
	unsigned char txRet;
	xbee_err ret;

	/* setup libxbee, using the USB to Serial adapter '/dev/ttyAMA0' at 9600 baud */
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
	
	switch()
	sscanf("000000000000FFFF", "%2x%2x%2x%2x%2x%2x%2x%2x", &address.addr64[0], &address.addr64[1], &address.addr64[2], &address.addr64[3],
														   &address.addr64[4], &address.addr64[5], &address.addr64[6], &address.addr64[7]   );
	
    /*
     * address.addr64[0] = 0x00;
     * address.addr64[1] = 0x13;
     * address.addr64[2] = 0xA2;
     * address.addr64[3] = 0x00;
     * address.addr64[4] = 0x40;
     * address.addr64[5] = 0x89;
     * address.addr64[6] = 0x16;
     * address.addr64[7] = 0x5F;
     */

	/* create a new AT connection to the Remote XBee */
	if ((ret = xbee_conNew(xbee, &con, "Remote AT", &address)) != XBEE_ENONE) {
		xbee_log(xbee, -1, "xbee_conNew() returned: %d (%s)", ret, xbee_errorToStr(ret));
		return ret;
	}

	/* 
	 * configure a callback for the connection
	 * this function is called every time a packet for this connection is recieved 
	 */
	if ((ret = xbee_conCallbackSet(con, myCB, NULL)) != XBEE_ENONE) {
		xbee_log(xbee, -1, "xbee_conCallbackSet() returned: %d", ret);
		return ret;
	}

	/* 
	 * send the AT command 'NI' (request the Node Identifier)
     * when the response is recieved, the packet will be directed to the callback function 
	 */
	ret = xbee_conTx(con, &txRet, "NI");

	/* 
	 * print out the return value
	 * if this is non-zero, then check 'enum xbee_errors' in xbee.h for its meaning
	 * alternatively look at the xbee_errorToStr() function 
	 */
	printf("tx: %d\n", ret);
	if (ret) {
		
		/* 
		 * if ret was non-zero, then some error occured
		 * if ret == XBEE_ETX then it is possible that txRet is now -17 / XBEE_ETIMEOUT
		 * alternatively, txRet will contain the status code returned by the XBee 
		 */
		printf("txRet: %d\n", txRet);
	} else {
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
