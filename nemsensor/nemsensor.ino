#include <LiquidCrystal.h>
#include <EEPROM.h>
#include <DS1302.h>

void updateTime(int force = 0);
void updateStatus(int force = 0);
void updateCurrentMo(int force = 0);
void updateTargetMo(int force = 0);
void updateStartTime(int force = 0);
void updateEndTime(int force = 0);
void updatePeriodTime(int force = 0);
void checkMo(int force = 0);

// keypad
#define right   0
#define up      1
#define down    2
#define left    3
#define select  4
#define none    5

// pins
const int sensorPin = A5;
const int relayPin = 3;
const int kCePin   = 11;
const int kIoPin   = 12;
const int kSclkPin = 13;
const int lcdBacklight = 10;

// user vars
int target_mo = EEPROM.read(0);
int start_hr = EEPROM.read(1);
int end_hr = EEPROM.read(2);
int period_min = EEPROM.read(3);
int backlight = EEPROM.read(4);

int last_target_mo;
int last_start_hr;
int last_end_hr;
int last_period_min;
int prev_backlight;

// vars
unsigned long currentMillis = 0;
boolean lights = true;

const int statusInterval = 1000;
unsigned long previousStatusMillis = 0;
const int currentMoInterval = 1000;
unsigned long previousCurrentMoMillis = 0;
const int clockInterval = 1000;
unsigned long previousClockMillis = 0;
const int varInterval = 250;
unsigned long previousVarMillis = 0;

unsigned long sensorInterval = period_min * 60000;
unsigned long previousSensorMillis = 0;

const int lightInterval = 30000;
unsigned long previousLightMillis = 0;

int last_min;
int last_hr;
int current_mo;
int last_current_mo;
int op_status;
int last_op_status;
int menu_index = 1;
int last_menu_index;

// Create RTC & LCD
DS1302 rtc(kCePin, kIoPin, kSclkPin);
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

void setup()
{
  pinMode(relayPin, OUTPUT);
  pinMode(lcdBacklight, OUTPUT);
  digitalWrite(relayPin,HIGH);
  analogWrite(lcdBacklight, backlight);
  //Serial.begin(9600);
  lcd.begin(16, 2);
  checkMo(1);
}

void welcome() {
  lcd.print("  NEMSENSOR V1  ");
  lcd.setCursor(0,1);
  for(int i = 0; i < 16; i++) {
    lcd.print(".");
    delay(100);    
  }  
  delay(400);  
}

void loop()
{
  currentMillis = millis();
  menu();
  checkMo();
  lightsOff();
}

int btn_state;
int last_btn_state = 5;
unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 50;

byte findBtn()
{
  int btn_value = analogRead(A0);
  if (btn_value > 1000) return none;
  if (btn_value < 50) return right;
  if (btn_value < 195) return up;
  if (btn_value < 380) return down;
  if (btn_value < 555) return left;
  if (btn_value < 790) return select;
}

void menu()
{
  int reading = findBtn();
  if (reading != last_btn_state)
  {
    lastDebounceTime = millis();
  }
  if ((millis() - lastDebounceTime) > debounceDelay)
  {
    if (reading != btn_state)
    {
      btn_state = reading;
      if(!lights) {
        lightsOn();
        return;
      }
      switch(btn_state)
      {
        case select:
          menu_index = menu_index + 1;
          if(menu_index > 5) menu_index = 1;
          break; 
        case left:
          if(menu_index == 1) {
            backlight -= 25;
            setLight();
          }
          if(menu_index == 2) {
            if(target_mo != 5) target_mo -= 5;
          }
          if(menu_index == 3) {
            if (start_hr == 0) start_hr = 23;
            else start_hr -=1;
          }
          if(menu_index == 4) {
            if (end_hr == 0) end_hr = 23;
            else end_hr -= 1;
          }
          if(menu_index == 5) {
            if (period_min == 5 || period_min == 1) period_min = 1;
            else period_min -= 5;
            updateSensorInterval();
          }
          break;
        case right:
          if(menu_index == 1) {
            backlight += 25;
            setLight();
          }
          if(menu_index == 2) {
            if(target_mo != 100) target_mo += 5;
          }
          if(menu_index == 3) {
            if (start_hr == 23) start_hr = 0;
            else start_hr += 1;
          }
          if(menu_index == 4) {
            if (end_hr == 23) end_hr = 0;
            else end_hr += 1;
          }
          if(menu_index == 5) {
            if (period_min == 60) period_min = 60;
            else if (period_min == 1) period_min = 5;
            else period_min += 5;
            updateSensorInterval();
          }
          break;
        case down:
          if(menu_index == 1) { EEPROM.write(4,backlight); flashIcon(); updateTime(1); }
          if(menu_index == 2) { EEPROM.write(0,target_mo); flashIcon(); }
          if(menu_index == 3) { EEPROM.write(1,start_hr); flashIcon(); }
          if(menu_index == 4) { EEPROM.write(2,end_hr); flashIcon(); }
          if(menu_index == 5) { EEPROM.write(3,period_min); flashIcon(); }
          break;
        case up:
          if (menu_index == 1)
          {
            checkMo(1);
          }
          break;
      }
      if(btn_state == 5)
      {
        previousLightMillis = currentMillis;
      }
    }
  }
  last_btn_state = reading;
  displayMenu();
}

void displayMenu()
{
  if(menu_index != last_menu_index)
  {
    last_menu_index = menu_index;
    printStatic(menu_index);
  }
  switch(menu_index)
  {
    case 1:
      updateTime();
      updateCurrentMo();
      updateStatus();
      break;
    case 2:
      updateTargetMo();
      break;
    case 3:
      updateStartTime();
      break;
    case 4:
      updateEndTime();
      break;
    case 5:
      updatePeriodTime();
      break;
  }
}

void printStatic(int menu_i)
{
  lcd.clear();
  switch(menu_i)
  {
    case 1:
      printHome();
      break;
    case 2:
      printTargetMo();
      break;
    case 3:
      printStartTime();
      break;
    case 4:
      printEndTime();
      break;
    case 5:
      printPeriodTime();
      break;
  }
}

void printHome()
{
  lcd.setCursor(0,0);
  lcd.setCursor(0,1);
  lcd.print("Nem:");
  updateTime(1);
  updateCurrentMo(1);
  updateStatus(1);
  Serial.println("printHome()");
}

void updateTime(int force)
{
  if (currentMillis - previousClockMillis >= clockInterval || force)
  {
    Time t = rtc.time();
    int current_min = t.min;
    if(last_min != current_min || force)
    {
      char buf[50];
      snprintf(buf, sizeof(buf), "%02d.%02d.%04d %02d:%02d:%02d",
              t.date, t.mon, t.yr,
              t.hr, t.min, t.sec);
      lcd.setCursor(0,0);
      lcd.print(buf);
      last_min = current_min;
      previousClockMillis = currentMillis;
      Serial.println("updateTime()");
    }
  }
}

void updateCurrentMo(int force)
{
  if (currentMillis - previousCurrentMoMillis >= currentMoInterval || force)
  {
    if(last_current_mo != current_mo || force)
    {
      lcd.setCursor(4,1);
      lcd.print("    ");
      lcd.setCursor(4,1);
      lcd.print(current_mo);
      lcd.print("%");
      last_current_mo = current_mo;
      previousCurrentMoMillis = currentMillis;
      Serial.println("updateCurrentMo()");
    }
  }
}

void updateStatus(int force)
{
  if (currentMillis - previousStatusMillis >= statusInterval || force)
  {
    if (op_status != last_op_status || force) {
      if (op_status != last_op_status) lightsOn();
      lcd.setCursor(13,1);
      if (op_status == 1) lcd.print("---");
      else if (op_status == 2) lcd.print("-/-");
      else if (op_status == 3) lcd.print("zZz");
      else lcd.print("...");
      last_op_status = op_status;
      previousStatusMillis = currentMillis;
      Serial.println("updateStatus()");
    }    
  }
}

void printTargetMo()
{
  lcd.setCursor(0,0);
  lcd.print("   Hedef Nem:   ");
  lcd.setCursor(0,1);
  lcd.print("-");
  lcd.setCursor(15,1);
  lcd.print("+");
  updateTargetMo(1);
  Serial.println("printTargetMo()");
}

void updateTargetMo(int force)
{
  if (currentMillis - previousVarMillis >= varInterval || force)
  {
    if(last_target_mo != target_mo || force)
    {
      lcd.setCursor(6,1);
      lcd.print("%"); 
      lcd.print("   ");
      lcd.setCursor(7,1);
      lcd.print(target_mo);
      last_target_mo = target_mo;
      previousVarMillis = currentMillis;
      Serial.println("updateTargetMo()");
    }
  }
}

void printStartTime()
{
  lcd.setCursor(0,0);
  lcd.print("   Baslangic:   ");
  lcd.setCursor(0,1);
  lcd.print("-");
  lcd.setCursor(15,1);
  lcd.print("+");
  updateStartTime(1);
  lcd.setCursor(7,1);
  lcd.print(":00");
  Serial.println("printStartTime()");
}

void updateStartTime(int force)
{
  if (currentMillis - previousVarMillis >= varInterval || force)
  {
    if(last_start_hr != start_hr || force)
    {
      lcd.setCursor(5,1);
      if(start_hr < 10) { lcd.print("0"); }
      lcd.print(start_hr);
      last_start_hr = start_hr;
      previousVarMillis = currentMillis;
      Serial.println("updateStartTime()");
    }
  }
}

void printEndTime()
{
  lcd.setCursor(0,0);
  lcd.print("     Bitis:     ");
  lcd.setCursor(0,1);
  lcd.print("-");
  lcd.setCursor(15,1);
  lcd.print("+");
  updateEndTime(1);
  lcd.setCursor(7,1);
  lcd.print(":00");
  Serial.println("printEndTime()");
}

void updateEndTime(int force)
{
  if (currentMillis - previousVarMillis >= varInterval || force)
  {
    if(last_end_hr != end_hr || force)
    {
      lcd.setCursor(5,1);
      if(end_hr < 10) { lcd.print("0"); }
      lcd.print(end_hr);
      last_end_hr = end_hr;
      previousVarMillis = currentMillis;
      Serial.println("updateEndTime()");
    }
  }
}

void printPeriodTime()
{
  lcd.setCursor(0,0);
  lcd.print("Sensor Periyodu:");
  lcd.setCursor(0,1);
  lcd.print("-");
  lcd.setCursor(15,1);
  lcd.print("+");
  updatePeriodTime(1);
  lcd.setCursor(8,1);
  lcd.print("dk");
  Serial.println("printPeriodTime()");
}

void updatePeriodTime(int force)
{
  if (currentMillis - previousVarMillis >= varInterval || force)
  {
    if(last_period_min != period_min || force)
    {
      lcd.setCursor(5,1);
      if(period_min < 10) { lcd.print(" "); }
      lcd.print(period_min);
      last_period_min = period_min;
      previousVarMillis = currentMillis;
      Serial.println("updatePeriodTime()");
    }
  }
}

void checkMo(int force)
{
  if (currentMillis - previousSensorMillis >= sensorInterval || force)
  {
    boolean is_time = isTime();
    //if(is_time || force)
    //{
      current_mo = analogRead(sensorPin);
      current_mo = map(current_mo,1023,0,0,100);
      if(is_time && current_mo < target_mo)
      {
        setRelay(true);
        op_status = 1;
      }
      else if (is_time)
      {
        setRelay(false);
        op_status = 2;
      } else {
        setRelay(false);
        op_status = 3;
      }
    //}
    previousSensorMillis = currentMillis;
    Serial.println("checkMo()");
  }
}

void setRelay(boolean low)
{
  if(low) digitalWrite(relayPin,LOW);
  else digitalWrite(relayPin,HIGH);
  Serial.println("setRelay()");
}

boolean isTime()
{
  Serial.println("isTime()");
  int current_hr = rtc.time().hr;
  if(start_hr < end_hr)
  {
    if(current_hr >= start_hr && current_hr < end_hr) return true;
    return false;
  }
  else
  {
    if(current_hr < end_hr && current_hr <= start_hr) return true;
    return false;
  }
}

void setLight()
{
    if(backlight > 255) backlight = 255;
    if(backlight < 0) backlight = 0;
    analogWrite(lcdBacklight, backlight);
    previousLightMillis = currentMillis;
    Serial.println("setLight()");
}

void lightsOff()
{
  if(currentMillis - previousLightMillis >= lightInterval && lights)
  {
    if(backlight>10) analogWrite(lcdBacklight, 1);    
    previousLightMillis = currentMillis;
    lights = false;
    menu_index = 1;
    Serial.println("lightsOff()");
  }
}

void lightsOn()
{
  lights = true;
  analogWrite(lcdBacklight, backlight);
  Serial.println("lightsOn()");
}

void updateSensorInterval()
{
  sensorInterval = period_min * 60000;
  Serial.println("updateSensorInterval()");
}

byte saveIcon[8] = {
  B00100,
  B00100,
  B10101,
  B01110,
  B00100,
  B10001,
  B11111,
};
byte brightnessIcon[8] = {
  B00000,
  B01010,
  B00000,
  B10101,
  B00000,
  B01010,
  B00000,
};

void flashIcon()
{
  lcd.createChar(0, saveIcon);
  lcd.createChar(1, brightnessIcon);
  for (int i=0; i<3; i++ )
  {
    if(menu_index == 1) {
      lcd.setCursor(11,0);
      lcd.print("   ");
      lcd.write(byte(1));
      lcd.write(byte(0));
      delay(300);
      lcd.setCursor(11,0);
      lcd.print("     ");
      delay(150);
    } else {
      lcd.setCursor(15,0);
      lcd.write(byte(0));
      delay(300);
      lcd.setCursor(15,0);
      lcd.print(" ");
      delay(150);
    }
  } 
  printStatic(menu_index);
}
