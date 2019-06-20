//#include <SoftwareSerial.h>
#include<EEPROM.h>
#include <LiquidCrystal.h>
#include <OneWire.h>
#include <DS1302.h>
#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
#include "printf.h"
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);
byte addr[8];

#define period 300
#define mtrace TRUE
float T1_;
#define D1_ 0.5
RF24 radio(53,49);//(9,10);
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
//SoftwareSerial softSerial(2,3);                                      
OneWire  ds(14);
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
  radio.printDetails();
  
}                                                                   
boolean WaitForOK(){
  unsigned long time=millis();
  int pos;
  do { 
     MyLCD("WfOK","start");
   if(mtrace) Serial.print('.');
   String Answer = Serial1.readString();
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
   String Answer = Serial1.readString();
     pos = Answer.indexOf("todolist");
    if (pos!=-1 &&mtrace){
      fin = Answer.indexOf("fin");
      todo = Answer.substring(pos+9, fin);
      Serial.print('Анализируем ');Serial.print(todo);
      AnalizeCommand(todo);
    };
      
    if (millis()-time>15000){ MyLCD("wfTodo","abort");return false; }
  }while (pos == -1);
   MyLCD("wfTodo","done"); 
  return true;
}
void AnalizeCommand(String S){
  S=S.substring(S.indexOf("---")+3);  S=S+' ';
  while(S.length()>2){
    String com, N,d;    char K;    int N1;    float D;
    com = S.substring(0,S.indexOf(" "));
    S=S.substring(S.indexOf(" ")+1);
    K=com.charAt(0);
    N=com.substring(1,com.indexOf("="));
    N1=N.toInt();
    d=com.substring(com.indexOf("=")+1);
    D=d.toFloat();
    printf("%i -s",K);
     switch(K){
      case 112://-----'p'------:
        switch (N1){
          case 1:
            T1_ = d.toFloat();
            EEPROM.put(0,T1_);
            Serial.println("p1 set");
            break;
          case 2:
            int i=round(d.toFloat());
            i = i | B10000000;
            chtemp=1;
            S1 = i;
            Serial.print("p2 set to ");
            Serial.println(i);
            break;
        };
        break;
      case 107://------'k'-------:
         switch (N1){
        case 2:
          if (D==1) {digitalWrite (K2, LOW);}
          if (D==0) {digitalWrite (K2, HIGH);}
          break;
        case 3:
          if (D==1) {digitalWrite (K3, LOW);}
          if (D==0) {digitalWrite (K3, HIGH);}
          break;
        case 4:
          if (D==1) {digitalWrite (K4, LOW);}
          if (D==0) {digitalWrite (K4, HIGH);}
          break;
        case 5:
          if (D==1) {S1=1;}
          if (D==0) {S1=0;}
          break;
      }
    }
  }
}
float getTemp(void) {
  byte i;byte present = 0;byte type_s; byte data[12];float celsius;
//  byte addr[]={0x28,0x8e,0x8f,0x45,0x92,0x18,0x2,0x63};
  char tmp[10];
  // MyLCD("getTemp","start");
  ds.reset();ds.select(addr);ds.write(0x44, 1);delay(1000);present = ds.reset();ds.select(addr);ds.write(0xBE);for ( i = 0; i < 9; i++) {data[i] = ds.read();}int16_t raw = (data[1] << 8) | data[0];
  if (type_s) {raw = raw << 3;  if (data[7] == 0x10) {raw = (raw & 0xFFF0) + 12 - data[6];}} else {byte cfg = (data[4] & 0x60);if (cfg == 0x00) raw = raw & ~7; else if (cfg == 0x20) raw = raw & ~3;
  else if (cfg == 0x40) raw = raw & ~1; } celsius = (float)raw / 16.0; dtostrf(celsius, 5, 1, tmp);MyLCD("GT Done",tmp);return(celsius);
   }
void(* resetFunc) (void) = 0; // Reset MC function
 
void SendData(){
  char mbuffer[100];
  char tmp[10], tmp1[10];
  int retr=0;
  T1_cur=getTemp();
 dtostrf(T1_cur, 5, 3, tmp);
 dtostrf(T2_cur, 5, 3, tmp1);
  MyLCD("SendData","start");
 do {
    actualize();
    retr++;
    Serial1.println("AT+RST");
    if(!WaitForOK())continue;
    Serial1.println("AT+CWMODE=1");
    if(!WaitForOK())continue;
    Serial1.println(MWiFi);
    if(!WaitForOK())continue;
    Serial1.println("AT+CIPSTART=\"TCP\",\"shrimp.pp.ua\",80");
    if(!WaitForOK())continue;
    sprintf(mbuffer, "GET /datatrans.php?pass=ardu&retry=%d&p1=%s&p2=%s HTTP/1.1\r\nHost: shrimp.pp.ua\r\n\r\n",retr,tmp,"-10");//tmp1);
    int j=1;
    do{ j++;} while (mbuffer[j]!=0);
    //GZ=GZ+" HTTP/1.1\r\nHost: shrimp.pp.ua\r\n\r\n";
    Serial1.print("AT+CIPSEND=");Serial1.println(j);
    if(!WaitForOK())continue;
    if(mtrace) Serial.println(mbuffer);
    Serial1.println(mbuffer);
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
 String retr=String(radioRetry);
  MyLCD("Get_data",retr);
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
      if (millis() - started_waiting_at > 3000)
        timeout = true;
    if ( timeout )
    {
     printf("Failed, response timed out.\n\r");
    }
    else
    {
      float  got;
      radio.read( &got, sizeof(float) );
      T2_cur= got;
      Serial.println(got);
    }
 /* if(T2_cur==-5){
       begin_radio();
       get_data();} else {S1=2;};*/
   MyLCD("get_data","done");
       radio.stopListening();
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
  K1 = 22 ; K2 = 23 ; K3 = 24; K4 = 25;
  pinMode(K1 , OUTPUT);pinMode(K2 , OUTPUT);pinMode(K3 , OUTPUT);pinMode(K4 , OUTPUT);pinMode(24,OUTPUT);
  digitalWrite (K1, HIGH);digitalWrite (K2, HIGH);digitalWrite (K3, HIGH);digitalWrite (K4, HIGH);
// AnalizeCommand(" ---p2=24.3 k2=1 k3=0 k4=0 k5=1");  
  EEPROM.get(0,T1_);
  Serial.begin(57600);
  printf_begin;
//  begin_radio();
  lcd.begin(16, 2);
  MyLCD("seek term","");
  seekTerm();
  MyLCD("Shrimp!","Wow");
  Serial1.begin(9600);
                                        
  MWiFi = "AT+CWJAP=\""+MSSID+"\",\""+MPASS+"\"";
  time=millis();
 
  }
void actualize(){
   // radioRetry=0;
   // get_data();
    T1_cur = getTemp();
    char tmp[10];
 dtostrf(T1_cur, 5, 3, tmp);
  MyLCD("Temp = ",tmp);

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
 actualize();  
 // SendData();
 //wait(period);   

}
