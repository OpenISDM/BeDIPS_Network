#include "../src/UDP_API.h"

int main(){

    sudp_config udp_config;
    
    printf("Success\n");

    udp_initial( &udp_config);
    
    printf("init success\n");
    
    udp_addpkt(&udp_config.pkt_Queue, "127.0.0.1", "CC", 2);

    printf("Addpkt successs\n");
   
    sleep(1);

    udp_config.shutdown = true;

    udp_release( &udp_config);

    return 0;

}
