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
  char  a_string[10];
  
  while (1) 
  {
    if (display_updated == false)
       {
       if ( ( air_quality_acquired == true ) && (temp_hum_acquired == true) ) // both data sets read to display
            {
            display.init();
            display.flipScreenVertically();
            display.clear();

            // Set font for the larger characters
            display.setFont(ArialMT_Plain_10);  // was 10 or 16
            display.setTextAlignment(TEXT_ALIGN_LEFT);

            
            sprintf( a_string , "%4.1f", temperature); 
            display.drawString(0, 0,   "Temp");          display.drawString (35, 0, a_string);    //display.drawString(90, 0, "C");
            
            sprintf( a_string , "%4.1f", humidity);
            display.drawString(0, 10,  "Humid");         display.drawString (35, 10, a_string);   //display.drawString(90, 10, "%RH");

            sprintf( a_string , "%2.0d", Aqi);
            display.drawString(0, 20,  "AQI");           display.drawString (30, 20, a_string);   //display.drawString(90, 20, "ppb");

            sprintf( a_string , "%5.0d", Tvoc);
            display.drawString(0, 30,  "TVOC");          display.drawString (23, 30, a_string); 

            sprintf( a_string , "%5.0lu", Co2);
            display.drawString(0, 40,  "eCO2");          display.drawString (30, 40, a_string);  
        
            
            sprintf( a_string , "%5.0lu", Hp0);
            display.drawString(62, 0,  "HP0");           display.drawString (90, 0, a_string);

            sprintf( a_string , "%5.0lu", Hp1);
            display.drawString(62, 10,  "HP1");           display.drawString (90, 10, a_string);

            sprintf( a_string , "%5.0lu", Hp2);
            display.drawString(62, 20,  "HP2");           display.drawString (90, 20, a_string);

            /*
            sprintf( a_string , "%5.0lu", Hp3);
            display.drawString(62, 30,  "HP3");           display.drawString (90, 30, a_string);
            */
            sprintf( a_string , "%3.1f", battery_voltage);
            display.drawString(62, 30,  "Vbatt");         display.drawString (90, 30, a_string);


            display.display();




            display_updated = true;

            //vTaskDelay(1000 / portTICK_PERIOD_MS);  // A long sleep ms. (6 seconds)
            }
       /*else
            {   // data for display not ready
            vTaskDelay(1000 / portTICK_PERIOD_MS);  // Set this to fixed delay for 1000ms, and try again
            } */
    
       
        } // end of the dat test loop and display
  vTaskDelay(1000 / portTICK_PERIOD_MS);  // A long sleep ms. (6 seconds)
  }    // end while loop
}      // main function loop