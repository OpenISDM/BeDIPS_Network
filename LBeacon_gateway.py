import socket, serial
from xbee import XBee, ZigBee
from time import sleep

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
        # print msg
        if not msg:
            pass

        # 1. for allocation (file transfer)
        elif msg.split(' ')[0] == 'trans':
            # ACK message
            clientsocket.send('ACK1')

            file_name = msg.split(' ')[1]
            dest_addr = msg.split(' ')[2]
            print file_name, dest_addr

            # Save packet from commander
            f = open(file_name,'wb')
            msg = clientsocket.recv(1024)
            print "Receiving from remote..."
            while (msg):
                f.write(msg)
                msg = clientsocket.recv(1024)
            print "File Received from remote."
            f.close()

            # Send to beacons
            f = open(file_name,'rb')
            # dest_addr = '\x00\x13\xA2\x00\x40\xE7\xDA\x61'
            # dest_addr = '\x00\x13\xA2\x00\x40\xE7\xDA\x5F'
            
            # ===
            # Max packet payload in XBee (broadcast: 84 bytes, single: 255)
            # XBee Fragmentation when file > 255
            # ===
            xbee_fragment = f.read(250)
            frag_num = 1
            while (xbee_fragment):
                xbee_fragment =  str(frag_num) + "_*" + xbee_fragment
                # min transfer interval by testing
                sleep(0.35)
                xbee.tx(dest_addr_long = dest_addr
                    , dest_addr = '\xFF\xFE'
                    , data = xbee_fragment)
                xbee_fragment = f.read(250)
                frag_num = frag_num + 1
                # delay for sent packet too fast

            f.close()

            # EOF packet
            sleep(0.5)
            xbee.tx(dest_addr_long = dest_addr
                    , dest_addr = '\xFF\xFE'
                    , data = 'EOF')

            clientsocket.send("GW: File Sent to "+ dest_addr.encode("HEX"))
            clientsocket.close()

        # 2. for command (short message)
        elif msg.split(' ')[0] == 'cmd':
            print "Client send: " + msg
            # ACK message
            clientsocket.send('ACK2')
            # clientsocket.send("Receviced command.\n")

            data = msg.split(' ')[1]
            dest_addr = msg.split(' ')[2]

            # str msg to dict msg
            # msg_dic = eval(msg)

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
            # xbee.tx(dest_addr_long = msg_dic['dest_addr'], dest_addr = '\xFF\xFE'
            #   , data = msg_dic['data'])

            xbee.tx(dest_addr_long = dest_addr
                    , dest_addr = '\xFF\xFE'
                    , data = data)
            print "XBee command sent."
            clientsocket.send("Command sent to " + dest_addr.encode("HEX"))
            clientsocket.close()

        else:
            clientsocket.send(msg.split(' ')[0] + ' not found...')
            clientsocket.close()


    except KeyboardInterrupt:
        serversocket.close()
        break


serversocket.close()


