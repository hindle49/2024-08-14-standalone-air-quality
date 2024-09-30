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
      WIFI_enabled = !WIFI_enabled;  // Toggle the Wifi Status

      //Start the button task because the button has woken the system.
      xTaskCreatePinnedToCore(v_read_buttons,
      "BUTTONS",
      2048,
      NULL,
      2,  //Priority
      NULL,
      1);
      
      start_RTOS_tasks();
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
      
      digitalWrite(RED_LED, HIGH);  // flash the red led for 500ms
      delay(500);
      digitalWrite(RED_LED, LOW);
      delay(500);
      digitalWrite(RED_LED, HIGH); // flash the green led for 500ms
      delay(500);
      digitalWrite(RED_LED, LOW);
      delay(500);

      OLED_StartUpMessages();    // Puts the Hindle message and version number of the display

      // Now branch if the Wifi button was pressed at startup 

      if (request_new_WiFi)
          {
            // display WiFi Manager Logo message.
            OLED_WiFi_Manager_Messages();
            // Start the Wifi task
                xTaskCreatePinnedToCore(v_Wifi_connection,
                "WIFI_CONNECTION",
                4096,
                NULL,
                2,  //Priority
                NULL,
                1);
          }
      else
         {
         start_RTOS_tasks();
         }
      break;
  }

}

void vTestForDeepSleep(void *parameters) 
{
  unsigned long wake_up_time = millis();  // define the elapsedTime and set to the present time.
                                        // this will be used to measure how long the unot has been awake. 
  
  while(1)  //for the rtos task
  {
  //Run a multi layer test to see if the main loop, should go around again, or deep sleep

  if ( 
        (
          (air_quality_acquired == false) ||   // check to see if these flags are all clear 
          (temp_hum_acquired    == false) ||   // and
          (display_updated      == false) ||   // that the time hasn't expired. 
          (wifi_button_timeout  == false) ||
          ((WIFI_enabled == true) && (WiFi_Connected       == false) )   ||   // is the wifi enabled but not connected yet
          ((WIFI_enabled == true) && (data_sent_to_brocker == false) )        // add sent data flag.

        )                                      // if loop
        &&
        ( 
          millis() <= wake_up_time + MAX_WAKE_TIME 
        )
    ) 
    {
    vTaskDelay(5000 / portTICK_PERIOD_MS) ; // 5 seconds
    } 
   
else
    {                                        // If the above parameters failed, do a timed deep sleep.
     debugln("Entering deep sleep");
     esp_sleep_enable_timer_wakeup(SLEEP_TIME * 1000000);
     esp_deep_sleep_start();
    }

 

  } // end of the for loop
 
}