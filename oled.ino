void OLED_StartUpMessages() 

{
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
  //display.clear();
  //display.display();


}


void v_updateTheDisplay(void *parameters)  // The is the main display setting am to run once a second
{
  char  a_string[5];
  
  while (1) 
  {
    if (display_updated == false)
       {
       if ( ( air_quality_acquired == true ) && (temp_hum_acquired == true) ) // both data sets read to display
            {
            display.clear();

            // Set font for the larger characters
            display.setFont(ArialMT_Plain_10);  // was 10 or 16
            display.setTextAlignment(TEXT_ALIGN_LEFT);

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
            sprintf( a_string , "%02d", Hp1);
            display.drawString(0, 40,  "Toluene");   display.drawString (63, 40, a_string);  
        
            display.drawString(0, 50,  "AQI");
            display.display();

            display_updated = true;

            vTaskDelay(60000 / portTICK_PERIOD_MS);  // A long sleep ms. (60 seconds)
            }
       else
            {   // data for display not ready
            vTaskDelay(1000 / portTICK_PERIOD_MS);  // Set this to fixed delay for 1000ms, and try again
            } 
    
       
        } // end of the dat test loop and display
  }    // end while loop
}      // main function loop