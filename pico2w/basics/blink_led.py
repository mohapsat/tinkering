import board
import digitalio
import time


led = digitalio.DigitalInOut(board.GP15)
led.direction = digitalio.Direction.OUTPUT

while True:
    led.value = True
    time.sleep(1)
    led.value = False
    time.sleep(1)

"""
https://datasheets.raspberrypi.com/picow/pico-2-w-pinout.pdf

"""
