void start_RTOS_tasks()

  {
    
    if (WIFI_enabled)
    {
      xTaskCreatePinnedToCore(v_Wifi_connection,
      "WIFI_CONNECTION",
      4096,
      NULL,
      2,  //Priority
      NULL,
      1);

      xTaskCreatePinnedToCore(v_send_data,
      "SEND_DATA",
      4096,
      NULL,
      2,  //Priority
      NULL,
      1);

    }
        
    
    // Start ready for deep sleep
    xTaskCreatePinnedToCore(vTestForDeepSleep,
      "SLEEP_TEST",
      4096,
      NULL,
      2,  //Priority
      NULL,
      1);

      // Start air quality, temperature, hunidity read task
      xTaskCreatePinnedToCore(vReadAirQuality,
      "AIR_QUALITY",
      4096,
      NULL,
      3,  //Priority
      NULL,
      1);

      // Start OLED task
      xTaskCreatePinnedToCore(v_updateTheDisplay,
      "OLED",
      8192,
      NULL,
      2,  //Priority
      NULL,
      1);    
    
    xTaskCreatePinnedToCore(v_ReadBatteryVoltage,
      "BATTERY",
      2048,
      NULL,
      2,  //Priority
      NULL,
      1);


  }