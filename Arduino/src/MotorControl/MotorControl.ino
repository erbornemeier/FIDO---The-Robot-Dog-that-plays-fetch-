#include <Wire.h>

const int addr = 0x43;

enum RECEIVE_ACTIONS {BALL_DATA};

void setup() {
    Wire.begin(addr);
    Wire.onReceive(receiveData);
    Wire.onRequest(nothing);
=======
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

void receiveData(int num_bytes) {

    int cmd = Wire.read();
    int bytes = Wire.read();
    
    if (num_bytes == bytes + 2) {
        if (cmd == BALL_DATA) {
          int size = i2cGetInt();
          int offset = i2cGetInt();
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
