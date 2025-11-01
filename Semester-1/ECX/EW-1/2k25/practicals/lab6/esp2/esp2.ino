#include <ESP32Servo.h>
Servo gateServo;

int irPin = 15;
int servoPin = 18;
int irState = 0;

void setup() {
  Serial.begin(115200);
  gateServo.attach(servoPin);
  pinMode(irPin, INPUT);
  gateServo.write(0);
  Serial.println("IR Sensor Servo Control Started");
}

void loop() {
  irState = digitalRead(irPin);
  
  if (irState == LOW) {   // Object detected
    gateServo.write(90);
    Serial.println("Object detected: Servo rotated to 90 degrees");
    delay(1000);
  } else {
    gateServo.write(0);
    Serial.println("No object: Servo at 0 degrees");
  }
  delay(200);
}