#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
RF24 radio(9,10);
const uint64_t pipes[2] = { 0xF0F0F0F0E1LL, 0xF0F0F0F0D2LL };
void setup() {
  // put your setup code here, to run once:
 Serial.begin(57600);
   radio.begin();
   radio.openWritingPipe(pipes[0]);
   radio.setRetries(15,15);
   radio.stopListening();
}
int i;
void loop() {
  radio.write( i++, 2 );
  delay(1000);
  // put your main code here, to run repeatedly:

}
