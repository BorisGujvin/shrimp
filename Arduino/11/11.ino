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
