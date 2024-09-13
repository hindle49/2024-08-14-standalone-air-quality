
#define bar_left_x1     0
#define bar_middle_x2  64
#define bar_right_x3  128

#define vbatt_top_y    59
#define tvoc_top_y     42
#define co2_top_y      42

#define bar_height 5

#define vbatt_min 2000
#define vbatt_max 4500

#define tvoc_min 0
#define tvoc_max 300      //65000

#define co2_min 300
#define co2_max 2000       //65000


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


}


void v_updateTheDisplay(void *parameters)  // The is the main display setting am to run once a second
{
  char  a_string[10];
  int bar_length = 0; //use as a temp variable for calculating the bar length

  //battery_voltage = constrain (battery_voltage, vbatt_min, vbatt_max); // limit the bar graph range
  //Tvoc            = constrain (Tvoc           ,  tvoc_min, tvoc_max ); // limit the bar graph range 
  //Co2             = constrain (Co2            ,  co2_min,   co2_max ); // limit the bar graph rang
  
  while (1) 
  {
    if (display_updated == false)
       {
       if ( ( air_quality_acquired == true ) && (temp_hum_acquired == true) ) // both data sets read to display
            {
            display.init();
            display.flipScreenVertically();
            display.clear();

            // Set font for the smaller characters
            display.setFont(ArialMT_Plain_10);  // was 10 or 16
            display.setTextAlignment(TEXT_ALIGN_LEFT);

            
            sprintf( a_string , "%4.1f", temperature); 
            display.drawString(0, 0,   "Temp");          display.drawString (35, 0, a_string);    //display.drawString(90, 0, "C");
            
            sprintf( a_string , "%4.1f", humidity);
            display.drawString(0, 10,  "Humid");         display.drawString (35, 10, a_string);   //display.drawString(90, 10, "%");

            
            //sprintf( a_string , "%2.0d", Aqi);
            //display.drawString(0, 20,  "AQI");           display.drawString (30, 20, a_string);   //display.drawString(90, 20, "ppb");

            //sprintf( a_string , "%5.0d", Tvoc);
            //display.drawString(0, 30,  "TVOC");          display.drawString (23, 30, a_string); 

            //sprintf( a_string , "%5.0lu", Co2);
            //display.drawString(0, 40,  "eCO2");          display.drawString (30, 40, a_string);  
        

            sprintf( a_string , "%4.0d", battery_voltage);
            display.drawString(62, 0,  "Vbatt");         display.drawString (90, 0, a_string);

            //TVOC (left)
            bar_length = map (Tvoc, tvoc_min, tvoc_max, 0, 64); // bar_left_x1, bar_middle_x2);
            display.drawString(5, 31,  "TVOC ppb");
            display.drawRect(bar_left_x1, tvoc_top_y, 64,            bar_height);  // x, y, w, h
            display.fillRect(bar_left_x1, tvoc_top_y, bar_length,    bar_height); 

            //Co2 (right)     fix this one first
            bar_length = map (Co2, co2_min, co2_max, 0, 63); //bar_middle_x2, bar_right_x3);
            display.drawString(70, 31,  "eCO2 ppm");
            display.drawRect(bar_middle_x2 +1, co2_top_y, 63,          bar_height);  // x, y, w, h
            display.fillRect(bar_middle_x2 +1, co2_top_y, bar_length,  bar_height);
            debug("Bar length value = ");
            debugln(bar_length);

            // Battery Bar Graph (left)
            bar_length = map (battery_voltage, vbatt_min, vbatt_max, 0,64); //bar_left_x1, bar_middle_x2);
            display.drawString(20, 46,  "Vbatt");
            display.drawRect(bar_left_x1, vbatt_top_y, 64,          bar_height);  // x, y, w, h
            display.fillRect(bar_left_x1, vbatt_top_y, bar_length,  bar_height); 

            // AQI
            display.drawString(70, 46,  "AQI");
            if (Aqi == 1) display.drawString(70, 54,  "Excellent");
            if (Aqi == 2) display.drawString(70, 54,  "Good     ");
            if (Aqi == 3) display.drawString(70, 54,  "Moderate ");
            if (Aqi == 4) display.drawString(70, 54,  "Poor     ");
            if (Aqi == 5) display.drawString(70, 54,  "Unhealthy");

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