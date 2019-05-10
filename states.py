import datetime
import serial
import sys
import threading
import time
from pigps import *

#Getter of dest variable. Used in process of changing state.
def where_to():
	return dest

class States:
    #Initial method which assigns given values to object and calls setup method
    def __init__(self, state_number, header_name, active_flag, transitions_to = [], states_list = []):
        self.setup()
        self.state_number = state_number
        self.header_name = header_name
        self.active_flag = active_flag
        self.transitions_to = transitions_to
        self.err = True
        self.ser = serial.Serial()
        states_list.append(self.state_number)

    #Setup connections with peripherals
    def setup(self):
        pass

    #Setter of object's active_flag varibale
    def set_active_flag(self):
        self.active_flag = 1

    #Getter of object's active_flag varibale
    def reset_active_flag(self):
        self.active_flag = 0        
    
    #Shows number of the active state
    def print_header(self):
        print("\n>>>  {}  <<<".format(self.header_name))

    #Shows number of the active state
    def print_screen_nr(self):
        print("Number of screen: {}".format(self.state_number))

    #Shows elements of the list where legal transitions are stored
    def print_transitions(self):
        print("Available transitions to: {}".format(self.transitions_to))

    #Read the number of demanding state from keyboard and returns it after validation
    def transition(self):
        global dest
        try:
            dest = int(input("Transition to: "))
        except: 
            ValueError      #To handle with incorrect input
        if dest in self.transitions_to:
            return dest
        else:
            return self.transition()
    
    #Execute print methods
    def say_hello(self):
        self.print_header()
        self.print_screen_nr()
        self.print_transitions()

    #Executes another methods
    def _main(self):
        self.say_hello()
        self.transition()

class Menu(States):
    pass

class Cycling(States):
    def setup(self):
        self.ser = serial.Serial(port="/dev/ttyS0", baudrate=9600, timeout=0.5)
        self.newdata = self.ser.readline()
        self.newdata = str(self.newdata, 'utf-8')

    def print_basic_data(self):
        try:
            self.newdata = self.ser.readline()
            self.newdata = str(self.newdata, 'utf-8')
            if(self.newdata.find("$GPGGA") == 0):
                print("\n{}".format(self.newdata.rstrip()))	#remove EOL
                print("Latitude:  {}".format(getLat(self.newdata)))
                print("Longitude: {}".format(getLon(self.newdata)))
                print("Altitude:  {}".format(getAlt(self.newdata)))
                print("    Time:  {}".format(getTime(self.newdata)))
                self.err = False
            else:
                self.print_basic_data()
        except:
            print("Trying to fix...")
            time.sleep(1)
            self.err = True

    def print_calc_data(self):
        if(self.err == False):
            print("   Speed:  -")
            print("Distance:  -")

    def _main(self):
        self.say_hello()
        self.setup()
        print("        __         ")
        print("      __\ \        ")
        print("    (___)) )       ")
        print("        /_/        ")
        self.transition()
        #threading.Thread(target=self.transition).start()
        #while (dest == self.state_number):
        #    self.print_basic_data()
        #    self.print_calc_data()

    def running(self):
        self.say_hello()
        self.setup()
        threading.Thread(target=self.transition).start()
        while (dest == self.state_number):
            self.print_basic_data()
            self.print_calc_data()

    def paused(self):
        self.say_hello()
        print("     __    __      ")
        print("    |  |  |  |     ")
        print("    |  |  |  |     ")
        print("    |__|  |__|     ")
        self.transition()



class Settings(States):
    pass

class Exit(States):
    def _main(self):
        self.say_hello()
        print("\n------- pi-gps 2019 -------")
        sys.exit()


