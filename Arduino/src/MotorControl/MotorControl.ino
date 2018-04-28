//define IO pin
#define ENB 6
#define IN3 9
#define IN4 11

const int NUM_WHEELS = 4;
const int WHEEL_PINS = {};


void setup() {
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENB, OUTPUT);
  digitalWrite(ENB, HIGH);  
}

void loop() {
  //digitalWrite(IN3, LOW);
  //digitalWrite(IN4, HIGH);
  //delay(500);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  //delay(500);
  //digitalWrite(IN3, HIGH);
  //digitalWrite(IN4, LOW);
  //delay(500);
  //digitalWrite(IN3, HIGH);
  //digitalWrite(IN4, LOW);
  //delay(500);
  
}
