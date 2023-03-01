#include <SoftwareSerial.h>

SoftwareSerial mySerial(10, 11);
byte address;
byte readByte1;
byte readByte2;
byte readByte3;
byte readByte4;
void lookForSignal() {
  while (mySerial.available() < 5)
    ;
  address = char(mySerial.read());
  readByte1 = char(mySerial.read());
  readByte2 = char(mySerial.read());
  readByte3 = char(mySerial.read());
  readByte4 = char(mySerial.read());
}
void recieveSignal() {
  while (mySerial.available() < 1)
    ;
  address = char(mySerial.read());
  while (mySerial.available() < 1)
    ;
  readByte1 = char(mySerial.read());
  while (mySerial.available() < 1)
    ;
  readByte2 = char(mySerial.read());
  while (mySerial.available() < 1)
    ;
  readByte3 = char (mySerial.read());
    while (mySerial.available() < 1);
  readByte4 = char (mySerial.read());
}
void simpleSignal() {
  while (mySerial.available() < 1)
    ;
  address = char(mySerial.read());
  Serial.println(address);
}


void setup() {
  // put your setup code here, to run once:
  mySerial.begin(4800);
  Serial.begin(4800);
  pinMode(A3, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  void simpleSignal();
}
