#include <LiquidCrystal.h>
#include <EEPROM.h>
#include <DS1302.h>
#include "date_builder.h"

const int kCePin   = 11;
const int kIoPin   = 12;
const int kSclkPin = 13;

// Create RTC & LCD
DS1302 rtc(kCePin, kIoPin, kSclkPin);
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

void setup() {
  // set default vars
  int target_mo = 25;
  int start_hr = 18;
  int end_hr = 8;
  int period_min = 1;
  int backlight = 100;
  // write default vars to EEPROM
  EEPROM.write(0,target_mo);
  EEPROM.write(1,start_hr);
  EEPROM.write(2,end_hr);
  EEPROM.write(3,period_min);
  EEPROM.write(4,backlight);
  // set current time
  setTime();
  lcd.begin(16, 2);
  lcd.print("  NEMSENSOR V1  ");
  lcd.setCursor(0,1);
  lcd.print(" SETUP COMPLETE ");
  delay(2500);
}

void loop() {
}
