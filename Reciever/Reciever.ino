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

//Reading wireless transmissions
void lookForSignal() {
  while (mySerial.available() < 5)
    ;
  address = char(mySerial.read());
  packet1 = char(mySerial.read());
  packet2 = char(mySerial.read());
  packet3 = char(mySerial.read());
  packet4 = char(mySerial.read());
}

void receiveSignal() {
  while (mySerial.available() < 1);
  address = char(mySerial.read());
  if (address == 1) {
    Serial.print("Address is correct: ");
    Serial.println(address);

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
  byte checksum = ~(packet1 + packet2 + packet3 + packet4) + 1;
  if (checksum != checksumPacket) {
    Serial.print(checksum);
    Serial.print(" | ");
    Serial.print(checksumPacket);
    Serial.print("  ");
    Serial.println("!# !# !# Checksum did not verify. #! #! #!");
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
  LMPower = packet1;
  RMPower = packet2;
  Serial.print("LM speed is ");
  Serial.println(LMPower);
  Serial.println(" at direction ");
  if ((packet3 >> 3) & 1) {
    Serial.println("lmccw");
  } else {
    Serial.println("lmcw");
  }
    Serial.print("RM speed is ");
  Serial.println(LMPower);
  Serial.println(" at direction");
  if ((packet3 >> 2) & 1) {
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
  packet3 = 0b00001000;
}

void loop() {
  // put your main code here, to run repeatedly:
    receiveSignal();
    if (verifyChecksum()){
      Serial.print("Left Motor Power: ");
      Serial.print(packet1);
      Serial.print("| Right Motor Power: ");
      Serial.print(packet2);
      Serial.print("| Left Motor Direction: ");
      Serial.print(packet3 >> 3);
      Serial.print("| Right Motor Direction: ");
      Serial.print(packet3 >> 2);
      Serial.print("| ");
      Serial.println(packet4);
    }
  delay(1000);
}
