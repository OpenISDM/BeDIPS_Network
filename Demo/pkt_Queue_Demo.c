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
 *      This file demonstrate how the pkt_Queue work and how to use it.
 *
 * File Name:
 *
 *      pkt_Queue_Demo.c
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
        printf("Queue is null\n");
    }

    printf("pkt_Queue length : %d\n", queue_len(&pkt_queue));

    printf("add and delete recursively\n");

    addpkt(&pkt_queue, Data, "0123456789ABCDEF", "1");
    printf("pkt_Queue length : %d\n", queue_len(&pkt_queue));
    display_pkt("Test-front", pkt_queue.front.next);
    display_pkt("Test-rear", pkt_queue.rear.next);

    delpkt(&pkt_queue);
    printf("pkt_Queue length : %d\n", queue_len(&pkt_queue));

    addpkt(&pkt_queue, Data, "0123456789ABCDEF", "2");
    printf("pkt_Queue length : %d\n", queue_len(&pkt_queue));
    display_pkt("Test-front", pkt_queue.front.next);
    display_pkt("Test-rear", pkt_queue.rear.next);

    delpkt(&pkt_queue);
    printf("pkt_Queue length : %d\n", queue_len(&pkt_queue));

    addpkt(&pkt_queue, Data, "0123456789ABCDEF", "3");
    printf("pkt_Queue length : %d\n", queue_len(&pkt_queue));
    display_pkt("Test-front", pkt_queue.front.next);
    display_pkt("Test-rear", pkt_queue.rear.next);

    delpkt(&pkt_queue);
    printf("pkt_Queue length : %d\n", queue_len(&pkt_queue));

    addpkt(&pkt_queue, Data, "0123456789ABCDEF", "4");
    printf("pkt_Queue length : %d\n", queue_len(&pkt_queue));
    display_pkt("Test-front", pkt_queue.front.next);
    display_pkt("Test-rear", pkt_queue.rear.next);

    delpkt(&pkt_queue);
    printf("pkt_Queue length : %d\n", queue_len(&pkt_queue));

    addpkt(&pkt_queue, Data, "0123456789ABCDEF", "5");
    printf("pkt_Queue length : %d\n", queue_len(&pkt_queue));
    display_pkt("Test-front", pkt_queue.front.next);
    display_pkt("Test-rear", pkt_queue.rear.next);

    delpkt(&pkt_queue);
    printf("pkt_Queue length : %d\n", queue_len(&pkt_queue));

    addpkt(&pkt_queue, Data, "0123456789ABCDEF", "6");
    printf("pkt_Queue length : %d\n", queue_len(&pkt_queue));
    display_pkt("Test-front", pkt_queue.front.next);
    display_pkt("Test-rear", pkt_queue.rear.next);

    delpkt(&pkt_queue);
    printf("pkt_Queue length : %d\n", queue_len(&pkt_queue));

    addpkt(&pkt_queue, Data, "0123456789ABCDEF", "7");
    printf("pkt_Queue length : %d\n", queue_len(&pkt_queue));
    display_pkt("Test-front", pkt_queue.front.next);
    display_pkt("Test-rear", pkt_queue.rear.next);

    delpkt(&pkt_queue);
    printf("pkt_Queue length : %d\n", queue_len(&pkt_queue));
    printf("Add * 3\n");

    addpkt(&pkt_queue, Data, "0123456789ABCDEF", "8");
    printf("pkt_Queue length : %d\n", queue_len(&pkt_queue));
    display_pkt("Test-front", pkt_queue.front.next);
    display_pkt("Test-rear", pkt_queue.rear.next);

    addpkt(&pkt_queue, Data, "0123456789ABCDEF", "9");
    printf("pkt_Queue length : %d\n", queue_len(&pkt_queue));
    display_pkt("Test-front", pkt_queue.front.next);
    display_pkt("Test-rear", pkt_queue.rear.next);

    delpkt(&pkt_queue);
    printf("pkt_Queue length : %d\n", queue_len(&pkt_queue));
    display_pkt("Test-front", pkt_queue.front.next);
    display_pkt("Test-rear", pkt_queue.rear.next);

    delpkt(&pkt_queue);
    printf("pkt_Queue length : %d\n", queue_len(&pkt_queue));
    display_pkt("Test-front", pkt_queue.front.next);
    display_pkt("Test-rear", pkt_queue.rear.next);
    printf("Add * 2\n");

    addpkt(&pkt_queue, Data, "0123456789ABCDEF", "10");
    printf("pkt_Queue length : %d\n", queue_len(&pkt_queue));
    display_pkt("Test-front", pkt_queue.front.next);
    display_pkt("Test-rear", pkt_queue.rear.next);

    addpkt(&pkt_queue, Data, "0123456789ABCDEF", "11");
    printf("pkt_Queue length : %d\n", queue_len(&pkt_queue));
    display_pkt("Test-front", pkt_queue.front.next);
    display_pkt("Test-rear", pkt_queue.rear.next);
    printf("Add total 12\n");

    addpkt(&pkt_queue, Data, "0123456789ABCDEF", "12");
    printf("pkt_Queue length : %d\n", queue_len(&pkt_queue));
    display_pkt("Test-front", pkt_queue.front.next);
    display_pkt("Test-rear", pkt_queue.rear.next);

    addpkt(&pkt_queue, Data, "0123456789ABCDEF", "13");
    printf("pkt_Queue length : %d\n", queue_len(&pkt_queue));
    display_pkt("Test-front", pkt_queue.front.next);
    display_pkt("Test-rear", pkt_queue.rear.next);

    addpkt(&pkt_queue, Data, "0123456789ABCDEF", "14");
    printf("pkt_Queue length : %d\n", queue_len(&pkt_queue));
    display_pkt("Test-front", pkt_queue.front.next);
    display_pkt("Test-rear", pkt_queue.rear.next);

    Free_Packet_Queue(&pkt_queue);
    printf("pkt_Queue length : %d\n", queue_len(&pkt_queue));

    Free_Packet_Queue(&pkt_queue);

    return 0;
}
