/*
  Copyright (c) 2016 Academia Sinica, Institute of Information Science

  License:

      GPL 3.0 : The content of this file is subject to the terms and
      cnditions defined in file 'COPYING.txt', which is part of this
      source code package.

  Project Name:

      BeDIPS

  File Description:

      This file contains the program to connect to xbee by API mode and in
      the project, we use it for data transmission most.

  File Name:

      xbee_API.c

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

#include "xbee_API.h"

xbee_err xbee_initial(pxbee_config xbee_config){

    int ret;

    if ((ret = xbee_setup( &xbee_config -> xbee, xbee_config -> xbee_mode
       , xbee_config -> xbee_device, 9600)) != XBEE_ENONE)       return ret;

    if((ret = xbee_validate(xbee_config -> xbee)) != XBEE_ENONE) return ret;

    if(ret = init_Packet_Queue( &xbee_config -> pkt_Queue) != pkt_Queue_SUCCESS) return ret;

    if(ret = init_Packet_Queue( &xbee_config -> Received_Queue) != pkt_Queue_SUCCESS) return ret;

    xbee_connector(xbee_config);

    pthread_create(&xbee_config -> xbee_send, NULL, xbee_send_pkt, (void*) xbee_config);

    xbee_config -> shutdown = false;

    return XBEE_ENONE;
}

int xbee_LoadConfig(pxbee_config xbee_config){

    int count = 0, ret;

    char ch, AT_Command[30];

    FILE *cfg;

    cfg = fopen(xbee_config -> config_location, "r");           // read mode

    if (cfg == NULL)

        return File_OPEN_ERROR;

    memset(AT_Command, 0, 30 * sizeof(char));

    bool ATWR = false;

    if(ret = xbee_Send_Command( &xbee_config -> xbee_datastream, "+++"
     , "OK") != 0) return ret;

    count = 0;

    while((ch = fgetc(cfg)) != EOF && !ATWR){

        if(ch == '\n'){

            printf("%s\n", AT_Command);

            if(AT_Command[0] == 'A' && AT_Command[1] == 'T') {

                if((AT_Command[2] == 'W' && AT_Command[3] == 'R')
                || (AT_Command[2] == 'A' && AT_Command[3] == 'C')
                || (AT_Command[2] == 'R' && AT_Command[3] == 'E')
                || (AT_Command[2] == 'I' && AT_Command[3] == 'D')) {

                    sprintf(AT_Command, "%s\r", AT_Command);

                    if(ret = xbee_Send_Command( &xbee_config -> xbee_datastream
                     , AT_Command, "OK") != 0) return ret;

                if(AT_Command[2] == 'W' && AT_Command[3] == 'R') ATWR = true;

                }

                else{

                    char command[6], arg[26];

                    memset(command, 0, 6 * sizeof(char));

                    memset(arg, 0, 26 * sizeof(char));

                    sscanf(AT_Command, "%s %s\n", command, arg);

                    sprintf(AT_Command, "%s\r", AT_Command);

                    sprintf(command, "%s\r", command);

                    if(ret = xbee_Send_Command( &xbee_config -> xbee_datastream
                     , AT_Command, "OK")) return ret;

                    if(ret = xbee_Send_Command( &xbee_config -> xbee_datastream
                     , command, arg)) return ret;

                }

            }

            memset(AT_Command, 0, 30 * sizeof(char));

            count = 0;

        }

        else{

            AT_Command[count] = ch;

            count ++;

        }

    }

    fclose(cfg);

    return 0;

}

xbee_err xbee_connector(pxbee_config xbee_config){

    int ret;

    bool Require_CallBack = true;

    if((ret = xbee_conValidate(xbee_config -> con)) == XBEE_ENONE){

    	if(is_null( &xbee_config -> pkt_Queue))  return XBEE_ENONE;

        else if(address_compare(xbee_config -> pkt_Queue.Queue[xbee_config
             -> pkt_Queue.front].address, xbee_config -> pkt_Queue.address))

            return XBEE_ENONE;

        else{

            Require_CallBack = !(xbee_check_CallBack(xbee_config, true));

            // Close connection
            if ((ret = xbee_conEnd(xbee_config -> con)) != XBEE_ENONE)

                return ret;

        }

    }

    int Mode;

    struct xbee_conAddress address;

    struct xbee_conSettings settings;

    memset( &address, 0, sizeof(address));

    memset(xbee_config -> pkt_Queue.address, 0, sizeof(unsigned char) * 8);

    address.addr64_enabled = 1;

    printf("Fill Address to the Connector\n");

    if(!is_null( &xbee_config -> pkt_Queue)){

        array_copy(xbee_config -> pkt_Queue.Queue[xbee_config
                -> pkt_Queue.front].address, address.addr64, Address_length);

        array_copy(xbee_config -> pkt_Queue.Queue[xbee_config
                -> pkt_Queue.front].address, xbee_config -> pkt_Queue.address
                 , Address_length);

        Mode = xbee_config -> pkt_Queue.Queue[xbee_config
            -> pkt_Queue.front].type;

    }

    else

        Mode = Data;

    char* strMode = type_to_str(Mode);

    if(Mode == Local_AT){

        if((ret = xbee_conNew(xbee_config -> xbee, &xbee_config -> con, strMode
                            , NULL)) != XBEE_ENONE)

            return ret;

    }

    else if(Mode == Data){

        if((ret = xbee_conNew(xbee_config -> xbee, &xbee_config -> con, strMode
                            , &address)) != XBEE_ENONE)

            return ret;

    }

    else

        return XBEE_EFAILED;

    if(Require_CallBack)

        /* Set CallBack Function to call CallBack if packet received          */
        if((ret = xbee_conCallbackSet(xbee_config -> con, CallBack
                                    , NULL)) != XBEE_ENONE) return ret;

    if((ret = xbee_conValidate(xbee_config -> con)) != XBEE_ENONE) return ret;

    /* If settings.catchAll = 1, then all packets will receive                */
    if ((ret = xbee_conSettings(xbee_config -> con, NULL, &settings))
                                != XBEE_ENONE) return ret;

    settings.catchAll = 1;

    if ((ret = xbee_conSettings(xbee_config -> con, &settings, NULL))
                                != XBEE_ENONE) return ret;

    if ((ret = xbee_conDataSet(xbee_config -> con, &xbee_config
                            -> Received_Queue, NULL)) != XBEE_ENONE) return ret;

    return XBEE_ENONE;

}

void *xbee_send_pkt(void *xbeeconfig){

    pxbee_config xbee_config = (pxbee_config) xbeeconfig;

    while(!(xbee_config -> shutdown) && !xbee_check_CallBack(xbee_config, false)){

        if(!(is_null( &xbee_config -> pkt_Queue))){

            if( !(address_compare(xbee_config -> pkt_Queue.Queue[xbee_config
              -> pkt_Queue.front].address, xbee_config -> pkt_Queue.address)))

                return XBEE_ENONE;

            //Assemble the content.

            char tmp_content[MAX_XBEE_PKT_LENGTH];

            memset(&tmp_content, 0, sizeof(char) * MAX_XBEE_PKT_LENGTH);

            //Copy hex identification to content.

            array_copy(xbee_config -> pkt_Queue.Queue[xbee_config
                    -> pkt_Queue.front].identification, &tmp_content[0]
                     , sizeof(char) * identification_length_Hex);

            char hex_Data_fragmentation[2];

            //Convert integer Data Fragmentation to hex.

            sprintf(hex_Data_fragmentation, "%02x", xbee_config
         -> pkt_Queue.Queue[xbee_config -> pkt_Queue.front].Data_fragmentation);

            char_to_hex( (char *) &hex_Data_fragmentation, &tmp_content[3]
                       , sizeof(char) * 2);

            char hex_Data_offset[4];

            //Convert integer Data offset to hex.

            sprintf(hex_Data_offset, "%04x", xbee_config
                -> pkt_Queue.Queue[xbee_config -> pkt_Queue.front].Data_offset);

            char_to_hex( (char *) &hex_Data_fragmentation, &tmp_content[4]
                       , sizeof(char) * 4);

            array_copy(xbee_config -> pkt_Queue.Queue[xbee_config
                    -> pkt_Queue.front].Reserved, &tmp_content[6]
                     , sizeof(char) * XBEE_RESERVE_BYTE);

            array_copy(xbee_config -> pkt_Queue.Queue[xbee_config
                    -> pkt_Queue.front].content, &tmp_content[9]
                     , sizeof(char) * MAX_XBEE_DATA_LENGTH);

            xbee_connTx(xbee_config -> con, NULL, (unsigned char *) &tmp_content
                      , MAX_XBEE_PKT_LENGTH);

            delpkt( &xbee_config -> pkt_Queue);

            usleep(xbee_usleep_time);

            xbee_connector(xbee_config);

        }

    }

}

xbee_err xbee_addpkt(pxbee_config xbee_config, unsigned int type, char *raw_addr
                   , char *header, char *content, int size){

    char identification[identification_length];

    memset(identification, 0, sizeof(char) * identification_length);

    generate_identification(identification, identification_length);

    unsigned int offset = 0;

    unsigned int  Data_fragmentation = 1;

    char tmp_content[MAX_XBEE_DATA_LENGTH];

    while(Data_fragmentation == 1){

        memset(tmp_content, 0, sizeof(char) * MAX_XBEE_DATA_LENGTH);

        int loc = 0;

        for(loc = 0;loc < MAX_XBEE_DATA_LENGTH;loc ++){

            tmp_content[loc] = content[offset * XBEE_PKT_OFFSET_TIMES + loc];

            if((offset * XBEE_PKT_OFFSET_TIMES + loc  + 1) == size){

                break;

            }

        }

        if((offset + 1) * XBEE_PKT_OFFSET_TIMES <= size){

            Data_fragmentation = 0;

        }

        addpkt( &xbee_config -> pkt_Queue, type, identification
             , Data_fragmentation, offset, raw_addr, /*tmp_content*/ "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"
             , MAX_XBEE_DATA_LENGTH);

        offset = offset + (MAX_XBEE_DATA_LENGTH / XBEE_PKT_OFFSET_TIMES);

    }

    return 0;

}

bool xbee_check_CallBack(pxbee_config xbee_config, bool ignore_pkt_Queue){

    /* Pointer point_to_CallBack will store the callback function.       */
    /* If pointer point_to_CallBack is NULL, break the Loop              */

    int ret;

    void *point_to_CallBack;

    if ((ret = xbee_conCallbackGet(xbee_config -> con
     , (xbee_t_conCallback *) &point_to_CallBack)) != XBEE_ENONE) return true;

    if (point_to_CallBack == NULL && (ignore_pkt_Queue
     || is_null( &xbee_config -> pkt_Queue))) return true;

    return false;

}

xbee_err xbee_release(pxbee_config xbee_config){

    int ret;

    if(xbee_conValidate(xbee_config -> con) != XBEE_ENONE){

        if ((ret = xbee_conEnd(xbee_config -> con)) != XBEE_ENONE) return ret;

    }

    Free_Packet_Queue( &xbee_config -> pkt_Queue);

    Free_Packet_Queue( &xbee_config -> Received_Queue);

    /* Close xbee                                                            */
    xbee_shutdown(xbee_config -> xbee);

    return XBEE_ENONE;

}

/*  Data Transmission                                                        */
void CallBack(struct xbee *xbee, struct xbee_con *con, struct xbee_pkt **pkt
                                                            , void **data) {

    pkt_ptr Received_Queue = (pkt_ptr) *data;

    if ((( *pkt) -> dataLen > 0 ) && (str_to_type(( *pkt) -> conType) == Data)) {

        //addpkt(Received_Queue, str_to_type(( *pkt) -> conType)
        //     , print_address(( *pkt) -> address.addr64), ( *pkt)->data);

        //display_pkt("Received Data", Received_Queue, Received_Queue->front);

        print_content(( *pkt)->data, MAX_XBEE_PKT_LENGTH);

        xbee_log(xbee, 10, "rx: [%s]\n", ( *pkt)->data);

    }

}
