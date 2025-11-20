const int TOUCH_PIN = 5;     // Grove Touch Sensor signal pin
const int LED_PIN   = LED_BUILTIN;   // MCU built-in LED

void setup() {
  Serial.begin(115200);

  pinMode(TOUCH_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);

  digitalWrite(LED_PIN, LOW);  // LED OFF initially

  Serial.println("Touch Sensor Test on D6");
}

void loop() {
  int value = digitalRead(TOUCH_PIN);

  if (value == HIGH) {
    Serial.println("TOUCHED");
    digitalWrite(LED_PIN, HIGH);  // LED ON
  } else {
    Serial.println("Not touched");
    digitalWrite(LED_PIN, LOW);   // LED OFF
  }

  delay(150);
}
