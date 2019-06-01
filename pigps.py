# templates of data packs
GPGGA = "$GPGGA,hhmmss.ss,llll.ll,a,yyyyy.yy,a,x,xx,x.x,x.x,M,x.x,M,x.x,xxxx"
# 				6		 16	    24 26      35 37 39 42 46 50 52 56 58  62

#returns index of nr'th comma in data, nr has range(1,2,3,...)
def findComma(data, nr):
	ret = 0	
	for i in range(len(data)):
		if(data[i] == ","):
			nr = nr-1
			if(nr == 0):
				ret = i
			if((ret > 0) and (i == ret+1)): 	#empty slot!
				return 0
			if((ret > 0) and (i > ret+1)): 		#found before
				return ret
	return ret

def getTime(data):			#GPPGA
	k = findComma(data, 1)	#offset
	if(data.find("$GPGGA") == 0 and k>0):
		try:
			time = data[k+1:k+3] + ':' + data[k+3:k+5] + ':' + data[k+5:k+7]
			return time
		except:
			return -1
	else:
		return -1
			
def getLat(data):			#GPPGA
	k = findComma(data, 2)	#offset
	if(data.find("$GPGGA") == 0 and k>0):
		try:
			tmp = data[k+1:k+4] + data[k+4:k+8]
			lat = str(float(tmp[0:2]) + float(tmp[3:8])/60)
			lat = float(lat[:8])
			if (data[k+9] == 'S'):
				lat *= -1
			return lat
		except:
			return -1
	else:
		return -1
			
def getLon(data):			#GPPGA
	k = findComma(data, 4)	#offset
	if(data.find("$GPGGA") == 0 and k>0):
		try:
			tmp = data[k+1:k+4] + data[k+4:k+9]
			lon = str(float(tmp[0:3]) + float(tmp[4:8])/60)
			lon = float(lon[:8])
			if (data[k+10] == 'W'):
				lon *= -1
			return lon
		except:
			return -1
	else:
		return -1
			
def getFixQuality(data):	#GPPGA
	k = findComma(data, 6)	#offset
	if(data.find("$GPGGA") == 0 and k>0):
		try:
			fix = int(data[k+1])
			return fix
		except:
			return -1
	else:
		return -1
	
def getAlt(data):			#GPPGA
	k = findComma(data, 9)	#offset
	end = findComma(data, 10)
	if(data.find("$GPGGA") == 0 and k>0):
		try:
			alt = float(data[k+1:end])
			return alt
		except:
			return -1
	else:
		return -1


def getDate(data):			#GPRMC
	if(data.find("$GPRMC") == 0):	#check if line is correct
		try:
			if(data.count(',') == 11):	#check if number of commas if correct 
				for i in data:			#create an array of commas' indexes
					if(data[i] == ','):
						comArr.append(i)
				date = data[17:23]
				return date
		except:
			return -1
	else:
		return -1
				
def getTimeISO(data):		#GPRMC
	if(data.find("$GPRMC") == 0):
		try:
			time = '20' + data[45:47] + '-' + data[47:49] + '-' + data[49:51] + 'T'
			time += data[7:9] + ':' + data[9:11] + ':' + data[11:13]
			return time
		except:
			return -1
	else:
		return -1