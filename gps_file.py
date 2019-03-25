#!/usr/bin/env python
# -*- coding: utf-8 -*-

import time
import datetime
import serial
from pygps import *

#serial setup
ser = serial.Serial(port="/dev/ttyS0", baudrate=9600, timeout=0.5)

#create file
#filename = str(datetime.datetime.now().isoformat()) + '.gpx'
filename = 'log.gpx'
i=1

#xml data
xml_version = "1.0"
encoding = "UTF-8"
standalone="yes"

#gpx data
gpx_version="1.1" 
creator="TomTom.2014 with Barometer" 
xsi_schemaLocation="http://www.topografix.com/GPX/1/1 http://www.topografix.com/GPX/1/1/gpx.xsd"
xmlns="http://www.topografix.com/GPX/1/1"
xmlns_gpxtpx="http://www.garmin.com/xmlschemas/TrackPointExtension/v1"
xmlns_xsi="http://www.w3.org/2001/XMLSchema-instance"
    
#metadata
name="pi-gps"
activity = "Cycling"

try:
	f=open(filename,"w+")
	
	f.write("<?xml version=\"{}\" encoding=\"{}\" standalone=\"{}\"?>\n".format(xml_version, encoding, standalone))
	f.write("<gpx version=\"{}\" creator=\"{}\" xsi:schemaLocation=\"{}\" xmlns=\"{}\" xmlns:gpxtpx=\"{}\" xmlns:xsi=\"{}\">\n".format(gpx_version, creator, xsi_schemaLocation, xmlns, xmlns_gpxtpx, xmlns_xsi))
	f.write("\t<metadata>\n\t\t<name>{}</name>\n\t</metadata>\n".format(name))
	f.write("\t<trk>\n")
	f.write("\t\t<name>{}</name>\n".format(activity))
	f.write("\t\t<trkseg>\n")
		
	
	while (i<61):
		try:
			newdata = ser.readline()
			newdata = str(newdata, 'utf-8')
			if(newdata.find("$GPGGA") == 0):
				print("Step nr: {}".format(i))
				f.write("\t\t\t<trkpt lat=\"{}\" lon=\"{}\">\n".format(getLat(newdata), getLon(newdata)))
				f.write("\t\t\t\t<ele>{}</ele>\n".format(getAlt(newdata)))
				f.write("\t\t\t\t<time>{}</time>\n".format(datetime.datetime.now().isoformat()))
				f.write("\t\t\t</trkpt>\n")
				
				i+=1
				time.sleep(1)
		except:
			pass
	f.write("\t\t</trkseg>\n")
	f.write("\t</trk>\n")
	f.write("</gpx>\n")
	
finally:
	f.close()
	



