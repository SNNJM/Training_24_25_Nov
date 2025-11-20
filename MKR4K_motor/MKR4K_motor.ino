const int IN1 = 2;
const int IN2 = 3;
const int IN3 = 4;
const int IN4 = 5;

int seq8[8][4] = {
  {1,0,0,0},
  {1,1,0,0},
  {0,1,0,0},
  {0,1,1,0},
  {0,0,1,0},
  {0,0,1,1},
  {0,0,0,1},
  {1,0,0,1}
};

void setup() {
  Serial.begin(115200);
  Serial.println("Stepper Motor Test");

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
}

void stepForward() {
  for (int i = 0; i < 512; i++) {
    int s = i % 8;
    digitalWrite(IN1, seq8[s][0]);
    digitalWrite(IN2, seq8[s][1]);
    digitalWrite(IN3, seq8[s][2]);
    digitalWrite(IN4, seq8[s][3]);
    delay(3);
  }
}

void stepBackward() {
  for (int i = 0; i < 512; i++) {
    int s = 7 - (i % 8);
    digitalWrite(IN1, seq8[s][0]);
    digitalWrite(IN2, seq8[s][1]);
    digitalWrite(IN3, seq8[s][2]);
    digitalWrite(IN4, seq8[s][3]);
    delay(3);
  }
}

void loop() {
  Serial.println("Forward...");
  stepForward();
  delay(500);

  Serial.println("Backward...");
  stepBackward();
  delay(500);
}
