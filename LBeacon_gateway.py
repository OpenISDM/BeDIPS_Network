import socket, serial
from xbee import XBee, ZigBee


# host = '192.168.1.176' ifconfig
port = 1234

# Comm through USB socket
ser = serial.Serial('/dev/ttyUSB0', 9600)
xbee = ZigBee(ser)

# create an INET, STREAMing socket
serversocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
# bind the socket to a public host,and a well-known port
serversocket.bind(('', port))
# become a server socket
serversocket.listen(3)

while True:
    try:
        (clientsocket, address) = serversocket.accept()
        print "Client Info: ", clientsocket, address
        msg = clientsocket.recv(1024)
        if not msg:
            pass
        else:
            print "Client send: " + msg
            clientsocket.send("Receviced command.\n")

            # str msg to dict msg
            msg_dic = eval(msg)

            # Xbee command
            # Send transmit_request
            """
            The coordinator can be addressed by either setting the 64-bit address to 
            all 0x00s and the 16-bit address to 0xFFFE, OR by setting the 64-bit address 
            to the coordinator's 64-bit address and the 16-bit address to 0x0000. 

            For all other transmissions, setting the 16-bit address to the correct 16- bit 
            address can help improve performance when transmitting to multiple destinations. 
            If a 16-bit address is not known, this field should be set to 0xFFFE (unknown).
            """
            xbee.tx(dest_addr_long = msg_dic['dest_addr'], dest_addr = '\xFF\xFE'
              , data = msg_dic['data'])
            print "XBee command sent."

            clientsocket.close()
    except KeyboardInterrupt:
        break

serversocket.close()

