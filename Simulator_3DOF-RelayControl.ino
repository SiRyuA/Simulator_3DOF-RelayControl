/*
 * Arduino Mega
 * Motor Relay Control
 * 3 DOF Motion Simulator
 * 
 * Date : 20150903
 * Artist by SiRyuA.
 */

#define UP 1
#define DOWN 0
#define STOP 2
#define timedelay 5000

// Relay Port
int Motor_pinRelay[6]={22, 28, 36, 27, 12, 37};

// Axis Data
char AxisData;
int Degree,
    GPos = 0,
    GPos1 = 0,
    GPos10 = 0,
    GPos100 = 0;

// Start
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Motor_pinSetup();
}

void loop() {
  Motor_Move();
}

// Pin Include
void Motor_pinSetup(void) {
  for(char i=0; i<=8; i++) {
    pinMode(Motor_pinRelay[i], OUTPUT);
  }
}

// Motor Control
void Motor_Control(int motor, char mode) {
  if(mode == UP) {
    digitalWrite(Motor_pinRelay[motor], HIGH);
    digitalWrite(Motor_pinRelay[(motor+3)], LOW);
  }
  else if(mode == DOWN) {
    digitalWrite(Motor_pinRelay[motor], LOW);
    digitalWrite(Motor_pinRelay[(motor+3)], HIGH);
  }
  else {
    digitalWrite(Motor_pinRelay[motor], LOW);
    digitalWrite(Motor_pinRelay[(motor+3)], LOW);
  }
}

void Motor_Move(void) {
  while(Serial.available() > 0) {
    AxisData = Serial.read();
    
    if(AxisData == 'L') Motor_Pos(AxisData);
    if(AxisData == 'R') Motor_Pos(AxisData);
    if(AxisData == 'C') Motor_Pos(AxisData);
    if(AxisData == 'S') {
      Serial.println("S");
      Motor_Control(0, STOP);
      Motor_Control(1, STOP);
      Motor_Control(2, STOP);
    }
  }
}

void Motor_Pos(char which_motor) {
  delay(2);
  int GPos100 = Serial.read()- '0';
  delay(2);
  int GPos10  = Serial.read()- '0';
  delay(2);
  int GPos1   = Serial.read()- '0';
  
  if(GPos1 < 0) GPos = GPos10 + 10*GPos100;
  if(GPos1 < 0 && GPos10 < 0) GPos = GPos100;
  if(GPos100 < 0) GPos = 127;
  if(GPos1 >= 0 && GPos10 >= 0 && GPos100 >= 0) GPos = 100 * GPos100 + 10 * GPos10 + GPos1;
  
  Degree = map(GPos, 0, 255, 0, 180);
  
  if(which_motor == 'L'){
    Serial.print("L = ");
    Serial.println(Degree);

    if(Degree >= 90) Motor_Control(0, UP);
    else Motor_Control(0, DOWN);
  }
  
  if(which_motor == 'R'){
    Serial.print("R = ");
    Serial.println(Degree);

    if(Degree >= 90) Motor_Control(2, UP);
    else Motor_Control(2, DOWN);
  }
  
  if(which_motor == 'C'){
    Serial.print("C = ");
    Serial.println(Degree);

    if(Degree >= 90) Motor_Control(1, UP);
    else Motor_Control(1, DOWN);
  }
}


