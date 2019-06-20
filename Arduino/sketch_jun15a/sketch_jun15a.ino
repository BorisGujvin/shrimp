#define RED 9 // Присваиваем имя RED для пина 11
#define GREEN 11 // Присваиваем имя GREEN для пина 12
#define BLUE 10 // Присваиваем имя BLUE для пина 13
 
void setup()
{
  pinMode(RED, OUTPUT); // Используем Pin11 для вывода
  pinMode(GREEN, OUTPUT); // Используем Pin12 для вывода
  pinMode(BLUE, OUTPUT); // Используем Pin13 для вывода
}
 
void loop()
{
 digitalWrite(RED, LOW); // Включаем красный свет
  digitalWrite(GREEN, LOW);
  digitalWrite(BLUE, LOW);
 
  delay(1000); // Устанавливаем паузу для эффекта
  
  digitalWrite(GREEN, LOW);
  digitalWrite(BLUE, LOW);
 for (int i=0;i<100;i++){analogWrite(RED, i);delay(20);} // Включаем красный свет
 
  delay(1000); // Устанавливаем паузу для эффекта
 
  digitalWrite(RED, LOW);
  digitalWrite(RED, LOW);
for (int i=0;i<100;i++){analogWrite(GREEN, i);delay(20);} // Включаем красный свет

 
  delay(1000); // Устанавливаем паузу для эффекта
 
  digitalWrite(RED, LOW);
  digitalWrite(GREEN, LOW);
 for (int i=0;i<100;i++){analogWrite(BLUE, i);delay(20);} // Включаем красный свет

  delay(1000); // Устанавливаем паузу для эффекта
}

