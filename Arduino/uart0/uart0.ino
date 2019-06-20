#include <SoftwareSerial.h>
#include<EEPROM.h>
#include <LiquidCrystal.h>
#include <OneWire.h>
#include <DS1302.h>
#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
#include "printf.h"
const int rs = A0, en = A1, d4 = A2, d5 = A3, d6 = A4, d7 = A5;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
byte addr[8];

#define period 300
#define mtrace TRUE
float T1_;
#define D1_ 0.5
RF24 radio(9,10);
const uint64_t pipes[2] = { 0xF0F0F0F0E1LL, 0xF0F0F0F0D2LL };
float T2_cur;
float T1_cur;
String MSSID="smartumhome";
String MPASS="smartumkharkov0612";                                          
String MWiFi;
String todo;
long S1=2;
int chtemp;
int radioRetry;
unsigned long time;
SoftwareSerial softSerial(2,3);                                      
OneWire  ds(7);
int K1,K2,K3,K4; 
//DS1302 rtc(A0, A1, A2);

void begin_radio(){

  radio.begin();
  radio.setRetries(15,15);
  radio.setChannel  ( 100 ) ;
  radio.openWritingPipe(pipes[0]);
  radio.openReadingPipe(1,pipes[1]);
  radio.setPALevel(RF24_PA_HIGH);// Указываем мощность передатчика (MIN=-18dBm, LOW=-12dBm, HIGH=-6dBm, MAX=0dBm)
  radio.startListening();

}                                                                   
void(* resetFunc) (void) = 0; // Reset MC function
 
void get_data(void){
 char tmp[10];

    radio.stopListening();
    radio.openWritingPipe(pipes[0]);
    radio.openReadingPipe(1,pipes[1]);
    unsigned long time = 7;
    delay(20);
    bool ok = radio.write( &time, sizeof(unsigned long) );
    T2_cur=-5;
    radio.startListening();
    unsigned long started_waiting_at = millis();
    bool timeout = false;
    while ( ! radio.available() && ! timeout )
      if (millis() - started_waiting_at > 2000 )//3000
        timeout = true;
    if ( timeout )
    {
//      printf("Failed, response timed out.\n\r");
    }
    else
    {
      float  got;
      radio.read( &got, sizeof(float) );
      T2_cur= got;
    }
    Serial.println(T2_cur);
}
void setup(){      
/*  rtc.halt(false);
  rtc.writeProtect(false);                                                 
  */
 
  Serial.begin(57600);
  printf_begin;
  begin_radio();                                        
}

void loop(){
//  exit;
get_data();

}
