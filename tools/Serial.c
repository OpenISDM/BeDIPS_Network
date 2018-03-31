#include "Serial.h"

int main()
{
    xbee_datastream = -1;
    xbee_device = "/dev/ttyUSB0";
    xbee_Serial_buffer = 50;
    
    xbee_Serial_init(&xbee_datastream, xbee_device);
    xbee_Serial_Tx(&xbee_datastream, xbee_Serial_buffer);
    xbee_Serial_Rx(&xbee_datastream, xbee_Serial_buffer);

    //----- CLOSE THE SERIAL -----
    close(xbee_datastream);

    return 0;
}

int xbee_Serial_init(int *xbee_datastream, char *xbee_device )
{
    //  Open the Serial
    //  The flags (defined in fcntl.h):
    //  Access modes (use 1 of these):
    //      O_RDONLY - Open for reading only.
    //      O_RDWR - Open for reading and writing.
    //      O_WRONLY - Open for writing only.
    //
    //  O_NDELAY / O_NONBLOCK (same function) - Enables nonblocking mode. 
    //      When set read requests on the file can return immediately with a failure status 
    //      If there is no input immediately available (instead of blocking). 
    //      Likewise, write requests can also return immediately with a failure status 
    //      If the output can't be written immediately.
    //
    //  O_NOCTTY - When set and path identifies a terminal device, 
    //  	   open() shall not cause the terminal device to become 
    //             the controlling terminal for the process.
    
    //Open in non blocking read/write mode
    if ((*xbee_datastream = open(xbee_device, O_RDWR | O_NOCTTY | O_NDELAY)) == -1)      
    {
        //ERROR  CAN'T OPEN SERIAL PORT
        printf("Error - Unable to open Serial.  Ensure it is not in use by another application\n");
    }
                    
    //CONFIGURE THE SERIAL
    //The flags (defined in /usr/include/termios.h --- 
    //see http://pubs.opengroup.org/onlinepubs/007908799/xsh/termios.h.html):
    //  Baud rate: B1200, B2400, B4800, B9600, B19200, B38400, 
    //             B57600, B115200, B230400, B460800, B500000, 
    //             B576000, B921600, B1000000, B1152000, B1500000, 
    //             B2000000, B2500000, B3000000, B3500000, B4000000
    //  CSIZE  : CS5, CS6, CS7, CS8
    //  CLOCAL : Ignore modem status lines
    //  CREAD  : Enable receiver
    //  IGNPAR : Ignore characters with parity errors
    //  ICRNL  : Map CR to NL on input 
    //  (Use for ASCII comms where you want to auto correct end of line characters
    //   don't use for bianry comms!)
    //  PARENB : Parity enable
    //  PARODD : Odd parity (else even)
    
    struct termios options;

    tcgetattr(*xbee_datastream, &options);

    options.c_cflag = B9600 | CS8 | CLOCAL | CREAD;
    options.c_iflag = IGNPAR;
    options.c_oflag = 0;
    options.c_lflag = 0;

    tcflush(*xbee_datastream, TCIFLUSH);
    tcsetattr(*xbee_datastream, TCSANOW, &options);
    
    return 0;
}

int xbee_Serial_Tx(int* xbee_datastream, int xbee_Serial_buffer)
{
    //----- TX BYTES -----
    unsigned char tx_buffer[xbee_Serial_buffer];
    unsigned char *p_tx_buffer;
                
    p_tx_buffer = &tx_buffer[0];
    *p_tx_buffer++ = '+';
    *p_tx_buffer++ = '+';
    *p_tx_buffer++ = '+';

    if (*xbee_datastream != -1)
    {
	printf("Start Write\n");
        int count = write(*xbee_datastream, &tx_buffer[0], (p_tx_buffer - &tx_buffer[0]));      
	//Datastream, bytes to write, number of bytes to write
        if (count < 0)
        {
            printf("Serial TX error\n");
        }
	printf("Count : %d\n",count);
    }

    return 0;
}

int xbee_Serial_Rx(int *xbee_datastream, int xbee_Serial_buffer)
{
    //----- CHECK FOR ANY RX BYTES -----
    if (*xbee_datastream != -1)
    {
        // Read up to 255 characters from the port if they are there
        unsigned char rx_buffer[xbee_Serial_buffer];
	printf("Start Read\n");
        int rx_length = 0;      //Datastream, buffer to store in, number of bytes to read (max)
	
        do
	{	

        	rx_length = read(*xbee_datastream, (void*)rx_buffer, xbee_Serial_buffer);
        	printf("I am enter :%d\n",rx_length);
		if (rx_length < 0)
        	{
            		//An error occured (will occur if there are no bytes)
			printf("Read Error\n");
        	}
       		else if (rx_length == 0)
        	{
            		//No data waiting
			printf("No Data\n");
        	}
        	else
        	{
            		//Bytes received
            		rx_buffer[rx_length] = '\0';
            		printf("%i bytes read : %s\n", rx_length, rx_buffer);
			if(rx_buffer[0] == 'O' && rx_buffer[1] == 'K'){
				printf("All most done\n");

			}
			else
				printf("Fail\n");
        	}
    	}while(rx_length <= 0);
    }

    return 0;
}
