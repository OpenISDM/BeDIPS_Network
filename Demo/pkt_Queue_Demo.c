/*
  Copyright (c) 2016 Academia Sinica, Institute of Information Science

  License:

     GPL 3.0 : The content of this file is subject to the terms and conditions
     defined in file 'COPYING.txt', which is part of this source code package.

  Project Name:

     BeDIS

  File Description:

     This file demonstrate how the pkt_queue work and how to use it.

  File Name:

     pkt_queue_Demo.c

  Abstract:

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

#include "../src/pkt_Queue.h"

int main(){

    spkt_ptr pkt_queue;

    printf("%d\n", queue_len(&pkt_queue));

    /* Initialize Queue for packets                                          */
    init_Packet_Queue(&pkt_queue);

    for(int i = 0; i < MAX_QUEUE_LENGTH;i++){

        addpkt(&pkt_queue, Data, Broadcast, "CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC", 90);

        sleep(1);

    }

    delpkt(&pkt_queue);

    sleep(1);

    addpkt(&pkt_queue, Data, Broadcast, "CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC", 90);

    sleep(1);

    delpkt(&pkt_queue);

    sleep(1);

    delpkt(&pkt_queue);

    sleep(1);

    addpkt(&pkt_queue, Data, Broadcast, "CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC", 90);

    sleep(1);

    Free_Packet_Queue(&pkt_queue);

    sleep(1);

    return 0;

}
