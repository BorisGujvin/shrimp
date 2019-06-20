/*
   Пример простейшего таймера на дисплее с регистром TM1637
   AlexGyver Technologies https://alexgyver.ru/
*/
/* // нумерация портов слева направо!
   tm1637.display(port, num);  // вывести цифру num (0-9) в порт port (0-3)
   tm1637.display(ARRAY);      // вывести ARRAY, где ARRAY - массив из 4х цифр ( byte ARRAY[4] = {0, 1, 2, 3}; )
   tm1637.point(state);        // включить/выключить двоеточие (1 и 0 соответственно)
   tm1637.clearDisplay();      // очистить дисплей
   tm1637.set(7);              // яркость, 0 - 7 (минимум - максимум)
*/
//--- ПИНЫ ---
#define CLK 5
#define DIO 4
//--- ПИНЫ ---
byte sec, mins;
byte TIME[4];
unsigned long halfsec;
boolean dots;
#include "TM1637.h"
TM1637 tm1637(CLK, DIO);
void setup()
{
  tm1637.init();  // инициализация
  tm1637.set(7);  // яркость, 0 - 7 (минимум - максимум)
}
void loop() {
  if (millis() - halfsec > 500) {  // если прошло полсекунды
    halfsec = millis();            // сбросить таймер
    dots = !dots;                  // переключить точки
    if (dots) tm1637.point(0);     // выключить точки
    else tm1637.point(1);          // включить точки
    sec = millis() / 1000;         // получить секунды со старта Ардуины
    mins = floor(sec / 60);        // получить целые минуты из секунд
    sec = sec - (mins * 60);       // найти оставшиеся целые секунды
    TIME[0] = mins / 10;           // получить десятки минут
    TIME[1] = mins % 10;           // получить единицы минут
    TIME[2] = sec / 10;            // получить десятки секунд
    TIME[3] = sec % 10;            // получить единицы секунд
    tm1637.display(TIME);          // вывести массив на дисплей
  }
}
