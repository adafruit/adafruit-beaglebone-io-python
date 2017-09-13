#!/usr/bin/env python3
# Reads the PAUSE button using interupts and sets the LED
# Pin table at https://github.com/beagleboard/beaglebone-blue/blob/master/BeagleBone_Blue_Pin_Table.csv

# Import PyBBIO library:
import Adafruit_BBIO.GPIO as GPIO
import time

button= "PAUSE"  # PAUSE=P8_9, MODE=P8_10
LED1  = "RED"
LED2  = "WIFI"

# Set the GPIO pins:
GPIO.setup(LED1,  GPIO.OUT)
GPIO.setup(LED2,  GPIO.OUT)
GPIO.setup(button, GPIO.IN)

print("Running...")

while True:
  state = GPIO.input(button)
  GPIO.output(LED1, state)
  GPIO.output(LED2, state)
  
  GPIO.wait_for_edge(button, GPIO.BOTH)
  print("Pressed")