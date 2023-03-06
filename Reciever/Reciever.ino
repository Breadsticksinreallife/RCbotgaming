#include <SoftwareSerial.h>

SoftwareSerial mySerial(10, 11);
byte address;
byte readByte1;
byte readByte2;
byte readByte3;
byte readByte4;
byte readByte5;
void lookForSignal() {
  while (mySerial.available() < 5)
    ;
  address = char(mySerial.read());
  readByte1 = char(mySerial.read());
  readByte2 = char(mySerial.read());
  readByte3 = char(mySerial.read());
  readByte4 = char(mySerial.read());
}
void receiveSignal() {
  while (mySerial.available() < 1)
    ;
  address = char(mySerial.read());
  if (address != 1) {
    Serial.println("no address found");
    return;
  }
  while (mySerial.available() < 1)
    ;
  readByte1 = char(mySerial.read());
  while (mySerial.available() < 1)
    ;
  readByte2 = char(mySerial.read());
  while (mySerial.available() < 1)
    ;
  readByte3 = char(mySerial.read());
  while (mySerial.available() < 1)
    ;
  readByte4 = char(mySerial.read());
  while (mySerial.available() < 1)
    ;
  readByte5 = char(mySerial.read());
  Serial.print("b5 is ");
  Serial.println(readByte5);
}
void checksumGoofs() {
  byte checksum = ~(readByte1 + readByte2 + readByte3 + readByte4) + 1;
  if (checksum == readByte5) {
    Serial.print("cs is ");
    Serial.println(checksum);
  }
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
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  //simpleSignal();
  receiveSignal();
  checksumGoofs();
  delay(10);
}
