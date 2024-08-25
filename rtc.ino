void startDS3231RTC() {

  myRTC.setClockMode(false);  // set to 24h

}

void getTimeFromRTC()
  {
  //DateTime now = myRTC.now();
  
  tm_hour        = myRTC.getHour(h12Flag, pmFlag);   // what is the PM flag
  tm_min         = myRTC.getMinute();
  tm_sec         = myRTC.getSecond();
  tm_temperature = myRTC.getTemperature();
  
  }