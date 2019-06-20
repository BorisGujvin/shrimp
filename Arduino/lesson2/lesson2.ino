int button = 2;
int led = 8;
void setup() {
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(button, INPUT);
}
void loop(){
  if (digitalRead(button) == HIGH) {
    do{
      delay(25);}
    while (digitalRead(button) == HIGH);
    digitalWrite(led, LOW);
    led++;
    if(led==11)led=8;
    digitalWrite(led, HIGH);
  }
}

