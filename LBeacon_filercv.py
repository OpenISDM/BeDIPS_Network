from xbee import XBee, ZigBee
import serial
from time import sleep

def func_resemble (file_name):
    f = open(file_name,'wb')
    response = xbee.wait_read_frame()
    
    # TODO: not address packet loss

    order_count = 1
    resemble_dic = {}
    while (response['rf_data'] != 'EOF'):
        # receive fragment in order
        if int(response['rf_data'].split('_*')[0]) == order_count:
            f.write(response['rf_data'].split('_*')[1])
        # not in order
        else: 
            if resemble_dic.get(order_count) == None:
                resemble_dic[order_count] = response['rf_data'].split('_*')[1]
                print response['rf_data'].split('_*')[0]
            else:
                f.write(resemble_dic[order_count])

        order_count = order_count + 1
        response = xbee.wait_read_frame()
    
    print "File " + file_name + " saved."
    f.close()


ser = serial.Serial('/dev/ttyUSB0', 9600)
xbee = ZigBee(ser, escaped=True)

while True:
    try:
        
        func_resemble('instruction_detail')
        break

    except KeyboardInterrupt:
        break

# # Asynchronous Mode
# def print_data(data):
#     """
#     This method is called whenever data is received
#     from the associated XBee device. Its first and
#     only argument is the data contained within the
#     frame.
#     """
#     print data

# xbee = ZigBee(ser, callback=print_data)

# while True:
#     try:
#         sleep(0.001)
#     except KeyboardInterrupt:
#         break

# xbee.halt()

ser.close()


