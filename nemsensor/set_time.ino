const unsigned char date[] =
{
    BUILD_YEAR_CH0, BUILD_YEAR_CH1, BUILD_YEAR_CH2, BUILD_YEAR_CH3,
    BUILD_MONTH_CH0, BUILD_MONTH_CH1,
    BUILD_DAY_CH0, BUILD_DAY_CH1,
    BUILD_HOUR_CH0, BUILD_HOUR_CH1,
    BUILD_MIN_CH0, BUILD_MIN_CH1,
    BUILD_SEC_CH0, BUILD_SEC_CH1
};

Time::Day dayofweek(int d, int m, int y)
{
    static int t[] = { 0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4 };
    y -= m < 3;
    int dow = ( y + y/4 - y/100 + y/400 + t[m-1] + d) % 7;
    
    switch(dow) 
    {
      case 0: return Time::kSunday;
      case 1: return Time::kMonday;
      case 2: return Time::kTuesday;
      case 3: return Time::kWednesday;
      case 4: return Time::kThursday;
      case 5: return Time::kFriday;
      case 6: return Time::kSaturday;
    }
}

void setTime() 
{
  // Initialize a new chip by turning off write protection and clearing the
  // clock halt flag. These methods needn't always be called. See the DS1302
  // datasheet for details.
  rtc.writeProtect(false);
  rtc.halt(false);

  // Make a new time object to set the date and time.
  // Sunday, September 22, 2013 at 01:38:50.
  // Time t(2013, 9, 22, 1, 38, 50, Time::kSunday);
  int d = (date[6]-'0')*10 + (date[7]-'0');
  int m = (date[4]-'0')*10 + (date[5]-'0');
  int y = ((date[0]-'0')*1000 + (date[1]-'0')*100 + (date[2]-'0')*10 + (date[3]-'0'));
  int h = (date[8]-'0')*10 + (date[9]-'0');
  int min = (date[10]-'0')*10 + (date[11]-'0');
  int sec = (date[12]-'0')*10 + (date[13]-'0');
  Time t(y,m, d, h, min, sec, dayofweek(d, m, y));
  
  // Set the time and date on the chip.
  rtc.time(t);
}

String dayAsString(const Time::Day day) 
{
  switch (day) 
  {
    case Time::kSunday: return "Pazar";
    case Time::kMonday: return "Pazartesi";
    case Time::kTuesday: return "Salı";
    case Time::kWednesday: return "Çarşamba";
    case Time::kThursday: return "Perşembe";
    case Time::kFriday: return "Cuma";
    case Time::kSaturday: return "Cumartesi";
  }
  return "(unknown day)";
}
