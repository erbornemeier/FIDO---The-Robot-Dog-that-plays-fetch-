#include <Wire.h>

const int addr = 0x43;

enum RECEIVE_ACTIONS {BALL_DATA};

void setup() {
    Wire.begin(addr);
    Wire.onReceive(receiveData);
    Wire.onRequest(nothing);
}

void loop() {
  // put your main code here, to run repeatedly:

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
