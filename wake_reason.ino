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
      start_RTOS_tasks();
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

      OLED_StartUpMessages();    // Puts the Hindle message and version number of the display
      start_RTOS_tasks();
      break;
  }

}

void vTestForDeepSleep(void *parameters) 
{
  unsigned long wake_up_time = millis();  // define the elapsedTime and set to the present time.
                                        // this will be used to measure how long the unot has been awake. 
  
  for(;;)  //for the rtos task
  {
  //Run a multi layer test to see if the main loop, should go around again, or deep sleep

  if ( 
        (
          (air_quality_acquired == false) ||   // check to see if these flags are all clear 
          (temp_hum_acquired    == false) ||   // and
          (display_updated      == false)      // that the time hasn't expired. 
        )                                      // if loop
        &&
        ( 
          millis() <= wake_up_time + MAX_WAKE_TIME 
        )
    )  
      vTaskDelay(5000 / portTICK_PERIOD_MS) ; // 5 seconds
else
    {                                        // If the above parameters failed, do a timed deep sleep.
     debugln("Entering deep sleep");
     esp_sleep_enable_timer_wakeup(SLEEP_TIME * 1000000);
     esp_deep_sleep_start();
    }

  } // end of the for loop
 
}