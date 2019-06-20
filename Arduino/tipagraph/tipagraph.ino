#include "GyverTM1637.h"
GyverTM1637 disp(12, 11);
#include <SoftwareSerial.h>
#include<EEPROM.h>
#include <OneWire.h>
#include <SPI.h>
byte addr[8];

#define period 300
#define mtrace TRUE
float T1_;
#define D1_ 0.5
float T1_cur;
String MSSID="smartumhome";
String MPASS="smartumkharkov0612";                                          
String MWiFi;
String todo;
long S1=2;
int chtemp;
unsigned long time;
SoftwareSerial softSerial(2,3);                                      
OneWire  ds(7);
int K1,K2; 
boolean WaitForOK(){
  unsigned long time=millis();
  int pos;
  do { 
   if(mtrace) Serial.print('.');
   String Answer = softSerial.readString();
   if(mtrace) Serial.println(Answer);
    pos = Answer.indexOf('OK');
    if (millis()-time>15000){return false; }
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
      Serial.print("Анализируем ");Serial.print(todo);
      AnalizeCommand(todo);
    };
      
    if (millis()-time>15000){ return false; }
  }while (pos == -1);
  return true;
}
void AnalizeCommand(String S){
  Serial.println();
  Serial.println();
  Serial.println(S);
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
          case 4:
            T1_ = d.toFloat();
            EEPROM.put(0,T1_);
            Serial.println("p4 set");
            break;
        };
        break;
      case 107://------'k'-------:
         switch (N1){
        case 2:
          if (D==1) {digitalWrite (K2, LOW);}
          if (D==0) {digitalWrite (K2, HIGH);}
          break;
      }
    }
  }
}
float getTemp(void) {
  byte i;byte present = 0;byte type_s; byte data[12];float celsius;
//  byte addr[]={0x28,0x8e,0x8f,0x45,0x92,0x18,0x2,0x63};
  char tmp[10];

  ds.reset();ds.select(addr);ds.write(0x44, 1);delay(1000);present = ds.reset();ds.select(addr);ds.write(0xBE);for ( i = 0; i < 9; i++) {data[i] = ds.read();}int16_t raw = (data[1] << 8) | data[0];
  if (type_s) {raw = raw << 3;  if (data[7] == 0x10) {raw = (raw & 0xFFF0) + 12 - data[6];}} else {byte cfg = (data[4] & 0x60);if (cfg == 0x00) raw = raw & ~7; else if (cfg == 0x20) raw = raw & ~3;
  else if (cfg == 0x40) raw = raw & ~1; } celsius = (float)raw / 16.0; dtostrf(celsius, 5, 1, tmp);return(celsius);
   }
void(* resetFunc) (void) = 0; // Reset MC function
 
void SendData(){
  char mbuffer[50];
  char tmp[10];
  int retr=0;
 T1_cur=getTemp();
 do {
  Serial.println("senddata start");
    actualize();
    dtostrf(T1_cur, 5, 3, tmp);
    disp_symbol(0);
    retr++;
    softSerial.println("AT+RST");
    if(!WaitForOK())continue;
    softSerial.println("AT+CWMODE=1");
    if(!WaitForOK())continue;
    disp_symbol(1);
    softSerial.println(MWiFi);
    if(!WaitForOK())continue;
    disp_symbol(2);
    softSerial.println("AT+CIPSTART=\"TCP\",\"shrimp.pp.ua\",80");
    if(!WaitForOK())continue;
    disp_symbol(3);
    sprintf(mbuffer, "GET /datatrans.php?pass=ardu&retry=%d&p4=%s HTTP/1.1\r\nHost: shrimp.pp.ua\r\n\r\n",retr,tmp);
    int j=1;
    do{ j++;} while (mbuffer[j]!=0);
    //GZ=GZ+" HTTP/1.1\r\nHost: shrimp.pp.ua\r\n\r\n";
    softSerial.print("AT+CIPSEND=");softSerial.println(j);
    if(!WaitForOK())continue;
    disp_symbol(4);
    if(mtrace) Serial.println(mbuffer);
    softSerial.println(mbuffer);
    if (retr==10) return;
 }while (!WaitForToDo());
}

void seekTerm(){
  disp_symbol(8);
    while ( !ds.search(addr))  Serial.println("Нет термометра((");
  Serial.println("Термометр найден");
   disp_symbol(0);
} 
void setup(){     
  pinMode(9,OUTPUT);
  pinMode(10,OUTPUT);
  digitalWrite(10,LOW);
  digitalWrite(9,HIGH); 
  disp.clear();
  disp.brightness(7);  // яркость, 0 - 7 (минимум - максимум)
  
  EEPROM.get(0,T1_);
  Serial.begin(9600);
  seekTerm();
  softSerial.begin(9600);
                                   
  MWiFi = "AT+CWJAP=\""+MSSID+"\",\""+MPASS+"\"";
  time=millis();
  K1 = 4 ; K2 = 5;
  pinMode(K1 , OUTPUT);pinMode(K2 , OUTPUT);
  digitalWrite (K1, HIGH);digitalWrite (K2, HIGH);
}
void actualize(){
    T1_cur = getTemp();
    disp_float(T1_cur);
    Serial.println(T1_cur);
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
  Serial.println(time/1000);
}

void disp_float(float F){
  byte di[] ={63,6,91,79,102,109,125,39,127,111,99};
unsigned int RAWtemp = F * 100;
int8_t Digits[] = {0x00,0x00,0x00,0x00};  // буфер для цифр LED-индикатора
Digits[0] = di[(byte)(RAWtemp / 1000) % 10]; // раскидываем 4-значное число на цифры
Digits[1] = di[((byte)(RAWtemp / 100) % 10)]|B10000000;
Digits[2] = di[(byte)(RAWtemp / 10) % 10];
Digits[3] = 99;
disp.displayByte(Digits);
}

void disp_symbol(int i){
  int8_t Digits[] = {64,64,64,64};  // буфер для цифр LED-индикатора
  disp.displayByte(Digits);
  disp.display(3,i);
}


void loop(){
  SendData();
  wait(period);   
}
