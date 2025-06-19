import time
import board
import pwmio
import wifi
import socketpool
import digitalio
import adafruit_requests
import adafruit_connection_manager
from adafruit_motor import servo
from secrets import secrets

# Connect to Wi-Fi with retries
print("🌐 Connecting to Wi-Fi...")
max_attempts = 5
for attempt in range(1, max_attempts + 1):
    try:
        wifi.radio.connect(secrets["ssid"], secrets["password"])
        print("✅ Connected to", secrets["ssid"])
        print("📡 IP address:", wifi.radio.ipv4_address)
        break
    except Exception as e:
        print(f"❌ Attempt {attempt} failed:", e)
        time.sleep(2)
else:
    print("🚫 Could not connect to Wi-Fi after several attempts.")
    raise SystemExit

# Set up PWM servo on GP14
pwm = pwmio.PWMOut(board.GP14, frequency=50)
my_servo = servo.Servo(pwm)

led = digitalio.DigitalInOut(board.GP15)
led.switch_to_output()

# Create HTTP server
pool = socketpool.SocketPool(wifi.radio)
cm = adafruit_connection_manager.get_connection_manager(pool)
requests = adafruit_requests.Session(pool, cm)

server = pool.socket(pool.AF_INET, pool.SOCK_STREAM)
server.settimeout(1.0)
server.bind(("0.0.0.0", 80))
server.listen(1)

print("🚀 Ready! Use:")
print(f"curl http://{wifi.radio.ipv4_address}/servo?angle=90")

while True:
    try:
        conn, addr = server.accept()
        print(f"🔌 Connection from {addr}")
        
        buffer = bytearray(1024)
        size = conn.recv_into(buffer)
        request = bytes(buffer[:size]).decode("utf-8")
        print("📥 Request:", request)

        led.value = False

        # Parse angle from URL
        try:
            if "/servo?angle=" in request:
                angle_str = request.split("/servo?angle=")[1].split(" ")[0]
                angle = int(angle_str)
                if 0 <= angle <= 180:
                    led.value = True                    
                    my_servo.angle = angle
                    time.sleep(1)
                    led.value = False                    
                    response = f"HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n\r\nSet angle to {angle}"
                    
                else:
                    response = "HTTP/1.1 400 Bad Request\r\n\r\nAngle must be 0–180"
            else:
                response = "HTTP/1.1 404 Not Found\r\n\r\nUse /servo?angle=NN"
        except Exception as e:
            response = f"HTTP/1.1 500 Internal Server Error\r\n\r\nError: {e}"

        conn.send(response.encode("utf-8"))
        conn.close()

    except OSError as e:
        # Timeout or transient disconnects are ignored
        pass
    except Exception as e:
        print("⚠️ Unexpected error:", e)
        time.sleep(1)

