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
 *   	This file contains the program for the waiting queue.
 *
 * File Name:
 *
 *      pkt_Queue.c
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

#include "pkt_Queue.h"


/* Queue initialize and free */

int init_Packet_Queue(pkt_ptr pkt_queue){

    int ret;

    pthread_mutex_init( &pkt_queue -> mutex, 0);

    pkt_queue -> front = -1;

    pkt_queue -> rear  = -1;

    for(int num = 0 ; num < MAX_QUEUE_LENGTH ; num ++){

        pkt_queue -> Queue[num].type = NONE;

    }

    return pkt_Queue_SUCCESS;

}

int Free_Packet_Queue(pkt_ptr pkt_queue){

    int ret;

    while ( !(is_null(pkt_queue))){

        delpkt(pkt_queue);

    }

    pthread_mutex_destroy( &pkt_queue -> mutex);

    return pkt_Queue_SUCCESS;

}

/* New : add pkts */

int addpkt(pkt_ptr pkt_queue, unsigned int type, unsigned char *identification
         , unsigned int Data_fragmentation, unsigned int Data_offset
         , char *raw_addr, char *content, int content_size) {

    int ret;

    pthread_mutex_lock( &pkt_queue -> mutex);

    printf("--------- Content ---------\n");
    printf("type               : %s\n", type_to_str(type));

    printf("identification     : ");

    print_content(identification, identification_length);

    printf("\n");
    printf("Data Fragmentation : %d\n", Data_fragmentation);

    printf("Data_offset        : %d\n", Data_offset);

    printf("address            : ");

    print_content(raw_addr, Address_length);

    printf("\n");
    printf("--------- content ---------\n");

    print_content(content, MAX_DATA_LENGTH);

    printf("\n");
    printf("---------------------------\n");

    if(is_null(pkt_queue)){

        pkt_queue -> front = 0;

        pkt_queue -> rear  = 0;

    }

    else{

        if(is_full(pkt_queue)){

            pthread_mutex_unlock( &pkt_queue -> mutex);

            return pkt_Queue_FULL;

        }

        else{

            if( pkt_queue -> rear == MAX_QUEUE_LENGTH - 1){

                pkt_queue -> rear = 0;

            }

            else{

                pkt_queue -> rear ++ ;

            }

        }

    }

    pkt_queue -> Queue[pkt_queue -> rear].type = type;

    char_to_hex(identification, pkt_queue
             -> Queue[pkt_queue -> rear].identification, identification_length);

    char_to_hex(raw_addr, pkt_queue -> Queue[pkt_queue -> rear].address,
                Address_length);

    pkt_queue -> Queue[pkt_queue -> rear].Data_offset = Data_offset;

    memset(pkt_queue -> Queue[pkt_queue -> rear].content, 0
         , MAX_DATA_LENGTH * sizeof(char));

    strncpy(pkt_queue -> Queue[pkt_queue -> rear].content, content
          , content_size);

    display_pkt("addedpkt", pkt_queue, pkt_queue -> rear);

    printf("= pkt_queue len  =\n");

    printf("%d\n", queue_len(pkt_queue));

    printf("==================\n");

    pthread_mutex_unlock( &pkt_queue -> mutex);

    return pkt_Queue_SUCCESS;

}

/* Delete : delete pkts */

int delpkt(pkt_ptr pkt_queue) {

    pthread_mutex_lock( &pkt_queue -> mutex);

    if(is_null(pkt_queue)) {

        pthread_mutex_unlock( &pkt_queue -> mutex);

        return pkt_Queue_SUCCESS;

    }

    display_pkt("deledpkt", pkt_queue, pkt_queue -> front);

    memset(pkt_queue -> Queue[pkt_queue -> front].content, 0
         , MAX_DATA_LENGTH * sizeof(char));

    pkt_queue -> Queue[pkt_queue -> front].type = NONE;

    if(pkt_queue -> front == pkt_queue -> rear){

        pkt_queue -> front = -1;

        pkt_queue -> rear  = -1;

    }
    else{

        if(pkt_queue -> front == MAX_QUEUE_LENGTH - 1){

            pkt_queue -> front = 0;

        }

        else{

            pkt_queue -> front += 1;

        }

    }

    printf("= pkt_queue len  =\n");

    printf("%d\n", queue_len(pkt_queue));

    printf("==================\n");

    pthread_mutex_unlock( &pkt_queue -> mutex);

    return pkt_Queue_SUCCESS;

}

void display_pkt(char *content, pkt_ptr pkt_queue, int pkt_num){

    if(pkt_num < 0 && pkt_num >= MAX_QUEUE_LENGTH)

        return;

    char *char_addr = hex_to_char(pkt_queue -> Queue[pkt_num].address
                                , Address_length_Hex);

    char *identification =hex_to_char(pkt_queue -> Queue[pkt_num].identification
                                   , identification_length_Hex);

    printf("==================\n");

    printf("%s\n", content);

    printf("==================\n");

    printf("====== type ======\n");

    printf("%s\n", type_to_str(pkt_queue -> Queue[pkt_num].type));

    printf("==Identification==\n");

    char *Identification_char = hex_to_char(identification
                                          , identification_length_Hex);

    printf("%s\n", Identification_char);

    printf("===== address ====\n");

    char *address_char = hex_to_char(pkt_queue -> Queue[pkt_num].address
                                   , Address_length_Hex);

    printf("%s\n", address_char);

    printf("==== content =====\n");

    print_content(pkt_queue -> Queue[pkt_num].content, MAX_XBEE_DATA_LENGTH);

    printf("\n");

    printf("==================\n");

    free(Identification_char);
    free(address_char);
    free(char_addr);

    return;

}

/* Tools */

char* type_to_str(int type){

    switch(type){

        case Data:

            return "Data";

            break;

        case Local_AT:

            return "Local AT";

            break;

        case UDP:

            return "UDP";

            break;

        default:

            return "UNKNOWN";
    }
}

int str_to_type(const char *conType){

    if(memcmp(conType, "Transmit Status"
     , strlen("Transmit Status") * sizeof(char)) == 0){

        return Data;

    }

    else if(memcmp(conType, "Data", strlen("Data") * sizeof(char)) == 0){

        return Data;

    }

    else{

        return UNKNOWN;

    }

}

void char_to_hex(char *raw, unsigned char *raw_hex, int size){

    for(int i = 0 ; i < (size/2) ; i ++){

        char tmp[2];

        tmp[0] = raw[i * 2];

        tmp[1] = raw[i * 2 + 1];

        raw_hex[i] = strtol(tmp,(void *) NULL, 16);

    }

}

char *hex_to_char(unsigned char *hex, int size){

    int char_size = size * 2;

    char *char_addr = malloc(sizeof(char) * ((char_size * 2) + 1));

    memset(char_addr, 0, sizeof(char) * (char_size + 1));

    for(int len = 0;len < size;len ++){

        sprintf(&char_addr[len * 2], "%02x", hex[len]);

    }

    return char_addr;
}

void array_copy(unsigned char *src, unsigned char *dest, int size){

    memcpy(dest, src, size);

    return;

}

bool address_compare(unsigned char *addr1,unsigned char *addr2){

    if (memcmp(addr1, addr2, 8) == 0){

        return true;

    }

    return false;

}

pPkt get_pkt(pkt_ptr pkt_queue){

    if(is_null(pkt_queue)){

        return NULL;

    }

    display_pkt("Get_pkt", pkt_queue, pkt_queue -> front);

    return &(pkt_queue -> Queue[pkt_queue -> front]);

}

bool is_null(pkt_ptr pkt_queue){

    if (pkt_queue->front == -1 && pkt_queue->rear == -1){

        return true;

    }

    return false;

}

bool is_full(pkt_ptr pkt_Queue){

    if(pkt_Queue -> front == pkt_Queue -> rear + 1){

        return true;

    }

    else if(pkt_Queue -> front == 0 && pkt_Queue -> rear == MAX_QUEUE_LENGTH - 1){

        return true;

    }

    else{

        return false;

    }
}

int queue_len(pkt_ptr pkt_queue){

    if (pkt_queue -> front == 0 && pkt_queue -> rear == 0){

        return 1;

    }

    else if(pkt_queue -> front == -1 && pkt_queue -> rear == -1){

        return 0;

    }

    else if (pkt_queue -> front == pkt_queue -> rear){

        return 1;

    }

    else if (pkt_queue -> rear > pkt_queue -> front){

        return (pkt_queue -> rear - pkt_queue -> front + 1);

    }

    else if (pkt_queue -> front > pkt_queue -> rear){

        return ((MAX_QUEUE_LENGTH - pkt_queue -> front) + pkt_queue -> rear + 1);

    }

    else{

        return queue_len_error;

    }

    return queue_len_error;

}

void print_content(char *content, int size){

    for(int loc = 0; loc < size; loc ++){

        printf("%c", content[loc]);

    }

}

void generate_identification(char *identification, int size){

    char str[] = "0123456789ABCDEF";

    memset(identification, 0 , size * sizeof(char));

    struct timeval tv;

    struct timezone tz;

    gettimeofday (&tv , &tz);

    /* Seed number for rand() */
    srand((unsigned int) (tv.tv_sec * 1000 + tv.tv_usec));

    for(int length = 0;length < size;length ++) {

        identification[length] = str[rand() % 16];

        srand(rand());

    }

}
