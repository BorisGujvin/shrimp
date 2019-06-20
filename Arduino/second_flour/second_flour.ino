
#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
#include "printf.h"

RF24 radio(9,10);
const uint64_t pipes[2] = { 0xF0F0F0F0E1LL, 0xF0F0F0F0D2LL };
float T2_cur;

void begin_radio(){
  radio.begin();
  radio.setRetries(15,15);
  radio.setChannel  ( 100 ) ;
  radio.openWritingPipe(pipes[0]);
  radio.openReadingPipe(1,pipes[1]);
  radio.startListening();

}
void setup(void)
{
  Serial.begin(57600);
  begin_radio();
  printf_begin();
  printf("\n\rпрограмма для второго этажа\n\r");
}
void get_data(void){
 char tmp[10];
    radio.stopListening();
    radio.openWritingPipe(pipes[0]);
    radio.openReadingPipe(1,pipes[1]);

    printf("Now sending request ");
    unsigned long time = 5555;
    delay(20);
    bool ok = radio.write( &time, sizeof(unsigned long) );
    
    if (ok)
      printf("ok...");
    else
      printf("failed.\n\r");
       
    radio.startListening();
    unsigned long started_waiting_at = millis();
    bool timeout = false;
    while ( ! radio.available() && ! timeout )
      if (millis() - started_waiting_at > 3000 )
        timeout = true;

    // Describe the results
    if ( timeout )
    {
      printf("Failed, response timed out.\n\r");
    }
    else
    {
      float  got;
      radio.read( &got, sizeof(float) );
      T2_cur= got;
      dtostrf(got, 5, 3, tmp);
     printf("Ответ  %s ",tmp);
    }
  
}
void loop(void)
{
get_data();
    delay(1000);
  }

