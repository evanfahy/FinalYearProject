#!/usr/bin/env python
# -*- coding: utf-8 -*-

import time
import serial

print "Initialising UART serial communication with PIC16."
time.sleep(.5)

ser = serial.Serial(
 port='/dev/ttyUSB0',
 baudrate = 10417,
 parity=serial.PARITY_NONE,
 stopbits=serial.STOPBITS_ONE,
 bytesize=serial.EIGHTBITS,
 timeout=1
)

time.sleep(.3)

ser.flushInput()
ser.flushOutput()

rx = 0
tx = 0
rxFlag = 0
handshakeVerify = 0

print ("Serial communication connected via: " + ser.portstr)

#Connecting with PIC
try:
	print "Establishing connection 1 with PIC"
	print "Sending handshake 1 to PIC"
	tx = "1"
	ser.write(tx)
	time.sleep(.01)

except:
	print "Error: Sending handshake 1"

try:
	#Reading from UART
	print "Reading....."
	while (rxFlag != 1):
                if ser.inWaiting()>0:
			rx = ser.readline()
			print ("RX: " + rx)
			rxFlag = 1
			break

except:
	print "Error: Reading handshake 1"

try:
	if "1" in rx:
		print "Connection 1 Established"
	else:
		print "Connection 1 is bad"
		
except:
	print "Error!"
finally:
	ser.close()
	print "------------------"
