#include <SoftwareSerial.h>

SoftwareSerial mySerial(10, 11);
byte address;
byte readByte1;
byte readByte2;
byte readByte3;
byte readByte4;
byte readByte5;
int LMDir;
int RMDir;
int LMPower;
int RMPower;

//Motor definition stuff
#define leftMotor_IN1 2
#define leftMotor_IN2 4
#define leftMotor_PWM_pin 5  //capped at <0, 255>

#define rightMotor_IN1 7
#define rightMotor_IN2 8
#define rightMotor_PWM_pin 6
void LMcw() {
  digitalWrite(leftMotor_IN1, HIGH);
  digitalWrite(leftMotor_IN2, LOW);
}
void LMccw() {
  digitalWrite(leftMotor_IN1, LOW);
  digitalWrite(leftMotor_IN2, HIGH);
}
void RMcw() {
  digitalWrite(rightMotor_IN1, HIGH);
  digitalWrite(rightMotor_IN2, LOW);
}
void RMccw() {
  digitalWrite(rightMotor_IN1, LOW);
  digitalWrite(rightMotor_IN2, HIGH);
}
//Reading wireless transmissions
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
  /*if (address != 1) {
    Serial.println("no address found");
    return;
  }*/
  while (mySerial.available() < 1)
    ;
  readByte1 = char(mySerial.read());  //Left Motor Power
  while (mySerial.available() < 1)
    ;
  readByte2 = char(mySerial.read());  //Right Motor Power
  while (mySerial.available() < 1)
    ;
  readByte3 = char(mySerial.read());  // 0000, LMdir, RMdir, B1S, B2S -- 0 is off / cw, 1 is on / ccw
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
    /*Serial.print("cs is ");
    Serial.println(checksum);*/
  }
}
void simpleSignal() {
  while (mySerial.available() < 1)
    ;
  address = char(mySerial.read());
  Serial.println(address);
}
//Translating transmission into outputs
void motorMove() {
  LMPower = readByte1;
  RMPower = readByte2;
  Serial.print("LM speed is ");
  Serial.println(LMPower);
  Serial.println(" at direction ");
  if ((readByte3 >> 3) & 1) {
    Serial.println("lmccw");
  } else {
    Serial.println("lmcw");
  }
    Serial.print("RM speed is ");
  Serial.println(LMPower);
  Serial.println(" at direction");
  if ((readByte3 >> 2) & 1) {
    Serial.println("rmccw");
  } else {
    Serial.println("rmcw");
  }
}


void setup() {
  // put your setup code here, to run once:
  mySerial.begin(4800);
  Serial.begin(9600);
  pinMode(leftMotor_IN1, OUTPUT);
  pinMode(leftMotor_IN2, OUTPUT);
  pinMode(rightMotor_IN1, OUTPUT);
  pinMode(rightMotor_IN2, OUTPUT);
  readByte3 = 0b00001000;
}

void loop() {
  // put your main code here, to run repeatedly:
  receiveSignal();
  checksumGoofs();
  delay(10);
//  analogWrite(leftMotor_PWM_pin, (readByte1 * 4) - 1);
//  analogWrite(rightMotor_PWM_pin, (readByte2 * 4) - 1);
//  motorMove();
}
