# templates of data packs
GPGGA = "$GPGGA,hhmmss.ss,llll.ll,a,yyyyy.yy,a,x,xx,x.x,x.x,M,x.x,M,x.x,xxxx"
# 				6		 16	    24 26      35 37 39 42 46 50 52 56 58  62

#returns index of nr'th comma in data, nr start from 1
def findComma(data, nr):
	ret = -1	
	for i in range(len(data)):
		if(data[i] == ","):
			nr = nr-1
			if(nr == 0):
				ret = i
			if((ret > 0) and (i == ret+1)): 	#empty slot!
				ret = 0
				return ret
			if((ret > 0) and (i > ret+1)): 		#found before
				return ret
	return ret

def findDot(data, start, stop):
	ret = -1
	for i in range(start, stop):
		if(data[i] == "."):
			ret = i
			return ret
	return ret

#returns time in string format
def getTime(data):				#GPPGA
	k = findComma(data, 1)	 	#comma before expression
	end = findComma(data, 2) 	#comma after expression
	time_zone = 2				#current time zone
	if(data.find("$GPGGA") == 0 and k>0 and end-k == 10):
		try:
			time_total_s = int(data[k+1:k+3])*3600 + int(data[k+3:k+5])*60 + int(data[k+5:k+7]) + time_zone*3600
			if(time_total_s >= 86400):
				time_total_s -= 86400
			time_h = int(time_total_s/3600)
			time_m = int((time_total_s - time_h*3600)/60)
			time_s = int(time_total_s - time_h*3600 - time_m*60)
			time = str(time_h) + ':' + str(time_m) + ':' + str(time_s)
			i = 0
			while(len(time) != 8):
				if(i==7 and len(time) == 7):
					time = time[0:i-1] + '0' + time[i-1:]
				if(time[i] == ':' and (time[i-2] == ':' or i == 1)):
					time = time[0:i-1] + '0' + time[i-1:]		
					i += 1
				i += 1
			return time					
		except:
			return -1
	else:
		return -1

#returns postition's latitude		
def getLat(data):				#GPPGA
	k = findComma(data, 2)	 	#comma before expression
	end = findComma(data, 3) 	#comma after expression
	dot = findDot(data, k, end) #dot in expression
	N = 6 	#max number of digits after comma 
	if(data.find("$GPGGA") == 0 and k>0):
		try:
			tmp = data[k+1:end]
			lat = str(float(tmp[0:dot-k-3]) + float(tmp[dot-k-3:])/60)
			dot_in = findDot(lat, 0, len(lat))
			lat = float(lat[:dot_in+N+1])
			if (data[end+1] == 'S'):
				lat *= -1
			return lat
		except:
			return -1
	else:
		return -1

#returns postition's longitude		
def getLon(data):				#GPPGA
	k = findComma(data, 4)	 	#comma before expression
	end = findComma(data, 5) 	#comma after expression
	dot = findDot(data, k, end) #dot in expression
	N = 6 	#max number of digits after comma
	if(data.find("$GPGGA") == 0 and k>0):
		try:
			tmp = data[k+1:end]
			lon = str(float(tmp[0:dot-k-3]) + float(tmp[dot-k-3:])/60)
			dot_in = findDot(lon, 0, len(lon))
			lon = float(lon[:dot_in+N+1])
			if (data[end+1] == 'W'):
				lon *= -1
			return lon
		except:
			return -1
	else:
		return -1
			
#returns fix quality
def getFixQuality(data):		#GPPGA
	k = findComma(data, 6)	 	#comma before expression
	if(data.find("$GPGGA") == 0 and k>0):
		try:
			fix = int(data[k+1])
			return fix
		except:
			return -1
	else:
		return -1
	
#returns altitude
def getAlt(data):				#GPPGA
	k = findComma(data, 9)	 	#comma before expression
	end = findComma(data, 10)	#comma after expression
	if(data.find("$GPGGA") == 0 and k>0):
		try:
			alt = float(data[k+1:end])
			return alt
		except:
			return -1
	else:
		return -1

#returns date
def getDate(data):			#GPRMC
	if(data.find("$GPRMC") == 0):	#check if line is correct
		try:
			if(data.count(',') == 11):	#check if number of commas if correct 
				for i in data:			#create an array of commas' indexes
					if(data[i] == ','):
						comArr.append(i) #??? correct this
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