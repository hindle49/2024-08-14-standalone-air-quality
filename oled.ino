void OLED_StartUpMessages() {
  display.init();
  display.clear();
  display.display();
  display.setContrast(CONTRAST_HIGH, PRECHARGE_HIGH, COMDETECT_HIGH);  // appears to have no effect
  display.setBrightness(BRIGHTNESS_HIGH);                              //brightness appears to have no effect

  display.flipScreenVertically();

  display.clear();

  display.drawRect(0, 0, 128, 64);
  display.drawRect(1, 1, 126, 62);
  display.drawRect(2, 2, 124, 60);

  display.drawRect(6, 6, 116, 52);
  display.drawRect(7, 7, 114, 50);
  display.drawRect(8, 8, 112, 48);


  display.setFont(ArialMT_Plain_24);
  display.setTextAlignment(TEXT_ALIGN_CENTER);
  display.drawString(64, 20, "HINDLE");
  display.display();

  delay(5000);  // allow time for user to read the display

  // now repaeat the same process but with the sketch versio number

  display.clear();

  display.drawRect(0, 0, 128, 64);
  display.drawRect(1, 1, 126, 62);
  display.drawRect(2, 2, 124, 60);

  display.drawRect(6, 6, 116, 52);
  display.drawRect(7, 7, 114, 50);
  display.drawRect(8, 8, 112, 48);

  display.setFont(ArialMT_Plain_10);
  display.setTextAlignment(TEXT_ALIGN_CENTER);
  display.drawString(64, 10, SKETCH_NAME);
  display.drawString(64, 25, "Version");
  char version_string[4];
  itoa(VER, version_string, 10);
  display.drawString(64, 40, version_string);
  display.display();

  delay(2000);  // allow time for user to read the display
  display.clear();
  display.display();
}


void v_updateTheDisplay(void *parameters)  // The is the main display setting am to run once a second
{
  char  a_string[5];
  
  while (1) {
    static bool colon;  // This will be used to flash the time colon

    //getTimeFromRTC(); //Getet the time frm the RTC module

    display.clear();

    display.drawHorizontalLine(0, 52, 128);  // Horizontal line near the bottom
    
    // Set font for the larger characters
    display.setFont(ArialMT_Plain_10);  // was 10 or 16
    display.setTextAlignment(TEXT_ALIGN_LEFT);

    // When in OTA, show larger charters for the SSID progress
    if (OTA_Request)    // check if it's in OTA mode
      {
      display.drawString(0, 0, String(WIFI_SSID));
      display.drawString(0, 52, "OTA");
      }

    if (have_acquired_new_unix_time)
       {
       display.drawString(0, 30, "UINX time acquired");
       }
    if (WiFi_Connected)
       {
       display.drawString(0, 15, "Wifi Connected"); 
       }


   // time bottom right  
    display.drawString(97, 52, String(tm_hour));
    if (colon) display.drawString(111, 52, ":");
    colon = !colon;
    if (tm_min < 10)  // if the minutes are less than 10, the figure will need padding with an extra '0'
        display.drawString(116, 52, "0" + String(tm_min));
    else 
        display.drawString(116, 52, String(tm_min));
    
     // chnage to a smaller font
     // Set font for the larger characters
    display.setFont(ArialMT_Plain_10);  // was 10
    display.setTextAlignment(TEXT_ALIGN_LEFT);  

    if ( (!OTA_Request ) /*&& (system_mode == AUTO)  */)
        {
          if (system_mode == AUTO )
            {
            switch (day_state)
              {
              case NIGHT:           display.drawString(30, 52, "Night");         break;
              case EARLY_MORNING:   display.drawString(30, 52, "Early Morning"); break;
              case MORNING:         display.drawString(30, 52, "Morning");       break;
              case DAY:             display.drawString(30, 52, "Day");           break;
              case EARLY_EVENING:   display.drawString(30, 52, "Early Evening"); break;
              case EVENING:         display.drawString(30, 52, "Evening");       
              //default: 
              }
           }
          
        sprintf( a_string , "%4.1f C", tm_temperature);
        display.drawString(90, 30, a_string);
          
        sprintf( a_string , "%3d Lux", averageLux);
        display.drawString(85, 40, a_string);
        
        sprintf( a_string , "%02X", local.pattern);
        display.drawString(0, 0,   "Temperature");        display.drawString (63, 0, a_string);  
        sprintf( a_string , "%02X", local.step);
        display.drawString(0, 10,  "Humidity");      display.drawString (63, 10, a_string);  
        sprintf( a_string , "%02X", local.sync);
        display.drawString(0, 20,  "Ehanol");     display.drawString (63, 20, a_string);  
        sprintf( a_string , "%02X", local.brightness);
        display.drawString(0, 30,  "Ozone");     display.drawString (63, 30, a_string);  
        sprintf( a_string , "%02d", display_time_varable);
        display.drawString(0, 40,  "Toluene");   display.drawString (63, 40, a_string);  
        
        display.drawString(0, 50,  "AQI");
        }


    // Needs more lines to show the pattern, step, sync and britness.
    // Maybe show time to the next pattern (in seconds)

    display.display();
    vTaskDelay(1000 / portTICK_PERIOD_MS);  // Set this to fixed delay for 1000ms, that the display is updated once a second
  }
}