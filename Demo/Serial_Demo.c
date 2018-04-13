#include "Serial.h"

int main(){
    int xbee_datastream = -1;
    char* xbee_device = "/dev/ttyUSB0";
    int xbee_Serial_buffer = 50;
    
    xbee_Serial_init(&xbee_datastream, xbee_device);
    printf("+++\n");
    xbee_Send_Command(&xbee_datastream, xbee_Serial_buffer, "+++", "OK");
    getchar();
    printf("ATID 55\n");
    xbee_Send_Command(&xbee_datastream, xbee_Serial_buffer, "ATID 55\r", "OK");
    getchar();
    printf("ATID\n");
    xbee_Send_Command(&xbee_datastream, xbee_Serial_buffer, "ATID\r", "55");
    getchar();
    printf("ATWR\n");
    xbee_Send_Command(&xbee_datastream, xbee_Serial_buffer, "ATWR\r", "OK");
    getchar();
    //----- CLOSE THE SERIAL -----
    close(xbee_datastream);

    return 0;
}
