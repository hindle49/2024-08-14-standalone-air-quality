void start_RTOS_tasks()

  {
    
    // Start if deep sleep can start a deep sleep
    xTaskCreatePinnedToCore(vTestForDeepSleep,
      "SLEEP_TEST",
      2048,
      NULL,
      2,  //Priority
      NULL,
      1);

      // Start air quality, temperature, hunidity read task
      xTaskCreatePinnedToCore(vreadAirQuality,
      "AIR_QUALITY",
      2048,
      NULL,
      2,  //Priority
      NULL,
      1);

      // Start OLED task
      xTaskCreatePinnedToCore(v_updateTheDisplay,
      "OLED",
      2048,
      NULL,
      2,  //Priority
      NULL,
      1);     
    
    
    /*
    xTaskCreatePinnedToCore(v_read_buttons,
      "BUTTONS",
      2048,
      NULL,
      2,  //Priority
      NULL,
      1);
    */


  }