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
long S1;
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
  radio.startListening();

}                                                                   
boolean WaitForOK(){
  unsigned long time=millis();
  int pos;
  do { 
     MyLCD("WfOK","start");
   if(mtrace) Serial.print('.');
   String Answer = softSerial.readString();
   if(mtrace) Serial.println(Answer);
    pos = Answer.indexOf('OK');
    if (millis()-time>15000){ MyLCD("WfOK","abort");return false; }
  }while (pos == -1);
   MyLCD("WfOK","done"); 
  return true;
}
boolean WaitForToDo(){
  unsigned long time=millis();
  long pos, fin;
  do {
     MyLCD("wftodo","start"); 
   if(mtrace) Serial.print('+');
   String Answer = softSerial.readString();
     pos = Answer.indexOf("todolist");
    if (pos!=-1 &&mtrace){
      fin = Answer.indexOf("fin");
      todo = Answer.substring(pos+9, fin);
      AnalizeCommand(todo);
      if(todo.substring(0,2)=="ON") {digitalWrite (K2, LOW);};
      if(todo.substring(0,2)=="OF") {digitalWrite (K2, HIGH);};
      if(todo.substring(3,5)=="ON") {digitalWrite (K3, LOW);};
      if(todo.substring(3,5)=="OF") {digitalWrite (K3, HIGH);};
      if(todo.substring(6,8)=="ON") {digitalWrite (K4, LOW);};
      if(todo.substring(6,8)=="OF") {digitalWrite (K4, HIGH);};
      if((todo.substring(9,11)=="ON") && (chtemp!=1)) {S1=1;};
      if((todo.substring(9,11)=="OF") && (chtemp!=1)) {S1=0;};
      chtemp=0;
  
  };
      
    if (millis()-time>15000){ MyLCD("wfTodo","abort");return false; }
  }while (pos == -1);
   MyLCD("wfTodo","done"); 
  return true;
}
void AnalizeCommand(String S){
  S=S.substring(S.indexOf("---")+3);
  while(S.length()>2){
    String com,N,d;
    com = S.substring(0,S.indexOf(" "));
    S=S.substring(S.indexOf(" ")+1);
    N=com.substring(0,com.indexOf("="));
    d=com.substring(com.indexOf("=")+1);
    switch(N.toInt()){
      case 1:
        T1_ = d.toFloat();
        EEPROM.put(0,T1_);
        break;
      case 2:
      int i=round(d.toFloat());
        i = i | B10000000;
        Serial.print("значение для низа ");
        Serial.println(i);
        chtemp=1;
        S1 = i;
        break;
    }
  }
}
float getTemp(void) {
  byte i;byte present = 0;byte type_s; byte data[12];float celsius;
//  byte addr[]={0x28,0x8e,0x8f,0x45,0x92,0x18,0x2,0x63};
  char tmp[10];
   MyLCD("getTemp","start");
  ds.reset();ds.select(addr);ds.write(0x44, 1);delay(1000);present = ds.reset();ds.select(addr);ds.write(0xBE);for ( i = 0; i < 9; i++) {data[i] = ds.read();}int16_t raw = (data[1] << 8) | data[0];
  if (type_s) {raw = raw << 3;  if (data[7] == 0x10) {raw = (raw & 0xFFF0) + 12 - data[6];}} else {byte cfg = (data[4] & 0x60);if (cfg == 0x00) raw = raw & ~7; else if (cfg == 0x20) raw = raw & ~3;
  else if (cfg == 0x40) raw = raw & ~1; } celsius = (float)raw / 16.0; dtostrf(celsius, 5, 1, tmp);MyLCD("GT Done",tmp);return(celsius);
   }
void(* resetFunc) (void) = 0; // Reset MC function
 
void SendData(){
  char mbuffer[50];
  char tmp[10], tmp1[10];
  int retr=0;
  T1_cur=getTemp();
 dtostrf(T1_cur, 5, 3, tmp);
 dtostrf(T2_cur, 5, 3, tmp1);
  MyLCD("SendData","start");
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
    if (retr==10) resetFunc();
 }while (!WaitForToDo());
 MyLCD("SendData","done");
}
void get_data(void){
 char tmp[10];
 radioRetry++;
 if(radioRetry==20){
    MyLCD("No radio","connect");
    delay(5000);
    resetFunc();  
 }
  MyLCD("Get_data","start");
    radio.stopListening();
    radio.openWritingPipe(pipes[0]);
    radio.openReadingPipe(1,pipes[1]);
    unsigned long time = S1;
    Serial.print(S1);
    delay(20);
    bool ok = radio.write( &time, sizeof(unsigned long) );
    T2_cur=-5;
    radio.startListening();
    unsigned long started_waiting_at = millis();
    bool timeout = false;
    while ( ! radio.available() && ! timeout )
      if (millis() - started_waiting_at > 3000 )
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
  if(T2_cur==-5){
       begin_radio();
       get_data();} else {S1=2;};
   MyLCD("get_data","done");
}
void MyLCD(String s1, String s2){
  lcd.clear();
  lcd.print(s1);
  lcd.setCursor(0,1);
  lcd.print(s2);
}
void seekTerm(){
    while ( !ds.search(addr))  Serial.println("Нет термометра((");
  Serial.println("Термометр найден");
} 
void setup(){      
/*  rtc.halt(false);
  rtc.writeProtect(false);                                                 
  */
 
  EEPROM.get(0,T1_);
  Serial.begin(9600);
  lcd.begin(16, 2);
  MyLCD("seek term","");

  seekTerm();
  MyLCD("Shrimp!","Wow");
  softSerial.begin(9600);
  printf_begin();
  begin_radio();                                         
//  AnalizeCommand(" ON OF ON ON ---1=23.3 2=29.1 ");  
  MWiFi = "AT+CWJAP=\""+MSSID+"\",\""+MPASS+"\"";
  time=millis();
  K1 = 4 ; K2 = 5 ; K3 = 6; K4 = 7;
  pinMode(K1 , OUTPUT);pinMode(K2 , OUTPUT);pinMode(K3 , OUTPUT);pinMode(K4 , OUTPUT);
  digitalWrite (K1, HIGH);digitalWrite (K2, HIGH);digitalWrite (K3, HIGH);digitalWrite (K4, HIGH);
}
void actualize(){
    radioRetry=0;
    get_data();
    T1_cur = getTemp();
    if (T1_cur<T1_)digitalWrite (K1, LOW);
    if (T1_cur>T1_+D1_)digitalWrite (K1, HIGH);
}
void wait(int s){
  unsigned long p = millis();
   while((millis()-time)/1000<s){
    actualize();
    delay(5000);
  }
  time=millis();
  //  Serial.println(rtc.getTimeStr());
}
void loop(){
  radioRetry=0;
  get_data();  
  SendData();
 wait(period);   

}
