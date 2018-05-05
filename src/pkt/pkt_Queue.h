#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define Gateway   "0000000000000000"
#define Broadcast "000000000000FFFF"

enum {Data, Local_AT};

/* packet format in the Queue */
struct pkt {

	//"Data"
	int type;

	// Brocast:     000000000000FFFF;
	// Coordinator: 0000000000000000
	unsigned char address[8];

	// Data
	char *content;

  struct pkt *next;
};

typedef struct pkt sPkt;
typedef sPkt* pPkt;

typedef enum {Lock_Queue,unLock_Queue} Locker;

struct pkt_header {

    // front point to the first of thr Pkt Queue
    // rear  point to the end of the Pkt Queue
    pPkt front;
    pPkt rear;
    Locker locker;
    int len;
};

typedef struct pkt_header spkt_ptr;
typedef spkt_ptr* pkt_ptr;

/* Create Packet Queue Header */
void init_Packet_Queue(pkt_ptr pkt_queue);

void Free_Packet_Queue(pkt_ptr pkt_queue);

/* Add new Packet to the end of Queue */
void addpkt(pkt_ptr pkt_queue, int type, char *raw_addr, char *content);

/* Delete the end of Queue */
void delpkt(pkt_ptr pkt_queue);

void delallpkt(pkt_ptr pkt_queue);

char* type_to_str(int type);

char* print_address(unsigned char* address);

void display_pkt(char* content, pPkt pkt);

/* Fill the address from raw(char) to addr(Hex) */
void Fill_Address(char *raw, unsigned char* addr);
