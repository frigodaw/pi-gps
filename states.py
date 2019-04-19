import time
import datetime
import serial
import sys
from pigps import *

class States:
    def __init__(self, state_number, header_name, active_flag, transitions_to = [], states_list = []):
        self.state_number = state_number
        self.header_name = header_name
        self.active_flag = active_flag
        self.transitions_to = transitions_to
        states_list.append(self.state_number)

    def set_active_flag(self):
        self.active_flag = 1

    def reset_active_flag(self):
        self.active_flag = 0        
    
    def print_header(self):
        print("\n>>>  {}  <<<".format(self.header_name))

    def print_content(self):
        print("Number of screen: {}".format(self.state_number))
    
    def _main(self):
        self.set_active_flag()
        self.print_header()
        self.print_content()
        print("Active flag before: {}".format(self.active_flag))
        self.reset_active_flag()
        print("Active flag after: {}".format(self.active_flag))

class Menu(States):
    pass

class Cycling(States):
    pass

class Settings(States):
    pass

class Exit(States):
    pass


