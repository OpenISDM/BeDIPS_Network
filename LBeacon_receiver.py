from xbee import XBee, ZigBee
import serial

ser = serial.Serial('/dev/ttyUSB0', 9600)

xbee = ZigBee(ser, escaped=True)
# xbee = XBee(ser)

while True:
    try:
        response = xbee.wait_read_frame()
        print response
    except KeyboardInterrupt:
        break

ser.close()
