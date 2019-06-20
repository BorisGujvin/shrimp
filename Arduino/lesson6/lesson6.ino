
void setup() {
  //Initialize serial and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

}

void loop() {
  int i=analogRead(A7);
int j = map(i,100,800,40,5000);
tone(9,j);
Serial.print(i);
Serial.print("     ");
Serial.println(j);
}
