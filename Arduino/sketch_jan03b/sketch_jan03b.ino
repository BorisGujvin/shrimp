#include<EEPROM.h>
float T1;
byte got_time;
void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
 EEPROM.get(0,T1);
 Serial.println(T1);
  got_time=B10011000;
  Serial.println(got_time);
      if((got_time & B10000000)!=0){
         got_time = got_time & B01111111;
           Serial.println(got_time);
         T1=float(got_time);
         Serial.println(T1);
         EEPROM.put(0,T1);
       }
}

void loop() {

}
