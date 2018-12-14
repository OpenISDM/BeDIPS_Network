/*
  Copyright (c) 2016 Academia Sinica, Institute of Information Science

  License:

     GPL 3.0 : The content of this file is subject to the terms and conditions
     defined in file 'COPYING.txt', which is part of this source code package.

  Project Name:

     BeDIS

  File Description:

     This file demonstrate how to use Serial.c and Serial.h to setup xbee S2C in
     AT mode.

  File Name:

     xbee_Serial_Demo.c
1
Abstract:

     BeDIS uses LBeacons to deliver 3D coordinates and textual descriptions of
     their locations to users' devices. Basically, a LBeacon is an inexpensive,
     Bluetooth Smart Ready device. The 3D coordinates and location description
     of every LBeacon are retrieved from BeDIS (Building/environment Data and
     Information System) and stored locally during deployment and maintenance
     times. Once initialized, each LBeacon broadcasts its coordinates and
     location description to Bluetooth enabled user devices within its coverage
     area.

  Authors:
     Gary Xiao		, garyh0205@hotmail.com
*/
#include "../src/xbee_API.h"


int main(){

    sxbee_config xbee_config;

    xbee_config.xbee_datastream = -1;

    xbee_config.xbee_device = "/dev/ttyAMA0";

    xbee_config.config_location = "./xbee_config.conf";

    xbee_Serial_Power_Reset(xbee_Serial_Power_Pin);

    xbee_Serial_init(&xbee_config.xbee_datastream, xbee_config.xbee_device);

    xbee_LoadConfig(&xbee_config);

    printf("ATID 55\n");
    xbee_Send_Command(&xbee_config.xbee_datastream, "ATID 55\r", "OK");
    getchar();
    printf("ATSH\n");
    xbee_Send_Command_result(&xbee_config.xbee_datastream, "ATSH\r");
    getchar();
    printf("ATWR\n");
    xbee_Send_Command(&xbee_config.xbee_datastream, "ATWR\r", "OK");
    getchar();
    //----- CLOSE THE SERIAL -----
    close(xbee_config.xbee_datastream);

    return 0;
}
