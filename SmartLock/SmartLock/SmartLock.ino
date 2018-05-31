//#include <SoftwareSerial.h>
#include <LiquidCrystal.h>
#include <Servo.h>

const int rs=4,en=5,d4=6,d5=7,d6=8,d7=9;
//SoftwareSerial mySerial(12,13); //rx=12, tx=13
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
char command;
Servo myServo;

void setup() {
  Serial.begin(9600);
  pinMode(11,OUTPUT);
  digitalWrite(11,HIGH);
  lcd.begin(16,2);
  lcd.setCursor(0,0);
  myServo.attach(10);
}

void loop() {
  lcd.clear();
  lcd.setCursor(0,0);
  command = Serial.read();
  if(command == 'u')
  {
    //unlock
    myServo.write(0);
    lcd.print("unlocked");
  }
  if(command == 'l')
  {
    //lock
    myServo.write(180);
    lcd.print("locked");
  }
  delay(1000);
}
