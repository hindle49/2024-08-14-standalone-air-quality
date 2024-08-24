/*
Function to establish the reason by which ESP32
has been awaken from sleep
*/
void wakeup_reason()
 {
  esp_sleep_wakeup_cause_t wakeup_reason;

  wakeup_reason = esp_sleep_get_wakeup_cause();

  switch(wakeup_reason)
  {
    case ESP_SLEEP_WAKEUP_EXT0 : 
      debugln("Wakeup caused by external signal using RTC_IO"); 
      //ex0_wake_up();
      break;

    case ESP_SLEEP_WAKEUP_EXT1 : 
      debugln("Wakeup caused by external signal using RTC_CNTL");
      // Jump Somewhere
      break;

    case ESP_SLEEP_WAKEUP_TIMER : 
      debugln("Wakeup caused by timer");
      break;

    case ESP_SLEEP_WAKEUP_TOUCHPAD : 
      debugln("Wakeup caused by touchpad");
      // Jump Somewhere 
      break;
    
    case ESP_SLEEP_WAKEUP_ULP :
      debugln("Wakeup caused by ULP program");
      // Jump Somewhere 
      break;

    default : 
      debug("Wakeup was not caused by deep sleep: "); 
      debugln(wakeup_reason );
      break;
  }

}