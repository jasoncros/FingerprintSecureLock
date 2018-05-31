/* This program uses an Arduino Uno to control a fingerprint-secure lock which: 
       -opens/closes with a servo motor
       -supports up to 162 different fingerprints,
       -displays fingerprint info on an LCD
   To be implemented in the future:
       -Time/Date displayed on LCD
       -Bluetooth control via Android app */
       
#include <Wire.h>
#include "RTClib.h"       
#include <Adafruit_Fingerprint.h>
#include <LiquidCrystal.h>
#include <SoftwareSerial.h>
#include <Servo.h>

// pin #2 is IN from sensor (GREEN wire)
// pin #3 is OUT from arduino (WHITE wire)
// servo: pos 0 = lock, pos 90 = unlock

//initialization
char command;
int getFingerprintIDez(); //not needed?
void updateServo();
const int rs=4,en=5,d4=6,d5=7,d6=8,d7=9;
int pos=0;

Servo myServo;
SoftwareSerial mySerial(2,3);
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
RTC_DS1307 rtc;

int inPin = 11;         // the number of the input pin
int outPin = 12;       // the number of the output pin

int state = HIGH;      // the current state of the output pin
int reading;           // the current reading from the input pin
int previous = LOW;    // the previous reading from the input pin

// the follow variables are long's because the time, measured in miliseconds,
// will quickly become a bigger number than can be stored in an int.
long time = 0;         // the last time the output pin was toggled
long debounce = 500;   // the debounce time, increase if the output flickers

void setup()
{ 
  Serial.begin(9600); // initialize the serial communications
  lcd.begin(16,2); lcd.setCursor(0,0); lcd.print("Scan your finger");
  finger.begin(57600); // set the data rate for the sensor serial port
  lcd.setCursor(0,1);
  setupRTC();
  updateRTC();
  myServo.attach(10);
  //pinMode(inPin, INPUT);
  //pinMode(outPin, OUTPUT);
}

void loop() 
{
  if(state == HIGH){
    readData();
  }
  else {
    getFingerprintID();
  }

  readData();
  getFingerprintID();
  delay(100);
}

uint8_t getFingerprintID()
{ 
  uint8_t p = finger.getImage();
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
      
// success...
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
    
// converted...
  p = finger.fingerFastSearch();
  if (p == FINGERPRINT_OK){
    lcd.clear();
    lcd.print(" Found match! ");
    delay(1000);
  }
  else if(p == FINGERPRINT_NOTFOUND){
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(" Did not match! ");
    delay(1000);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Scan finger! ");
    lcd.setCursor(0,1);
    updateRTC();
    return p;
  }
  else{ return p; }

// if match found
  lcd.clear();
  lcd.setCursor(0,0);
  switch (finger.fingerID){
    case 1:
    case 2:
      lcd.print("Welcome Jason!");
      lcd.setCursor(0,1);
      updateRTC();
      break;
    case 3:
    case 4:
      lcd.print("Welcome Juan!");
      lcd.setCursor(0,1);
      updateRTC();
      break;
    case 5:
    case 6:
      lcd.print("Welcome James!");
      lcd.setCursor(0,1);
      updateRTC();
      break;
    case 7:
    case 8:
      lcd.print("Welcome Edward!");
      lcd.setCursor(0,1);
      updateRTC();
      break;
    default:
      break;
  }
  updateServo();
}

// returns -1 if failed, otherwise returns ID #, not needed?
/*int getFingerprintIDez() {
  uint8_t p = finger.getImage();
  if (p != FINGERPRINT_OK) return -1;
  p = finger.image2Tz();
  if (p != FINGERPRINT_OK) return -1;
  p = finger.fingerFastSearch();
  if (p != FINGERPRINT_OK) return -1;
  // found a match!
  delay(10);
  delay(10);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Found ID # ");
  lcd.print(finger.fingerID);
  lcd.setCursor(0, 1);
  lcd.print("confidence ");
  lcd.print(finger.confidence);
  return finger.fingerID;
}*/

void updateServo(){
  /*turns servo to unlock when finger matches,
  then waits 5s for user to unlock door.
  if no match, skip this (if statement)*/
  myServo.attach(10);
  for(pos=0;pos<=120;pos++){
    myServo.write(pos);//unlock door
    delay(15);
  }
  delay(5000); //wait 5 seconds to open door
  //reset LCD, update time
  lcd.clear();
  lcd.print("Scan finger!");
  lcd.setCursor(0,1);
  updateRTC();
  
  for(pos=120;pos>=0;pos--){
    myServo.write(pos); //lock door again for security
    delay(15);
  }
  myServo.detach();
}

void setupRTC(){
  //GND for DS 1307
  pinMode(A2, OUTPUT);
  digitalWrite(A2, LOW);
  //power for DS 1307
  pinMode(A3, OUTPUT);
  digitalWrite(A3, HIGH);

  //for safety (if DS 1307 isn't working)
  if (! rtc.begin()) {
    lcd.print("Couldn't find RTC");
    while (1);
  }
  //sets the RTC to the date & time this sketch was compiled
  rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
}

void updateRTC(){
  DateTime now = rtc.now();

  lcd.print(now.month(), DEC);
  lcd.print("/");
  lcd.print(now.day(), DEC);
  lcd.print("/");
  lcd.print(now.year()%2000, DEC);
  lcd.print(" ");
  if(now.hour() > 12)
    lcd.print(now.hour()%12, DEC);
  else
    lcd.print(now.hour(), DEC);
  lcd.print(":");
  printDigits(now.minute());
  if(now.hour() > 12)
    lcd.print("PM");
  else
    lcd.print("AM");
}

void printDigits(byte digits){
  // utility function for digital clock display: prints colon and leading 0
  if(digits < 10)
    lcd.print('0');
  lcd.print(digits, DEC);   
}

void readData(){
  command = Serial.read();
  if(command == 'u')
  {
    //unlock
    lcd.clear();
    lcd.print("unlocked");
    myServo.write(0);
    delay(500);
  }
  if(command == 'l')
  {
    //lock
    lcd.clear();
    lcd.print("locked");
    myServo.write(180);
    delay(500);
  }
}

void buttonToSwitch(){
  reading = digitalRead(inPin);

  // if the input just went from LOW and HIGH and we've waited long enough
  // to ignore any noise on the circuit, toggle the output pin and remember
  // the time
  if (reading == HIGH && previous == LOW && millis() - time > debounce) {
    if (state == HIGH)
      state = LOW;
    else
      state = HIGH;

    time = millis();    
  }

  digitalWrite(outPin, state);

  previous = reading;
}

