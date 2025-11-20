const int TOUCH_PIN = 6;
const int TEMP_PIN = A0;

const int IN1=2, IN2=3, IN3=4, IN4=5;

bool motorOn=false;
unsigned long startTime=0;
unsigned long lastTouch=0;

float prevTemp=0;

// ----- ML Decision Tree -----
int rf_predict(float temp_now, float temp_rise, float motor_time){
  if (temp_now > 36.9) return 1;
  if (motor_time > 120) return 1;
  if (temp_rise > 0.6) return 1;
  return 0;
}

float readTemp(){
  int raw=analogRead(TEMP_PIN);
  float voltage = raw*(3.3/1023.0);
  float resistance=(3.3-voltage)*10000/voltage;
  return 1/(log(resistance/10000)/3975 + 1/298.15) - 273.15;
}

void stepForward(){
  for(int i=0;i<4;i++){
    switch(i){
      case 0: digitalWrite(IN1,HIGH); digitalWrite(IN2,LOW); digitalWrite(IN3,LOW); digitalWrite(IN4,LOW); break;
      case 1: digitalWrite(IN1,LOW); digitalWrite(IN2,HIGH); digitalWrite(IN3,LOW); digitalWrite(IN4,LOW); break;
      case 2: digitalWrite(IN1,LOW); digitalWrite(IN2,LOW); digitalWrite(IN3,HIGH); digitalWrite(IN4,LOW); break;
      case 3: digitalWrite(IN1,LOW); digitalWrite(IN2,LOW); digitalWrite(IN3,LOW); digitalWrite(IN4,HIGH); break;
    }
    delay(3);
  }
}

void setup(){
  Serial.begin(115200);
  pinMode(TOUCH_PIN,INPUT);
  pinMode(IN1,OUTPUT); pinMode(IN2,OUTPUT); pinMode(IN3,OUTPUT); pinMode(IN4,OUTPUT);
}

void loop(){
  float temp_now = readTemp();
  float temp_rise = temp_now - prevTemp;
  prevTemp = temp_now;

  float motor_time = motorOn ? (millis()-startTime)/1000.0 : 0;

  int pred = rf_predict(temp_now, temp_rise, motor_time);

  if(digitalRead(TOUCH_PIN)==HIGH && millis()-lastTouch>300){
    motorOn = !motorOn;
    startTime = millis();
    lastTouch = millis();
  }

  if(pred == 1){
    motorOn = false;
  }

  if(motorOn){
    stepForward();
  } else {
    digitalWrite(IN1,LOW); digitalWrite(IN2,LOW);
    digitalWrite(IN3,LOW); digitalWrite(IN4,LOW);
  }

  Serial.print("T="); Serial.print(temp_now);
  Serial.print(" Rise="); Serial.print(temp_rise);
  Serial.print(" Time="); Serial.print(motor_time);
  Serial.print(" | ML=");
  Serial.println(pred==0?"SAFE":"UNSAFE");
}
