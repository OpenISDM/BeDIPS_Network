/*
 * Copyright (c) 2016 Academia Sinica, Institute of Information Science
 *
 * License:
 *
 *      GPL 3.0 : The content of this file is subject to the terms and
 *      cnditions defined in file 'COPYING.txt', which is part of this
 *      source code package.
 *
 * Project Name:
 *
 *      BeDIPS
 *
 * File Description:
 *
 *   	This file contains the program to set up a star network by XBEE S2C
 *      module in order to deal with NSI(Network Setup and Initialization)
 *      and Data transmission between Gateway and LBeacon. And This file is
 *		for LBeacon.
 *
 * File Name:
 *
 *      LBeacon_Zigbee_LBeacon.c
 *
 * Abstract:
 *
 *      BeDIPS uses LBeacons to deliver 3D coordinates and textual
 *      descriptions of their locations to users' devices. Basically, a
 *      LBeacon is an inexpensive, Bluetooth Smart Ready device. The 3D
 *      coordinates and location description of every LBeacon are retrieved
 *      from BeDIS (Building/environment Data and Information System) and
 *      stored locally during deployment and maintenance times. Once
 *      initialized, each LBeacon broadcasts its coordinates and location
 *      description to Bluetooth enabled user devices within its coverage
 *      area.
 *
 * Authors:
 *      Gary Xiao		, garyh0205@hotmail.com
 */

#include "../src/xbee_API.h"

int main(void) {

    char* xbee_mode = "xbeeZB";

    char* xbee_device = "/dev/ttyAMA0";

    int xbee_baudrate = 9600;

    int LogLevel = 100;

    struct xbee *xbee;

    struct xbee_con *con;

    spkt_ptr pkt_Queue, Received_Queue;

    xbee_initial(xbee_mode, xbee_device, xbee_baudrate
                            , LogLevel, &xbee, &pkt_Queue, &Received_Queue);
    printf("Start establishing Connection to xbee\n");


    /*--------------Configuration for connection in Data mode----------------*/
    /* In this mode we aim to get Data.                                      */
    /*-----------------------------------------------------------------------*/

    printf("Establishing Connection...\n");

    xbee_connector(&xbee, &con, &pkt_Queue, &Received_Queue);

    printf("Connection Successfully Established\n");

    /* Start the chain reaction!                                             */

    if((ret = xbee_conValidate(con)) != XBEE_ENONE){
        xbee_log(xbee, 1, "con unvalidate ret : %d", ret);
        return ret;
    }

        addpkt(&pkt_Queue, Data, Gateway, "AAAAA");
        addpkt(&pkt_Queue, Data, Gateway, "AAAAA");
        addpkt(&pkt_Queue, Data, Gateway, "AAAAA");
        addpkt(&pkt_Queue, Data, Broadcast, "AAAAA");
        addpkt(&pkt_Queue, Data, "0013A2004127CE8B", "AAAAA");
        addpkt(&pkt_Queue, Data, Gateway, "AAAAA");
        addpkt(&pkt_Queue, Data, Gateway, "AAAAA");
        addpkt(&pkt_Queue, Data, Gateway, "AAAAA");
        addpkt(&pkt_Queue, Data, "0013A2004127CE8B", "AAAAA");
        addpkt(&pkt_Queue, Data, "0013A2004127CE8B", "AAAAA");
        addpkt(&pkt_Queue, Data, "0013A2004127CE8B", "AAAAA");
        addpkt(&pkt_Queue, Data, "0013A2004127CE8B", "AAAAA");
        addpkt(&pkt_Queue, Data, Gateway, "AAAAA");
        addpkt(&pkt_Queue, Data, Gateway, "AAAAA");
        addpkt(&pkt_Queue, Data, Gateway, "AAAAA");
        addpkt(&pkt_Queue, Data, Gateway, "AAAAA");
        addpkt(&pkt_Queue, Data, Gateway, "AAAAA");
        addpkt(&pkt_Queue, Data, Gateway, "AAAAA");
        addpkt(&pkt_Queue, Data, Gateway, "AAAAA");
        addpkt(&pkt_Queue, Data, Gateway, "AAAAA");
        addpkt(&pkt_Queue, Data, Gateway, "AAAAA");
        addpkt(&pkt_Queue, Data, Broadcast, "AAAAA");
        addpkt(&pkt_Queue, Data, "0013A2004127CE8B", "AAAAA");
        addpkt(&pkt_Queue, Data, Gateway, "AAAAA");
        addpkt(&pkt_Queue, Data, Gateway, "AAAAA");
        addpkt(&pkt_Queue, Data, Broadcast, "AAAAA");
        addpkt(&pkt_Queue, Data, "0013A2004127CE8B", "AAAAA");
        addpkt(&pkt_Queue, Data, Gateway, "AAAAA");
        addpkt(&pkt_Queue, Data, Gateway, "AAAAA");
        addpkt(&pkt_Queue, Data, Broadcast, "AAAAA");
        addpkt(&pkt_Queue, Data, "0013A2004127CE8B", "AAAAA");
        addpkt(&pkt_Queue, Data, Gateway, "AAAAA");
        addpkt(&pkt_Queue, Data, Gateway, "AAAAA");
        addpkt(&pkt_Queue, Data, Broadcast, "AAAAA");
        addpkt(&pkt_Queue, Data, "0013A2004127CE8B", "AAAAA");
        addpkt(&pkt_Queue, Data, Gateway, "AAAAA");
        addpkt(&pkt_Queue, Data, Gateway, "AAAAA");
        addpkt(&pkt_Queue, Data, Broadcast, "AAAAA");
        addpkt(&pkt_Queue, Data, "0013A2004127CE8B", "AAAAA");
        addpkt(&pkt_Queue, Data, Gateway, "AAAAA");
        addpkt(&pkt_Queue, Data, Gateway, "AAAAA");
        addpkt(&pkt_Queue, Data, Broadcast, "AAAAA");
        addpkt(&pkt_Queue, Data, "0013A2004127CE8B", "AAAAA");
        addpkt(&pkt_Queue, Data, Gateway, "AAAAA");
        addpkt(&pkt_Queue, Data, Gateway, "AAAAA");
        addpkt(&pkt_Queue, Data, Broadcast, "AAAAA");
        addpkt(&pkt_Queue, Data, "0013A2004127CE8B", "AAAAA");
        addpkt(&pkt_Queue, Data, Gateway, "AAAAA");
        addpkt(&pkt_Queue, Data, Gateway, "AAAAA");
        addpkt(&pkt_Queue, Data, Broadcast, "AAAAA");
        addpkt(&pkt_Queue, Data, "0013A2004127CE8B", "AAAAA");
        addpkt(&pkt_Queue, Data, Gateway, "AAAAA");
        addpkt(&pkt_Queue, Data, Gateway, "AAAAA");

    while(1) {

        if(xbee_check_CallBack(con, &pkt_Queue, false)) break;

        if(!xbee_check_CallBack(con, &pkt_Queue, true))
            addpkt(&pkt_Queue, Data, Gateway, "AAAAA");

        /* If there are remain some packet need to send in the Queue,        */
        /* send the packet                                                   */
        xbee_send_pkt(con, &pkt_Queue);

        //usleep(2000000);

        printf("Queue Length is %d\n", queue_len(&pkt_Queue));

        xbee_connector(&xbee, &con, &pkt_Queue, &Received_Queue);

        pPkt tmppkt = get_pkt(&Received_Queue);
        if (tmppkt != NULL){

            printf("Get Address\n");
            printf("Type : %s\n", type_to_str(tmppkt -> type));
            printf("Address: %s\n", print_address(tmppkt -> address));
            printf("Content: %s\n", tmppkt -> content);

            /* If data[0] == '@', callback will be end.                       */
            if(tmppkt -> content[0] == '@'){

                xbee_conCallbackSet(con, NULL, NULL);
                printf("*** DISABLED CALLBACK... ***\n");

            }

            delpkt(&Received_Queue);
        }
    }

    printf("Stop xbee ...\n");

    Free_Packet_Queue(&pkt_Queue);
    Free_Packet_Queue(&Received_Queue);


    /* Close connection                                                      */
    if ((ret = xbee_conEnd(con)) != XBEE_ENONE) {
        xbee_log(xbee, 10, "xbee_conEnd() returned: %d", ret);
        return ret;
    }

    Free_Packet_Queue(&pkt_Queue);
    Free_Packet_Queue(&Received_Queue);

    printf("Stop connection Succeeded\n");

    /* Close xbee                                                            */
    xbee_shutdown(xbee);
    printf("Shutdown Xbee Succeeded\n");

    return 0;
}
