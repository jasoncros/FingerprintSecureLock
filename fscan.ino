#include <Adafruit_Fingerprint.h>
#include <LiquidCrystal.h>
#include <SoftwareSerial.h>
#include <Servo.h>

int getFingerprintIDez();
// pin #1 is IN from sensor (GREEN wire)
// pin #0 is OUT from arduino (WHITE wire)
SoftwareSerial mySerial(0,1);

//const int rs=9,en=8,d4=7,d5=6,d6=5,d7=4;
//LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);
void setup()
{ Serial.begin(9600); // initialize the serial communications:
lcd.begin(16,2); lcd.setCursor(0,0); lcd.print("Scan your finger");
//pinMode(13,OUTPUT);
//pinMode(12,OUTPUT);
//pinMode(11, OUTPUT);
//pinMode(A0, INPUT);
finger.begin(57600); // set the data rate for the sensor serial port
}
void loop() // run over and over again
{
getFingerprintID();
delay(100);
digitalWrite (13,HIGH);
}
uint8_t getFingerprintID()
{ uint8_t p = finger.getImage();
switch (p)
{
case FINGERPRINT_OK:
lcd.clear();
lcd.print(" Image taken... ");
delay(1000);
break;
case FINGERPRINT_NOFINGER:
return p;
case FINGERPRINT_PACKETRECIEVEERR:
return p;
case FINGERPRINT_IMAGEFAIL:
return p;
default:
return p; }
// OK success!
p = finger.image2Tz();
switch (p) {
case FINGERPRINT_OK:
break;
case FINGERPRINT_IMAGEMESS:
return p;
case FINGERPRINT_PACKETRECIEVEERR:
return p;
case FINGERPRINT_FEATUREFAIL:
return p;
case FINGERPRINT_INVALIDIMAGE:
return p;
default:
return p; }
// OK converted!
p = finger.fingerFastSearch();
if (p == FINGERPRINT_OK)
{
lcd.clear();
lcd.print(" Found match! ");
//digitalWrite(11, HIGH);
delay(1000);
//digitalWrite(11,LOW); // turn on green LED to indicate match
}
else if(p == FINGERPRINT_NOTFOUND)
{
lcd.clear();
lcd.setCursor(0,0);
lcd.print(" Did not match! ");
delay(1000);
lcd.clear();
lcd.setCursor(0,0);
lcd.print(" scan finger! ");
return p;
}
else
{ return p; }
// IF FOUND A MATCH............
lcd.clear();
lcd.setCursor(0,0);
lcd.print("Found ID #");
lcd.print(finger.fingerID);
lcd.setCursor(0,1);
lcd.print("confidence ");
lcd.print(finger.confidence); }
// returns -1 if failed, otherwise returns ID #
int getFingerprintIDez() {
uint8_t p = finger.getImage();
if (p != FINGERPRINT_OK) return -1;
p = finger.image2Tz();
if (p != FINGERPRINT_OK) return -1;
p = finger.fingerFastSearch();
if (p != FINGERPRINT_OK) return -1;
// found a match!
digitalWrite(13, LOW);
delay(10);
digitalWrite(13, HIGH);
delay(10);
lcd.clear();
lcd.setCursor(0, 0);
lcd.print("Found ID # ");
lcd.print(finger.fingerID);
lcd.setCursor(0, 1);
lcd.print("confidence ");
lcd.print(finger.confidence);
return finger.fingerID;
}
