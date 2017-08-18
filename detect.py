#!/usr/bin/env python3

from ard_core import *
import serial

with serial.Serial(find_port(), 9600) as ser:
    while True:
        data = ser.readline()
        if data:
            print(data)
