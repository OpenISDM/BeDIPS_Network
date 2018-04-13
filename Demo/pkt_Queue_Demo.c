#include "pkt_Queue.h"

int main(){

  pkt_ptr pkt_queue = malloc(sizeof(spkt_ptr));

  /* Initialize Queue for packets                                          */
  init_Packet_Queue(pkt_queue);

  for(int i=0;i<10;i++){
    addpkt(pkt_queue, Data, Gateway, "AAAAA");
  }

  delallpkt(pkt_queue);

  addpkt(pkt_queue,Data, Gateway, "AAAAA");

  delpkt(pkt_queue);

  free(pkt_queue);
  
  return 0;
}
