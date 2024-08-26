void vmain_loop (void *parameters)

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
     esp_sleep_enable_timer_wakeup(SLEEP_TIME * 1000000);
     esp_deep_sleep_start();
    }

  } // end of the for loop
}