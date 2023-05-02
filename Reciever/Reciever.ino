#include <SoftwareSerial.h>

SoftwareSerial mySerial(10, 11);
byte address, packet1, packet2, packet3, packet4, checksumPacket;
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

void receiveSignal() {
  while (mySerial.available() < 1);
  address = char(mySerial.read());
  if (address == 1) {

    while (mySerial.available() < 1);
    packet1 = char(mySerial.read());  //Left Motor Power

    while (mySerial.available() < 1);
    packet2 = char(mySerial.read());  //Right Motor Power

    while (mySerial.available() < 1);
    packet3 = char(mySerial.read());  // 0000, LMdir, RMdir, button 1, button 2 -- 0 is off or cw, 1 is on or ccw

    while (mySerial.available() < 1);
    packet4 = char(mySerial.read());

    while (mySerial.available() < 1);
    checksumPacket = char(mySerial.read());
  }
}

bool verifyChecksum() {
  byte checksumVer = packet1 + packet2 + packet3 + packet4 + checksumPacket;
  Serial.println("-");
  Serial.println(packet1);
  Serial.println(packet2);
  Serial.println(packet3);
  Serial.println(packet4);
  Serial.println(checksumVer);
  return checksumVer == 0;
}

void setup() {
  // put your setup code here, to run once:
  mySerial.begin(4800);
  Serial.begin(9600);
  pinMode(leftMotor_IN1, OUTPUT);
  pinMode(leftMotor_IN2, OUTPUT);
  pinMode(rightMotor_IN1, OUTPUT);
  pinMode(rightMotor_IN2, OUTPUT);
  packet3 = 0b00001000;
}

void loop() {
  // put your main code here, to run repeatedly:
    receiveSignal();
    if (verifyChecksum()){
      Serial.println("g");
    } else {
      Serial.println("b");
    }
}