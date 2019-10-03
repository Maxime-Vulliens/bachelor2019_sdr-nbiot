'''
main.py This script runs directly after boot.py and should contain

the main code you wish to run on your device.

Run is used with default built-in library to run on RAM
To have a persistent code  -> use upload feature (upload alos extra librairies)

To format flash we can use :
import os
os.mkfs('/flash')

REPL is an interactive shell accessible by COM port (115200)
'''

import pycom
import time

# Disable default blue blinking 
pycom.heartbeat(False)

# Blink leds
while True:
    pycom.rgbled(0xFF0000)  # Red
    time.sleep(1)
    pycom.rgbled(0x00FF00)  # Green
    time.sleep(1)
    pycom.rgbled(0x0000FF)  # Blue
    time.sleep(1)