#include <Servo.h> //используем библиотеку для работы с сервоприводом

Servo servo; //объявляем переменную servo типа Servo

void setup() //процедура setup

{

servo.attach(10); //привязываем привод к порту 10

}

void loop() //процедура loop

{

servo.write(0); //ставим вал под 0

delay(5000); //ждем 2 секунды

servo.write(180); //ставим вал под 180

delay(2000); //ждем 2 секунды

}

