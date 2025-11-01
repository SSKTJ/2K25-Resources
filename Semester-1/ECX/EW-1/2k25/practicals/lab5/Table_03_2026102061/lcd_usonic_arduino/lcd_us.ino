
#include <LiquidCrystal.h>
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

const int trigPin = 13;
const int echoPin = 10; 

long duration;
float distance;

void clearS() {
  for (int i = 0; i < 50; i++) {
    Serial.println();
  }
}

void setup() {
  lcd.begin(16, 2);
  lcd.print(" Distance Meter");
  delay(1000);
  lcd.clear();

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  Serial.begin(9600);
  clearS();
  Serial.println("Distance: (in cm)");
}

void loop() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);

  distance = duration * 0.0343 / 2;

  lcd.setCursor(0, 0);
  lcd.print(" Distance:");
  lcd.setCursor(0, 1);
  lcd.print(" ");
  lcd.print(distance);
  lcd.print(" cm   "); 

  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  delay(50);
}
