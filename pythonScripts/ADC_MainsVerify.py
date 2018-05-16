#!/usr/bin/env python
# -*- coding: utf-8 -*-

import time
import serial

print "Starting Timer1 to verify 50Hz AC signal"

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
rx1 = 0
tx = 0
rxFlag = 0
handshakeVerify = 0

#Connecting with PIC
try:
	print "Establishing connection 2 with PIC"
	print "Sending handshake 2 to PIC"
	tx = "2"
	ser.write(tx)
	time.sleep(.01)

except:
	print "Error: Sending handshake 2"

try:
	#Reading from UART
	print "Reading....."
	while (rxFlag != 1):
                if ser.inWaiting()>0:
			rx = ser.readline()
			ser.flushInput()
			rxFlag = 1
			break

except:
	print "Error: Reading handshake 2"

try:
	if "2" in rx:
		print "Connection 2 Established"
		ser.flushInput()
	else:
		print "Connection 2 is bad"
		
except:
	print "Error! "

#Parse POW from data.txt and send to PIC
try:
        verifyFreq = 0
        while verifyFreq != 1:
                print "Waiting for ADC/Timer1..."
		#Reading from Serial
		rxFlag = 0
                while (rxFlag != 1):
                        if ser.inWaiting() > 0:
                                rx = ser.readline()
                                rxFlag = 1
				break

		print ("Bool: Received " + rx)
		if "1" in rx:
                	print "50Hz frequeny is verified"
			verifyFreq = 1

		else:
			print "Frequency is out of 50Hz range"
except:
        "Error: Could not verify frequency of signal"

try:
	#Reading from UART
	print "Waiting for decimal value of ADC reading..."
	time.sleep(.1)
	rxFlag = 0
	while (rxFlag != 1):
                if ser.inWaiting()>0:
			rx1 = ser.readline()
			ser.flushInput()
			rxFlag = 1

	print ("ADC decimal value: " + rx1)
	ser.close()

except:
	print "Error: Reading handshake"


