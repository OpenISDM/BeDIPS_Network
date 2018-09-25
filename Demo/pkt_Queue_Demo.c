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
 *      This file demonstrate how the pkt_queue work and how to use it.
 *
 * File Name:
 *
 *      pkt_queue_Demo.c
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

#include "../src/pkt_Queue.h"

int main(){

    spkt_ptr pkt_queue ;

    /* Initialize Queue for packets                                          */
    init_Packet_Queue(&pkt_queue);

    if(is_null(&pkt_queue)){
        printf("Queue is NULL.\n");
    }

    char len[10];

    printf("add and delete recursively.\n");

    addpkt(&pkt_queue, Data, "0123456789ABCDEF", "1");

    display_pkt("Test-front", &pkt_queue, pkt_queue.front);
    display_pkt("Test-rear", &pkt_queue, pkt_queue.rear);

    address_copy(pkt_queue.Queue[pkt_queue.front].address, pkt_queue.address);
    if(address_compare(pkt_queue.Queue[pkt_queue.front].address
                     , pkt_queue.address)){
        printf("Address The Same.\n");
    }

    display_pkt("Test-front", &pkt_queue, pkt_queue.front);
    display_pkt("Test-rear", &pkt_queue, pkt_queue.rear);

    delpkt(&pkt_queue);

    int l = MAX_QUEUE_LENGTH;
    while(l --){

        char l_str[10];
        memset(l_str, 0, sizeof(char) * 10);
        sprintf(l_str, "%d", l);
        addpkt(&pkt_queue, Data,"0123456789ABCDEF", l_str);
    }

    display_pkt("Test-front", &pkt_queue, pkt_queue.front);
    display_pkt("Test-rear", &pkt_queue, pkt_queue.rear);

    delpkt(&pkt_queue);

    addpkt(&pkt_queue, Data, "0123456789ABCDEF", "END");

    Free_Packet_Queue(&pkt_queue);

    char id[Address_length + 1];

    memset(id, 0, sizeof(char) * (Address_length + 1));

    generate_identification(id);

    printf("%s\n", id);

    return 0;
}
