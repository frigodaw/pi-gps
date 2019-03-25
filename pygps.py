def getTime(data):
	time = data[7:9] + ':' + data[9:11] + ':' + data[11:13]
	return time
	

def getLon(data):
	tmp = data[17:20] + data[20:27]
	lon = str(float(tmp[0:2]) + float(tmp[3:10])/60)
	lon = float(lon[:8])
	if (data[28] == 'S'):
		lon *= -1
	return lon

def getLat(data):
	tmp = data[30:33] + data[33:41]
	lat = str(float(tmp[0:3]) + float(tmp[4:11])/60)
	lat = float(lat[:8])
	if (data[42] == 'W'):
		lat *= -1
	return lat

def getFixQuality(data):
	fix = int(data[44])
	return fix

def getAlt(data):
	alt = float(data[54:59])
	return alt