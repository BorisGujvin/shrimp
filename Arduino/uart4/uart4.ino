#include <SoftwareSerial.h>
#include <OneWire.h>

#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
#include "printf.h"

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
boolean WaitForOK(){
  unsigned long time=millis();
  int pos;
  do { 
   if(mtrace) Serial.print('.');
   String Answer = softSerial.readString();
   if(mtrace) Serial.println(Answer);
    pos = Answer.indexOf('OK');
    if (millis()-time>15000){ return false; }
  }while (pos == -1); 
  return true;
}
boolean WaitForToDo(){
  unsigned long time=millis();
  long pos, fin;
  do { 
   if(mtrace) Serial.print('+');
   String Answer = softSerial.readString();
     pos = Answer.indexOf("todolist");
    if (pos!=-1 &&mtrace){
      fin = Answer.indexOf("fin");
      todo = Answer.substring(pos+9, fin);
      Serial.print("---------------");Serial.print(todo);Serial.println("---------------");
      Serial.println(todo.substring(0,2));Serial.print("II");
      Serial.println(todo.substring(3,5));
      if(todo.substring(0,2)=="ON") {digitalWrite (K2, LOW);};
      if(todo.substring(0,2)=="OF") {digitalWrite (K2, HIGH);};
      if(todo.substring(3,5)=="ON") {digitalWrite (K3, LOW);};
      if(todo.substring(3,5)=="OF") {digitalWrite (K3, HIGH);};
      if(todo.substring(6,8)=="ON") {digitalWrite (K4, LOW);};
      if(todo.substring(6,8)=="OF") {digitalWrite (K4, HIGH);};
      if(todo.substring(9,11)=="ON") {S1=1;};
      if(todo.substring(9,11)=="OF") {S1=0;};
      
  
  };
      
    if (millis()-time>15000){ return false; }
  }while (pos == -1); 
  return true;
}
float getTemp(void) {
  byte i;
  byte present = 0;
  byte type_s;
  byte data[12];
  byte addr[8];
  float celsius, fahrenheit;

addr[0]=0x28; addr[1]=0x8e;addr[2]=0x8f;addr[3]=0x45;addr[4]=0x92;addr[5]=0x18;addr[6]=0x2;addr[7]=0x63;
  ds.reset();
  ds.select(addr);
  ds.write(0x44, 1);        // start conversion, with parasite power on at the end
  delay(1000);
  present = ds.reset();
  ds.select(addr);    
  ds.write(0xBE);         // Read Scratchpad
  for ( i = 0; i < 9; i++) {           // we need 9 bytes
    data[i] = ds.read();  }
  int16_t raw = (data[1] << 8) | data[0];
  if (type_s) {
    raw = raw << 3; // 9 bit resolution default
    if (data[7] == 0x10) {
      // "count remain" gives full 12 bit resolution
      raw = (raw & 0xFFF0) + 12 - data[6];
    }
  } else {
    byte cfg = (data[4] & 0x60);
    // at lower res, the low bits are undefined, so let's zero them
    if (cfg == 0x00) raw = raw & ~7;  // 9 bit resolution, 93.75 ms
    else if (cfg == 0x20) raw = raw & ~3; // 10 bit res, 187.5 ms
    else if (cfg == 0x40) raw = raw & ~1; // 11 bit res, 375 ms
    //// default is 12 bit resolution, 750 ms conversion time
  }
  celsius = (float)raw / 16.0;
  fahrenheit = celsius * 1.8 + 32.0;
 // Serial.print("  Temperature = ");
  //Serial.print(celsius);
  //Serial.println(" Celsius ");
  return(celsius);
 }
void SendData(){
  char mbuffer[50];
  char tmp[10], tmp1[10];
  int retr=0;
  T1_cur=getTemp();
 dtostrf(T1_cur, 5, 3, tmp);
 dtostrf(T2_cur, 5, 3, tmp1);
 
 do {
    actualize();
    retr++;
    softSerial.println("AT+RST");
    if(!WaitForOK())continue;
    softSerial.println("AT+CWMODE=1");
    if(!WaitForOK())continue;
    softSerial.println(MWiFi);
    if(!WaitForOK())continue;
    softSerial.println("AT+CIPSTART=\"TCP\",\"shrimp.pp.ua\",80");
    if(!WaitForOK())continue;
    sprintf(mbuffer, "GET /datatrans.php?pass=ardu&retry=%d&p1=%s&p2=%s HTTP/1.1\r\nHost: shrimp.pp.ua\r\n\r\n",retr,tmp,tmp1);
    int j=1;
    do{ j++;} while (mbuffer[j]!=0);
    //GZ=GZ+" HTTP/1.1\r\nHost: shrimp.pp.ua\r\n\r\n";
    softSerial.print("AT+CIPSEND=");softSerial.println(j);
    if(!WaitForOK())continue;
    if(mtrace) Serial.println(mbuffer);
    softSerial.println(mbuffer);
 }while (!WaitForToDo());
}
void get_data(void){
 char tmp[10];
    radio.stopListening();
    radio.openWritingPipe(pipes[0]);
    radio.openReadingPipe(1,pipes[1]);

//    printf("Now sending request ");
    unsigned long time = S1;
    Serial.print(S1);
    delay(20);
    bool ok = radio.write( &time, sizeof(unsigned long) );
 //   S1=2;
    T2_cur=-5;
    radio.startListening();
    unsigned long started_waiting_at = millis();
    bool timeout = false;
    while ( ! radio.available() && ! timeout )
      if (millis() - started_waiting_at > 3000 )
        timeout = true;

    // Describe the results
    if ( timeout )
    {
//      printf("Failed, response timed out.\n\r");
    }
    else
    {
      float  got;
      radio.read( &got, sizeof(float) );
      T2_cur= got;
      dtostrf(got, 5, 3, tmp);
  //   printf("Ответ  %s ",tmp);
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
void actualize(){
    get_data();
    T1_cur = getTemp();
    if (T1_cur<T1_)digitalWrite (K1, LOW);
    if (T1_cur>T1_+D1_)digitalWrite (K1, HIGH);
}
void wait(int s){
  unsigned long p = millis();
   while((millis()-time)/1000<s){
    actualize();
    if(mtrace) if(softSerial.available()){    Serial.write(softSerial.read());} // Передаём данные из программной шины UART в аппаратную  (от модуля     через Arduino к компьютеру)
    if(mtrace) if(    Serial.available()){softSerial.write(    Serial.read());} // Передаём данные из аппаратной  шины UART в программную (от компьютера через Arduino к модулю    )
  }
  time=millis();
}
void loop(){
 // get_data();
  SendData();
if(mtrace) if(softSerial.available()){    Serial.write(softSerial.read());} // Передаём данные из программной шины UART в аппаратную  (от модуля     через Arduino к компьютеру)
if(mtrace) if(    Serial.available()){softSerial.write(    Serial.read());} // Передаём данные из аппаратной  шины UART в программную (от компьютера через Arduino к модулю    )
 wait(period);   

}
