int redPin = 25;
int greenPin = 26;
int bluePin = 27;
int buttonPin = 14;
int buttonState = 0;
int lastState = 0;
int colorState = 0;

void setup() {
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);
  Serial.begin(115200);
  Serial.println("RGB LED Control Started");
}

void loop() {
  buttonState = digitalRead(buttonPin);

  if (buttonState == LOW && lastState == HIGH) {
    colorState = (colorState + 1) % 4;
    delay(200); // debounce delay
  }
  lastState = buttonState;

  switch (colorState) {
    case 0: digitalWrite(redPin, HIGH);
            digitalWrite(greenPin, LOW);
            digitalWrite(bluePin, LOW);
            break;
    case 1: digitalWrite(redPin, LOW);
            digitalWrite(greenPin, HIGH);
            digitalWrite(bluePin, LOW);
            break;
    case 2: digitalWrite(redPin, LOW);
            digitalWrite(greenPin, LOW);
            digitalWrite(bluePin, HIGH);
            break;
    default:digitalWrite(redPin, LOW);
            digitalWrite(greenPin, LOW);
            digitalWrite(bluePin, LOW);
  }
}