/*
  Copyright (c) 2016 Academia Sinica, Institute of Information Science

  License:

       GPL 3.0 : The content of this file is subject to the terms and
       cnditions defined in file 'COPYING.txt', which is part of this
       source code package.

  Project Name:

       BeDIPS

  File Description:

       This file demonstrate how the UDP_Demo work and how to use it.

  File Name:

       UDP_Demo.c

  Abstract:

       BeDIPS uses LBeacons to deliver 3D coordinates and textual
       descriptions of their locations to users' devices. Basically, a
       LBeacon is an inexpensive, Bluetooth Smart Ready device. The 3D
       coordinates and location description of every LBeacon are retrieved
       from BeDIS (Building/environment Data and Information System) and
       stored locally during deployment and maintenance times. Once
       initialized, each LBeacon broadcasts its coordinates and location
       description to Bluetooth enabled user devices within its coverage
       area.

  Authors:
       Gary Xiao		, garyh0205@hotmail.com
 */
#include "../src/pkt_Queue.h"
#include "../src/UDP_API.h"


int main(){
    sudp_config udp_config;

    printf("Success\n");

    udp_initial( &udp_config);

    printf("init success\n");

    udp_addpkt(&udp_config, "127.0.0.1", "TESTING...", 10);

    printf("Addpkt successs\n");

    sleep(1);

    while(1){
        udp_addpkt(&udp_config, "127.0.0.1", "TESTING...", 10);
    }

    udp_config.shutdown = true;

    udp_release( &udp_config);

    return 0;

}
