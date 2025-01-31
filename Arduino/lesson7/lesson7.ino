const int leds[]={3,4,5,6,7,8,9,10};
const int LIGHT=A0; // Контакт A0 для входа фоторезистора
const int MIN_LIGHT=200; // Нижний порог освещенности
const int MAX_LIGHT=900; // верхний порог освещенности
// Переменная для хранения данных фоторезистора
int val = 0;
void setup()
{
// Сконфигурировать контакты светодиодов как выход
for(int i=0;i<8;i++)
pinMode(leds[i],OUTPUT);
pinMode(12,OUTPUT);
digitalWrite(12,HIGH);
}
void loop()
{
val = analogRead(LIGHT); // Чтение показаний фоторезистора
// Применение функции map()
val = map(val, MIN_LIGHT, MAX_LIGHT, 8, 0);
// ограничиваем, чтобы не превысило границ
val = constrain(val, 0, 8);
// зажечь кол-во светодиодов, пропорциональное освещенности,
// остальные потушить
for(int i=1;i<9;i++)
{
if(i>=val) // зажечь светодиоды
digitalWrite(leds[i-1],HIGH);
else // потушить светодиоды
digitalWrite(leds[i-1],LOW);
}
delay(100); // пауза перед следующим измерением
}


