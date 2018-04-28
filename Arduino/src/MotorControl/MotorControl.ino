#include <Wire.h>

const int addr = 0x43;

enum RECEIVE_ACTIONS {BALL_DATA};
enum WHEEL_SIDE {LEFT, RIGHT};  //controls both right motors and both left motors


int ENA=5; 
int IN1=6;
int IN2=7;
int ENB=11; 
int IN3=8;
int IN4=9;

void setup() {
    //Wire.begin(addr);
    //Wire.onReceive(receiveData);
    //Wire.onRequest(nothing);
//=======

volatile int ball_size;
volatile int ball_offset;

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
    setMotor(LEFT, -100);
    
    setMotor(RIGHT, -100);  
}

/*
void receiveData(int num_bytes) {

    int cmd = Wire.read();
    int bytes = Wire.read();
    
    if (num_bytes == bytes + 2) {
        if (cmd == BALL_DATA) {
          ball_size = i2cGetInt();
          ball_offset = i2cGetInt();
          //TODO: change above to volatile int objects for feedback loop
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
*/

void setMotor(int WHEEL_SIDE, int speed){
    switch (WHEEL_SIDE) {
        case LEFT:
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
        case RIGHT:
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



