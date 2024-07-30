/*
This example will set 5V on DC's output
Warning: DC seems uses 5V TTL, don't connect it directly to ESP's GPIO. Use cheap level translator
DC's TX pin must be connected to ESP's GPIO-25 (RX)
DC's RX pin must be connected to ESP's GPIO-26 (TX)
*/
#include "xy6020l.h"
xy6020l xy(Serial1, 0);
void setup() {
 Serial.begin(115200);
 Serial1.begin(115200, SERIAL_8N1, 25, 26);
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
