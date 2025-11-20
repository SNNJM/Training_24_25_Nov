#include <Arduino.h>
#include <math.h>

const int TEMP_PIN = A0;

const float B = 4275;
const float R0 = 100000.0;

float readTemp() {
  int adc = analogRead(TEMP_PIN);
  float R = (1023.0 / adc - 1.0) * R0;
  float tempK = 1.0 / ((log(R / R0) / B) + (1.0 / 298.15));
  return tempK - 273.15;
}

void setup() {
  Serial.begin(115200);
  Serial.println("Temperature Sensor Test (A0)");
}

void loop() {
  float t = readTemp();
  Serial.print("Temp: ");
  Serial.print(t);
  Serial.println(" °C");
  delay(500);
}
