int echoPin = 2; 
int trigPin = 3; 
 
void setup() { 
    Serial.begin (57600); 
    pinMode(trigPin, OUTPUT); 
    pinMode(echoPin, INPUT); 
} 
 
void loop() { 
    int duration, cm; 
    digitalWrite(trigPin, LOW); 
    delayMicroseconds(2); 
    digitalWrite(trigPin, HIGH); 
    delayMicroseconds(10); 
    digitalWrite(trigPin, LOW); 
    duration = pulseIn(echoPin, HIGH); 
    cm = duration / 58;
    if (cm>0){
    Serial.print(cm); 
    Serial.println(" cm"); }
    delay(100);
}
