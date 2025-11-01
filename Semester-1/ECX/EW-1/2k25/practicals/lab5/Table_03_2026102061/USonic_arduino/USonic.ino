int trigPin = 13;
int echoPin = 12;
long duration;
float distance;

void clearSerialMonitor() {
  for (int i = 0; i < 50; i++) {
    Serial.println();
  }
}

void setup() {
  pinMode(trigPin, OUTPUT); 
  pinMode(echoPin, INPUT);
  Serial.begin(9600);
  clearSerialMonitor();
  Serial.println("Ultrasonic Distance Measurement");
}
int cnt=1;
void loop() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);

  distance = duration * 0.0343 / 2;
  Serial.print(cnt);
  Serial.print(". Distance: ");
  cnt++;
  Serial.print(distance);
  Serial.println(" cm");

  delay(500); 
  //if(cnt==21) {Serial.end();}
}