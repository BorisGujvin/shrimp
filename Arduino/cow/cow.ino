#define BUZZER_PIN   9  // пин с пищалкой
#define PLAYER_COUNT 2   // количество игроков-ковбоев
// вместо перечисления всех пинов по-одному, мы объявляем пару
// списков: один с номерами пинов с кнопками, другой — со
// светодиодами. Списки также называют массивами (англ. array)
int buttonPins[PLAYER_COUNT] = {2, 3};
int ledPins[PLAYER_COUNT] = {6, 7};
 
void setup()
{
  pinMode(BUZZER_PIN, OUTPUT);
  for (int player = 0; player < PLAYER_COUNT; ++player) {
    // при помощи квадратных скобок получают значение в массиве
    // под указанным в них номером. Нумерация начинается с нуля
    pinMode(ledPins[player], OUTPUT);
    pinMode(buttonPins[player], INPUT_PULLUP);
  }
}
 
void loop()
{
  // даём сигнал «пли!», выждав случайное время от 2 до 7 сек
  delay(random(2000, 7000));
  tone(BUZZER_PIN, 3000, 250); // 3 килогерца, 250 миллисекунд
 
  for (int player = 0; ; player = (player+1) % PLAYER_COUNT) {
    // если игрок номер «player» нажал кнопку...
    if (!digitalRead(buttonPins[player])) {
      // ...включаем его светодиод и сигнал победы на 1 сек
      digitalWrite(ledPins[player], HIGH);
      tone(BUZZER_PIN, 1000, 1000);
      delay(1000);
      digitalWrite(ledPins[player], LOW);
      break; // Есть победитель! Выходим (англ. break) из цикла
    }
  }
}

