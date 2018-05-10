#include "../src/pkt_Queue.h"

int main(){

  pkt_ptr pkt_queue = malloc(sizeof(spkt_ptr));

  /* Initialize Queue for packets                                          */
  init_Packet_Queue(pkt_queue);

  if(pkt_queue->front->next == NULL && pkt_queue->rear->next == NULL){
      printf("Queue is null\n");
  }

    addpkt(pkt_queue, Data, "0123456789ABCDEF", "1");
    delpkt(pkt_queue);
    addpkt(pkt_queue, Data, "0123456789ABCDEF", "2");
    delpkt(pkt_queue);
    addpkt(pkt_queue, Data, "0123456789ABCDEF", "3");
    delpkt(pkt_queue);
    addpkt(pkt_queue, Data, "0123456789ABCDEF", "4");
    delpkt(pkt_queue);
    addpkt(pkt_queue, Data, "0123456789ABCDEF", "5");
    delpkt(pkt_queue);
    addpkt(pkt_queue, Data, "0123456789ABCDEF", "6");
    delpkt(pkt_queue);
    addpkt(pkt_queue, Data, "0123456789ABCDEF", "7");
    delpkt(pkt_queue);
    addpkt(pkt_queue, Data, "0123456789ABCDEF", "8");
    addpkt(pkt_queue, Data, "0123456789ABCDEF", "9");
    delpkt(pkt_queue);

    delpkt(pkt_queue);

  addpkt(pkt_queue, Data, "0123456789ABCDEF", "10");
  addpkt(pkt_queue, Data, "0123456789ABCDEF", "11");

  delallpkt(pkt_queue);

  Free_Packet_Queue(pkt_queue);

  return 0;
}
