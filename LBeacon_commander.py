"""
his is a remote client (in the same local network) to connect to Xbee gateway. 
Set instruction below.
"""
import socket, sys

# Local IP address of Gateway
host = '192.168.1.176'
port = 1234


try:
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
except socket.error, msg:
    sys.stderr.write("[ERROR] %s\n" % msg[1])
    sys.exit(1)
 
try:
    sock.connect((host, port))
except socket.error, msg:
    sys.stderr.write("[ERROR] %s\n" % msg[1])
    exit(1)

"""
Instruction set here

1. trans file_name dest_addr
2. cmd message dest_addr

"""

# broacast address test (not for file transfer)
# dest_addr = '\x00\x00\x00\x00\x00\x00\xFF\xFF'
# specific dest
dest_addr = '\x00\x13\xA2\x00\x40\xE7\xDA\x61'
# dest_addr = '\x00\x13\xA2\x00\x40\xE7\xDA\x5F'

# instruction = raw_input("?")
file_name = 'instruction_detail'
# ===instruction #1=== Tranfer file
instruction = 'trans ' + file_name + ' ' + dest_addr

# ===instruction #2=== Command (short msg)
# which direction, table saved in beacon-- by Jane
data = '2'
# instruction = 'cmd ' + data + ' ' + dest_addr

sock.send(instruction)



response = sock.recv(1024)
if response == 'ACK1':
    f = open(file_name,'rb')
    print 'Sending file ( '+ file_name +' ) ...'
    l = f.read(1024)
    while (l):
        # print 'Sending to gateway...'
        sock.send(l)
        l = f.read(1024)
    f.close()
    print "File sent to gateway."
    sock.shutdown(socket.SHUT_WR)
elif response == 'ACK2':
    print 'GW: Received command.'
    print sock.recv(1024)

else:
    print response

sock.close()
