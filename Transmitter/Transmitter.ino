

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

}

byte test = 42;

void loop() {
    sendaByte(1); // address byte

    sendaByte(2);
    sendaByte(4);
    sendaByte(6);
    sendaByte(8);

    sendaByte(~(2 + 4 + 6 + 8) + 1);
}