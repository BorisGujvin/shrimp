void setup() {
  for (int i=2; i<12;i++){pinMode(i, OUTPUT);}
  pinMode(12, INPUT );
  digitalWrite(11,HIGH);
  Serial.begin(9600);
}
int md = 60,
    ld = 300,
    current;
void(* resetFunc) (void) = 0;
void ReadS(int i){
  if(digitalRead(12)){
 //          Serial.print(i);
           if(i==6){
              for(int j=2; j<11;j++){
                 digitalWrite(j,HIGH);
              }
              tone(11,5000,100);
              delay(2000);
              for(int j=2; j<11;j++){
                 digitalWrite(j,LOW);
              }
                resetFunc();
           } else{
              for(int j=2; j<11;j++){
                 digitalWrite(j,LOW);
              }
              tone(11,200,200);
              delay(2000);
              resetFunc();
           }
  }
}
void loop() {
 delay(ld);
 for (int i=2; i<10;i++){
    digitalWrite(i,LOW);
    digitalWrite(i+1,HIGH);
    delay(md);
    ReadS(i+1);
 }
 delay(ld);
 for (int i=10; i>2;i--){
    digitalWrite(i,LOW);
    digitalWrite(i-1,HIGH);
    delay(md);
    ReadS(i-1);
 }
}
