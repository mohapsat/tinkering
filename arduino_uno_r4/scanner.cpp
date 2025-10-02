// Sentry

#include <Servo.h>

Servo servo_0;

int pos = 0;    // servo position
int servo_0_pin = 9;   // use pin 9 for servo

int trig_pin = 8;
int echo_pin = 7;
float cm = 0;   // use float for better accuracy

int br = 9600; // serial baud rate
bool detected = false;

long readUltrasonicDistance(int triggerPin, int echoPin)
{
  pinMode(triggerPin, OUTPUT);  
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(2);
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);

  pinMode(echoPin, INPUT);
  return pulseIn(echoPin, HIGH, 30000UL); // 30ms timeout
}

void setup() {
  servo_0.attach(servo_0_pin); 
  Serial.begin(br);
}

void loop() {
  // sweep from 0 to 90
  for (pos = 0; pos <= 90; pos += 1) {
    servo_0.write(pos);
    detect_object();
    delay(20);
  }

  delay(5000);

  // sweep from 90 back to 0
  for (pos = 90; pos >= 0; pos -= 1) {
    servo_0.write(pos);
    detect_object();
    delay(20);
  }
  delay(5000);
}

void detect_object() {
  long duration = readUltrasonicDistance(trig_pin, echo_pin);

  if (duration > 0) {
    cm = duration / 58.0;  // µs to cm
    if (cm >= 2 && cm <= 140) {
      detected = true;
      Serial.print("Detected at ");
      Serial.print(cm, 1);
      Serial.println(" cm");
    } else {
      detected = false;
    }
  } else {
    detected = false;
  }
}
