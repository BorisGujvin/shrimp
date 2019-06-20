int button = 2;
int led = 8;
void setup() {
pinMode(led, OUTPUT);
pinMode(button, INPUT);
}
void loop(){
if (digitalRead(button) == HIGH) {
digitalWrite(led, HIGH);
}
else {
digitalWrite(led, LOW);
}
}
