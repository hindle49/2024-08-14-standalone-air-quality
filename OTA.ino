void run_OTA_start() 
{
  OTA_Request = true;   // set flag to show system is doing an OTA update
  // This is the task for running the OTA functions
  xTaskCreatePinnedToCore(vOTA, "OTA", 8192,NULL,
                          3,    //Priority
                          NULL,
                          1);  //Core


  // This is the task for full wifi
   xTaskCreatePinnedToCore(v_wifi_connection, "FULL_WIFI", 2048, NULL,
                           2,  //Priority
                           NULL,
                           1); //Core

// This is the task for getting a new unix time
   xTaskCreatePinnedToCore(vget_new_unix_time, "GET_UNIX_TIME", 2048, NULL,
                           2,  //Priority
                           NULL,
                           1); //Core


}


void vOTA(void *parameters)
{
   bool OTA_Setup = false;  
   while(1)
    {
      if ( !OTA_Setup )  // first run the OTA  setup
         { 
         OTA_Setup = true;
   
         while (WiFi_Connected == false) 
            { 
              vTaskDelay(3000 / portTICK_PERIOD_MS);  // wait 3 s abd try again
            }   // loop until connected
         

          // Port defaults to 3232
          // ArduinoOTA.setPort(3232);
          // Hostname defaults to esp3232-[MAC]
          ArduinoOTA.setHostname("Air_Quality");
          // No authentication by default
          // ArduinoOTA.setPassword("admin");
          // Password can be set with it's md5 value as well
          // MD5(admin) = 21232f297a57a5a743894a0e4a801fc3
          // ArduinoOTA.setPasswordHash("21232f297a57a5a743894a0e4a801fc3");

         ArduinoOTA
        .onStart([]() 
           {
           String type;
           if (ArduinoOTA.getCommand() == U_FLASH)
              type = "sketch";
           else // U_SPIFFS
              type = "filesystem";

            // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
           Serial.println("Start updating " + type);
           })

        .onEnd([]() 
           {
           Serial.println("\nEnd");
           })

        .onProgress([](unsigned int progress, unsigned int total) 
           {
           Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
           })

        .onError([](ota_error_t error) 
           {
           Serial.printf("Error[%u]: ", error);
           if      (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
           else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
           else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
           else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
           else if (error == OTA_END_ERROR) Serial.println("End Failed");
           });

        ArduinoOTA.begin();

        Serial.println("Ready");
        Serial.print("IP address: ");
        Serial.println(WiFi.localIP());
     } // End of setup

  ArduinoOTA.handle();  
  vTaskDelay(50 / portTICK_PERIOD_MS);  // 50mS
  }  // End of the while loop
} // end of the task loop
