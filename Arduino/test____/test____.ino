#include <SoftwareSerial.h>
#include <OneWire.h>

#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
// #include "printf.h"

#define period 300
#define mtrace TRUE
#define T1_ 27
#define D1_ 1
RF24 radio(9,10);
const uint64_t pipes[2] = { 0xF0F0F0F0E1LL, 0xF0F0F0F0D2LL };
float T2_cur;
float T1_cur;
String MSSID="smartumhome";
String MPASS="smartumkharkov0612";                                          
String MWiFi;
String todo;
long S1;
unsigned long time;
SoftwareSerial softSerial(2,3);                                      
 OneWire  ds(7);
 int K1,K2,K3,K4; 
 void begin_radio(){
//  printf_begin();
  radio.begin();
  radio.setRetries(15,15);
  radio.setChannel  ( 100 ) ;
  radio.openWritingPipe(pipes[0]);
  radio.openReadingPipe(1,pipes[1]);
  radio.startListening();

}                                                                   
void get_data(void){
 char tmp[10];
    radio.stopListening();
    radio.openWritingPipe(pipes[0]);
    radio.openReadingPipe(1,pipes[1]);
 Serial.println("Запрос");
//    printf("Now sending request ");
    unsigned long time = 0;
    delay(20);
    bool ok = radio.write( &time, sizeof(unsigned long) );
    T2_cur=-1;
    radio.startListening();
    unsigned long started_waiting_at = millis();
    bool timeout = false;
    while ( ! radio.available() && ! timeout )
      if (millis() - started_waiting_at > 3000 )
        timeout = true;

    // Describe the results
    if ( timeout )
    {
      Serial.println("Failed, response timed out.\n\r");
    }
    else
    {
      float  got;
      radio.read( &got, sizeof(float) );
      T2_cur= got;
      dtostrf(got, 5, 3, tmp);
     Serial.println(tmp);
    }
  
}
void setup(){                                                       
softSerial.begin(9600);
  begin_radio();                                         
if(mtrace)Serial.begin(9600);
  MWiFi = "AT+CWJAP=\""+MSSID+"\",\""+MPASS+"\"";
  time=millis();
  K1 = 4 ; K2 = 5 ; K3 = 6; K4 = 7;
  pinMode(K1 , OUTPUT);pinMode(K2 , OUTPUT);pinMode(K3 , OUTPUT);pinMode(K4 , OUTPUT);
  digitalWrite (K1, HIGH);digitalWrite (K2, HIGH);digitalWrite (K3, HIGH);digitalWrite (K4, HIGH);
}
void loop(){
  get_data();
  if(mtrace){Serial.print("Start duos. T2=");Serial.println(T2_cur);};
if(mtrace) if(softSerial.available()){    Serial.write(softSerial.read());} // Передаём данные из программной шины UART в аппаратную  (от модуля     через Arduino к компьютеру)
if(mtrace) if(    Serial.available()){softSerial.write(    Serial.read());} // Передаём данные из аппаратной  шины UART в программную (от компьютера через Arduino к модулю    )
   

}
