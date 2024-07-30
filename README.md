# xy6020l Library
Originally forked from https://github.com/Jens3382/xy6020l

This library implementing XY6020L DC-DC converted board using Arduino or ESP8266\ESP32
# Simple usage on Arduino, which is setting 5V on output
```C
#include "xy6020l.h"

// We are using softwareserial arduino's lib to use custom UART pins
// On ESP you should comment next 2 lines
#include <SoftwareSerial.h>
SoftwareSerial Serial1 =  SoftwareSerial(12, 13);
xy6020l xy(Serial1, 0);
void setup() {
 Serial.begin(115200);
 Serial1.begin(115200); // On ESP you should comment this line
 // Serial1.begin(115200, SERIAL_8N1, 12, 13); // On ESP you should uncomment this line
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
```
## Description of code
```SoftwareSerial Serial1 =  SoftwareSerial(12, 13);``` - set PINs 12 and 13 as RX and TX. Please keep in mind, works on Arduinos AVR

```xy6020l xy(Serial1, 0);``` - Creating modbus instance on Serial1. Instead of Serial1 you can use and serial object, available on your board. Notice: on ESPs, Serial usually using as main UART.

```Serial1.begin(115200, SERIAL_8N1, 12, 13);``` - if you want use custom RX and TX on ESP, you should also mention parity and stop bits.

``` xy.task();``` - this command is builtin and must be used in loop, to control xy6020l's state and parameters. But you can experiment without it.

``` if(xy.HRegUpdated()) {``` - we can start work with xy6020l only when it's ready.

``` xy.setCV(500);``` - set 5V voltage. Please remember, these value is using divider "100", so if we want set there 7.5V - we should use 750, if we want 13.5V - we should use 1350, and so on.

```int OutCC = xy.getActC();``` - reading current A (amper) on output. If there are nothing connected to output, this value should be 0. There is also divider 100, so to understand real amperage in A, this value should be divided by 100.

```int OutV = xy.getActV();``` - reading current V (volt) on output.

# Functions of library (assume we created instance named xy)
```int V = xy.getInV();``` - reading voltage on input pins. Will be 0 if no power, 1950 when 19.5V and so on.
 This value also will be shown if you will switch on xy6020l with hardware button.

```int V = xy.getActV();``` - reading voltage on output pins. It reads actual voltage on pins. This means if you will setup xy6020l to 14V, connect 12V battery to output - value will be 1400 (like configured). But if you disconnect input voltage - value will 1200 (or so), because battery still stay connected and have own voltage.

```int I = xy.getActC();``` - reading actual current on output. Should be 0 if nothing connected. You can determine power usage in Watts, if do ```int P =  xy.getActV()*xy.getActA();```

```int T = xy.getTemp();``` - reading actual xy6020l's temperature.

```xy.setOutput(bool);``` - activate or deactivate xy6020l output. It's equal to hardware switch.

```bool W = xy.getOutputOn();``` - return true if xy6020l is switched on, or false if off.

```xy.setPreset(int preset);``` - activating "preset" number. What is - we will see later.

```int preset = xy.getPreset();``` - returning num of preset using at current time.

```xy.SetMemory(tMemory& mem);``` - saving preset in memory.

```xy.GetMemory(tMemory* pMem);``` - reading values of preset to pMem pointer.

```xy.PrintMemory(tMemory& mem);``` - print memory values to serial console.

# What are presets and how to work with memory?

XY6020L DC converter have some ready presets in memory. These presets are designed to set XY6020L working mode, to automate some popular behavior.

Let's imagine you want build power source, with protection when overcurrent. Usually you can implement these checks in code - reading actual CC, compare it to threshold, deactivate DC if overload, etc.

But these actions also could be done automatically by XY6020L engine, you should just set some values.

Now preset example with description:

```C
    Mem.Nr = 2; // Number of preset
    Mem.VSet = 1200; // Output voltage (12V)
    Mem.ISet =  500; // Output max current (5A)
    Mem.sLVP = 1000; // Low voltage threshold (10V)
    Mem.sOVP = 1300; // High voltage threshold (13V)
    Mem.sOCP =  620; // High current threshold (6.2A)
    Mem.sOPP = 1040; // High power threshold (104 watts)
    Mem.sOHPh= 0; // How many hours output will work
    Mem.sOHPm= 0; // How many minutes output will work
    Mem.sOAH = 0; 
    Mem.sOWH = 0;
    Mem.sOTP = 110; // High temperature threshold
    Mem.sINI = 0;
```
This example means, so XY6020L will set 12V 5A output, which will be switched off if voltage will be lower than 10V, higher than 13V, current higher than 6.2A and temperature higher than 110c.

Code which activate this preset:

```C
#include "xy6020l.h"

// We are using softwareserial arduino's lib to use custom UART pins
// On ESP you should comment next 2 lines
#include <SoftwareSerial.h>
SoftwareSerial Serial1 =  SoftwareSerial(12, 13);
xy6020l xy(Serial1, 0);
void setup() {
 Serial.begin(115200);
 Serial1.begin(115200); // On ESP you should comment this line
 // Serial1.begin(115200, SERIAL_8N1, 12, 13); // On ESP you should uncomment this line
 xy.setPreset(2);
 xy.setOutput(true); 
}

void loop() {
  xy.task();
  if(xy.HRegUpdated()) {
   xy.setPreset(2);
   int OutCC = xy.getActC();
   int OutV = xy.getActV();
   Serial.print("Out A: ");Serial.print(OutCC);
   Serial.print("; Out V: ");Serial.print(OutV);
   Serial.println();  
  }
}
```
# Can I configure my own preset?
Yes, you can

```C
#include "xy6020l.h"

// We are using softwareserial arduino's lib to use custom UART pins
// On ESP you should comment next 2 lines
#include <SoftwareSerial.h>
SoftwareSerial Serial1 =  SoftwareSerial(12, 13);
xy6020l xy(Serial1, 0);
tMemory Mem;
void setup() {
 Serial.begin(115200);
 Serial1.begin(115200); // On ESP you should comment this line
 // Serial1.begin(115200, SERIAL_8N1, 12, 13); // On ESP you should uncomment this line
        Mem.Nr = 0; // Keep in mind, we are saving preset with this number, and we must use this number in future
        Mem.VSet =  410;
        Mem.ISet =   50;
        Mem.sLVP = 1000;
        Mem.sOVP =  500;
        Mem.sOCP =  100;
        Mem.sOPP =   40;
        Mem.sOHPh= 0;
        Mem.sOHPm= 0;
        Mem.sOAH = 0;
        Mem.sOWH = 0;
        Mem.sOTP = 110;
        Mem.sINI = 0;
        xy.SetMemory(Mem);
        xy.PrintMemory(Mem);
 xy.setPreset(0); // We used prevoulsly created preset 0
 xy.setOutput(true); 
}

void loop() {
  xy.task();
  if(xy.HRegUpdated()) {
   xy.setPreset(0); // We used prevoulsly created preset 0
   int OutCC = xy.getActC();
   int OutV = xy.getActV();
   Serial.print("Out A: ");Serial.print(OutCC);
   Serial.print("; Out V: ");Serial.print(OutV);
   Serial.println();  
  }
}
```
# Known problems
1. Sometimes device doesn't accept new commands and UART must be re-connected.
2. ESP8266 and ESP32 have 3.3V TTL, but XY6020L have 5V levels, so don't connect pins directly. Instead, use level converter, https://learn.sparkfun.com/tutorials/retired---using-the-logic-level-converter
3. ESP32 shields with MicroUSB and AMS1117 converter, may have not enough 5V power to start XY6020L's UART - you should use external 5V power source, or use shields with type-C.


Main author: Jens Gleissberg

This fork author: Vitaliy Blats

Date: 2024

License: GNU Lesser General Public License v3.0 or later
