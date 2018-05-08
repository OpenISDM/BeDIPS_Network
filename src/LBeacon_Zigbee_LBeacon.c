#include "LBeacon_Zigbee.h"

int main(void) {

    unsigned char txRet;

    struct xbee *xbee;

    struct xbee_con *con;

    pkt_ptr pkt_Queue = malloc(sizeof(spkt_ptr));

    xbee_initial(&xbee, pkt_Queue);

    /*-----------------Configuration connection in AT mode-------------------*/
    /* In this mode we aim to get the address of xbee.                       */
    /*-----------------------------------------------------------------------*/

    printf("Start establishing Connection to xbee\n");


    /*--------------Configuration for connection in Data mode----------------*/
    /* In this mode we aim to get Data.                                      */
    /*-----------------------------------------------------------------------*/

    printf("Establishing Connection...\n");
    
    xbee_connector(&xbee, &con, pkt_Queue);
    
    printf("Connection Successfully Established\n");

    /* Start the chain reaction!                                             */

    if((ret = xbee_conValidate(con)) != XBEE_ENONE){
        xbee_log(xbee, 1, "con unvalidate ret : %d", ret);
        return ret;
    }
    
    while(1) {
        /* Pointer point_to_CallBack will store the callback function.       */
        /* If pointer point_to_CallBack is NULL, break the Loop              */
        void *point_to_CallBack;

        if ((ret = xbee_conCallbackGet(con, (xbee_t_conCallback*)
            &point_to_CallBack))!= XBEE_ENONE) {
            xbee_log(xbee, -1, "xbee_conCallbackGet() returned: %d", ret);
            return ret;
        }
        
	if (point_to_CallBack == NULL){
            printf("Stop Xbee...\n");
            break;
        }


    	addpkt(pkt_Queue, Data, Gateway, "AAAAA");
        
	/* If there are remain some packet need to send in the Queue,        */
        /* send the packet                                                   */
        if(pkt_Queue->front->next != NULL){

            /* Shutdown the connection                                       */
            //if ((ret = xbee_conEnd(con)) != XBEE_ENONE) {
                //xbee_log(xbee, -1, "xbee_conEnd() returned: %d", ret);
                //return ret;
            //}

            xbee_conTx(con, NULL, pkt_Queue->front->next->content);

            delpkt(pkt_Queue);
        }
        else{
            xbee_log(xbee, -1, "xbee packet Queue is NULL.");
        }
        usleep(2000000);
    }

    printf("Jump out while\n");

    Free_Packet_Queue(pkt_Queue);
    
    /* Close connection                                                      */
    if ((ret = xbee_conEnd(con)) != XBEE_ENONE) {
        xbee_log(xbee, 10, "xbee_conEnd() returned: %d", ret);
        return ret;
    }

    printf("Stop connection Succeeded\n");
    
    /* Close xbee                                                            */
    xbee_shutdown(xbee);
    printf("Shutdown Xbee Succeeded\n");

    return 0;
}
