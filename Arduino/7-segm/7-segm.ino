#include "timer-api.h"

int R[]={0,10,11,12,13}; //ноги для разрядов
int Current, c;
 int Dig[]={0,0,0,0,0};
int symbol[]={  B11000000,  //0
                B11111010,  //1
                B10100100,  //2
                B10110000,  //3
                B10011010,  //4
                B10010001,  //5
                B10000001,  //6
                B11111000,  //7
                B10000000,  //8
                B10010000   //9
                };
void timer_handle_interrupts(int timer) {
  digitalWrite(R[Current],LOW);
  Current++;
  if(Current==5)Current=1;
  PORTD=symbol[Dig[Current]];//выводим сразу в восемь ног на порту D
  digitalWrite(R[Current],HIGH);
    
}
void setup() {
  // put your setup code here, to run once:
  timer_init_ISR_200Hz(TIMER_DEFAULT);
for (int i=0;i<8;i++) pinMode(i,OUTPUT);
for (int i=1;i<5;i++) pinMode(R[i],OUTPUT);
 Current=1;
c=0;
}
;
void loop() {
  c++;
  Dig[4]=c % 10;
  Dig[3]=(c % 100)/10;
  Dig[2]=(c % 1000)/100;
  Dig[1]=(c % 10000)/1000;
delay(1000);
}
