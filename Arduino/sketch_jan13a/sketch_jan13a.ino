#include <DS1302.h>
DS1302 rtc(2, 3, 4);
void setup() {
  // put your setup code here, to run once:
Serial.begin(57600);
MySetTime(" 17:04:55 ");
}
void MySetTime(String St){
   String hh,mm,ss;
   int H,M,S;
   hh=St.substring(1,3);
   mm=St.substring(4,6);
   ss=St.substring(7,9);
   H=hh.toInt();
   M=mm.toInt();
   S=ss.toInt();
   rtc.setTime(H, M, S);
   
}
void loop() {
  // put your main code here, to run repeatedly:

}
