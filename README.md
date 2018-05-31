This was part of a Senior Project developed using Arduino IDE (for the Arduino Uno
 microcontroller) and Android Studio (in order to create an associated mobile app). 

The project consists of an embedded system which allowed for the automation of a fingerprint-
secure smart lock. It can either be controlled via a microcontroller (like the Arduino) or 
through Bluetooth from a signal sent via our SmartLock mobile application. 

The user must first enroll the desired fingerprints into memory, then (as will be indicated
 by the LCD) a servo motor will turn the deadbolt lock into the unlocked position for 5 
seconds, then lock it again. For convenience, the time and date will also be displayed on 
the door (which is powered internally by a CR-1220 battery commonly used in watches). 
The entire device, however, was designed to be powered by a wall outlet.

**Parts List:**

*Necessary*
* RioRand Spring SM-S4303R (RR-BR301) Servo Motor
* Fingerprint Recognition Module (Part #071405)
* Arduino Uno R3 (and USB or power cable)
* Breadboard/PCB
* Assorted Resistors or Potentiometers

*Optional*
* Facthuang 1602 LCD
* HM-10 BLE (Bluetooth Low Energy) Module
* CR 1220 Battery

**References/Datasheets**

1. https://cdn-shop.adafruit.com/datasheets/TC1602A-01T.pdf
2. https://www.arduino.cc/en/uploads/Main/arduino-uno-schematic.pdf
3. http://www.wecl.com.hk/distribution/catalogs/058-31-0328.pdf
4. http://www.atmel.com/images/Atmel-8271-8-bit-AVR- Microcontroller-ATmega48A-48PA-88A-88PA-168A-168PA-328-328P_datasheet_Complete.pdf
5. https://cdn-learn.adafruit.com/downloads/pdf/adafruit-optical-fingerprint-sensor.pdf

**Schematic:**

![Schematic](https://github.com/jasoncros/FingerprintSecureLock/sch_w18.png "Schematic")