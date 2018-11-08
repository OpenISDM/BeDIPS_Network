srcPath = ../src/
CC = gcc -std=gnu99

LBeacon_Zigbee_Gateway.out: $(srcPath)xbee_Serial.o $(srcPath)xbee_API.o $(srcPath)pkt_Queue.o LBeacon_Zigbee_Gateway.c
	$(CC) -lxbee -lwiringPi $(srcPath)xbee_Serial.o $(srcPath)xbee_API.o $(srcPath)pkt_Queue.o LBeacon_Zigbee_Gateway.c -o LBeacon_Zigbee_Gateway.out
LBeacon_Zigbee_Beacon.out: $(srcPath)xbee_Serial.o $(srcPath)xbee_API.o $(srcPath)pkt_Queue.o LBeacon_Zigbee_Beacon.c
	$(CC) -lxbee -lwiringPi $(srcPath)xbee_Serial.o $(srcPath)xbee_API.o $(srcPath)pkt_Queue.o LBeacon_Zigbee_Beacon.c -o LBeacon_Zigbee_Beacon.out
UDP_Demo.out: $(srcPath)UDP_API.o $(srcPath)pkt_Queue.o UDP_Demo.c
	$(CC) -lpthread $(srcPath)UDP_API.o $(srcPath)pkt_Queue.o ./UDP_Demo.c -o UDP_Demo.out
pkt_Queue_Demo.out: $(srcPath)pkt_Queue.o pkt_Queue_Demo.c
	$(CC) $(srcPath)pkt_Queue.o ./pkt_Queue_Demo.c -o pkt_Queue_Demo.out
xbee_Serial_Demo.out: $(srcPath)xbee_Serial.o $(srcPath)xbee_API.o $(srcPath)pkt_Queue.o xbee_Serial_Demo.c
	$(CC) -lxbee $(srcPath)xbee_Serial.o $(srcPath)xbee_API.o $(srcPath)pkt_Queue.o ./xbee_Serial_Demo.c -o xbee_Serial_Demo.out
pkt_Queue.o:
	cd $(srcPath) && make pkt_Queue.o
xbee_Serial.o:
	cd $(srcPath) && make xbee_Serial.o
xbee_API.o:
	cd $(srcPath) && make xbee_API.o
UDP_API.o:
	cd $(srcPath) && make UDP_API.o
clean:
	rm -f *.o *.out *.h.gch
	cd $(srcPath) && make clean
