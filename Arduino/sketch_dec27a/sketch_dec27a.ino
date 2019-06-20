#ifndef analogInputToDigitalPin
#define analogInputToDigitalPin(p) -1
#endif
#ifndef digitalPinHasPWM
#define digitalPinHasPWM(p) -1
#endif
void setup(){
  Serial.begin(9600); while(!Serial){;}
  Serial.println("IARDUINO.RU");
  Serial.println((String) "=======================================\r\nSPI BUS:\r\n\tCS:   " + SS + "\r\n\tMOSI: " + MOSI + "\r\n\tMISO: " + MISO + "\r\n\tSCK:  " + SCK);
  Serial.println((String) "---------------------------------------\r\nI2C BUS:\r\n\tSDA:  " + SDA + "\r\n\tSCL:  " + SCL);
  Serial.println((String) "---------------------------------------\r\nDIGITAL PINS:\r\n\tAll:  " + NUM_DIGITAL_PINS + " pins (D0-D" + (NUM_DIGITAL_PINS-1) + ")\r\n\tI/0:  " + (NUM_DIGITAL_PINS-NUM_ANALOG_INPUTS) + " pins (D0-D" + (NUM_DIGITAL_PINS-NUM_ANALOG_INPUTS-1) + ")");
  Serial.println((String) "---------------------------------------\r\nANALOG INPUTS:\r\n\tAll:  " + NUM_ANALOG_INPUTS + " pins (A0-A" + (NUM_ANALOG_INPUTS-1) + ")"); for(uint8_t i=0; i<NUM_DIGITAL_PINS; i++){if(analogInputToDigitalPin(i)>=0){Serial.println((String) "\tA" + i + (i<10?" ":"") + " = D" + analogInputToDigitalPin(i));}}
  Serial.print  (         "---------------------------------------\r\nPWM OUTPUTS:\r\n\tAll:  "); for(uint8_t i=0, j=0; i<NUM_DIGITAL_PINS; i++){if(digitalPinHasPWM(i)){j++;} if(i>=NUM_DIGITAL_PINS-1){Serial.println((String) j + " pins");}} for(uint8_t i=0; i<NUM_DIGITAL_PINS; i++){if(digitalPinHasPWM(i)){Serial.println((String) "\tD" + i);}}
  Serial.print  (         "---------------------------------------\r\nINTERRUPT INPUTS:\r\n\tAll:  "); for(uint8_t i=0, j=0; i<NUM_DIGITAL_PINS; i++){if(digitalPinToInterrupt(i)!=NOT_AN_INTERRUPT){j++;} if(i>=NUM_DIGITAL_PINS-1){Serial.println((String) j + " pins");}} for(uint8_t i=0; i<NUM_DIGITAL_PINS; i++){if(digitalPinToInterrupt(i)!=NOT_AN_INTERRUPT){Serial.println((String) "\tD" + i + (i<10?" ":"") + " = int_" + digitalPinToInterrupt(i));}}
}
void loop(){
}
