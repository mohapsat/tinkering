import board
import digitalio
import time

led = digitalio.DigitalInOut(board.GP15)
led.direction = digitalio.Direction.OUTPUT

switch = digitalio.DigitalInOut(board.GP14)
switch.direction = digitalio.Direction.INPUT
switch.pull = digitalio.Pull.UP # to avoid floating GP14 when it's not grounded


while True:
    print("Switch value:", switch.value)
    
    if switch.value:
        led.value = True
    else:
        led.value = False
    
    time.sleep(0.2)
