
#include <SPI.h>
#include<EEPROM.h>
#include "nRF24L01.h"
#include "RF24.h"
#include "printf.h"
#include <OneWire.h>

RF24 radio(9,10);
 OneWire  ds(2);
 float T1_cur;
 float T1 = 22;
 const float D = 0.5;
 const int K1 = 3, K2 = 4;
byte addr[8];
void seekTerm(){
    while ( !ds.search(addr))  Serial.println("Нет термометра((");
  Serial.println("Термометр найден");
} 
// Radio pipe addresses for the 2 nodes to communicate.
const uint64_t pipes[2] = { 0xF0F0F0F0E1LL, 0xF0F0F0F0D2LL };

void setup(void){
  //EEPROM.put(0,26.0);
  EEPROM.get(0,T1);
  Serial.begin(57600);
  Serial.println(T1);exit;
  printf_begin();
  seekTerm();
  printf("\n\rпрограмма для аквариума на первом этаже  v 8.0\n\r");
  radio.begin();
  radio.setRetries(15,15);
  radio.setPALevel(RF24_PA_MAX);// Указываем мощность передатчика (MIN=-18dBm, LOW=-12dBm, HIGH=-6dBm, MAX=0dBm)
  radio.setChannel  ( 5 ) ;
       radio.openWritingPipe(pipes[1]);
      radio.openReadingPipe(1,pipes[0]);//главный - 0, мы-1
  radio.startListening();
  radio.printDetails();
  pinMode(K1,OUTPUT);pinMode(K2,OUTPUT);
  digitalWrite(K1,HIGH);digitalWrite(K2,HIGH);
}
float getTemp(void) {
  byte i; byte present = 0; byte type_s;byte data[12];float celsius;

//byte addr[]={0x28,0x21,0x11,0x66,0x1e,0x13,0x1,0xee};
  ds.reset(); ds.select(addr);ds.write(0x44, 1);  delay(300); present = ds.reset(); ds.select(addr);ds.write(0xBE); for ( i = 0; i < 9; i++) {data[i] = ds.read(); }
  int16_t raw = (data[1] << 8) | data[0]; if (type_s) {raw = raw << 3; if (data[7] == 0x10) {raw = (raw & 0xFFF0) + 12 - data[6];}} else {
    byte cfg = (data[4] & 0x60);if (cfg == 0x00) raw = raw & ~7; else if (cfg == 0x20) raw = raw & ~3;  else if (cfg == 0x40) raw = raw & ~1; }celsius = (float)raw / 16.0;return(celsius);
 }
void loop(void)
{
    if ( radio.available() )
    {
      // Dump the payloads until we've gotten everything
      unsigned long got_time;
      bool done = false;
      while (!done)
      {
        done = radio.read( &got_time, sizeof(unsigned long) );
       byte op = got_time;
       if (op!=2){
            if((op | 64)==op){digitalWrite(K2, LOW);Serial.println("light on");}else {digitalWrite(K2, HIGH);Serial.println("light off");};};
       if((op & B10000000)!=0){
         op = op & B00111111;
         T1=float(op);
         EEPROM.put(0,T1);
       }
      }
      radio.stopListening();
      
      radio.write( &T1_cur, sizeof(float) );
      radio.startListening();
    }
    float Tmp=getTemp();
    if(Tmp>0) {T1_cur  = Tmp;} else {T1_cur=100;};
    Serial.print(T1_cur);
    Serial.print("----");
    Serial.println(T1);
   
    if (T1_cur>(T1+D)) {digitalWrite(K1, HIGH);};
    if (T1_cur<T1)  { digitalWrite(K1,LOW);};
}

