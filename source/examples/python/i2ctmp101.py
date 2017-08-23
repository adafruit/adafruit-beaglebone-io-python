#!/usr/bin/env python3
# Read a TMP101 sensor

import smbus
import time
bus = smbus.SMBus(1)
address = 0x49

while True:
    temp = bus.read_byte_data(address, 0)
    print (temp, end="\r")
    time.sleep(0.25)
