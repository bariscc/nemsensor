#include <DS1302.h>
#include "build_defs.h"

int analogPin = A0;      // nemsensor analog output
int current_mo = 0;      // variable to store the value read
int target_mo = 15;      // target moisture

const int kCePin   = 9;  // Chip Enable
const int kIoPin   = 10;  // Input/Output
const int kSclkPin = 11;  // Serial Clock

// Create a DS1302 object.
DS1302 rtc(kCePin, kIoPin, kSclkPin);

const unsigned char date[] =
{
    BUILD_YEAR_CH0, BUILD_YEAR_CH1, BUILD_YEAR_CH2, BUILD_YEAR_CH3,
    // '-',
    BUILD_MONTH_CH0, BUILD_MONTH_CH1,
    // '-',
    BUILD_DAY_CH0, BUILD_DAY_CH1,
    // 'T',
    BUILD_HOUR_CH0, BUILD_HOUR_CH1,
    // ':',
    BUILD_MIN_CH0, BUILD_MIN_CH1,
    // ':',
    BUILD_SEC_CH0, BUILD_SEC_CH1,
    // '\0'
};

void setTime() {
  // Initialize a new chip by turning off write protection and clearing the
  // clock halt flag. These methods needn't always be called. See the DS1302
  // datasheet for details.
  rtc.writeProtect(false);
  rtc.halt(false);

  // Make a new time object to set the date and time.
  // Sunday, September 22, 2013 at 01:38:50.
  // Time t(2013, 9, 22, 1, 38, 50, Time::kSunday);

  Time t(((date[0]-'0')*1000 + (date[1]-'0')*100 + (date[2]-'0')*10 + (date[3]-'0')),
         (date[4]-'0')*10 + (date[5]-'0'),
         (date[6]-'0')*10 + (date[7]-'0'),
         (date[8]-'0')*10 + (date[9]-'0'),
         (date[10]-'0')*10 + (date[11]-'0'),
         (date[12]-'0')*10 + (date[13]-'0'),
         Time::kSunday);
  
  // Set the time and date on the chip.
  rtc.time(t);
}

void printTime() {
  // Get the current time and date from the chip.
  Time t = rtc.time();

  // Format the time and date and insert into the temporary buffer.
  char buf[50];
  snprintf(buf, sizeof(buf), "%02d-%02d-%04d %02d:%02d:%02d",           
           t.date, t.mon, t.yr,
           t.hr, t.min, t.sec);

  // Print the formatted string to serial so we can see the time.
  Serial.println(buf);
}

void setup()
{
  pinMode(7, OUTPUT);    // relay digital output
  Serial.begin(9600);    // debug
  // setTime();          // comment before production
}

void loop()
{
  current_mo = analogRead(analogPin);
  current_mo = map(current_mo,1023,0,0,100);
  
  printTime();
  
  Serial.print("mevcut nem: %");
  Serial.println(current_mo);
  Serial.print("hedef nem: %");
  Serial.println(target_mo);
  
  if(current_mo > target_mo) {
    digitalWrite(7,HIGH);
    Serial.println("Sulama Kapalı");
  } else {
    digitalWrite(7,LOW);   
    Serial.println("Sulama Açık"); 
  }
  Serial.println("");
  delay(5000);
} 
