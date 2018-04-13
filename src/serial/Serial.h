#include <stdio.h>
#include <unistd.h>         //Used for Serial
#include <fcntl.h>          //Used for Serial
#include <termios.h>        //Used for Serial
#include <string.h>

enum{ Remain , Ended };

//-------------------------
//----- SETUP Serial -----
//-------------------------

int xbee_datastream;

char *xbee_device;

int xbee_baudrate;

int xbee_Serial_buffer;

//Initialize xbee Serial connection and get the pointer of the xbee
int xbee_Serial_init(int *xbee_datastream, char *xbee_device );

//Send Serial command to xbee
int xbee_Serial_Tx(int *xbee_datastream, int xbee_Serial_buffer, char* Data);

//Receive Serial command to xbee
int xbee_Serial_Rx(int *xbee_datastream, int xbee_Serial_buffer, char* Data);

//Send AT command to xbee
int xbee_Send_Command(int *xbee_datastream, int xbee_Serial_buffer, char *Command, char *Command_Result);
