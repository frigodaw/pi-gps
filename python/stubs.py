def state_machine(demanded_state):
    switcher = {
        0: menu,
        1: cycling,
        2: settings,
        3: exit
    }
    func = switcher.get(demanded_state, "Not found!")
    return func()


def allowed_transition(demanded_state, allowed_states = []):
    for i in allowed_states:
        if(demanded_state == allowed_states[i]):
            return 1
    return 0


#std_ret == 0
def menu(): 
    print("\n>>>  MAIN MENU  <<<")
    print("1. cycling")
    print("2. settings")
    print("3. exit")
    std_ret = 0
    try:
        std_ret = int(input("Transition: "))
    except:
        std_ret = 0
    return std_ret
    
#std_ret == 1
def cycling():
    print("\n>>>  CYCLING  <<<")
    print("        __         ")
    print("      __\ \        ")
    print("    (___)) )       ")
    print("        /_/        ")

    try:
        std_ret = int(input("Transition: "))
    except:
        std_ret = 0
    return std_ret   

#std_ret == 10
def cycling_start():
    print("\n>>>  CYCLING  <<<")
    print("   Speed: ")
    print("Distance: ")
    print("Altitude: ")
    print("    Time: ")
    try:
        std_ret = int(input("Transition: "))
        if(allowed_transition(std_ret, arr = (1, 10, 11) != 1)):
            std_ret = 1
    except:
        std_ret = 10
    return std_ret   

#std_ret == 11
def cycling_pause():
    print("\n>>>  CYCLING  <<<")
    print("     __    __      ")
    print("    |  |  |  |     ")
    print("    |  |  |  |     ")
    print("    |__|  |__|     ")
    try:
        std_ret = int(input("Transition: "))
    except:
        std_ret = 0
    return std_ret   

#std_ret == 2
def settings():
    print("\n>>>  SETTINGS  <<<")
    try:
        std_ret = int(input("Transition: "))
    except:
        std_ret = 0
    return std_ret   

#std_ret == 3
def exit():
    print("\n>>>  EXIT  <<<")
    print("------- pi-gps 2019 -------")
    sys.exit()


def init():
    try:
        #serial setup
        ser = serial.Serial(port="/dev/ttyS0", baudrate=9600, timeout=0.5)

        #create file
        filename = str(datetime.datetime.now().isoformat()) + '.gpx'

        #xml data
        xml_version = "1.0"
        encoding = "UTF-8"
        standalone="yes"

        #gpx data
        gpx_version="1.1" 
        creator="pi-gps 2019" 
        xsi_schemaLocation="http://www.topografix.com/GPX/1/1 http://www.topografix.com/GPX/1/1/gpx.xsd"
        xmlns="http://www.topografix.com/GPX/1/1"
        xmlns_gpxtpx="http://www.garmin.com/xmlschemas/TrackPointExtension/v1"
        xmlns_xsi="http://www.w3.org/2001/XMLSchema-instance"
            
        #metadata
        name="pi-gps"
        activity = "Cycling"

        #return
        std_ret = 1

    except:
        std_ret = 0
        
    return std_ret

#start from main menu
number_of_active_state = 0
print("------- pi-gps 2019 -------")
while True:
    std_ret = state_machine(std_ret)