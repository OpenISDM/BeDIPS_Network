CC = gcc -std=gnu99

pkt_Queue.o: pkt_Queue.c pkt_Queue.h
	$(CC) pkt_Queue.c -c
xbee_API.o: xbee_API.c xbee_API.h
	$(CC) xbee_API.c -c
UDP_API.o: UDP_API.c UDP_API.h
	$(CC) UDP_API.c -c
xbee_Serial.o: xbee_Serial.c xbee_Serial.h
	$(CC) xbee_Serial.c -c
clean:
	rm -f *.o *.out *.h.gch
