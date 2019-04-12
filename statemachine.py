#!/usr/bin/env python
# -*- coding: utf-8 -*-

import time
import datetime
import serial
from pigps import *
from states import *


def state_machine(demanded_state):
    switcher = {
        0: menu,
        1: cycling,
        2: settings,
        3: exit
    }
    func = switcher.get(demanded_state, "Not found!")
    return func()


    
#start from main menu
std_ret = 0
print("------- pi-gps 2019 -------")
while True:
    std_ret = state_machine(std_ret)
