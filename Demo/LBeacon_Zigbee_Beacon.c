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

    struct xbee *xbee;

    struct xbee_con *con;

    spkt_ptr pkt_Queue, Received_Queue;

    xbee_initial(xbee_mode, xbee_device, xbee_baudrate
               , &xbee, &pkt_Queue, &Received_Queue);

    add_log(&pkt_Queue.xbee_log, collect_info, "Start establishing Connection to xbee."
          , false);

    /*--------------Configuration for connection in Data mode----------------*/
    /* In this mode we aim to get Data.                                      */
    /*-----------------------------------------------------------------------*/

    add_log(&pkt_Queue.xbee_log, collect_info, "Establishing Connection..."
          , false);

    xbee_connector(&xbee, &con, &pkt_Queue, &Received_Queue);

    add_log(&pkt_Queue.xbee_log, collect_info, "Connection Successfully Established."
          , false);

    if((ret = xbee_conValidate(con)) != XBEE_ENONE){
        char ret_value[50];

        memset(ret_value, 0, 50);

        sprintf(ret_value, "con unvalidate ret : %d", ret);

        add_log(&pkt_Queue.xbee_log, collect_info, ret_value, false);

        xbee_release(xbee, con, &pkt_Queue, &Received_Queue);

        return -1;
        
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

    /* Start the chain reaction!                                             */

    while(1) {

        if(xbee_check_CallBack(con, &pkt_Queue, false)) break;

        if(!xbee_check_CallBack(con, &pkt_Queue, true))

            addpkt(&pkt_Queue, Data, Gateway, "AAAAA");

        /* If there are remain some packet need to send in the Queue,        */
        /* send the packet                                                   */
        xbee_send_pkt(con, &pkt_Queue);

        //usleep(2000000);

        xbee_connector(&xbee, &con, &pkt_Queue, &Received_Queue);

        pPkt tmppkt = get_pkt(&Received_Queue);

        if (tmppkt != NULL){

            /* If data[0] == '@', callback will be end.                       */
            if(tmppkt -> content[0] == '@'){

                xbee_conCallbackSet(con, NULL, NULL);

                printf("*** DISABLED CALLBACK... ***\n");

            }

            delpkt(&Received_Queue);
        }
    }

    xbee_release(xbee, con, &pkt_Queue, &Received_Queue);

    return 0;
}
