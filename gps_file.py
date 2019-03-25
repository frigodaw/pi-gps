#!/usr/bin/env python
# -*- coding: utf-8 -*-

import time
import serial
from pygps import *

#serial setup
ser = serial.Serial(port="/dev/ttyS0", baudrate=9600, timeout=0.5)
i=1

while True:
	#try:
		newdata = ser.readline()
		newdata = str(newdata, 'utf-8')
		if(newdata.find("$GPGGA") == 0):

			print("{}) {}".format(i, newdata.rstrip()))	#remove EOL
			print("Time: {}".format(getTime(newdata)))
			print("Lon: {}".format(getLon(newdata)))
			print("Lat: {}".format(getLat(newdata)))
			print("Alt: {}".format(getAlt(newdata)))
			print("Fix quality: {}".format(getFixQuality(newdata)))
			print('\n')

			i+=1
			time.sleep(1)
	#except:
		#print("Trying to fix...")

