/*
This example will set output voltage to 5V
DC's TX pin must be connected to arduino's 12 pin (RX)
DC's RX pin must be connected to arduino's 13 pin (TX)
*/
#include "xy6020l.h"
#include <SoftwareSerial.h>
SoftwareSerial Serial1 =  SoftwareSerial(12, 13);
xy6020l xy(Serial1, 0);
void setup() {
 Serial.begin(115200);
 Serial1.begin(115200);
 xy.setOutput(true); 
}

void loop() {
  xy.task();
  if(xy.HRegUpdated()) {
   xy.setCV(500);
   int OutCC = xy.getActC();
   int OutV = xy.getActV();
   Serial.print("Out A: ");Serial.print(OutCC);
   Serial.print("; Out V: ");Serial.print(OutV);
   Serial.println();  
  }
}
