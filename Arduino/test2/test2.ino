#include <SoftwareSerial.h>
#include <OneWire.h>

#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
// #include "printf.h"

#define period 300
#define mtrace TRUE
#define T1_ 27
#define K1 1 
#define K2 2
#define K3 3
#define K4 4
long S1;
boolean WaitForToDo(){
  unsigned long time=millis();
  long pos, fin;
  do { 
   if(mtrace) Serial.print('+');
   String Answer = "todolist ON OF OF ON fin";
   String todo;
     pos = Answer.indexOf("todolist");
    if (pos!=-1 ){
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
      Serial.print(S1);
    if (millis()-time>15000){ return false; }
  }while (pos == -1); 
  return true;
}

void setup(){                                                       
Serial.begin(9600);
WaitForToDo();
}
void loop(){
}
