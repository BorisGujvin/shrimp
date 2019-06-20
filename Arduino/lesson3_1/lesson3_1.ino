int led = 9;
int analogPin = 0;
int val = 1;
 
void setup() {
   // put your setup code here, to run once:
  pinMode(led, OUTPUT);
 }
  
 int potensiom ()
 {
 int v;
 v = analogRead(analogPin);
 v /= 80;
 return v+1;
 }
 
void loop()
 {
 if(val != potensiom())
{
  delay(100);
  val = potensiom();
 for (int i = 0; i < val; i++)
 {
  digitalWrite(led, HIGH); 
 delay(100);              
 digitalWrite(led, LOW);  
 delay(500); 
 }
}
}

