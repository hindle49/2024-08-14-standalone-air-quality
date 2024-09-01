void start_RTOS_tasks()

  {
    
    // Start ready for deep sleep
    xTaskCreatePinnedToCore(vTestForDeepSleep,
      "SLEEP_TEST",
      2048,
      NULL,
      2,  //Priority
      NULL,
      1);

      // Start air quality, temperature, hunidity read task
      xTaskCreatePinnedToCore(vReadAirQuality,
      "AIR_QUALITY",
      2048,
      NULL,
      3,  //Priority
      NULL,
      1);


      /*
      // Start OLED task
      xTaskCreatePinnedToCore(v_updateTheDisplay,
      "OLED",
      2048,
      NULL,
      2,  //Priority
      NULL,
      1); 
      */    
    
    
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