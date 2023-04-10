int joystickNaught1, joystickNaught2;

void timer1Setup(){
    TCCR1A = 0; // first zero the registers
    TCCR1B = 0; // in preparation to set bits
    OCR1A = 0;
    TCNT1 = 0;

    // now set bits!
    DDRB |= (1 << DDB5); // set PB5 as an output
    TCCR1A |= (1 << COM1A0); //Timer1 in toggle mode Table 14-2
    TCCR1B |= (1 << WGM12); //Start timer 1 in CTC mode Table 14-4
    TCCR1B |= (1 << CS10); // Prescaler table 14-5
    OCR1A = 221; //CTC Compare value, 14.10.12 n = 221 = 1991/9 = (fclk / (2 ftgt) ) - 1
}

void offy() {
    TCCR1A = 0;
    TCCR1B = 0;
    OCR1A = 0;
    TCNT1 = 0;
}

void sendaByte (byte dataByte) {
    timer1Setup(); // start carrier frequency
    delayMicroseconds(100); // ~2 full cycle running start
    Serial1.write(dataByte); // byte byte, sorry, bye bye byte
    while (!(UCSR1A & (1 << TXC1))); // USART Control Status Reg
    // (p.209),
    // wait for the transmit
    // complete flag, TXC1 bit 6
    // to be set
    delayMicroseconds(100); // modulation overlap on the back end
    offy(); // turn off modulator signal to save power
}

void setup() {
    Serial1.begin(4800); // 36kHz/4800 = 7.5 carrier freq cycles
    // pinMode(A0, INPUT); //Joystick - V
    // pinMode(A1, INPUT); //Joystick - H
    Serial.begin(9600);

    digitalWrite(8, HIGH);
    delayMicroseconds(5);
    joystickNaught1 = analogRead(0);
    joystickNaught2 = analogRead(1);
    digitalWrite(8, LOW);
}

byte test = 42;

//Packet 1:
// Left Motor Power
// Packet 2:
// Right Motor Power
// Packet 3:
// 0000 + LeftMotorDir, RightMotorDir, Button1State, Button2State: 1 is CCW, 0 IS CW
// Packet 4:
// No information yet (send 4);

void loop() {
    sendaByte(1); // address byte
    delay(10);

    digitalWrite(8, HIGH);
    delayMicroseconds(5);
    unsigned long test1, test2;
    int joystickVal1 = analogRead(0);
    int joystickVal2 = analogRead(1);
    if (joystickVal1 - joystickNaught1 < 0) {
        test1 = 255 - (255.0 * joystickVal1 / joystickNaught1);
    } else {
        test1 = ( (255.0 * joystickVal1) - (255 * joystickNaught1) ) / (1023 - joystickNaught1);
    }

if (joystickVal2 - joystickNaught2 < 0) {
        test2 = 255 - ((255.0 * joystickVal2) / (joystickNaught2));
    } else {
        test2 = ( 255.0 * (joystickVal2 - joystickNaught2) ) / (1023 - joystickNaught2);
    }

    digitalWrite(8, LOW);

    Serial.print("(");
    Serial.print(test1);
    Serial.print(", ");
    Serial.print(test2);
    Serial.print(")  ");

//765, 766 @ Jz = 511
    Serial.print("[");
    Serial.print(joystickVal1);
    Serial.print(", ");
    Serial.print((joystickVal2));
    Serial.println("]");

    sendaByte(test1);
    delay(10);
    sendaByte(test2);
    delay(10);

    byte temp3 = 0b0;
    if ( (joystickVal1 - 512) > 0) {
        temp3 |= 0b1000;
    };

    if ( (joystickVal2 - 512) > 0) {
        temp3 |= 0b0100;
    };
    sendaByte(temp3);
    delay(10);

    sendaByte(8);
    delay(10);

    sendaByte(~(joystickVal1 + joystickVal2 + temp3 + 8) + 1);
}