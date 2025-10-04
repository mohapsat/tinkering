// Sentry

#include <Servo.h>
#include "Arduino_LED_Matrix.h"

ArduinoLEDMatrix matrix;
Servo servo_0;
Servo servo_1;

int pos = 0;    // servo position
int servo_0_pin = 9;   // use pin 9 for servo
int servo_1_pin = 10;    // hand wave servo
int buzz_pin = 2;

int detection_distance = 50; // in cm
int start_angle = 30;
int end_angle = 140;

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
  servo_1.attach(servo_1_pin); 
  servo_1.write(0);
  pinMode(buzz_pin,OUTPUT);
  Serial.begin(br);
  matrix.begin();

}

void loop() {

  detect_object();

  // sweep from 0 to 90
  for (pos =start_angle; pos <= end_angle; pos += 1) {
    servo_0.write(pos);
    detect_object();
    delay(10);
  }

  delay(1000);

  // sweep from 90 back to 0
  for (pos = end_angle; pos >= start_angle; pos -= 1) {
    servo_0.write(pos);
    detect_object();
    delay(10);
  }
  delay(1000);
}

void detect_object() {
  long duration = readUltrasonicDistance(trig_pin, echo_pin);

  if (duration > 0) {
    cm = duration / 58.0;  // µs to cm
    if (cm >= 2 && cm <= detection_distance) {
      detected = true;
      Serial.print("Detected at ");
      Serial.print(cm, 1);
      Serial.println(" cm");
      flashMatrix();
      buzz();
      wave_hand();
    } else {
      detected = false;
    }
  } else {
    detected = false;
  }
}


void wave_hand() {

    servo_1.write(0);
    delay(100);
    servo_1.write(60);
    delay(100);
    servo_1.write(0);
    delay(100);
    servo_1.write(60);
    delay(100);
    servo_1.write(0);
    delay(100);
    servo_1.write(60);
    delay(100);
    servo_1.write(0);
  }
  


void buzz() {
    tone(buzz_pin, 1000, 150);
    delay(200); 
    tone(buzz_pin, 1500, 150);
    delay(200);
    tone(buzz_pin, 500, 150);
   
}

void flashMatrix() {
  // Simple pattern (full grid ON)
  uint8_t smiley[8][12] = { 
    {1,1,1,1,1,1,1,1,1,1,1,1},
    {1,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,1,1,0,0,0,1,1,0,0,1},
    {1,0,1,1,0,0,0,1,1,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,1,1,1,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,1},
    {1,1,1,1,1,1,1,1,1,1,1,1}
  };
  
  matrix.renderBitmap(smiley, 8, 12); 
  delay(500);          
  matrix.clear();       // turn off
}
