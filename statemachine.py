#!/usr/bin/env python
# -*- coding: utf-8 -*-

import time
import datetime
import serial
from pigps import *
from states import *

#Screen objects
states_list = []
menu     =     Menu(0, "MENU",     0, [1,3], states_list)
cycling  =  Cycling(1, "CYCLING",  0, [2,3], states_list)
settings = Settings(2, "SETTINGS", 0, [0,3], states_list)
exit     =     Exit(3, "EXIT",     0, [3], states_list)

#Function to change screen content
def start_state(state_nr):
    switcher = {
        0: menu._main,
        1: cycling._main,
        2: settings._main,
        3: exit._main
    }
    func = switcher.get(state_nr, "Not found!")
    return func()    

#Initial assignment
initial_state = menu.state_number
current_state = initial_state

#Start of the state machine loop
print("------- pi-gps 2019 -------")
while True:
	start_state(current_state)
	current_state = where_to()
    



    

