// ----------------------
// Pin Assignments
// ----------------------
const int TOUCH_PIN = 5;     // Touch Sensor
const int TEMP_PIN  = A0;    // Temperature Sensor

// Stepper Motor Pins
const int IN1 = 2;
const int IN2 = 3;
const int IN3 = 4;
const int IN4 = 5;

// ----------------------
// Motor & Safety Flags
// ----------------------
bool motorEnabled = false;      // User toggle
bool safetyLocked = false;      // Temp override
unsigned long lastTouch = 0;

// ----------------------
// Temperature Threshold
// ----------------------
const float TEMP_HIGH = 37.0;   // °C: lock motor
const float TEMP_LOW  = 36.0;   // °C: unlock motor

// ----------------------
// Stepper Motor Functions
// ----------------------
void stepMotor(int step) {
  switch (step) {
    case 0: digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);  digitalWrite(IN3, LOW);  digitalWrite(IN4, LOW); break;
    case 1: digitalWrite(IN1, LOW);  digitalWrite(IN2, HIGH); digitalWrite(IN3, LOW);  digitalWrite(IN4, LOW); break;
    case 2: digitalWrite(IN1, LOW);  digitalWrite(IN2, LOW);  digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW); break;
    case 3: digitalWrite(IN1, LOW);  digitalWrite(IN2, LOW);  digitalWrite(IN3, LOW);  digitalWrite(IN4, HIGH); break;
  }
}

void stepForward() {
  for (int i = 0; i < 4; i++) {
    stepMotor(i);
    delay(3);
  }
}

// ----------------------
// Temperature Reading
// ----------------------
float readTemperature() {
  int raw = analogRead(TEMP_PIN);

  float voltage = raw * (3.3 / 1023.0);
  float resistance = (3.3 - voltage) * 10000 / voltage;
  float temperature = 1 / (log(resistance / 10000) / 3975 + 1/298.15) - 273.15;

  return temperature;
}

// ----------------------
// Setup
// ----------------------
void setup() {
  Serial.begin(115200);

  pinMode(TOUCH_PIN, INPUT);

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  // Force everything OFF at start
  motorEnabled = false;
  safetyLocked = false;

  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);

  Serial.println("Automation System Ready");
}

// ----------------------
// Main Loop
// ----------------------
void loop() {

  // ---------------------------------
  // 1. TOUCH SENSOR — Toggle Motor
  // ---------------------------------
  int touch = digitalRead(TOUCH_PIN);

  if (!safetyLocked && touch == HIGH && (millis() - lastTouch) > 400) {
    motorEnabled = !motorEnabled;
    lastTouch = millis();

    Serial.print("Motor: ");
    Serial.println(motorEnabled ? "ON" : "OFF");
  }

  // ---------------------------------
  // 2. Read Temperature
  // ---------------------------------
  float tempC = readTemperature();

  Serial.print("TEMP: ");
  Serial.println(tempC);

  // ---------------------------------
  // 3. SAFETY LOGIC
  // ---------------------------------

  // High temperature → force off
  if (tempC >= TEMP_HIGH) {
    safetyLocked = true;
    motorEnabled = false;
    Serial.println(">> TEMP HIGH — MOTOR LOCKED <<");
  }

  // Cool enough → unlock
  if (tempC <= TEMP_LOW && safetyLocked) {
    safetyLocked = false;
    Serial.println(">> TEMP NORMAL — USER CAN TURN MOTOR ON <<");
  }

  // ---------------------------------
  // 4. MOTOR OPERATION
  // ---------------------------------
  if (motorEnabled && !safetyLocked) {
    stepForward();
  } else {
    // FULL motor OFF
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, LOW);
  }

  delay(50);
}
