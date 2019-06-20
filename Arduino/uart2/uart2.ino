#include <SoftwareSerial.h>
#include <OneWire.h>
#define period 300
#define mtrace FALSE
String MSSID="smartumhome";
String MPASS="smartumkharkov0612";                                          
String MWiFi;
unsigned long time;
SoftwareSerial softSerial(2,3);                                      
 OneWire  ds(10);
 int K1,K2,K3,K4;                                                                    
boolean WaitForOK(){
  unsigned long time=millis();
  int pos;
  do { 
   // Serial.print('.');
   String Answer = softSerial.readString();
   if(mtrace) Serial.println(Answer);
    pos = Answer.indexOf('OK');
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
  float t1;
  t1=getTemp();
  char ts1[]="1234567";
 dtostrf(t1, 5, 3, ts1);
 softSerial.println("AT+RST");
  WaitForOK();
  softSerial.println("AT+CWMODE=1");
  WaitForOK();
  softSerial.println(MWiFi);
  WaitForOK();
  softSerial.println("AT+CIPSTART=\"TCP\",\"shrimp.pp.ua\",80");
  WaitForOK();

  String GZ = "GET /datatrans.php?pass=ardu&p1=";
  int i=0;
  while (ts1[i]!='\0'){
    GZ=GZ+ts1[i]; i++;};
   GZ=GZ+" HTTP/1.1\r\nHost: shrimp.pp.ua\r\n\r\n";
  softSerial.print("AT+CIPSEND=");softSerial.println(GZ.length());

  WaitForOK();
  if(mtrace) Serial.println(GZ);
  softSerial.println(GZ);
}
void setup(){                                                        //
softSerial.begin(9600);                                         // Инициируем передачу данных по программной шине UART на скорости 38400 (между модулем и Arduino)
if(mtrace)    Serial.begin(9600);
  MWiFi = "AT+CWJAP=\""+MSSID+"\",\""+MPASS+"\"";
  time=millis();
  K1 = 4 ; K2 = 5 ; K3 = 6; K4 = 7;
  pinMode(K1 , OUTPUT);pinMode(K2 , OUTPUT);pinMode(K3 , OUTPUT);pinMode(K4 , OUTPUT);
  digitalWrite (K1, LOW);digitalWrite (K2, LOW);digitalWrite (K3, LOW);digitalWrite (K4, LOW);
}
void wait(int s){
  unsigned long p = millis();
  int ledState=LOW;
  while((millis()-time)/1000<s){
/*  if (millis()-p>1000){
    p=millis();
    digitalWrite (K1, !digitalRead(K1)); 
  }*/
  }
  time=millis();

}
void loop(){
  SendData();
 // if(softSerial.available()){    Serial.write(softSerial.read());} // Передаём данные из программной шины UART в аппаратную  (от модуля     через Arduino к компьютеру)
 // if(    Serial.available()){softSerial.write(    Serial.read());} // Передаём данные из аппаратной  шины UART в программную (от компьютера через Arduino к модулю    )
 wait(period);   

}
