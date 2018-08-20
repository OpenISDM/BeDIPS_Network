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
 *		for Gateway
 *
 * File Name:
 *
 *      LBeacon_Zigbee_Gateway.c
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

    int ret;

    sxbee_config xbee_config;

    xbee_config.xbee_mode = "xbeeZB";

    xbee_config.xbee_device = "/dev/ttyAMA0";

    xbee_config.xbee_datastream = -1;

    xbee_config.config_location = "./xbee_Gateway_config.conf";

    xbee_Serial_Power_Reset(xbee_Serial_Power_Pin);

    xbee_Serial_init(&xbee_config.xbee_datastream, xbee_config.xbee_device);

    xbee_LoadConfig(&xbee_config);

    close(xbee_config.xbee_datastream);

    xbee_initial(&xbee_config);

    printf("Start establishing Connection to xbee.");

    /*--------------Configuration for connection in Data mode----------------*/
    /* In this mode we aim to get Data.                                      */
    /*-----------------------------------------------------------------------*/

    printf("Establishing Connection...\n");

    xbee_connector(&xbee_config);

    printf("Connection Successfully Established.\n");

    if((ret = xbee_conValidate(xbee_config.con)) != XBEE_ENONE){

        printf("con unvalidate ret : %d\n", ret);

        xbee_release(&xbee_config);

        return -1;

    }

    /* Start the chain reaction!                                             */

    while(1) {

        if(xbee_check_CallBack(&xbee_config, false)) break;

        /* If there are remain some packet need to send in the Queue,        */
        /* send the packet                                                   */
        xbee_send_pkt(&xbee_config);

        usleep(xbee_usleep_time);

        xbee_connector(&xbee_config);

        pPkt tmppkt = get_pkt(&xbee_config.Received_Queue);

        if (tmppkt != NULL){

            /* If data[0] == '@', callback will be end.                       */
            if(tmppkt -> content[0] == '@'){

                xbee_conCallbackSet(xbee_config.con, NULL, NULL);

                printf("*** DISABLED CALLBACK... ***\n");

            }

            delpkt(&xbee_config.Received_Queue);

        }

    }

    xbee_release(&xbee_config);

    return 0;
}
