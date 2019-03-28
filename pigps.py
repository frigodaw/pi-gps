def getTime(data):			#GPPGA
	if(data.find("$GPGGA") == 0):
		time = data[7:9] + ':' + data[9:11] + ':' + data[11:13]
		return time
	else:
		return -1
			
def getLat(data):			#GPPGA
	if(data.find("$GPGGA") == 0):
		tmp = data[17:20] + data[20:27]
		lat = str(float(tmp[0:2]) + float(tmp[3:10])/60)
		lat = float(lat[:8])
		if (data[28] == 'S'):
			lat *= -1
		return lat
	else:
		return -1
			
def getLon(data):			#GPPGA
	if(data.find("$GPGGA") == 0):
		tmp = data[30:33] + data[33:41]
		lon = str(float(tmp[0:3]) + float(tmp[4:11])/60)
		lon = float(lon[:8])
		if (data[42] == 'W'):
			lon *= -1
		return lon
	else:
		return -1
			
def getFixQuality(data):	#GPPGA
	if(data.find("$GPGGA") == 0):
		fix = int(data[44])
		return fix
	else:
		return -1
	
def getAlt(data):			#GPPGA
	if(data.find("$GPGGA") == 0):
		alt = float(data[54:59])
		return alt
	else:
		return -1



def getDate(data):			#GPRMC
	if(data.find("$GPRMC") == 0):	#check if line is correct
		if(data.count(',') == 11):	#check if number of commas if correct 
			for i in data:			#create an array of commas' indexes
				if(data[i] == ','):
					comArr.append(i)
			date = data[17:23]
			return date
	else:
		return -1
				
def getTimeISO(data):		#GPRMC
	if(data.find("$GPRMC") == 0):
		time = '20' + data[45:47] + '-' + data[47:49] + '-' + data[49:51] + 'T'
		time += data[7:9] + ':' + data[9:11] + ':' + data[11:13]
		return time
	else:
		return -1