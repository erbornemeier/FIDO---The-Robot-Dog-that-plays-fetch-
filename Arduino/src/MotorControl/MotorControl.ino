#include <Wire.h>

const int addr = 0x43;

enum RECEIVE_ACTIONS {BALL_DATA, PICKUP};
enum WHEEL_SIDE {LEFT, RIGHT};  //controls both right motors and both left motors

const unsigned int DEFAULT_SPEED = 100;
const unsigned int ERROR_THRESHOLD = 2;
const unsigned long PICKUP_TIME = 3000;
const float K_P = 0.35;
//const float K_P = 5.0; //debug

int ENA=5; 
int IN1=6;
int IN2=7;
int ENB=11; 
int IN3=8;
int IN4=9;

volatile int ball_size = 0;
volatile int ball_offset = 0;

volatile bool needs_pickup = false;

void setup() {
    Serial.begin(9600);
  
    Wire.begin(addr);
    Wire.onReceive(receiveData);
    Wire.onRequest(nothing);

    pinMode(IN1,OUTPUT);
    pinMode(IN2,OUTPUT);
    pinMode(IN3,OUTPUT);
    pinMode(IN4,OUTPUT);
    pinMode(ENA,OUTPUT);
    pinMode(ENB,OUTPUT);
    digitalWrite(ENA,HIGH);  
    digitalWrite(ENB,HIGH);      

}
void loop() {
    /*setMotor(LEFT, -100);
    setMotor(RIGHT, -100);
    */

    if (Serial.available() > 0){
        int goTo = Serial.read();
        Serial.println(goTo);
        ball_offset = goTo;
    }
    goToBall();

    if (needs_pickup){
        pickup_ball();
        needs_pickup = false;
        while(1){} //debug
    }
    
}

void pickup_ball(){
      unsigned long start_time = millis();

      while (millis() - start_time < PICKUP_TIME){
         setMotor(LEFT, DEFAULT_SPEED);
         setMotor(RIGHT, DEFAULT_SPEED); 
         delay(50);
      }
      setMotor(LEFT, 0);
      setMotor(RIGHT, 0);
  
}

void goToBall(){
    int error = ball_offset;

    while(abs(error) > ERROR_THRESHOLD && !needs_pickup){
        int leftMotorVal  = errorToMotorOut(LEFT,  error);
        int rightMotorVal = errorToMotorOut(RIGHT, error);
        setMotor(LEFT,  leftMotorVal );
        setMotor(RIGHT, rightMotorVal);
        Serial.print("LEFT: ");
        Serial.print(leftMotorVal);
        
        Serial.print(" RIGHT: ");
        Serial.println(rightMotorVal);
        delay(50);
        error = ball_offset;  
        
    }

    setMotor(LEFT, DEFAULT_SPEED);
    setMotor(RIGHT, DEFAULT_SPEED);
  
}

int errorToMotorOut(int WHEEL_SIDE, int error){
    int val = ((K_P * error)*(WHEEL_SIDE==LEFT?1:-1)) + DEFAULT_SPEED;
    if (abs(val) > 255) val = val>0?255:-255;
    return val;
}

void setMotor(int WHEEL_SIDE, int speed){
    switch (WHEEL_SIDE) {
        case RIGHT:
            if (speed < 0) {
                analogWrite(ENA, -speed);
                digitalWrite(IN1,LOW);    //left wheels backup  
                digitalWrite(IN2,HIGH);  
            }
            else {
                analogWrite(ENA, speed);
                digitalWrite(IN1,HIGH);      
                digitalWrite(IN2,LOW);
            }
            break;
        case LEFT:
            if (speed < 0) {
                analogWrite(ENB, -speed);
                digitalWrite(IN3,HIGH);    //right wheels backup  
                digitalWrite(IN4,LOW);    
            }
            else {
                analogWrite(ENB, speed);
                digitalWrite(IN3,LOW);      
                digitalWrite(IN4,HIGH);
            }
            break;    
    }
   
}

void receiveData(int num_bytes) {

    int cmd = Wire.read();
    int bytes = Wire.read();
    
    if (num_bytes == bytes + 2) {
        if (cmd == BALL_DATA) {
          ball_size = i2cGetInt();
          ball_offset = i2cGetInt();
        }
        else if (cmd == PICKUP){
            dumpData();
            needs_pickup = true;  
        }
        else {
            dumpData();
        }
    }
    else {
        // We have an unexpected message, throw it out.
        dumpData();   
    }
    
}


void nothing(){}

// Reads the next 2 bytes from the i2c bus and splices them together to make a signed 16-bit integer.
int i2cGetInt() {
    return ((Wire.read() << 8) | Wire.read());
}

void dumpData() {
    while (Wire.available()) Wire.read();
}


