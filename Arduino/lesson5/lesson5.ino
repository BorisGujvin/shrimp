void setup() {
pinMode(4, OUTPUT);
pinMode(5, OUTPUT);
pinMode(2, OUTPUT);
pinMode(3, OUTPUT);
pinMode(6, OUTPUT);
pinMode(7, OUTPUT);
pinMode(8, OUTPUT);
pinMode(9, OUTPUT);
}
int md = 30,
    ld = 500;
void loop() {
 delay(ld);
 digitalWrite(2,LOW);
 digitalWrite(3,HIGH);
 delay(md);
 digitalWrite(3,LOW);
 digitalWrite(4,HIGH);
 delay(md);
 digitalWrite(4,LOW);
 digitalWrite(5,HIGH);
 delay(md);
 digitalWrite(5,LOW);
 digitalWrite(6,HIGH);
 delay(md);
 digitalWrite(6,LOW);
 digitalWrite(7,HIGH);
 delay(md);
 digitalWrite(7,LOW);
 digitalWrite(8,HIGH);
 delay(md);
 digitalWrite(8,LOW);
 digitalWrite(9,HIGH);
 delay(ld);
 digitalWrite(9,LOW);
 digitalWrite(8,HIGH);
 delay(md);
 digitalWrite(8,LOW);
 digitalWrite(7,HIGH);
 delay(md);
 digitalWrite(7,LOW);
 digitalWrite(6,HIGH);
 delay(md);
 digitalWrite(6,LOW);
 digitalWrite(5,HIGH);
 delay(md);
 digitalWrite(5,LOW);
 digitalWrite(4,HIGH);
 delay(md);
 digitalWrite(4,LOW);
 digitalWrite(3,HIGH);
 delay(md);
 digitalWrite(3,LOW);
 digitalWrite(2,HIGH);

}
