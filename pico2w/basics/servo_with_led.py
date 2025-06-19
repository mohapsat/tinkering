import time
import board
import digitalio
import pwmio
from adafruit_motor import servo

# Set up PWM on GP14
pwm = pwmio.PWMOut(board.GP14, frequency=50)
led = digitalio.DigitalInOut(board.GP15)
led.switch_to_output()

servo_gp14 = servo.Servo(pwm)

angle = 0
step = 1  # direction

while True:
    servo_gp14.angle = angle
    led.value = not led.value  # toggle LED for vis
    time.sleep(0.02)  # small delay

    angle += step

    if angle >= 180 or angle <= 0:
        step = -step  # reverse

