#include <Servo.h>

Servo servo_0;

int pos = 0;
int servo_0_pin = 9;   

int trig_pin = 8;
int echo_pin = 7;
float cm = 0;

int br = 9600; 
bool detected = false;

int bestPos = 0;         // angle where object is closest
float bestDist = 9999.0; // shortest distance seen in a sweep

long readUltrasonicDistance(int triggerPin, int echoPin)
{
  pinMode(triggerPin, OUTPUT);  
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(2);
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);

  pinMode(echoPin, INPUT);
  return pulseIn(echoPin, HIGH, 30000UL); // timeout
}

void setup() {
  servo_0.attach(servo_0_pin); 
  Serial.begin(br);
}

void loop() {
  bestDist = 9999.0;
  bestPos = 0;

  // sweep 0 -> 90 and look for closest object
  for (pos = 0; pos <= 90; pos += 2) {
    servo_0.write(pos);
    delay(20); // smooth motion
    float d = getDistance();
    if (d > 2 && d < 140) {     // valid range
      if (d < bestDist) {
        bestDist = d;
        bestPos = pos;
      }
    }
  }

  // if we saw something, move servo to track it
  if (bestDist < 9999.0) {
    Serial.print("Tracking object at ");
    Serial.print(bestDist, 1);
    Serial.print(" cm, angle ");
    Serial.println(bestPos);
    servo_0.write(bestPos);
    delay(1000); // hold there 1s before next sweep
  } else {
    Serial.println("No object detected, rescanning...");
  }
}

float getDistance() {
  long duration = readUltrasonicDistance(trig_pin, echo_pin);
  if (duration > 0) {
    return duration / 58.0;
  } else {
    return 9999.0; // invalid
  }
}
