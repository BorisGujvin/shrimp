#define RED 11 // Присваиваем имя RED для пина 11
#define GREEN 12 // Присваиваем имя GREEN для пина 12
#define BLUE 13 // Присваиваем имя BLUE для пина 13
 
void setup()
{
  pinMode(RED, OUTPUT); // Используем Pin11 для вывода
  pinMode(GREEN, OUTPUT); // Используем Pin12 для вывода
  pinMode(BLUE, OUTPUT); // Используем Pin13 для вывода
}
 
void loop()
{
 
  digitalWrite(RED, HIGH); // Включаем красный свет
  digitalWrite(GREEN, LOW);
  digitalWrite(BLUE, LOW);
 
  delay(1000); // Устанавливаем паузу для эффекта
 
  digitalWrite(RED, LOW);
  digitalWrite(GREEN, HIGH); // Включаем зеленый свет
  digitalWrite(BLUE, LOW);
 
  delay(1000); // Устанавливаем паузу для эффекта
 
  digitalWrite(RED, LOW);
  digitalWrite(GREEN, LOW);
  digitalWrite(BLUE, HIGH); // Включаем синий свет
 
  delay(1000); // Устанавливаем паузу для эффекта
}
