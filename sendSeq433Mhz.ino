// Ce programme s'inspire de https://ignorantofthings.com/reverse-engineering-a-remote-control-power-socket-for-arduino-integration/
// Le but est d'avoir un programme minimaliste qui n'utilise pas de bibliothèque 433 Mhz.

#include <Arduino.h>

#define TX_PIN 13                   // physical pin to which 433MHz module is connected

// Frequency 433,92 Mhz
// Trames obtenues à l'aide de Universal Radio hacker (https://github.com/jopohl/urh)
#define BIT_LEN 510                 // length of individual bit pulses (in microseconds)
#define SEQ_RPT 6                   // number of times to repeat full sequence
#define SEQ_PAUSE 9708              // time in microseconds to pause between repeated sequences
#define SEQ_LEN 17                  // 17 bytes
byte SEQ1_ON[] =  {0x83,0x26,0x67,0x32,0x66,0x73,0x27,0x27,0x32,0x73,0x26,0x67,0x26,0x67,0x27,0x33,0x32};  // 17 bytes
byte SEQ1_OFF[] = {0x83,0x26,0x67,0x32,0x66,0x73,0x27,0x27,0x32,0x73,0x26,0x67,0x26,0x67,0x33,0x33,0x32};  // 17 bytes
byte SEQ2_ON[] =  {0x83,0x26,0x67,0x32,0x66,0x73,0x27,0x27,0x32,0x73,0x26,0x67,0x26,0x67,0x27,0x33,0x26};  // 17 bytes
byte SEQ2_OFF[] = {0x83,0x26,0x67,0x32,0x66,0x73,0x27,0x27,0x32,0x73,0x26,0x67,0x26,0x67,0x33,0x33,0x26};  // 17 bytes
byte SEQ3_ON[] =  {0x83,0x26,0x67,0x32,0x66,0x73,0x27,0x27,0x32,0x73,0x26,0x67,0x26,0x67,0x27,0x32,0x72};  // 17 bytes
byte SEQ3_OFF[] = {0x83,0x26,0x67,0x32,0x66,0x73,0x27,0x27,0x32,0x73,0x26,0x67,0x26,0x67,0x33,0x32,0x72};  // 17 bytes
byte SEQG_ON[] =  {0x83,0x26,0x67,0x32,0x66,0x73,0x27,0x27,0x32,0x73,0x26,0x67,0x26,0x66,0x67,0x33,0x32};  // 17 bytes
byte SEQG_OFF[] = {0x83,0x26,0x67,0x32,0x66,0x73,0x27,0x27,0x32,0x73,0x26,0x67,0x26,0x66,0x73,0x33,0x32};  // 17 bytes

/*--------------------------------------------------------------------*/

void sendPulse(bool bit) {          // 1 bit
  digitalWrite(TX_PIN, bit);        // set pin state based on pulse bit
  delayMicroseconds(BIT_LEN);       // hold pin state for the bit length time
  digitalWrite(TX_PIN, LOW);        // pull pin low once bit sent
}

/*--------------------------------------------------------------------*/

void sendByte(byte b) {             // 8 bits
  // 0b00000000 à 0b11111111
  // 0x00       à 0xFF
  for (byte i = 8; i > 0; i--) {
    bool bit = b >> (i - 1) & 1;    // extract individual bits from byte
    sendPulse(bit);                 // send pulse sequence of extracted bit
  }
}

/*--------------------------------------------------------------------*/

void sendSequence(byte seq[]) {
  for (int i = 0; i < SEQ_RPT; i++) {    // repeat command to ensure it's registered by receiver
    for (int j = 0; j < SEQ_LEN; j++) {
      sendByte(seq[j]);
    }
    delayMicroseconds(SEQ_PAUSE);        // delay between repeated sequences
  }
}

/*--------------------------------------------------------------------*/

void setup() {
  pinMode(TX_PIN, OUTPUT);
}

/*--------------------------------------------------------------------*/

void loop() {
  sendSequence(SEQ1_ON);
  delay(1000);                      // wait one second
  sendSequence(SEQ1_OFF);
  delay(1000);                      // wait one second
}

/*--------------------------------------------------------------------*/
