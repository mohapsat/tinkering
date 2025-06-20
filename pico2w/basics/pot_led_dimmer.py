import board
import analogio
import pwmio
import time

# Potentiometer on A0
pot = analogio.AnalogIn(board.A0)

# PWM LED on GP15
led = pwmio.PWMOut(board.GP15, frequency=5000, duty_cycle=0)

while True:
    pot_val = pot.value  # 0–65535
    brightness = int(pot_val / 65535 * 65535)  # Scale to duty_cycle
    led.duty_cycle = brightness
    print("Pot:", pot_val, "→ Brightness:", brightness)
    time.sleep(0.5)

