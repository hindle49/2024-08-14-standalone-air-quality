void startDS3231RTC() {

  
 /* if (!myRTC.begin()) {
    
    debugln("Couldn't find RTC Module");
    
    display.setFont(ArialMT_Plain_10);
    display.setTextAlignment(TEXT_ALIGN_CENTER);
    display.drawString(64, 25, "Couldn't find RTC");
    display.display();

    while (1)
      delay(10);
  }

  if (rtc.lostPower()) {
    
    display.setFont(ArialMT_Plain_10);
    display.setTextAlignment(TEXT_ALIGN_CENTER);
    display.drawString(64, 25, "Time has been reset");
    display.display();
    delay(3000);
    display.clear();
    display.display();
  } */

  //rtc.disable32K();
  //rtc.writeSqwPinMode(DS3231_SquareWave1Hz);  //
  //rtc.clearAlarm(1);
  //rtc.clearAlarm(2);
  //rtc.disableAlarm(1);
  //rtc.disableAlarm(2);
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