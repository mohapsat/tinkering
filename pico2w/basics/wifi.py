import time
import wifi
import socketpool
import ipaddress
import adafruit_requests
from secrets import secrets
import board
import digitalio


"""
Prerequisite: Create a secrets.py and place it in CIRCUITPY folder, next to code.py
not in any folder.

secrets = {
    "ssid": "<name of wifi>",
    "password": "<wifi_password>"
}
"""

print("Connecting to Wi-Fi...")

led = digitalio.DigitalInOut(board.LED)
led.direction = digitalio.Direction.OUTPUT


try:
    wifi.radio.connect(secrets["ssid"], secrets["password"])
    print("✅ Connected to", secrets["ssid"])
    print("🌐 IP address:", wifi.radio.ipv4_address)
    
    while True:
        led.value = True
        time.sleep(0.2)
        led.value = False
        time.sleep(0.2)
    
except Exception as e:
    print("❌ Failed to connect:", e)

# Optional: Blink onboard LED when connected
