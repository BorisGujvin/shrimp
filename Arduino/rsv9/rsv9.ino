#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
RF24 radio(9,10);
const uint64_t pipes[2] = { 0xF0F0F0F0E1LL, 0xF0F0F0F0D2LL };
void setup() {
  // put your setup code here, to run once:
 Serial.begin(57600);
 Serial.println("start...");
   radio.begin();
   radio.openReadingPipe(1,pipes[1]);
   radio.setRetries(15,15);
   radio.startListening();

}
int i;
void loop() {


unsigned long started_waiting_at = millis();
    bool timeout = false;
    while ( ! radio.available() && ! timeout )
      if (millis() - started_waiting_at > 2000 )
        timeout = true;

    // Describe the results
    if ( timeout )
    {
      Serial.print("Failed, response timed out.\n\r");
    }
    else
    {
      // Grab the response, compare, and send to debugging spew
      unsigned long got_time;
      radio.read( &i, sizeof(int) );

      // Spew it

    }


  
radio.read( &i, 2 );
Serial.println(i);
  // put your main code here, to run repeatedly:

}
