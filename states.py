import time
import datetime
import serial
import sys
from pigps import *

#Getter of dest variable. Used in process of changing state.
def where_to():
	return dest

class States:
    #Initial method which assigns given values to object.
    def __init__(self, state_number, header_name, active_flag, transitions_to = [], states_list = []):
        self.state_number = state_number
        self.header_name = header_name
        self.active_flag = active_flag
        self.transitions_to = transitions_to
        states_list.append(self.state_number)

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
    pass

class Settings(States):
    pass

class Exit(States):
    def _main(self):
        self.say_hello()
        print("\n------- pi-gps 2019 -------")
        sys.exit()


