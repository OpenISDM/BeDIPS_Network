#include "pkt_Queue.h"

/* Initialize Queue                                                          */
void init_Packet_Queue(pkt_ptr pkt_queue) {

    pkt_queue->front = pkt_queue->rear = malloc(sizeof(sPkt));
    (pkt_queue->front)->next = (pkt_queue->rear)->next = NULL;

    return;
}

/* A function for create new packet in queue                                 */
void addpkt(spkt_ptr* pkt_queue, int type, char *raw_addr, char *content ) {
    pPkt newpkt = malloc(sizeof(sPkt));

    printf("------Content------\n");
    printf("type    : %s\n", type_to_str(type));
    printf("address : %s\n", raw_addr);
    printf("content : %s\n", content);
    printf("-------------------\n");

    if((pkt_queue->front)->next == NULL) {
        (pkt_queue->front)->next = newpkt;
    }

    newpkt -> type = type;
    newpkt->address = raw_addr;
    newpkt -> content = content;
    newpkt->next = NULL;

    (pkt_queue->rear)->next = newpkt;
    pkt_queue->rear = newpkt;

    display_pkt("Addedpkt", pkt_queue->rear);

    return;
}

/* A function for delete a sended Packet in queue                            */
void delpkt(spkt_ptr* pkt_queue) {

    sPkt* tmpnode;
    if((pkt_queue->front)->next == NULL) {
        printf("Packet Queue is empty!\n");
        return;
    }

    tmpnode = (pkt_queue->front)->next;
    (pkt_queue->front)->next = tmpnode->next;
    display_pkt("deledpkt",tmpnode);
    
    free(tmpnode);
    return;
}

void delallpkt(spkt_ptr* pkt_queue) {
    while ((pkt_queue->front)->next != NULL){
        delpkt(pkt_queue);
    }
    return;
}

/* Fill the address from raw(char) to addr(Hex)                              */
void Fill_Address(char *raw,unsigned char addr[8]){
//    sscanf(raw, "%2x%2x%2x%2x%2x%2x%2x%2x", &addr[0], &addr[1], &addr[2]
//              , &addr[3], &addr[4], &addr[5], &addr[6], &addr[7]);
}

char* type_to_str(int type){
  switch(type){
    case Data:
      return "Data";
      break;
    case AT:
      return "AT";
      break;
    default:
      return "UNKNOWN";
    }
  }

void display_pkt(char* content, pPkt pkt){
  printf("------ %12s ------\n",content);
  printf("type    : %s\n", type_to_str(pkt->type));
  printf("address : %s\n", pkt->address);
  printf("content : %s\n", pkt->content);
  printf("--------------------------\n");
}
