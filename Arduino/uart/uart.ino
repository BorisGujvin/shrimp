#include <SoftwareSerial.h>                                          // Подключаем библиотеку SoftwareSerial для общения с модулем по программной шине UART
SoftwareSerial softSerial(2,3);                                      // Создаём объект softSerial указывая выводы RX, TX (можно указывать любые выводы Arduino UNO)
                                                                     // В данном случае вывод TX модуля подключается к выводу 2 Arduino, а вывод RX модуля к выводу 3 Arduino.
//  Инициируем работу шин UART с указанием скоростей обеих шин:      //
void WaitForOK(){
  int pos;
  do { 
    Serial.print('.');
   String Answer = softSerial.readString();
   Serial.println(Answer);
    pos = Answer.indexOf('OK');
  }while (pos == -1); 
}
void setup(){                                                        //
softSerial.begin(115200);                                         // Инициируем передачу данных по программной шине UART на скорости 38400 (между модулем и Arduino)
    Serial.begin(9600); 

}

void loop(){                                                         //
   if(softSerial.available()){    Serial.write(softSerial.read());} // Передаём данные из программной шины UART в аппаратную  (от модуля     через Arduino к компьютеру)
   if(    Serial.available()){softSerial.write(    Serial.read());} // Передаём данные из аппаратной  шины UART в программную (от компьютера через Arduino к модулю    )
}
