#!/usr/bin/env python
# -*- coding: utf-8 -*-

import time
import serial
from pigps import *

#serial setup
ser = serial.Serial(port="/dev/ttyS0", baudrate=9600, timeout=0.5)
i=1
GPGGA_basic = "$GPGGA,hhmmss.ss,llll.ll,a,yyyyy.yy,a,x,xx,x.x,x.x,M,x.x,M,x.x,xxxx"
GPGGA = "$GPGGA,134658.00,5106.97,N,11402.30,W,2,09,1.0,1048.4,M,-16.27,M,08,AAAA*60"
####### "$GPGGA,hhmmss.ss,llll.ll,a,yyyyy.yy,a,x,xx,x.x,xxxx.x,M,zxx.xx,M,x.x,xxxx"
GPGGA_commas = []
while True:	
	try:
		newdata = ser.readline()
		newdata = str(newdata, 'utf-8')
		if(newdata.find("$GPGGA") == 0):
			#newdata = GPGGA		#replace data with constat string
			print("{}) {}".format(i, newdata.rstrip()))	#remove EOL
			print("Time: {}".format(getTime(newdata)))
			print("Lon: {}".format(getLon(newdata)))
			print("Lat: {}".format(getLat(newdata)))
			print("Alt: {}".format(getAlt(newdata)))
			print("Fix quality: {}".format(getFixQuality(newdata)))
			
			
			for j in range(len(GPGGA)):
				if(newdata[j] == ","):
					GPGGA_commas.append(i)
			for j in range(len(GPGGA_commas)):
				#print("nr {} : {}". format(j+1, GPGGA_commas[j]))
				pass
			GPGGA_commas = []
			miejsce = 6
			index = findComma(newdata, miejsce)	
			#print("przecinek nr {} jest ma indeks: {}".format(miejsce, index))

		if(newdata.find("$GPRMC") == 0):
			print("{}) {}".format(i, newdata.rstrip()))	#remove EOL
			print("Date: {}".format(getDate(newdata)))
			print("TimeISO: {}".format(getTimeISO(newdata)))
			print('\n')
			i+=1
		

	except:
		print("Trying to fix...")

